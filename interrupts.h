/*
 * interrupts.h
 *
 *  Created on: 07.01.2019
 *  Author: Denis Shashunkin
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <msp430.h>


extern volatile unsigned char master_buff[];
extern volatile unsigned char slave_buff[];
extern volatile int master_n, slave_n, sem;

extern unsigned char spi_data[2];
extern char spi_str[64];

extern volatile int indx;

extern int init_interrupts();

#endif /* INTERRUPTS_H_ */
