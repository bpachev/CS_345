// os345mmu.c - LC-3 Memory Management Unit
// **************************************************************************
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// **                                                                   **
// ** The code given here is the basis for the CS345 projects.          **
// ** It comes "as is" and "unwarranted."  As such, when you use part   **
// ** or all of the code, it becomes "yours" and you are responsible to **
// ** understand any algorithm or method presented.  Likewise, any      **
// ** errors or problems become your responsibility to fix.             **
// **                                                                   **
// ** NOTES:                                                            **
// ** -Comments beginning with "// ??" may require some implementation. **
// ** -Tab stops are set at every 3 spaces.                             **
// ** -The function API's in "OS345.h" should not be altered.           **
// **                                                                   **
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// ***********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <assert.h>
#include "os345.h"
#include "os345lc3.h"

// ***********************************************************************
// mmu variables

#define ADVANCE_BIG_HAND     bigClockHand = (bigClockHand + 2) % (LC3_RPT_END - LC3_RPT);


// LC-3 memory
unsigned short int memory[LC3_MAX_MEMORY];

// statistics
int memAccess;						// memory accesses
int memHits;						// memory hits
int memPageFaults;					// memory faults
int nextPage;						// swap page size
int pageReads;						// page reads
int pageWrites;						// page writes
extern int bigClockHand; //offset of current RPT entry 
extern int littleClockHand; //offset within current UPT
extern TCB tcb[];
extern int curTask;

int getFrame(int);
int getAvailableFrame(void);

int advanceClock();
void dump_rpt_and_upt();

void dump_rpt_and_upt()
{
  int rpt, upt;
  for (rpt = 0; rpt < 64; rpt+=2)
  {
    if (MEMWORD(rpt+TASK_RPT) || MEMWORD(rpt+TASK_RPT+1))
    {
      outPTE("  RPT  =", rpt+TASK_RPT);
      for(upt = 0; upt < 64; upt+=2)
      {
        if (DEFINED(MEMWORD(rpt+TASK_RPT)) &&(DEFINED(MEMWORD((FRAME(MEMWORD(rpt+TASK_RPT))<<6)+upt))
                          || PAGED(MEMWORD((FRAME(MEMWORD(rpt+TASK_RPT))<<6)+upt+1))))
        {
          outPTE("    UPT=", (FRAME(MEMWORD(rpt+TASK_RPT))<<6)+upt);
        }
      }
    }
  }
}


int advanceClock()
{
/*  //advance little hand
  if (DEFINED(MEMWORD(LC3_RPT+bigClockHand)))
  {
    littleClockHand += 2; // increment once to get off current valid frame
    while (littleClockHand < 64 && 
      !DEFINED(MEMWORD((FRAME(MEMWORD(LC3_RPT+bigClockHand))<<6) + littleClockHand)) )
    {
      littleClockHand += 2;
    }
    
    if (littleClockHand < 64) return 0;
    else littleClockHand = 0;
  }
  
  bigClockHand = (bigClockHand+2) % (LC3_RPT_END - LC3_RPT);
  while (!DEFINED(MEMWORD(LC3_RPT + bigClockHand)))
  {
    bigClockHand = (bigClockHand+2) % (LC3_RPT_END - LC3_RPT);    
  } */
  littleClockHand += 2;
  if (littleClockHand >= 64)
  {
    littleClockHand = 0;
    ADVANCE_BIG_HAND
  }

  return 0;
}


