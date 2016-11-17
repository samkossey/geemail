#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "message.cpp"

using namespace std;

string UsersTable = "users";
string MessagesTable = "messages";

struct A{
    string b;
    int c;
};

struct B{
    vector<string> s;
};


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int callbackString(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    A *tableData = (A *)data ;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        tableData->b = argv[i];
    }
    printf("\n");
    return 0;
}

static int callbackVector(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    B *vData = (B *)data ;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        vData->s.push_back(argv[i]);
    }
    printf("\n");
    return 0;
}

static int callbackInt(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    A *numberHolder = (A *)data ;
    numberHolder->c = argc;
    printf("\n");
    return 0;
}

static int callbackTE(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    A *numberHolder = (A *)data ;
    int j = stoi(argv[0]);
    numberHolder->c = j;
    printf("\n");
    return 0;
}

void tablesExist(){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    };
    string sql = "select count(*) from sqlite_master where type='table' and name='" + UsersTable +";";
    rc = sqlite3_exec(db, sql.c_str(), callbackTE, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Checked for user table\n");
    }
    if (numberHolder.c == 0){
        sql = "create table users( ID integer primary key autoincrement, UserName varchar, ";
        sql += "Password varchar, Salt varchar);";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Created users table!\n");
    }
    }
    
    sql = "select count(*) from sqlite_master where type='table' and name='" + MessagesTable +";";
    rc = sqlite3_exec(db, sql.c_str(), callbackTE, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Checked for message table\n");
    }
    
    if (numberHolder.c == 0){
        sql = "create table messages( ID integer primary key autoincrement, FRM varchar, ";
        sql += "TOM varchar, Message text, Salt varchar);";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Created messages table!\n");
    }
    }
    sqlite3_close(db);
}

bool userExists(string UserName){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    };
    string sql = "select count(*) from " + UsersTable +" where Username='" + UserName + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackInt, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Welcome!\n");
    }
    sqlite3_close(db);
    if (numberHolder.c == 0){
        return false;
    }
    else return true;
}

void createUser(string Username, string Password, unsigned char* Salt){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    string data = "Callback function called";
    //string* test;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    };
    string sql = "insert into " + UsersTable + " ('NAME', 'PASSWORD', 'SALT') values ('" + Username 
    + "', '" + Password + "', '" 
    + (reinterpret_cast<char*>(Salt)) + "');";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Registered Successfully!\n");
    }
    sqlite3_close(db);
}

vector<string> getUsers(){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    B vData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    };
    string sql = "select username from " + UsersTable;
    rc = sqlite3_exec(db, sql.c_str(), callbackVector, &vData, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Welcome!\n");
    }
    sqlite3_close(db);
    return vData.s;
}

string getPassword(string Username){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A tableData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    };
    string sql = "select password from " + UsersTable +" where username='" + Username + "');";
    rc = sqlite3_exec(db, sql.c_str(), callbackString, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Registered Successfully!\n");
    }
    sqlite3_close(db);
    return tableData.b;
}

string getSalt(string Username){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A tableData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    };
    string sql = "select salt from " + UsersTable + " where username='" + Username + "');";
    rc = sqlite3_exec(db, sql.c_str(), callbackString, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Registered Successfully!\n");
    }
    sqlite3_close(db);
    return tableData.b;
}

int getMessageCount(string UserName){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    };
    string sql = "select count(*) from " + MessagesTable + " where to='" + UserName + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackInt, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Welcome!\n");
    }
    sqlite3_close(db);
    return numberHolder.c;
}

void sendMessage(message message){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    string data = "Callback function called";
    //string* test;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    };
    string sql = "insert into " + MessagesTable + "(To, From, Message) values ('" + message.To
    + "', '" + message.From + "', " + message.Message + ");";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Message sent!\n");
    }
    sqlite3_close(db);
}

//message getMessages(string username){
    
//}

int main(){
    //unsigned char hash[4]={0x00,0x10,0x20,0x30};
    //cout << createUserString("Sam", hash, hash);
    return 0;
}