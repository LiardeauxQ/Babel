//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_USERSESSION_HPP
#define BABEL_SERVER_USERSESSION_HPP

#include <string>
#include <vector>

class UserSession {
public:
    static UserSession *get()
    {
        if (!session_)
            session_ = new UserSession();
        return session_;
    }

    void connectUser(const std::string &username)
    {
        isConnected_ = true;
        username_ = username;
    }

    bool isConnected() const { return isConnected_; }

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

    const std::string &getUsername() const { return username_; }

private:
    UserSession() :
            isConnected_(false)
    {}

    ~UserSession()
    {
        delete session_;
    }

    bool isConnected_;
    std::string username_;
    std::vector<std::string> friends_;
    static UserSession *session_;
};

#endif //BABEL_SERVER_USERSESSION_HPP
