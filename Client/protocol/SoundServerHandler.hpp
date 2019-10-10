//
// Created by Quentin Liardeaux on 10/6/19.
//

#ifndef BABEL_SERVER_SOUNDSERVERHANDLER_HPP
#define BABEL_SERVER_SOUNDSERVERHANDLER_HPP

#include <string>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <boost/array.hpp>

#include "../AudioController.hpp"
#include "../SoundManager.hpp"

static const size_t BUFFER_SIZE = 1024;
static const size_t BUFFER_RECEIVE_SIZE = sizeof(float) * BUFFER_SIZE;

typedef boost::asio::ip::udp BoostUdp;

class SoundServerHandler {
public:
    explicit SoundServerHandler(BoostUdp::endpoint &remoteEndpoint);

    ~SoundServerHandler() = default;

    void start();
    void stop();

    void handleRead(boost::system::error_code ec, size_t received);

    void handleSend(boost::system::error_code ec, size_t transfered);

private:
    void dispatchUdpPackets(const bool *isRunning);


    boost::asio::io_service ioService_;

    BoostUdp::endpoint remoteEndpoint_;

    boost::thread soundThread_;

    BoostUdp::socket socket_;
    BoostUdp::socket sendSocket_;

    std::vector<float> toSend_;
    float toReceive_[BUFFER_SIZE];

    bool isRunning_ = false;

    AudioController audioController_;

    std::unique_ptr<SoundManager> soundManager_;
};


#endif //BABEL_SERVER_SOUNDSERVERHANDLER_HPP
