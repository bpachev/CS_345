// os345p1.c - Command Line Processor
// ***********************************************************************
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
#include "os345signals.h"

// The 'reset_context' comes from 'main' in os345.c.  Proper shut-down
// procedure is to long jump to the 'reset_context' passing in the
// power down code from 'os345.h' that indicates the desired behavior.

extern jmp_buf reset_context;
// -----


#define NUM_COMMANDS 56
typedef struct								// command struct
{
	char* command;
	char* shortcut;
	int (*func)(int, char**);
	char* description;
} Command;

// ***********************************************************************
// project 1 variables
//
extern long swapCount;					// number of scheduler cycles
extern char inBuffer[];					// character input buffer
extern Semaphore* inBufferReady;		// input buffer ready semaphore
extern bool diskMounted;				// disk has been mounted
extern char dirPath[];					// directory path
extern char* commandBuf[COMMAND_BUF_LEN];
extern int curr_command;
extern int last_command;
Command** commands;						// shell commands


// ***********************************************************************
// project 1 prototypes
Command** P1_init(void);
Command* newCommand(char*, char*, int (*func)(int, char**), char*);

void mySigIntHandler()
{
	printf("Hellomynameisinigomontoyayoukilledmyfatherpreparetodie");
}

// ***********************************************************************
// myShell - command line interpreter
//
// Project 1 - implement a Shell (CLI) that:
//
// 1. Prompts the user for a command line.
// 2. WAIT's until a user line has been entered.
// 3. Parses the global char array inBuffer.
// 4. Creates new argc, argv variables using malloc.
// 5. Searches a command list for valid OS commands.
// 6. If found, perform a function variable call passing argc/argv variables.
// 7. Supports background execution of non-intrinsic commands.
//
int P1_shellTask(int argc, char* argv[])
{
	int i, found, newArgc;					// # of arguments
	char** newArgv;							// pointers to arguments
  char * buf = 0;
  int background;
	// initialize shell commands
	commands = P1_init();					// init shell commands

//	sigAction(mySigIntHandler, mySIGINT);


	while (1)
	{
		// output prompt
		if (diskMounted) printf("\n%s>>", dirPath);
		else printf("\n%ld>>", swapCount);

		SEM_WAIT(inBufferReady);			// wait for input buffer semaphore
		if (!inBuffer[0]) continue;		// ignore blank lines

		SWAP										// do context switch

		{
			// ?? >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// ?? parse command line into argc, argv[] variables
			// ?? must use malloc for argv storage!
      background = 0; //reset flag. If background is true, the command line ends with an ampersand.
      //and a child task is created to execute the command
      int seen_qoute = 0;
			static char *sp = 0;
      char * last_char = 0;
      newArgv = (char**)malloc(MAX_ARGS*sizeof(char*));

			// init arguments
			newArgc = 1;
			buf = sp = strdup(inBuffer);				// point to input string
      if (!commandBuf[last_command]) commandBuf[last_command] = strdup(buf);
      else
      {
        free(commandBuf[last_command]);
        commandBuf[last_command] = strdup(buf);
      }
      
      curr_command = last_command = NEXT_COM(last_command);
      
      newArgv[0] = sp;
      if (!sp)
      {
        printf("Out of memory in strdup\n");
        goto err;
      }
      
			// parse input string
			while (*sp)
			{
        switch (*sp)
        {
          case ' ':
            if (seen_qoute) break;
            *sp = 0;
            if (*(sp+1) == ' ' || *(sp+1) == '"') break;
            newArgv[newArgc++] = (sp+1);
            if (newArgc == MAX_ARGS)
            {
              printf("Too many arguments.\n");
              goto err;
            }
            break;
          case '"':
            if (seen_qoute)
            {
              *sp = 0;
              seen_qoute = 0;
              break;
            }
            else
            {
              seen_qoute = 1;
              *sp++ = 0;
              newArgv[newArgc++] = (sp);
              if (newArgc == MAX_ARGS)
              {
                printf("Too many arguments.\n");
                goto err;
              }              
              continue;
            }
          default:
            break;
        }
        sp++;
			}
			
			if (seen_qoute)
      {
        printf("\nQoute not closed.");
        goto err;
      }
			
			last_char = newArgv[newArgc-1] + strlen(newArgv[newArgc-1]) - 1;
     // printf("%s %s",last_char,newArgv[newArgc-1]);
			if (*(last_char) == '&') 
      {
        background = 1;
        *last_char = 0; //get rid of ampersand
      }
			
		}	// ?? >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

		// look for command
		for (found = i = 0; i < NUM_COMMANDS; i++)
		{
			if (!strcmp(newArgv[0], commands[i]->command) ||
				 !strcmp(newArgv[0], commands[i]->shortcut))
			{
				// command found
        if (background)
        {
          createTask(newArgv[0],commands[i]->func,3,newArgc,newArgv);
          break;
        }
        
				int retValue = (*commands[i]->func)(newArgc, newArgv);
				if (retValue) printf("\nCommand Error %d", retValue);
				found = TRUE;
				break;
			}
		}
		if (!found)	printf("\nInvalid command!");

		// ?? free up any malloc'd argv parameters
err:
    free(buf);
    free(newArgv);
		for (i=0; i<INBUF_SIZE; i++) inBuffer[i] = 0;
	}
	return 0;						// terminate task
} // end P1_shellTask


