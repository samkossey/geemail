#include <stdio.h>
#include <gcrypt.h>
#include <iostream>
#include <bitset>
#include <string.h>

#include "hashing.cpp"

using namespace std;

string create_stream (string hash, string nonce, string message) {
    gcry_error_t     gcryError;
    gcry_cipher_hd_t gcryCipherHd;
    size_t           index;
    
    const char * salsaKey = hash.c_str(); // 32 bytes
    const char * iniVector = nonce.c_str(); // 8 bytes

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
    
    size_t txtLength = message.length();
    char * encBuffer = (char*)malloc(txtLength);
    char * textBuffer = (char*)malloc(txtLength);
    memset(textBuffer, 0, txtLength);

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
    char stream[txtLength * 2];
    int currIndex = 0;
    for (index = 0; index<txtLength-1; index++){
        // char b[2];
        // snprintf(b, 3, "%02X", (unsigned char)encBuffer[index]);
        // stream[currIndex] = b[0];
        // stream[currIndex + 1] = b[1];
        // currIndex = currIndex + 2;
        sprintf(stream+index*2, "%02X", (unsigned char)encBuffer[index]);
        //printf("\n");
    }
        //sprintf("%02X", (unsigned char)encBuffer[index]);
        
    
    //TODO: bitwise xor the stream with the text
    
    //const char* a = "hi";
    //unsigned char b = bitset<2>("hi");
    //cout << b << endl;
    //const char* b = (const char*)encBuffer;
    //unsigned char test = b ^ a;
    //cout <<  test << endl;
    //return encBuffer;
    return string(stream);
}

string encrypt (string hash, string nonce, string message) {
    string keystream = create_stream(hash, nonce, message);
    return keystream;
    
}

string decrypt (string hash, string nonce, string message) {
    string keystream = create_stream(hash, nonce, message);
    return keystream;
}

int main(){
    string a = "hello";
    string b = "hellohel";
    string c = "12345678901234567890123456789012";
    cout << encrypt(c, b, a) << endl;
    //cout << encrypt()
    return 0;
}
//2F349243