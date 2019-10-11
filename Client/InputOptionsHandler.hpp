//
// Created by Quentin Liardeaux on 10/11/19.
//

#ifndef BABEL_SERVER_INPUTOPTIONSHANDLER_HPP
#define BABEL_SERVER_INPUTOPTIONSHANDLER_HPP

#include <vector>
#include <tuple>
#include <string>
#include <stdexcept>

class InputOptionsHandler {
public:
    InputOptionsHandler(int argc, char **argv);
    ~InputOptionsHandler() = default;

    std::string getLocalIp();
    std::string getRemoteIp();
    short getPort();
    bool isUdpOnly();

private:
    enum OptionId {
        UDP_ONLY,
        REMOTE_IP,
        LOCAL_IP,
        PORT,
        HELP,
    };

    struct OptionInfo {
        OptionId id;
        std::string shortName;
        std::string longName;
    };

    OptionInfo getOption(OptionId id);
    std::string getOptionValue(OptionInfo &info);

    int argc_;
    char **argv_;

    static std::vector<OptionInfo> options_;
};


#endif //BABEL_SERVER_INPUTOPTIONSHANDLER_HPP
