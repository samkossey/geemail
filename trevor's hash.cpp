#include <stdio.h>
#include <gcrypt.h>
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include "FileReader.cpp"


#define FILE "dictionary.txt"

using namespace std;

//given a string, creates a list of the words contained in that string
list<string> parseText(string s){
    list<string> l;
    l.push_back("");
    string currString = "";
    for (int i = 0; i < s.length(); i++){
        //a space indicates the start of a new word
        if (s.at(i) == ' '){
            l.push_back(currString);
            currString = "";
        }
        else{
            //if no space, continue building a word character by character
            currString += s.at(i);
        }
    }
    if (currString != ""){
        l.push_back(currString);
    }
    return l;
}

string hashString (string input){
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

int main () {
    string dict;
    File* file;
    
    string hashtomatch = "A8677DF7AAC4E90428D7226C5D28971273A5DDDB2F0338C963ACC521A6232934";
    
    file = new File(FILE);
    dict = file->readfile();
    delete(file);
    
    list<string> dictlist = parseText(dict);
    
    for (list<string>::const_iterator iterator = dictlist.begin(), end = dictlist.end(); iterator != end; ++iterator) {
        string hash = hashString(*iterator);
        if (hash==hashtomatch){
            cout << "Match: " << hash << endl << "Word: " << ((string) *iterator) << endl;
        }
    }
    
    // for(int i=0; i < dictlist.length(); i++){
    //     hash(dictlist[i]);
    // }
}