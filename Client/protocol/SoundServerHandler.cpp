//
// Created by Quentin Liardeaux on 10/6/19.
//

#include "SoundServerHandler.hpp"

static const auto localEndPoint = BoostUdp::endpoint(boost::asio::ip::address_v4::any(), 8081);

SoundServerHandler::SoundServerHandler(BoostUdp::endpoint &remoteEndpoint) :
        ioService_(),
        remoteEndpoint_(remoteEndpoint),
        socket_(BoostUdp::socket(ioService_, localEndPoint.protocol()))
{
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
        soundManager_->read(data_);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }
}

void SoundServerHandler::handleSend(boost::system::error_code ec)
{
    if (!ec) {
        soundManager_->write(toWrite_);
        toWrite_.clear();
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }
}

void SoundServerHandler::dispatchUdpPackets(bool *isRunning)
{
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
    }
}
