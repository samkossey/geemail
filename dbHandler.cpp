#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>  

#include "message.cpp"

#define UsersTable "users"
#define MessagesTable "messages"

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
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   //printf("\n");
   return 0;
}

static int callbackRM(void *data, int argc, char **argv, char **azColName){
    int i;
    //fprintf(stderr, "%s: ", (const char*)data);
    A *tableData = (A *)data ;
    tableData->b = argv[argc - 1];
    return 0;
}

static int callbackString(void *data, int argc, char **argv, char **azColName){
    int i;
    //fprintf(stderr, "%s: ", (const char*)data);
    A *tableData = (A *)data ;
    for(i=0; i<argc; i++){
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        tableData->b = argv[i];
    }
    //printf("\n");
    return 0;
}

static int callbackVector(void *data, int argc, char **argv, char **azColName){
    int i;
    //fprintf(stderr, "%s: ", (const char*)data);
    B *vData = (B *)data ;
    for(i=0; i<argc; i++){
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        vData->s.push_back(argv[i]);
    }
    //printf("\n");
    return 0;
}

static int callbackInt(void *data, int argc, char **argv, char **azColName){
    int i;
    //fprintf(stderr, "%s: ", (const char*)data);
    A *numberHolder = (A *)data ;
    numberHolder->c = argc;
    //printf("\n");
    return 0;
}

static int callbackCount(void *data, int argc, char **argv, char **azColName){
    int i;
    //fprintf(stderr, "%s: ", (const char*)data);
    A *numberHolder = (A *)data ;
    numberHolder->c = stoi(argv[0]);
    //printf("\n");
    return 0;
}

static int callbackTE(void *data, int argc, char **argv, char **azColName){
    int i;
    //fprintf(stderr, "%s: ", (const char*)data);
    A *numberHolder = (A *)data ;
    int j = stoi(argv[0]);
    numberHolder->c = j;
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
        throw runtime_error("Can't Open Database");
    };
    string sql = "select count(*) from sqlite_master where type='table' and name='" + string(UsersTable) +"';";
    rc = sqlite3_exec(db, sql.c_str(), callbackTE, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in checking for user table");
    }
    if (numberHolder.c == 0){
        sql = "create table users( ID integer primary key autoincrement, UserName varchar, ";
        sql += "Password varchar, Salt varchar);";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in creating user table");
    }
    }
    
    sql = "select count(*) from sqlite_master where type='table' and name='" + string(MessagesTable) +"';";
    rc = sqlite3_exec(db, sql.c_str(), callbackTE, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in checking for messages table");
    }
    
    if (numberHolder.c == 0){
        sql = "create table messages( MID integer primary key autoincrement, FRM varchar, ";
        sql += "TOM varchar, Message text, Salt varchar);";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in creating messages table");
    }
    }
    sqlite3_close(db);
}

bool userExists(string UserName){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select count(*) from " + string(UsersTable) +" where Username='" + UserName + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackCount, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in checking if a user exists");
    }
    // }else{
    //   fprintf(stdout, "Welcome!\n");
    // }
    sqlite3_close(db);
    if (numberHolder.c == 0){
        return false;
    }
    else return true;
}

void createUser(string Username, string Password, string Salt){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    string data = "Callback function called";
    //string* test;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "insert into " + string(UsersTable) + " ('USERNAME', 'PASSWORD', 'SALT') values ('" + Username 
    + "', '" + Password + "', '" 
    + Salt + "');";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error creating new user");
    }
    sqlite3_close(db);
}

vector<string> getUsers(){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    B vData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select username from " + string(UsersTable) + ";";
    rc = sqlite3_exec(db, sql.c_str(), callbackVector, &vData, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error getting usernames");
    }
    sqlite3_close(db);
    return vData.s;
}

string getPassword(string Username){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A tableData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select password from " + string(UsersTable) +" where username='" + Username + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackString, &tableData, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("error in getting the password");
    }
    sqlite3_close(db);
    return tableData.b;
}

string getSalt(string Username){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A tableData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select salt from " + string(UsersTable) + " where username='" + Username + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackString, &tableData, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in getting the salt");
    }
    sqlite3_close(db);
    return tableData.b;
}

int getMessageCount(string UserName){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A numberHolder;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select count(*) from " + string(MessagesTable) + " where tom='" + UserName + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackCount, &numberHolder, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in checking the messages count");
    }
    sqlite3_close(db);
    return numberHolder.c;
}

void sendMessage(message message){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    string data = "Callback function called";
    //string* test;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "insert into " + string(MessagesTable) + "(TOM, FRM, Message) values ('" + message.To
    + "', '" + message.From + "', " + message.Message + ");";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in sending a message");
    }
    sqlite3_close(db);
}

vector<string> messagesFrom(string myusername){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    B vData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select Frm from " + string(MessagesTable) + " where ";
    sql += "Tom='" + myusername + "';";
    rc = sqlite3_exec(db, sql.c_str(), callbackVector, &vData, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error in retreiving the user's inbox");
    }
    sqlite3_close(db);
    return vData.s;
}

string getRecentMessage(string myusername, string from){
    tablesExist();
    sqlite3* db;
    char *zErrMsg = 0;
    int rc;
    A tableData;
    rc = sqlite3_open("geemail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        throw runtime_error("Can't Open Database");
    };
    string sql = "select message from " + string(MessagesTable) + " where Tom='" + myusername;
    sql += "' AND Frm='" + from + "');";
    rc = sqlite3_exec(db, sql.c_str(), callbackRM, &tableData, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      throw runtime_error("Error getting the most recent message from the given user");
    }
    sqlite3_close(db);
    return tableData.b;
}

int maindb(){
    //unsigned char hash[4]={0x00,0x10,0x20,0x30};
    //cout << createUserString("Sam", hash, hash);
    cout << "message count " << getMessageCount("trevor") << endl;
    return 0;
}