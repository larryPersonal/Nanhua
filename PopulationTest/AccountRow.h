//
//  AccountRow.h
//  PopulationTest
//
//  Created by admin on 7/7/14.
//
//

#ifndef PopulationTest_AccountRow_h
#define PopulationTest_AccountRow_h

#include "cocos2d.h"
#include "UserAccount.h"
#include "ScrollArea.h"

using namespace cocos2d;

class AccountRow : CCObject
{
private:
    UserAccount* userAccount;
    
    CCLabelTTF* accountNameLabel;
    
    CCMenuItem* resetPasswordButton;
    CCMenuItem* deleteAccountButton;
    CCMenuItem* activateScenario1Button;
    CCMenuItem* activateScenario2Button;
    CCMenuItem* activateScenario3Button;
    CCMenuItem* activateScenario4Button;
    CCMenuItem* activateScenario5Button;
    
    CCMenuItem* deActivateScenario1Button;
    CCMenuItem* deActivateScenario2Button;
    CCMenuItem* deActivateScenario3Button;
    CCMenuItem* deActivateScenario4Button;
    CCMenuItem* deActivateScenario5Button;
    
    CCMenu* accountRowMenu;
    
    ScrollArea* scrollArea;
    
    int index;
    
public:
    AccountRow(ScrollArea* scrollArea, int index);
    ~AccountRow();
    
    static AccountRow* create(ScrollArea* scrollArea, int index);
    
    UserAccount* getUserAccount();
    
    void constructAccountRow();
    
    void clickResetPasswordButton();
    void clickDeleteAccountButton();
    void clickActivateScenario1Button();
    void clickActivateScenario2Button();
    void clickActivateScenario3Button();
    void clickActivateScenario4Button();
    void clickActivateScenario5Button();
    
    void clickDeactivateScenario1Button();
    void clickDeactivateScenario2Button();
    void clickDeactivateScenario3Button();
    void clickDeactivateScenario4Button();
    void clickDeactivateScenario5Button();
    
    void refresh();
    
    void clear();
};

#endif
