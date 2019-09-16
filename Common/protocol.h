//
// Created by Alexandre Fourcat on 9/16/19.
//

#ifndef BABEL_COMMON_PROTOCOL_H
#define BABEL_COMMON_PROTOCOL_H

// Should be updated at each modification.
#define VERSION 0x01

// 1 if the request id come from the server.
#define IS_SERVER_REQUEST(x) ((x) & 0b10000000)

// This is the final data structure send through internet during the client/server communication.
typedef struct {
    // Defined by CLIENT_REQ_ID if it's a client request. SERVER_REQ_ID otherwise.
    int id;

    // Defined by CLIENT_REQ_LEN if it's a client request. SERVER_REQ_LEN otherwise.
    int request_len;

    // The request payload.
    void *payload;
} request_t;


/// Client's request id.
enum CLIENT_REQ_ID {
    // Test the server connection.
    PING            = 0b00000000,

    // Connect to the server.
    HELLO           = 0b00000001,

    // Quit the server.
    GOODBYE         = 0b00000010,

    // Ask for friend connection status.
    FRIEND_STATUS   = 0b00000011,

    // Register to the server.
    REGISTER        = 0b00000100,

    // Ask for friend.
    FRIEND_REQUEST  = 0b00000101,

    // Invite to a call.
    CALL            = 0b00000111,

    // Close a in going call.
    BYE             = 0b00001000,

    // Accept a pending friend request.
    ACCEPT_FRIEND   = 0b00001001,
};

// Server's request id.
enum SERVER_REQ_ID {
    // Answer to the client PING.
    PONG            = 0b10000000,

    // Forward friend request.
    FRIEND_REQUEST  = 0b10000101,

    // Forward call request.
    CALL            = 0b10000111,

    // Forward bye request.
    BYE             = 0b10001000,

    // Forward the friend request acceptation.
    ACCEPT_FRIEND   = 0b10001001,
};

// ----------------- Client's payloads definitions ------------------------ //

typedef struct {
    time_t stamp;
} client_ping_t;

const size_t CLIENT_PING_SIZE = sizeof(client_ping_t);



typedef struct {
    char *username;
    char *password;
} client_hello_t;

const size_t CLIENT_HELLO_SIZE = sizeof(client_hello_t);



typedef struct {} client_goodbye_t;

const size_t CLIENT_GOODBYE_SIZE = sizeof(client_goodbye_t);



typedef struct {
    char *username;
} client_friend_request_t;

const size_t CLIENT_FRIEND_REQUEST_SIZE = sizeof(client_friend_request_t);



typedef struct {
    char *username;
    char *password;
} client_register_t;

const size_t CLIENT_REGISTER_SIZE = sizeof(client_register_t);



typedef struct {
    // Array of username to call.
    char **usernames;

    // The number of user stored in usernames.
    int number;
} client_call_t;

const size_t CLIENT_CALL_SIZE = sizeof(client_call_t);



typedef struct {} client_bye_t;

const size_t CLIENT_BYE_SIZE = sizeof(client_bye_t);



typedef struct {
    char *username;
} client_accept_friend_t;

const size_t CLIENT_ACCEPT_FRIEND = sizeof(client_accept_friend_t);


// ----------------- Server's payloads definitions ------------------------ //

typedef struct {
    time_t stamp;
} server_pong_t;

const size_t SERVER_PONG_SIZE = sizeof(server_pong_t);



typedef struct {
    // The asker username.
    char *username;
} server_friend_request_t;

const size_t SERVER_FRIEND_REQUEST_SIZE = sizeof(server_friend_request_t);



typedef struct {
    // The asker username.
    char **usernames;

    // Number of user inside usernames.
    int number;
} server_call_t;

const size_t SERVER_CALL_SIZE = sizeof(server_call_t);



typedef struct {
    // Leaver username.
    char *username;
} server_bye_t;

const size_t SERVER_BYE_SIZE = sizeof(server_bye_t);



typedef struct {
    // Acceptor username.
    char *username;
} server_accept_friend_t;

const size_t SERVER_ACCEPT_FRIEND_SIZE = sizeof(server_accept_friend_t);

#endif //BABEL_SERVER_PROTOCOL_H
