//
//  NotificationPopup.h
//  PopulationTest
//
//  Created by admin on 22/8/14.
//
//

#ifndef PopulationTest_NotificationPopup_h
#define PopulationTest_NotificationPopup_h

#include "cocos2d.h"

using namespace cocos2d;

class NotificationPopup : public CCLayer
{
public:
    static NotificationPopup* SP;
    
    CCSprite* blackScreen;
    CCSprite* background;
    // CCSprite* cancelButton;
    
    bool active;
    bool isJumpIn;
    bool isJumpOut;
    
    float targetScale;
    
    // scenario 3: build 2 hospitals
    CCLabelTTF* congratsTitleLabel_scenario3;
    CCLabelTTF* congratsContentLabel_scenario3;
    
    bool proceedToNextLevel;
    
public:
    NotificationPopup();
    ~NotificationPopup();
    
    static NotificationPopup* create();
    static NotificationPopup* getThis();
    
    void showUI();
    void hideUI();
    
    void scheduleJumpIn();
    void scheduleJumpOut();
    
    void jumpIn(float deltaTime);
    void jumpOut(float deltaTime);
    
    void createUI();
    void clearUI();
    
    void clickNotificationButton();
    
    void showScenario3Congratulations();
    void showScenario4Congratulations();
    void showScenario5Congratulations();
};

#endif
