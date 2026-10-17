#include <LiquidCrystal.h> 
#include <Arduino.h>


  
  
  LiquidCrystal lcd(13, 12, 23, 27, 26, 25); 
 
  // Definir el pin donde está conectado el buzzer
const int led=18; 
const int led2=19;

void setup() {
  lcd.begin(16, 2);
  pinMode(led, OUTPUT); 
  pinMode(led2, OUTPUT);
  lcd.print("pijajajajajajajjajajajsixseveeeen");
}

void loop() {
  digitalWrite(led, HIGH); 
  digitalWrite(led2, HIGH);
}
  
  
  