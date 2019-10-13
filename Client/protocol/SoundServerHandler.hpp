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

static const size_t BUFFER_SIZE_FLOAT = 256;
static const size_t BUFFER_SIZE_BYTES = BUFFER_SIZE_FLOAT * sizeof(float);

typedef boost::asio::ip::udp BoostUdp;

class SoundServerHandler {
public:
    explicit SoundServerHandler(BoostUdp::endpoint &remoteEndpoint);

    ~SoundServerHandler() = default;

    void start();
    void stop();

    void setRunningStatus(bool status);

    void handleRead(boost::system::error_code ec, size_t received);

    void handleSend(boost::system::error_code ec, size_t transfered);

private:
    void dispatchUdpPackets(const bool *isRunning);


    boost::asio::io_service ioService_;

    BoostUdp::endpoint remoteEndpoint_;

    boost::thread soundThread_;

    BoostUdp::socket socket_;
    BoostUdp::socket sendSocket_;

    float toReceive_[1024];
    float toSend_[1024];

    bool isRunning_ = false;

    AudioController audioController_;

    std::unique_ptr<SoundManager> soundManager_;
};


#endif //BABEL_SERVER_SOUNDSERVERHANDLER_HPP
