#include "vxWorks.h"
#include "sysLib.h"
#include "taskLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "semLib.h"
#include "cinterface.h"
#include "wdLib.h"
#include "time.h"
#include "ioLib.h"

static const struct timespec EMPTY_STRUCT;
static struct timespec start;
static struct timespec stop;
double timeDifference = 0.0;


void startMotor(void);
void stopMotor(void);

void resetSizeSensors(char conveyor);
void resetCountSensor(char conveyor);


char readSizeSensors(char conveyor);
char readCountSensor(char conveyor);

void setGates(char state);

SEM_ID COUNT_BELT0;
SEM_ID COUNT_BELT1;

WDOG_ID  timer_ID_SMALLBLOCKCLOSE01;
WDOG_ID  timer_ID_SMALLBLOCKOPEN01;
WDOG_ID	timer_ID_SMALLBLOCKCLOSE02;
WDOG_ID	timer_ID_SMALLBLOCKOPEN02;
WDOG_ID	timer_ID_SMALLBLOCKCLOSE03;
WDOG_ID	timer_ID_SMALLBLOCKOPEN03;
WDOG_ID	timer_ID_SMALLBLOCKCLOSE04;
WDOG_ID	timer_ID_SMALLBLOCKOPEN04;
WDOG_ID	timer_ID_SMALLBLOCKCLOSE05;
WDOG_ID	timer_ID_SMALLBLOCKOPEN05;

WDOG_ID	timer_ID_LARGEBLOCKCOUNT01;
WDOG_ID	timer_ID_LARGEBLOCKCOUNT02;
WDOG_ID	timer_ID_LARGEBLOCKCOUNT03;


WDOG_ID timer_ID_SMALLBLOCKCLOSE11;	
WDOG_ID timer_ID_SMALLBLOCKOPEN11;
WDOG_ID timer_ID_SMALLBLOCKCLOSE12;
WDOG_ID timer_ID_SMALLBLOCKOPEN12;
WDOG_ID timer_ID_SMALLBLOCKCLOSE13;
WDOG_ID timer_ID_SMALLBLOCKOPEN13;
WDOG_ID timer_ID_SMALLBLOCKCLOSE14;
WDOG_ID timer_ID_SMALLBLOCKOPEN14;
WDOG_ID timer_ID_SMALLBLOCKCLOSE15;
WDOG_ID timer_ID_SMALLBLOCKOPEN15;


WDOG_ID timer_ID_LARGEBLOCKCOUNT11;
WDOG_ID timer_ID_LARGEBLOCKCOUNT12;
WDOG_ID timer_ID_LARGEBLOCKCOUNT13;






 	 	/* +-------------------------+ */
               /* ¦ D E C L A R A T I O N S ¦ */
               /* +-------------------------+ */    
int i=0; /* SMALL BLOCK INCREMENT BELT 0 */
int j=0; /* SMALL BLOCK INCREMENT BELT 0 */
int k=0; /* SMALL BLOCK INCREMENT BELT 1 */
int l=0; /* LARGE BLOCK INCREMENT BELT 1 */
char gate0; /* SIZEDETECTION_BELT0 BELT 0 */
char gate1; /* SIZEDETECTION_BELT0 BELT 1 */
int Large0=0,Large1=0;  /* COUNT INCREMENTS */
int taskID,taskID1,taskID2,taskID3,taskID4;	/* STORAGE OF ID TASKS */

struct timespec des;


               /* +-------------------------+ */
               /* ¦       GATE PROCESSES    ¦  */
               /* +-------------------------+ */ 

void GATECLOSE0 (void) {  
	gate0 = '1';             /* IF TWO SMALL BLOCKS ARE DETECTED ON THE FIRST BELT
	                         BOTH GATE 1 AND 0  ARE CLOSED*/
	

	if (gate1 == '1') { 
		setGates(3);
	}

	else if (gate1 == '0') 
	{						 /* ELSE YOU LEAVE GATE 0 CLOSED AND GATE 1 OPEN*/
		setGates(1);
	}
}

void GATECLOSE1(void) { 
	gate1 = '1';            		  /* IF TWO SMALL BLOCKS ARE DETECTED ON THE SECOND BELT*/
	                          	  	  /* BOTH GATE 1 AND 0 ARE CLOSED*/
	if (gate0 == '1') {
		setGates(3);
	}

	else if (gate0 == '0') 
	{  								/*ELSE YOU LEAVE GATE 1 CLOSED AND GATE 0 OPEN*/
		setGates(2);
	}
}

