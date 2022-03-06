/*
Nama / NIM : Muhammad Rafli Fadilah Yusmar / 13220037
Mata Kuliah / Kelas : EL2208 Pemecahan Masalah dengan C / K01
Program digunakan untuk menyimulasikan algoritma dijkstra yang digunakan pada pemetaan kota di Pulau Jawa.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define node_max 50
#define max_len 255
#define int_max __INT_MAX__

// Fungsi digunakan untuk menampilkan graph berukuran n x n ke layar
void printgraph(int n, int graph[n][n]){
    printf("\n-1 menyatakan jarak tak hingga\n");
    printf("0 menyatakan bahwa kedua vertex sama\n\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(graph[i][j]==int_max){
                printf("%d ",-1);
            }
            else{
            printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
}

// Fungsi digunakan untuk mencari indeks yang akan diproses pada algoritma dijkstra
int idx_process(int n, int jarak_final[n], bool final[n]){
    bool found=false;
    int idx_min, val_minimum;
    for(int i=0;i<n;i++){
        //Inisiasi nilai minimum awal
        if(!final[i]&&!found){
            idx_min=i, val_minimum=jarak_final[i];
            found=true;
        }
        else if(found && !final[i] && jarak_final[i]<val_minimum){
            idx_min=i, val_minimum=jarak_final[i];
        }
    }
    if(found){
        return idx_min;
    }
    else{
        //Jarak minimum ke seluruh vertex telah ditemukan
        return int_max;
    }
}

int main (){
    char namafile_kota[max_len];
    // Input file kota
    printf("Selamat datang di database Kota-Kota Pulau Jawa.\n");
    printf("Silakan masukkan nama file database nama kota di Pulau Jawa: ");
    scanf("%s",&namafile_kota);
    FILE* fp_kota = fopen(namafile_kota,"r");

    // Baca isi file kota
    char baris[max_len];
    char* token;
    int n_kota=0;
    char nama_kota[node_max][max_len];
    while (fgets(baris, max_len, fp_kota)){
        token = strtok(baris,"\n");
        strcpy(nama_kota[n_kota],token);
        n_kota += 1;
    }
    printf("Pembacaan nama kota di Pulau Jawa berhasil.\n");
    printf("Terdapat %d kota di lokasi yang berbeda\n",n_kota);
    // Input graph (berisi jarak masing-masing kota). -1 atau 2147483647=INT_MAX menyatakan tak hingga.
    printf("Silahkan masukkan nama file yang berisi jarak masing-masing kota: ");
    char namafile_graph[max_len];
    scanf("%s",&namafile_graph);
    // File eksternal berbentuk matriks segitiga bawah
    int graph_kota[n_kota][n_kota];
    int j=0,k;
    FILE* fp_jarak_kota = fopen(namafile_graph,"r");
    while (fgets(baris, max_len, fp_jarak_kota)){
        k=0;
        token = strtok(baris," ");
        while (token != NULL) {
            graph_kota[j][k] = atoi(token);
            graph_kota[k][j] = atoi(token);
            if(atoi(token)==-1){
                graph_kota[j][k] = int_max;
                graph_kota[k][j] = int_max;
            }
            k+=1;
            token = strtok(NULL," ");
        }
        j+=1;
    }
    printf("Pembacaan jarak antar kota berhasil dilakukan.\n\n");
    //printgraph(n_kota,graph_kota);
    char progress='y';
    while(progress=='y'){
        // Pembacaan vertex awal dan akhir
        int idx_awal, idx_tujuan;
        printf("\nBerikut adalah daftar kota yang berada di Pulau Jawa.\n");
        for(int i=1;i<=n_kota;i++){
            printf("%d. %s\n",i,nama_kota[i-1]);
        }
        printf("\nPilih salah satu kota sebagai posisi awal (Cukup ketik angka saja): ");
        scanf(" %d",&idx_awal);
        idx_awal-=1;
        printf("Pilih salah satu kota sebagai tujuan (Cukup ketik angka saja): ");
        scanf(" %d",&idx_tujuan);
        idx_tujuan-=1;
        int jarak_final[n_kota]; //Jarak akhir ke masing-masing kota, akan di update terus sesuai jarak yang ditemukan
        bool final[n_kota]; //Memberikan keterangan apakah jarak sudah minimum
        int list_dilalui[n_kota][n_kota+1]; //Indeks yang dilalui hingga mencapai tujuan
        int idx_dilalui[n_kota]; //Mencatat jumlah vertex yang dilalui pada list_dilalui
        //Algoritma Dijsktra
        for(int i=0;i<n_kota;i++){
            //Set jarak menjadi tak hingga pada seluruh vertex selain vertex awal dan
            if(i==idx_awal){
                jarak_final[i]=0;
                final[i]=false;
            }
            else{
                jarak_final[i]=int_max;
                final[i]=false;
            }
            //Set list dilalui menjadi no vertex (dilambangkan dengan int_max)
            for(int j=0;j<=n_kota;j++){
                list_dilalui[i][j]=int_max;
            }
            idx_dilalui[i]=0;
        }
        //Inisiasi jarak awal pada vertex awal
        jarak_final[idx_awal]=0;
        list_dilalui[idx_awal][0]=idx_awal;
        idx_dilalui[idx_awal]+=1;
        int idx_now=idx_awal;
        //Menentukan index yang diproses
        while(idx_now!=int_max){
            final[idx_now]=true;
            //printf("idx process = %d\n",idx_now);
            for(int i = 0;i<n_kota;i++){
                //Update nilai saat jarak lebih pendek
                if(!final[i] && (graph_kota[idx_now][i]!=int_max) &&
                (jarak_final[idx_now]+graph_kota[idx_now][i]<jarak_final[i])){
                    jarak_final[i] = jarak_final[idx_now]+graph_kota[idx_now][i];
                    idx_dilalui[i]=idx_dilalui[idx_now]+1;
                    for(int j=0;j<idx_dilalui[i];j++){
                        if(j==idx_dilalui[i]-1){
                            list_dilalui[i][j]=i;
                        }
                        else{
                            list_dilalui[i][j]=list_dilalui[idx_now][j];
                        }
                    }
                    //printf("Jarak Final[%d] = %d\n",i,jarak_final[i]);
                }
            }
            idx_now = idx_process(n_kota,jarak_final,final);
        }
        printf("\nKota yang harus dilalui untuk mencapai %s dengan posisi awal %s adalah: \n",nama_kota[idx_tujuan],nama_kota[idx_awal]);
        for(int i=0;i<idx_dilalui[idx_tujuan];i++){
            if(list_dilalui[idx_tujuan][i]!=int_max){
                printf("%d. %s\n",i+1,nama_kota[list_dilalui[idx_tujuan][i]]);
            }
        }
        printf("\nJarak antara kedua kota tersebut adalah %d meter\n\n",jarak_final[idx_tujuan]);
        /* // Digunakan untuk debugging
        printf("Jarak final: %d\n",jarak_final[idx_tujuan]);
        printf("Indeks Tujuan: %d\n",idx_tujuan);
        for(int i=0;i<n_kota;i++){
            printf("Jarak Final[%d] = %d\n",i,jarak_final[i]);
        }
        for(int i=0;i<n_kota;i++){
            printf("List dilalui idx %d: ",i);
            for(int j=0;j<n_kota;j++){
                printf("%d ",list_dilalui[i][j]);
            }
            printf("\n");
        }*/
        printf("Apakah anda masih ingin mengelilingi kota-kota di Pulau Jawa? [y/n]: ");
        scanf(" %c",&progress);
    }
}
