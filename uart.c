/*
 * uart.c
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#include <msp430.h>
#include <string.h>

#include "uart.h"


volatile int UartState;
unsigned char RxCmd ;
unsigned char TxCmd ;



int init_uarts() {
    int ret_code = 0;

    __disable_interrupt();

    // UART 0
    // pin 38 P3.4/UCA0RXD/UCA0SOMI
    // pin 37 P3.3/UCA0TXD/UCA0SIMO
    /*
    P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
    */

    // UART 1
    // pin 46 P4.5/PM_UCA1RXD/PM_UCA1SOMI
    // pin 45 P4.4/PM_UCA1TXD/PM_UCA1SIMO

    PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers
    // Map UART to appropriate pins
    //P3MAP4 = PM_UCA0SOMI;


    P4MAP4 = PM_UCA1TXD;
    P4MAP5 = PM_UCA1RXD;

    P4MAP0 = PM_UCA1STE ;
    P4MAP1 = PM_UCB1SIMO ;
    P4MAP2 = PM_UCB1SOMI ;
    P4MAP3 = PM_UCB1CLK ;


#if 1
    P4MAP6 = PM_NONE ; // green mayak
    P4MAP7 = PM_NONE ; // red ujuk
//    P4MAP1 = PM_NONE ; // reset
//    P4MAP2 = PM_NONE ; // on mayak
//    P4MAP3 = PM_NONE ; // on ujuk
#endif

    PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers

    P4SEL = BIT0 | BIT1 | BIT3 | BIT4 | BIT5; // P3.3,4 = USCI_A0 TXD/RXD

    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_2;                     // SMCLK
    UCA1BR0 = 0x55;                              // 1MHz 115200 (see User's Guide)
    UCA1BR1 = 0x08;                              // 1MHz 115200
    UCA1MCTL = 0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    __enable_interrupt();

    return ret_code;
}


void SendByte( unsigned char byte) {
    while ( !( UCA1IFG & UCTXIFG ) );
    UCA1TXBUF = byte;
}


void SendBuffer( unsigned char* buff, int len ) {
    int i =0;
    for ( i = 0 ; i < len ; i++ ) {
        SendByte( buff[ i ] );
    }
}


void SendDbgMsg( char* str ) {

    if ( !str ) {
        return ;
    }

    SendBuffer( (unsigned char*)str, strlen( str ) ) ;
    SendByte( 0xd ) ;
    SendByte( 0xa ) ;
}
