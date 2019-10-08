//
// Created by alex on 9/24/19.
//

#include "Session.hpp"
#include <Common/User.hpp>

boost::shared_ptr<Session> Session::create(boost::asio::io_context& context, Database& conn, std::list<boost::shared_ptr<Session>>& sessions)
{
    return boost::shared_ptr<Session>(new Session(context, conn, sessions));
}

Session::Session(boost::asio::io_context& context, Database& conn, std::list<boost::shared_ptr<Session>>& sessions)
    : username_()
    , socket_(context)
    , request_()
    , data_ { conn, socket_, sessions }
{
}

void Session::updateAllUsersNewConnection(const std::string& exclude)
{
    Packet<server_friend_status_t> req {
        { SERVER_FRIEND_STATUS, SERVER_FRIEND_STATUS_SIZE },
        { { {} }, {} }
    };

    int cnt = 0;

    for (auto& session : data_.sessions) {
        if (session->username_.empty())
            continue;
        strncpy(req.payload.usernames[cnt], session->username_.c_str(), USERNAME_LEN);
        req.payload.status[cnt] = OK;
        cnt++;
    }

    for (auto& session : data_.sessions) {
        if (session->username_ == exclude) {
            std::cout << "Excluding " << exclude << "from update." << std::endl;
            continue;
        }
        std::cout << "Updating " << session->username_ << "users" << std::endl;
        boost::asio::write(session->getSocket(), boost::asio::buffer(&req, sizeof(req)));
    }
}

void Session::run()
{
    data_.sessions.push_back(shared_from_this());

    waitHeader(boost::system::error_code());
}

void Session::waitHeader(const boost::system::error_code& ec)
{
    if (ec) {
        std::cerr << ec.message() << std::endl;
    } else {
        boost::asio::async_read(
            socket_,
            boost::asio::buffer(request_.getHeaderRaw(), HEADER_SIZE),
            boost::bind(&Session::receivePacket, shared_from_this(), boost::asio::placeholders::error));
    }
}

Session::~Session()
{
    socket_.close();
    std::cout << "Deleting: " << username_ << "." << std::endl;
}

void Session::receivePacket(const boost::system::error_code& ec)
{
    if (!ec) {
        request_.setupPayload();
        boost::asio::async_read(
            socket_,
            boost::asio::buffer(request_.getPayload(), request_.getPayloadSize()),
            boost::bind(&Session::receiveBody, shared_from_this(), boost::asio::placeholders::error));
    } else if (ec == boost::asio::error::eof) {
        std::cout << "Disconnecting client: " << username_ << "." << std::endl;

        data_.sessions.remove_if([this](boost::shared_ptr<Session>& session) {
            return session->getSocket().native_handle() == socket_.native_handle();
        });

        for (auto& e : data_.sessions) {
            std::cout << "Username: " << e->username_ << std::endl;
        }
    } else {
        std::cerr << "Error while receiving packet." << ec.message() << std::endl;
    }
}

void Session::receiveBody(const boost::system::error_code& ec)
{
    if (!ec) {
        Session::handleRequest(request_, data_);
    } else {
        std::cerr << "Error while receiving body." << ec.message() << std::endl;
    }
}

void Session::ping(client_ping_t* payload, SharedData& data)
{
    std::cout << "Client: " << data.socket.native_handle() << " ping at: " << payload->stamp << "." << std::endl;

    Packet<server_ping_response_t> req {
        {
            SERVER_PING_RESPONSE,
            SERVER_PING_RESPONSE_SIZE,
        },
        { time(nullptr) }
    };

    boost::asio::async_write(
        data.socket,
        boost::asio::buffer(&req, sizeof(req)),
        boost::bind(&Session::waitHeader, shared_from_this(), boost::asio::placeholders::error));
}

int appendUserComplete(void* data, int argc, char** argv, char** colName)
{
    auto users = static_cast<std::vector<User>*>(data);

    users->push_back(User(argv[0], argv[1], std::atoi(argv[2])));
    return 0;
}

