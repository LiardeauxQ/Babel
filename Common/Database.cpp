//
// Created by alex on 9/23/19.
//

#include "Database.hpp"

static const char* CREATE_USERS_TABLE = "CREATE TABLE `users` (`id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, `username` TEXT NOT NULL UNIQUE, `password` TEXT NOT NULL)";
static const char* CREATE_FRIENDSHIP_TABLE = "CREATE TABLE `friendship` (`user` INT NOT NULL, `friend` INT NOT NULL, FOREIGN KEY (user) REFERENCES users(id), FOREIGN KEY (friend) REFERENCES users(id))";
static const char* CHECK_USERS_TABLE = "SELECT name FROM sqlite_master WHERE type='table' AND name='users'";
static const char* CHECK_FRIENDSHIP_TABLE = "SELECT name FROM sqlite_master WHERE type='table' AND name='friendship'";

Database::Database(const std::string& url)
    : connection_(nullptr)
    , errMsg_(nullptr)
{
    std::cout << "[DATABASE] Looking for database." << std::endl;
    std::ifstream file;
    file.open(url);

    int error = sqlite3_open(url.c_str(), &connection_);

    if (error)
        throw DatabaseError(error);

    if (file.fail()) {
        std::cout << "[DATABASE] Database doesn't exist." << std::endl;
        setupDb();
    } else if (!isValidDb()) {
        std::cout << "[DATABASE] Invalid database recreating one." << std::endl;
        sqlite3_close(connection_);
        remove(url.c_str());

        error = sqlite3_open(url.c_str(), &connection_);

        if (error)
            throw DatabaseError(error);

        setupDb();
    }
    std::cout << "[DATABASE] Database ready." << std::endl;
}

void Database::exec(const std::string& request, sqlCallback callback, void* data)
{
    int error = sqlite3_exec(connection_, request.c_str(), callback, data, &errMsg_);

    if (error)
        throw DatabaseError(error);
}

void Database::exec(const std::string& request)
{
    int error = sqlite3_exec(connection_, request.c_str(), nullptr, nullptr, &errMsg_);

    if (error)
        throw DatabaseError(error);
}

bool Database::isValidDb()
{
    bool user_present = false;
    bool friendship_present = false;

    this->exec(CHECK_USERS_TABLE, checkTable, &user_present);
    std::cout << "  -> `users` table [OK]" << std::endl;
    this->exec(CHECK_FRIENDSHIP_TABLE, checkTable, &friendship_present);
    std::cout << "  -> `friendship` table [OK]." << std::endl;
    return user_present && friendship_present;
}

// Set the local data to true.
int Database::checkTable(void* isPresent, int argc, char** argv, char** azColName)
{
    auto t = (bool*)isPresent;
    *t = true;

    return 0;
}

// Create all necessary table for a virgin database.
void Database::setupDb()
{
    std::cout << "  -> Setup of the new database" << std::endl;
    this->exec(CREATE_USERS_TABLE, nullptr, nullptr);
    std::cout << "      -> `users` table created successfully" << std::endl;
    this->exec(CREATE_FRIENDSHIP_TABLE, nullptr, nullptr);
    std::cout << "      -> `friendship` table created successfully" << std::endl;
}
