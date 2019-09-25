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

    explicit Database(const std::string& url)
        : connection_(nullptr)
        , errMsg_(nullptr)
    {
        std::ifstream file;
        file.open(url);

        int error = sqlite3_open(url.c_str(), &connection_);

        if (error)
            throw DatabaseError(error);

        if (file.fail()) {
            std::cout << "Database doesn't exist." << std::endl;
            setupDb();
        } else if (!isValidDb()) {
            std::cout << "Invalid database recreating one." << std::endl;
            sqlite3_close(connection_);
            remove(url.c_str());

            error = sqlite3_open(url.c_str(), &connection_);

            if (error)
                throw DatabaseError(error);

            setupDb();
        }
    }

    void exec(const std::string& request, sqlCallback callback, void* data)
    {
        int error = sqlite3_exec(connection_, request.c_str(), callback, data, &errMsg_);

        if (error)
            throw DatabaseError(error);
    }

    void exec(const std::string& request)
    {
        int error = sqlite3_exec(connection_, request.c_str(), nullptr, nullptr, &errMsg_);

        if (error)
            throw DatabaseError(error);
    }

private:
    // Check if the database contain all tables.
    bool isValidDb()
    {
        bool user_present = false;
        bool friendship_present = false;

        this->exec(CHECK_USERS_TABLE, checkTable, &user_present);
        std::cout << "`users` table ok." << std::endl;
        this->exec(CHECK_FRIENDSHIP_TABLE, checkTable, &friendship_present);
        std::cout << "`friendship` table ok." << std::endl;
        return user_present && friendship_present;
    }

    // Set the local data to true.
    static int checkTable(void* isPresent, int argc, char** argv, char** azColName)
    {
        auto t = (bool*)isPresent;
        *t = true;

        return 0;
    }

    // Create all necessary table for a virgin database.
    void setupDb()
    {
        std::cout << "Setup of the new database." << std::endl;
        this->exec(CREATE_USERS_TABLE, nullptr, nullptr);
        std::cout << "`users` table created successfully." << std::endl;
        this->exec(CREATE_FRIENDSHIP_TABLE, nullptr, nullptr);
        std::cout << "`friendship` table created successfully." << std::endl;
    }

    sqlite3* connection_;
    char* errMsg_;
};

#endif //BABEL_SERVER_DATABASE_HPP
