/*
 * led.c
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#include <msp430.h>
#include "led.h"



int init_leds() {
    int ret_code = 0;

    P1DIR = LED_GREEN_SYSTEM;
    P4DIR = LED_RED | LED_GREEN;

    P1OUT = 0x00;
    P4OUT = 0x00 ;

    return ret_code;
}


int toggle_led(volatile unsigned char* port, unsigned char bit) {
    int ret_code = 0;

    *port ^= bit;

    return ret_code;
}

int off_led(volatile unsigned char* port, unsigned char bit) {
    int ret_code = 0;

    *port &= ~bit;

    return ret_code;
}
