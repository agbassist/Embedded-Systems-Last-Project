/*
 * potentiometer.c
 *
 *  Created on: Feb 24, 2016
 *      Author: agriffin
 */

#include "potentiometer.h"


// Stellaris driver libraries
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "driverlib/adc.h"

unsigned long average = 0; //used to calulate and store the average of sample values

unsigned long sample; //used to temporarily store the sample from the ADC

int recieved = 0; //communicates with OLED task to let it know the queue has changed

unsigned int Queue[15]; //up to 15 values can be stored in the queue

unsigned int currentPosition = 0; //stores the position to update in the "queue"

void SamplingTask(void *pvParameters){

	//off-board circuit power source initialization
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x01);

	//ADC initialization
	SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 );
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0,ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC0_BASE, 0);

	//clears the interrupt
	ADCIntClear(ADC0_BASE,0);

	unsigned long i = 0; //used for iteration

	unsigned long numSamples = 50.0; //used to determine the number of samples

	for(i=0;i<15;i++){ //initialize values in the queue
		Queue[i] = 0;
	}

	while(true){

		recieved = 0; //reset this value

		average = 0;

		for(i = 0; i < numSamples; i ++){ //take 50 values and average them because the sensor is pretty unstable

			//Trigger the sample sequence.
			ADCProcessorTrigger(ADC0_BASE, 0);

			// Wait until the sample sequence has completed.
			while(!ADCIntStatus(ADC0_BASE, 0, false)){}

			// Read the value from the ADC.
			ADCSequenceDataGet(ADC0_BASE, 0, &sample);

			ADCIntClear(ADC0_BASE,0); //clears the interrupt

			average += sample;

			vTaskDelay(50*ONE_MS/numSamples); //take numSamples samples in 0.2 seconds
		}

		average = average / numSamples;

		for(i=14;i>0;i=i-1){

			Queue[i] = Queue[i-1]; //shifts the values of the "queue"
		}

		Queue[0] = (unsigned int)((average / 100)); //converts the long value to an int between 0 and 10

		recieved = 1; //tell the other tasks that there is a new average

		while(recieved == 1){} //wait until this value has been used
	}

}
