//
// Created by Alexandre Fourcat on 9/16/19.
//

#ifndef BABEL_COMMON_PROTOCOL_H
#define BABEL_COMMON_PROTOCOL_H

// Should be updated at each modification.

#include <bits/types/time_t.h>
#include <glob.h>

#define VERSION 0x05

// 1 if the request id come from the server.
#define IS_SERVER_REQUEST(x) ((x)&0b10000000)

// This is the final data structure send through internet during the client/server communication.
typedef struct {
    // Defined by CLIENT_REQ_ID if it's a client request. SERVER_REQ_ID otherwise.
    int id;

    // Defined by CLIENT_REQ_LEN if it's a client request. SERVER_REQ_LEN otherwise.
    int request_len;
} request_header_t;

const size_t HEADER_SIZE = sizeof(request_header_t);

#define USERNAME_LEN 1024
#define MESSAGE_LEN 1024
#define PASSWORD_LEN 1024
#define MAX_FRIENDS 20

enum RESULT {
    OK = 0b10101010,
    KO = 0b01010101
};

/// Client's request id.
enum CLIENT_REQ_ID {
    // Test the server connection.
    CLIENT_PING = 0b00000000,

    // Connect to the server.
    CLIENT_HELLO = 0b00000001,

    // Quit the server.
    CLIENT_GOODBYE = 0b00000010,

    // Ask for friend connection status.
    CLIENT_FRIEND_STATUS = 0b00000011,

    // Register to the server.
    CLIENT_REGISTER = 0b00000100,

    // Ask for friend.
    CLIENT_FRIEND_REQUEST = 0b00000101,

    // Invite to a call.
    CLIENT_CALL = 0b00000111,

    // Close a in going call.
    CLIENT_BYE = 0b00001000,

    // Accept a pending friend request.
    CLIENT_ACCEPT_FRIEND = 0b00001001,
};

/// Client's request id.
enum SERVER_REQ_ID {
        SERVER_PING = 0b10000000,

        SERVER_HELLO = 0b10000001,

        SERVER_GOODBYE = 0b10000010,

        SERVER_FRIEND_STATUS = 0b10000011,

        SERVER_REGISTER = 0b10000100,

        SERVER_FRIEND_REQUEST = 0b10000101,

        SERVER_CALL = 0b10000111,

        SERVER_BYE = 0b10001000,

        SERVER_ACCEPT_FRIEND = 0b10001001,
};

// ----------------- Client's payloads definitions ------------------------ //

typedef struct {
    time_t stamp;
} client_ping_t;

const size_t CLIENT_PING_SIZE = sizeof(client_ping_t);

typedef struct {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} client_hello_t;

const size_t CLIENT_HELLO_SIZE = sizeof(client_hello_t);

typedef struct {
} client_goodbye_t;

const size_t CLIENT_GOODBYE_SIZE = sizeof(client_goodbye_t);

typedef struct {
    char username[USERNAME_LEN];
} client_friend_request_t;

const size_t CLIENT_FRIEND_REQUEST_SIZE = sizeof(client_friend_request_t);

typedef struct {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} client_register_t;

const size_t CLIENT_REGISTER_SIZE = sizeof(client_register_t);

typedef struct {
    // Array of username to call.
    char usernames[USERNAME_LEN];

    // The number of user stored in usernames.
    int number;
} client_call_t;

const size_t CLIENT_CALL_SIZE = sizeof(client_call_t);

typedef struct {
} client_bye_t;

const size_t CLIENT_BYE_SIZE = sizeof(client_bye_t);

typedef struct {
    char username[USERNAME_LEN];
} client_accept_friend_t;

const size_t CLIENT_ACCEPT_FRIEND_SIZE = sizeof(client_accept_friend_t);

typedef struct {
} client_friend_status_t;

const size_t CLIENT_FRIEND_STATUS_SIZE = sizeof(client_friend_status_t);

// ----------------- Server's payloads definitions ------------------------ //

typedef struct {
    time_t stamp;
} server_ping_t;

const size_t SERVER_PING_SIZE = sizeof(server_ping_t);

typedef struct {
    int result;
} server_hello_t;

const size_t SERVER_HELLO_SIZE = sizeof(server_hello_t);

typedef struct {
    char message[MESSAGE_LEN];
} server_goodbye_t;

const size_t SERVER_GOODBYE_SIZE = sizeof(server_goodbye_t);

typedef struct {
    char usernames[MAX_FRIENDS][USERNAME_LEN];
    int status[MAX_FRIENDS];
} server_friend_status_t;

const size_t SERVER_FRIEND_STATUS_SIZE = sizeof(server_friend_status_t);

typedef struct {
    int result;
} server_register_t;

const size_t SERVER_REGISTER_SIZE = sizeof(server_register_t);

typedef struct {
    // The asker username.
    char username[USERNAME_LEN];
} server_friend_request_t;

const size_t SERVER_FRIEND_REQUEST_SIZE = sizeof(server_friend_request_t);

typedef struct {
    // The asker username.
    char usernames[USERNAME_LEN];

    // Number of user inside usernames.
    int number;
} server_call_t;

const size_t SERVER_CALL_SIZE = sizeof(server_call_t);

typedef struct {
    // Leaver username.
    char username[USERNAME_LEN];
} server_bye_t;

const size_t SERVER_BYE_SIZE = sizeof(server_bye_t);

typedef struct {
    // Acceptor username.
    char username[USERNAME_LEN];
} server_accept_friend_t;

const size_t SERVER_ACCEPT_FRIEND_SIZE = sizeof(server_accept_friend_t);

#endif //BABEL_SERVER_PROTOCOL_H
