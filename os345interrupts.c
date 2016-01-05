// os345interrupts.c - pollInterrupts	08/08/2013
// ***********************************************************************
// **   DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER ** DISCLAMER   **
// **                                                                   **
// ** The code given here is the basis for the BYU CS345 projects.      **
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
#include <time.h>
#include <assert.h>
//#include <sys/queue.h>

#include "os345.h"
#include "os345config.h"
#include "os345signals.h"

#define BACKSPACE  127
#define CNTRL_R 0x12
#define CNTRL_W 0x17
#define CNTRL_X 0x18
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define ESCP1 27
#define ESCP2 91

// **********************************************************************
//	local prototypes
//
void pollInterrupts(void);
static void keyboard_isr(void);
static void timer_isr(void);

// **********************************************************************
// **********************************************************************
// global semaphores

extern Semaphore* keyboard;				// keyboard semaphore
extern Semaphore* charReady;				// character has been entered
extern Semaphore* inBufferReady;			// input buffer ready semaphore

extern Semaphore* tics1sec;				// 1 second semaphore
extern Semaphore* tics10thsec;				// 1/10 second semaphore
extern Semaphore* tics10sec; // 10 second semaphore

extern char inChar;				// last entered character
extern int charFlag;				// 0 => buffered input
extern int inBufIndx;				// input pointer into input buffer
extern int cursor;
extern int escape_mode;
extern char inBuffer[INBUF_SIZE+1];	// character input buffer

extern time_t oldTime10; // old 10sec time 
extern time_t oldTime1;					// old 1sec time
extern clock_t myClkTime;
extern clock_t myOldClkTime;

extern int pollClock;				// current clock()
extern int lastPollClock;			// last pollClock

extern int superMode;						// system mode

extern TCB tcb[];             // task control block

extern char* commandBuf[COMMAND_BUF_LEN];
extern int curr_command;
extern int last_command;

extern PQueue rq;
extern DC_EVENT** dClock;
extern Semaphore* DCMutex;

extern int tics10thsec_dec;

// **********************************************************************
// **********************************************************************
// simulate asynchronous interrupts by polling events during idle loop
//
void pollInterrupts(void)
{
	// check for task monopoly
	pollClock = clock();
	assert("Timeout" && ((pollClock - lastPollClock) < MAX_CYCLES));
	lastPollClock = pollClock;

	// check for keyboard interrupt
	if ((inChar = GET_CHAR) > 0)
	{
	  keyboard_isr();
	}

	// timer interrupt
	timer_isr();

	return;
} // end pollInterrupts


// **********************************************************************
// keyboard interrupt service routine
//

void default_chr();

void default_chr()
{
  int j;
  escape_mode = 0;
  if (cursor == inBufIndx)
  {
    cursor++;
    inBuffer[inBufIndx++] = inChar;
    inBuffer[inBufIndx] = 0;
    printf("%c", inChar);   // echo character
  }
  else
  {          
    inBufIndx++;
    inBuffer[inBufIndx] = 0;
    for (j = inBufIndx - 1; j > cursor; j--)
    {
      inBuffer[j] = inBuffer[j-1];
    }
    inBuffer[cursor] = inChar;
    //printf("\n%s",inBuffer);          
    for (j = 0; j < cursor; j++) printf("\b");
    printf("%s",inBuffer);
    for (j = 0; j < inBufIndx - cursor-1; j++) printf("\b");
    cursor++;
  }
}

