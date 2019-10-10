//
// Created by Quentin Liardeaux on 10/6/19.
//

#include "SoundServerHandler.hpp"

static const auto localEndPoint = BoostUdp::endpoint(boost::asio::ip::address_v4::any(), 8081);

SoundServerHandler::SoundServerHandler(BoostUdp::endpoint& remoteEndpoint)
    : ioService_()
    , remoteEndpoint_(remoteEndpoint)
    , socket_(BoostUdp::socket(ioService_, localEndPoint.protocol()))
    , toRead_()
    , toWrite_()
{
    toRead_.reserve(512);
    toWrite_.reserve(512);
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
        std::cout << "Receiving: " << toWrite_.size() << "bytes." << std::endl;
        toWrite_.resize(512);
        soundManager_->write(toWrite_);
        toWrite_.clear();
        toWrite_.resize(0);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }
    audioController_.sleep(100);
    socket_.async_receive_from(boost::asio::buffer(&toWrite_.front(), 512), remoteEndpoint_, boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error));
}

void SoundServerHandler::handleSend(boost::system::error_code ec)
{
    if (!ec) {
        toRead_.clear();
        toRead_.resize(0);
        soundManager_->read(toRead_);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }

    std::cout << "Sending: " << toRead_.size() << "bytes." << std::endl;
    audioController_.sleep(100);
    socket_.async_send_to(boost::asio::buffer(&toRead_.front(), 512),
        remoteEndpoint_,
        boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error));
}

void SoundServerHandler::dispatchUdpPackets(bool* isRunning)
{
    while (true) {
        try {
            boost::asio::ip::udp::endpoint remote;
            soundManager_->start();

            soundManager_->read(toRead_);

            // socket_.async_receive_from(boost::asio::buffer(&toWrite_.front(), 512), remoteEndpoint_, boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error));
            socket_.async_send_to(boost::asio::buffer(&toRead_.front(), 512), remoteEndpoint_, boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error));

            ioService_.run();

            std::cout << "HERE" << std::endl;

            while (isRunning)
                audioController_.sleep(100);
            soundManager_->stop();
        } catch (const AudioControllerError& e) {
            std::cerr << e.what() << std::endl;
            return;
        }
    }
}
