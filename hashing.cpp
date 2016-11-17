#include <stdio.h>
#include <gcrypt.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

string hashIt(string input){
    char hash[65] = "";
    size_t index;
    size_t txtLength = input.length();
    char * hashBuffer = (char*) malloc(33);
    char * textBuffer = (char*) malloc(txtLength+1);
    memset(hashBuffer, 0, 33);

    strncpy(textBuffer, input.c_str(), txtLength);

    gcry_md_hash_buffer(
        GCRY_MD_SHA256, // gcry_cipher_hd_t
        hashBuffer,    // void *
        textBuffer,    // const void *
        txtLength);   // size_t

    //printf("hashBuffer = ");
    for (index = 0; index<32; index++)
        sprintf(hash+index*2, "%02X", (unsigned char)hashBuffer[index]);
    //printf("\n");
    return (string) hash;
}

string newSalt(){
    //TODO: Make this actually have a salt...
    return "00000000";
}

// string hash (const char* pw, size_t txtLength) {
//     size_t index;
//     char * hashBuffer = (char *)malloc(33);
//     char * textBuffer = (char *)malloc(txtLength+1);
//     memset(hashBuffer, 0, 33);

//     strncpy(textBuffer, pw, txtLength);

//     gcry_md_hash_buffer(
//         GCRY_MD_SHA256, // gcry_cipher_hd_t
//         hashBuffer,    // void *
//         textBuffer,    // const void *
//         txtLength);   // size_t

//     char a[64];
//     int currIndex = 0;
//     for (index = 0; index<32; index++){
//         char b[2];
//         snprintf(b, 3, "%02X", (unsigned char)hashBuffer[index]);
//         a[currIndex] = b[0];
//         a[currIndex + 1] = b[1];
//         currIndex = currIndex + 2;
//     }
//     //string c = a;
//     return (string) a;
// }

//int main(){
//     cout << hash("sam", 3) << endl;
//     return 0;
// }


