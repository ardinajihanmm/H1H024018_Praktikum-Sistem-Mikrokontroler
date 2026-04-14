# Modul 2 – Pemrograman GPIO
**Nama:** Ardina Jihan Mariska  
**NIM:** H1H024018  
**Shift Akhir:** B  
**Shift Akhir:** C  

## Percobaan 2A – Seven Segment Counter
### 2.5.4 Pertanyaan Praktikum 2A
#### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
![Rangkaian Arduino](Dokumentasi%20Percobaan%202A/rangkaian2A.png)

---
#### 2. Apa yang terjadi jika nilai `num` lebih dari 15?
Jika fungsi `displayHex()` dipanggil dengan `num > 15`, terjadi **array out-of-bounds access**.
**Penjelasan:**  
Array `hexPatterns` didefinisikan dengan ukuran `[16][7]`, artinya indeks yang valid hanya **0 sampai 15**. Jika `num = 16` misalnya, program akan mengakses `hexPatterns[16][x]` yang **tidak dialokasikan** dalam memori array tersebut.
**Dampak pada Arduino (AVR/ATmega328P):**  
Bahasa C/C++ pada Arduino tidak memiliki bounds checking otomatis. Akibatnya:
- Program membaca data dari lokasi memori yang berada di luar array — yaitu data apa pun yang kebetulan ada di memori setelahnya (bisa berisi variabel lain, kode program, atau sampah memori).
- Seven Segment Display akan menampilkan **pola acak dan tidak terdefinisi** — segmen menyala/mati secara tidak beraturan.
- Dalam skenario terburuk, jika data yang terbaca sangat besar atau memori yang diakses adalah bagian dari stack/program, bisa menyebabkan **perilaku tidak terduga (undefined behavior)** seperti program hang atau restart.
**Dalam program ini hal tersebut tidak terjadi** karena perulangan `for (int i = 0; i < 16; i++)` membatasi nilai `i` maksimal 15, sehingga `displayHex()` tidak pernah dipanggil dengan argumen > 15.
---
#### 3. Apakah program ini menggunakan Common Cathode atau Common Anode? Jelaskan alasannya!
Program ini menggunakan **Common Anode (CA)**.
**Bukti dari kode:**
```cpp
digitalWrite(pinA, hexPatterns[num][0] ? LOW : HIGH);
```
Logika ternary `? LOW : HIGH` menunjukkan:
- Bila nilai di `hexPatterns` = **1 (ON)** → pin diberi sinyal **LOW (0V)**
- Bila nilai di `hexPatterns` = **0 (OFF)** → pin diberi sinyal **HIGH (5V)**

