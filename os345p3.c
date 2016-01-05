// os345p3.c - Jurassic Park
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
#include <time.h>
#include <assert.h>
#include "os345.h"
#include "os345park.h"

// ***********************************************************************
// project 3 variables

// Jurassic Park
extern JPARK myPark;
extern Semaphore* parkMutex;						// protect park access
extern Semaphore* fillSeat[NUM_CARS];			// (signal) seat ready to fill
extern Semaphore* seatFilled[NUM_CARS];		// (wait) passenger seated
extern Semaphore* rideOver[NUM_CARS];			// (signal) ride over
extern Semaphore* DCMutex;
extern Semaphore* helloSem;
extern DC_EVENT** dClock;
Semaphore* visitorSems[NUM_VISITORS]; //visitor timer semaphores
Semaphore* passengerSem;
Semaphore* needPassenger;
Semaphore* passengerMutex;
Semaphore* seatTaken;
Semaphore* passengerSeated;
Semaphore* GiftSem;
Semaphore* MuseumSem;
Semaphore* ParkEntrance;
Semaphore* driverSem;
Semaphore* driverMutex;
Semaphore* driverSems[NUM_DRIVERS];
Semaphore* needTicket;
Semaphore* carReady;
Semaphore* driverReady;
Semaphore* tickets;
Semaphore* ticketReady;
Semaphore* wakeupDriver;
Semaphore* needDriver;
Semaphore* driverSemMutex;
Semaphore* receivedCarID;
int currCar;



// ***********************************************************************
// project 3 functions and tasks
void CL3_project3(int, char**);
void CL3_dc(int, char**);


// ***********************************************************************
// ***********************************************************************
// project3 command
int P3_project3(int argc, char* argv[])
{
  int i,j,k;
	char buf[32];
	char* newArgv[2];
  parkMutex = 0; //initialize to zero
  
	// start park
	sprintf(buf, "jurassicPark");
	newArgv[0] = buf;
	createTask( buf,				// task name
		jurassicTask,				// task
		MED_PRIORITY,				// task priority
		1,								// task count
		newArgv);					// task argument

	// wait for park to get initialized...
	while (!parkMutex) SWAP;
	printf("\nStart Jurassic Park...");
  
  char ** carArgv = (char**)malloc(sizeof(char*)*NUM_CARS); SWAP;
  if(!carArgv) 
  {
    printf("Out of Memory"); SWAP;
    exit(1);
  }
  
  for (i = 0; i < NUM_CARS; i++)
  {
    carArgv[i] = (char*)malloc(sizeof(char)); SWAP;
    if (!carArgv[i]) exit(1); SWAP;
  }
  
  
  for (i = 0; i < NUM_CARS; i++)
  {
    createTask("Car", carTask, MED_PRIORITY,i,carArgv); SWAP;
  }

  char ** visitorArgv = (char**)malloc(sizeof(char*)*NUM_VISITORS); SWAP;
  if(!visitorArgv) 
  {
    printf("Out of Memory"); SWAP;
    exit(1);
  }
  
  myPark.numOutsidePark = NUM_VISITORS; SWAP;
  
  passengerSem = 0; SWAP;
  driverSem = 0; SWAP;
  needPassenger = createSemaphore("needPassenger", COUNTING,0); SWAP;
  passengerMutex = createSemaphore("passengerMutex", BINARY,1); SWAP;
  driverMutex = createSemaphore("driverMutex", BINARY,1); SWAP;  
  seatTaken = createSemaphore("seatTaken", BINARY, 0); SWAP;
  passengerSeated = createSemaphore("passengerSeated",BINARY,0); SWAP;
  GiftSem = createSemaphore("GiftSem", COUNTING, MAX_IN_GIFTSHOP); SWAP;
  MuseumSem = createSemaphore("MuseumSem", COUNTING, MAX_IN_MUSEUM); SWAP;
  ParkEntrance = createSemaphore("ParkEntrance", COUNTING, MAX_IN_PARK); SWAP;
  
  needTicket = createSemaphore("needTicket", BINARY, 0); SWAP;
  needDriver = createSemaphore("needDriver", BINARY, 0); SWAP;
  wakeupDriver = createSemaphore("wakeupDriver", BINARY, 0); SWAP;
  ticketReady = createSemaphore("ticketReady", BINARY, 0); SWAP;
  tickets = createSemaphore("tickets", COUNTING, MAX_TICKETS); SWAP;
  driverReady = createSemaphore("driverReady", BINARY, 0); SWAP;
  carReady = createSemaphore("carReady", BINARY, 0); SWAP;
  driverSemMutex = createSemaphore("driverSemMutex", BINARY, 1); SWAP; 
  receivedCarID = createSemaphore("receivedCarID", BINARY, 0); SWAP;
  
  
  
  for (i = 0; i < NUM_VISITORS; i++)
  {
    visitorArgv[i] = (char*)malloc(sizeof(char)); SWAP;
    if (!visitorArgv[i]) exit(1); SWAP;
  }
  
  
  for (i = 0; i < NUM_VISITORS; i++)
  {
    createTask("Visitor", visitorTask, MED_PRIORITY,i, visitorArgv); SWAP;
  }

  
  char ** driverArgv = (char**)malloc(sizeof(char*)*NUM_DRIVERS); SWAP;
  if(!driverArgv) 
  {
    printf("Out of Memory"); SWAP;
    exit(1);
  }  
  for (i = 0; i < NUM_DRIVERS; i++)
  {
    driverArgv[i] = (char*)malloc(sizeof(char)); SWAP;
    if (!driverArgv[i]) exit(1); SWAP;
  }

  for (i = 0; i < NUM_DRIVERS; i++)
  {
    createTask("Driver", driverTask, MED_PRIORITY,i, driverArgv); SWAP;
  }

  
	return 0;
} // end project3


