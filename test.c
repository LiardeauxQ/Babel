/*
** EPITECH PROJECT, 2019
** test.c
** File description:
** a
*/

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "Common/protocol.h"
#include "/home/alex/.conan/data/portaudio/v190600.20161030/bincrafters/stable/source/sources/include/portaudio.h"

int send_ping(int s) {
    struct {
        request_header_t hdr;
        client_ping_t payload;
    } request = {
        {
            .id = CLIENT_PING,
            .request_len = CLIENT_PING_SIZE
        },
        {
            .stamp = time(NULL)
        }
    };

    write(s, &request, sizeof(request));

    request_header_t hdr = {0};

    read(s, &hdr, sizeof(hdr));

    if (hdr.id != SERVER_PING) {
        printf("Error !!\n");
        return 1;
    }

    server_ping_t server = {0};

    read(s, &server, sizeof(server));

    printf("Server stamp is: %ld\n", server.stamp);
    return 0;
}

int send_hello(int s) {
    struct {
        request_header_t hdr;
        client_hello_t payload;
    } request = {
        {
            .id = CLIENT_HELLO,
            .request_len = CLIENT_HELLO_SIZE
        },
        {
            .username = {0},
            .password = {0}
        }
    };

    memcpy(&request.payload.username, "Alexandra", 9);
    memcpy(&request.payload.password, "OuiOuiOui", 9);

    write(s, &request, sizeof(request));

    request_header_t hdr = {0};

    read(s, &hdr, sizeof(hdr));

    if (hdr.id != SERVER_HELLO) {
        printf("Error hdr = %c\n", hdr.id);
        return 1;
    }

    server_hello_t server = {0};

    read(s, &server, sizeof(server));

    printf("Connexion to server: %s\n", server.result == OK ? "Succeed" : "Failed");
    return 0;
}

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == -1) {
        printf("Error creating socket.\n");
        return 1;
    }

    struct sockaddr_in infos;

    infos.sin_family = AF_INET;
    infos.sin_addr.s_addr = INADDR_ANY;
    infos.sin_port = htons(8080);

    if (connect(s, (const struct sockaddr *) &infos, sizeof(infos)) < 0) {
        printf("Error bind socket.\n");
        return 1;
    }

    printf("Sending Ping!\n");
    if (send_ping(s))
        return 1;
    
    printf("Sending Hello!\n");
    if (send_hello(s))
        return 1;
}