void GATEOPEN0 (void) {
	gate0 = '0';           			/*IF TWO SMALL BLOCKS ARE DETECTED ON THE FIRST BELT
                            		BOTH GATE 1 AND O ARE LEFT OPEN*/
	
	if (gate1 == '0') {
		setGates(0);

	}

	else if (gate1 == '1') 
	{  							/*ELSE LEAVE GATE 1 CLOSED AND GATE 0 OPEN*/
		setGates(2);
	}
}

void GATEOPEN1(void) {
	gate1 = '0';              		 /*IF TWO SMALL BLOCKS ARE DETECTED ON THE SECOND BELT
	                           	   BOTH GATE 1 AND 0 ARE LEFT OPEN*/
	if (gate0 == '0') {
		setGates(0);
	}

	else if (gate0 == '1')
	{  								/*ELSE LEAVE GATE 0 CLOSED AND GATE 1 OPEN*/
		setGates(1);
	}
}

               /* +-------------------------+ */
               /* ¦       COUNT PROCESSES   ¦  */
               /* +-------------------------+ */ 

void COUNT0 (void) 

{
	while(1)
	/* -- without the while loop the function doesnt loop. so it takes the sem and count once */
	{
		semTake(COUNT_BELT0, WAIT_FOREVER);
		resetCountSensor(0);
        if (readCountSensor(0) != 0 )
        	{
        		printf("LARGE BLOCK BELT0 count\n");
        		++Large0;
        	}
	}    

}
void COUNT1 (void) 
{	/* This function takes a semaaphore when ever a large block is detected */

	/* */
	while(1)
	{
	semTake(COUNT_BELT1, WAIT_FOREVER);


	resetCountSensor(1);
     if (readCountSensor(1) != 0 )
        {
    	 	 printf("LARGE BLOCK BELT1 count\n");
    	 	 ++Large1;	    	
        }

	}    

}



void SEM_BELT0 (void)
{
	semGive(COUNT_BELT0);
}
void SEM_BELT1 (void)

{
	semGive (COUNT_BELT1);
}


              /* +------------------------------+ */
              /* ¦       SIZE PROCESSES    ¦ */
              /* +------------------------------+ */ 

void SizeDetection_Belt1 (void)

