#include <LiquidCrystal.h>

#include "timers_counters.h"
#include "lambda.h"
#include "efficiency.h"
#include "display.h"
#include "pins.h"
#include "config.h"
#include "communication.h"

// --- GLOBAL VARIABLES

configuration Configuration;

double Lambda = -1; // Last computed lambda
unsigned int LastMaxFringeCount = 0;
unsigned int LastMaxSlotCount = 0;
double Efficiency = -1; // Last computed efficiency

unsigned long FringeCount = 0; // Number of fringes counted since the last "useful signal" raising edge
unsigned long SlotCount = 0; // Number of slots counted since the last "useful signal" raising edge

// The following variables are used to compute the lambda
unsigned long MaxFringeCount = 0; // Maximum fringes counted in one time since the last "movement signal" raising edge
unsigned long MaxSlotCount = 0; // Maximum slots counted in one time since the last "movement signal" raising edge

bool ReadEfficiency = true;
unsigned long TempSlotCountWhenUsefulHigh = 0;
unsigned long SlotCountWhenUsefulHigh = 0;
unsigned long SlotCountWhenUsefulLow = 0;

LiquidCrystal Lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

// --- SETUP AND LOOP

void setup()
{
  Serial.begin(9600);
  while (!Serial) { }
  
  Lcd.begin(16, 2);

  setup_TC0_0();
  setup_TC0_1();

  pinMode(PIN_MOVEMENT, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_MOVEMENT), movementInterrupt, CHANGE);
  
  pinMode(PIN_USEFUL, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_USEFUL), usefulInterrupt, CHANGE);

  configuration memConfig = readConfiguration();
  if (memConfig.check == 1)
    Configuration = memConfig;
}

void loop()
{
  if (Serial.available() > 0)
  {
    int received = Serial.read();
    switch (received)
    {
      case (int)'r':
        Serial.println(Lambda);
        break;
      case (int)'c':
        {
          float f = readSerialFloat();
          if (!isnan(f))
          {
            Configuration.slot_step = f;
            write_configuration(Configuration);
            Serial.println(Configuration.slot_step);
          }
        }
        break;
      case (int)'l':
        {
          float f = readSerialFloat();
          if (!isnan(f))
          {
            float slotStep = getStep(LastMaxFringeCount, LastMaxSlotCount, f);
            if (slotStep != -1)
              Configuration.slot_step = slotStep;
            write_configuration(Configuration);
            Serial.println(Configuration.slot_step);
          }
        }
        break;
      case (int)'v':
        Serial.println(Configuration.slot_step);
        break;
    }
  }
}

// --- HANDLERS AND INTERRUPTIONS

void TC0_Handler()
{
  TC_GetStatus(TC0, 0);
  bool isUsefulSignalHigh = !((REG_TC0_SR0 >> 17) & 0b1);

  if (isUsefulSignalHigh && ReadEfficiency)
  {
    ReadEfficiency = false;
    SlotCountWhenUsefulHigh = 0;
    SlotCountWhenUsefulLow = 0;
  }

  else if (isUsefulSignalHigh && !ReadEfficiency)
  {
    TempSlotCountWhenUsefulHigh = REG_TC0_RA0;
  }

  else if (!isUsefulSignalHigh && !ReadEfficiency)
  {
    SlotCountWhenUsefulHigh += TempSlotCountWhenUsefulHigh;
    SlotCountWhenUsefulLow += REG_TC0_RA0;
  }
}

/**
 * Called each time the state of the slot signal changes
 */
void TC1_Handler(){
  TC_GetStatus(TC0, 1); // get status, TC0 channel 1, allow the interrupt to fire again
  
  int fringePerSlotCount = REG_TC0_RA1; // get data from capture register A for TC0 channel 1
  FringeCount += fringePerSlotCount;
  ++SlotCount;
}

/**
 * Called each time the state of the movement signal changes
 */
void movementInterrupt()
{
  // Bloc oscillant en fin de course
  if(digitalRead(PIN_MOVEMENT) == LOW)
  {
    // On desactive les interruptions du TC
    NVIC_DisableIRQ(TC0_IRQn);
    NVIC_DisableIRQ(TC1_IRQn);

    // Calcul et affichage du lambda
    Lambda = getLambda(MaxFringeCount, MaxSlotCount, Configuration.slot_step);
    displayLambda(Lambda, Lcd);
    LastMaxFringeCount = MaxFringeCount;
    MaxFringeCount = 0;
    LastMaxSlotCount = MaxSlotCount;
    MaxSlotCount = 0;

    Efficiency = getEfficiency(SlotCountWhenUsefulHigh, SlotCountWhenUsefulLow);
    displayEfficiency(Efficiency, Lcd);
    ReadEfficiency = true;
  }

  // Bloc oscillant en mouvement
  else
  {
    // On active les interruptions du TC
    NVIC_EnableIRQ(TC0_IRQn);
    NVIC_EnableIRQ(TC1_IRQn);
  }
}

/**
 * On each falling edge of the "useful signal" we update the
 * maximum slot and fringe variables.
 * On each rising edge we reset the slot and fringe counters
 * to count only when the "useful signal" is high.
 */
void usefulInterrupt()
{
  if (digitalRead(PIN_USEFUL) == LOW)
  {
    if (SlotCount > MaxSlotCount)
    {
      MaxSlotCount = SlotCount;
      MaxFringeCount = FringeCount;
    }
  }

  else
  {
    SlotCount = 0;
    FringeCount = 0;
  }
}

