/*
 * spi.c
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#include <msp430.h>
#include "spi.h"

//                   MSP430F525x
//                 -----------------
//                |                 |
//                |                 |
//                |             P3.3|-> Master Data Out (UCA0SIMO) -- SO
//                |                 |
//                |             P3.4|<- Master Data In (UCA0SOMI) -- nc
//                |                 |
//                |             P2.7|-> Serial Clock Out (UCA0CLK)



int init_spi() {
    int ret_code = 0;

    __disable_interrupt();

    // UCA0
    P3SEL |= BIT2 | BIT3 | BIT4;    // P3.3,4 option select
    P2SEL |= BIT7;                   // P2.7 option select

    P3DIR |= BIT4;
    P3DIR &= ~(BIT2+BIT3); // input
    P2DIR &= ~BIT7; // input

    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL__UCLK;
    UCA0BR0 = 20 ; // fSCL = SMCLK/ value
    UCA0BR1 = 0; //
    //UCA0MCTL = 0; // No modulation
    //UCCKPL
    //UCMODE_2
    UCA0CTL0 |= UCCKPH+UCSYNC+UCMSB;          // 3-pin, 8-bit SPI slave,
                                              // Clock polarity high, MSB
    //UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    UCA0CTL1 &= ~UCSWRST; // Release USCI state machine

    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt


    // UCB1
    P4DIR |= BIT2 ; // output
    P4DIR &=~(BIT0 | BIT1 | BIT3 ); // input

    UCB1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    //UCB1CTL1 |= UCSSEL__UCLK; // SMCLK
    //UCB1BR0 = 20; // /2
    //UCB1BR1 = 0; //
    //UCB1MCTL = 0; // No modulation

//#define UCCKPH                 (0x80)         /* Sync. Mode: Clock Phase */
//#define UCCKPL                 (0x40)         /* Sync. Mode: Clock Polarity */
//#define UCMST                  (0x08)         /* Sync. Mode: Master Select */
    UCB1CTL0 |= UCCKPH + UCSYNC + UCMSB;          // 3-pin, 8-bit SPI slave,
                                              // Clock polarity high, MSB
    UCB1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt


    __enable_interrupt();

    return ret_code;
}


