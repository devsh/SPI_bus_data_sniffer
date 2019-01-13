/**
 * main.c
 *
 *  Created on: 06.01.2019
 *  Author: Denis Shashunkin
 */


#include <msp430.h>
#include <stdio.h>
#include <string.h>


#include "power.h"
#include "clock.h"
#include "interrupts.h"
#include "uart.h"
#include "pio.h"
#include "led.h"
#include "button.h"
#include "spi.h"


char buff[1024];


int main(void)
{
    volatile int i;
    volatile int j;
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    //init_power();
    init_clocks();
    init_interrupts();
    init_leds();
    init_buttons();
    init_uarts();
    init_pio();
    init_spi();

    SendDbgMsg("Start ISTA sniffer application\r\n");

    if ( P1IN & CSN_CC1101 ) {
        SendDbgMsg("CSn Hi");
    } else {
        SendDbgMsg("CSn Lo");
    }

    if ( P1IN & GD00_CC1101 ) {
        SendDbgMsg("GDO0 Hi");
    } else {
        SendDbgMsg("GDO0 Lo");
    }

    if ( P1IN & GD02_CC1101 ) {
        SendDbgMsg("GDO2 Hi");
    } else {
        SendDbgMsg("GDO2 Lo");
    }


    for (;;) {
        // delay for a while
        //for (i = 0; i < 0xA000; i++) {
        //    for (j = 0; j < 0x4; j++);
        //}
        if ( i++ == 0xAA00) {

            //P4OUT ^= BIT6 ;
            i = 0;
            //toggle_led(&P4OUT, LED_RED);
            //toggle_led(&P4OUT, LED_GREEN);
            toggle_led(&P1OUT, LED_GREEN_SYSTEM);
        }

        if ( P1IN & CSN_CC1101 ) {
            //SendByte(0x7c);
        } else {
            SendByte(0x5f); // _
        }

        if (sem) {
            sem = 0;

            memset(buff, 0 , sizeof(buff));

            int index = 0;

            sprintf(buff,"\r\nM=%d [", master_n );

            for(index = 0; index < master_n; index++){
                sprintf(&buff[strlen(buff)]," %02x", master_buff[index]);
            }
            sprintf(&buff[strlen(buff)]," ]\r\n");

            sprintf(&buff[strlen(buff)],"S=%d [", slave_n);

            for(index = 0; index < slave_n; index++) {
                sprintf(&buff[strlen(buff)]," %02x",slave_buff[index]);
            }

            sprintf(&buff[strlen(buff)]," ]\r\n");


            master_n=0;
            slave_n=0;
            memset(master_buff, 0, 128);
            memset(slave_buff, 0, 128);

            P4OUT&=~LED_RED;
            P4OUT&=~LED_RED;

            SendDbgMsg(buff);
        }
/*
        if (indx == 2) {
            indx = 0;
            memset(spi_str, 0, 64);
            sprintf(spi_str, "rx: 0x%02x 0x%02x\r\n", spi_data[0], spi_data[1] );
            SendDbgMsg(spi_str);
        }
*/
    }

	return 0;
}


