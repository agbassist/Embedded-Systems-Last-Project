/*
 * potentiometer.h
 *
 *  Created on: Feb 24, 2016
 *      Author: Alex Griffin
 */

#include "system.h"

#ifndef SRC_POTENTIOMETER_H_
#define SRC_POTENTIOMETER_H_

extern unsigned int Queue[15]; //declare global queue, implemented as an array, to transfer data

extern int recieved;

extern void SamplingTask(void*);

#endif /* SRC_POTENTIOMETER_H_ */