**Penjelasan prinsip kerja:**
Pada Seven Segment **Common Anode**, semua **anoda** LED dalam satu display dihubungkan menjadi satu titik dan disambungkan ke **VCC (5V)**. Agar sebuah LED (segmen) menyala, arus harus mengalir dari anoda (VCC) melalui LED menuju katoda. Ini hanya terjadi jika katoda berada pada potensial yang **lebih rendah dari anoda**, yaitu ketika pin Arduino mengeluarkan **LOW (GND/0V)**. Sebaliknya, pada **Common Cathode**, semua katoda terhubung ke GND dan segmen menyala dengan sinyal **HIGH**.
**Perbandingan:**
| Kondisi       | Common Anode | Common Cathode |
|---------------|-------------|----------------|
| Segmen ON     | LOW (0V)    | HIGH (5V)      |
| Segmen OFF    | HIGH (5V)   | LOW (0V)       |
| COM terhubung ke | VCC (5V) | GND (0V)       |
---
#### 4. Modifikasi program agar tampilan berjalan dari F ke 0
```cpp
#include <Arduino.h>
// Mapping pin segmen: a  b  c   d   e  f  g  dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};
// Mendefinisikan 8 pin Arduino yang terhubung ke setiap segmen Seven Segment Display.
// Urutan: a=7, b=6, c=5, d=10, e=11, f=8, g=9, dp=4

// Alias nama pin agar kode lebih mudah dibaca
#define pinA  segmentPins[0]   // Pin untuk segmen a (atas)
#define pinB  segmentPins[1]   // Pin untuk segmen b (kanan atas)
#define pinC  segmentPins[2]   // Pin untuk segmen c (kanan bawah)
#define pinD  segmentPins[3]   // Pin untuk segmen d (bawah)
#define pinE  segmentPins[4]   // Pin untuk segmen e (kiri bawah)
#define pinF  segmentPins[5]   // Pin untuk segmen f (kiri atas)
#define pinG  segmentPins[6]   // Pin untuk segmen g (tengah)
#define pinDP segmentPins[7]   // Pin untuk segmen dp (titik)

// Pola bit untuk setiap karakter heksadesimal (urutan: a b c d e f g)
// Nilai 1 = segmen ON, nilai 0 = segmen OFF
// Perangkat: Common Anode → ON = LOW, OFF = HIGH
byte hexPatterns[16][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0 → a,b,c,d,e,f menyala; g mati
  {0, 1, 1, 0, 0, 0, 0}, // 1 → hanya b,c menyala
  {1, 1, 0, 1, 1, 0, 1}, // 2 → a,b,d,e,g menyala
  {1, 1, 1, 1, 0, 0, 1}, // 3 → a,b,c,d,g menyala
  {0, 1, 1, 0, 0, 1, 1}, // 4 → b,c,f,g menyala
  {1, 0, 1, 1, 0, 1, 1}, // 5 → a,c,d,f,g menyala
  {1, 0, 1, 1, 1, 1, 1}, // 6 → a,c,d,e,f,g menyala
  {1, 1, 1, 0, 0, 0, 0}, // 7 → a,b,c menyala
  {1, 1, 1, 1, 1, 1, 1}, // 8 → semua segmen menyala
  {1, 1, 1, 1, 0, 1, 1}, // 9 → a,b,c,d,f,g menyala
  {1, 1, 1, 0, 1, 1, 1}, // A → a,b,c,e,f,g menyala
  {0, 0, 1, 1, 1, 1, 1}, // b → c,d,e,f,g menyala
  {1, 0, 0, 1, 1, 1, 0}, // C → a,d,e,f menyala
  {0, 1, 1, 1, 1, 0, 1}, // d → b,c,d,e,g menyala
  {1, 0, 0, 1, 1, 1, 1}, // E → a,d,e,f,g menyala
  {1, 0, 0, 0, 1, 1, 1}  // F → a,e,f,g menyala
};

// Deklarasi fungsi displayHex sebelum digunakan
void displayHex(int num);

void setup() {
  // Inisialisasi semua pin sebagai OUTPUT
  // Dilakukan satu kali saat Arduino dinyalakan
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    // Gunakan loop agar lebih efisien daripada menulis 8 baris pinMode
  }
  
  // Matikan semua segmen di awal (HIGH = mati pada Common Anode)
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }
}

void loop() {
  // *** MODIFIKASI: Loop berjalan dari 15 (F) turun ke 0 ***
  // Menggunakan decrement (i--) agar urutan terbalik dibanding versi asli
  for (int i = 15; i >= 0; i--) {
    // i dimulai dari 15 (F) dan berkurang setiap iterasi hingga 0
    
    displayHex(i);    // Tampilkan karakter ke-i pada Seven Segment Display
    delay(1000);      // Tunda 1000ms (1 detik) sebelum menampilkan karakter berikutnya
  }
  // Setelah i mencapai 0 dan loop selesai, fungsi loop() dipanggil ulang
  // → counter otomatis kembali ke F dan mulai hitung mundur lagi
}

void displayHex(int num) {
  // Fungsi untuk menampilkan satu karakter heksadesimal pada Seven Segment Display
  // Parameter num: indeks karakter (0–15) yang ingin ditampilkan

  // Tulis pola bit ke setiap pin segmen (a sampai g)
  // Operator ternary: jika hexPatterns[num][x] == 1 → tulis LOW (ON)
  //                   jika hexPatterns[num][x] == 0 → tulis HIGH (OFF)
  // Ini karena display bertipe Common Anode: segmen menyala ketika pin LOW
  digitalWrite(pinA, hexPatterns[num][0] ? LOW : HIGH); // Segmen a
  digitalWrite(pinB, hexPatterns[num][1] ? LOW : HIGH); // Segmen b
  digitalWrite(pinC, hexPatterns[num][2] ? LOW : HIGH); // Segmen c
  digitalWrite(pinD, hexPatterns[num][3] ? LOW : HIGH); // Segmen d
  digitalWrite(pinE, hexPatterns[num][4] ? LOW : HIGH); // Segmen e
  digitalWrite(pinF, hexPatterns[num][5] ? LOW : HIGH); // Segmen f
  digitalWrite(pinG, hexPatterns[num][6] ? LOW : HIGH); // Segmen g

  // Segmen dp (titik desimal) selalu dimatikan karena tidak digunakan
  // HIGH = mati pada Common Anode
  digitalWrite(pinDP, HIGH);
}
```

