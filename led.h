/*
 * led.h
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#ifndef LED_H_
#define LED_H_


#include <msp430.h>


// port 1
#define LED_GREEN_SYSTEM    BIT0

// port 4
#define LED_GREEN           BIT6
#define LED_RED             BIT7


int init_leds();
int toggle_led(volatile unsigned char* port, unsigned char bit);
int off_led(volatile unsigned char* port, unsigned char bit);


#endif /* LED_H_ */
