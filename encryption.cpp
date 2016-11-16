#include <stdio.h>
#include <gcrypt.h>
#include <iostream>
#include <bitset>
#include "hashing.cpp"
using namespace std;

void encrypt (string pw) {
    gcry_error_t     gcryError;
    gcry_cipher_hd_t gcryCipherHd;
    size_t           index;
    
    const char * salsaKey = hash(pw, pw.length()); // 32 bytes
    const char * iniVector = "public?"; // 8 bytes

    gcryError = gcry_cipher_open(
        &gcryCipherHd, // gcry_cipher_hd_t *
        GCRY_CIPHER_SALSA20,   // int
        GCRY_CIPHER_MODE_STREAM,   // int
        0);            // unsigned int
    if (gcryError)
    {
        printf("gcry_cipher_open failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        //return 0;
    }
    printf("gcry_cipher_open worked\n");
    
    gcryError = gcry_cipher_setkey(gcryCipherHd, salsaKey, 32);
    if (gcryError)
    {
        printf("gcry_cipher_setkey failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        //return 0;
    }
    printf("gcry_cipher_setkey worked\n");
    
    gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, 8);
    if (gcryError)
    {
        printf("gcry_cipher_setiv failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        //return 0;
    }
    printf("gcry_cipher_setiv worked\n");
    
    size_t txtLength = 101;
    //char * encBuffer = (char*)malloc(txtLength);
    char * textBuffer = (char*)malloc(txtLength);
    memset(textBuffer, 0, 101);

    gcryError = gcry_cipher_encrypt(
        gcryCipherHd, // gcry_cipher_hd_t
        encBuffer,    // void *
        txtLength,    // size_t
        textBuffer,    // const void *
        txtLength);   // size_t
    if (gcryError)
    {
        printf("gcry_cipher_decrypt failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        //return 0;
    }
    printf("gcry_cipher_decrypt worked\n");
    
    printf("encBuffer = ");
    for (index = 0; index<txtLength-1; index++)
        printf("%02X", (unsigned char)encBuffer[index]);
    printf("\n");
    const char* a = "hi";
    //unsigned char b = bitset<2>("hi");
    //cout << b << endl;
    //const char* b = (const char*)encBuffer;
    //unsigned char test = b ^ a;
    //cout <<  test << endl;
    //return encBuffer;
}

int main(){
    const char* a = "hello";
    encrypt(a);
    return 0;
}