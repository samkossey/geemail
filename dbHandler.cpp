#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

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

bool userExists(string Username){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    };
    string sql = "select count(*) from users where Username='" + UserName + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackInt, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Welcome!\n");
    }
    sqlite3_close(db);
    if (numberHolder->c == 0){
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
        return 1;
    };
    string sql = "insert into USERS ('NAME', 'PASSWORD', 'SALT') values ('" + Username 
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
        return 1;
    };
    string sql = "select username from users";
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
        return 1;
    };
    string sql = "select password from users where username='" + Username + "');";
    rc = sqlite3_exec(db, sql.c_str(), callbackString, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Registered Successfully!\n");
    }
    sqlite3_close(db);
    return tableData->b;
}

unsigned char* getSalt(string Username){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A tableData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    };
    string sql = "select salt from users where username='" + Username + "');";
    rc = sqlite3_exec(db, sql.c_str(), callbackString, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Registered Successfully!\n");
    }
    sqlite3_close(db);
    return tableData->b;
}

int getMessageCount(string Username){
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    };
    string sql = "select count(*) from messages where to='" + UserName + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackInt, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Welcome!\n");
    }
    sqlite3_close(db);
    return numberHolder->c;
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
        return 1;
    };
    string sql = "insert into Messages(To, From, Message) values ('" + message.To
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
    unsigned char hash[4]={0x00,0x10,0x20,0x30};
    cout << createUserString("Sam", hash, hash);
}