int appendUser(void* data, int argc, char** argv, char** colName)
{
    auto users = static_cast<std::vector<std::string>*>(data);

    users->push_back(argv[0]);
    return 0;
}

void Session::hello(client_hello_t* payload, SharedData& data)
{
    Packet<server_hello_response_t> res {
        { SERVER_HELLO_RESPONSE,
            SERVER_HELLO_RESPONSE_SIZE },
        { KO }
    };

    std::vector<User> users;

    try {
        data.database.exec("SELECT username, password, id FROM users", appendUserComplete, &users);
    } catch (const DatabaseError& e) {
        std::cerr << "Cannot fetch users: " << e.what() << "." << std::endl;
        res.payload.result = KO;
    };

    for (auto& user : users) {
        if (std::strcmp(user.username.c_str(), payload->username) == 0 && std::strcmp(user.password.c_str(), payload->password) == 0) {
            res.payload.result = OK;
            username_ = std::string(payload->username);
            updateAllUsersNewConnection(username_);
            break;
        }
    }

    std::cout << "Hello: " << (res.payload.result == OK ? "OK" : "KO") << std::endl;

    boost::asio::async_write(
        data.socket,
        boost::asio::buffer(&res, sizeof(res)),
        boost::bind(&Session::waitHeader, shared_from_this(), boost::asio::placeholders::error));
}

void Session::friendRequest(client_friend_request_t* payload, SharedData& data)
{
}

void Session::goodbye(client_goodbye_t* payload, SharedData& data)
{
}

void Session::clientRegister(client_register_t* payload, SharedData& data)
{

    std::string sqlReq("INSERT INTO users (username, password) VALUES (\"");

    sqlReq += (std::string(payload->username) + "\", \"" + payload->password + "\")");

    Packet<server_register_response_t> res {
        { SERVER_REGISTER_RESPONSE,
            SERVER_REGISTER_RESPONSE_SIZE },
        { OK }
    };

    try {
        data.database.exec(sqlReq);
    } catch (DatabaseError& e) {
        std::cout << "Failed to register user: " << e.what() << std::endl;

        res.payload.result = KO;
    }

    std::cout << "Registered: " << (res.payload.result == OK ? "OK" : "KO") << std::endl;

    if (res.payload.result == OK) {
        username_ = std::string(payload->username);
        updateAllUsersNewConnection(username_);
    }

    boost::asio::async_write(
        data.socket,
        boost::asio::buffer(&res, sizeof(res)),
        boost::bind(&Session::waitHeader, shared_from_this(), boost::asio::placeholders::error));
}

void Session::call(client_call_t* payload, SharedData& data)
{
    Packet<server_call_response_t> res {
        { SERVER_CALL_RESPONSE,
            SERVER_CALL_RESPONSE_SIZE },
        { KO }
    };

    std::vector<std::string> users;
    bool userFound = false;

    try {
        data.database.exec("SELECT username FROM users", appendUser, &users);

        for (int i = 0; i < payload->number; ++i) {
            for (auto& user : users) {
                std::cout << user << " " << payload->usernames[i] << std::endl;
                if (std::strcmp(user.c_str(), payload->usernames[i]) == 0) {
                    userFound = true;
                    Packet<server_call_t> req {
                        { SERVER_CALL,
                            SERVER_CALL_SIZE },
                        { {}, 8080, {} }
                    };

                    memcpy(req.payload.username, username_.c_str(), username_.length());
                    memcpy(&req.payload.port, &payload->port, sizeof(short));
                    memcpy(req.payload.ip, payload->ip, 16);

                    for (auto& session : data.sessions) {
                        if (session->username_.empty() || session->username_ == username_)
                            continue;

                        if (session->username_ == user) {
                            std::cout << "Sending call to " << user << std::endl;
                            std::cout << sizeof(req) << std::endl;
                            boost::asio::write(session->getSocket(), boost::asio::buffer(&req, sizeof(req)));
                            res.payload.result = OK;
                            break;
                        }
                    }
                }
            }
        }
    } catch (const DatabaseError& e) {
        std::cerr << "Failed to fetch username: " << e.what() << "." << std::endl;
    };

    if (userFound && res.payload.result == KO)
        std::cerr << "User found but not connected." << std::endl;
    else if (!userFound)
        std::cerr << "User doesn't exist." << std::endl;

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(&res, sizeof(res)),
        boost::bind(&Session::waitHeader, shared_from_this(), boost::asio::placeholders::error));
}

