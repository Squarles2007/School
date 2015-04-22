
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
//#define QUEUE_SIZE 32



/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/

//Each device has an event array (queue) of size QUEUE_SIZE
//each device also has a head and tail index responseTime (RT) and turnaroundTime(TT)
//keeps up with events that have been processed
typedef struct deviceInfo{
	int head,tail;	
	Timestamp RT;
	Timestamp TT;
	int processed;
	int counter;
	Event eventQueue[QUEUE_SIZE];
}Device;




/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/

Device device[MAX_NUMBER_DEVICES]; //array of devices
int constant; //constant is &-ed with the value of head and tail for circular incrementing
int maxDevice;
int deviceNum;
int newEvent;



/*****************************************************************************\
*                               Function prototypes                           *
\*****************************************************************************/

void Control(void);
void InterruptRoutineHandlerDevice(void);
void BookKeeping();
Event dequeue(int deviceNum);
void enqueue(Event event);
//void printQueue(void); //testing only



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
  Event event;
  maxDevice = 0;
  int x = 0;
  newEvent = 0;


//intialize content
 int i = 0;
 for(i ; i < MAX_NUMBER_DEVICES ; i++)
 {	
	device[i].head 	= 0;
	device[i].tail 	= 0;
	device[i].TT 		= 0;
	device[i].RT    	= 0;
	device[i].processed	= 0;
	device[i].counter 	= 0;
}

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    while (1)
    {
        
            deviceNum = 0;
            while(deviceNum <= maxDevice)
            {


                x = deviceNum; //save value of devicenum to x to pervent shared data issues

                if(device[x].counter > 0)
                {
                  
		    event = dequeue(x);

                  Server(&event);
      	 	    device[x].TT += Now() - event.When;
                  
                  printf("\nServiced Device: %d  Event: %d  COUNTER : %d\n\n", event.DeviceID , event.EventID, device[x].counter);
      	 	   	
		    device[x].processed++;
      	 	    deviceNum = 0; //reset device number back to zero everytime . Zero has highest priority
		 }
                else
      	        { 
      	 		    
                  deviceNum++;

      	        }

     	    }	//end while(deviceNum)
           
        
    } //end while(1)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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
  Status CurrentStatus = Flags;
  Flags = 0;
  Event event;
  deviceNum = 0;
  int position = 0;
  newEvent = 1;

  while(CurrentStatus)
  {
  	if(CurrentStatus & 1){
  		  event = BufferLastEvent[position];
  		  
		  if(device[event.DeviceID].counter < QUEUE_SIZE ){
			++device[event.DeviceID].counter;
		  	enqueue(event);
		  }
  		  device[event.DeviceID].RT += Now() - event.When;
  		  DisplayEvent('x', &event);
        
  	}
  	position++;
    	CurrentStatus = CurrentStatus >> 1;
  }
}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This must print out the number of Events buffered not yet   *
*           not yet processed (Server() function not yet called)        *
\***********************************************************************/
void BookKeeping(void)
{
    // For EACH device, print out the following metrics :
    // 1) the percentage of missed events, 2) the average response time, and 
    // 3) the average turnaround time.
    // Print the overall averages of the three metrics 1-3 above
    int maxEvent = 100;
    double TotalAvgRT = 0;
    double TotalAvgTT = 0;
    double avgMissed = 0;
    int totalMissed	= 0;
    int i = 0;
    while(i <= maxDevice){
      avgMissed = (double) (maxEvent - device[i].processed) / maxEvent;
      totalMissed +=  maxEvent - device[i].processed;
  	  TotalAvgRT = device[i].RT / maxEvent;
      TotalAvgTT = device[i].TT / maxEvent;
  		printf("Device %d processed %d events out of %d\n", i, device[i].processed, maxEvent);
  		printf("\tMissed  percentage:		%10.3f\n", 		avgMissed );
  		printf("\tAverage response time:		%10.3f\n", 	TotalAvgRT);
  		printf("\tAverage turnaround time:	%10.3f\n", 		TotalAvgTT);
 		  i++;
 	  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * ENQUEUE 
 * Place new event on device[x]'s eventQueue at index tail. Then increment tail
 * @param event - Event
 * @return void
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void enqueue(Event event)
{
	int deviceNumX = event.DeviceID;
	device[deviceNumX].eventQueue[device[deviceNumX].tail] = event;
	
	device[deviceNumX].tail = (device[deviceNumX].tail + 1) & constant;
  	if(event.DeviceID > maxDevice) 
  	{ 
      		maxDevice = event.DeviceID;
  	}
	
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * DEQUEUE 
 * Return Event that is on Device[x]'s eventQueue at index head. increment head
 * @param deviceNum - int
 * @return event - Event
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
Event dequeue(int deviceNum)
{
	Event event = device[deviceNum].eventQueue[device[deviceNum].head] ;
	--device[deviceNum].counter;
	device[deviceNum].head = (device[deviceNum].head + 1) & constant; //increment head
	return event;
}

//not called.
void printQueue(void){
	int i = 0;
	int j = 0;
	for(i ; i <= maxDevice ; i++){
		j = 0;
		for(j ; j < QUEUE_SIZE ; j++){
			printf("%d | ", device[i].eventQueue[j].EventID);
		}
		printf("\n");
	}
}

