#include <Wire.h>

int ps = -1;
static int two = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP); 
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP); 
  pinMode(6, INPUT_PULLUP); 
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
}

byte pin[] = {2, 3, 8, 9, 10, 11, 12, 13, 99};
int estados[] = {1,1,1,1,1,1,1,1,1};

void loop() {
  bool p1 = digitalRead(2) == 0; // 2 I/O 
  bool p2 = digitalRead(3) == 0; // 3 I/O
  bool p3 = digitalRead(4) == 0; // 4 I/O
  bool p4 = digitalRead(5) == 0; // 5 I/O
  bool p5 = digitalRead(6) == 0; // 6 I/O
  bool p6 = digitalRead(7) == 0; // 7 I/O
  bool p7 = digitalRead(8) == 0; // 8 I/O
  
  two = p1 + p2 + p3 + p4 + p5 + p6 + p7; 

  if (ps < 0) {
    if (p1 && p2) ps = 0;
    else if (p1 && p3) ps = 1;
    else if (p1 && p4) ps = 2;
    else if (p1 && p5) ps = 3;
    else if (p1 && p6) ps = 4;
    else if (p1 && p7) ps = 5;
    else if (p2 && p3) ps = 6;
    else if (p2 && p4) ps = 7;
    else if (p6 && p7) ps = 8;
  }
  
  if (ps >= 0 && two < 2) {
	selectPin(ps);
    resetState();
  }
  //Serial.println(String(two));
  //Serial.println(String(p1)+String(p2)+String(p3)+String(p4)+String(p5));
  delay(100);
}

void selectPin(int p) {
  int estado = estados[p] == 0 ? 1 : 0;
  estados[p] = estado;
  transmitir(pin[p], estado);
}

void transmitir(byte pin, int est) {
  Wire.beginTransmission(1);
  Wire.write(pin);
  Wire.write(est);
  
  if (Wire.endTransmission() != 0) {
    Serial.println("Error en la transmisión");
  }
  
  Serial.println("Pin: " + String(pin) + " Estado: " + String(est));
}

void resetState() {
  ps = -1;
  two = 0;
}