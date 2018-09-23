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

double Lambda = -1; // Dernier lambda calcule
unsigned int LastMaxFringeCount = 0;
unsigned int LastMaxSlotCount = 0;
double Efficiency = -1; // Derniere efficience calculee

unsigned long CurrentUsefulFringeCount = 0; // Nombre de franges comptee depuis le dernier front montant du signal utile
unsigned long CurrentUsefulSlotCount = 0; // Nombre de fentes comptee depuis le dernier front montant du signal utile

unsigned long MaxFringeCount = 0; // Maximum de franges comptee pendant une periode utile
unsigned long MaxSlotCount = 0; // Maximum de fentes comptee pendant une periode utile

bool ReadEfficiency = true; // Indique si l'efficience doit etre lue ou non
unsigned long TempSlotCountWhenUsefulHigh = 0;
unsigned long SlotCountWhenUsefulHigh = 0;
unsigned long SlotCountWhenUsefulLow = 0;

bool InMovement = true; // Indique si le bloc oscillant est en mouvement

// Utilise pour le calcul de la frequence des fentes et du mouvement du bloc oscillant
int SlotFreqIndex = 0;
long SlotFreqValues[3];

LiquidCrystal Lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

// --- SETUP AND LOOP

void setup()
{
  Serial.begin(9600);
  while (!Serial) { }
  
  Lcd.begin(16, 2);

  // Setup des timers/counters
  setup_tc_slot_useful();
  setup_tc_fringe_slot();
  setup_tc_clock_slot();
  
  pinMode(PIN_USEFUL, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_USEFUL), usefulInterrupt, CHANGE);

  // Lecture de la configuration depuis la memoire
  configuration memConfig = readConfiguration();
  if (memConfig.check == 1)
    Configuration = memConfig;
}

void loop()
{
  // Lecture de la frequence des fentes
  TC_GetStatus(TC2, 1);
  SlotFreqValues[SlotFreqIndex] = REG_TC2_RA1;
  ++SlotFreqIndex;
  SlotFreqIndex = SlotFreqIndex % 3;
  // Une moyenne glissante est faite sur les valeurs du TC des fentes
  int slotFreqMean = SlotFreqValues[0] + SlotFreqValues[1] + SlotFreqValues[2];
  // Seuil bas de la frequence, en dessous de cette frequence on considere que le bloc est en fin de course
  if (slotFreqMean > 160000 && InMovement)
  {
    InMovement = false;
    movementStoped();
  }
  // Seuil haut de la frequence, au dessus de cette frequence on considere que le bloc est en mouvement
  else if (slotFreqMean < 20000 && !InMovement)
  {
    InMovement = true;
    movementStarted();
  }

  // IO port serie
  if (Serial.available() > 0)
  {
    int received = Serial.read();
    switch (received)
    {
      // Send lambda
      case (int)'r':
        Serial.println(Lambda);
        break;
      // Send efficiency
      case (int)'e':
        Serial.println(Efficiency);
        break;
      // Get slot step
      case (int)'v':
        Serial.println(Configuration.slot_step);
        break;
      // Receive slot step
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
      // Receive target lambda
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
    }
  }
}

// --- HANDLERS ET INTERRUPTIONS

void TC6_Handler()
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

void TC1_Handler(){
  TC_GetStatus(TC0, 1); // get status, TC0 channel 1, allow the interrupt to fire again
  CurrentUsefulFringeCount += REG_TC0_RA1;
  ++CurrentUsefulSlotCount;
}

void movementStarted()
{
  //Serial.println("Start");
}

void movementStoped()
{
  //Serial.println("Stop");
  // Calcul et affichage du lambda
  
  if (CurrentUsefulSlotCount > MaxSlotCount)
  {
    MaxSlotCount = CurrentUsefulSlotCount;
    MaxFringeCount = CurrentUsefulFringeCount;
  }
  CurrentUsefulSlotCount = 0;
  CurrentUsefulFringeCount = 0;
  
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

void usefulInterrupt()
{
  if (digitalRead(PIN_USEFUL) == LOW)
  {
    if (CurrentUsefulSlotCount > MaxSlotCount)
    {
      MaxSlotCount = CurrentUsefulSlotCount;
      MaxFringeCount = CurrentUsefulFringeCount;
    }
  }

  else
  {
    CurrentUsefulSlotCount = 0;
    CurrentUsefulFringeCount = 0;
  }
}

