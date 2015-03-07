
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




/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/
Event PriorityEveryArray[MAX_EVENT_ID * MAX_NUMBER_DEVICES];
int pIndex = 0;
double RT[MAX_NUMBER_DEVICES];
double TT[MAX_NUMBER_DEVICES];


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

  while (1){
	if(PriorityEventArray[0] != null){
		
		


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
	Event event;
	Status CurrentStatus = Flags;
	int position = 0;	

	Flags = 0;
	
	while(CurrentStatus){
		event = BufferLastEvent[position];		
		if(CurrentStatus & 1){
			RT[event.DeviceID] += Now() - event.When;
			DisplayEvent('x', &event);
			//insert(event);
			

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
}

/***********************************************************************\
INSERT performs insertion sort on PriotyEventArray to place events in
priority order.

\***********************************************************************/
void insert(Event event){
	
	if(pIndex == 0){
		PriorityEveryArray[0] = event;
	}
	else {
		PriorityEveryArray[pIndex] = event;
		int key;
		int j;
		Event keyEvent;
		int i = 1;
		for( i ; i < pIndex; i++){
			keyEvent = PriorityEveryArray[i];
			key = PriorityEveryArray[i].priority;
			j = i-1;
			while(j >= 0 && key > PriorityEveryArray[j].priority)
			{
				PriorityEveryArray[j-1] = PriorityEveryArray[j];
				j--;
			}
			PriorityEveryArray[j+1] = keyEvent;
		}

	} //end else
	pIndex++;
} //end inset