// ***********************************************************************
// ***********************************************************************
// P1 Project
//
int P1_project1(int argc, char* argv[])
{
	SWAP										// do context switch

	return 0;
} // end P1_project1



// ***********************************************************************
// ***********************************************************************
// quit command
//
int P1_quit(int argc, char* argv[])
{
	int i;

	// free P1 commands
	for (i = 0; i < NUM_COMMANDS; i++)
	{
		free(commands[i]->command);
		free(commands[i]->shortcut);
		free(commands[i]->description);
	}
	free(commands);

	// powerdown OS345
	longjmp(reset_context, POWER_DOWN_QUIT);
	return 0;
} // end P1_quit



// **************************************************************************
// **************************************************************************
// lc3 command
//
int P1_lc3(int argc, char* argv[])
{
	strcpy (argv[0], "0");
	return lc3Task(argc, argv);
} // end P1_lc3



// ***********************************************************************
// ***********************************************************************
// help command
//
int P1_help(int argc, char* argv[])
{
	int i;

	// list commands
	for (i = 0; i < NUM_COMMANDS; i++)
	{
		SWAP										// do context switch
		if (strstr(commands[i]->description, ":")) printf("\n");
		printf("\n%4s: %s", commands[i]->shortcut, commands[i]->description);
	}

	return 0;
} // end P1_help

int P1_add(int argc, char** argv)
{
  int i;
  int sum = 0;
  for (i = 1; i < argc; i++)
  {
    char * p = argv[i];
    int curr_int = 0;
    int mode = DEC;
    if (*p == '0') mode = OCT; 
    while (*p)
    {
      if (*p == 'x')
      {
        mode = HEX;
        p++;
        continue;
      }
      
      if (*p == '%')
      {
        mode = BIN;
        p++;
        continue;
      }
      
      switch (mode)
      {
        case DEC:
          if (!isdigit(*p)) return 1;
          curr_int = *p - '0' + 10*curr_int; 
          break;
        case HEX:
          if (isdigit(*p)) curr_int = 16*curr_int + *p - '0';
          else if (*p <= 'f' && *p >= 'a') curr_int = 16*curr_int + 10 + *p - 'a';
          else return 1;
          break;
        case BIN:
          if (!isdigit(*p) || *p > '1') return 1;
          curr_int = *p - '0' + 2*curr_int;
          break;
        case OCT:
          if (!isdigit(*p) || *p > '7') return 1;
          curr_int = *p - '0' + 8*curr_int; 
          break;
      }
      p++;
    }
    sum += curr_int;
  }
  printf("\n%d",sum);
  return 0;
}

int P1_args(int argc, char** argv)
{
  int i;
  for (i = 1; i < argc; i++)
  {
    printf("\n%s",argv[i]);
  }
  return 0;
}

int P1_time_waster(int arc, char** argv)
{
  printf("\n");
  int i = 0;
  while(1)
  {
    SWAP;
    i++;
    if(i == 100000) printf("Hi\n");
  }
  return 0;
}

// ***********************************************************************
// ***********************************************************************
// initialize shell commands
//
Command* newCommand(char* command, char* shortcut, int (*func)(int, char**), char* description)
{
	Command* cmd = (Command*)malloc(sizeof(Command));

	// get long command
	cmd->command = (char*)malloc(strlen(command) + 1);
	strcpy(cmd->command, command);

	// get shortcut command
	cmd->shortcut = (char*)malloc(strlen(shortcut) + 1);
	strcpy(cmd->shortcut, shortcut);

	// get function pointer
	cmd->func = func;

	// get description
	cmd->description = (char*)malloc(strlen(description) + 1);
	strcpy(cmd->description, description);

	return cmd;
} // end newCommand


