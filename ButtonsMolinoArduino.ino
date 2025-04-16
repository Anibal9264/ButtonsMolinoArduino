#include <Wire.h>
#include <Bounce2.h>

// Estado del sistema
int pinSeleccionado = -1;
int cantidadPresionados = 0;

// Pines de botones
const byte botones[] = {2, 3, 4, 5, 6, 7, 8};
const int numBotones = sizeof(botones) / sizeof(botones[0]);

// Pines de salida asociados a combinaciones
const byte pinesSalida[] = {2, 3, 8, 9, 10, 11, 12, 13, 99}; 
int estados[] =         { 1, 1, 1, 1,  1,  1,  1,  1,  1 };

// Objetos Bounce2 para el debounce
Bounce debouncers[numBotones];

void setup() {
  Serial.begin(9600);
  Wire.begin();

  for (int i = 0; i < numBotones; i++) {
    pinMode(botones[i], INPUT_PULLUP);
    debouncers[i].attach(botones[i]);
    debouncers[i].interval(25);
  }

  Serial.println("Sistema iniciado");
}

void loop() {
  bool presionado[numBotones];
  cantidadPresionados = 0;

  // Actualizar todos los botones
  for (int i = 0; i < numBotones; i++) {
    debouncers[i].update();
    presionado[i] = debouncers[i].read() == LOW;
    if (presionado[i]) cantidadPresionados++;
  }

  // Detectar combinaciones
  if (pinSeleccionado < 0) {
    pinSeleccionado = detectarCombinacion(presionado);
  }

  // Ejecutar acción si se soltaron los botones
  if (pinSeleccionado >= 0 && cantidadPresionados < 2) {
    ejecutarAccion(pinSeleccionado);
    reiniciarEstado();
  }

  delay(10);
}

int detectarCombinacion(bool p[]) {
  if (p[0] && p[1]) return 0;
  if (p[0] && p[2]) return 1;
  if (p[0] && p[3]) return 2;
  if (p[0] && p[4]) return 3;
  if (p[0] && p[5]) return 4;
  if (p[0] && p[6]) return 5;
  if (p[1] && p[2]) return 6;
  if (p[1] && p[3]) return 7;
  if (p[5] && p[6]) return 8;

  return -1;
}

void ejecutarAccion(int combinacion) {
  if (combinacion < 0 || combinacion >= sizeof(estados)) {
    Serial.println("Combinación inválida.");
    return;
  }

  estados[combinacion] = !estados[combinacion]; // Alternar estado
  byte pin = pinesSalida[combinacion];
  int estado = estados[combinacion];

  Wire.beginTransmission(1);
  Wire.write(pin);
  Wire.write(estado);

  if (Wire.endTransmission() != 0) {
    Serial.println("Error en la transmisión");
  }

  Serial.print("Pin: ");
  Serial.print(pin);
  Serial.print(" -> Estado: ");
  Serial.println(estado);
}

void reiniciarEstado() {
  pinSeleccionado = -1;
  cantidadPresionados = 0;
}
