//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_SESSION_HPP
#define BABEL_SERVER_SESSION_HPP

#include "Message.hpp"
#include "SharedData.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <vector>

typedef boost::asio::ip::tcp BoostTcp;

class Session : public boost::enable_shared_from_this<Session> {
public:
    static boost::shared_ptr<Session> create(boost::asio::io_context& context, Database& conn, std::vector<boost::shared_ptr<Session>>& session);

    [[nodiscard]] BoostTcp::socket& getSocket()
    {
        return socket_;
    }

    void waitHeader(const boost::system::error_code& ec);

    void run(std::vector<boost::shared_ptr<Session>>& sessions);

    void receivePacket(const boost::system::error_code& ec);

    void receiveBody(const boost::system::error_code& ec);

    void handleRequest(Message& request, SharedData& data);

    void friendRequest(client_friend_request_t *payload, SharedData& data);

    void ping(client_ping_t *payload, SharedData& data);

    void hello(client_hello_t *payload, SharedData& data);

    void goodbye(client_goodbye_t *payload, SharedData& data);

    void clientRegister(client_register_t *payload, SharedData& data);

    void call(client_call_t *payload, SharedData& data);

    void bye(client_bye_t *, SharedData& data);

    void acceptFriend(client_accept_friend_t *payload, SharedData& data);

    void friendStatus(client_friend_status_t *, SharedData& data);

    struct UserInformations {
        bool used;
        bool valid;
        char *username;
        char *password;
    };

private:
    explicit Session(boost::asio::io_context& context, Database& conn, std::vector<boost::shared_ptr<Session>>& sessions);

    std::string username_;

    BoostTcp::socket socket_;

    Message request_;

    SharedData data_;
};

#endif //BABEL_SERVER_SESSION_HPP
