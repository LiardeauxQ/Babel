//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_USERSESSION_HPP
#define BABEL_SERVER_USERSESSION_HPP

#include <string>
#include <vector>

class UserSession {
public:
    UserSession() :
        isConnected_(false)
    {}

    ~UserSession() = default;

    void connectUser(const std::string &username)
    {
        isConnected_ = true;
        username_ = username;
    }

    void addFriend(const std::string &username)
    {
        if (!isConnected_)
            return;
        friends_.push_back(username);
    }

    [[nodiscard]] const std::vector<std::string> &getFriends() const
    {
        return friends_;
    }
private:
    bool isConnected_;
    std::string username_;
    std::vector<std::string> friends_;
};

#endif //BABEL_SERVER_USERSESSION_HPP
