#include "UserTable.h"

#include <iostream>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>
#include <vector>

void handle_request(sqlite3*);

int main(int argc, char** argv){
    sqlite3 *db;
    int rc = sqlite3_open("amazoff.db", &db);    
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    while(true){
        handle_request(db);
    }

}

void handle_request(sqlite3 *db){ 
    UserTable userTable(db);
    std::cout << "Press:\n"                         \
                    "\t0: Add user\n"               \
                    "\t1: Update user balance\n"    \
                    "\t2: Print all users\n"        \
                    "\t3: Quit\n"; 
    uint32_t type = ~0;
    std::cin >> type;

    switch(type) {
        case 0: {
                    std::cout << "Please enter the user's name:\n";
                    char name[128];
                    std::cin >> name;
                    
                    std::cout << "Please enter the user's balance\n";
                    int balance;
                    std::cin >> balance;
                    int id = userTable.addUser(name, balance); 
                    std::cout << "New user's id: " << id << "\n";

                    break;
                }
        case 1: {
                    std::cout << "Please enter the user's id:\n";
                    int id;
                    std::cin >> id;
                    
                    std::cout << "Please enter the user's new balance\n";
                    int balance;
                    std::cin >> balance;
                    userTable.updateUserBalance(id, balance); 
                    break;
                }
        case 2: {
                    userTable.printAll();
                    break;
                }   
        case 3: {
                    sqlite3_close(db);
                    exit(0);
                }
    }

}
