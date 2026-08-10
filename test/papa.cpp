#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

HardwareSerial miSerial(1);
DFRobotDFPlayerMini miPlayer;

const int boton1 = 4;
const int boton2 = 5;
const int boton3 = 6;

volatile int flag = 1;

void IRAM_ATTR SONIDO() {
flag = 1;
}

void IRAM_ATTR SONIDO2() {
flag = 2;
}

void IRAM_ATTR SONIDO3() {
flag = 3;
}

void setup() {

Serial.begin(115200);

// UART para DFPlayer
// RX ESP32-S3 = GPIO16
// TX ESP32-S3 = GPIO17
miSerial.begin(9600, SERIAL_8N1, 16, 17);

Serial.println("Iniciando DFPlayer...");

if (!miPlayer.begin(miSerial)) {
Serial.println("No se encontro el DFPlayer.");
Serial.println("Revisa conexiones y tarjeta microSD.");

while (true) {
delay(1000);
}
}

Serial.println("DFPlayer iniciado correctamente.");

// Volumen: 0 a 30
miPlayer.volume(25);

pinMode(boton1, INPUT_PULLDOWN);
pinMode(boton2, INPUT_PULLDOWN);
pinMode(boton3, INPUT_PULLDOWN);

attachInterrupt(digitalPinToInterrupt(boton1), SONIDO, FALLING);
attachInterrupt(digitalPinToInterrupt(boton2), SONIDO2, FALLING);
attachInterrupt(digitalPinToInterrupt(boton3), SONIDO3, FALLING);
}

void loop() {

if (flag != 0) {

int pista = flag;

flag = 0;

miPlayer.playMp3Folder(pista);

Serial.print("Reproduciendo pista: ");
Serial.println(pista);

delay(300);
}
}