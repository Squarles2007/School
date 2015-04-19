
/*****************************************************************************\
* Laboratory Exercises COMP 3510                                              *
* Author: Saad Biaz                                                           *
* Date  : April 1, 2014                                                   *
\*****************************************************************************/

/*****************************************************************************\
*                             Global system headers                           *
\*****************************************************************************/


#include "common.h"

/*****************************************************************************\
*                             Global data types                               *
\*****************************************************************************/



/*****************************************************************************\
*                             Global definitions                              *
\*****************************************************************************/
#define MAX_EVENT_ID 100

#define QUEUE_SIZE 2
//#define QUEUE_SIZE 8



/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/

typedef struct deviceQueue {
	Event event;
	int head, tail;
}Queue;

typedef struct deviceInfo{
	int head,tail;	

	Timestamp totalRT;
	Timestamp totalTT;
	Timestamp RT;
	Timestamp TT;
	int processed;
	Event eventQueue[QUEUE_SIZE];
}Device;




/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/

Device device[MAX_NUMBER_DEVICES];
int constant;
int deviceNum;


/*****************************************************************************\
*                               Function prototypes                           *
\*****************************************************************************/

void Control(void);
void InterruptRoutineHandlerDevice(void);
void BookKeeping();


/*****************************************************************************\
* function: main()                                                            *
* usage:    Create an artificial environment for embedded systems. The parent *
*           process is the "control" process while children process will gene-*
*           generate events on devices                                        *
*******************************************************************************
* Inputs: ANSI flat C command line parameters                                 *
* Output: None                                                                *
*                                                                             *
* INITIALIZE PROGRAM ENVIRONMENT                                              *
* START CONTROL ROUTINE                                                       *
\*****************************************************************************/

int main (int argc, char **argv) {

   if (Initialization(argc,argv)){
     Control();
   } 
} /* end of main function */

/***********************************************************************\
 * Input : none                                                          *
 * Output: None                                                          *
 * Function: Monitor Devices and process events (written by students)    *
 \***********************************************************************/
void Control(void){
  constant = QUEUE_SIZE - 1;
  device.head = 0;
  device.tail = 0;
  deviceNum = 0;
 
  Event event;

  while (1)
  {
	if(device[0].eventQueue[device.tail] != NULL) { deviceNum = 0};
	while(device.head >= device.tail){
		event = dequeue(device[deviceNum]);
		Server(&event);
		device[deviceNum]
	}
	++deviceNum;
  }

}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This routine is run whenever an event occurs on a device    *
*           The id of the device is encoded in the variable flag        *
\***********************************************************************/
void InterruptRoutineHandlerDevice(void){
  printf("An event occured at %f  Flags = %d \n", Now(), Flags);
	// Put Here the most urgent steps that cannot wait
}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This must print out the number of Events buffered not yet   *
*           not yet processed (Server() function not yet called)        *
\***********************************************************************/
void BookKeeping(void){
  // For EACH device, print out the following metrics :
  // 1) the percentage of missed events, 2) the average response time, and 
  // 3) the average turnaround time.
  // Print the overall averages of the three metrics 1-3 above
}


Event dequeue(int deviceNum){
	Event event =NULL;
	if(devices[deivceNum].eventQueue[head] != NULL){
		event = devices[deviceNum].eventQueue[head];
	}
	return event;
}



