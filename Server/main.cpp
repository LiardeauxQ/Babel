/*
** EPITECH PROJECT, 2019
** main.cpp
** File description:
** Main file.
*/

#include "ServerApplication.hpp"
#include "ServerConfig.hpp"
#include "User.hpp"
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

        User user(conn, 2);

        std::cout << "username: " << user.getName() << std::endl;
        std::cout << "password: " << user.getPassword() << std::endl;
        std::cout << "id: " << user.getId() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Server failed: " << e.what() << std::endl;
    }
}
