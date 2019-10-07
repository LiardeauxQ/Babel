//
// Created by Quentin Liardeaux on 10/6/19.
//

#include "SoundServerHandler.hpp"

SoundServerHandler::SoundServerHandler(BoostUdp::endpoint &remoteEndpoint) :
        ioService_(),
        remoteEndpoint_(remoteEndpoint),
        socket_(BoostUdp::socket(ioService_))
{
    audioController_ = AudioController();
    soundManager_ = audioController_.createManager();
}

void SoundServerHandler::start()
{
    isRunning_ = true;
    socket_.open(BoostUdp::v4());
    soundThread_ = boost::thread(&SoundServerHandler::dispatchUdpPackets, this, &isRunning_);
}

void SoundServerHandler::stop()
{
    isRunning_ = false;
    socket_.close();
    soundThread_.join();
}

void SoundServerHandler::dispatchUdpPackets(bool *isRunning)
{
    try {
        std::vector<float> data;

        data.reserve(512);
        soundManager_->start();
        while (soundManager_->isActive() && isRunning) {
            soundManager_->read(data);
            socket_.send_to(boost::asio::buffer(data), remoteEndpoint_);
            socket_.receive_from(boost::asio::buffer(data), remoteEndpoint_);
            soundManager_->write(data);
        }
        soundManager_->stop();
    } catch (const AudioControllerError &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
