#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial dfSerial(2);
DFRobotDFPlayerMini dfPlayer;

const int boton1 = 4;
const int boton2 = 5;
const int boton3 = 13; // Cambiado desde el GPIO6 (el 6 también es de la memoria Flash)

volatile int flag = 1;
int ultimaPista = -1;

void IRAM_ATTR SONIDO() { flag = 1; }
void IRAM_ATTR SONIDO2() { flag = 2; }
void IRAM_ATTR SONIDO3() { flag = 3; }

void setup() {
  Serial.begin(115200);

  // PINES LIBRES DE CONFLICТО: RX = GPIO25, TX = GPIO26
  // Conecta RX2 (25) al TX del DFPlayer
  // Conecta TX2 (26) al RX del DFPlayer (con resistencia de 1k en medio)
  dfSerial.begin(9600, SERIAL_8N1, 25, 26);

  Serial.println("Iniciando DFPlayer Mini...");

  // Desactivamos el ACK obligatorio y el Reset de la librería para evitar que trabe el Watchdog
  if (!dfPlayer.begin(dfSerial, false, false)) { 
    Serial.println("Error: Verificá conexiones de 25/26 y tarjeta SD.");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("DFPlayer Mini conectado exitosamente.");
  dfPlayer.volume(20);

  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(boton3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(boton1), SONIDO, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton2), SONIDO2, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton3), SONIDO3, FALLING);
}

void loop() {
  
    //gigfigfikgigjgj
    Serial.print("Reproduciendo pista: ");
    Serial.println(flag);
    
    dfPlayer.playMp3Folder(flag);
  

  delay(3000); 
}