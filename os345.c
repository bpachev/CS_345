// os345.c - OS Kernel	09/12/2013
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

#include "os345.h"
#include "os345signals.h"
#include "os345config.h"
#include "os345lc3.h"
#include "os345fat.h"

// **********************************************************************
//	local prototypes
//
void pollInterrupts(void);
static int scheduler(void);
static int dispatcher(void);

//static void keyboard_isr(void);
//static void timer_isr(void);

int sysKillTask(int taskId);
static int initOS(void);

// **********************************************************************
// **********************************************************************
// global semaphores

Semaphore* semaphoreList;			// linked list of active semaphores
Semaphore* DCMutex;  // delta clock mutex
Semaphore* helloSem; //hello task semaphore

Semaphore* keyboard;				// keyboard semaphore
Semaphore* charReady;				// character has been entered
Semaphore* inBufferReady;			// input buffer ready semaphore

Semaphore* tics10sec;       // 10 second semaphore
Semaphore* tics1sec;				// 1 second semaphore
Semaphore* tics10thsec;				// 1/10 second semaphore

// **********************************************************************
// **********************************************************************
// global system variables

TCB tcb[MAX_TASKS];					// task control block
Semaphore* taskSems[MAX_TASKS];		// task semaphore
jmp_buf k_context;					// context of kernel stack
jmp_buf reset_context;				// context of kernel stack
volatile void* temp;				// temp pointer used in dispatcher

int scheduler_mode;					// scheduler mode
int superMode;						// system mode
int curTask;						// current task #
long swapCount;						// number of re-schedule cycles
char inChar;						// last entered character
int charFlag;						// 0 => buffered input
int inBufIndx;						// input pointer into input buffer
int cursor; // position of cursor in buffer (used for editing)
char inBuffer[INBUF_SIZE+1];		// character input buffer
char* commandBuf[COMMAND_BUF_LEN];
int curr_command;
int last_command;
int tics10thsec_dec;

int bigClockHand; //offset of current RPT entry pointed to by p4 clock
int littleClockHand; //offset of current UPT entry
int escape_mode;

//Message messages[NUM_MESSAGES];		// process message buffers

int pollClock;						// current clock()
int lastPollClock;					// last pollClock
bool diskMounted;					// disk has been mounted

time_t oldTime1;					// old 1sec time
time_t oldTime10;          // old 10sec time
clock_t myClkTime;
clock_t myOldClkTime;


int* rq;							// ready priority queue
DC_EVENT** dClock; // delta clock;

//p6 variables
//open file table allocation mask
int OFTalloc[NFILES];

int fair_sched_sel();

// **********************************************************************
// **********************************************************************
// OS startup
//
// 1. Init OS
// 2. Define reset longjmp vector
// 3. Define global system semaphores
// 4. Create CLI task
// 5. Enter scheduling/idle loop
//
int main(int argc, char* argv[])
{
	// save context for restart (a system reset would return here...)
	int resetCode = setjmp(reset_context);
	superMode = TRUE;						// supervisor mode

	switch (resetCode)
	{
		case POWER_DOWN_QUIT:				// quit
			powerDown(0);
			printf("\nGoodbye!!");
			return 0;

		case POWER_DOWN_RESTART:			// restart
			powerDown(resetCode);
			printf("\nRestarting system...\n");

		case POWER_UP:						// startup
			break;

		default:
			printf("\nShutting down due to error %d", resetCode);
			powerDown(resetCode);
			return resetCode;
	}

	// output header message
	printf("%s", STARTUP_MSG);

	// initalize OS
	if ( resetCode = initOS()) return resetCode;

	// create global/system semaphores here
	//?? vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	charReady = createSemaphore("charReady", BINARY, 0);
	inBufferReady = createSemaphore("inBufferReady", BINARY, 0);
	keyboard = createSemaphore("keyboard", BINARY, 1);
  tics10sec = createSemaphore("tics10sec",COUNTING,0);
	tics1sec = createSemaphore("tics1sec", BINARY, 0);
	tics10thsec = createSemaphore("tics10thsec", BINARY, 0);
  DCMutex = createSemaphore("DCMutex", BINARY, 1);
  helloSem = createSemaphore("helloSem", BINARY, 1);

  
	//?? ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// schedule CLI task
	createTask("myShell",			// task name
					P1_shellTask,	// task
					MED_PRIORITY,	// task priority
					argc,			// task arg count
					argv);			// task argument pointers

	// HERE WE GO................

	// Scheduling loop
	// 1. Check for asynchronous events (character inputs, timers, etc.)
	// 2. Choose a ready task to schedule
	// 3. Dispatch task
	// 4. Loop (forever!)

	while(1)									// scheduling loop
	{
		// check for character / timer interrupts
		pollInterrupts();

		// schedule highest priority ready task
		if ((curTask = scheduler()) < 0) continue;

		// dispatch curTask, quit OS if negative return
		if (dispatcher() < 0) break;
	}											// end of scheduling loop

	// exit os
	longjmp(reset_context, POWER_DOWN_QUIT);
	return 0;
} // end main


