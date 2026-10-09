#include <Arduino.h>

// =====================================================
// LEDs (Pines estables sin tirones lógicos de arranque)
// =====================================================
const int leds[4] = {
  13,
  25,
  27,
  32
};

// =====================================================
// BOTONES (Pines con resistencia PULL-UP interna real)
// =====================================================
const int botones[4] = {
  33,
  4,
  14,
  15
};

// =====================================================
// JUEGO CONFIGURACIÓN
// =====================================================
const int MAX_SECUENCIA = 10;
int secuencia[MAX_SECUENCIA];
int nivel;

void iniciarJuego() {
  nivel = 1;

  // Primer elemento de la secuencia aleatoria
  secuencia[0] = random(0, 4);

  Serial.println("\n========================");
  Serial.println("     ¡NUEVO JUEGO!      ");
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

    // Encender LED e informar en consola qué color/número se activa
    digitalWrite(leds[numero], HIGH);
    Serial.print("-> LED [");
    Serial.print(numero + 1);
    Serial.println("] encendido");
    delay(500);

    // Apagar LED
    digitalWrite(leds[numero], LOW);
    delay(250);
  }
}

// =====================================================
// ESPERAR BOTÓN
// =====================================================
int esperarBoton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      // LOW = botón presionado (gracias al INPUT_PULLUP)
      if (digitalRead(botones[i]) == LOW) {
        
        // Antirrebote básico
        delay(40);

        // Esperar a que el jugador suelte el botón
        while (digitalRead(botones[i]) == LOW) {
          delay(5);
        }

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
    // Esperar a que presione un botón
    int boton = esperarBoton();

    // Mostrar visualmente qué botón pulsó encendiendo su LED e informando en consola
    digitalWrite(leds[boton], HIGH);
    Serial.print("Apretaste boton [");
    Serial.print(boton + 1);
    Serial.println("]");
    delay(200);
    digitalWrite(leds[boton], LOW);

    // Comprobar si falló
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
  Serial.println("Reiniciando juego en 3 segundos...");

  // Parpadeo de advertencia de los 4 LEDs juntos
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

// =====================================================
// GANAR JUEGO (Nivel Máximo logrado)
// =====================================================
void ganar() {
  Serial.println("\n************************");
  Serial.println("   ¡GANASTE EL JUEGO!   ");
  Serial.println("************************");
  Serial.println("¡Alcanzaste el Nivel MAX!");
  Serial.println("Reiniciando juego...");

  // Animación de luces de victoria (Ida y vuelta)
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

// =====================================================
// SETUP PRINCIPAL
// =====================================================
void setup() {
  // Inicialización del Monitor Serie a alta velocidad
  Serial.begin(115200);
  while (!Serial) {
    ; // Esperar a que se conecte el puerto serie (solo necesario en algunas placas nativas)
  }

  Serial.println("\n==================================");
  Serial.println("      INICIANDO SIMON GAME        ");
  Serial.println("==================================");

  // Configuración de pines de LEDs
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Configuración de pines de Botones
  for (int i = 0; i < 4; i++) {
    pinMode(botones[i], INPUT_PULLUP);
  }

  // Ruido del pin analógico 34 para generar una semilla realmente aleatoria
  randomSeed(analogRead(34));

  Serial.println("¡Preparado!");
  delay(2000);

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

  // Muestra la secuencia de luces y logs
  mostrarSecuencia();
  delay(500);

  // Evalúa la respuesta del jugador
  bool correcto = jugadorRepiteSecuencia();

  if (correcto) {
    Serial.println("\n>>> ¡CORRECTO! <<<");
    Serial.println("Pasando al siguiente nivel...");
    delay(1500);

    if (nivel < MAX_SECUENCIA) {
      // Agrega un nuevo paso aleatorio al Simón Dice
      secuencia[nivel] = random(0, 4);
      nivel++;
    } else {
      ganar();
      delay(2000);
      iniciarJuego();
    }
  } else {
    perder();
    delay(2000);
    iniciarJuego();
  }
}
