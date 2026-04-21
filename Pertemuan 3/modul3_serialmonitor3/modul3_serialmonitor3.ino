#include <Wire.h>               // Mengimpor library untuk komunikasi I2C
#include <LiquidCrystal_I2C.h>   // Mengimpor library untuk LCD I2C
#include <Arduino.h>             // Library utama Arduino

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inisialisasi LCD dengan alamat I2C dan ukuran 16x2
const int pinPot = A0;              // Pin untuk membaca potensiometer

void setup() {
  Serial.begin(9600);               // Memulai komunikasi serial dengan baud rate 9600
  lcd.init();                        // Inisialisasi LCD
  lcd.backlight();                   // Menyalakan backlight LCD
}

void loop() {
  int nilai = analogRead(pinPot);   // Membaca nilai analog dari potensiometer

  // Menghitung volt berdasarkan nilai ADC (0-1023)
  float volt = nilai * (5.0 / 1023.0);  // Mengonversi nilai ADC ke volt (5V maksimum)

  // Menghitung persen berdasarkan nilai ADC (0-1023)
  int persen = map(nilai, 0, 1023, 0, 100);

  // Mapping nilai ADC (0-1023) ke panjang bar (0-16)
  int panjangBar = map(nilai, 0, 1023, 0, 16);

  // Menampilkan nilai ADC, Volt, dan Persen di Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilai);
  Serial.print(" Volt: ");
  Serial.print(volt, 2);  // Menampilkan volt dengan 2 angka desimal
  Serial.print(" Persen: ");
  Serial.print(persen);
  Serial.println("%");

  // Baris 1: Menampilkan nilai ADC pada LCD
  lcd.setCursor(0, 0);               // Menempatkan kursor pada baris pertama
  lcd.print("ADC: ");
  lcd.print(nilai);                  // Menampilkan nilai ADC
  lcd.print(" ");                    // Menghapus sisa karakter

  // Baris 2: Menampilkan bar berdasarkan panjang bar (level)
  lcd.setCursor(0, 1);               // Menempatkan kursor pada baris kedua
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255);           // Menampilkan bar menggunakan karakter 255
    } else {
      lcd.print(" ");                 // Menampilkan spasi untuk sisa bagian bar
    }
  }

  delay(200);                        // Delay 200ms untuk pembaruan nilai dan tampilan
}