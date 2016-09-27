/*
 * oled.c
 *
 *  Created on: Aug 28, 2015
 *      Author: Alex Griffin
 * 
 */

// Header File
#include "oled.h"

// Base library
#include "system.h"

// Stellaris driver libraries
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// OLED driver library
#include "rit128x96x4.h" // This is a driver for the OLED display extension

// Standard C library
#include "stdio.h"

#include "potentiometer.h"

unsigned char lframe[94], rframe[94], hframe[64], blank[512], bar[512];

unsigned int i; //used for iteration throughout the program

void refreshScreen(){ //clears the entire OLED display, then puts on the axis's of the graph

	for(i = 0; i < 96; i++){
			RIT128x96x4ImageDraw(blank, 0, i, 128, 1);
	}

	RIT128x96x4ImageDraw(lframe, 8, 1, 2, 94);

	for(i=0;i<10;i++){
		RIT128x96x4ImageDraw(hframe, 0, (i*9 + 5), 128, 1);
	}

	RIT128x96x4ImageDraw(hframe, 0, 95, 128, 1);

}

void refreshScreen2(){

	int x = 0;
	int y = 0;

	for(y=0;y<11;y++){
		for(x=0;x<15;x++){
			RIT128x96x4ImageDraw(blank, x*8 + 10, 96-y*9, 6, 8);
	}
	}

}

// The function to generate and hold the initial start screen
void startScreen(){

    // The loop is used to initialize the arrays
    for(i = 0; i < 94; i++){
    	lframe[i] = 0xF0; 				// Vertical line for the left edge
    	rframe[i] = 0x0F; 				// Vertical line for the right edge
    	if(i < 64) hframe[i] = 0xFF; 	// Horizontal line for top and bottom
    }

    for(i=0;i<512;i++){
        bar[i] = 0xFF;
        blank[i] = 0x00;
    }


    //  Initialize the OLED display
    RIT128x96x4Init(1000000);
    RIT128x96x4ImageDraw(hframe, 0, 0, 128, 1);
    RIT128x96x4ImageDraw(hframe, 0, 95, 128, 1);
    RIT128x96x4ImageDraw(lframe, 0, 1, 2, 94);
    RIT128x96x4ImageDraw(rframe, 127, 1, 2, 94);
	RIT128x96x4StringDraw(" Press the", 32, 24, 15);
	RIT128x96x4StringDraw("\"Select\" Button", 16, 33, 15);
	RIT128x96x4StringDraw("To Continue", 32, 42, 15);

	// Enable the select button, wait for the press, then reset and disable the port
	// The button is disabled after the splash screen and the port is reset to default for future use.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_7); 	// Set the select button signal PG7 as input
	while(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_7));	// Wait until the signal changes (button pressed)
	SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOG);

	refreshScreen(); //after the button press, refresh the screen
}

// The OLED Task definition
void OLEDTask(void *pvParameters){


	unsigned int position = 10;
	unsigned int right = 1;
	while(true) {

/*
		refreshScreen2();

		RIT128x96x4ImageDraw(bar, position, 5, 6, 90);
		vTaskDelay(500*ONE_MS);

		if(right == 1){
			position = position + 8;
		}
		else{
			position = position - 8;
		}

		if(position < 10){
			right = 1;
			position = 10;
		}

		if(position == 122){
			right = 0;
		}
*/


		if(recieved == 1){ //if a new value has been found, update the display

			refreshScreen2();

			for(i = 0;i<15;i++){

				if(Queue[i] != 0){
					RIT128x96x4ImageDraw(bar, 122-i*8, 95-Queue[i]*9, 6, 9*Queue[i]);
				}
			}
			vTaskDelay(20*ONE_MS);

			recieved = 0;
		}
	}//endwhile
}

