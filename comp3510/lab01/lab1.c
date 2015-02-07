
/*****************************************************************************\
* Laboratory Exercises COMP 3510                                              *
* Author: Saad Biaz                                                           *
* Date  : January 18, 2014                                                    *
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


#define NUM_EVENTS_PER_DEVICE 100


/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/

typedef struct deviceEvents {
    double responseTimes[NUM_EVENTS_PER_DEVICE];
    double turnAroundTimes[NUM_EVENTS_PER_DEVICE];
    double hasProcessed[NUM_EVENTS_PER_DEVICE];   //boolean 1 or 0
    double averageResponseTime;
    double averageTurnAroundTime;
    int missedEvents;
    int seenEvents;
} DeviceEvents;


/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/
DeviceEvents deviceEventsInfo[MAX_NUMBER_DEVICES];



/*****************************************************************************\
*                               Function prototypes                           *
\*****************************************************************************/

void Control(void);



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
* WHILE JOBS STILL RUNNING                                                    *
*    CREATE PROCESSES                                                         *
*    RUN LONG TERM SCHEDULER                                                  *
*    RUN SHORT TERM SCHEDULER                                                 *
*    DISPATCH NEXT PROCESS                                                    *
\*****************************************************************************/

int main (int argc, char **argv) {

   if (Initialization(argc,argv)){
     Control();
   } 
} /* end of main function */

/***********************************************************************\
 * Input : none                                                          *
 * Output: None                                                          *
 * Function: Monitor Devices and process events                          *
 \***********************************************************************/
void Control(void){
  int i = 0;
  Status LastStatus = 0;
  Event event;

  //Another copy of Flags
  Status SnapShot = 0;

  while (1) {
    
   // printf("%10.3f   Flags = %d - \n ", Now(), Flags);
    //sleep(1); // Just to slow down to have time to see Flags
    if (Flags != LastStatus)	
	{
      		LastStatus = Flags;
		SnapShot = Flags;
      		printf("\n >>>>>>>>>  >>> When: %10.3f  Flags = %d\n", Now(),Flags);
		

		//This will be the amount of binary ones we find in "Flags"
		int c = 0;

		//bit
		int b = 0;

		int x = 31;
		for(x; x >= 0; x--)
		{
			b = SnapShot >> x;
			
			if(b & 1)
			{
				c++;
			}	
		}

		//bit
		b = 0;
		int y = 31;
		for(y;y >= 0; y--)
		{
			b = LastStatus >> y;

			//If this bit is one then this device at index 'i' is set
			if(b & 1)
			{
				//Use Biaz's method to display this events content
				DisplayEvent('a', &BufferLastEvent[y]);
				Server(&BufferLastEvent[y]);

				//Decrement the amount of ones we found
				c--;
			}
			else
			{
				//This device at index 'i' is not set
			}

			//If c = 0, then we have no need to keep searching for set bits
			if(c == 0)
			{
				break;
			}

		}
		//printf("FLAGS BEFORE RESET: %d",Flags);
		//Set Flags back to zero
		Flags = 0;
	
    	}

  } //end while(1)
}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This must print out the number of Events buffered not yet   *
*           not yet processed (Server() function not yet called)        *
\***********************************************************************/
void BookKeeping(void){
  printf("\n >>>>>> Done\n");
}









