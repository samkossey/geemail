#include <stdio.h>
#include <gcrypt.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

string hash (const char* pw, size_t txtLength) {
    size_t index;
    char * hashBuffer = (char *)malloc(33);
    char * textBuffer = (char *)malloc(txtLength+1);
    memset(hashBuffer, 0, 33);

    strncpy(textBuffer, pw, txtLength);

    gcry_md_hash_buffer(
        GCRY_MD_SHA256, // gcry_cipher_hd_t
        hashBuffer,    // void *
        textBuffer,    // const void *
        txtLength);   // size_t

    const char a[64];
    int currIndex = 0;
    for (index = 0; index<32; index++){
        char b[2];
        snprintf(b, 3, "%02X", (unsigned char)hashBuffer[index]);
        a[currIndex] = b[0];
        a[currIndex + 1] = b[1];
        currIndex = currIndex + 2;
    }
    string c = a;
    return c;
}

//int main(){
//     cout << hash("sam", 3) << endl;
//     return 0;
// }