/**
 * Contants and functions for using the Arduino Timer/Counters
 */

#ifndef HEADER_TIMER_COUNTER
#define HEADER_TIMER_COUNTER

#include "pins.h"

// Bit numbers for the TC registers
const byte COVFS=0, LOVRS=1, CPAS=2, CPBS=3, CPCS=4, LDRAS=5, LDRBS=6, ETRGS=7,
           CLKSTA=16, MTIOA=17, MTIOB= 18; // TC_IERx and TC_SRx bits
const byte CLKEN=0, CLKDIS=1, WSTRG=2; // TC_CCRx bits
const byte TCCLKS=0, CLKI=3, BURST=4, LDBSTOP=6, LDBDIS=7, ETRGEDG=8, ABETRG=10,
           CPCTRG=14, WAVE=15, LDRA=16, LDRB=18; // TC_CMRx bits

void setup_tc_slot_useful()
{
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);
  REG_TC2_CCR0 = 0; // clear all bits in TC0_CCR0
  REG_TC2_CCR0 = 1 << CLKEN; // enable clock
  REG_TC2_CMR0 = 0; // capture mode, mclk/2, clk on rising edge
  // burst disabled, clk not disabled when loading RA/RB
  // capture mode
  REG_TC2_CMR0 = (
    (3 << ETRGEDG) | // Trigger on both edges of trigger input
    (1 << ABETRG) | // TIOA is used as the external trigger
    (3 << LDRA) | // Load RA on each edge of trigger input
    (5 << TCCLKS) // Use external clock XC0
  );
  REG_TC2_IER0 = 0; // disable TC0 interrupts
  REG_TC2_IER0 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC0 RA and RB capture interrupts
  REG_TC2_IMR0 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC0 capture interrupt mask
  REG_TC2_IDR0 = !((1 << LDRAS) | (1 << LDRBS)); // disable all other timer interrupts except capture
  NVIC_EnableIRQ(TC6_IRQn); // enable TC0 interrupts
  
  pinMode(PIN_TCLK0, INPUT);
  pinMode(PIN_TIOA6, INPUT);
}

/**
 * Enables the timer/counter TC0-1, the pin A5 is used as external clock and A7 as interrupt
 */
void setup_tc_fringe_slot()
{
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC1);
  REG_TC0_CCR1 = 0; // clear all bits in TC0_CCR1
  REG_TC0_CCR1 = 1 << CLKEN; // enable clock
  REG_TC0_CMR1 = 0; // capture mode, mclk/2, clk on rising edge
  // burst disabled, clk not disabled when loading RA/RB
  // capture mode
  REG_TC0_CMR1 = (
    (3 << ETRGEDG) | // Trigger on both edges of trigger input
    (1 << ABETRG) | // TIOA is used as the external trigger
    (3 << LDRA) | // Load RA on each edge of trigger input
    (6 << TCCLKS) // Use external clock XC1
  );
  REG_TC0_IER1 = 0; // disable TC1 interrupts
  REG_TC0_IER1 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC1 RA and RB capture interrupts
  REG_TC0_IMR1 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC1 capture interrupt mask
  REG_TC0_IDR1 = !((1 << LDRAS) | (1 << LDRBS)); // disable all other timer interrupts except capture
  NVIC_EnableIRQ(TC1_IRQn); // enable TC1 interrupts
  
  pinMode(PIN_TCLK1, INPUT);
  pinMode(PIN_TIOA1, INPUT);
}

void setup_tc_clock_slot()
{
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC7);
  REG_TC2_CCR1 = 0; // clear all bits in TC0_CCR1
  REG_TC2_CCR1 = 1 << CLKEN; // enable clock
  REG_TC2_CMR1 = 0; // capture mode, mclk/2, clk on rising edge
  // burst disabled, clk not disabled when loading RA/RB
  // capture mode
  REG_TC2_CMR1 = (
    (3 << ETRGEDG) | // Trigger on both edges of trigger input
    (1 << ABETRG) | // TIOA is used as the external trigger
    (3 << LDRA) | // Load RA on each edge of trigger input
    (2 << TCCLKS) // MCK/2
  );
  REG_TC2_IER1 = 0; // disable TC1 interrupts
  REG_TC2_IER1 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC1 RA and RB capture interrupts
  REG_TC2_IMR1 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC1 capture interrupt mask
  REG_TC2_IDR1 = !((1 << LDRAS) | (1 << LDRBS)); // disable all other timer interrupts except capture
  //NVIC_EnableIRQ(TC7_IRQn); // enable TC1 interrupts
  
  pinMode(PIN_TIOA7, INPUT);
}

#endif

