#include <LiquidCrystal.h>
#include <Arduino.h>
// =====================================================
// LCD DR ROBOT
// =====================================================

// Shield D11 -> RS
// Shield D12 -> E
// Shield D0  -> D4
// Shield D1  -> D5
// Shield D2  -> D6
// Shield D3  -> D7

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


// =====================================================
// LEDs
// =====================================================

const int leds[4] = {
  25,
  26,
  27,
  32
};


// =====================================================
// BOTONES
// =====================================================

const int botones[4] = {
  33,
  21,
  4,
  13
};


// =====================================================
// JUEGO
// =====================================================

const int MAX_SECUENCIA = 10;

int secuencia[MAX_SECUENCIA];

int nivel;


void iniciarJuego() {

  nivel = 1;

  // Primer elemento de la secuencia
  secuencia[0] = random(0, 4);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Nuevo juego!");

  lcd.setCursor(0, 1);
  lcd.print("Preparate...");

  delay(1500);
}


// =====================================================
// MOSTRAR SECUENCIA
// =====================================================

void mostrarSecuencia() {

  for (int i = 0; i < nivel; i++) {

    int numero = secuencia[i];


    // Encender LED
    digitalWrite(leds[numero], HIGH);

    delay(500);


    // Apagar LED
    digitalWrite(leds[numero], LOW);

    delay(250);
  }
}


int esperarBoton() {

  while (true) {

    for (int i = 0; i < 4; i++) {

      // LOW = botón presionado
      if (digitalRead(botones[i]) == LOW) {

        // Antirrebote
        delay(40);


        // Esperar a que se suelte
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

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Tu turno!");

  lcd.setCursor(0, 1);
  lcd.print("Repite la sec.");

  for (int i = 0; i < nivel; i++) {

    // Esperar botón
    int boton = esperarBoton();


    // Mostrar qué botón pulsó
    digitalWrite(leds[boton], HIGH);

    delay(200);

    digitalWrite(leds[boton], LOW);


    // Comprobar
    if (boton != secuencia[i]) {

      return false;
    }
  }

  return true;
}


// =====================================================
// ESPERAR BOTÓN
// =====================================================




// =====================================================
// GAME OVER
// =====================================================

void perder() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   GAME OVER");

  lcd.setCursor(0, 1);
  lcd.print("Llegaste al ");
  lcd.print(nivel);


  // Parpadeo de los 4 LEDs
  for (int j = 0; j < 3; j++) {

    // Encender todos
    for (int i = 0; i < 4; i++) {

      digitalWrite(leds[i], HIGH);
    }

    delay(250);


    // Apagar todos
    for (int i = 0; i < 4; i++) {

      digitalWrite(leds[i], LOW);
    }

    delay(250);
  }
}


// =====================================================
// GANAR
// =====================================================

void ganar() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   GANASTE!");

  lcd.setCursor(0, 1);
  lcd.print("Nivel MAX!");


  // Animación de victoria
  for (int j = 0; j < 3; j++) {

    // Ida
    for (int i = 0; i < 4; i++) {

      digitalWrite(leds[i], HIGH);

      delay(100);

      digitalWrite(leds[i], LOW);
    }


    // Vuelta
    for (int i = 2; i >= 0; i--) {

      digitalWrite(leds[i], HIGH);

      delay(100);

      digitalWrite(leds[i], LOW);
    }
  }
}
// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();

  // LEDs
  for (int i = 0; i < 4; i++) {

    pinMode(leds[i], OUTPUT);

    digitalWrite(leds[i], LOW);
  }

  // Botones
  for (int i = 0; i < 4; i++) {

    pinMode(botones[i], INPUT_PULLUP);
  }

  // Semilla aleatoria
  randomSeed(analogRead(34));

  // Pantalla de inicio
  lcd.setCursor(0, 0);
  lcd.print("   SIMON GAME");

  lcd.setCursor(0, 1);
  lcd.print("   Preparado!");

  delay(2000);

  iniciarJuego();
}


// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  // Mostrar nivel
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Nivel: ");
  lcd.print(nivel);

  lcd.setCursor(0, 1);
  lcd.print("Mira la secuencia");

  delay(1500);


  // Mostrar secuencia
  mostrarSecuencia();


  delay(500);


  // Turno del jugador
  bool correcto = jugadorRepiteSecuencia();


  if (correcto) {

    // -----------------------------------------------
    // RESPUESTA CORRECTA
    // -----------------------------------------------

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("   CORRECTO!");

    lcd.setCursor(0, 1);
    lcd.print("Siguiente nivel");

    delay(1000);


    if (nivel < MAX_SECUENCIA) {

      // Agregar nuevo elemento
      secuencia[nivel] = random(0, 4);

      nivel++;

    } else {

      ganar();

      delay(2000);

      iniciarJuego();
    }

  } else {

    // -----------------------------------------------
    // RESPUESTA INCORRECTA
    // -----------------------------------------------

    perder();

    delay(2000);

    iniciarJuego();
  }
}


// =====================================================
// INICIAR JUEGO
// =====================================================

