#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // pin analog input
const int servoPin = 9;       // pin digital PWM untuk servo

// ===================== VARIABEL =====================
int pos = 0; // sudut servo (0–180)
int val = 0; // nilai ADC (0–1023)

void setup() {

  // Hubungkan servo ke pin
  myservo.attach(servoPin);

  // Aktifkan serial monitor
  Serial.begin(9600);
}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  val = analogRead(potensioPin);

  // ===================== KONVERSI DATA =====================
  pos = map(val,
            0,     // minimum ADC
            1023,  // maksimum ADC
            0,     // sudut minimum
            180);  // sudut maksimum

  // ===================== OUTPUT SERVO =====================
  myservo.write(pos);

  // ===================== MONITORING =====================
  Serial.print("ADC Potensio: ");
  Serial.print(val);

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  // ===================== STABILISASI =====================
  delay(15);
}