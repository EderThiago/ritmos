#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

// =====================================================
// HARDWARE SERIAL & DFPLAYER
// =====================================================
HardwareSerial miSerial(1);
DFRobotDFPlayerMini miPlayer;

// =====================================================
// LEDs (Pines estables sin tirones lógicos de arranque)
// =====================================================
const int leds[4] = {
  13,
  2, // Modificado en base a tu penúltima lista estable de LEDs
  27,
  5
};

// =====================================================
// BOTONES DEL JUEGO (Con resistencia PULL-UP interna)
// =====================================================
const int botones[4] = {
  33,
  4,
  14,
  15
};

// =====================================================
// BOTONES DE CONFIGURACIÓN
// =====================================================
const int PIN_INICIO     = 16; 
const int PIN_DIFICULTAD = 17; 

// =====================================================
// JUEGO CONFIGURACIÓN
// =====================================================
const int MAX_SECUENCIA = 30; 
int secuencia[MAX_SECUENCIA];
int nivel;

// Modos de dificultad: false = FÁCIL, true = DIFÍCIL
bool modoDificil = false; 

void iniciarJuego() {
  nivel = 1;

  // Primer elemento de la secuencia aleatoria
  secuencia[0] = random(0, 4);

  Serial.println("\n========================");
  Serial.print("  ¡PARTIDA INICIADA! (Modo ");
  Serial.print(modoDificil ? "DIFICIL" : "FACIL");
  Serial.println(")");
  Serial.println("========================");
  Serial.println("Preparate...");

  delay(1500);
}

// =====================================================
// MOSTRAR SECUENCIA
// =====================================================
void mostrarSecuencia() {
  Serial.println("\nMira la secuencia atentamente...");
  delay(500);

  for (int i = 0; i < nivel; i++) {
    int numero = secuencia[i];

    // Encender LED, informar en consola y reproducir pista asociada (1 a 4)
    digitalWrite(leds[numero], HIGH);
    Serial.print("-> LED [");
    Serial.print(numero + 1);
    Serial.println("] encendido");
    
    miPlayer.playMp3Folder(numero + 1); // Pistas 1, 2, 3 o 4 según el LED
    delay(500);

    // Apagar LED
    digitalWrite(leds[numero], LOW);
    delay(250);
  }
}

// =====================================================
// ESPERAR UN BOTÓN DEL JUEGO
// =====================================================
int esperarBoton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(botones[i]) == LOW) {
        delay(40); // Antirrebote
        while (digitalRead(botones[i]) == LOW) { delay(5); } // Esperar soltar
        return i;
      }
    }
  }
}

// =====================================================
// TURNO DEL JUGADOR
// =====================================================
bool jugadorRepiteSecuencia() {
  Serial.println("\n------------------------");
  Serial.println(" ¡Tu turno! Repeti la secuencia...");
  Serial.println("------------------------");

  for (int i = 0; i < nivel; i++) {
    int boton = esperarBoton();

    // Encender LED, informar en consola y sonar la misma pista del LED
    digitalWrite(leds[boton], HIGH);
    Serial.print("Apretaste boton [");
    Serial.print(boton + 1);
    Serial.println("]");
    
    miPlayer.playMp3Folder(boton + 1);
    delay(200);
    digitalWrite(leds[boton], LOW);

    if (boton != secuencia[i]) {
      return false;
    }
  }
  return true;
}

// =====================================================
// GAME OVER
// =====================================================
void perder() {
  Serial.println("\n************************");
  Serial.println("       GAME OVER        ");
  Serial.println("************************");
  Serial.print("Llegaste hasta el nivel: ");
  Serial.println(nivel);
  
  miPlayer.playMp3Folder(5); // Pista 5: Sonido de perder/Game Over
  
  // Parpadeo de advertencia de los 4 LEDs juntos
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 4; i++) { digitalWrite(leds[i], HIGH); }
    delay(250);
    for (int i = 0; i < 4; i++) { digitalWrite(leds[i], LOW); }
    delay(250);
  }
}

// =====================================================
// GANAR JUEGO (Nivel Máximo logrado)
// =====================================================
void ganar() {
  Serial.println("\n************************");
  Serial.println("   ¡GANASTE EL JUEGO!   ");
  Serial.println("************************");
  Serial.println("¡Alcanzaste el Nivel MAX!");

  miPlayer.playMp3Folder(6); // Pista 6: Sonido de victoria extendido

  // Animación de luces de victoria (Ida y vuelta)
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], HIGH); delay(100); digitalWrite(leds[i], LOW);
    }
    for (int i = 2; i >= 0; i--) {
      digitalWrite(leds[i], HIGH); delay(100); digitalWrite(leds[i], LOW);
    }
  }
}

