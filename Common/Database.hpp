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

static const char* CREATE_USERS_TABLE = "CREATE TABLE `users` (`id` INT PRIMARY KEY, `username` TEXT NOT NULL,`password` TEXT NOT NULL)";
static const char* CREATE_FRIENDSHIP_TABLE = "CREATE TABLE `friendship` (`user` INT NOT NULL, `friend` INT NOT NULL, FOREIGN KEY (user) REFERENCES users(id), FOREIGN KEY (friend) REFERENCES users(id))";
static const char* CHECK_USERS_TABLE = "SELECT name FROM sqlite_master WHERE type='table' AND name='users'";
static const char* CHECK_FRIENDSHIP_TABLE = "SELECT name FROM sqlite_master WHERE type='table' AND name='friendship'";

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
