//
// Created by Quentin Liardeaux on 10/11/19.
//

#include "InputOptionsHandler.hpp"

std::vector<InputOptionsHandler::OptionInfo> InputOptionsHandler::options_ = {
    OptionInfo { UDP_ONLY, "-u", "--udpOnly" },
    OptionInfo { REMOTE_IP, "-r", "--remoteIp" },
    OptionInfo { LOCAL_IP, "-l", "--localIp" },
    OptionInfo { PORT, "-p", "--port" },
    OptionInfo { HELP, "-h", "--help" },
};

InputOptionsHandler::InputOptionsHandler(int argc, char** argv)
    : argc_(argc)
    , argv_(argv)
{
}

InputOptionsHandler::OptionInfo InputOptionsHandler::getOption(OptionId id)
{
    for (auto option : options_) {
        if (option.id == id) {
            return option;
        }
    }
    return OptionInfo {};
}

std::string InputOptionsHandler::getOptionValue(OptionInfo& info)
{
    std::string value;

    for (int i = 0; i < argc_; i++) {
        if (!std::strcmp(info.shortName.c_str(), argv_[i])
            || !std::strcmp(info.longName.c_str(), argv_[i])) {
            value = (i + 1 == argc_) ? argv_[i] : argv_[i + 1];
            break;
        }
    }
    return value;
}

std::string InputOptionsHandler::getLocalIp()
{
    OptionInfo info = getOption(LOCAL_IP);
    std::string ip = getOptionValue(info);

    if (ip.size() == 0 || ip == info.shortName || ip == info.longName)
        throw std::logic_error("Cannot find local Ip option");
    return ip;
}

std::string InputOptionsHandler::getRemoteIp()
{
    OptionInfo info = getOption(REMOTE_IP);
    std::string ip = getOptionValue(info);

    if (ip.size() == 0 || ip == info.shortName || ip == info.longName)
        throw std::logic_error("Cannot find remote Ip option");
    return ip;
}

short InputOptionsHandler::getPort()
{
    OptionInfo info = getOption(PORT);
    std::string port = getOptionValue(info);

    if (port.size() == 0 || port == info.shortName || port == info.longName)
        throw std::logic_error("Cannot find port option");
    return std::stoi(port);
}

bool InputOptionsHandler::isUdpOnly()
{
    OptionInfo info = getOption(UDP_ONLY);
    std::string value = getOptionValue(info);

    if (value.size() > 0 && (value == info.longName || value == info.shortName))
        return true;
    return false;
}