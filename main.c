/********************************************************************************************
 *
 * 		Author: 		Alex Griffin
 *		Purpose:		Samples values from an infrared distance sensor, and graphs these samples to the OLED.
 *
 ********************************************************************************************/

// Project libraries
#include "src/system.h"
#include "src/oled.h"
#include "src/led.h"
#include "src/potentiometer.h"

//*************************************************************************************************
//	Main program to initialize hardware and execute Tasks.
//*************************************************************************************************
void main()  {
	// Create the tasks, the definitions are passed to the scheduler
	xTaskCreate(LEDTask, "LEDTask", 32, NULL, 0, NULL);
	xTaskCreate(OLEDTask, "OLEDTask", 512, NULL, 2, NULL);
	xTaskCreate(SamplingTask,"SamplingTask",32,NULL,2,NULL);
	//  Initialize system
	systemInit();

	// Display the initial image
	startScreen();

	// Start the FreeRTOS Scheduler
	vTaskStartScheduler();

	// The infinite loop is necessary for the FreeRTOS Scheduler
    while(true);
}

