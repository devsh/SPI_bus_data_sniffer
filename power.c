/*
 * power.c
 *
 *  Created on: 07.01.2019
 *  Author: Denis Shashunkin
 */


#include "power.h"


int init_power() {
    int ret_code = 0;

    return ret_code;
}


void SetVCoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE | SVSHRVL0 * level | SVMHE | SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE | SVMLE | SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG | SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE | SVSLRVL0 * level | SVMLE | SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}
