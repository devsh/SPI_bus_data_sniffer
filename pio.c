/*
 * pio.c
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#include <msp430.h>
#include "pio.h"


int init_pio() {
    int ret_code = 0;

    __disable_interrupt();
    P1DIR &= ~(GD00_CC1101 | GD02_CC1101 | CSN_CC1101); // set as input
    P1REN |= GD00_CC1101 | GD02_CC1101 | CSN_CC1101;
    P1REN |= GD00_CC1101 | GD02_CC1101 | CSN_CC1101; // P1.3 Resistor enabled
    //P1OUT &= ~CSN_CC1101; // Resistor as pull-down
    P1OUT |= CSN_CC1101; // Resistor as pull-up
    //P1OUT &= ~(GD00_CC1101 + GD02_CC1101); // Resistor as pull-down

    // 0: PxIFG is set on low to high transition
    // 1: PxIFG is set on high to low transition
    //P1IES &= ~(GD00_CC1101 + GD02_CC1101); // Lo->Hi edge selected
    P1IES |= CSN_CC1101; // Hi->Lo edge selected
    //P1IES &= ~CSN_CC1101; // Lo->Hi edge selected

    // clear any pending IRQ
    // 0: no interrupt pending
    // 1: interrupt pending
    P1IFG &= ~(GD00_CC1101 | GD02_CC1101 | CSN_CC1101); // IFG cleared

    // interrupts enabled
    // 0: interrupt disabled
    // 1: interrupt enabled
    P1IE |= (GD00_CC1101 | GD02_CC1101 | CSN_CC1101) ;

    __enable_interrupt();

    return ret_code;
}


