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
    , toReceive_()
    , toSend_()
{
    memset(toReceive_, 0, BUFFER_SIZE_FLOAT * sizeof(float));
    memset(toSend_, 0, BUFFER_SIZE_FLOAT * sizeof(float));
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
    size_t rec = received / sizeof(float);

    if (!ec) {
        soundManager_->write(toReceive_, rec);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }

    socket_.async_receive_from(
        boost::asio::buffer(toReceive_, BUFFER_SIZE_BYTES),
        remoteEndpoint_,
        boost::bind(&SoundServerHandler::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SoundServerHandler::handleSend(boost::system::error_code ec, size_t /* bytes_transfered */)
{
    if (!ec) {
        soundManager_->read(toSend_, BUFFER_SIZE_FLOAT);
    } else {
        std::cerr << "Error: " << ec.message() << std::endl;
    }

    socket_.async_send_to(
        boost::asio::buffer(toSend_, BUFFER_SIZE_BYTES),
        remoteEndpoint_,
        boost::bind(&SoundServerHandler::handleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SoundServerHandler::setRunningStatus(bool status)
{
    isRunning_ = status;
}

void SoundServerHandler::dispatchUdpPackets(const bool* isRunning)
{
    try {
        soundManager_->start();
        audioController_.sleep(50);
        int data;

        while (*isRunning) {
            size_t i = soundManager_->read(toSend_, 1024);
            std::cout << "Sending: " << i << " floats." << std::endl;
            socket_.send_to(boost::asio::buffer(toSend_, i * sizeof(float)), remoteEndpoint_);
            size_t read_size = socket_.receive_from(boost::asio::buffer(toReceive_, 1024 * sizeof(float)),
                                                    remoteEndpoint_);
            std::cout << "Writing: " << read_size / sizeof(float) << " floats." << std::endl;
            soundManager_->write(toReceive_, read_size / sizeof(float));
        }
        soundManager_->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
