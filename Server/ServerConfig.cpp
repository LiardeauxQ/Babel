//
// Created by alex on 9/23/19.
//

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(const char* path)
    : configPath_(path)
    , databaseUrl(DEFAULT_DB)
    , maxUser(DEFAULT_MAX_USER)
    , port(DEFAULT_PORT)
    , host(DEFAULT_HOST)
{
    std::ifstream file(path);

    if (file.good())
        parseFile(file);
}

ServerConfig::ServerConfig()
    : configPath_(DEFAULT_CONFIG_PATH)
{
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
