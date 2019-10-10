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

typedef boost::asio::ip::udp BoostUdp;

class SoundServerHandler {
public:
    explicit SoundServerHandler(BoostUdp::endpoint &remoteEndpoint);

    ~SoundServerHandler() = default;

    void start();
    void stop();

    void handleRead(boost::system::error_code ec);

    void handleSend(boost::system::error_code ec, size_t bytesTransfered);

private:
    void dispatchUdpPackets(bool *isRunning);


    boost::asio::io_service ioService_;

    BoostUdp::endpoint remoteEndpoint_;

    boost::thread soundThread_;

    BoostUdp::socket socket_;
    BoostUdp::socket sendSocket_;

    std::vector<float> toRead_;
    std::vector<float> toWrite_;

    bool isRunning_;

    AudioController audioController_;

    std::unique_ptr<SoundManager> soundManager_;
};


#endif //BABEL_SERVER_SOUNDSERVERHANDLER_HPP
