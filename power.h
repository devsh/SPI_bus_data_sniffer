/*
 * power.h
 *
 *  Created on: 07.01.2019
 *  Author: Denis Shashunkin
 */

#ifndef POWER_H_
#define POWER_H_


#include <msp430.h>


int init_power();
void SetVCoreUp (unsigned int level);


#endif /* POWER_H_ */
