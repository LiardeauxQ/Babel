//
// Created by alex on 9/24/19.
//

#include "Session.hpp"

boost::shared_ptr<Session> Session::create(boost::asio::io_context& context, Database& conn, std::vector<boost::shared_ptr<Session>>& sessions)
{
    return boost::shared_ptr<Session>(new Session(context, conn, sessions));
}

Session::Session(boost::asio::io_context& context, Database& conn, std::vector<boost::shared_ptr<Session>>& sessions)
    : username_()
    , socket_(context)
    , request_()
    , data_ { conn, socket_, sessions}
{
}

void Session::run(std::vector<boost::shared_ptr<Session>>& sessions)
{
    sessions.push_back(shared_from_this());

    waitHeader(boost::system::error_code());
}

void Session::waitHeader(const boost::system::error_code& ec)
{
    if (ec)
        std::cerr << ec.message() << std::endl;

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(request_.getHeaderRaw(), HEADER_SIZE),
        boost::bind(&Session::receivePacket, shared_from_this(), boost::asio::placeholders::error));
}

void Session::receivePacket(const boost::system::error_code& ec)
{
    if (!ec) {
        request_.setupPayload();

        std::cout << "Id: " << request_.getId() << std::endl;
        std::cout << "Size payload: " << request_.getPayloadSize() << std::endl;

        boost::asio::async_read(
            socket_,
            boost::asio::buffer(request_.getPayload(), request_.getPayloadSize()),
            boost::bind(&Session::receiveBody, shared_from_this(), boost::asio::placeholders::error));
    } else {
        std::cerr << "Error while receiving packet." << ec.message() << std::endl;
    }
}

void Session::receiveBody(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "Body received" << std::endl;
        Session::handleRequest(request_, data_);
    } else {
        std::cerr << "Error while receiving body." << ec.message() << std::endl;
    }
}

void Session::ping(client_ping_t* payload, SharedData& data)
{
    std::cout << "Client: " << data.socket.native_handle() << " ping at: " << payload->stamp << "." << std::endl;

    struct {
        request_header_t hdr;
        server_ping_t payload;
    } request = {
        {
            SERVER_PING,
            SERVER_PING_SIZE,
        },
        { time(nullptr) }
    };

    boost::asio::async_write(
        data.socket,
        boost::asio::buffer(&request, sizeof(request)),
        boost::bind(&Session::waitHeader, this, boost::asio::placeholders::error));
}

int checkUserPassword(void* data, int argc, char** argv, char** colName)
{
    auto infos = static_cast<Session::UserInformations*>(data);

    if (argc < 2)
        return 1;

    infos->used = true;

    if (strcmp(argv[0], infos->username) == 0 && strcmp(argv[1], infos->password) == 0)
        infos->valid = true;
    return 0;
}

int checkUserExist(void* data, int argc, char** argv, char** colName)
{
    auto infos = static_cast<Session::UserInformations*>(data);

    infos->used = true;

    if (strcmp(argv[0], infos->username) == 0)
        infos->valid = true;
}

void Session::hello(client_hello_t* payload, SharedData& data)
{
    std::cout << "Username: " << payload->username << std::endl;
    std::cout << "Password: " << payload->password << std::endl;

    Session::UserInformations infos {
        false,
        false,
        payload->username,
        payload->password
    };

    // TODO: hash password.

    data.database.exec("SELECT username, password FROM users", checkUserPassword, &infos);

    if (infos.valid)
        username_ = std::string(payload->username);

    struct {
        request_header_t hdr;
        server_hello_t payload;
    } res {
        { SERVER_HELLO,
            SERVER_HELLO_SIZE },
        { infos.valid ? OK : KO }
    };

    boost::asio::async_write(
        data.socket,
        boost::asio::buffer(&res, sizeof(res)),
        boost::bind(&Session::waitHeader, this, boost::asio::placeholders::error));
}

void Session::friendRequest(client_friend_request_t* payload, SharedData& data)
{
}

void Session::goodbye(client_goodbye_t* payload, SharedData& data)
{
}

void Session::clientRegister(client_register_t* payload, SharedData& data)
{
}

void Session::call(client_call_t* payload, SharedData& data)
{
    int number_client = payload->number;

    for (int i = 0; i < number_client; ++i) {
        UserInformations infos {
            false,
            false,
            payload->usernames[i],
            nullptr,
        };

        data.database.exec("SELECT username FROM users", checkUserExist, &infos);

        if (infos.valid) {
            struct {
                request_header_t hdr;
                server_call_t payload;
            } req {
                { SERVER_CALL,
                    SERVER_CALL_SIZE },
                { {} }
            };

            memcpy(req.payload.username, username_.c_str(), username_.length());
            bool sent = false;

            for (auto& session: data.sessions) {
                if (session->username_.empty())
                    continue;

                if (session->username_ == infos.username) {
                    sent = true;
                    boost::asio::write(session->getSocket(), boost::asio::buffer(&req, sizeof(req)));
                    break;
                }
            }

            if (!sent)
                std::cerr << "User doesn't exist or is not logged." << std::endl;
        } else {
            std::cerr << "Invalid user." << std::endl;
        }
    }
    waitHeader(boost::system::error_code());
}

void Session::bye(client_bye_t*, SharedData& data)
{
}

void Session::acceptFriend(client_accept_friend_t* payload, SharedData& data)
{
}

void Session::friendStatus(client_friend_status_t*, SharedData& data)
{
}

void Session::handleRequest(Message& request, SharedData& data)
{
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
    default:
        throw "Unknown request.";
    }
}