int getFrame(int notme)
{
	int i, frame, rpt;
//  int adr = LC3_FBT - 1;        // index to frame bit table
 // int fmask = 0x0001;         // bit mask  
	frame = getAvailableFrame();
  //printf("\n%d getAvailableFrame returned -- Yoda \n",frame);
	if (frame >=0) return frame;
  
	//printf("We're toast!!!!!!!!!!!! notme = %d\n",notme);

  int adr, curr_entry, curr_entry2, curr_frame,upt1,upt2,adr_u;
  
  for (i = 0; i < 100000; i++)
  {
    int is_user_table = 0;
    adr = (FRAME(MEMWORD(LC3_RPT+bigClockHand))<<6) + littleClockHand;
    curr_entry = MEMWORD(adr);
    curr_entry2 = MEMWORD(adr + 1);
    curr_frame = FRAME(curr_entry);
    adr_u = LC3_RPT+bigClockHand;
    upt1 = MEMWORD(LC3_RPT+bigClockHand);
    upt2 = MEMWORD(LC3_RPT+bigClockHand + 1);

    if (!DEFINED(upt1))
    {
      ADVANCE_BIG_HAND
      continue;
    }
    
    //check if user page table has children
    if (littleClockHand == 0 && DEFINED(upt1))
    {
      int has_children = 0;
      int j;
      
      for (j = 0; j < 64; j+=2)
      {
        if (DEFINED(MEMWORD((FRAME(MEMWORD(LC3_RPT+bigClockHand))<<6) + j)))
        {
          has_children = 1;
          break;
        }
      }
      //if no children, treat like data frame
      if (!has_children)
      {
        adr = adr_u;
        curr_entry = upt1;
        curr_entry2 = upt2;
        curr_frame = FRAME(curr_entry);
        is_user_table = 1;        
      }
      
      if(REFERENCED(upt1)&&has_children)
      {        
        MEMWORD(adr_u) = CLEAR_REF(upt1);
      }
      
    }
      
    if (DEFINED(curr_entry))
    {
//      printf("\%04x %d, %d, curr_frame = $%d\n",curr_entry,bigClockHand,littleClockHand,curr_frame);              
      if (REFERENCED(curr_entry))
      {
        MEMWORD(adr) = CLEAR_REF(curr_entry);
      }
      
      else if (curr_frame != notme)
      {
 //       printf("\n%04x\n",curr_frame);
        //dump rpt and upt
        //dump_rpt_and_upt();

        if (PAGED(curr_entry2))
        {
          if(is_user_table || DIRTY(curr_entry)) accessPage(SWAPPAGE(curr_entry2), curr_frame, PAGE_OLD_WRITE);
        }
        
        else
        {
          curr_entry2 |= accessPage(SWAPPAGE(curr_entry2), curr_frame, PAGE_NEW_WRITE);
          curr_entry2 = SET_PAGED(curr_entry2);
        }
        curr_entry = CLEAR_DEFINED(curr_entry);
        curr_entry = CLEAR_DIRTY(curr_entry);
        MEMWORD(adr) = curr_entry;
        MEMWORD(adr+1) = curr_entry2;
        if (!is_user_table) advanceClock(); 
        else  ADVANCE_BIG_HAND
       /* if(curr_frame<192||curr_frame >= 218)
        {
          printf("Returning frame %d, bc %d, lc %d, curr_entry: %04x, \n upt1 = %04x is_user_table=%d\n",curr_frame,bigClockHand,littleClockHand,curr_entry,upt1, is_user_table);
          exit(1);
        }
        else
        {
          printf("Returning frame %d, bc %d, lc %d, \ncurr_entry: %04x, is_user_table=%d",curr_frame,bigClockHand,littleClockHand,curr_entry,is_user_table);
        }*/
        return curr_frame;
        break;
      }
    }
    advanceClock();
  }
  
	return frame;
}
// **************************************************************************
// **************************************************************************
// LC3 Memory Management Unit
// Virtual Memory Process
// **************************************************************************
//           ___________________________________Frame defined
//          / __________________________________Dirty frame
//         / / _________________________________Referenced frame
//        / / / ________________________________Pinned in memory
//       / / / /     ___________________________
//      / / / /     /                 __________frame # (0-1023) (2^10)
//     / / / /     /                 / _________page defined
//    / / / /     /                 / /       __page # (0-4096) (2^12)
//   / / / /     /                 / /       /
//  / / / /     / 	             / /       /
// F D R P - - f f|f f f f f f f f|S - - - p p p p|p p p p p p p p

#define MMU_ENABLE	1

