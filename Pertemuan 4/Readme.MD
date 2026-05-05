# Modul 4 – Analog to Digital Converter (ADC) dan Pulse Width Modulation (PWM)

**Nama:** Ardina Jihan Mariska
**NIM:** H1H024018
**Shift Awal:** Ardina Jihan Mariska
**Shift Akhir:** H1H024018


---

## Pertanyaan & Jawaban Praktikum 4A
![Rangkaian percobaan 4A: Arduino Uno terhubung dengan potensiometer pada pin A0 dan motor servo pada pin 9](dokumentasi/rangkaian percobaan4A.png)

1. Apa fungsi perintah `analogRead()` pada rangkaian praktikum ini?
Fungsi `analogRead()` bertugas membaca nilai tegangan analog yang dihasilkan oleh potensiometer pada pin A0. Potensiometer bertindak sebagai pembagi tegangan variabel: saat diputar, tegangan pada pin tengahnya berubah secara kontinu antara 0 V dan 5 V. Fungsi `analogRead()` mengaktifkan ADC 10-bit Arduino untuk mengambil sampel tegangan tersebut dan mengonversinya menjadi nilai integer 0–1023, yang kemudian digunakan sebagai representasi digital posisi putaran potensiometer untuk menentukan sudut target motor servo.

---

2. Mengapa diperlukan fungsi `map()` dalam program tersebut?
Fungsi `map()` diperlukan karena terdapat perbedaan skala antara output ADC dan input yang dibutuhkan motor servo. `analogRead()` menghasilkan nilai 0–1023 (ADC 10-bit), sedangkan `myservo.write()` hanya menerima nilai 0–180 (derajat). Tanpa konversi, nilai ADC 1023 yang langsung dikirim ke servo akan menyebabkan servo mencoba bergerak ke 1023° yang jauh melebihi batas fisiknya dan berpotensi merusak komponen. Fungsi `map()` melakukan interpolasi linear untuk memetakan setiap nilai di rentang 0–1023 ke nilai proporsional di rentang 0–180 secara otomatis.

---

3. Modifikasi program agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023.

### Kode program 4A sebelum modifikasi
```cpp
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // pin analog input
const int servoPin = 9;       // pin digital PWM untuk servo

// ===================== VARIABEL =====================
int pos = 0; // sudut servo (0–180)
int val = 0; // nilai ADC (0–1023)

void setup() {
  myservo.attach(servoPin); // hubungkan servo ke pin
  Serial.begin(9600);       // aktifkan serial monitor
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
```

### Penjelasan Kode 4A

| Baris | Penjelasan |
|---|---|
| `#include <Servo.h>` | Menyertakan library bawaan Arduino untuk mengontrol motor servo. Tanpa library ini, sinyal PWM untuk servo harus dihitung dan dikirim secara manual. |
| `Servo myservo;` | Membuat objek bertipe `Servo` bernama `myservo`. Objek ini merepresentasikan satu unit motor servo dan menyediakan method seperti `attach()` dan `write()`. |
| `const int potensioPin = A0;` | Mendefinisikan pin A0 sebagai input analog potensiometer. Dideklarasikan `const` karena nilainya tidak berubah selama program berjalan. |
| `const int servoPin = 9;` | Pin 9 dipilih karena merupakan salah satu pin PWM (`~`) pada Arduino Uno. Library Servo membutuhkan pin PWM untuk menghasilkan sinyal kontrol servo. |
| `int pos = 0;` | Variabel penyimpan sudut servo (0–180), diinisialisasi ke 0 agar servo berada di posisi awal saat program dimulai. |
| `int val = 0;` | Variabel penyimpan nilai ADC (0–1023), diinisialisasi ke 0 sebelum pembacaan pertama dilakukan. |
| `myservo.attach(servoPin)` | Mengaktifkan kontrol servo pada pin 9. Setelah baris ini, library Servo mulai mengirimkan sinyal PWM ke pin tersebut secara otomatis di background. |
| `Serial.begin(9600)` | Menginisialisasi komunikasi UART pada baud rate 9600 bps agar data dapat ditampilkan di Serial Monitor Arduino IDE. |
| `analogRead(potensioPin)` | Membaca tegangan analog pada pin A0 menggunakan ADC 10-bit. Mengembalikan nilai integer 0–1023 yang merepresentasikan tegangan 0–5 V dari potensiometer. |
| `map(val, 0, 1023, 0, 180)` | Melakukan interpolasi linear: mengonversi skala ADC (0–1023) ke skala sudut servo (0–180). Setiap nilai ADC dipetakan ke sudut yang proporsional. |
| `myservo.write(pos)` | Mengirimkan perintah posisi ke servo. Library Servo mengonversi nilai derajat (0–180) ke lebar pulsa PWM yang sesuai (1–2 ms dalam periode 20 ms). |
| `delay(15)` | Memberikan jeda 15 ms agar servo punya waktu cukup mencapai posisi target sebelum perintah berikutnya dikirim. Tanpa delay, servo bisa bergetar (jitter). |

---

### Kode Modifikasi 4A (Servo Rentang 30°–150°)