// =====================================================
// MENU DE CONFIGURACIÓN ANTES DE EMPEZAR
// =====================================================
void menuConfiguracion() {
  Serial.println("\n==================================");
  Serial.println("          MENU DE INICIO          ");
  Serial.println("==================================");
  Serial.print("Dificultad actual: [ ");
  Serial.print(modoDificil ? "DIFICIL" : "FACIL");
  Serial.println(" ]");
  Serial.println("-> Presiona Pul. DIFICULTAD (GPIO 17) para cambiar.");
  Serial.println("-> Presiona Pul. INICIO (GPIO 16) para empezar a jugar.");
  Serial.println("----------------------------------");

  while (true) {
    // Detectar botón de cambio de dificultad
    if (digitalRead(PIN_DIFICULTAD) == LOW) {
      delay(40); // Antirrebote
      while (digitalRead(PIN_DIFICULTAD) == LOW) { delay(5); } // Esperar soltar
      
      modoDificil = !modoDificil; 
      
      Serial.print("Cambio de dificultad -> MODO: ");
      Serial.println(modoDificil ? "DIFICIL (Se añaden +2 LEDs)" : "FACIL (Se añade +1 LED)");
    }

    // Detectar botón de inicio de partida
    if (digitalRead(PIN_INICIO) == LOW) {
      delay(40); 
      while (digitalRead(PIN_INICIO) == LOW) { delay(5); } 
      
      break; 
    }
    delay(10);
  }
}

// =====================================================
// SETUP PRINCIPAL
// =====================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  // Comunicación serial con el DFPlayer (RX=25, TX=26 de la placa al módulo)
  miSerial.begin(9600, SERIAL_8N1, 25, 26);
  
  Serial.println("Iniciando DFPlayer...");
  if (!miPlayer.begin(miSerial)) {
    Serial.println("No se encontro el DFPlayer.");
    Serial.println("Verifica conexiones, alimentacion a 5V y tarjeta MicroSD.");
    while (true) { delay(1000); } 
  }
  Serial.println("DFPlayer iniciado correctamente.");
  miPlayer.volume(15); // Volumen seguro (0-30) para evitar picos de corriente excesivos

  // Configuración de pines de LEDs
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Configuración de pines de Botones de juego
  for (int i = 0; i < 4; i++) {
    pinMode(botones[i], INPUT_PULLUP);
  }

  // Configuración de botones de menú
  pinMode(PIN_INICIO, INPUT_PULLUP);
  pinMode(PIN_DIFICULTAD, INPUT_PULLUP);

  randomSeed(analogRead(34));

  // Desplegar menú inicial interactivo
  menuConfiguracion();
  iniciarJuego();
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================
void loop() {
  Serial.println("\n------------------------");
  Serial.print("NIVEL: ");
  Serial.println(nivel);
  Serial.println("------------------------");
  delay(1000);

  mostrarSecuencia();
  delay(500);

  bool correcto = jugadorRepiteSecuencia();

  if (correcto) {
    Serial.println("\n>>> ¡CORRECTO! <<<");
    delay(1000);

    if (nivel < MAX_SECUENCIA) {
      if (modoDificil) {
        // MODO DIFÍCIL: Añade dos pasos nuevos al patrón
        if (nivel + 2 <= MAX_SECUENCIA) {
          secuencia[nivel] = random(0, 4);
          secuencia[nivel + 1] = random(0, 4);
          nivel += 2;
          Serial.println("¡Secuencia acertada! Se sumaron +2 pasos al patrón.");
        } else {
          ganar();
          delay(2000);
          menuConfiguracion(); 
          iniciarJuego();
        }
      } else {
        // MODO FÁCIL: Añade un solo paso nuevo al patrón
        secuencia[nivel] = random(0, 4);
        nivel++;
        Serial.println("Pasando al siguiente nivel (+1 paso)...");
      }
      delay(1000);
    } else {
      ganar();
      delay(2000);
      menuConfiguracion();
      iniciarJuego();
    }
  } else {
    perder();
    delay(2000);
    menuConfiguracion(); 
    iniciarJuego();
  }
}