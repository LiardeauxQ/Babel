/*
** EPITECH PROJECT, 2019
** main.cpp
** File description:
** Main file.
*/

#include "ServerApplication.hpp"
#include "ServerConfig.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        ServerConfig config {
            .databaseUrl = "./data.db",
            .maxUser = 100
        };

        ServerApplication server(config);

        auto conn = server.getConnection();

    } catch (const std::exception& e) {
        std::cerr << "Server failed: " << e.what() << std::endl;
    }
}
