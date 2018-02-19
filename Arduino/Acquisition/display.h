/**
 * Functions for displaying the results
 */

#ifndef HEADER_DISPLAY
#define HEADER_DISPLAY

void displayLambda(double lambda, LiquidCrystal lcd)
{  
  lcd.setCursor(0, 0);
  lcd.print("Lambda =");
  lcd.setCursor(0, 1);
  lcd.print(lambda);
}

void displayEfficiency(double efficiency, LiquidCrystal lcd)
{  
  lcd.setCursor(0, 0);
  lcd.print("Efficiency =");
  lcd.setCursor(0, 1);
  lcd.print(efficiency);
}

#endif