// ***********************************************************************
// ***********************************************************************

int carTask(int argc, char** argv)
{
  int i;
  Semaphore* driverDone;
  Semaphore* rideDone[NUM_SEATS];
  int carID = argc; SWAP;
  while (1)
  {
    for (i = 0; i < NUM_SEATS; i++)
    {
      
      SEM_WAIT(fillSeat[carID]); SWAP;
      SEM_SIGNAL(needPassenger); SWAP;
      SEM_WAIT(seatTaken); SWAP;
      rideDone[i] = passengerSem; SWAP;
      SEM_SIGNAL(passengerSeated); SWAP;      
      SEM_SIGNAL(seatFilled[carID]); SWAP;    
      
    //  printf("\n filled %dth seat",i);
      if (i == NUM_SEATS - 1) // last passenger, so get driver
      {
        SEM_WAIT(driverMutex); SWAP;
        SEM_SIGNAL(needDriver); SWAP;
        SEM_SIGNAL(wakeupDriver); SWAP;
        SEM_WAIT(driverReady); SWAP;
        driverDone = driverSem; SWAP;
        currCar = carID; SWAP;
        SEM_SIGNAL(carReady); SWAP;
        SEM_WAIT(receivedCarID); SWAP;        
        SEM_SIGNAL(driverMutex); SWAP;
        
      }
      
      
    }

    SEM_WAIT(parkMutex); SWAP;
    myPark.numInCars += NUM_SEATS; SWAP;
    myPark.numInCarLine -= NUM_SEATS; SWAP;
    SEM_SIGNAL(parkMutex); SWAP;
    
    SEM_WAIT(rideOver[carID]); SWAP;
    SEM_SIGNAL(driverDone); SWAP;
  
    
    for (i = 0; i < NUM_SEATS; i++)
    {
      SEM_SIGNAL(rideDone[i]); SWAP;
    } 
  }
  return 0;
}


int driverTask(int argc, char** argv)
{
  char buf[32];
  int ID = argc; SWAP;
  int temp; SWAP;
  sprintf(buf, "driverSem%d", ID); SWAP;
  driverSems[ID] = createSemaphore(buf,BINARY,0); SWAP; 
  
  while(1)
  {
    
    SEM_WAIT(wakeupDriver); SWAP;
    
    if (semTryLock(needTicket))
    {
      myPark.drivers[ID] = -1; SWAP;
      SEM_WAIT(tickets); SWAP;
      myPark.numTicketsAvailable--; SWAP;
      SEM_SIGNAL(ticketReady); SWAP;
      myPark.drivers[ID] = 0; SWAP;
    }
    else if (semTryLock(needDriver))
    {
      SEM_WAIT(driverSemMutex); SWAP;
      driverSem = driverSems[ID]; SWAP;
      SEM_SIGNAL(driverReady); SWAP;
      SEM_WAIT(carReady); SWAP;
      myPark.drivers[ID] = currCar + 1; SWAP;
      SEM_SIGNAL(receivedCarID); SWAP;
      SEM_SIGNAL(driverSemMutex); SWAP;
      SEM_WAIT(driverSems[ID]); SWAP;
      myPark.drivers[ID] = 0; SWAP;
    }
    
    
    
  }
  
  return 0;
}