## Percobaan 1B – Kontrol Counter dengan Push Button
### 2.6.4 Pertanyaan Praktikum 1B
#### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
![Rangkaian Arduino](Dokumentasi%20Percobaan1B/rangkaian1B.png)

#### 2. Mengapa push button menggunakan mode `INPUT_PULLUP`? Apa keuntungannya?
**Alasan penggunaan `INPUT_PULLUP`:**

Ketika sebuah pin dikonfigurasi sebagai `INPUT` biasa tanpa pull-up atau pull-down, dan tombol dalam kondisi tidak ditekan (tidak terhubung ke mana pun), pin tersebut berada dalam kondisi **floating** — yaitu tegangan pada pin tidak terdefinisi dan dapat berfluktuasi secara acak antara HIGH dan LOW akibat noise elektromagnetik atau induksi dari lingkungan sekitar. Kondisi ini menyebabkan `digitalRead()` memberikan hasil yang tidak bisa diandalkan.
`INPUT_PULLUP` mengatasi masalah ini dengan mengaktifkan **resistor pull-up internal** (~20–50 kΩ) yang sudah tersedia di dalam chip ATmega328P, sehingga pin selalu terhubung ke VCC (HIGH) secara internal saat tombol tidak ditekan.

**Keuntungan `INPUT_PULLUP` dibandingkan rangkaian biasa:**
| Aspek | `INPUT_PULLUP` | Rangkaian Pull-Up Eksternal |
|-------|---------------|---------------------------|
| Komponen tambahan | Tidak ada (built-in) | Butuh resistor ~10kΩ eksternal |
| Kompleksitas wiring | Lebih sederhana | Lebih banyak kabel |
| Konsumsi arus | Sangat kecil (~50µA) | Tergantung nilai resistor |
| Keandalan | Terjamin oleh pabrikan | Tergantung kualitas komponen |
| Kemudahan kode | `pinMode(pin, INPUT_PULLUP)` | `pinMode(pin, INPUT)` + rangkaian |
| Logika baca | Aktif LOW (ditekan = LOW) | Aktif HIGH (ditekan = HIGH) |

**Cara kerja dengan `INPUT_PULLUP`:**
- Tombol **tidak ditekan** → pin terhubung ke VCC melalui pull-up → `digitalRead()` = **HIGH**
- Tombol **ditekan** → pin terhubung langsung ke GND → `digitalRead()` = **LOW**
---

#### 3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya?
**Dari sisi Hardware:**
| No. | Kemungkinan Penyebab | Cara Diagnosa |
|-----|----------------------|---------------|
| 1 | Kabel jumper dari pin Arduino ke breadboard tidak terpasang dengan benar atau longgar | Periksa dan tekan ulang setiap kabel jumper |
| 2 | Resistor 220Ω tidak terpasang pada jalur segmen yang bermasalah | Lacak jalur rangkaian dari pin Arduino ke segmen |
| 3 | Resistor putus (nilai yang terukur oleh multimeter tidak sesuai atau ∞) | Ukur resistor dengan multimeter |
| 4 | LED segmen pada Seven Segment Display rusak/terbakar | Coba hubungkan segmen langsung ke 5V melalui resistor |
| 5 | Koneksi COM (anoda bersama) tidak terhubung ke 5V dengan baik | Periksa jalur 5V ke pin COM Seven Segment |
| 6 | Jalur GND Arduino tidak terhubung ke breadboard | Periksa kabel GND |
| 7 | Seven Segment Display dipasang terbalik di breadboard | Cek datasheet posisi pin COM dan segmen |

