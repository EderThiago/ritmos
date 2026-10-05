#include <LiquidCrystal.h>
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

HardwareSerial miSerial(1);
DFRobotDFPlayerMini miPlayer;
// =====================================================
// LCD DR ROBOT
// =====================================================

// Shield D11 -> RS
// Shield D12 -> E
// Shield D0  -> D4
// Shield D1  -> D5
// Shield D2  -> D6
// Shield D3  -> D7
//RW EN GND LA PUTA MADREEEEEEEE QUEE LO RE MIL PARIOOOOOOO
//USAR GND DE MEDIO OYEEEEE MAC
const int LCD_RS = 23;
const int LCD_E  = 22;
const int LCD_D4 = 16;
const int LCD_D5 = 17;
const int LCD_D6 = 18;
const int LCD_D7 = 19;

LiquidCrystal lcd(
  LCD_RS,
  LCD_E,
  LCD_D4,
  LCD_D5,
  LCD_D6,
  LCD_D7
);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("   SIMON GAME");

  lcd.setCursor(0, 1);
  lcd.print("   Preparado!");

  delay(2000);

  
}
void loop() {
  lcd.setCursor(0, 1);
  lcd.print("   Preparado!");
}