{
	int res40,res41,res42,res43,res44;
	int res50,res51,res52,res53,res54;
	int res60,res61,res62;

	int First_Reading1 = 0;
	int Second_Reading1=0;

	int currentstate2;

	int TimerCount_s2=0, TimerCount_sL2=0;

	/*Create timers five different sets of timers*/
/*SBCT1 SMALL BLOCK CLOSE/OPEN TIMER*/
	timer_ID_SMALLBLOCKCLOSE11 = wdCreate();
	timer_ID_SMALLBLOCKOPEN11 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE12 = wdCreate();
	timer_ID_SMALLBLOCKOPEN12 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE13 = wdCreate();
	timer_ID_SMALLBLOCKOPEN13 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE14 = wdCreate();
	timer_ID_SMALLBLOCKOPEN14 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE15 = wdCreate();
	timer_ID_SMALLBLOCKOPEN15 = wdCreate();

	timer_ID_LARGEBLOCKCOUNT11 = wdCreate();
	timer_ID_LARGEBLOCKCOUNT12 = wdCreate();
	timer_ID_LARGEBLOCKCOUNT13 = wdCreate();


	while(1){
		taskDelay(0.3 * sysClkRateGet());
		resetSizeSensors(1);

		First_Reading1 = Second_Reading1;
		Second_Reading1 = readSizeSensors(1);

		if (First_Reading1==1 && Second_Reading1==0)

			{ 
				k++;
				printf("Small Block BELT1\n");
				currentstate2=1;
			}

		else if (First_Reading1==1 && Second_Reading1==3)
			{ 
				l++;
				printf("Large Block BELT1\n");
				currentstate2=2;
				

			}


		              /* +-------------------------+ */
		              /* ¦ GATE0 & COUNT0 PROCESSES ¦    */
		              /* +-------------------------+ */ 
		
		
switch (currentstate2)

{

case 1:
	if (TimerCount_s2 ==0)
	{
	currentstate2=0;
	res40 = wdStart(timer_ID_SMALLBLOCKCLOSE11, 2.5 * sysClkRateGet(), (FUNCPTR)GATECLOSE1, 0);	
	res50 = wdStart(timer_ID_SMALLBLOCKOPEN11, 4.2* sysClkRateGet(), (FUNCPTR)GATEOPEN1, 0);
	TimerCount_s2=1;
	break;
	}

	else if (TimerCount_s2 ==1)
	{	
	currentstate2=0;
	res41 = wdStart(timer_ID_SMALLBLOCKCLOSE12,2.5 * sysClkRateGet(), (FUNCPTR)GATECLOSE1, 0);	
	res51 = wdStart(timer_ID_SMALLBLOCKOPEN12, 4.2 * sysClkRateGet(), (FUNCPTR)GATEOPEN1, 0);
	TimerCount_s2=2;
	break;
	}
	else if (TimerCount_s2 ==2)
	{
		
	currentstate2=0;

	res42 = wdStart(timer_ID_SMALLBLOCKCLOSE13, 2.5 * sysClkRateGet(), (FUNCPTR)GATECLOSE1, 0);	
	res52 = wdStart(timer_ID_SMALLBLOCKOPEN13, 4.2 * sysClkRateGet(), (FUNCPTR)GATEOPEN1, 0);
	TimerCount_s2=3;
	break;
	}

	else if (TimerCount_s2 ==3)
	{
	
	currentstate2=0;
	res43 = wdStart(timer_ID_SMALLBLOCKCLOSE14, 2.5 * sysClkRateGet(), (FUNCPTR)GATECLOSE1, 0);	
	res53 = wdStart(timer_ID_SMALLBLOCKOPEN14, 4.2 * sysClkRateGet(), (FUNCPTR)GATEOPEN1, 0);
	TimerCount_s2=4;
	break;
	}

	else if (TimerCount_s2 ==4)
	{

	currentstate2=0;
	res44 = wdStart(timer_ID_SMALLBLOCKCLOSE15, 2.5 * sysClkRateGet(), (FUNCPTR)GATECLOSE1, 0);	
	res54 = wdStart(timer_ID_SMALLBLOCKOPEN15, 4.2 * sysClkRateGet(), (FUNCPTR)GATEOPEN1, 0);
	TimerCount_s2=0;
	break;
	}

case 2:
	if (TimerCount_sL2 ==0)
	{
	
	currentstate2=0;
	res60 = wdStart(timer_ID_LARGEBLOCKCOUNT11, 4.8 * sysClkRateGet(), (FUNCPTR)SEM_BELT1, 0);
	TimerCount_sL2 =1;
	setGates(0);
	break;
	}
	if (TimerCount_sL2 ==1)
	{
	
	currentstate2=0;
	res61 = wdStart(timer_ID_LARGEBLOCKCOUNT12, 4.8 * sysClkRateGet(), (FUNCPTR)SEM_BELT1, 0);
	setGates(0);
	TimerCount_sL2 =2;
	break;
	}
	
	if (TimerCount_sL2 ==2)
	{
	currentstate2=0;
	res62 = wdStart(timer_ID_LARGEBLOCKCOUNT13, 4.8 * sysClkRateGet(), (FUNCPTR)SEM_BELT1, 0);
	setGates(0);
	TimerCount_sL2 =0;
	break;
	}
default :
	break;

		}

}

}





              /* +------------------------------+ */
              /* ¦       SIZE PROCESSES         ¦ */
              /* +------------------------------+ */ 