**Dari sisi Software:**
| No. | Kemungkinan Penyebab | Cara Diagnosa |
|-----|----------------------|---------------|
| 1 | Salah memetakan nomor pin di `segmentPins[]` — misal pin d ditulis `11` padahal seharusnya `10` | Bandingkan `segmentPins[]` dengan tabel wiring fisik |
| 2 | Pola bit di `hexPatterns` salah untuk karakter tertentu — segmen yang seharusnya ON ditulis 0 | Cek referensi pola Seven Segment dan bandingkan dengan array |
| 3 | `pinMode()` untuk pin segmen yang bermasalah tidak dipanggil | Pastikan semua 8 pin ada di `setup()` |
| 4 | Logika HIGH/LOW terbalik (program dikira Common Cathode padahal Common Anode) | Cek apakah program menggunakan `? LOW : HIGH` atau `? HIGH : LOW` |
| 5 | Variabel `num` yang dikirim ke `displayHex()` melebihi 15 sehingga pola yang dibaca garbage | Tambahkan validasi `if (num < 0 || num > 15) return;` |
---

#### 4. Modifikasi dua push button (Increment & Decrement)

```cpp
#include <Arduino.h>
// ============================================================
// KONFIGURASI PIN
// ============================================================

// Mapping pin segmen: a  b  c   d   e  f  g  dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};
// Delapan pin Arduino yang terhubung ke segmen a,b,c,d,e,f,g,dp

#define pinA  segmentPins[0]
#define pinB  segmentPins[1]
#define pinC  segmentPins[2]
#define pinD  segmentPins[3]
#define pinE  segmentPins[4]
#define pinF  segmentPins[5]
#define pinG  segmentPins[6]
#define pinDP segmentPins[7]

const int btnUp   = 2;   // Pin push button INCREMENT (tambah)
const int btnDown = 3;   // Pin push button DECREMENT (kurang) — pin baru
// Pin 3 dipilih karena berdekatan dengan pin 2 dan mendukung interrupt hardware

// ============================================================
// DATA
// ============================================================

// Pola bit karakter heksadesimal (urutan: a b c d e f g)
// 1 = segmen ON, 0 = segmen OFF
// Common Anode: ON = LOW, OFF = HIGH
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

// ============================================================
// VARIABEL GLOBAL
// ============================================================

int  currentDigit  = 0;     // Nilai counter saat ini (0–15)
bool lastUpState   = HIGH;  // State tombol UP pada iterasi sebelumnya
bool lastDownState = HIGH;  // State tombol DOWN pada iterasi sebelumnya
// Keduanya diinisialisasi HIGH karena INPUT_PULLUP default HIGH saat tidak ditekan

// ============================================================
// DEKLARASI FUNGSI
// ============================================================
void displayHex(int num);

// ============================================================
// SETUP — dijalankan sekali saat Arduino menyala
// ============================================================
void setup() {
  // Inisialisasi semua pin segmen sebagai OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    // Loop lebih efisien daripada menulis 8 baris pinMode secara manual
  }

  // Inisialisasi pin tombol sebagai INPUT dengan pull-up internal aktif
  pinMode(btnUp,   INPUT_PULLUP);
  // Pull-up internal: pin default HIGH saat tidak ditekan, LOW saat ditekan
  // Tidak perlu resistor eksternal → menyederhanakan rangkaian

  pinMode(btnDown, INPUT_PULLUP);
  // Sama seperti btnUp, untuk tombol decrement

  // Tampilkan nilai awal (0) di Seven Segment sejak program dimulai
  displayHex(currentDigit);
}

// ============================================================
// LOOP — dijalankan terus-menerus
// ============================================================
void loop() {

  // Baca kondisi kedua tombol di iterasi ini
  bool upState   = digitalRead(btnUp);
  // HIGH = tidak ditekan, LOW = ditekan
  bool downState = digitalRead(btnDown);

  // ---- TOMBOL UP (INCREMENT) ----
  if (lastUpState == HIGH && upState == LOW) {
    // Edge detection falling edge tombol UP:
    // kondisi ini hanya true pada satu siklus pertama tombol ditekan

    currentDigit++;
    // Tambah counter sebesar 1

    if (currentDigit > 15) currentDigit = 0;
    // Wrap-around: setelah F (15), kembali ke 0
    // Membentuk siklus circular: 0 → 1 → ... → F → 0

    displayHex(currentDigit);
    // Perbarui tampilan Seven Segment dengan nilai counter baru
  }

  // ---- TOMBOL DOWN (DECREMENT) ----
  if (lastDownState == HIGH && downState == LOW) {
    // Edge detection falling edge tombol DOWN:
    // sama seperti tombol UP, hanya true satu siklus saat pertama ditekan

    currentDigit--;
    // Kurangi counter sebesar 1

    if (currentDigit < 0) currentDigit = 15;
    // Wrap-around ke bawah: setelah 0, kembali ke F (15)
    // Membentuk siklus circular: F → E → ... → 0 → F
    // Catatan: perlu cek < 0 bukan < 0 karena int bisa negatif

    displayHex(currentDigit);
    // Perbarui tampilan Seven Segment dengan nilai counter baru
  }

  // ---- UPDATE STATE UNTUK ITERASI BERIKUTNYA ----
  lastUpState   = upState;
  lastDownState = downState;
  // Simpan kondisi tombol saat ini agar bisa dibandingkan di siklus loop() berikutnya
  // WAJIB ada di akhir loop — jika dihapus, edge detection tidak berfungsi
}

// ============================================================
// FUNGSI displayHex — menampilkan satu karakter ke Seven Segment
// ============================================================
void displayHex(int num) {
  // Validasi batas agar tidak terjadi array out-of-bounds
  if (num < 0 || num > 15) return;

  // Tulis sinyal ke setiap pin segmen sesuai pola di hexPatterns
  // Ternary: nilai 1 (ON) → LOW, nilai 0 (OFF) → HIGH
  // Karena display Common Anode: segmen menyala saat pin = LOW (GND)
  digitalWrite(pinA, hexPatterns[num][0] ? LOW : HIGH); // Segmen a (atas)
  digitalWrite(pinB, hexPatterns[num][1] ? LOW : HIGH); // Segmen b (kanan atas)
  digitalWrite(pinC, hexPatterns[num][2] ? LOW : HIGH); // Segmen c (kanan bawah)
  digitalWrite(pinD, hexPatterns[num][3] ? LOW : HIGH); // Segmen d (bawah)
  digitalWrite(pinE, hexPatterns[num][4] ? LOW : HIGH); // Segmen e (kiri bawah)
  digitalWrite(pinF, hexPatterns[num][5] ? LOW : HIGH); // Segmen f (kiri atas)
  digitalWrite(pinG, hexPatterns[num][6] ? LOW : HIGH); // Segmen g (tengah)

  // Segmen dp selalu mati — tidak digunakan dalam counter ini
  digitalWrite(pinDP, HIGH); // HIGH = mati pada Common Anode
}
```

