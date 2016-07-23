/*
 * system.c
 *
 *  Created on: Aug 28, 2015
 *      Author: Brad Torrence
 */

// Always include the header file inside the associated source file!
#include "system.h"

// Stellaris driver libraries
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"

/*************************************************************************************************
 *  FreeRTOS interrupt service routines
 *************************************************************************************************/
extern void xPortPendSVHandler();
extern void vPortSVCHandler();
extern void xPortSysTickHandler();

/*************************************************************************************************
 *	Initializes the system clock and registers the interrupt service routines
 ************************************************************************************************/
void systemInit() {
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    // Register the callback functions for FreeRTOS
    IntRegister(11, vPortSVCHandler);
    IntRegister(14, xPortPendSVHandler);
    SysTickIntRegister(xPortSysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

