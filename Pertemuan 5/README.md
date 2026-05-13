# Modul 5 – Real-Time Operating System: Multitasking dan Komunikasi Task

**Mata Kuliah** : Sistem Tertanam dan Mikrokontroler  
**Nama**        : Dina  
**NIM**         : H1H024018  
**Shift Awal**  : C 
**Shift Akhir** : C

## Percobaan 5A – Multitasking LED
### Kode Program 5A

```cpp
#include <Arduino_FreeRTOS.h>

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

void setup() {
  Serial.begin(9600);

  // Mendaftarkan tiga task ke scheduler FreeRTOS
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint,  "task3", 128, NULL, 1, NULL);

  // Memulai scheduler – setelah baris ini loop() tidak dieksekusi lagi
  vTaskStartScheduler();
}

void loop() {
  // Sengaja dikosongkan – kendali diserahkan ke FreeRTOS scheduler
}

// Task 1: Kedipkan LED pada pin D8 setiap 200ms
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while (1) {
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);  // Tunda 200ms, lepas CPU
    digitalWrite(8, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// Task 2: Kedipkan LED pada pin D7 setiap 300ms
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while (1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);  // Tunda 300ms, lepas CPU
    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// Task 3: Cetak counter bertambah setiap 500ms
void Taskprint(void *pvParameters) {
  int counter = 0;
  while (1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Tunda 500ms, lepas CPU
  }
}
```

### Penjelasan Kode 5A

| Baris / Bagian | Penjelasan |
|---|---|
| `#include <Arduino_FreeRTOS.h>` | Menyertakan library FreeRTOS untuk Arduino. Library ini menyediakan semua fungsi kernel RTOS: task management, scheduler, dan delay API. |
| `void TaskBlink1(void *pvParameters)` | Deklarasi prototype task. Semua task FreeRTOS harus bertipe `void` dan menerima parameter `void*`. |
| `Serial.begin(9600)` | Inisialisasi komunikasi serial dengan baud rate 9600 untuk output Serial Monitor. |
| `xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL)` | Mendaftarkan TaskBlink1 ke scheduler. Parameter: fungsi task, nama (untuk debugging), ukuran stack (128 word = 256 byte), parameter (NULL), prioritas (1), handle (NULL). |
| `vTaskStartScheduler()` | Memulai FreeRTOS scheduler. Setelah baris ini, CPU diserahkan ke kernel. Fungsi `loop()` tidak akan pernah dieksekusi lagi. |
| `loop() {}` | Dikosongkan karena kontrol eksekusi sudah diambil oleh scheduler. |
| `while(1)` dalam setiap task | Task FreeRTOS harus memiliki loop tak terbatas. Task tidak boleh return (exit) karena akan menyebabkan undefined behavior. |
| `digitalWrite(8, HIGH)` | Mengaktifkan (HIGH) pin digital 8 untuk menyalakan LED. |
| `vTaskDelay(200 / portTICK_PERIOD_MS)` | Menangguhkan task selama 200ms. Selama delay ini CPU dibebaskan untuk task lain. Berbeda dengan `delay()` biasa yang bersifat blocking. `portTICK_PERIOD_MS` adalah konstanta konversi ms ke tick (1 tick = 1ms pada Arduino FreeRTOS default). |
| `counter++` di Taskprint | Variabel lokal counter yang terus bertambah, menunjukkan task berjalan berulang kali. |

---

### Pertanyaan Praktikum 5A

#### Pertanyaan 1: Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Ketiga task tidak berjalan secara benar-benar bersamaan (paralel), melainkan bergantian secara sangat cepat (concurrent) sehingga tampak seolah-olah bersamaan. Arduino Uno hanya memiliki satu inti CPU (single-core), sehingga pada satu saat hanya satu task yang benar-benar dieksekusi.

**Mekanisme Scheduler FreeRTOS:**
FreeRTOS menggunakan algoritma preemptive priority-based scheduling dengan time-slicing untuk task yang berprioritas sama:

1. Tick Interrupt: Setiap 1ms (default tick rate Arduino FreeRTOS), hardware timer menghasilkan interrupt.
2. Context Switch: Pada setiap tick interrupt, scheduler FreeRTOS memeriksa task mana yang siap (Ready state) dan menentukan task berikutnya yang akan dijalankan.
3. Time-Slicing: Karena ketiga task memiliki prioritas yang sama (1), scheduler menggilirnya secara round-robin.
4. Blocked State: Saat task memanggil `vTaskDelay()`, task tersebut masuk ke status Blocked dan CPU diberikan ke task lain yang Ready. Inilah kunci mengapa ketiga task bisa "berjalan bersamaan".

**Ilustrasi Timeline:**
```
Waktu (ms): 0   1   2   ...  200  201  202  ...  300  301  ...  500
Task1:      RUN BLK BLK ...  RUN  BLK  BLK  ...  BLK  BLK  ...  BLK
Task2:      BLK RUN BLK ...  BLK  RUN  BLK  ...  RUN  BLK  ...  BLK
Task3:      BLK BLK RUN ...  BLK  BLK  RUN  ...  BLK  RUN  ...  RUN
```
*(RUN = sedang dieksekusi, BLK = Blocked/menunggu delay)*

Hasilnya: LED D8 berkedip setiap 400ms (HIGH 200ms + LOW 200ms), LED D7 berkedip setiap 600ms, dan counter bertambah setiap 500ms — semuanya berjalan mandiri tanpa mengganggu satu sama lain.

---

#### Pertanyaan 2: Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
Untuk menambahkan task keempat (misalnya LED ketiga pada pin D6 yang berkedip setiap 400ms), langkah-langkahnya adalah:

**Langkah 1 – Deklarasikan prototype task baru:**
```cpp
void TaskBlink3( void *pvParameters );
```

**Langkah 2 – Daftarkan task di dalam `setup()` sebelum `vTaskStartScheduler()`:**
```cpp
xTaskCreate(
  TaskBlink3,  // Pointer ke fungsi task
  "task4",     // Nama task (untuk debugging)
  128,         // Stack size (word). Tambah jika task kompleks
  NULL,        // Parameter (tidak digunakan)
  1,           // Prioritas (sama dengan task lain = round-robin)
  NULL         // Handle task (NULL = tidak perlu referensi)
);
```

**Langkah 3 – Tulis implementasi fungsi task:**
```cpp
void TaskBlink3(void *pvParameters) {
  pinMode(6, OUTPUT);  // Inisialisasi pin D6 sebagai output
  while (1) {
    Serial.println("Task4");
    digitalWrite(6, HIGH);
    vTaskDelay(400 / portTICK_PERIOD_MS);
    digitalWrite(6, LOW);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}
```

**Catatan:**
- **Stack size**: Jika task keempat menggunakan lebih banyak variabel lokal atau memanggil fungsi nested, naikkan stack size (misal 256).
- **Memori SRAM**: Arduino Uno hanya memiliki 2KB SRAM. Setiap task mengonsumsi sekitar 128–256 byte untuk stack, ditambah overhead FreeRTOS ~200 byte. Pastikan total stack tidak melebihi kapasitas.
- **Prioritas**: Jika task keempat memiliki kebutuhan respons lebih cepat, beri prioritas lebih tinggi (misalnya `2`).

---

#### Pertanyaan 3: Modifikasi dengan Potensiometer untuk Kontrol Kecepatan LED