**Perubahan rangkaian dari Percobaan 2B versi 1 tombol:**
Tambahan wiring Push Button DECREMENT:
Arduino Uno     Push Button DOWN
-----------     ----------------
Pin 3    ──────── Kaki 1
GND      ──────── Kaki 2
(INPUT_PULLUP aktif, tidak perlu resistor eksternal)
```

---
**Ringkasan logika dua tombol:**
| Aksi | Kondisi Edge Detection | Operasi | Wrap-around |
|------|------------------------|---------|-------------|
| Tekan UP | `lastUp==HIGH && up==LOW` | `currentDigit++` | >15 → reset ke 0 |
| Tekan DOWN | `lastDown==HIGH && down==LOW` | `currentDigit--` | <0 → reset ke 15 |
---

```
## 2.7 Pertanyaan Praktikum
#### 1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!
```
Percobaan 2A – Seven Segment Counter:
Program `modul2_output.ino` berhasil menampilkan karakter heksadesimal 0 hingga F secara berurutan pada Seven Segment Display dengan interval 1 detik per karakter. Program menggunakan array `hexPatterns[16][7]` sebagai lookup table pola bit dan fungsi `displayHex()` yang mengonversi pola bit menjadi sinyal LOW/HIGH ke setiap pin segmen sesuai karakteristik Common Anode. Setelah karakter F ditampilkan, counter secara otomatis kembali ke 0 dan siklus berulang tanpa henti. Tidak ada segmen yang mati atau tampil tidak sesuai urutan, dan program berjalan stabil tanpa error.
Percobaan 1B – Kontrol Counter dengan Push Button:
Program `modul2_input.ino` berhasil mengintegrasikan push button sebagai input digital untuk mengendalikan counter secara manual. Nilai counter bertambah satu setiap kali tombol ditekan, dan di-reset ke 0 setelah melewati nilai 15 (F). Implementasi `INPUT_PULLUP` pada pin 2 berhasil mencegah kondisi floating tanpa memerlukan resistor eksternal. Teknik edge detection (falling edge) dengan membandingkan `lastUpState` dan `upState` berhasil memastikan setiap penekanan tombol hanya menghasilkan satu increment, sehingga tidak terjadi penghitungan ganda. Tampilan Seven Segment diperbarui secara real-time setiap kali tombol ditekan.
```
#### 2. Bagaimana prinsip kerja dari Seven Segment Display dalam menampilkan angka dan karakter?
Seven Segment Display adalah komponen yang terdiri dari 8 buah LED yang disusun membentuk pola tertentu: tujuh segmen berbentuk garis (a, b, c, d, e, f, g) yang membentuk angka, ditambah satu segmen titik (dp). Dengan mengaktifkan kombinasi segmen yang tepat, display dapat membentuk tampilan angka 0–9 dan karakter huruf A–F.
```
Dua tipe konfigurasi:
1. Common Cathode (CC): Semua katoda LED terhubung ke GND. Segmen menyala dengan sinyal HIGH dari mikrokontroler.
2. Common Anode (CA): Semua anoda LED terhubung ke VCC. Segmen menyala dengan sinyal LOW dari mikrokontroler, karena arus mengalir dari VCC (anoda) melalui LED ke pin mikrokontroler yang mengeluarkan GND.
Pada praktikum ini (Common Anode)
- Untuk menampilkan angka `0`: segmen a, b, c, d, e, f harus menyala (LOW) dan g harus mati (HIGH).
- Pola ini disimpan dalam array `hexPatterns` sebagai `{1,1,1,1,1,1,0}` di mana nilai 1 berarti segmen aktif (dan akan dikonversi ke LOW oleh fungsi `displayHex()`).
---
```
#### 3. Jelaskan bagaimana sistem counter bekerja pada program tersebut!
Counter Percobaan 2A (Otomatis):
Sistem counter pada Percobaan 2A bekerja secara otomatis berbasis waktu. Mekanismenya:
1. Fungsi `loop()` terus dijalankan berulang.
2. Setiap iterasi, variabel `i` pada perulangan `for` diinkremen dari 0 hingga 15.
3. Fungsi `displayHex(i)` dipanggil dengan nilai `i` saat itu, menampilkan karakter heksadesimal ke-i.
4. `delay(1000)` menahan eksekusi selama 1 detik sebelum nilai `i` berikutnya ditampilkan.
5. Setelah `i = 15` (karakter F), loop `for` selesai, fungsi `loop()` selesai, lalu dipanggil ulang dari awal — `i` kembali ke 0 secara otomatis.
Counter Percobaan 2B (Manual via Push Button):
Sistem counter pada Percobaan 2B bekerja secara event-driven (berbasis kejadian penekanan tombol). Mekanismenya menggunakan teknik edge detection:
1. Variabel `currentDigit` menyimpan nilai counter yang persisten (tidak di-reset tiap siklus loop).
2. Setiap siklus `loop()`, kondisi tombol dibaca via `digitalRead(btnUp)`.
3. Sistem memeriksa apakah terjadi falling edge: kondisi tombol berubah dari HIGH (tidak ditekan) menjadi LOW (baru ditekan).
4. Hanya saat falling edge terdeteksi, `currentDigit` diinkremen dan tampilan diperbarui.
5. `lastUpState` diperbarui di akhir setiap siklus untuk menjadi referensi siklus berikutnya.
