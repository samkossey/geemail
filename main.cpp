#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <stdexcept>
#include <iostream>

#define PASS_SIZE 

//#include "dbhandler.cpp"
#include "secure_functions.cpp"
#include "authentication.cpp"
#include "hashing.cpp"
#include "dbHandler.cpp"

using namespace std;


int main(int argc, char **argv){
    if (argc == 1){
        try{
            bool loggedin = false;
            string Option = "";
            string Username = "";
            string password = "";
            string PasswordHash = "";
            string salt = "";
            //secure_string Password;
            while(!loggedin){
                cout << "Username: ";
                cin >> Username;
                cin.get();
                //Username = "hi";
                //fseek(stdin,0,SEEK_END);
                if(userExists(Username)){
                    password=getpass("Password: ",true);
                    salt = getSalt(Username);
                    PasswordHash = hashIt(password+salt);
                    if(getPassword(Username) == PasswordHash){
                        loggedin = true;
                    } else {
                        cout << "Invalid Password" << endl;
                    }
                } else {
                    cout << "User doesn't exist, Creating account..." << endl;
                    password=getpass("New Password: ",true);
                    if(password == getpass("Verify Password: ",true)){
                        cout << "Creating user..." << endl;
                        salt=newSalt();
                        createUser(Username,hashIt(password+salt),salt);
                        cout << "User Created Successfully, Please login" << endl;
                    } else {
                        cout << "Passwords didn't match, please try again" << endl;
                    }
                }
            }
            
            bool done = false;
            string action = "";
            cout << endl << endl;
            system("clear");
            cout << "=========================================" << endl;
            cout << "           Welcome to GEE-MAIL!" << endl;
            cout << "=========================================" << endl;
            while(!done){
                cout << "You have " << getMessageCount(Username) << " message(s)." << endl;
                cin >> action;
            }
            
            //out << "Log in or Create Account?" << endl;
            
            // cout << "Username: ";
            // cin >> Username;
            // if userExists(Username)
            // password=getpass("Password: ",true);
            // PasswordHash = hashIt(password)
            
            //Password.set("test");
            //out << Password.get() << endl;
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