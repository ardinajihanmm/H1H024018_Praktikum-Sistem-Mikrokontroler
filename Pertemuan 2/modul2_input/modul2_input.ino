#include <Arduino.h>

// mapping pin: a b c d e f g dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};

// Array pola Hexadecimal (1 = ON, 0 = OFF)
// Urutan index: {a, b, c, d, e, f, g}
byte hexPatterns[16][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}, // 9
  {1, 1, 1, 0, 1, 1, 1}, // A
  {0, 0, 1, 1, 1, 1, 1}, // b
  {1, 0, 0, 1, 1, 1, 0}, // C
  {0, 1, 1, 1, 1, 0, 1}, // d
  {1, 0, 0, 1, 1, 1, 1}, // E
  {1, 0, 0, 0, 1, 1, 1}  // F
};

void setup() {
  // Inisialisasi semua pin sebagai OUTPUT
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinDP,OUTPUT);
}

void loop() {
  for (int i = 0; i < 16; i++) {
    displayHex(i);
    delay(1000); // Berganti setiap 1 detik
  }
}

void displayHex(int num) {
  digitalWrite(pinA, hexPatterns[num][0] ? LOW : HIGH);
  digitalWrite(pinB, hexPatterns[num][1] ? LOW : HIGH);
  digitalWrite(pinC, hexPatterns[num][2] ? LOW : HIGH);
  digitalWrite(pinD, hexPatterns[num][3] ? LOW : HIGH);
  digitalWrite(pinE, hexPatterns[num][4] ? LOW : HIGH);
  digitalWrite(pinF, hexPatterns[num][5] ? LOW : HIGH);
  digitalWrite(pinG, hexPatterns[num][6] ? LOW : HIGH);

  // Matikan titik (DP)
  digitalWrite(pinDP, HIGH); 
}