void Session::bye(client_bye_t*, SharedData& data)
{
}

void Session::acceptFriend(client_accept_friend_t* payload, SharedData& data)
{
}

void Session::friendStatus(client_friend_status_t*, SharedData& data)
{
    Packet<server_friend_status_t> res {
        { SERVER_FRIEND_STATUS,
            SERVER_FRIEND_STATUS_SIZE },
        { { {} },
            {} }
    };

    int cnt = 0;

    for (auto& session : data.sessions) {
        if (session->socket_.native_handle() == socket_.native_handle())
            continue;

        memcpy(res.payload.usernames[cnt], session->username_.c_str(), session->username_.length());
        res.payload.status[cnt] = OK;
        cnt++;
    }

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(&res, sizeof(res)),
        boost::bind(&Session::waitHeader, shared_from_this(), boost::asio::placeholders::error));
}

void Session::acceptCall(client_accept_call_t* payload, SharedData& data)
{
    Packet<server_accept_call_response_t> res {
        { SERVER_CALL_RESPONSE,
            SERVER_CALL_RESPONSE_SIZE },
        { OK }
    };

    bool userFound = false;

    if (strcmp(payload->username, username_.c_str()) == 0) {
        res.payload.result = KO;
    } else {
        for (auto& session : data.sessions) {
            if (strcmp(session->username_.c_str(), payload->username) == 0) {
                Packet<server_accept_call_t> req {
                    { SERVER_ACCEPT_CALL,
                        SERVER_ACCEPT_CALL_SIZE },
                    { {}, payload->port, {} }
                };

                memcpy(req.payload.username, payload->username, USERNAME_LEN);
                memcpy(req.payload.ip, payload->ip, 16);

                boost::asio::write(session->getSocket(), boost::asio::buffer(&req, sizeof(req)));
                userFound = true;
                break;
            }
        }

        if (!userFound) {
            res.payload.result = KO;
            std::cerr << "User " << payload->username << " not found." << std::endl;
        }
    }

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(&res, sizeof(res)),
        boost::bind(&Session::waitHeader, shared_from_this(), boost::asio::placeholders::error));
}

void Session::handleRequest(Message& request, SharedData& data)
{
    std::cout << std::endl;
    switch (request.getId()) {
    case CLIENT_HELLO:
        hello((client_hello_t*)request.getPayload(), data);
        break;
    case CLIENT_PING:
        ping((client_ping_t*)request.getPayload(), data);
        break;
    case CLIENT_ACCEPT_FRIEND:
        acceptFriend((client_accept_friend_t*)request.getPayload(), data);
        break;
    case CLIENT_FRIEND_STATUS:
        friendStatus((client_friend_status_t*)request.getPayload(), data);
        break;
    case CLIENT_FRIEND_REQUEST:
        friendRequest((client_friend_request_t*)request.getPayload(), data);
        break;
    case CLIENT_BYE:
        bye((client_bye_t*)request.getPayload(), data);
        break;
    case CLIENT_CALL:
        call((client_call_t*)request.getPayload(), data);
        break;
    case CLIENT_GOODBYE:
        goodbye((client_goodbye_t*)request.getPayload(), data);
        break;
    case CLIENT_REGISTER:
        clientRegister((client_register_t*)request.getPayload(), data);
        break;
    case CLIENT_ACCEPT_CALL:
        acceptCall((client_accept_call_t*)request.getPayload(), data);
        break;
    default:
        throw "Unknown request.";
    }
}