Command** P1_init()
{
	int i  = 0;
	Command** commands = (Command**)malloc(sizeof(Command*) * NUM_COMMANDS);

	// system
	commands[i++] = newCommand("quit", "q", P1_quit, "Quit");
	commands[i++] = newCommand("kill", "kt", P2_killTask, "Kill task");
	commands[i++] = newCommand("reset", "rs", P2_reset, "Reset system");

	// P1: Shell
	commands[i++] = newCommand("project1", "p1", P1_project1, "P1: Shell");
	commands[i++] = newCommand("help", "he", P1_help, "OS345 Help");
	commands[i++] = newCommand("lc3", "lc3", P1_lc3, "Execute LC3 program");
  commands[i++] = newCommand("add","add", P1_add,"Add All Arguments");
  commands[i++] = newCommand("args","args", P1_args,"Display Arguments");
  commands[i++] = newCommand("time_waster","time_waster",P1_time_waster,"Run Indefinitely");
  
	// P2: Tasking
	commands[i++] = newCommand("project2", "p2", P2_project2, "P2: Tasking");
	commands[i++] = newCommand("semaphores", "sem", P2_listSems, "List semaphores");
	commands[i++] = newCommand("tasks", "lt", P2_listTasks, "List tasks");
	commands[i++] = newCommand("signal1", "s1", P2_signal1, "Signal sem1 semaphore");
	commands[i++] = newCommand("signal2", "s2", P2_signal2, "Signal sem2 semaphore");
  commands[i++] = newCommand("test_queue", "testq", P2_test_queue, "Test Queue Functions");
  commands[i++] = newCommand("dump_rq", "dump_rq", P2_dump_rq, "Print Ready Queue");
  
	// P3: Jurassic Park
	commands[i++] = newCommand("project3", "p3", P3_project3, "P3: Jurassic Park");
	commands[i++] = newCommand("deltaclock", "dc", P3_dc, "List deltaclock entries");
  commands[i++] = newCommand("testdclock", "tdc", P3_tdc, "Test deltaclock");
  commands[i++] = newCommand("p3hello","p3hello",P3_hello,"Add hello event to clock");
  
	// P4: Virtual Memory
	commands[i++] = newCommand("project4", "p4", P4_project4, "P4: Virtual Memory");
	commands[i++] = newCommand("frametable", "dft", P4_dumpFrameTable, "Dump bit frame table");
	commands[i++] = newCommand("initmemory", "im", P4_initMemory, "Initialize virtual memory");
	commands[i++] = newCommand("touch", "vma", P4_vmaccess, "Access LC-3 memory location");
	commands[i++] = newCommand("stats", "vms", P4_virtualMemStats, "Output virtual memory stats");
	commands[i++] = newCommand("crawler", "cra", P4_crawler, "Execute crawler.hex");
	commands[i++] = newCommand("memtest", "mem", P4_memtest, "Execute memtest.hex");

	commands[i++] = newCommand("frame", "dfm", P4_dumpFrame, "Dump LC-3 memory frame");
	commands[i++] = newCommand("memory", "dm", P4_dumpLC3Mem, "Dump LC-3 memory");
	commands[i++] = newCommand("page", "dp", P4_dumpPageMemory, "Dump swap page");
	commands[i++] = newCommand("virtual", "dvm", P4_dumpVirtualMem, "Dump virtual memory page");
	commands[i++] = newCommand("root", "rpt", P4_rootPageTable, "Display root page table");
	commands[i++] = newCommand("user", "upt", P4_userPageTable, "Display user page table");

	// P5: Scheduling
	commands[i++] = newCommand("project5", "p5", P5_project5, "P5: Scheduling");
//	commands[i++] = newCommand("stress1", "t1", P5_stress1, "ATM stress test1");
//	commands[i++] = newCommand("stress2", "t2", P5_stress2, "ATM stress test2");

	// P6: FAT
	commands[i++] = newCommand("project6", "p6", P6_project6, "P6: FAT");
	commands[i++] = newCommand("change", "cd", P6_cd, "Change directory");
	commands[i++] = newCommand("copy", "cf", P6_copy, "Copy file");
	commands[i++] = newCommand("define", "df", P6_define, "Define file");
	commands[i++] = newCommand("delete", "dl", P6_del, "Delete file");
	commands[i++] = newCommand("directory", "dir", P6_dir, "List current directory");
	commands[i++] = newCommand("mount", "md", P6_mount, "Mount disk");
	commands[i++] = newCommand("mkdir", "mk", P6_mkdir, "Create directory");
	commands[i++] = newCommand("run", "run", P6_run, "Execute LC-3 program");
	commands[i++] = newCommand("space", "sp", P6_space, "Space on disk");
	commands[i++] = newCommand("type", "ty", P6_type, "Type file");
	commands[i++] = newCommand("unmount", "um", P6_unmount, "Unmount disk");

	commands[i++] = newCommand("fat", "ft", P6_dfat, "Display fat table");
	commands[i++] = newCommand("fileslots", "fs", P6_fileSlots, "Display current open slots");
	commands[i++] = newCommand("sector", "ds", P6_dumpSector, "Display disk sector");
	commands[i++] = newCommand("chkdsk", "ck", P6_chkdsk, "Check disk");
	commands[i++] = newCommand("final", "ft", P6_finalTest, "Execute file test");

	commands[i++] = newCommand("open", "op", P6_open, "Open file test");
	commands[i++] = newCommand("read", "rd", P6_read, "Read file test");
	commands[i++] = newCommand("write", "wr", P6_write, "Write file test");
	commands[i++] = newCommand("seek", "sk", P6_seek, "Seek file test");
	commands[i++] = newCommand("close", "cl", P6_close, "Close file test");

	assert(i == NUM_COMMANDS);

	return commands;

} // end P1_init
