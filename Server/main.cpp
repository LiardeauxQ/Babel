/*
** EPITECH PROJECT, 2019
** main.cpp
** File description:
** Main file.
*/

#include "ServerApplication.hpp"
#include "ServerConfig.hpp"
#include "User.hpp"

int main(int argc, char* argv[])
{
    try {
        ServerConfig config("./server.cfg");

        ServerApplication server(config);

        server.run();

    } catch (const std::exception& e) {
        std::cerr << "Server failed: " << e.what() << std::endl;
    }
}


