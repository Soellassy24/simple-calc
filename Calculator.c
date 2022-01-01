#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char inputAngka[100];
float dataAkhir[100];        //Pake array, tapi float
int hitunganData = -1;       //hitungan dari array dataAkhir
int length;
int hasilInt;

char stringTampungAngkaSementara[100];        //string isi angka
int hitunganString = -1;      //hitungan dari string berisi angka
int kasusMinus = 1;

void push_num(){ //semisal ada 2 atau lebih digit, maka fungsi ini dalam stringTampungAngkaSementara[], akan disatukan di dalam dataAkhir[]
   if(hitunganString > -1){
      dataAkhir[++hitunganData] = atof(stringTampungAngkaSementara) * kasusMinus;
      hitunganString = -1;
      kasusMinus = 1;
      memset(stringTampungAngkaSementara, 0, sizeof(stringTampungAngkaSementara)); //stringTampungAngkaSementara akan diisi dengan 0 sesuai dengan panjang string stringTampungAngkaSementara (reset proses)
   }
}
void push_op(char op){ //menerapkan konsep mundur 1, lakukan operasi, mundur 1, operasi
    float a = dataAkhir[hitunganData - 1];
    float b = dataAkhir[hitunganData];
    float c = 0;
    if(op == '+'){
        c = a + b;
    }else if(op == '-'){
        c = a - b;
    }else if(op == '*'){
        c = a * b;
    }else if(op == '/'){
        c = a / b;
    }
    dataAkhir[--hitunganData] = c;
}
void removeSpaces(char inputAngka[], int length){
    for(int i = 0; i < length; i++){  
        if(inputAngka[i] == ' '){  
            for(int j = i; j < length; j++){  
                inputAngka[j]=inputAngka[j+1];  
            }  
        length--;  
        }
    }
}
void validation(char inputAngka[], int length){
    int parenthesisCount = 0;

    for(int i = 0; i < length; i++){
        if (inputAngka[i] == ' '){ //percabangan if untuk kasus spasi kelebihan
            printf("Masukkan spasi secara benar!");
            exit(0);
        }
        if (inputAngka[i] == '('){ //operator sebelum kurung
            if (inputAngka[i-1] == '\0'){
                continue;
            }
            if (inputAngka[i-1] == '+' || inputAngka[i-1] == '-' || inputAngka[i-1] == '*' || inputAngka[i-1] == '/' || inputAngka[i-1] == '('){
                continue;
            }else{
                printf("Ketik Operator sebelum tanda kurung!");
                exit(0);
            }
        }
        if(inputAngka[i] == ')'){ //operator setelah kurung
            if (inputAngka[i+1] == '\0'){
                continue;
            }
            if (inputAngka[i+1] == '+' || inputAngka[i+1] == '-' || inputAngka[i+1] == '*' || inputAngka[i+1] == '/' || inputAngka[i+1] == ')'){
                continue;
            }else{
                printf("Ketik Operator setelah tanda kurung!");
                exit(0);
            }
        }
        if (inputAngka[i] == '('){  //hitung kurung, jika '(', maka +1
            parenthesisCount++;
            continue;
        }
        if (inputAngka[i] == '+' || inputAngka[i] == '-' || inputAngka[i] == '*' || inputAngka[i] == '/'){ //operator tidak boleh berdempetan
            if (inputAngka[i+1] == '+' || inputAngka[i+1] == '-' || inputAngka[i+1] == '*' || inputAngka[i+1] == '/'){
                printf("Operator tidak valid");
                exit(0);
            }else{ //hanya boleh berupa buka kurung dan angka (digit selanjutnya)
                if(inputAngka[i+1] == '('){continue;}
                if (inputAngka[i+1] >= '0' && inputAngka[i+1] <= '9'){continue;}
            }
        }
        if(inputAngka[i] == ')'){ //hitung kurung, jika ')', maka -1
            parenthesisCount--;
            continue;
        }
    }
    if(parenthesisCount != 0){ //jika hasil akhirnya bukan 0, maka program akan salah
        printf("Kurung tidak tepat jumlahnya");
        exit(0);
    }
}
int main(){
    char inputAngka[100]; //= "-20.5+(100-(3+2)*8)/(8-5)-10";
    printf("Masukkan operasi hitung : ");
    scanf("%[^\n]s", &inputAngka);
    length = strlen(inputAngka);

    char *tmp;
    tmp = (char*)inputAngka;
    char stringOperator[100];
    int hitunganOperator = -1; //dimulai dari -1, supaya pas diincrement menyesuaikan sama hitungan array
    int last_tag = 1; //0 ketika selanjutnya masih angka, 1 ketika selanjutnya operator
    while(*tmp != '\0'){ //ketika pointer tmp tidak sama dengan NULL
        char ch = *tmp;
        ++tmp;
        removeSpaces(inputAngka, length);
        validation(inputAngka, length);
        if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '('){  //ketika ch = operator/kurung buka, maka digit langsung di pushnum()
            push_num();
            if((ch == '-') && last_tag ){  //kalo karakter pertama minus, dan selanjutnya 
                kasusMinus = -1;//diganti jadi -1
                last_tag = 0;//diganti jadi 0
                continue;
            }
            last_tag = 1; //diubah menjadi 1, karena sedang distep operator.
            while(hitunganOperator > -1){
                if(ch == '('){ //buka kurung pada ch, akan dibreak terlebih dahulu, karena pasti ada angka lagi
                    break;
                }
                char op = stringOperator[hitunganOperator];
                if(op == '('){ //pada kasus buka kurung, kemudian buka kurung lagi
                    break;
                }
                if(ch == '+' || ch == '-'){ //jika tidak ada operator selain * dan /, maka akan dioperasikan langsung
                    push_op(op); 
                    --hitunganOperator;
                }else{
                    if(op == '+' || op == '-'){ // sedangkan jika ada * dan /, tapi ada tambah kurang, dibreak dulu
                        break;
                    }else{ //kali bagi langsung dioperasikan
                    push_op(op);
                    --hitunganOperator;
                    }
                }
            }
            stringOperator[++hitunganOperator] = ch; //operator ditambahkan kedalam stringOperator
        }else if(ch == ')'){
            push_num();
            last_tag = 0;
            while(stringOperator[hitunganOperator] != '('){
                push_op(stringOperator[hitunganOperator]); //operasi dalam kurung terlebih dahulu dilakukan di sini
                --hitunganOperator;            //array dalam dataAkhir[] akan berubah sesuai operasi yg ada dalam kurung
            }
        --hitunganOperator;
        }else{ //jika tidak ada operator/buka kurung, maka angka bentuk string akan disimpan di stringTampungAngkaSementara
            last_tag = 0;
            stringTampungAngkaSementara[++hitunganString] = ch;
        }
    }// end of while *tmp
    push_num();//The final data is pushed into the stack (semua angka setelah perkalian pembagian kelar, baru dimasukin ke array dataAkhir[] )
    while(hitunganOperator > -1){//The final operator is pushed into the stack (pengerjaan dari kiri ke kanan)
        push_op(stringOperator[hitunganOperator]);
        --hitunganOperator;
    }

    printf("\n%s = ", inputAngka);

    hasilInt = (int)(dataAkhir[0]); //aslinya bisa diprint jadi float, untuk antisipasi pembagian yang ga habis
    //printf("%.2f\n", dataAkhir[0]);
    printf("%d\n", hasilInt);
    return 0;
}

//inputAngka = semua input string mentahan di situ
//stringOperator = string yang isinya operator doang
//*tmp = char khusus untuk nentuin operator selanjutnya
//pushnum() = buat masukin angka DOANG ke dataAkhir[]
//hitunganOperator = int khusus menghitung character non angka
//hitunganString = indeks dari array stringTampungAngkaSementara[], jadi misal ada 3 digit, makan array akan berisi 0,1,2
                                                                                                                //  1,2,1
//hitunganData
//ch = karakter per karakter secara mentah
//op = karakter per karakter secara mentah, tapi kalo operator, dieksekusi secara sendiri

//Ikhwanul Abiyu - Dec 2021
