/*
 * pio.c
 *
 *  Created on: 07.01.2019
 *  Author: Denis Shashunkin
 */

#include "pio.h"
#include "uart.h"
#include "led.h"
#include "interrupts.h"

#include <string.h>

volatile int sem = 0;

unsigned char ucb1RxByte = 0;
unsigned char uca0RxByte = 0;
unsigned char uca1RxByte = 0;
unsigned char pin_state;


volatile int master_n = 0;
volatile int slave_n = 0;
volatile unsigned char master_buff[128];
volatile unsigned char slave_buff[128];


int i = 0 ;

int init_interrupts() {
    int ret_code = 0;

    master_n=0;
    slave_n=0;
    memset(master_buff, 0, sizeof(master_buff));
    memset(slave_buff, 0, sizeof(slave_buff));

    return ret_code;
}


#pragma vector=PORT1_VECTOR

__interrupt void port1_ISR(void) {

    switch( P1IFG & 0xff ) {
        case BIT1:
            //P1IFG &= ~BIT1;    // обнуляем флаг прерывания для P1.3
            P4OUT ^= BIT7 ;
            P4OUT ^= BIT6 ;
            P1IE |= BIT1; // Разрешение прерываний на P1.2
            P1IFG &= ~BIT1; // Очистка флага прерываний для P1.2
            SendDbgMsg("Button ISR\r\n");

        case CSN_CC1101:


            //SendDbgMsg("CSn ISR\r\n");
            //while (!(UCB1IFG&UCTXIFG));

            //while(UCA0STAT & UCBUSY);//while (!(IFG2 & UCA0RXIFG));
            //SendByte(0x5f);
            //SendByte(0x7c); // |

            sem = 1;
            //SendByte(0x50); // P

            P1IFG &= ~CSN_CC1101;

            break;

        case GD00_CC1101:
            P1IFG &= ~GD00_CC1101;
            //SendDbgMsg("GDO0 ISR\r\n");
            break;

        case GD02_CC1101:
            P1IFG &= ~GD02_CC1101;
            //SendDbgMsg("GDO2 ISR\r\n");
            break;

        default:
            P1IFG = 0;
    }
}

// UCA0
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV,4))
    {
        case 0:
            break;                             // Vector 0 - no interrupt

        case 2:                                // Vector 2 - RXIFG
            //while ( !( UCA0IFG & UCTXIFG ) ) ; // USCI_A0 TX buffer ready?

            //uca0RxByte = UCA0RXBUF ;
            //SendByte(uca0RxByte);

            //SendByte(0x53); // S
            slave_buff[slave_n++] = UCA0RXBUF;
            //slave_n++;
            //P4OUT ^= LED_RED;
            //UCA0IFG &= ~UCRXIFG;

            break;

        case 4:
            break;                             // Vector 4 - TXIFG

        default:
            break;
    }

}


// UCA1
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA1IV,4))
    {
        case 0:
            break;                             // Vector 0 - no interrupt

        case 2:                                   // Vector 2 - RXIFG
            //while ( !( UCA1IFG & UCTXIFG ) ) ;             // USCI_A0 TX buffer ready?

            //uca1RxByte = UCA1RXBUF ;

            break;

        case 4:
            break;                             // Vector 4 - TXIFG

        default:
            break;
    }

}

unsigned char spi_data[2];
char spi_str[64];

volatile int indx = 0;
// I -0x49
// O - 4F

#pragma vector= USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {

    switch (__even_in_range(UCB1IV, 4)) {
        //Vector 2 - RXIFG
    case USCI_UCRXIFG:
            //while (!(UCB1IFG & UCTXIE)) ;
            //spi_data[indx++] = UCB1RXBUF;
            //while (!(UCB1IFG&UCTXIFG));
            //while(P1IN & CSN_CC1101);                     //Wait till SO = low
            //SendByte(UCB1RXBUF);
            //UCB1IFG &= ~UCRXIFG;
            //SendByte(0x5f); // _
            //SendByte(0x53); // S

            //SendByte(0x4D); // M
            master_buff[master_n++] = UCB1RXBUF;
            //while( !(IFG2 & UCA0RXIFG) );
            //master_n++;

            //P4OUT ^= LED_GREEN;

            //UCB1IFG &= ~UCRXIFG;


            break;

        default:
            break;
    }
}
