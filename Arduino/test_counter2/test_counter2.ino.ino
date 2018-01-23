volatile unsigned long CaptureCountA=0, CaptureCountB, TimerCount=0;
volatile bool CaptureFlag=0;

// define bit numbers for the TC registers we are using
const byte COVFS=0, LOVRS=1, CPAS=2, CPBS=3, CPCS=4, LDRAS=5, LDRBS=6, ETRGS=7,
           CLKSTA=16, MTIOA=17, MTIOB= 18; // TC_IERx and TC_SRx bits
const byte CLKEN=0, CLKDIS=1, WSTRG=2; // TC_CCRx bits
const byte TCCLKS=0, CLKI=3, BURST=4, LDBSTOP=6, LDBDIS=7, ETRGEDG=8, ABETRG=10,
           CPCTRG=14, WAVE=15, LDRA=16, LDRB=18; // TC_CMRx bits

void setup() {
  Serial.begin(9600); // initilize serial port to 9600 baud
  while (!Serial) { } // wait for serial port to initialize

  // setup TC0 bank, channel 1 (TC1) for input capture mode w/clk of 42MHz
  pmc_enable_periph_clk (ID_TC0); // enable clock the TC0 channeL 0, to access TC0_WPMR
  REG_TC0_WPMR = 0x54494D00; //WPKEY, disable
  pmc_set_writeprotect(false); // enable write to TC registers
  pmc_enable_periph_clk (ID_TC1); // enable clock the TC0 channel 1
  REG_TC0_CCR1 = 0; // clear all bits in TC0_CCR1
  REG_TC0_CCR1 = 1 << CLKEN; // enable clock
  REG_TC0_CMR1 = 0; // capture mode, mclk/2, clk on rising edge
  // burst disabled, clk not disabled when loading RA/RB
  // capture mode
  REG_TC0_CMR1=((3 << ETRGEDG) | // trigger on both edges of trigger input
                (1 << ABETRG) | // TIOA is used as the external trigger
                (1 << LDRA) | // load RA on rising edge of trigger input
                (2 << LDRB) |
                (6 << TCCLKS)
               ); // load RB on falling edge of trigger input
  REG_TC0_IER1 = 0; // disable TC1 interrupts
  REG_TC0_IER1 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC1 RA and RB capture interrupts
  REG_TC0_IMR1 = ((1 << LDRAS) | (1 << LDRBS)); // enable TC1 capture interrupt mask
  REG_TC0_IDR1 = !((1 << LDRAS) | (1 << LDRBS)); // disable all other timer interrupts except capture
  //NVIC_EnableIRQ(TC1_IRQn); // enable TC1 interrupts

  // print the startup header
  Serial.println("Timer Capture");

  // use the DUE to output a PWM frequency on pin 7, pin A7 input to the timer capture, just for testing
  pinMode(A7, INPUT); // TIOA1, input to TC1 for capture, A7 on DUE
  // analogWrite(7,127); // start PWM signal on pin 7, 50% duty cycle
  analogWrite(7,64); // start PWM signal on pin 7, 25% duty cycle
  // a jumper needs to be installed between pin 7 (PWM output) and A7 (TC0,1 TIOA pin)
}

void loop() {
  /*if (CaptureFlag) {
    CaptureFlag=0;
    Serial.print(CaptureCountA);
    Serial.print(", ");
    Serial.print(CaptureCountB);
    Serial.print(", ");
    Serial.println(TimerCount);
  }
  else {
    Serial.println(REG_TC0_CV1);
  }*/

  Serial.print("REG_TC0_RA1 = ");
  Serial.print(REG_TC0_RA1);
  Serial.print(", REG_TC0_RB1 = ");
  Serial.print(REG_TC0_RB1);
  Serial.print(", REG_TC0_CV1 = ");
  Serial.print(REG_TC0_CV1);
  Serial.println("");
}

/*void TC1_Handler(){
  TimerCount=REG_TC0_CV1; // save the timer counter register, for testing
  TC_GetStatus(TC0, 1); // get status, TC0 channel 1, allow the interrupt to fire again
  if (MTIOA) {
    CaptureCountA = REG_TC0_RA1; // get data from capture register A for TC0 channel 1
  }
  else {
    CaptureCountB = REG_TC0_RB1; // get data from caputre register B for TC0 channel 1
  }
  CaptureFlag=1; // set flag idicating a new capture value present
}*/