Perubahan: parameter output `map()` diubah dari `(0, 180)` menjadi `(30, 150)` sehingga meskipun potensiometer diputar penuh, servo hanya bergerak dalam batas 30° hingga 150°.

```cpp
#include <Servo.h>

Servo myservo;

const int potensioPin = A0;
const int servoPin = 9;

int pos = 0;
int val = 0;

void setup() {
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(potensioPin);

  // MODIFIKASI: batas output diubah dari (0, 180) → (30, 150)
  // ADC=0    → pos=30°  (servo tidak pernah ke 0°)
  // ADC=511  → pos=90°  (titik tengah tetap sama)
  // ADC=1023 → pos=150° (servo tidak pernah ke 180°)
  pos = map(val, 0, 1023, 30, 150);

  myservo.write(pos);

  Serial.print("ADC: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo (30-150): ");
  Serial.println(pos);

  delay(15);
}
```

### Penjelasan Perubahan Kode Modifikasi 4A

| Bagian | Kode Asli | Kode Modifikasi | Penjelasan |
|---|---|---|---|
| Batas bawah output `map()` | `0` | `30` | Nilai ADC=0 sekarang menghasilkan sudut 30°, bukan 0° |
| Batas atas output `map()` | `180` | `150` | Nilai ADC=1023 sekarang menghasilkan sudut 150°, bukan 180° |
| Rentang gerak servo | 0° – 180° | 30° – 150° | Total rentang berkurang dari 180° menjadi 120° |
| Label Serial Monitor | `"Sudut Servo: "` | `"Sudut Servo (30-150): "` | Diperbarui agar output lebih informatif |

Rumus pemetaan baru: `pos = (val / 1023) × (150 - 30) + 30 = (val / 1023) × 120 + 30`

---

Hanya satu baris yang berubah. Rentang input ADC tetap 0–1023, namun output sekarang dibatasi ke 30°–150°. Fungsi `map()` secara otomatis menghitung: `pos = (val / 1023) × 120 + 30`.

---

## Pertanyaan & Jawaban Praktikum 4B
![Rangkaian percobaan 4B: Arduino Uno terhubung dengan potensiometer pada pin A0 dan LED melalui resistor 220Ω pada pin 9](dokumentasi/rangkaian percobaan4B.png)

1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`!
`analogWrite()` tidak menghasilkan tegangan analog sejati, melainkan menyalakan dan mematikan LED dengan sangat cepat menggunakan teknik PWM (frekuensi ±490 Hz pada pin 9 Arduino Uno). Karena pergantian ON-OFF terjadi jauh lebih cepat dari kemampuan mata manusia mendeteksi kedipan (±60 Hz), mata mengintegrasikan cahaya tersebut dan mempersepsikannya sebagai kecerahan yang proporsional dengan duty cycle. Semakin besar nilai yang dikirim (0–255), semakin lama LED dalam kondisi ON per periode, sehingga semakin terang yang terlihat.

---

2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
Keduanya dihubungkan oleh pemetaan linear melalui fungsi `map()`:

```
pwm = map(nilaiADC, 0, 1023, 0, 255)
    = nilaiADC × (255 / 1023)
    ≈ nilaiADC × 0.2493
```

Hubungannya proporsional langsung: semakin besar nilai ADC, semakin besar nilai PWM, semakin terang LED. Perbedaan resolusi terjadi karena ADC menggunakan 10-bit (1024 level) sedangkan PWM 8-bit (256 level). Fungsi `map()` menjembatani perbedaan resolusi ini.

| Nilai ADC | Nilai PWM | Duty Cycle | Kondisi LED     |
|-----------|-----------|------------|-----------------|
| 0         | 0         | 0%         | Mati            |
| 256       | ~64       | ~25%       | Redup           |
| 511       | ~127      | ~50%       | Setengah terang |
| 767       | ~191      | ~75%       | Agak terang     |
| 1023      | 255       | 100%       | Terang penuh    |

---

3. Modifikasi program agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200.

### Kode Percobaan 4B sebelum modifikasi

```cpp
#include <Arduino.h>

// ===================== PIN SETUP =====================
const int potPin = A0;   // pin analog potensiometer
const int ledPin = 9;    // pin PWM untuk LED

// ===================== VARIABEL =====================
int nilaiADC = 0;  // nilai pembacaan ADC (0–1023)
int pwm = 0;       // nilai PWM (0–255)

void setup() {
  pinMode(ledPin, OUTPUT); // atur pin LED sebagai output
  Serial.begin(9600);      // aktifkan serial monitor
}