int visitorTask(int argc, char** argv)
{
  char buf[32];
  int visitorId = argc; SWAP;
  sprintf(buf, "visitorSem%d", visitorId); SWAP;
  visitorSems[visitorId] = createSemaphore(buf,BINARY,0); SWAP; 
 
  //wait to request park entrace
  insertDeltaClock(dClock, rand()%PARK_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;
  
  //request park entrance
  SEM_WAIT(ParkEntrance); SWAP;
  
  SEM_WAIT(parkMutex); SWAP;
  
  myPark.numOutsidePark--; SWAP;
  myPark.numInPark++; SWAP;
  myPark.numInTicketLine++; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;

  //wait to request tickets
  insertDeltaClock(dClock, rand()%PARK_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;

  //request ticket  
//  SEM_WAIT(tickets); SWAP;
  SEM_WAIT(driverMutex); SWAP;  
  SEM_SIGNAL(needTicket); SWAP;  
  SEM_SIGNAL(wakeupDriver); SWAP;
  SEM_WAIT(ticketReady); SWAP;
  SEM_SIGNAL(driverMutex); SWAP;
  
  //get into museum line
  SEM_WAIT(parkMutex); SWAP;
  myPark.numInTicketLine--; SWAP;
  myPark.numInMuseumLine++; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;
  
  //wait to get into museum
  insertDeltaClock(dClock, rand()%PARK_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;

  
  //try to get into museum
  SEM_WAIT(MuseumSem); SWAP;
  
  //get into museum
  SEM_WAIT(parkMutex); SWAP;
  myPark.numInMuseumLine--; SWAP;
  myPark.numInMuseum++; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;
  
  //random wait inside museum
  insertDeltaClock(dClock, rand()%MAX_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;

  
  //get out of musuem
  SEM_WAIT(parkMutex); SWAP;
  myPark.numInMuseum--; SWAP;
  myPark.numInCarLine++; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;
  
  //give up musuem sem
  SEM_SIGNAL(MuseumSem); SWAP;
  
  //random wait
  insertDeltaClock(dClock, rand()%MAX_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;

  //get in car
  SEM_WAIT(passengerMutex); SWAP;
  SEM_WAIT(needPassenger); SWAP;
  passengerSem = visitorSems[visitorId]; SWAP;
  SEM_SIGNAL(seatTaken); SWAP;
  SEM_WAIT(passengerSeated); SWAP;
  SEM_SIGNAL(tickets); SWAP; //release ticket
  myPark.numTicketsAvailable++; SWAP;  
  SEM_SIGNAL(passengerMutex); SWAP;

  SEM_WAIT(visitorSems[visitorId]); SWAP;

  //get in gift line
  SEM_WAIT(parkMutex); SWAP;
  myPark.numInGiftLine++; SWAP;
  myPark.numInCars--; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;

  //random wait
  insertDeltaClock(dClock, rand()%MAX_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;
  
  //try to get into gift shop
  SEM_WAIT(GiftSem); SWAP;
  
  SEM_WAIT(parkMutex); SWAP;
  myPark.numInGiftLine--; SWAP;
  myPark.numInGiftShop++; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;
  
  //random wait
  insertDeltaClock(dClock, rand()%MAX_WAIT,visitorSems[visitorId]); SWAP;
  SEM_WAIT(visitorSems[visitorId]); SWAP;

  //leave park
  SEM_WAIT(parkMutex); SWAP;
  myPark.numInGiftShop--; SWAP;
  myPark.numInPark--; SWAP;
  myPark.numExitedPark++; SWAP;
  SEM_SIGNAL(parkMutex); SWAP;
  SEM_SIGNAL(GiftSem); SWAP;
  SEM_SIGNAL(ParkEntrance); SWAP;
  
  return 0;
}

// ***********************************************************************
// ***********************************************************************
// delta clock command
int P3_dc(int argc, char* argv[])
{
	printf("\nDelta Clock"); SWAP;
  SEM_WAIT(DCMutex); SWAP;
  int i;
  for (i = dClock[0]->delta; i >= 1; i--)
  {
    printf("\nEvent %s, delta %d",dClock[i]->event->name,dClock[i]->delta);  SWAP;
  }
  
  SEM_SIGNAL(DCMutex); SWAP;
	return 0;
} // end CL3_dc

int P3_tdc(int argc, char** argv)
{
  while (1)
  {
    SEM_WAIT(helloSem); SWAP;
    printf("Hello!\n"); SWAP;
  }
  return 0; //don't know how we'd ever get here, but just in case . . .
}

int P3_hello(int argc, char** argv)
{
  if (argc <= 1) return -1; SWAP;
  char *p = argv[1]; SWAP;
  int time = 0; SWAP;
  while (*p)
  {
    if (!isdigit(*p)) return -1; SWAP;
    time = 10*time + *p - '0'; SWAP;
    p++;
  }
  
  insertDeltaClock(dClock,time,helloSem); SWAP;
  return 0;
}

int insertDeltaClock(DC_EVENT** dClock, int time, Semaphore* sem)
{
  int i;  
  if (dClock[0]->delta == MAX_DC || time < 0) return -1; SWAP;
  SEM_WAIT(DCMutex); SWAP;
  for (i = dClock[0]->delta; i >= 1 && time >= dClock[i]->delta; i--)
  {
    time -= dClock[i]->delta; SWAP;
    dClock[i+1] = dClock[i]; SWAP;
  }
  dClock[i+1] = (DC_EVENT*)malloc(sizeof(DC_EVENT)); SWAP;
  dClock[i+1]->delta = time; SWAP;
  dClock[i+1]->event = sem; SWAP;
  dClock[0]->delta++; SWAP;  
  SEM_SIGNAL(DCMutex); SWAP;
  return 0;
}

// only for kernel use
void deleteDeltaClock(DC_EVENT** dClock, Semaphore* sem)
{
  int i,j;
  if (!dClock[0]->delta) return;
  for (i = dClock[0]->delta; i >= 1; i--)
  {
    if (sem == dClock[i]->event)
    {
      
      if (i > 1)
      {
        dClock[i-1]->delta += dClock[i]->delta;
      }
      
      free(dClock[i]);
      dClock[0]->delta--;
      for (j = i; j <= dClock[0]->delta; j++)
      {
        dClock[j] = dClock[j+1];
      }
      return;
    }
  }
  
  return;
  
}

//only for kernel use
DC_EVENT* popDeltaClock(DC_EVENT** dClock)
{
  if (!dClock[0]->delta) return NULL;
  return dClock[dClock[0]->delta--];
}

/*
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// delta clock command
int P3_dc(int argc, char* argv[])
{
	printf("\nDelta Clock");
	// ?? Implement a routine to display the current delta clock contents
	//printf("\nTo Be Implemented!");
	int i;
	for (i=0; i<numDeltaClock; i++)
	{
		printf("\n%4d%4d  %-20s", i, deltaClock[i].time, deltaClock[i].sem->name);
	}
	return 0;
} // end CL3_dc


// ***********************************************************************
// display all pending events in the delta clock list
void printDeltaClock(void)
{
	int i;
	for (i=0; i<numDeltaClock; i++)
	{
		printf("\n%4d%4d  %-20s", i, deltaClock[i].time, deltaClock[i].sem->name);
	}
	return;
}


// ***********************************************************************
// test delta clock
int P3_tdc(int argc, char* argv[])
{
	createTask( "DC Test",			// task name
		dcMonitorTask,		// task
		10,					// task priority
		argc,					// task arguments
		argv);

	timeTaskID = createTask( "Time",		// task name
		timeTask,	// task
		10,			// task priority
		argc,			// task arguments
		argv);
	return 0;
} // end P3_tdc



// ***********************************************************************
// monitor the delta clock task
int dcMonitorTask(int argc, char* argv[])
{
	int i, flg;
	char buf[32];
	// create some test times for event[0-9]
	int ttime[10] = {
		90, 300, 50, 170, 340, 300, 50, 300, 40, 110	};

	for (i=0; i<10; i++)
	{
		sprintf(buf, "event[%d]", i);
		event[i] = createSemaphore(buf, BINARY, 0);
		insertDeltaClock(ttime[i], event[i]);
	}
	printDeltaClock();

	while (numDeltaClock > 0)
	{
		SEM_WAIT(dcChange)
		flg = 0;
		for (i=0; i<10; i++)
		{
			if (event[i]->state ==1)			{
					printf("\n  event[%d] signaled", i);
					event[i]->state = 0;
					flg = 1;
				}
		}
		if (flg) printDeltaClock();
	}
	printf("\nNo more events in Delta Clock");

	// kill dcMonitorTask
	tcb[timeTaskID].state = S_EXIT;
	return 0;
} // end dcMonitorTask


extern Semaphore* tics1sec;

// ********************************************************************************************
// display time every tics1sec
int timeTask(int argc, char* argv[])
{
	char svtime[64];						// ascii current time
	while (1)
	{
		SEM_WAIT(tics1sec)
		printf("\nTime = %s", myTime(svtime));
	}
	return 0;
} // end timeTask
*/

