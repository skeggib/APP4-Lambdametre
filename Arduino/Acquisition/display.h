/**
 * Functions for displaying the results
 */

#ifndef HEADER_DISPLAY
#define HEADER_DISPLAY

void displayLambda(double lambda, LiquidCrystal lcd)
{
  // TODO remove for release
  Serial.print("Lambda = ");
  Serial.println(lambda);
  
  lcd.setCursor(0, 0);
  lcd.print("Lambda =");
  lcd.setCursor(0, 1);
  lcd.print(lambda);
}

void displayEfficiency(double efficiency, LiquidCrystal lcd)
{
  // TODO remove for release
  Serial.print("Efficiency = ");
  Serial.println(efficiency);
  
  lcd.setCursor(0, 0);
  lcd.print("Efficiency =");
  lcd.setCursor(0, 1);
  lcd.print(efficiency);
}

#endif
