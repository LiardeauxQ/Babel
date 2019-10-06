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

#include "../AudioController.hpp"
#include "../SoundManager.hpp"

typedef boost::asio::ip::udp BoostUdp;

class SoundServerHandler {
public:
    SoundServerHandler(const std::string &readIpAddress, int readPort,
            const std::string &writeIpAddress, int writePort);

    ~SoundServerHandler() = default;

    void start();
    void stop();

private:
    void dispatchUdpPackets(bool *isRunning);

    boost::asio::io_context context_;
    boost::thread soundThread_;
    BoostUdp::socket readSocket_;
    BoostUdp::socket writeSocket_;
    std::string readIpAddress_;
    std::string writeIpAddress_;
    int readPort_;
    int writePort_;
    bool isRunning_;
    AudioController audioController_;
    std::unique_ptr<SoundManager> soundManager_;
};


#endif //BABEL_SERVER_SOUNDSERVERHANDLER_HPP