unsigned short int *getMemAdr(int va, int rwFlg)
{
	unsigned short int pa;
	int rpta, rpte1, rpte2;
	int upta, upte1, upte2;
	int rptFrame, uptFrame;

	rpta = TASK_RPT + RPTI(va);
	rpte1 = memory[rpta];
	rpte2 = memory[rpta+1];

	// turn off virtual addressing for system RAM
	if (va < 0x3000) return &memory[va];
#if MMU_ENABLE
	if (DEFINED(rpte1))
	{
          memHits++;
		// defined
	}
	else
	{
                memPageFaults++;
		// fault
		rptFrame = getFrame(-1);
		rpte1 = SET_DEFINED(rptFrame);
		if (PAGED(rpte2))
		{
			accessPage(SWAPPAGE(rpte2), rptFrame, PAGE_READ);
		}
		else
		{
			memset(&memory[(rptFrame<<6)], 0, 128);
		}
	}


	memory[rpta] = rpte1 = SET_REF(rpte1);
	memory[rpta+1] = rpte2;

	upta = (FRAME(rpte1)<<6) + UPTI(va);
	upte1 = memory[upta];
	upte2 = memory[upta+1];

	if (DEFINED(upte1))
	{
          memHits++;
		// defined
	}
	else
	{
		memPageFaults++;
		// fault
		uptFrame = getFrame(FRAME(memory[rpta]));
		upte1 = SET_DEFINED(uptFrame);
		if (PAGED(upte2))
		{
			accessPage(SWAPPAGE(upte2), uptFrame, PAGE_READ);
		}
		else
		{
		}
	}

	memory[upta] = SET_REF(upte1);
	memory[upta+1] = upte2;
        if (rwFlg)
        {
          memory[upta] = SET_DIRTY(memory[upta]);
        }

  memory[rpta] = SET_REF(memory[rpta]);

        memAccess += 2; //one for the user table and one for the data frame
	return &memory[(FRAME(upte1)<<6) + FRAMEOFFSET(va)];
#else
	return &memory[va];
#endif
} // end getMemAdr


// **************************************************************************
// **************************************************************************
// set frames available from sf to ef
//    flg = 0 -> clear all others
//        = 1 -> just add bits
//
void setFrameTableBits(int flg, int sf, int ef)
{	int i, data;
	int adr = LC3_FBT-1;             // index to frame bit table
	int fmask = 0x0001;              // bit mask

	// 1024 frames in LC-3 memory
	for (i=0; i<LC3_FRAMES; i++)
	{	if (fmask & 0x0001)
		{  fmask = 0x8000;
			adr++;
			data = (flg)?MEMWORD(adr):0;
		}
		else fmask = fmask >> 1;
		// allocate frame if in range
		if ( (i >= sf) && (i < ef)) data = data | fmask;
		MEMWORD(adr) = data;
	}
	return;
} // end setFrameTableBits


// **************************************************************************
// get frame from frame bit table (else return -1)
int getAvailableFrame()
{
  //printf("\nGet Available Frame Called");
	int i, data;
	int adr = LC3_FBT - 1;				// index to frame bit table
	int fmask = 0x0001;					// bit mask

	for (i=0; i<LC3_FRAMES; i++)		// look thru all frames
	{	if (fmask & 0x0001)
		{  fmask = 0x8000;				// move to next work (maybe word?)
			adr++;
			data = MEMWORD(adr);
 //     if (data) printf("\n%04x\n",data);            
		}
		else fmask = fmask >> 1;		// next frame
		// deallocate frame and return frame #
		if (data & fmask)
		{  MEMWORD(adr) = data & ~fmask;
			return i;
		}
	}
//	printf("\nGet Available Frame REturned");
	return -1;
} // end getAvailableFrame



// **************************************************************************
// read/write to swap space
int accessPage(int pnum, int frame, int rwnFlg)
{
   static unsigned short int swapMemory[LC3_MAX_SWAP_MEMORY];

   if ((nextPage >= LC3_MAX_PAGE) || (pnum >= LC3_MAX_PAGE))
   {
      printf("\nVirtual Memory Space Exceeded!  (%d)", LC3_MAX_PAGE);
      exit(-4);
   }
   switch(rwnFlg)
   {
      case PAGE_INIT:                    		// init paging
         nextPage = 0;
         return 0;

      case PAGE_GET_ADR:                    	// return page address
         return (int)(&swapMemory[pnum<<6]);

      case PAGE_NEW_WRITE:                   // new write (Drops thru to write old)
         pnum = nextPage++;

      case PAGE_OLD_WRITE:                   // write
         //printf("\n    (%d) Write frame %d (memory[%04x]) to page %d", p.PID, frame, frame<<6, pnum);
         memcpy(&swapMemory[pnum<<6], &memory[frame<<6], 1<<7);
         pageWrites++;
         return pnum;

      case PAGE_READ:                    // read
         //printf("\n    (%d) Read page %d into frame %d (memory[%04x])", p.PID, pnum, frame, frame<<6);
      	memcpy(&memory[frame<<6], &swapMemory[pnum<<6], 1<<7);
         pageReads++;
         return pnum;
	
      case PAGE_FREE:                   // free page
         break;
   }
   return pnum;
} // end accessPage
