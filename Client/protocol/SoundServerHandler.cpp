//
// Created by Quentin Liardeaux on 10/6/19.
//

#include "SoundServerHandler.hpp"

SoundServerHandler::SoundServerHandler(const std::string &readIpAddress, int readPort,
                                       const std::string &writeIpAddress, int writePort) :
        context_(),
        readSocket_(BoostUdp::socket(context_)),
        writeSocket_(BoostUdp::socket(context_)),
        readIpAddress_(readIpAddress),
        readPort_(readPort),
        writeIpAddress_(writeIpAddress),
        writePort_(writePort)
{
    audioController_ = AudioController();
    soundManager_ = audioController_.createManager();
}

void SoundServerHandler::start()
{
    isRunning_ = true;
    std::cout << readIpAddress_ << std::endl;
    std::cout << readPort_ << std::endl;
    std::cout << writeIpAddress_ << std::endl;
    std::cout << writePort_ << std::endl;
    try {
        std::cout << writeSocket_.local_endpoint().port() << std::endl;
        std::cout << readSocket_.local_endpoint().port() << std::endl;

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Cannot init the udp connection between " << readIpAddress_;
        std::cerr << ":" << readPort_ << " and " << writeIpAddress_ << ":" << writePort_;
        std::cerr << std::endl;
        return;
    }
    soundThread_ = boost::thread(&SoundServerHandler::dispatchUdpPackets, this, &isRunning_);
}

void SoundServerHandler::stop()
{
    isRunning_ = false;
    readSocket_.close();
    writeSocket_.close();
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
            writeSocket_.send(boost::asio::buffer(data));
            readSocket_.receive(boost::asio::buffer(data));
            soundManager_->write(data);
        }
        soundManager_->stop();
    } catch (const AudioControllerError &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