void SizeDetection_Belt0 (void) {
		/***/
	int currentstate;

               int First_Reading0=0;
	int Second_Reading0=0;
	
	int res10,res11,res12,res13,res14;
	int res20,res21,res22,res23,res24 ;
	int res30,res31,res32;

	int TimerCount_s=0, TimerCount_s2L2=0;
	/*Create timers five different sets of timers*/
	/*SLBS2 SIZEDETECTION_BELT0 LARGE BLOCK SIZEDETECTION_BELT0 SMALL BLOCK CLOSE/OPEN TIMER*/
	timer_ID_SMALLBLOCKCLOSE01 = wdCreate();
	timer_ID_SMALLBLOCKOPEN01 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE02 = wdCreate();
	timer_ID_SMALLBLOCKOPEN02 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE03 = wdCreate();
	timer_ID_SMALLBLOCKOPEN03 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE04 = wdCreate();
	timer_ID_SMALLBLOCKOPEN04 = wdCreate();
	timer_ID_SMALLBLOCKCLOSE05 = wdCreate();
	timer_ID_SMALLBLOCKOPEN05 = wdCreate();



	timer_ID_LARGEBLOCKCOUNT01 = wdCreate();
	timer_ID_LARGEBLOCKCOUNT02 = wdCreate();
	timer_ID_LARGEBLOCKCOUNT03 = wdCreate();


	while(1)

		{
		clock_getres(CLOCK_REALTIME, &des);
		taskDelay(0.3 * sysClkRateGet());
		resetSizeSensors(0);
		First_Reading0 = Second_Reading0;
		Second_Reading0 = readSizeSensors(0);

		if (First_Reading0==1 && Second_Reading0==0)

			{ 
				k++;
				printf("Small Block BELT0\n");
				currentstate=1;

			}
		else if (First_Reading0==1 && Second_Reading0==3)
			{ 
				j++;
				printf("Large Block BELT0\n");
				currentstate=2;
				
			
			}

		
		
		                      /* +-------------------------+ */
				              /* ¦ GATE 1 & COUNT1 PROCESSES ¦    */
				              /* +-------------------------+ */ 
		
		
switch (currentstate)

{
case 1:
	if (TimerCount_s ==0)
	{
		currentstate=0;
	res10 = wdStart(timer_ID_SMALLBLOCKCLOSE01, 2.5 * sysClkRateGet(), (FUNCPTR) GATECLOSE0, 0);	
	res20 = wdStart(timer_ID_SMALLBLOCKOPEN01, 4.2 * sysClkRateGet(), (FUNCPTR) GATEOPEN0, 0);
	TimerCount_s =1;

	break;
	}

	else if (TimerCount_s ==1)
	{	
	currentstate=0;
	res11 = wdStart(timer_ID_SMALLBLOCKCLOSE02, 2.5 * sysClkRateGet(), (FUNCPTR) GATECLOSE0, 0);	
	res21 = wdStart(timer_ID_SMALLBLOCKOPEN02, 4.2 * sysClkRateGet(), (FUNCPTR) GATEOPEN0, 0);
	TimerCount_s =2;
	break;
	}

	else if (TimerCount_s ==2)
	{
		currentstate=0;
	res12 = wdStart(timer_ID_SMALLBLOCKCLOSE03, 2.5 * sysClkRateGet(), (FUNCPTR) GATECLOSE0, 0);	
	res22 = wdStart(timer_ID_SMALLBLOCKOPEN03, 4.2 * sysClkRateGet(), (FUNCPTR) GATEOPEN0, 0);
	TimerCount_s =3;
	break;
	}

	else if (TimerCount_s ==3)
	{
		currentstate=0;
	res13 = wdStart(timer_ID_SMALLBLOCKCLOSE04, 2.5 * sysClkRateGet(), (FUNCPTR) GATECLOSE0, 0);	
	res23 = wdStart(timer_ID_SMALLBLOCKOPEN04, 4.2 * sysClkRateGet(), (FUNCPTR) GATEOPEN0, 0);
	TimerCount_s =4;
	break;
	}

	else if (TimerCount_s ==4)
	{
	currentstate=0;
	res14 = wdStart(timer_ID_SMALLBLOCKCLOSE05, 2.5 * sysClkRateGet(), (FUNCPTR)GATECLOSE0, 0);	
	res14 = wdStart(timer_ID_SMALLBLOCKOPEN05, 4.2 * sysClkRateGet(), (FUNCPTR)GATEOPEN0, 0);
	TimerCount_s =0;
	break;
	}

case 2:
	if (TimerCount_s2L2 ==0)
	{
	currentstate=0;
	res30 = wdStart(timer_ID_LARGEBLOCKCOUNT01, 4.6 * sysClkRateGet(), (FUNCPTR) SEM_BELT0, 0);
	TimerCount_s2L2 =1;
	setGates(0);
	break;
	}

	if (TimerCount_s2L2 ==1)
	{
		currentstate=0;
	res31 = wdStart(timer_ID_LARGEBLOCKCOUNT02, 4.6 * sysClkRateGet(), (FUNCPTR) SEM_BELT0, 0);
	setGates(0);
	TimerCount_s2L2 =2;
	break;
	}
	
	if (TimerCount_s2L2 ==2)
	{
		currentstate=0;
	res32 = wdStart(timer_ID_LARGEBLOCKCOUNT03, 4.6 * sysClkRateGet(), (FUNCPTR) SEM_BELT0, 0);
	setGates(0);
	TimerCount_s2L2 =0;
	break;
	}
default :
	break;

		}

}
}

                      /* +----------------------------------------+ */
		              /* ¦         HARDWARE TEST PROCESSES        ¦ */
		              /* +----------------------------------------+ */ 



