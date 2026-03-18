#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct Roti {
    int id;
    char namaRoti[100];
    int harga;
    int stok;
};

const char* pRoti = "roti.txt";

char beli[100] = "";
int hargaBeli = 0;
int jumlahBeli = 0;

int hitungData() {
    FILE* f = fopen(pRoti, "r");
    if (!f) return -1;
    int n = 0;
    char baris[256];
    while (fgets(baris, sizeof(baris), f)) n++;
    fclose(f);
    return n;
}

Roti** bacaData(int& n) {
    n = hitungData();
    if (n == -1) {
        cout << "Error! Tambahkan data roti dulu, untuk membuat file, ya!" << endl;
        cout << "Press any key to continue . . . ";
        cin.get();
        n = 0;
        return NULL;
    }
    if (n == 0) {
        cout << "Error! data roti kosong, tambahkan data roti di menu pertama!" << endl;
        cout << "Press any key to continue . . . ";
        cin.get();
        return NULL;
    }

    Roti** data = new Roti*[n];
    FILE* f = fopen(pRoti, "r");
    if (!f) { n = 0; return NULL; }

    for (int i = 0; i < n; i++) {
        data[i] = new Roti();
        char baris[256];
        fgets(baris, sizeof(baris), f);
        sscanf(baris, "%d|%[^|]|%d|%d", &data[i]->id, data[i]->namaRoti, &data[i]->harga, &data[i]->stok);
    }
    fclose(f);
    return data;
}

void simpanData(Roti** data, int n) {
    FILE* f = fopen(pRoti, "w");
    if (!f) {
        cout << "Error! FIle tidak bisa diakses!";
        return;
    }
    for (int i = 0; i < n; i++)
        fprintf(f, "%d|%s|%d|%d\n", data[i]->id, data[i]->namaRoti, data[i]->harga, data[i]->stok);
    fclose(f);
}

void gantiData(Roti** data, int n) {
    if (!data) return;
    for (int i = 0; i < n; i++) delete data[i];
    delete[] data;
}

void bubbleAsc(Roti** data, int n) {
    for (int i = 0; i<n-1; i++)
        for (int j = 0; j<n-i-1; j++)
            if (data[j]->harga > data[j+1]->harga) {
                Roti* tmp = data[j];
                data[j] = data[j+1];
                data[j+1] = tmp;
            }
}

int partisi(Roti** data, int kiri, int kanan) {
    int pivot = data[kanan]->harga;
    int i = kiri - 1;
    for (int j = kiri; j < kanan; j++) {
        if (data[j]->harga > pivot) {
            i++;
            Roti* tmp = data[i]; data[i] = data[j]; data[j] = tmp;
        }
    }
    Roti* tmp = data[i+1]; data[i+1] = data[kanan]; data[kanan] = tmp;
    return i+1;
}

void quickDsc(Roti** data, int kiri, int kanan) {
    if (kiri < kanan) {
        int p = partisi(data, kiri, kanan);
        quickDsc(data, kiri, p - 1);
        quickDsc(data, p + 1, kanan);
    }
}

void bubbleNama(Roti** data, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (strcmp(data[j]->namaRoti, data[j+1]->namaRoti) > 0) {
                Roti* tmp = data[j]; data[j] = data[j+1]; data[j+1] = tmp;
            }
}

int seqSearch(Roti** data, int n, const char* nama) {
    for (int i = 0; i < n; i++)
        if (strcmp(data[i]->namaRoti, nama) == 0) return i;
    return -1;
}

int binarySearch(Roti** data, int n, const char* nama) {
    int kiri = 0, kanan = n - 1;
    while (kiri <= kanan) {
        int tengah = (kiri + kanan) / 2;
        int cmp = strcmp(data[tengah]->namaRoti, nama);
        if (cmp == 0) return tengah;
        else if (cmp < 0) kiri = tengah + 1;
        else kanan = tengah - 1;
    }
    return -1;
}

void tampilData(Roti** data, int n) {
    for (int i = 0; i < n; i++) {
        cout << "\nID Roti   : " << data[i]->id
             << "\nNama Roti : " << data[i]->namaRoti
             << "\nHarga Roti: " << data[i]->harga
             << "\nStok Roti : " << data[i]->stok << endl;
    }
}

