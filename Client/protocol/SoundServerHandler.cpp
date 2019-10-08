//
// Created by Quentin Liardeaux on 10/6/19.
//

#include "SoundServerHandler.hpp"
#include <sys/socket.h>

static const auto localEndPoint = BoostUdp::endpoint(boost::asio::ip::address_v4::any(), 8081);

SoundServerHandler::SoundServerHandler(BoostUdp::endpoint &remoteEndpoint) :
        ioService_(),
        remoteEndpoint_(remoteEndpoint),
        socket_(BoostUdp::socket(ioService_, localEndPoint.protocol()))
{
    toRead_ = {1};
    audioController_ = AudioController();
    soundManager_ = audioController_.createManager();
}

void SoundServerHandler::start()
{
    isRunning_ = true;
    socket_.bind(localEndPoint);
    soundThread_ = boost::thread(&SoundServerHandler::dispatchUdpPackets, this, &isRunning_);
}

void SoundServerHandler::stop()
{
    isRunning_ = false;
    socket_.close();
    soundThread_.join();
}

void SoundServerHandler::handleRead(boost::system::error_code ec)
{
    if (!ec) {
        data_.clear();
        soundManager_->write(data_);
        if (data_.size() > 0)
            std::cout << "received " << data_.size() << std::endl;
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }
    boost::asio::ip::udp::endpoint remote;
    socket_.async_send_to(boost::asio::buffer(toRead_), remoteEndpoint_, boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error));
}

void SoundServerHandler::handleSend(boost::system::error_code ec)
{
    if (!ec) {
        soundManager_->read(toWrite_);
        if (toWrite_.size() > 0)
            std::cout << "sending " << toWrite_.size() << std::endl;
        toWrite_.clear();
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }

    boost::asio::ip::udp::endpoint remote;
    socket_.async_receive_from(boost::asio::buffer(toWrite_, 512), remote, boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error));
}

void SoundServerHandler::dispatchUdpPackets(bool *isRunning)
{
    int myFirstUdpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    unsigned short port = 8081;

    sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    if (bind(myFirstUdpSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        return;
    }

    unsigned short portDst = 8081;
    sockaddr_in to = { 0 };
    inet_pton(AF_INET, "10.109.252.166", &to.sin_addr.s_addr);
    to.sin_family = AF_INET;
    to.sin_port = htons(portDst);

    while (1)
    {
        std::string data;
        std::getline(std::cin, data);
        if (data.empty())
            break;
        int ret = sendto(myFirstUdpSocket, data.data(), static_cast<int>(data.length()), 0, reinterpret_cast<const sockaddr*>(&to), sizeof(to));
        if (ret <= 0)
        {
            std::cout << "error" << std::endl;
            break;
        }
        char buff[1500] = { 0 };
        sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ret = recvfrom(myFirstUdpSocket, buff, 1499, 0, reinterpret_cast<sockaddr*>(&from), &fromlen);
        if (ret <= 0)
        {
            std::cout << "error" << std::endl;
            break;
        }
        std::cout << "Recu : " << buff << " de "<< std::endl;
    }
    /*
    try {
        std::vector<float> toWrite;

        boost::asio::ip::udp::endpoint remote;
        data_.reserve(512);
        toWrite.reserve(512);
        soundManager_->start();

        socket_.async_send_to(boost::asio::buffer(data_, 512), remoteEndpoint_, boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error));

        socket_.async_receive_from(boost::asio::buffer(toWrite_, 512), remote, boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error));

        ioService_.run();
        std::cout << "HERE" << std::endl;
        while (isRunning) {
            audioController_.sleep(100);
        }
        soundManager_->stop();
    } catch (const AudioControllerError &e) {
        std::cerr << e.what() << std::endl;
        return;
    }*/
}
