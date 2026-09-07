#include <Arduino.h>
#include <LiquidCrystal.h>
#define led 17
// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(21, 22, 18, 19, 23, 5);

void setup() {
  // Inicializar LCD de 16 columnas x 2 filas
  lcd.begin(16, 2);
  

  // Primera fila
  lcd.setCursor(0, 0);
  lcd.print("Hola mundo!");
  
  // Segunda fila
  lcd.setCursor(0, 1);
  lcd.print("ESP32 + LCD");
}

void loop() {
  lcd.print("Hola mundo!");
  delay(1000);
}