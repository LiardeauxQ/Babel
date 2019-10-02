//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_DATABASE_HPP
#define BABEL_SERVER_DATABASE_HPP

#include "DatabaseError.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>

class Database {
public:
    typedef int (*sqlCallback)(void* data, int argc, char** argv, char** colName);

    explicit Database(const std::string& url);

    void exec(const std::string& request, sqlCallback callback, void* data);
    void exec(const std::string& request);
private:
    bool isValidDb();
    static int checkTable(void* isPresent, int argc, char** argv, char** azColName);
    void setupDb();

    sqlite3* connection_;
    char* errMsg_;
};

#endif //BABEL_SERVER_DATABASE_HPP