void tambahRoti() {
    system("cls");
    int jumlah;
    cout << "Masukkan jumlah data roti: ";
    cin >> jumlah;
    cin.ignore();

    if (jumlah <= 0) {
        cout << "Error! masukin jumlah yang bener ya, minimal 1!" << endl;
        cout << "Press any key to continue . . . ";
        cin.get();
        return;
    }

    int nLama = 0;
    Roti** lama = bacaData(nLama);
    int lastid = 0;
    for (int i = 0; i < nLama; i++)
        if (lama[i]->id > lastid) lastid = lama[i]->id;
    gantiData(lama, nLama);

    Roti** baru = new Roti*[jumlah];
    for (int i = 0; i < jumlah; i++) {
        baru[i] = new Roti();
        baru[i]->id = lastid+i+1;
        cout << "\nID roti : " << baru[i]->id << "\n";
        cout << "Nama Roti: ";
        cin.getline(baru[i]->namaRoti, 100);
        cout << "Harga Roti: ";
        cin >> baru[i]->harga;
        cout << "Stok Roti: ";
        cin >> baru[i]->stok;
        cin.ignore();
    }

    FILE* f = fopen(pRoti, "a");
    if (!f) {
        cout << "Error! file tidak bisa diakses!" << endl;
        gantiData(baru, jumlah);
        return;
    }
    for (int i = 0; i < jumlah; i++)
        fprintf(f, "%d|%s|%d|%d\n", baru[i]->id, baru[i]->namaRoti, baru[i]->harga, baru[i]->stok);
    fclose(f);
    gantiData(baru, jumlah);

    cout << "\nData telah tersimpan" << endl;
}

void lihatRoti() {
    system("cls");
    int n = 0;
    Roti** data = bacaData(n);
    if (n == 0 || !data) {
        return;
    }

    cout << "Jumlah data roti: " << n << endl;
    cout << "==============================" << endl;
    cout << "Pilih Metode Lihat:" << endl;
    cout << "1. Ascending" << endl;
    cout << "2. Descending" << endl;
    cout << "Masukkan metode: ";
    int metode;
    cin >> metode;

    if (metode == 1) bubbleAsc(data, n);
    else if (metode == 2) quickDsc(data, 0, n - 1);
    else {
        cout << "Error! pilih 1 atau 2!" << endl;
        gantiData(data, n);
        return;
    }

    tampilData(data, n);
    gantiData(data, n);

    cout << endl;
    cout << "Press any key to continue . . . ";
    cin.ignore();
    cin.get();
}

void editRoti() {
    system("cls");
    int n = 0;
    Roti** data = bacaData(n);
    if (n == 0 || !data) {
        cout << "Error! data tidak ditemukan!" << endl;
        return;
    }

    cout << "Jumlah data roti: " << n << endl;
    cout << "Masukkan ID roti yang mau di edit: ";
    int idCari;
    cin >> idCari;
    cin.ignore();

    int posisi = -1;
    for (int i = 0; i < n; i++)
        if (data[i]->id == idCari) { posisi = i; break; }

    if (posisi == -1) {
        cout << "Error! data tidak ditemukan!" << endl;
        gantiData(data, n);
        return;
    }

    cout << "Data ditemukan!" << endl << endl;
    cout << "Data Lama: " << endl;
    cout << "Nama Roti : " << data[posisi]->namaRoti << endl;
    cout << "Harga Roti: " << data[posisi]->harga << endl;
    cout << "Stok Roti : " << data[posisi]->stok << endl << endl;
    cout << "Data Baru: " << endl;
    cout << "Nama Roti: ";
    cin.getline(data[posisi]->namaRoti, 100);
    cout << "Harga Roti: ";
    cin >> data[posisi]->harga;
    cout << "Stok Roti: ";
    cin >> data[posisi]->stok;
    cin.ignore();

    simpanData(data, n);
    gantiData(data, n);

    cout << endl;
    cout << "Data roti berhasil diperbarui!" << endl;
    cout << "Press any key to continue . . . ";
    cin.get();
}

