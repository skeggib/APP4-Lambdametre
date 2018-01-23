const int movementPin = 52;
const int fringePin = 50;
const int slotPin = 48;
const int usefulPin = 46;

bool fringeState = false;
int fringeCount = 0;

bool slotState = false;
int slotCount = 0;

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
      Serial.print("rF = ");
      Serial.print((double)fringeCount / 2.0);
      Serial.print(" Hz");
      Serial.println("");
      
      fringeCount = 0;
      slotCount = 0;
      
      delay(50);
      state = Idle;
      break;
  }
}

void reading() {
  while (state == Reading) {
    if (digitalRead(usefulPin) == HIGH) {
      /*if (digitalRead(fringePin) != fringeState) {
        fringeState = !fringeState;
        ++fringeCount;
      }*/
  
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
  state = digitalRead(movementPin) == HIGH ? Reading : Computing;
}

