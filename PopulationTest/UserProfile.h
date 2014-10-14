//
//  UserProfile.h
//  PopulationTest
//
//  Created by admin on 2/9/14.
//
//

#ifndef PopulationTest_UserProfile_h
#define PopulationTest_UserProfile_h

#include "cocos2d.h"
#include "GameConfig.h"

using namespace cocos2d;

class UserProfile
{
private:
    static UserProfile* SP;
    
public:
    int gameLevel;
    int number_of_hospital;
    
    /* player profile */
    std::string username;
    std::string password;
    bool gender;
    bool isLoggedIn;
    bool hasGameHUD;
    
    ConfigSettings* configSettings;
    SettingsLevel* settingsLevel;
    SystemConfig* systemConfig;
    
public:
    UserProfile();
    ~UserProfile();
    
    static UserProfile* getThis();
};

#endif
