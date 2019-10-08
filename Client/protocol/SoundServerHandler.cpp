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
        while (isRunning) {
            soundManager_->read(data);
            std::cout << data.size() << std::endl;
            std::cout << "while send" << std::endl;
            socket_.send_to(boost::asio::buffer(data), remoteEndpoint_);
            std::cout << "while receive" << std::endl;
            socket_.receive_from(boost::asio::buffer(data), remoteEndpoint_);
            std::cout << "has received" << std::endl;
            soundManager_->write(data);
            audioController_.sleep(100);
        }
        soundManager_->stop();
    } catch (const AudioControllerError &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
