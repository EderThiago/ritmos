#include <Arduino.h>
#include <LiquidCrystal.h> // Incluimos la librería para el LCD sin I2C

// =====================================================
// LCD 1602 (Pines asignados en modo 4 bits)
// Pines LCD:     RS  E   D4  D5  D6  D7
// =====================================================
LiquidCrystal lcd(13, 12, 23, 27, 26, 25); 


// =====================================================
// LEDs (Ajustados para no colisionar con el LCD)
// Cambiados el 13 y 27 por los pines seguros 18 y 19
// =====================================================
const int leds[4] = {
  19, // Cambiado (antes 13, usado por el LCD)
  18,
  5, // Cambiado (antes 27, usado por el LCD)
  2
};

// =====================================================
// BOTONES DEL JUEGO (Con resistencia PULL-UP interna)
// =====================================================
const int botones[4] = {
  15,
  4,
  33,
  14
};

// =====================================================
// BOTONES DE CONFIGURACIÓN
// =====================================================
const int PIN_INICIO     = 16; 
const int PIN_DIFICULTAD = 17; 
const int pinBuzzer = 32;
const int frec[4] = {262, 523, 1047, 2093}; 

// =====================================================
// JUEGO CONFIGURACIÓN
// =====================================================
const int MAX_SECUENCIA = 30; 
int secuencia[MAX_SECUENCIA];
int nivel;

// Modos de dificultad: false = FÁCIL, true = DIFÍCIL
bool modoDificil = false; 

// =====================================================
// FUNCIÓN PARA IMPRIMIR EN SERIAL Y LCD SIMULTÁNEAMENTE
// =====================================================
void mostrarMensaje(String linea1, String linea2) {
  // Imprimir en el Monitor Serial
  Serial.println("-----------------------");
  Serial.println("LCD L1: " + linea1);
  Serial.println("LCD L2: " + linea2);
  
  // Imprimir en la Pantalla LCD
  lcd.clear();             
  lcd.setCursor(0, 0);     
  lcd.print(linea1);
  lcd.setCursor(0, 1);     
  lcd.print(linea2);
}

void iniciarJuego() {
  nivel = 1;
  secuencia[0] = random(0, 4);

  String difStr = modoDificil ? "DIFICIL" : "FACIL";
  mostrarMensaje("PARTIDA INICIADA", "Modo: " + difStr);
  delay(1500);
  
  mostrarMensaje("Preparate...", "");
  delay(1500);
}

// =====================================================
// MOSTRAR SECUENCIA
// =====================================================
void mostrarSecuencia() {
  mostrarMensaje("Mira la secuencia", "atentamente...");
  delay(1000);

  for (int i = 0; i < nivel; i++) {
    int numero = secuencia[i];

    // Encender LED e informar en consola/LCD
    digitalWrite(leds[numero], HIGH);
    tone(pinBuzzer, frec[numero], 500); 
    
    mostrarMensaje("Mostrando...", "LED: [" + String(numero + 1) + "]");
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
  mostrarMensaje("¡Tu turno!", "Repeti secuencia");
  delay(500);

  for (int i = 0; i < nivel; i++) {
    int boton = esperarBoton();

    digitalWrite(leds[boton], HIGH);
    tone(pinBuzzer, frec[boton], 500);
    
    mostrarMensaje("Apretaste:", "Boton [" + String(boton + 1) + "]");
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
  mostrarMensaje("   GAME OVER    ", "Nivel logrado: " + String(nivel));
  
  // Parpadeo de advertencia de los 4 LEDs juntos
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 4; i++) { digitalWrite(leds[i], HIGH); }
    tone(pinBuzzer, 150, 500);
    delay(250);
    for (int i = 0; i < 4; i++) { digitalWrite(leds[i], LOW); }
    delay(250);
  }
}

// =====================================================
// GANAR JUEGO (Nivel Máximo logrado)
// =====================================================
void ganar() {
  mostrarMensaje("¡GANASTE EL JUEGO!", "¡Nivel MAX logrado!");

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
  String difStr = modoDificil ? "DIFICIL" : "FACIL";
  mostrarMensaje(" MENU DE INICIO ", "Dificultad: " + difStr);

  // Bucle infinito esperando que se decida iniciar el juego
  while (true) {
    // Detectar botón de cambio de dificultad
    if (digitalRead(PIN_DIFICULTAD) == LOW) {
      delay(40); // Antirrebote
      while (digitalRead(PIN_DIFICULTAD) == LOW) { delay(5); } // Esperar soltar
      
      modoDificil = !modoDificil; // Alternar dificultad
      difStr = modoDificil ? "DIFICIL" : "FACIL";
      
      mostrarMensaje("Cambio Modo: ", "-> " + difStr);
      delay(1000); // Pequeña pausa para que se lea el cambio
      mostrarMensaje(" MENU DE INICIO ", "Dificultad: " + difStr);
    }

    // Detectar botón de inicio de partida
    if (digitalRead(PIN_INICIO) == LOW) {
      delay(40); // Antirrebote
      while (digitalRead(PIN_INICIO) == LOW) { delay(5); } // Esperar soltar
      break; // Salir del menú e ir al juego
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

  // Inicializar el LCD (16 columnas y 2 filas)
  lcd.begin(16, 2); 
  mostrarMensaje("Iniciando hardware", "Cargando...");

  // Configuración de pines de LEDs
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Configuración de pines de Botones de juego
  for (int i = 0; i < 4; i++) {
    pinMode(botones[i], INPUT_PULLUP);
  }

  // Configuración de nuevos botones de menú
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
  mostrarMensaje("NIVEL JUEGO:", ">> " + String(nivel) + " <<");
  delay(1000);

  mostrarSecuencia();
  delay(500);

  bool correcto = jugadorRepiteSecuencia();

  if (correcto) {
    mostrarMensaje(">>> ¡CORRECTO! <<<", "Siguiente nivel");
    delay(1000);

    if (nivel < MAX_SECUENCIA) {
      if (modoDificil) {
        if (nivel + 2 <= MAX_SECUENCIA) {
          secuencia[nivel] = random(0, 4);
          secuencia[nivel + 1] = random(0, 4);
          nivel += 2;
          mostrarMensaje("Modo Dificil!", "Sumados +2 pasos");
        } else {
          ganar();
          delay(2000);
          menuConfiguracion(); 
          iniciarJuego();
        }
      } else {
        secuencia[nivel] = random(0, 4);
        nivel++;
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
