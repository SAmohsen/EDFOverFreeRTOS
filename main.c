
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "semphr.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

/*Max Number For Adc Readings*/
#define MAX_NUMBER_OF_SAMPLES 10

/*Stubs For Reading ADC Channel*/
#define ADC_CHANNEL 2 
#define ADC_READ_CHANNEL(PIN) 10;{\
                        long int j;\
                         for(j = 0; j < 10000; j++)\
                            {}\
                         }

/*Stubs For Processing ADC Measurments*/
#define PROCESS_SAMPLES(ARR)   {\
                       long  int j;\
                         for(j = 0; j <55000; j++)\
                            {}\
                         }

												 
/*global Array To Hold Samples*/
unsigned long g_samples[MAX_NUMBER_OF_SAMPLES] ; 
unsigned long numOfSamples = 0 ; 

/*Periodic Task To Make Use Off Adc Readings*/
void vProcessingTask(void *pvParameters)
{
  TickType_t xLastWakeTime1 = xTaskGetTickCount();
	for (;;)
	{
			/*Process Samples To Control System Actuators*/
			PROCESS_SAMPLES(g_samples) ; 
		
			/*Debugging Task In Real Time*/
			GPIO_write(PORT_0,PIN2,PIN_IS_LOW);
			vTaskDelayUntil( &xLastWakeTime1, pdMS_TO_TICKS(35) );/*Delay 35 MS*/
		  GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
	}
}
/*Periodic Task To Sample Data From Sesnor*/
void vSamplingTask(void *pvParameters)
{
	/*E.t :1.343*/
	TickType_t xLastWakeTime1 = xTaskGetTickCount();
	for (;;)
	{
		 /*read Data From ADc Channel*/
		g_samples[numOfSamples] = ADC_READ_CHANNEL(ADC_CHANNEL) ;
		numOfSamples++;
		if(numOfSamples==MAX_NUMBER_OF_SAMPLES)
		{
			numOfSamples = 0 ; 
		}
		/*Debugging Task In Real Time*/
		  GPIO_write(PORT_0,PIN3,PIN_IS_LOW);
		  vTaskDelayUntil( &xLastWakeTime1,pdMS_TO_TICKS(15) ); /* Dealy 35 ms*/ 
			GPIO_write(PORT_0,PIN3,PIN_IS_HIGH);
	}
}
static void prvSetupHardware( void ) ; 

//extern int xTaskCreatePeriodic() ; 	
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

  /* Create Tasks here */
  xTaskCreatePeriodic(vProcessingTask,"Periodic Task To Make Use Off Adc Readings",200,NULL,1,NULL,35);
  xTaskCreatePeriodic(vSamplingTask,"Periodic Task To Sample Data From Sesnor",200,NULL,3,NULL,15);
	
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