void Hardware(void) 

{
	int state_Size_TEST_2=0;
	int state_Size_TEST=0;
	char test='0';
	while (1) {
                test = getchar();                   /*take the USER_INTERFACE response from the user*/
                printf("\n PRESS :1 IF YOU WANT TO START TEST\n");
                if (test == '1')

                {
                		/*if 1 is pressed then suspend tasks - pause all the other tasks */
                        taskSuspend(taskID);
                        taskSuspend(taskID1);
                        taskSuspend(taskID3);
                        taskSuspend(taskID4);
                        printf("\nTHE SYSTEM GATES AND MOTORS WILL BE TESTED \n");

                        setGates(1);  
                        printf("\nGate of Belt 1 Close\n");
                        taskDelay(2 * sysClkRateGet());         		     /*2 seconds delay*/  
                        setGates(2);                                                 /*close the gate on belt 2*/
                        printf("\nGate of Belt 2 Close\n");
                        taskDelay(2 * sysClkRateGet());         		     /*2 seconds delay*/

                        setGates(0);                                                 /*close the gate on belt 1*/
                        printf("\n Both Gates open\n");
                        
                        taskDelay(2 * sysClkRateGet());         			/* 2 seconds delay*/
                        
                        stopMotor();                                                 /*start the motor*/
                        printf("\nMotor Start\n");
                        
                        
                        taskDelay(2 * sysClkRateGet());         			/* 2 seconds delay*/
                        stopMotor();                                                 /*stop the motor*/

                        printf("\nMotor Stopped\n");
                        taskDelay(2 * sysClkRateGet());        					 /* 2 seconds delay*/

                        startMotor();                                                /*start the motor*/

                        printf("\nMotor Start");
                        printf("\nPlease put a block on conveyor belt 0\n");



                  while (1)
                  {    
                	  resetSizeSensors(0);                                /*reset size SizeDetection_Belt0s of belt 1*/
                      state_Size_TEST = readSizeSensors(0);				/*read the size SizeDetection_Belt0s of belt 1 */
                      if (state_Size_TEST == 1)

                      	  {                                                        /*when size SizeDetection_Belt0 of belt 1 detec an object then escape the while loop*/
                    	  	  printf("\nSize Size Detection on conveyor belt 0 working.\n");
                    	  	  break;
                      	  }

                  }
			printf("Checking count Size Detection on conveyor belt 0\n");

			printf("\n");
			char CountSensor1HT;
			while (CountSensor1HT != 1) 

				{
					resetCountSensor(0); /*reset count SizeDetection_Belt0 */
					CountSensor1HT = readCountSensor(0);

				}
			printf("Count Size Detection on conveyor belt 0 working !\n");
            taskDelay(2 * sysClkRateGet());         		     /*2 seconds delay*/   
            printf("\nPlease put a block on conveyor belt 1");
       		while (1)
       		 	 {
                     resetSizeSensors(1);                                /*reset size SizeDetection_Belt0s of belt 2*/
                     state_Size_TEST_2 = readSizeSensors(1);                 /*read the size SizeDetection_Belt0s of belt 2 */
                     if (state_Size_TEST_2 == 1)

                     	 {                                                        /*when size SizeDetection_Belt0 of belt 2 detec an object then escape the while loop*/
                    	 	 printf("\n Size Size Detection on conveyor belt 1 working  \n");
                    	 	 break;
                     	 }

       		 	 }

            taskDelay(1 * sysClkRateGet());         		     /*2 seconds delay*/ 

            printf("\nChecking count Size Detection_Belt0 on conveyor belt 1  \n");
			char countSizeDetection_Belt1HT;
			while (countSizeDetection_Belt1HT != 1) 
				{

				resetCountSensor(1);													 /*reset count SizeDetection_Belt0 */
				countSizeDetection_Belt1HT = readCountSensor(1);

				}

			printf("Count Size Detection  on conveyor belt 1 working!\n");

            taskDelay(2 * sysClkRateGet());         /*add a 2 seconds delay*/
			printf("\n");
			
            printf("\nHarware testing completed");
            			resetSizeSensors(0);                                /*reset size SizeDetection_Belt0s of belt 1*/
            			resetSizeSensors(1);                                /*reset size SizeDetection_Belt0s of belt 2*/

            			/*Resume all the other task that have been suspend*/
            			USER_INTERFACE();
            			taskResume(taskID);
                        taskResume(taskID1);
                        taskResume(taskID3);
                        taskResume(taskID4);
                        
                       

}
        }

}



                      /* +-------------------------+ */
		              /* ¦          GUI            ¦ */
		              /* +-------------------------+ */ 

