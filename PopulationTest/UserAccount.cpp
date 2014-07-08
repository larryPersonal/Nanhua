//
//  UserAccount.cpp
//  PopulationTest
//
//  Created by admin on 7/7/14.
//
//

#include "UserAccount.h"

UserAccount::UserAccount()
{
    accountName = "";
    isScenario1_activated = false;
    isScenario2_activated = false;
    isScenario3_activated = false;
    isScenario4_activated = false;
    isScenario5_activated = false;
}

UserAccount::~UserAccount()
{
    
}

std::string UserAccount::getAccountName()
{
    return accountName;
}

void UserAccount::setAccountName(std::string accName)
{
    accountName = accName;
}

bool UserAccount::getScenario1Status()
{
    return isScenario1_activated;
}

void UserAccount::setScenario1Status(bool flag)
{
    isScenario1_activated = flag;
}

bool UserAccount::getScenario2Status()
{
    return isScenario2_activated;
}

void UserAccount::setScenario2Status(bool flag)
{
    isScenario2_activated = flag;
}

bool UserAccount::getScenario3Status()
{
    return isScenario3_activated;
}

void UserAccount::setScenario3Status(bool flag)
{
    isScenario3_activated = flag;
}

bool UserAccount::getScenario4Status()
{
    return isScenario4_activated;
}

void UserAccount::setScenario4Status(bool flag)
{
    isScenario4_activated = flag;
}

bool UserAccount::getScenario5Status()
{
    return isScenario5_activated;
}

void UserAccount::setScenario5Status(bool flag)
{
    isScenario5_activated = flag;
}