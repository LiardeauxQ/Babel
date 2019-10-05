---
title: The Babel Client-Server protocol 
abbrev: I-D
docname: RFC draft
date: 2019-10-05
category: info
ipr: trust200902

stand_alone: yes
pi: [toc, sortrefs, symrefs]

author:
 -
    ins: A. Fourcat
    name: Alexandre Fourcat
    organization: Epitech.
    email: alexandre.fourcat@epitech.eu
 
 -
    ins: T. Nicollet
    name: Thomas Nicollet
    organization: Epitech.
    email: thomas.nicollet@epitech.eu

 -
    ins: L. Quentin
    name: Quentin Liardeaux
    organization: Epitech.
    email: quentin.liardeaux@epitech.eu

 -
    ins: P. Kilian
    name: Kilian Perrier
    organization: Epitech.
    email: kilian.perrier@epitech.eu

normative:
  RFC2119:

informative:

--- abstract

The RFC is written for the Babel project.
It will present the binary protocol written to sustain the clients of the Babel software.

The Babel is a multi-platform (Windows, Linux, MacOs) VOIP software.

--- middle

# Introduction

The binary protocol use a versioning system based on ??? versioning.
The binary protocol permit transfer of instruction and is hardly typed.

(The response are structure based on request etc...)


## Terminology

We need you to understand the concepts of "SOCKET", "PORT", "USER", "DATABASE"
The project will be done with C like struct and types.

USERNAME_SIZE tell to the protocol's users the max size of the username.

PASSWORD_SIZE tell to the protocol's users the max size of the password.

MAX_USER_IN_CALL tell to the protocol's users the max user in a VOIP conversation.

# Client requests

The client can connect to the server, register and do lot's of actions during the runtime.

## Server based request

### CLIENT_PING

Ping the server to check connection and latency.

Content:

* time_t stamp

Response: SERVER_PING_RESPONSE


### CLIENT_HELLO

This is the login request. When the connection is established  it should be the first request made from the client.

Content:

* char username[USERNAME_SIZE]
* char password[PASSWORD_SIZE]

Response: SERVER_HELLO_RESPONSE

### CLIENT_GOODBYE

This is the request to close the connection to the server from the client.

Content: EMPTY

Response: SERVER_GOODBYE_RESPONSE

### CLIENT_FRIEND_STATUS

Request for the status (Connected, InCall etc...) of the user friends.

Content: EMPTY

Response: SERVER_FRIEND_STATUS_RESPONSE

### CLIENT_REGISTER

Request to register client with username and password into the server database.

Content:

* char username[USERNAME_LEN]

* char password[PASSWORD_LEN]

Response: SERVER_REGISTER_RESPONSE

## Server proxy request

### CLIENT_FRIEND_REQUEST

Send a friend request to the user given in payload.

Content:

* char username[USERNAME_LEN]

Response: SERVER_FRIEND_REQUEST_RESPONSE

### CLIENT_CALL

Request to begin a data transfer through client.

Content:

* char username[MAX_USER_IN_CALL][USERNAME_LEN]
* short port
* char ip[16]

Response: SERVER_CALL_RESPONSE

### CLIENT_BYE

Request to close the running VOIP conversation.

Content: EMPTY

Response: SERVER_BYE_RESPONSE

### CLIENT_ACCEPT_FRIEND

Accept friend requested from CLIENT_FRIEND_REQUEST.

Content:

* char message[MESSAGE_LEN]

Response: SERVER_ACCEPT_FRIEND_RESPONSE

### CLIENT_ACCEPT_CALL

Accept call requested from CLIENT_CALL.

Content:

* char username[USERNAME_LEN]
* short port
* char ip[16]

Response: SERVER_ACCEPT_CALL_RESPONSE

# Server requests

## *_RESPONSE

All the previous client request responses.

result is an int that can contain either OK or KO.

Content:

* int result

Response: EMPTY

## *

In other case the server just forward and make safe ans possibility check as a proxy.

Content:

* Same as client's requests.

Response: EMPTY
