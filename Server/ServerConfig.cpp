//
// Created by alex on 9/23/19.
//

#include "ServerConfig.hpp"

static const char* DEFAULT_DB = "data.db";
static const size_t DEFAULT_MAX_USER = 100;
static const size_t DEFAULT_PORT = 8080;
static const char* DEFAULT_HOST = "0.0.0.0";

ServerConfig::ServerConfig(const char* path)
    : databaseUrl(DEFAULT_DB)
    , maxUser(DEFAULT_MAX_USER)
    , port(DEFAULT_PORT)
    , host(DEFAULT_HOST)
    , configPath_(path)
{
    std::ifstream file(path);

    if (file.good())
        parseFile(file);
}

void ServerConfig::parseFile(std::ifstream& file)
{
    std::string line;

    while (std::getline(file, line)) {
        size_t pos = line.find('=');

        std::string ri(line.begin() + pos + 1, line.end());
        std::string le(line.begin(), line.begin() + pos);

        if (le == "database_url") {
            databaseUrl = ri;
        } else if (le == "port") {
            port = (short)std::stoi(ri);
        } else if (le == "max_user") {
            maxUser = std::stol(ri);
        }
    }
}
