// ================================
// SIMON - ESP32
// ================================
#include <Arduino.h>
// LEDs
const int leds[4] = {
    16, 17, 18, 19
  };
  
  // Pulsadores
  const int botones[4] = {
    25, 26, 27, 32
  };
  
  // Máxima longitud de la secuencia
  const int MAX_SECUENCIA = 10;
  
  int secuencia[MAX_SECUENCIA];
  
  int nivel = 1;
  
  void iniciarJuego() {
  
    nivel = 1;
  
    // Primer elemento aleatorio
    secuencia[0] = random(0, 4);
  
    Serial.println();
    Serial.println("Nuevo juego");
  
    delay(1000);
  }
  
  
  // --------------------------------
  // MOSTRAR SECUENCIA
  // --------------------------------
  void mostrarSecuencia() {
  
    Serial.println("Mostrando secuencia...");
  
    for (int i = 0; i < nivel; i++) {
  
      int led = secuencia[i];
  
      digitalWrite(leds[led], HIGH);
  
      delay(500);
  
      digitalWrite(leds[led], LOW);
  
      delay(250);
    }
  }
  
  int esperarBoton() {
  
    while (true) {
  
      for (int i = 0; i < 4; i++) {
  
        // INPUT_PULLUP:
        // LOW = botón presionado
        if (digitalRead(botones[i]) == LOW) {
  
          // Pequeño debounce
          delay(30);
  
          // Esperar a que se suelte
          while (digitalRead(botones[i]) == LOW) {
            delay(5);
          }
  
          return i;
        }
      }
    }
  }
  // --------------------------------
  // JUGADOR REPITE LA SECUENCIA
  // --------------------------------
  bool jugadorRepiteSecuencia() {
  
    Serial.println("Tu turno!");
  
    for (int i = 0; i < nivel; i++) {
  
      int boton = esperarBoton();
  
      Serial.print("Boton: ");
      Serial.println(boton + 1);
  
      // Encender el LED correspondiente
      digitalWrite(leds[boton], HIGH);
      delay(250);
      digitalWrite(leds[boton], LOW);
  
      // Comprobar
      if (boton != secuencia[i]) {
  
        return false;
      }
    }
  
    return true;
  }
  
  
  // --------------------------------
  // ESPERAR BOTÓN
  // --------------------------------
  
  
  
  // --------------------------------
  // PERDER
  // --------------------------------
  void perder() {
  
    Serial.println("GAME OVER");
  
    // Todos los LEDs parpadean
    for (int j = 0; j < 3; j++) {
  
      for (int i = 0; i < 4; i++) {
        digitalWrite(leds[i], HIGH);
      }
  
      delay(250);
  
      for (int i = 0; i < 4; i++) {
        digitalWrite(leds[i], LOW);
      }
  
      delay(250);
    }
  }
  
  
  // --------------------------------
  // CELEBRACIÓN
  // --------------------------------
  void celebrarVictoria() {
  
    for (int j = 0; j < 3; j++) {
  
      for (int i = 0; i < 4; i++) {
  
        digitalWrite(leds[i], HIGH);
        delay(100);
        digitalWrite(leds[i], LOW);
      }
  
      for (int i = 2; i >= 0; i--) {
  
        digitalWrite(leds[i], HIGH);
        delay(100);
        digitalWrite(leds[i], LOW);
      }
    }
  }
  
  // --------------------------------
  // SETUP
  // --------------------------------
  void setup() {
  
    Serial.begin(115200);
  
    // Configurar LEDs
    for (int i = 0; i < 4; i++) {
      pinMode(leds[i], OUTPUT);
      digitalWrite(leds[i], LOW);
    }
  
    // Configurar botones
    // INPUT_PULLUP = botón conectado a GND
    for (int i = 0; i < 4; i++) {
      pinMode(botones[i], INPUT_PULLUP);
    }
  
    // Semilla aleatoria
    randomSeed(analogRead(34));
  
    Serial.println("=== SIMON ESP32 ===");
    Serial.println("Comenzando...");
  
    delay(1000);
  
    iniciarJuego();
  }
  
  
  // --------------------------------
  // LOOP
  // --------------------------------
  void loop() {
  
    // Mostrar secuencia
    mostrarSecuencia();
  
    // Esperar a que el jugador la repita
    if (jugadorRepiteSecuencia()) {
  
      Serial.println("¡CORRECTO!");
  
      // Agregar un nuevo elemento
      if (nivel < MAX_SECUENCIA) {
        secuencia[nivel] = random(0, 4);
        nivel++;
  
        Serial.print("Nivel: ");
        Serial.println(nivel);
  
        delay(700);
      }
      else {
        Serial.println("¡GANASTE EL JUEGO!");
  
        celebrarVictoria();
  
        delay(2000);
        iniciarJuego();
      }
  
    } else {
  
      Serial.println("¡ERROR!");
  
      perder();
  
      delay(1500);
  
      iniciarJuego();
    }
  }
  
  
  // --------------------------------
  // INICIAR JUEGO
  // --------------------------------
  