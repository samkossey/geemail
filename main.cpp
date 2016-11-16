#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <stdexcept>
#include <iostream>

#define PASS_SIZE 

//#include "dbhandler.cpp"
#include "secure_functions.cpp"

using namespace std;


int main(int argc, char **argv){
    if (argc == 1){
        try{
            string Option;
            string Username;
            secure_string Password;
            
            
            cout << "Log in or Create Account?" << endl;
            
            cout << "Username: " << endl;
            Password.set("test");
            cout << Password.get() << endl;
            //Password->string_ = "Passw0rd";
            //cout << "Password: " << Password->get() << endl;
            
        } catch (invalid_argument const &e){
            cout << "Argument Error: " << e.what() << endl;
            return 1;
        } catch (bad_alloc const &e){
            cout << "Memory Error: " << e.what() << endl;
            return 2;
        } catch (runtime_error const &e){
            cout << "Runtime Error: " << e.what() << endl;
            return 3;
        } catch (exception const &e){
            cout << "Other Error: " << e.what() << endl;
            return 4;
        }
    }
    return 0;
}