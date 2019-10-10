//
// Created by Quentin Liardeaux on 10/6/19.
//

#include "SoundServerHandler.hpp"

static const auto localEndPoint = BoostUdp::endpoint(boost::asio::ip::address_v4::any(), 8081);

SoundServerHandler::SoundServerHandler(BoostUdp::endpoint& remoteEndpoint)
    : ioService_()
    , remoteEndpoint_(remoteEndpoint)
    , socket_(ioService_, localEndPoint.protocol())
    , sendSocket_(ioService_)
    , toSend_()
{
    toSend_.reserve(512);
    memset(toReceive_, 0, BUFFER_RECEIVE_SIZE);
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

void SoundServerHandler::handleRead(boost::system::error_code ec, size_t received)
{
    size_t rec = received / 4;

    std::cout << "Received: " << rec << " float." << std::endl;

    if (!ec) {
        soundManager_->write(std::vector<float>(toReceive_, toReceive_ + rec));
        memset(toReceive_, 0, BUFFER_RECEIVE_SIZE);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }

    socket_.async_receive_from(
        boost::asio::buffer(toReceive_, BUFFER_RECEIVE_SIZE),
        remoteEndpoint_,
        boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SoundServerHandler::handleSend(boost::system::error_code ec, size_t /* bytes_transfered */)
{
    if (!ec) {
        toSend_.clear();
        toSend_.resize(0);
        soundManager_->read(toSend_);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }

    audioController_.sleep(20);

    std::cout << "Sending: " << toSend_.size() << " bytes." << std::endl;

    socket_.async_send_to(
        boost::asio::buffer(&toSend_.front(), toSend_.size()),
        remoteEndpoint_,
        boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


void SoundServerHandler::dispatchUdpPackets(const bool* isRunning)
{
    try {
        soundManager_->start();

        audioController_.sleep(100);

        soundManager_->read(toSend_);

        socket_.async_send_to(
            boost::asio::buffer(&toSend_.front(), toSend_.size()),
            remoteEndpoint_,
            boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

        socket_.async_receive_from(
            boost::asio::buffer(toReceive_, BUFFER_RECEIVE_SIZE),
            remoteEndpoint_,
            boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

        ioService_.run();

        while (*isRunning && soundManager_->isActive())
            audioController_.sleep(100);

        soundManager_->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
