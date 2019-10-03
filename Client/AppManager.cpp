//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

AppManager::AppManager(std::unique_ptr<ServerRequest> request) :
    request_(std::move(request)),
    session_(UserSession()),
    notifHandler_(NotificationHandler()),
    widget_(ui::MainWidget())
{

}

void AppManager::start()
{
    widget_.resize(500, 500);
    widget_.show();
}

void AppManager::askToLog()
{

}