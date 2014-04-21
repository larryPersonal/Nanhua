//
//  SystemMenu.h
//  PopulationTest
//
//  Created by Serious Games on 27/5/13.
//
//

#ifndef __PopulationTest__SystemMenu__
#define __PopulationTest__SystemMenu__

#include "cocos2d.h"

using namespace cocos2d;

class SystemMenu : public CCObject
{
    
public:
    static SystemMenu* SP;
    
    CCSprite* blackScreen;
    CCSprite* systemMenu_background;
    CCMenuItemImage* systemMenu_resumeButton;
    CCMenuItemImage* systemMenu_optionButton;
    CCMenuItemImage* systemMenu_restartButton;
    CCMenuItemImage* systemMenu_exitButton;
    
    CCArray* menuItems;
    CCMenu* newMenu;
    
    bool show;
    bool hide;
    
public:
    static SystemMenu* create(CCLayer*);
    SystemMenu(CCLayer*);
    ~SystemMenu();
    bool init(CCLayer*);
    static SystemMenu* getThis();
    
    void clickResumeButton();
    void clickOptionButton();
    void clickExitButton();
    void clickRestartButton();
    
    void scheduleShowSystemMenu();
    void scheduleHideSystemMenu();
    void showSystemMenu(float);
    void hideSystemMenu(float);
    void removeSystemMenu();
    
    void releaseAll();
};

#endif /* defined(__PopulationTest__SystemMenu__) */