void USER_INTERFACE (void) {
	
	      printf("+------------------------------------------------+ \n ");
		  printf("| WELCOME TO JOHN AND MICAH'S CONVEYOR BELT      | \n" );
		  printf("|          757903        &        809056         | \n");
		  printf("+------------------------------------------------+ \n ");
		
		   printf("PRESS N : TO START THE MOTOR \n");
		   printf("PRESS M : TO STOP THE MOTOR \n"); 	
		   printf("PRESS Q : FOR Small Block(s) Dectected on BELT 0 \n");
		   printf("PRESS W : FOR LARGE Block(s) Dectected on BELT 0 \n");
		   printf("PRESS A : FOR Small Block(s) Dectected on BELT 1 \n");
		   printf("PRESS S : FOR LARGE Block(s) Dectected on BELT 1 \n");
		   printf("PRESS Z : FOR LARGE Block(s) Collected on BELT 0 \n");
		   printf("PRESS X : FOR LARGE Block(s) Collected on BELT 1 \n");
		   printf("PRESS E : TO FREE UP MEMORY USED/ SHUT DOWN \n");
		   printf("PRESS R : FOR HARDWARE TESTING \n");
		   printf("PRESS C : TO RESET ALL COUNTERS \n\n\n");
		   
		   printf("Enter character: \n");
		
		
		
    while (1) {
    	char buf;
        /* Read character from the standard USER_INTERFACE */
        buf = getchar();
        	switch(buf)
        		{
        	case 'N':
        		startMotor();
        	     break;
        	     
        	case 'M':
        		stopMotor();
        	    break;
        	    
        	    
        	case 'Q':
        		printf("%d Small Block(s) Dectected on BELT 0 \n",i);
        		break;
        	case 'W':
        		printf("%d LARGE Block(s) Dectected on BELT 0 \n",j);
        		break;
         	case 'A':
         		printf("%d Small Block(s) Dectected on BELT 1 \n",k);
         		break;
            case 'S':
            	printf("%d LARGE Block(s) Dectected on BELT 1 \n",l);
                break;
            case 'Z':
            	printf("%d LARGE Block(s) Collected on BELT 0 \n",Large0);
            	break;
            case 'X':
            	printf("%d LARGE Block(s) Collected on BELT 1 \n",Large1);
            	break;
            case 'C':
            	i=0;
            	j=0;
            	k=0;
            	l=0;
            	Large0=0;
            	Large1=0;
            	printf("ALL COUNTERS ARE EQUAL TO ZERO \n");
            	break;
            case 'R':
            	printf("\n\nHardware Testing...\n");
            	Hardware();
            	break;

            case 'E':
              	printf("\t\tSystem Shutting Down\n");
               	printf(".");
               	taskDelay(0.3 * sysClkRateGet());
               	printf(".");

               	taskDelay(0.3 * sysClkRateGet());
               	printf(".\n");
               	taskDelay(0.3 * sysClkRateGet());
                printf("\nShutdown Complete!\n");
                stopMotor();
             	
   
       /* DELETE ALL WATCHDOG TIMERS */
		wdDelete (timer_ID_SMALLBLOCKCLOSE11);
		wdDelete (timer_ID_SMALLBLOCKOPEN11);
		wdDelete (timer_ID_SMALLBLOCKCLOSE12);
		wdDelete (timer_ID_SMALLBLOCKOPEN12);
		wdDelete (timer_ID_SMALLBLOCKCLOSE13);
		wdDelete (timer_ID_SMALLBLOCKOPEN13);
		wdDelete (timer_ID_SMALLBLOCKCLOSE14);
		wdDelete (timer_ID_SMALLBLOCKOPEN14);
		wdDelete (timer_ID_SMALLBLOCKCLOSE15);
		wdDelete (timer_ID_SMALLBLOCKOPEN15);

		wdDelete (timer_ID_LARGEBLOCKCOUNT11);
		wdDelete (timer_ID_LARGEBLOCKCOUNT12);
		wdDelete (timer_ID_LARGEBLOCKCOUNT13);


		wdDelete (timer_ID_SMALLBLOCKCLOSE01);
		wdDelete (timer_ID_SMALLBLOCKOPEN01);
		wdDelete (timer_ID_SMALLBLOCKCLOSE02);
		wdDelete (timer_ID_SMALLBLOCKOPEN02);
		wdDelete (timer_ID_SMALLBLOCKCLOSE03);
		wdDelete (timer_ID_SMALLBLOCKOPEN03);
		wdDelete (timer_ID_SMALLBLOCKCLOSE04);
		wdDelete (timer_ID_SMALLBLOCKOPEN04);
		wdDelete (timer_ID_SMALLBLOCKCLOSE05);
		wdDelete (timer_ID_SMALLBLOCKOPEN05);


		wdDelete(timer_ID_LARGEBLOCKCOUNT01);
		wdDelete(timer_ID_LARGEBLOCKCOUNT02);
		wdDelete(timer_ID_LARGEBLOCKCOUNT03);
		
            	/* DELETE ALL SEMAPHORES */
            	semDelete(COUNT_BELT0);
               	semDelete(COUNT_BELT1);
               	/* DELETE ALL TASKS */
            	taskDelete(taskID);
            	taskDelete(taskID1);
            	taskDelete(taskID2);
            	taskDelete(taskID3);
            	taskDelete(taskID4);

            	exit(0);
            	printf(" Memory Freed  \n");

            	break;
        	default:
        		break;

        	}
    }

}

                      /* +-------------------------+ */
		              /* ¦            MAIN         ¦ */
		              /* +-------------------------+ */ 
