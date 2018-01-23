#include "tc.h"

const byte movementPin = 50;

bool SlotStateChanged = 0;

unsigned long FringeCount = 0;
unsigned long SlotCount = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial) { }

  //setup_TC0_1();

  pinMode(movementPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(movementPin), movementInterrupt, HIGH);
}

void loop()
{
  Serial.println(digitalRead(movementPin));
  if (SlotStateChanged) {
    SlotStateChanged = false;
  }
}

void TC1_Handler(){
  TC_GetStatus(TC0, 1); // get status, TC0 channel 1, allow the interrupt to fire again
  
  int fringePerSlotCount = REG_TC0_RA1; // get data from capture register A for TC0 channel 1
  FringeCount += fringePerSlotCount;
  ++SlotCount;
  
  SlotStateChanged = true; // set flag idicating a new capture value present
}

double getLambda(unsigned long fringeCount, unsigned long slotCount)
{
  return 0;
}

void movementInterrupt()
{
  Serial.println(digitalRead(movementPin));
  return;
  if(digitalRead(movementPin) == LOW)
  {
    NVIC_DisableIRQ(TC1_IRQn);
  
    double lambda = getLambda(FringeCount, SlotCount);
    Serial.print("Lambda = ");
    Serial.print(lambda);
    Serial.println("");
  }
  else
  {
    NVIC_EnableIRQ(TC1_IRQn);
  }
}

