/*
 * clock.c
 *
 *  Created on: 07.01.2019
 *  Author: Denis Shashunkin
 */


#include "clock.h"

#include "power.h"

// 20.48MHz


int init_clocks() {
    int ret_code = 0;

    // first disable XT1
    // todo

    // Power settings
    SetVCoreUp(1u);
    SetVCoreUp(2u);
    SetVCoreUp(3u);

    UCSCTL3 = SELREF__REFOCLK;    // select REFO as FLL source
    UCSCTL6 = XT1OFF | XT2OFF;    // turn off XT1 and XT2

    P5SEL |= BIT2+BIT3;                       // Port select XT2

    //  PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers
    //  P4MAP7 = PM_MCLK;
    //  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers

    #if 0
    P1DIR |= BIT0;                            // ACLK set out to pins
    P1SEL |= BIT0;

    P6DIR |= BIT0;                            // SMCLK set out to pins
    P6SEL |= BIT0;
    #endif

    //  P4DIR |= BIT7;                            // MCLK set out to pins
    //  P4SEL |= BIT7;

    UCSCTL6 &= ~XT2OFF;                       // Enable XT2
    //   UCSCTL6 |= XCAP_3;                        // Internal load cap
    UCSCTL3 |= SELREF_2;                      // FLLref = REFO
                                        // Since LFXT1 is not used,
                                        // sourcing FLL with LFXT1 can cause
                                        // XT1OFFG flag to set
    UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO
    //UCSCTL6 |= XT2DRIVE_2;
    // Loop until XT1,XT2 & DCO stabilizes
    do {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                       // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    } while (SFRIFG1&OFIFG);                   // Test oscillator fault flag


    UCSCTL5 = 0;  //all divider =1

    UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
    UCSCTL6 |= XT2DRIVE_2;                                // expected frequency
    UCSCTL4 = SELS_5 + SELM_5 + SELA_5;               // SMCLK=MCLK=XT2

    return ret_code;
}