void menuKaryawan() {
    int pilihan;
    bool lanjut = true;

    while (lanjut) {
        system("cls");
        cout << "==================" << endl;
        cout << "  WELCOME BAKER!" << endl;
        cout << "==================" << endl;
        cout << "1. Tambah menu roti" << endl;
        cout << "2. Lihat menu roti" << endl;
        cout << "3. Edit menu roti" << endl;
        cout << "4. Logout" << endl;
        cout << "Masukkan menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                tambahRoti();
                char ulang;
                cout << "Kembali ke menu? (y/n)" << endl;
                cin >> ulang;
                cin.ignore();
                if (ulang != 'y' && ulang != 'Y') lanjut = false;
                break;
            }
            case 2:
                lihatRoti();
                break;
            case 3:
                editRoti();
                break;
            case 4:
                cout << "\nTerimakasih Baker, selamat beristirahat! 😴" << endl;
                cout << "Press any key to continue . . . ";
                cin.ignore(); cin.get();
                lanjut = false;
                break;
            default:
                cout << "Error! masukkan angka yang sesuai!" << endl;
        }
    }
}

void cariRoti() {
    system("cls");
    int n = 0;
    Roti** data = bacaData(n);
    if (n == 0 || !data) {
        return;
    }

    cout << "Pilih Metode Cari: " << endl;
    cout << "1. Sequential" << endl;
    cout << "2. Binary" << endl;
    cout << "Masukkan metode: ";
    int metode;
    cin >> metode;
    cin.ignore();

    char nama[100];
    cout << "Masukkan nama roti yang dicari: ";
    cin.getline(nama, 100);
    cout << "Jumlah data roti: " << n << endl;

    int posisi = -1;
    if (metode == 1) {
        posisi = seqSearch(data, n, nama);
    } else if (metode == 2) {
        bubbleNama(data, n);
        posisi = binarySearch(data, n, nama);
    } else {
        cout << "Error! masukkan angka yang sesuai!" << endl;
        gantiData(data, n);
        return;
    }

    if (posisi == -1) {
        cout << "Error! roti tidak ditemukan!" << endl;
    } else {
        cout << "Roti ditemukan" << endl;
        cout << "Nama  : " << data[posisi]->namaRoti << endl;
        cout << "Harga : " << data[posisi]->harga << endl;
        cout << "Stok  : " << data[posisi]->stok << endl;
    }
    gantiData(data, n);
}

void beliRoti() {
    system("cls");
    int n = 0;
    Roti** data = bacaData(n);
    if (n == 0 || !data) {
        return;
    }

    cout << "Jumlah data roti: " << n << endl;
    cout << "Pilih Metode Lihat:" << endl;
    cout << "1. Ascending" << endl;
    cout << "2. Descending" << endl;
    cout << "Masukkan metode: ";
    int metode;
    cin >> metode;

    if (metode == 1) bubbleAsc(data, n);
    else if (metode == 2) quickDsc(data, 0, n-1);
    else {
        cout << "Error! masukkan angka yang sesuai!" << endl;
        gantiData(data, n);
        return;
    }

    tampilData(data, n);
    cout << endl;
    cout << "Press any key to continue . . . ";
    cin.ignore();
    cin.get();

    cout << "Masukkan ID roti yang ingin dibeli: ";
    int idBeli;
    cin >> idBeli;

    int posisi = -1;
    for (int i = 0; i < n; i++)
        if (data[i]->id == idBeli) { posisi = i; break; }

    if (posisi == -1) {
        cout << "Error! roti tidak ditemukan!" << endl;
        gantiData(data, n);
        return;
    }

    if (data[posisi]->stok == 0) {
        cout << "Yah, stoknya abis! Mau coba roti lain? :3" << endl;
        gantiData(data, n);
        return;
    }

    cout << "Masukkan jumlah beli: ";
    int jumlah;
    cin >> jumlah;
    cin.ignore();

    if (jumlah <= 0 || jumlah > data[posisi]->stok) {
        cout << "Jumlah melebihi stok tersedia (maksimal " << data[posisi]->stok << ") ya!" << endl;
        gantiData(data, n);
        return;
    }

    strcpy(beli, data[posisi]->namaRoti);
    hargaBeli = data[posisi]->harga;
    jumlahBeli = jumlah;

    gantiData(data, n);

    data = bacaData(n);
    for (int i = 0; i < n; i++)
        if (data[i]->id == idBeli) { data[i]->stok -= jumlah; break; }
    simpanData(data, n);
    gantiData(data, n);

    cout << "\nPembelian berhasil! Silakan pilih menu Invoice untuk mencetak struk, ya!" << endl;
    cout << "Press any key to continue . . . ";
    cin.ignore();
    cin.get();
}