void loop() {

  // ===================== PEMBACAAN SENSOR =====================
  nilaiADC = analogRead(potPin);

  // ===================== PEMROSESAN DATA =====================
  pwm = map(nilaiADC,
            0,     // minimum ADC
            1023,  // maksimum ADC
            0,     // PWM minimum
            255);  // PWM maksimum

  // ===================== OUTPUT PWM =====================
  analogWrite(ledPin, pwm);

  // ===================== MONITORING =====================
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM: ");
  Serial.println(pwm);

  // ===================== STABILISASI =====================
  delay(50);
}
```

### Penjelasan Kode 4B

| Baris | Penjelasan |
|---|---|
| `#include <Arduino.h>` | Menyertakan header dasar Arduino. Di lingkungan Arduino IDE header ini disertakan otomatis, namun perlu ditulis eksplisit jika menggunakan platform seperti PlatformIO. |
| `const int potPin = A0;` | Mendefinisikan pin A0 sebagai input analog dari potensiometer. Sama seperti 4A, nilainya konstan selama program berjalan. |
| `const int ledPin = 9;` | Pin 9 dipilih karena merupakan pin PWM (`~`). Fungsi `analogWrite()` hanya dapat digunakan pada pin yang mendukung PWM. |
| `int nilaiADC = 0;` | Variabel penyimpan nilai pembacaan ADC (0–1023), diinisialisasi ke 0. |
| `int pwm = 0;` | Variabel penyimpan nilai PWM hasil konversi (0–255), diinisialisasi ke 0 sehingga LED mati saat program baru dimulai. |
| `pinMode(ledPin, OUTPUT)` | Menetapkan pin 9 sebagai output digital/PWM. Langkah ini wajib sebelum menggunakan `analogWrite()` pada pin tersebut. |
| `Serial.begin(9600)` | Menginisialisasi komunikasi serial pada baud rate 9600 bps untuk menampilkan data ADC dan PWM di Serial Monitor. |
| `analogRead(potPin)` | Membaca ADC 10-bit dari potensiometer pada pin A0, menghasilkan nilai 0–1023 sesuai posisi putaran. |
| `map(nilaiADC, 0, 1023, 0, 255)` | Mengonversi nilai ADC 10-bit (1024 level) ke nilai PWM 8-bit (256 level) secara linear. Rasio konversi: 255/1023 ≈ 0,249 per satuan ADC. |
| `analogWrite(ledPin, pwm)` | Menghasilkan sinyal PWM pada pin 9 dengan duty cycle sebesar `(pwm/255) × 100%`. Frekuensi PWM default pada pin 9 Arduino Uno adalah ±490 Hz. |
| `delay(50)` | Jeda 50 ms untuk menstabilkan sistem dan memperlambat output Serial Monitor agar data mudah dibaca. Lebih panjang dari delay 4A karena LED tidak membutuhkan waktu stabilisasi mekanik. |

---

### Kode Modifikasi 4B (LED Kecerahan PWM 50–200)

Perubahan: ditambahkan fungsi `constrain()` setelah `map()` untuk memotong nilai PWM agar selalu berada dalam rentang 50–200. LED tidak pernah terlalu redup (< PWM 50) maupun terlalu terang (> PWM 200).

```cpp
#include <Arduino.h>

const int potPin = A0;
const int ledPin = 9;

int nilaiADC = 0;
int pwm = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  nilaiADC = analogRead(potPin);

  // Langkah 1: konversi ADC ke rentang PWM penuh seperti biasa
  pwm = map(nilaiADC, 0, 1023, 0, 255);

  // MODIFIKASI: potong nilai PWM ke rentang 50–200 menggunakan constrain()
  // Jika pwm < 50  → dipaksa menjadi 50  (LED tidak pernah terlalu redup)
  // Jika pwm > 200 → dipaksa menjadi 200 (LED tidak pernah terlalu terang)
  // Jika 50 ≤ pwm ≤ 200 → nilai tidak berubah
  pwm = constrain(pwm, 50, 200);

  analogWrite(ledPin, pwm);

  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM (50-200): ");
  Serial.println(pwm);

  delay(50);
}
```

### Penjelasan Perubahan Kode Modifikasi 4B

| Bagian | Kode Asli | Kode Modifikasi | Penjelasan |
|---|---|---|---|
| Batas bawah PWM | 0 (LED mati) | 50 (LED selalu sedikit menyala) | LED tidak pernah sepenuhnya mati meski potensiometer di minimum |
| Batas atas PWM | 255 (terang penuh) | 200 (sedikit di bawah maksimum) | LED tidak pernah mencapai kecerahan penuh |
| Fungsi tambahan | — | `constrain(pwm, 50, 200)` | Memotong nilai di luar rentang tanpa mengubah nilai yang sudah di dalam rentang |
| Label Serial Monitor | `"PWM: "` | `"PWM (50-200): "` | Diperbarui agar output lebih informatif |

**Efek pada potensiometer:** ada zona mati di kedua ujung putaran — saat potensiometer berada di posisi sangat rendah, kecerahan tetap di PWM 50; saat di posisi sangat tinggi, kecerahan berhenti naik di PWM 200. Seluruh rentang tengah potensiometer tetap menggeser kecerahan secara linear dalam batas tersebut.

---

Penambahan `constrain(pwm, 50, 200)` setelah `map()` memotong nilai PWM yang keluar dari rentang yang diinginkan. Efeknya: di ujung bawah potensiometer LED tidak pernah lebih redup dari PWM 50, dan di ujung atas tidak pernah lebih terang dari PWM 200. Ada zona di kedua ujung potensiometer di mana kecerahan tidak berubah meski potensiometer terus diputar.
