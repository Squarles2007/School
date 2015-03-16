
/*****************************************************************************\
* Laboratory Exercises COMP 3510                                              *
* Author: Saad Biaz                                                           *
* Date  : March 5, 2013          							 *
* Group: Alex Aguirre and Kullen Williams                                     *
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





/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/
typedef struct deviceQueue {
	int head, tail;
	Event EventArray[MAX_NUMBER_DEVICES*MAX_EVENT_ID];
} Queue;



/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/
double RT[MAX_NUMBER_DEVICES];
double TT[MAX_NUMBER_DEVICES];
double EventCount[MAX_NUMBER_DEVICES] = {0};
Queue eventQueue;



/*****************************************************************************\
*                               Function prototypes                           *
\*****************************************************************************/

void Control(void);
void InterruptRoutineHandlerDevice(void);
void BookKeeping();
//void insert(Event event);  // this will be used if you wanted to use function queue
void enqueue(Event event);
Event dequeue(void);



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
  eventQueue.head = 0;
  eventQueue.tail = 0;
  Event event;
  while (1)
  {	
	if(eventQueue.head != eventQueue.tail)
	{
		event = dequeue();
		Server(&event);
		TT[event.DeviceID] += Now() - event.When;
	}
  } //end while(1)

} //end control 


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This routine is run whenever an event occurs on a device    *
*           The id of the device is encoded in the variable flag        *
\***********************************************************************/
void InterruptRoutineHandlerDevice(void){
 	printf("An event occured at %f  Flags = %d \n", Now(), Flags);
	// Put Here the most urgent steps that cannot wait
	Status CurrentStatus = Flags;  //snapShot of Flag status
	Flags = 0; //rest Flags
	Event event;
	int position = 0;	

	while(CurrentStatus){
		if(CurrentStatus & 1){
			event = BufferLastEvent[position];
			enqueue(event);
			RT[event.DeviceID] += Now() - event.When;
			DisplayEvent('x', &event);
			EventCount[event.DeviceID] = EventCount[event.DeviceID] + 1;

		} // end if

		position++;
		CurrentStatus = CurrentStatus >> 1;

	} //end While CurrentStatus
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
  double TotalAvgRT = 0;
  double TotalAvgTT = 0;
  int i = 0;
  double avgMissed = 0;
  double missed = 0;
  double avgRT = 0;
  double avgTT = 0;
  while(EventCount[i] > 0){
	printf("\nDevice %d processed%10.f events out of 100\n",i,EventCount[i]);
    	printf("	Device %d missed percentage:	%10.f percent\n", i, (100 - EventCount[i]));
    	printf("	AVG Response Time:			%10.3f\n", RT[i]/100);
	printf("	AVG Turnaroun Time:			%10.3f\n", TT[i]/100); 
	avgRT += RT[i];
       avgTT += TT[i];
       avgMissed += (100 - EventCount[i]);
	i++;
  }
  	
  avgMissed = (avgMissed/(i *100) * 100);
  printf("\n\nAVG total missed events:	%10.3f\n", avgMissed);
  printf("AVG total response time:		%10.3f\n", avgRT/(i * 100));
  printf("AVG total turnaround time:	%10.3f\n", avgTT/(i * 100));
}
	
/***********************************************************************\
INSERT performs insertion sort on PriotyEventArray to place events in
priority order.
Lowest Priority number has highest priority

\***********************************************************************/

//NOT USED FOR LAB02
// void insert(Event event){
	
// 	pIndex++;	
// 	//printf("INSERT P-INDEX = %d\n",pIndex);
// 	if(pIndex == 0){
// 		PriorityEventArray[0] = event;
// 	}
// 	else {
		
// 		int key;
// 		int j;
// 		Event keyEvent;
// 		keyEvent = event;

// 		j = pIndex-1;
// 		key = PriorityEventArray[pIndex].priority;
	
// 		while(key > PriorityEventArray[j].priority && j >= 0){
// 			PriorityEventArray[j-1] = PriorityEventArray[j];
// 			j--;
// 		}
// 		PriorityEventArray[j+1] = keyEvent;

// 	} //end else
		
// } //end insert

void enqueue(Event event){
	eventQueue.EventArray[eventQueue.tail] = event;
	eventQueue.tail++;
}

Event dequeue(void){
	Event event = eventQueue.EventArray[eventQueue.head];
	eventQueue.head++;
	return event;
}