static void keyboard_isr()
{
  int j;
	// assert system mode
	assert("keyboard_isr Error" && superMode);

	semSignal(charReady);					// SIGNAL(charReady) (No Swap)
	if (charFlag == 0)
	{
   // printf("%d");
		switch (inChar)
		{
      case BACKSPACE:
        if(!inBufIndx||!cursor) break;
        if (inBufIndx == cursor)
        {
          inBufIndx--;
          inBuffer[inBufIndx] = 0;
          cursor--;
          printf("\b \b");
          break;
        }
        else
        {
          for (j = cursor; j < inBufIndx; j++)
          {
            inBuffer[j-1] = inBuffer[j];
          }
          inBufIndx--;
          inBuffer[inBufIndx] = 0;
          for (j = 0; j < cursor; j++) printf("\b");
          printf("%s ",inBuffer);
          cursor--;          
          for (j = 0; j < inBufIndx - cursor+1; j++) printf("\b");          
          break;
        }
			case '\r':
			case '\n':
			{
				cursor = inBufIndx = 0;				// EOL, signal line ready
				semSignal(inBufferReady);	// SIGNAL(inBufferReady)
				break;
			}

      case CNTRL_R: // ^r
      {
        int taskId;
        sigSignal(-1, mySIGCONT);
        for (taskId = 0; taskId < MAX_TASKS; taskId++)
        {
          tcb[taskId].signal &= ~mySIGSTOP;
          tcb[taskId].signal &= ~mySIGTSTP;
        }
        break;
      }
			
      case CNTRL_W: // ^w
      {
        sigSignal(-1, mySIGTSTP);
        break;
      }
      
			case CNTRL_X:						// ^x
			{
				cursor = inBufIndx = 0;
				inBuffer[0] = 0;
				sigSignal(0, mySIGINT);		// interrupt task 0
				semSignal(inBufferReady);	// SEM_SIGNAL(inBufferReady)
				break;
			}
			
      case UP: // up arrow
      {
        if (!escape_mode)
        {
          default_chr();
          break;
        }
        if (!commandBuf[curr_command]) commandBuf[curr_command] = strdup(inBuffer);
        else memcpy(commandBuf[curr_command],inBuffer,INBUF_SIZE+1);
        curr_command = PREV_COM(curr_command);
        if (!commandBuf[curr_command])
        {
          curr_command = NEXT_COM(curr_command);
          break;
        }

        for (j = 0; j < inBufIndx; j++)
        {
          printf("\b \b");
        }
        
        memcpy(inBuffer,commandBuf[curr_command],INBUF_SIZE+1);
        cursor = inBufIndx = strlen(inBuffer);        
        printf("%s",inBuffer);
        break;
      }

      case DOWN: // down arrow
      {
        if (!escape_mode)
        {
          default_chr();
          break;
        }
        
        if (!commandBuf[curr_command]) commandBuf[curr_command] = strdup(inBuffer);
        else memcpy(commandBuf[curr_command],inBuffer,inBufIndx+1);
        curr_command = NEXT_COM(curr_command);
        if (!commandBuf[curr_command])
        {
          curr_command = PREV_COM(curr_command);
          break;
        }        
        for (j = 0; j < inBufIndx; j++)
        {
          printf("\b \b");
        }
        
        memcpy(inBuffer,commandBuf[curr_command],INBUF_SIZE+1);
        cursor = inBufIndx = strlen(inBuffer);
        printf("%s",inBuffer);        
        break;
      }

      case RIGHT:
      {
        if (!escape_mode)
        {
          default_chr();
          break;
        }

        if (cursor == inBufIndx) break;
        else
        {
          cursor++;
          printf("%c%c%c",ESCP1,ESCP2,RIGHT);
        }
        break;
      }
      
      case LEFT:
      {
        if (!escape_mode)
        {
          default_chr();
          break;
        }

        if (!cursor) break;
        cursor--;
        printf("%c%c%c",ESCP1,ESCP2,LEFT);
        break;        
      }
      
      case ESCP1:
      case ESCP2:
        escape_mode = 1;
        break;
      
			default:
			{
        default_chr();
			}
		}
	}
	else
	{
		// single character mode
		inBufIndx = 0;
		inBuffer[inBufIndx] = 0;
	}
	return;
} // end keyboard_isr


// **********************************************************************
// timer interrupt service routine
//
static void timer_isr()
{
	time_t currentTime;						// current time

	// assert system mode
	assert("timer_isr Error" && superMode);

	// capture current time
  	time(&currentTime);

  	// one second timer
  	if ((currentTime - oldTime1) >= 1)
  	{
		// signal 1 second
  	   semSignal(tics1sec);
		oldTime1 += 1;
  	}

  	// ten second timer
  	if ((currentTime - oldTime10) >= 10)
    {
      semSignal(tics10sec);
      oldTime10 += 10;
    }
  	
	// sample fine clock
	myClkTime = clock();
	if ((myClkTime - myOldClkTime) >= ONE_TENTH_SEC)
	{
    if (semTryLock(DCMutex))
    {
      if(dClock[0]->delta && dClock[dClock[0]->delta]->delta-- == 0)
      {
        DC_EVENT* next = popDeltaClock(dClock);
        SEM_SIGNAL(next->event);
        free(next);
      }
      
      //make up for lost time and handle simultaneous events
      while (dClock[0]->delta && dClock[dClock[0]->delta]->delta <= tics10thsec_dec)
      {
        tics10thsec_dec -= dClock[dClock[0]->delta]->delta; //decrement lost time
        DC_EVENT* next = popDeltaClock(dClock);
        SEM_SIGNAL(next->event);
        free(next);        
      }
      
      tics10thsec_dec = 0; //set lost time to zero
      SEM_SIGNAL(DCMutex);
    }
    
    else tics10thsec_dec++; //increment lost time
    
		myOldClkTime = myOldClkTime + ONE_TENTH_SEC;   // update old
		semSignal(tics10thsec);
	}

	
	// ?? add other timer sampling/signaling code here for project 2

	return;
} // end timer_isr
