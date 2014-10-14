//
//  UserProfile.cpp
//  PopulationTest
//
//  Created by admin on 2/9/14.
//
//

#include "UserProfile.h"

UserProfile* UserProfile::SP;

UserProfile::UserProfile()
{
    UserProfile::SP = this;
    
    username = "";
    password = "";
    gender = false;
    isLoggedIn = false;
    hasGameHUD = false;
    
    configSettings = new ConfigSettings();
    settingsLevel = new SettingsLevel();
    systemConfig = new SystemConfig();
    
    gameLevel = 0;
    number_of_hospital = 0;
}

UserProfile::~UserProfile()
{
    UserProfile::SP = NULL;
}

UserProfile* UserProfile::getThis()
{
    return SP;
}