// Selects next task to be executed according to a fair scheduling policy.
// If all time allotments are 0, returns -1.
int fair_sched_sel()
{
  int i;
  int task = -1;
  for (i = 1; i <= rq[0]; i++)
  {
    int tID = Q_TID(rq,i);    
    if (tcb[tID].time)
    {
      //decrement time remaining and schedule task.
      tcb[tID].time--;
      task = tID;
     // printf("task %d, time %d",task,tcb[task].time);
      return task;
    }
  }
  return task;
}


// **********************************************************************
// **********************************************************************
// scheduler
//
static int scheduler()
{
	int nextTask, i;
	// ?? Design and implement a scheduler that will select the next highest
	// ?? priority ready task to pass to the system dispatcher.

	// ?? WARNING: You must NEVER call swapTask() from within this function
	// ?? or any function that it calls.  This is because swapping is
	// ?? handled entirely in the swapTask function, which, in turn, may
	// ?? call this function.  (ie. You would create an infinite loop.)

	// ?? Implement a round-robin, preemptive, prioritized scheduler.

	// ?? This code is simply a round-robin scheduler and is just to get
	// ?? you thinking about scheduling.  You must implement code to handle
	// ?? priorities, clean up dead tasks, and handle semaphores appropriately.

	// schedule next task
  if (!scheduler_mode)
  {
  	if ((nextTask = DeQ(rq,-1)) >= 0)
    {
      enQ(rq, nextTask,tcb[nextTask].priority);
    }
    else return -1;
  }
  
  else //fair scheduler
  {
    if (!rq[0]) return -1;
    
    nextTask = fair_sched_sel();
    
    if (nextTask < 0)
    {
      int tid;
      int max_children = 0;
      int max_id = -1;
      int num_ready_children[MAX_TASKS];
      memset(num_ready_children, 0, MAX_TASKS*sizeof(int));
      
      for (i = 1; i <= rq[0]; i++)
      {
        tid = Q_TID(rq,i);
        //printf("tid: %d",tid);
        if (tid) // don't want myshell to count as its own child :)
        {
          num_ready_children[tcb[tid].parent]++;
          if (num_ready_children[tcb[tid].parent] > max_children && tcb[tid].parent)
          {
            max_children++; //if max_chilren is exceeded, it will be only by 1.
            max_id = tcb[tid].parent;
          }
        }
        
      }
      
//      printf("MAX_id: %d",max_id);
      
      for (i = 1; i <= rq[0]; i++)
      {
        tid = Q_TID(rq,i);
        if (tid) // again, my_shell is special (because its parent could be un-initialized
        {
          int parent = tcb[tid].parent;
          tcb[tid].time = (max_children + 1) / (num_ready_children[parent] + 1);
        //  printf("id %d, time %d, parent %d\n",tid, tcb[tid].time, parent);
          if(!parent) //child of shell
          {
            //give parent the left-overs
            tcb[tid].time =  (max_children + 1) % (num_ready_children[tid] + 1) +
            (max_children + 1) / (num_ready_children[tid] + 1);
          }
        }
        
        else
        {
          tcb[tid].time = (max_children + 1) / (num_ready_children[tid] + 1) +
          (max_children + 1) % (num_ready_children[tid] + 1); 
        }
      //  printf("tid: %d, time %d\n",tid, tcb[tid].time);
      }
      
      
      
      nextTask = fair_sched_sel();
      //printf("nextTask %d\n",nextTask);
//      exit(1);
    }
        
  }
  
  
	// mask sure nextTask is valid
	while (!tcb[nextTask].name)
	{
		if (++nextTask >= MAX_TASKS) nextTask = 0;
	}
  if (signals()) return -1;

	if (tcb[nextTask].signal & mySIGSTOP) return -1;
  
	return nextTask;
} // end scheduler



