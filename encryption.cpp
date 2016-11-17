#include <stdio.h>
#include <gcrypt.h>
#include <iostream>
#include <bitset>
#include <string.h>

#include "hashing.cpp"

using namespace std;

unsigned char* create_stream (string hash, string nonce, int messageLength) {
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
    
    size_t txtLength = messageLength;
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
        sprintf(stream+index*2, "%02X", (unsigned char)encBuffer[index]);
    }
        //sprintf("%02X", (unsigned char)encBuffer[index]);
        
    cout << (string) stream << endl;
    //TODO: bitwise xor the stream with the text
    
    //const char* a = "hi";
    //unsigned char b = bitset<2>("hi");
    //cout << b << endl;
    //const char* b = (const char*)encBuffer;
    //unsigned char test = b ^ a;
    //cout <<  test << endl;
    return (unsigned char*)encBuffer;
    //return string(stream);
}

string encrypt (string hash, string nonce, string message) {
    unsigned char* keystream = create_stream(hash, nonce, message.length());
    const char* messageBytes = message.c_str();
    unsigned char* outputBuffer = (unsigned char*)malloc(sizeof(string)*message.length());
    for(int i=0; i < message.length(); i++){
        //cout << "messageBytes[i] " << messageBytes[i] << endl;
        //cout << "keystream[i] " << keystream[i] << endl;
        outputBuffer[i]=messageBytes[i]^keystream[i];
        //cout << "outputBuffer[i] " << outputBuffer[i] << endl;
    }
    char stream[message.length() * 2];
    for (int index = 0; index<message.length(); index++){
        cout << "OUTPUT FOR INDEX " << (unsigned char)outputBuffer[index] << endl;
        sprintf(stream+index*2, "%02X", (unsigned char)outputBuffer[index]);
    }
    return (string) stream;
    
}

string decrypt (string hash, string nonce, string encryption) {
    unsigned char* keystream = create_stream(hash, nonce, encryption.length()/2);
    const char* encryptedBytes = encryption.c_str();
    unsigned char* preXOR = (unsigned char*)malloc(sizeof(string)*encryption.length()/2);
    for (int k = 0; k < encryption.length() /2; k++){
        char enc = encryptedBytes[k] + encryptedBytes[k+1];
        preXOR[k] = (enc >= 'A')? (enc - 'A' + 10): (enc - '0');
    }
    
    char* outputBuffer = (char*)malloc(sizeof(string)*encryption.length()/2);
    
    for(int i=0; i < encryption.length()/2; i++){
        //cout << "encryptedBytes[i] " << encryptedBytes[i] << endl;
        //cout << "keystream[i] " << keystream[i] << endl;
        outputBuffer[i]=preXOR[i]^keystream[i];
        //cout << "outputBuffer[i] " << outputBuffer[i] << endl;
    }
    // char stream[encryption.length()];
    // for (int index = 0; index<encryption.length()/2; index++){
    //     sprintf(stream+index*2, "%02X", (unsigned char)outputBuffer[index]);
    // }
    return (string) outputBuffer;
}

int main(){
    string a = "ab";
    string b = "hellohel";
    string c = "12345678901234567890123456789012";
    cout << "encrypted text: " << encrypt(c, b, a) << endl;
    string encrypted = encrypt(c,b,a);
    cout << "decryption " << decrypt(c,b,encrypted) << endl;
    //cout << encrypt()
    return 0;
}
//2F349243