void invoiceRoti() {
    system("cls");
    if (strlen(beli) == 0) {
        cout << "Error! belum ada transaksi, yuk beli dulu :3" << endl;
        return;
    }
    int total = hargaBeli * jumlahBeli;
    cout << "===== STRUK PEMBELIAN =====" << endl;
    cout << "Nama Roti : " << beli << "\n";
    cout << "Harga     : " << hargaBeli << "\n";
    cout << "Jumlah    : " << jumlahBeli << "\n";
    cout << "Total     : " << total << "\n";
    cout << "============================" << endl;
    cout << "Press any key to continue . . . ";
    cin.ignore();
    cin.get();
}

void menuPembeli() {
    int pilihan;
    bool lanjut = true;

    while (lanjut) {
        system("cls");
        cout << "==================" << endl;
        cout << " WELCOME BUYER! 🛒" << endl;
        cout << "==================" << endl;
        cout << "1. Cari roti" << endl;
        cout << "2. Beli roti" << endl;
        cout << "3. Invoice Roti" << endl;
        cout << "4. Logout" << endl;
        cout << "Masukkan menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                cariRoti();
                char ulang;
                cout << "kembali ke menu?(y/n) ";
                cin >> ulang;
                cin.ignore();
                if (ulang != 'y' && ulang != 'Y') lanjut = false;
                break;
            }
            case 2: {
                beliRoti();
                char ulang;
                cout << "kembali ke menu?(y/n) ";
                cin >> ulang;
                cin.ignore();
                if (ulang != 'y' && ulang != 'Y') lanjut = false;
                break;
            }
            case 3: {
                invoiceRoti();
                char ulang;
                cout << "kembali ke menu?(y/n) ";
                cin >> ulang;
                cin.ignore();
                if (ulang != 'y' && ulang != 'Y') lanjut = false;
                break;
            }
            case 4:
                cout << "\nTerima kasih telah berbelanja disini! Have a nice day! 🌞" << endl;
                cout << "Press any key to continue . . . ";
                cin.ignore();
                cin.get();
                lanjut = false;
                break;
            default:
                cout << "Error! masukkan angka yang sesuai!" << endl;
        }
    }
}

bool loginKaryawan() {
    system("cls");
    const char* username = "kry";
    const char* password = "123";

    cout << "====================================" << endl;
    cout << "         Halo Karyawan!" << endl;
    cout << "  Silahkan login untuk melanjutkan" << endl;
    cout << "====================================" << endl;

    char uname[50], pass[50];
    cout << "Masukkan Username: ";
    cin >> uname;
    cout << "Masukkan Password: ";
    cin >> pass;
    cin.ignore();

    if (strcmp(uname, username) == 0 && strcmp(pass, password) == 0) {
        cout << "Login berhasil!\n";
        return true;
    }
    cout << "Password atau username salah, coba lagi ya!" << endl;
    return false;
}

int main() {
    int pilihan;

    while (true) {
        system("cls");
        cout << "============================" << endl;
        cout << "  WELCOME TO WARM LOAF BAKERY" << endl;
        cout << "============================" << endl;
        cout << "1. Masuk sebagai karyawan" << endl;
        cout << "2. Masuk sebagai pembeli" << endl;
        cout << "Masukkan menu: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            if (loginKaryawan()) menuKaryawan();
        } else if (pilihan == 2) {
            menuPembeli();
        } else {
            cout << "Error! masukkan angka yang sesuai!" << endl;
        }
    }

    return 0;
}