// **********************************************************************
// **********************************************************************
// dispatch curTask
//
static int dispatcher()
{
	int result;

	// schedule task
	switch(tcb[curTask].state)
	{
		case S_NEW:
		{
			// new task
			printf("\nNew Task[%d] %s", curTask, tcb[curTask].name);
			tcb[curTask].state = S_RUNNING;	// set task to run state

			// save kernel context for task SWAP's
			if (setjmp(k_context))
			{
				superMode = TRUE;					// supervisor mode
				break;								// context switch to next task
			}

			// move to new task stack (leave room for return value/address)
			temp = (int*)tcb[curTask].stack + (STACK_SIZE-8);
			SET_STACK(temp);
			superMode = FALSE;						// user mode

			// begin execution of new task, pass argc, argv
			result = (*tcb[curTask].task)(tcb[curTask].argc, tcb[curTask].argv);

			// task has completed
			if (result) printf("\nTask[%d] returned %d", curTask, result);
			else printf("\nTask[%d] returned %d", curTask, result);
			tcb[curTask].state = S_EXIT;			// set task to exit state

			// return to kernal mode
			longjmp(k_context, 1);					// return to kernel
		}

		case S_READY:
		{
			tcb[curTask].state = S_RUNNING;			// set task to run
		}

		case S_RUNNING:
		{
			if (setjmp(k_context))
			{
				// SWAP executed in task
				superMode = TRUE;					// supervisor mode
				break;								// return from task
			}
			if (signals()) break;
			longjmp(tcb[curTask].context, 3); 		// restore task context
		}

		case S_BLOCKED:
		{
			break;
		}

		case S_EXIT:
		{
			if (curTask == 0) return -1;			// if CLI, then quit scheduler
			// release resources and kill task
			sysKillTask(curTask);					// kill current task
			break;
		}

		default:
		{
			printf("Unknown Task[%d] State", curTask);
			longjmp(reset_context, POWER_DOWN_ERROR);
		}
	}
	return 0;
} // end dispatcher



// **********************************************************************
// **********************************************************************
// Do a context switch to next task.

// 1. If scheduling task, return (setjmp returns non-zero value)
// 2. Else, save current task context (setjmp returns zero value)
// 3. Set current task state to READY
// 4. Enter kernel mode (longjmp to k_context)

void swapTask()
{
	assert("SWAP Error" && !superMode);		// assert user mode

	// increment swap cycle counter
	swapCount++;

	// either save current task context or schedule task (return)
	if (setjmp(tcb[curTask].context))
	{
		superMode = FALSE;					// user mode
		return;
	}

	// context switch - move task state to ready
	if (tcb[curTask].state == S_RUNNING) tcb[curTask].state = S_READY;

	// move to kernel mode (reschedule)
	longjmp(k_context, 2);
} // end swapTask



// **********************************************************************
// **********************************************************************
// system utility functions
// **********************************************************************
// **********************************************************************

// **********************************************************************
// **********************************************************************
// initialize operating system
static int initOS()
{
	int i;

	// make any system adjustments (for unblocking keyboard inputs)
	INIT_OS

	// reset system variables
	curTask = 0;						// current task #
	swapCount = 0;						// number of scheduler cycles
	scheduler_mode = 0;					// default scheduler
	inChar = 0;							// last entered character
	charFlag = 0;						// 0 => buffered input
	inBufIndx = 0;						// input pointer into input buffer
	cursor = 0;
	semaphoreList = 0;					// linked list of active semaphores
	diskMounted = 0;					// disk has been mounted
  curr_command = 0;
  last_command = 0;
  tics10thsec_dec = 0;
  bigClockHand = 0;
  littleClockHand = 0;
  escape_mode = 0; //not in escape mode
  
  for (i = 0; i < COMMAND_BUF_LEN; i++) commandBuf[i] = 0;
	
	// malloc ready queue
	rq = (int*)malloc(MAX_TASKS * sizeof(int));
	if (rq == NULL) return 99;

  // malloc delta clock
  dClock = (DC_EVENT**)malloc((MAX_DC+1)*sizeof(DC_EVENT*));
  if (!dClock) printf("Delta clock malloc failed\n");
  dClock[0] = (DC_EVENT*)malloc(sizeof(DC_EVENT));
  dClock[0]->delta = 0;
  dClock[0]->event = 0;
  
  //initialize p6 variables
  memset(OFTalloc,0, NFILES*sizeof(int)); 
  
  
	// capture current time
	lastPollClock = clock();			// last pollClock
	time(&oldTime1);
  time(&oldTime10);

	// init system tcb's
	for (i=0; i<MAX_TASKS; i++)
	{
		tcb[i].name = NULL;				// tcb
		taskSems[i] = NULL;				// task semaphore
	}

	// init tcb
	for (i=0; i<MAX_TASKS; i++)
	{
		tcb[i].name = NULL;
	}

	// initialize lc-3 memory
	initLC3Memory(LC3_MEM_FRAME, 0xF800>>6);

	// ?? initialize all execution queues

	return 0;
} // end initOS



// **********************************************************************
// **********************************************************************
// Causes the system to shut down. Use this for critical errors
void powerDown(int code)
{
	int i;
	printf("\nPowerDown Code %d", code);

	// release all system resources.
	printf("\nRecovering Task Resources...");

	// kill all tasks
	for (i = MAX_TASKS-1; i >= 0; i--)
		if(tcb[i].name) sysKillTask(i);

	// delete all semaphores
	while (semaphoreList)
		deleteSemaphore(&semaphoreList);

	// free ready queue
	free(rq);

	// ?? release any other system resources
	// ?? deltaclock (project 3)

	RESTORE_OS
	return;
} // end powerDown