int main() 

{
	
	
    startMotor();
    COUNT_BELT0 = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
    if (COUNT_BELT0 == NULL) 
    	{
   	       	printf("\n\nCannot create task release semaphore! Terminating task...\n");
   	       	exit(0);

    	}
    COUNT_BELT1 = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
      if (COUNT_BELT1 == NULL) 
      	{
    	  printf("\n\nCannot create task release semaphore! Terminating task...\n");
    	  exit(0);
      	}

            	
   printf("PRESS N : TO START THE MOTOR \n");
   printf("PRESS M : TO STOP THE MOTOR \n"); 	
   printf("PRESS Q : FOR Small Block(s) Dectected on BELT 0 \n");
   printf("PRESS W : FOR LARGE Block(s) Dectected on BELT 0 \n");
   printf("PRESS A : FOR Small Block(s) Dectected on BELT 1 \n");
   printf("PRESS S : FOR LARGE Block(s) Dectected on BELT 1 \n");
   printf("PRESS Z : FOR LARGE Block(s) Collected on BELT 0 \n");
   printf("PRESS X : FOR LARGE Block(s) Collected on BELT 1 \n");
   printf("PRESS E : TO FREE UP MEMORY USED/ SHUT DOWN \n");
   printf("PRESS R : FOR HARDWARE TESTING \n");
   printf("PRESS C : TO RESET ALL COUNTERS \n\n\n");



   /* Create tasks that executes function*/
    taskID = taskSpawn("Task COUNT0", 115, 0, 20000, (FUNCPTR) COUNT0, 0,0,0,0,0,0,0,0,0,0);
     taskID1 = taskSpawn("Task COUNT1", 120, 0, 20000, (FUNCPTR) COUNT1, 0,0,0,0,0,0,0,0,0,0);
     taskID2 = taskSpawn("USER_INTERFACE task", 130, 0, 20000, (FUNCPTR) USER_INTERFACE, 0,0,0,0,0,0,0,0,0,0);
     taskID3 = taskSpawn ("Read SizeDetection_Belt0 task", 100, 0, 20000, (FUNCPTR)SizeDetection_Belt0, 0,0,0,0,0,0,0,0,0,0);
     taskID4 = taskSpawn ("Read SizeDetection_Belt1 task B2", 110, 0, 20000, (FUNCPTR)SizeDetection_Belt1, 0,0,0,0,0,0,0,0,0,0);

}
