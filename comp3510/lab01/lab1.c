
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
//keeps track of the total response time and turn around time for the duration 
//of the program then once the program is finish bookkeepsing will calculate
//the avg by dividing by avgCount which is incr every time an event is served
double totalResponseTime = 0;      
double totalTurnAroundTime = 0;
double avgCounter = 0;

/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/



/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/



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
  double eventStartT, eventResponseT,eventTurnAroundT; // time markers for each event
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
		//Set Flags back to zero
		Flags = 0;

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
				event = BufferLastEvent[y];
				eventStartT = event.When;
				

				DisplayEvent('a', &BufferLastEvent[y]);
				
				totalResponseTime = totalResponseTime +(Now() - eventStartT);   //take time before Event is serviced then add to totalResonse
				Server(&BufferLastEvent[y]);
				totalTurnAroundTime = totalTurnAroundTime + ( Now() - eventStartT); //take time after event is serviced then add to totalTurnarround
				avgCounter++;
				
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
	double avgResponseTime = totalResponseTime / avgCounter;
	double avgTurnAroundTime = totalTurnAroundTime / avgCounter;
  printf("\n >>>>>> Done\n");
  printf("Average Response Time: %d\n", avgResponseTime);
  printf("Average TurnAround Time: %d\n", avgTurnAroundTime);
}









