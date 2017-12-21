void setup() {
  Serial.begin(9600);
  Btmr_setup();
  pinMode(22, INPUT);
  //pio_TCLK1();
}

void loop() {
  Serial.print(TC0->TC_CHANNEL[0].TC_RA);
  Serial.print(" ");
  Serial.print(TC0->TC_CHANNEL[0].TC_RB);
  Serial.println();
}

void Btmr_setup() {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(TC_INTERFACE_ID + 0 * 3 + 0);
  //uint32_t reg_val = TC_BMR_TC1XC1S_TCLK1;
  //TC0->TC_BMR |= reg_val;

  TcChannel *t = &(TC0->TC_CHANNEL)[0];
  t->TC_CCR = TC_CCR_CLKDIS; // Disable clock ?
  t->TC_IDR = 0xFFFFFFFF;
  t->TC_SR;
  t->TC_CMR = TC_CMR_TCCLKS_XC1
             | TC_CMR_LDRA_RISING
             | TC_CMR_LDRB_FALLING
             | TC_CMR_ABETRG
             | TC_CMR_ETRGEDG_FALLING;

  t->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  t->TC_IER =  TC_IER_LDRBS;
  t->TC_IDR = ~TC_IER_LDRBS;

  NVIC_DisableIRQ(TC1_IRQn);
  NVIC_ClearPendingIRQ(TC1_IRQn);
  NVIC_SetPriority(TC1_IRQn, 0);
  NVIC_EnableIRQ(TC1_IRQn);
}

void pio_TCLK1() 
{
  PIOA->PIO_PDR = PIO_PA4A_TCLK1;
  PIOA->PIO_IDR = PIO_PA4A_TCLK1;
}
