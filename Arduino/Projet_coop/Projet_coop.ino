const int movementPin = 52;
const int fringePin = 50;
const int slotPin = 48;
const int usefulPin = 46;

bool fringeState = false;
int fringeCount = 0;
int maxFringeCount = 0;

bool slotState = false;
int slotCount = 0;
int maxSlotCount = 0;

bool usefulState = false;

enum State { Idle, Reading, Computing };
State state = Idle;

void setup() {
  Serial.begin(9600);
  
  pinMode(movementPin, INPUT);
  pinMode(fringePin, INPUT);
  pinMode(slotPin, INPUT);
  pinMode(usefulPin, INPUT);
  
  attachInterrupt(
    digitalPinToInterrupt(movementPin),
    interruptMovement,
    CHANGE);
    
  attachInterrupt(
    digitalPinToInterrupt(slotPin),
    interruptSlot,
    RISING);
    
  attachInterrupt(
    digitalPinToInterrupt(fringePin),
    interruptFringe,
    CHANGE);

  attachInterrupt(
    digitalPinToInterrupt(usefulPin),
    interruptUseful,
    CHANGE);
    
  attachInterrupt(
    digitalPinToInterrupt(usefulPin),
    interruptUsefulFalling,
    FALLING);
    
}

void loop() { 
  switch (state) {
    case Idle:
      //Serial.println("Idle");
      break;
    
    case Reading:
      //Serial.println("Reading");
      
      reading();
      
      break;
    
    case Computing:
    
      Serial.print("N_Fringes = ");
      Serial.print((double)maxFringeCount / 2.0);
      Serial.print(" Hz");
      Serial.println("");
      
      Serial.print("N_Slots = ");
      Serial.print((double)maxSlotCount / 2.0);
      Serial.print(" Hz");
      Serial.println("");
      
      fringeCount = 0;
      maxFringeCount = 0;
      slotCount = 0;
      maxSlotCount = 0;
      
      delay(50);
      state = Idle;
      break;
  }
}

void reading() {
  while (state == Reading) {
    if (usefulState) {
      if (digitalRead(fringePin) != fringeState) {
        fringeState = !fringeState;
        ++fringeCount;
      }
      if (digitalRead(slotPin) != slotState) {
        slotState = !slotState;
        ++slotCount;
      }
    }
  }
}

void interruptFringe() {
  if (state == Reading) ++fringeCount;
}

void interruptSlot() {
  state = state == Reading ? Computing : Reading;
}

void interruptMovement() {
  state = digitalRead(movementPin) == LOW ? Reading : Computing;
}

void interruptUseful() {
  usefulState = digitalRead(usefulPin) == HIGH ? true : false;
}

void interruptUsefulFalling() {
  if(fringeCount > maxFringeCount && slotCount > maxSlotCount)
  {
    maxFringeCount = fringeCount;
    maxSlotCount = slotCount;
  }
}

