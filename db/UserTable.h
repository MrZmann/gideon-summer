#pragma once
#include <sqlite3.h>

class UserTable {
public:
    UserTable (sqlite3* db) :
        db(db) 
    {
        // Create table if it doesn't exist

    }

    int addUser(char* name, int balance);

    bool updateUserName(int id, char* name);

    bool updateUserBalance(int id, int balance);

    void printAll();

    
private:
    sqlite3* db;
};
