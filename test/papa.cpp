#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

HardwareSerial miSerial(1);
DFRobotDFPlayerMini miPlayer;

// =============================
// BOTONES
// =============================

const int boton1 = 4;
const int boton2 = 5;
const int boton3 = 13;

// =============================
// FLAG
// =============================

volatile int flag = 1;

// =============================
// INTERRUPCIONES
// =============================

void IRAM_ATTR SONIDO() {
    flag = 1;
}

void IRAM_ATTR SONIDO2() {
    flag = 2;
}

void IRAM_ATTR SONIDO3() {
    flag = 3;
}

// =============================
// SETUP
// =============================

void setup() {

    Serial.begin(115200);

    // DFPlayer
    // RX ESP32 = GPIO18
    // TX ESP32 = GPIO19

    miSerial.begin(9600, SERIAL_8N1, 25, 26);

    Serial.println("Iniciando DFPlayer...");

    if (!miPlayer.begin(miSerial)) {

        Serial.println("No se encontro el DFPlayer.");
        Serial.println("Revisa conexiones y tarjeta microSD.");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("DFPlayer iniciado correctamente.");

    // Volumen: 0 - 30
    miPlayer.volume(25);

    // Botones
    pinMode(boton1, INPUT_PULLUP);
    pinMode(boton2, INPUT_PULLUP);
    pinMode(boton3, INPUT_PULLUP);

    // Interrupciones
    attachInterrupt(
        digitalPinToInterrupt(boton1),
        SONIDO,
        FALLING
    );

    attachInterrupt(
        digitalPinToInterrupt(boton2),
        SONIDO2,
        FALLING
    );

    attachInterrupt(
        digitalPinToInterrupt(boton3),
        SONIDO3,
        FALLING
    );
}

// =============================
// LOOP
// =============================

void loop() {

    if (flag != 0) {

        int pista = flag;

        

        miPlayer.playMp3Folder(pista);

        Serial.print("Reproduciendo pista: ");
        Serial.println(pista);

        delay(300);
    }
}