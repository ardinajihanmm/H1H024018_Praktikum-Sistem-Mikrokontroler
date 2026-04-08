Nama : Ardina Jihan Mariska
NIM : H1H024018


1.5 Pertanyaan Praktikum
1. Pada kondisi apa program masuk ke blok if? Program akan mengeksekusi blok if ketika nilai variabel timeDelay sudah mencapai kurang dari atau sama dengan 100 ms. Kondisi ini menandakan bahwa kecepatan kedipan LED sudah berada pada titik paling cepat, sehingga program memberikan jeda sementara lalu mengembalikan nilai delay ke kondisi awal.
2. Pada kondisi apa program masuk ke blok else? Blok else dijalankan selama nilai timeDelay masih lebih besar dari 100 ms. Pada bagian ini, nilai delay akan terus dikurangi sebesar 100 ms di setiap siklus, sehingga waktu kedip LED menjadi semakin singkat dan terlihat semakin cepat.
3. Apa fungsi dari perintah delay(timeDelay)? Perintah delay(timeDelay) digunakan untuk menghentikan sementara jalannya program sesuai nilai timeDelay dalam milidetik. Fungsi ini mengatur lama LED dalam kondisi menyala maupun mati, sehingga membentuk pola kedipan dengan kecepatan tertentu.
4. Jika program yang dibuat memiliki alur mati, lambat, cepat, reset (mati), ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang → mati! Modifikasi pola cepat → sedang → mati dilakukan dengan mengubah logika percabangan pada variabel timeDelay. Ketika nilai delay mencapai batas minimum (≤100 ms), program tidak langsung melakukan reset, melainkan mengubah nilai delay menjadi sedang (500 ms). Setelah itu, pada kondisi tertentu, LED dibuat mati sementara sebelum akhirnya nilai timeDelay dikembalikan ke kondisi awal. Dengan cara ini, perubahan pola kedipan LED

 ![alt text](<penjelasan kode1.5.png>)

Awal: LED berkedip lambat (1500 ms)
Kemudian: semakin cepat karena delay dikurangi
Saat mencapai cepat (≤100 ms):
→ tidak langsung reset, tapi pindah ke kecepatan sedang (500 ms)
Setelah fase sedang:
→ LED mati sebentar, lalu kembali ke kondisi awal

1.6 Pertanyaan praktikum
1. Gambarkan rangkaian schematic 5 LED running
Rangkaian terdiri dari beberapa LED (pada praktikum digunakan 6 LED tapi untuk soal ini memakai 5 LED sesuai intruksi) yang masing-masing dihubungkan ke pin digital Arduino, yaitu pin 2 sampai pin 6. Setiap LED disusun seri dengan resistor 1k Ohm untuk membatasi arus. Kaki anoda LED terhubung ke pin digital Arduino, sedangkan kaki katoda dihubungkan ke GND.

![alt text](<percobaan perulangan.jpeg.png>)

3. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan?
Efek pergerakan LED dari kiri ke kanan dihasilkan menggunakan perulangan for dengan nilai awal pin kecil menuju pin yang lebih besar (2 sampai 7). Pada setiap iterasi, LED pada pin tertentu dinyalakan, kemudian diberikan jeda menggunakan delay, lalu dimatikan sebelum berpindah ke LED berikutnya. Proses ini menciptakan efek visual seolah-olah cahaya bergerak secara berurutan ke arah kanan.

4. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri?
Untuk menghasilkan gerakan sebaliknya, digunakan perulangan for kedua dengan arah berlawanan, yaitu dari pin 7 menuju pin 2. Nilai pin dikurangi setiap iterasi sehingga LED menyala secara berurutan dari kanan ke kiri. Kombinasi kedua perulangan ini menghasilkan efek bolak-balik (running light).

5. Program 3 LED kanan dan kiri bergantian
 ![alt text](<penjelasan kode1.6.png>)

Program ini tidak lagi memakai perulangan running, tapi diganti pola kelompok LED
LED dibagi jadi:
kiri → pin 2, 3, 4
kanan → pin 5, 6, 7
Kedua kelompok menyala secara bergantian
Delay digunakan supaya perpindahan terlihat jelas
Loop membuat pola ini berjalan terus menerus

1.7 Pertanyaan analisis
1. Praktikum ini berhasil menunjukkan penerapan struktur percabangan untuk mengatur kecepatan kedip LED, serta struktur perulangan untuk mengontrol penyalaan beberapa LED secara berurutan.
2. Penggunaan perulangan seperti for sangat mempermudah dalam mengatur banyak LED tanpa harus menuliskan kode berulang secara manual. Hal ini membuat program lebih efisien, terstruktur, dan mudah dimodifikasi.
3. Struktur percabangan if-else bekerja dengan cara mengevaluasi kondisi tertentu, dalam hal ini nilai timeDelay. Berdasarkan hasil evaluasi tersebut, program menentukan apakah perlu mengurangi delay atau mengatur ulang nilai agar pola kedipan tetap berjalan.
4. Kombinasi antara perulangan dan percabangan memungkinkan Arduino menjalankan proses berulang sekaligus mengambil keputusan berdasarkan kondisi tertentu. Dengan demikian, sistem dapat menjadi lebih dinamis dan mampu merespons perubahan yang terjadi selama program berjalan. 


