//
//  UserAccount.h
//  PopulationTest
//
//  Created by admin on 7/7/14.
//
//

#ifndef PopulationTest_UserAccount_h
#define PopulationTest_UserAccount_h

#include "cocos2d.h"

using namespace cocos2d;

class UserAccount
{
private:
    std::string accountName;
    
    bool isScenario1_activated;
    bool isScenario2_activated;
    bool isScenario3_activated;
    bool isScenario4_activated;
    bool isScenario5_activated;
    
public:
    UserAccount();
    ~UserAccount();
    
    std::string getAccountName();
    void setAccountName(std::string accName);
    
    bool getScenario1Status();
    void setScenario1Status(bool flag);
    
    bool getScenario2Status();
    void setScenario2Status(bool flag);
    
    bool getScenario3Status();
    void setScenario3Status(bool flag);
    
    bool getScenario4Status();
    void setScenario4Status(bool flag);
    
    bool getScenario5Status();
    void setScenario5Status(bool flag);
};

#endif