*(Kode modifikasi lengkap tersedia di bagian [Kode Modifikasi 5A](#kode-modifikasi-5a--potensiometer-kontrol-kecepatan-led) di bawah)*

**Penjelasan Hasil:**  
Dengan menambahkan potensiometer pada pin A0, nilai ADC (0–1023) dibaca oleh task khusus dan disimpan di variabel global `volatile`. Nilai ini kemudian di-mapping ke rentang delay (50ms–1000ms) menggunakan `map()`. Kedua task LED membaca variabel delay tersebut sehingga kecepatan kedip LED berubah secara real-time seiring perputaran potensiometer. Penggunaan `volatile` memastikan compiler tidak melakukan optimasi yang mengabaikan perubahan nilai dari task lain.

---

## Percobaan 5B – Komunikasi Task (Queue + DHT22)

### Kode Program 5B

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Struktur data untuk menyimpan hasil pembacaan sensor
struct readings {
  int temp;  // Suhu dalam derajat Celsius
  int h;     // Kelembaban relatif dalam persen
};

QueueHandle_t my_queue;  // Handle queue global

void setup() {
  Serial.begin(9600);

  // Membuat queue berkapasitas 1 item, ukuran satu struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Mendaftarkan task pembaca sensor dan task tampilan
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display,   "display",      128, NULL, 0, NULL);
}

void loop() {}

// Task 1: Membaca data sensor (simulasi) dan mengirim ke queue
void read_data(void *pvParameters) {
  struct readings x;
  for (;;) {
    x.temp = 54;  // Nilai statis (simulasi); ganti dengan pembacaan DHT22 nyata
    x.h    = 30;
    xQueueSend(my_queue, &x, portMAX_DELAY);  // Kirim ke queue, tunggu jika penuh
    vTaskDelay(100);                           // Delay 100 tick (~100ms)
  }
}

// Task 2: Menerima data dari queue dan tampilkan ke Serial Monitor
void display(void *pvParameters) {
  struct readings x;
  for (;;) {
    // Blokir hingga ada data di queue (portMAX_DELAY = tunggu selamanya)
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);
      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}
```

### Penjelasan Kode 5B

| Baris / Bagian | Penjelasan |
|---|---|
| `#include <queue.h>` | Header FreeRTOS untuk API queue: `xQueueCreate`, `xQueueSend`, `xQueueReceive`. |
| `struct readings { int temp; int h; }` | Mendefinisikan tipe data kustom yang akan dikirim melalui queue. Menggunakan struct memungkinkan pengiriman beberapa nilai sekaligus dalam satu operasi queue yang atomik. |
| `QueueHandle_t my_queue` | Variabel global untuk menyimpan referensi (handle) ke queue yang dibuat. Harus global agar dapat diakses oleh kedua task. |
| `xQueueCreate(1, sizeof(struct readings))` | Membuat queue dengan kapasitas **1 item** bertipe `struct readings`. Kapasitas 1 cocok untuk skenario "data terbaru selalu ditampilkan" — jika task display terlambat, data lama akan digantikan. Mengembalikan `QueueHandle_t`. |
| `xTaskCreate(read_data, ..., 0, ...)` | Mendaftarkan task read_data dengan prioritas 0 (paling rendah). Prioritas kedua task sama sehingga menggunakan round-robin. |
| `xQueueSend(my_queue, &x, portMAX_DELAY)` | Mengirim satu item bertipe `struct readings` ke queue. `portMAX_DELAY` berarti task akan menunggu tanpa batas jika queue penuh. Thread-safe karena FreeRTOS melindungi operasi ini dengan critical section. |
| `vTaskDelay(100)` | Menangguhkan task read_data selama 100 tick. Memberi kesempatan task display untuk mengambil data dari queue. |
| `xQueueReceive(my_queue, &x, portMAX_DELAY)` | Mengambil satu item dari queue ke variabel `x`. Jika queue kosong, task display akan blokir (Blocked state) hingga ada data tersedia — efisien karena tidak membuang CPU untuk polling. Return `pdPASS` jika berhasil. |
| `Serial.print/println` | Mencetak nilai suhu dan kelembaban. Karena hanya task display yang memanggil Serial, tidak ada kekhawatiran race condition pada Serial output. |

---

### Pertanyaan Praktikum 5B
![Rangkaian percobaan 5b:(Dokumentasi/percobaan5b.png)]
#### Pertanyaan 1: Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Kedua task berjalan secara bergantian (concurrent), bukan secara paralel. Pada Arduino Uno yang single-core, hanya satu task yang aktif di CPU pada satu waktu.

**Mekanisme Koordinasi via Queue:**
Pola ini dikenal sebagai Producer-Consumer pattern:

```
[Task read_data]  →  xQueueSend()  →  [Queue: 1 slot]  →  xQueueReceive()  →  [Task display]
   (Producer)                                                                    (Consumer)
```

**Urutan eksekusi yang terjadi:**

1. t=0ms: Scheduler menjalankan `read_data`. Task mengisi struct dengan nilai suhu dan kelembaban, lalu memanggil `xQueueSend()` → data masuk ke queue → queue penuh.
2. `read_data` memanggil `vTaskDelay(100)` → masuk status Blocked.
3. Scheduler menjalankan `display`. Task memanggil `xQueueReceive()` → data berhasil diambil dari queue → queue kosong.
4. `display` mencetak data ke Serial Monitor, lalu kembali ke `xQueueReceive()` → karena queue kosong, task Blocked menunggu.
5. t=100ms: `read_data` bangun dari delay → kembali ke langkah 1.

**Hasilnya**: Kedua task tampak bekerja secara simultan karena pergantian yang sangat cepat, tetapi pada kenyataannya mereka terkoordinasi melalui queue — `read_data` tidak bisa terus menerus mengisi queue jika display belum membacanya (karena kapasitas queue = 1).

---

#### Pertanyaan 2: Apakah program ini berpotensi mengalami race condition? Jelaskan!
**Tidak berpotensi mengalami race condition**
Race condition terjadi ketika dua atau lebih thread/task mengakses sumber daya bersama (shared resource) secara bersamaan tanpa mekanisme sinkronisasi, sehingga hasil akhir bergantung pada urutan eksekusi yang tidak deterministik.

**Kenapa program ini AMAN dari race condition?**

1. Queue sebagai mekanisme sinkronisasi: Semua akses ke data (struct readings) dilakukan MELALUI queue FreeRTOS, bukan melalui variabel global yang diakses langsung. FreeRTOS memproteksi operasi `xQueueSend()` dan `xQueueReceive()` dengan critical section(menonaktifkan interrupt sementara) sehingga dua task tidak bisa mengakses queue secara bersamaan.
2. Transfer data atomik: `xQueueSend()` menyalin seluruh struct `readings` ke dalam buffer internal queue dalam satu operasi yang tidak dapat diinterupsi. `xQueueReceive()` menyalin dari buffer ke variabel lokal task secara atomik. Tidak ada state di mana data setengah-ditulis dibaca oleh task lain.
3. Ownership yang jelas: Setelah `xQueueSend()` berhasil, data "dimiliki" oleh queue. Setelah `xQueueReceive()` berhasil, data "dimiliki" oleh task display. Tidak ada shared ownership yang bersamaan.
4. Tidak ada variabel global bersama: Struct `x` di dalam `read_data` dan struct `x` di dalam `display` adalah variabel lokal masing-masing task (ada di stack task masing-masing), bukan shared global variable.

**Skenario yang BISA menyebabkan race condition (tidak terjadi di program ini):**
```cpp
// CONTOH BERBAHAYA 
struct readings globalData;  // Variabel global bersama

void read_data(...) {
    globalData.temp = dht.readTemperature();  // ← Task 1 menulis
    // Jika preempted di sini, Task 2 bisa membaca data yang tidak lengkap!
    globalData.h = dht.readHumidity();
    ...
}
void display(...) {
    Serial.println(globalData.temp);  // ← Task 2 membaca langsung
    Serial.println(globalData.h);
}
```
Program di atas berpotensi race condition. Program percobaan 5B menghindari ini dengan menggunakan queue.

---

#### Pertanyaan 3: Modifikasi dengan Sensor DHT22 Sesungguhnya

*(Kode modifikasi lengkap tersedia di bagian [Kode Modifikasi 5B](#kode-modifikasi-5b--dht22-sesungguhnya-dinamis) di bawah)*

**Penjelasan Hasil:**  
Dengan mengganti nilai statis (`x.temp = 54; x.h = 30;`) menggunakan library DHT.h (`dht.readTemperature()` dan `dht.readHumidity()`), nilai yang ditampilkan di Serial Monitor berubah secara real-time sesuai kondisi lingkungan nyata. Saat sensor dihembusi napas, nilai kelembaban langsung meningkat. Penambahan field `valid` (boolean) pada struct memungkinkan task display mendeteksi dan menampilkan pesan error jika pembacaan sensor gagal (return `NaN`). Heat index juga dihitung menggunakan `dht.computeHeatIndex()`.

---

## Kode Modifikasi 5A – Potensiometer Kontrol Kecepatan LED

Potensiometer pada pin A0 mengontrol kecepatan kedip kedua LED secara real-time.

```cpp
#include <Arduino_FreeRTOS.h>

// Variabel global volatile: nilainya bisa berubah kapan saja dari task lain
// Menyimpan delay dalam milidetik hasil pembacaan potensiometer
volatile uint16_t ledDelay = 200;  // Default 200ms

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void TaskReadPot( void *pvParameters );

void setup() {
  Serial.begin(9600);

  xTaskCreate(TaskBlink1,  "blink1",   128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2,  "blink2",   128, NULL, 1, NULL);
  xTaskCreate(TaskReadPot, "readPot",  128, NULL, 2, NULL);  // Prioritas lebih tinggi

  vTaskStartScheduler();
}

void loop() {}

// Task membaca nilai potensiometer (pin A0) dan update delay global
void TaskReadPot(void *pvParameters) {
  while (1) {
    int potValue = analogRead(A0);  // Baca ADC: 0 – 1023
    // Map ADC ke rentang delay 50ms – 1000ms
    ledDelay = (uint16_t) map(potValue, 0, 1023, 50, 1000);
    Serial.print("Pot: "); Serial.print(potValue);
    Serial.print(" -> Delay: "); Serial.println(ledDelay);
    vTaskDelay(50 / portTICK_PERIOD_MS);  // Update setiap 50ms
  }
}

// Task 1: LED pin D8, kecepatan mengikuti potensiometer
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while (1) {
    digitalWrite(8, HIGH);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);  // Baca delay dari variabel global
    digitalWrite(8, LOW);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);
  }
}

// Task 2: LED pin D7, kecepatan mengikuti potensiometer (sedikit offset)
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while (1) {
    digitalWrite(7, HIGH);
    // Offset 1.5x agar kedua LED tidak sinkron persis
    vTaskDelay((ledDelay * 3 / 2) / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay((ledDelay * 3 / 2) / portTICK_PERIOD_MS);
  }
}
```

**Rangkaian Potensiometer:**
```
Arduino 5V  ──┬── Pin 1 Pot
              │
Arduino A0  ──┤── Pin 2 Pot (wiper)
              │
Arduino GND ──┴── Pin 3 Pot
```

**Penjelasan:**
- `volatile uint16_t ledDelay`: Kata kunci `volatile` mencegah compiler mengoptimasi pembacaan variabel ini, karena nilainya dapat berubah kapan saja oleh task lain (TaskReadPot). Tanpa `volatile`, compiler bisa meng-cache nilai lama dan task LED tidak akan pernah melihat perubahan.
- `map(potValue, 0, 1023, 50, 1000)`: Fungsi Arduino bawaan yang memetakan rentang 0–1023 ke 50–1000 secara linear.
- TaskReadPot diberi prioritas 2 (lebih tinggi dari task LED) agar pembacaan potensiometer lebih responsif dan tidak terlambat saat kedua task LED sedang aktif.
- Catatan penting: Akses ke `ledDelay` dari beberapa task secara teknis adalah shared resource. Pada data tipe `uint16_t` di AVR (Arduino Uno), pembacaan/penulisan tidak dijamin atomik. Untuk keamanan penuh, idealnya menggunakan `portENTER_CRITICAL()` atau semaphore. Namun dalam konteks praktikum ini, efek race condition pada variabel delay tidak berbahaya (hanya menyebabkan sedikit variasi timing).

---

## Kode Modifikasi 5B – DHT22 Sesungguhnya (Dinamis)

Mengganti nilai statis dengan pembacaan sensor DHT22 nyata menggunakan library DHT.h.

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ── Konfigurasi DHT22 ───────────────────────────────────────────
#define DHTPIN   2      // Pin data DHT22 → Arduino D2
#define DHTTYPE  DHT22  // Tipe sensor: DHT22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// ── Struktur data (diperluas dari versi statis) ─────────────────
struct readings {
  float temp;  // Suhu dalam derajat Celsius (float untuk presisi)
  float h;     // Kelembaban relatif dalam persen
  bool  valid; // Flag: true jika pembacaan berhasil (tidak NaN)
};

QueueHandle_t my_queue;

void setup() {
  Serial.begin(9600);
  dht.begin();  // Inisialisasi sensor DHT22

  Serial.println(F("=== DHT22 FreeRTOS Queue Monitor ==="));
  Serial.println(F("Menunggu pembacaan pertama (2 detik)..."));

  // Queue tetap kapasitas 1 item
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Pastikan queue berhasil dibuat
  if (my_queue == NULL) {
    Serial.println(F("ERROR: Gagal membuat queue!"));
    while(1);  // Hentikan eksekusi jika queue gagal
  }

  xTaskCreate(read_data, "read_sensors", 256, NULL, 1, NULL);  // Stack 256 untuk DHT
  xTaskCreate(display,   "display",      128, NULL, 1, NULL);
}

void loop() {}

// ── Task 1: Baca sensor DHT22 dan kirim ke queue ────────────────
void read_data(void *pvParameters) {
  struct readings data;

  for (;;) {
    // Baca sensor (fungsi ini membutuhkan ~250ms untuk sampling)
    data.h    = dht.readHumidity();
    data.temp = dht.readTemperature();  // Celsius (default)

    // Validasi: isnan() mengembalikan true jika pembacaan gagal
    data.valid = !(isnan(data.h) || isnan(data.temp));

    // Kirim struct ke queue (overwrite jika display belum membaca)
    // Gunakan xQueueOverwrite() agar data terbaru selalu tersedia
    xQueueOverwrite(my_queue, &data);

    // Delay 2000ms: DHT22 butuh min. 2 detik antar pembacaan
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// ── Task 2: Terima data dari queue dan tampilkan ─────────────────
void display(void *pvParameters) {
  struct readings data;

  for (;;) {
    // Blokir hingga ada data (portMAX_DELAY = tunggu selamanya)
    if (xQueueReceive(my_queue, &data, portMAX_DELAY) == pdPASS) {

      if (data.valid) {
        // Pembacaan berhasil: tampilkan semua nilai
        Serial.print(F("Suhu      : "));
        Serial.print(data.temp, 1);   // 1 desimal
        Serial.println(F(" \xB0C"));

        Serial.print(F("Kelembaban: "));
        Serial.print(data.h, 1);
        Serial.println(F(" %"));

        // Heat index (indeks panas / feels-like temperature)
        float hi = dht.computeHeatIndex(data.temp, data.h, false);
        Serial.print(F("Heat Index: "));
        Serial.print(hi, 1);
        Serial.println(F(" \xB0C"));

        Serial.println(F("--------------------"));
      } else {
        // Pembacaan gagal: tampilkan pesan error
        Serial.println(F("ERROR: Gagal membaca DHT22!"));
        Serial.println(F("Periksa koneksi kabel sensor."));
        Serial.println(F("--------------------"));
      }
    }
  }
}
```

**Perbedaan dari Program Statis (5B awal):**

| Aspek | Versi Statis (5B awal) | Versi DHT22 Nyata (Modifikasi) |
|---|---|---|
| Tipe data suhu/kelembaban | `int` | `float` (lebih presisi) |
| Sumber data | Nilai hardcoded (54, 30) | Sensor DHT22 via `dht.readTemperature()` |
| Validasi data | Tidak ada | Field `valid` + cek `isnan()` |
| Error handling | Tidak ada | Cetak pesan error jika sensor gagal |
| Queue send | `xQueueSend()` | `xQueueOverwrite()` – data terbaru selalu ada |
| Delay | 100 tick | 2000ms – sesuai spec DHT22 (min 2s/sampel) |
| Stack read_data | 128 word | 256 word – karena DHT library lebih besar |
| Heat index | Tidak ada | Dihitung via `dht.computeHeatIndex()` |

**Contoh Output Serial Monitor:**
```
=== DHT22 FreeRTOS Queue Monitor ===
Menunggu pembacaan pertama (2 detik)...
Suhu      : 28.5 °C
Kelembaban: 72.3 %
Heat Index: 31.2 °C
--------------------
Suhu      : 28.6 °C
Kelembaban: 73.1 %
Heat Index: 31.5 °C
--------------------
```

---
