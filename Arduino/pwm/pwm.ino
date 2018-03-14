// Bit numbers for the TC registers
const byte COVFS=0, LOVRS=1, CPAS=2, CPBS=3, CPCS=4, LDRAS=5, LDRBS=6, ETRGS=7,
           CLKSTA=16, MTIOA=17, MTIOB= 18; // TC_IERx and TC_SRx bits
const byte CLKEN=0, CLKDIS=1, SWTRG=2; // TC_CCRx bits

// TC_CMRx bits capture mode
const byte CMR_CAP_TCCLKS  =  0,
           CMR_CAP_CLKI    =  3,
           CMR_CAP_BURST   =  4,
           CMR_CAP_LDBSTOP =  6,
           CMR_CAP_LDBDIS  =  7,
           CMR_CAP_ETRGEDG =  8,
           CMR_CAP_ABETRG  = 10,
           CMR_CAP_CPCTRG  = 14,
           CMR_CAP_WAVE    = 15,
           CMR_CAP_LDRA    = 16,
           CMR_CAP_LDRB    = 18;

// TC_CMRx bits waveform mode
const byte CMR_WAV_TCCLKS  =  0,
           CMR_WAV_CLKI    =  3,
           CMR_WAV_BURST   =  4,
           CMR_WAV_CPCSTOP =  6,
           CMR_WAV_CPCDIS  =  7,
           CMR_WAV_EEVTEDG =  8,
           CMR_WAV_EEVT    = 10,
           CMR_WAV_ENETRG  = 12,
           CMR_WAV_WAVSEL  = 13,
           CMR_WAV_WAVE    = 15,
           CMR_WAV_ACPA    = 16,
           CMR_WAV_ACPC    = 18,
           CMR_WAV_AEEVT   = 20,
           CMR_WAV_ASWTRG  = 22,
           CMR_WAV_BCPB    = 24,
           CMR_WAV_BCPC    = 26,
           CMR_WAV_BEEVT   = 28,
           CMR_WAV_BSWTRG  = 30;

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  setPwm0(2.4, 0.2);
}

void setPwm0(double frequency, double duty)
{
  pinMode(2, OUTPUT);
  analogWrite(2, 26);
  
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC0);
  
  double mck_freq = 84E6;
  double mck_divider = (double)(pow(4, (REG_TC0_CMR0 >> CMR_WAV_TCCLKS) & 0b111) * 2);

  REG_TC0_RC0 = (uint32_t)((mck_freq / mck_divider) / frequency);
  REG_TC0_RA0 = (double)REG_TC0_RC0 * duty;
}

void loop() {
}
