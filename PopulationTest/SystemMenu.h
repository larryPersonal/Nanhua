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

class SystemMenu : public CCLayer
{
    
public:
    static SystemMenu* SP;
    
    CCSprite* blackScreen;
    CCSprite* systemMenu_background;
    CCMenuItem* systemMenu_resumeButton;
    CCMenuItem* systemMenu_optionButton;
    CCMenuItem* systemMenu_restartButton;
    CCMenuItem* systemMenu_exitButton;
    
    CCArray* menuItems;
    CCMenu* newMenu;
    
    bool show;
    bool hide;
    
    bool goingToEndGame = false;
    
    CCSpriteBatchNode* systemMenuNode;
    
    bool isActive;
    
public:
    SystemMenu();
    ~SystemMenu();
    void createUI();
    static SystemMenu* getThis();
    static SystemMenu* create();
    
    void clickResumeButton();
    void clickOptionButton();
    void clickExitButton();
    void clickRestartButton();
    
    void scheduleShowSystemMenu();
    void scheduleHideSystemMenu();
    void showSystemMenu(float);
    void hideSystemMenu(float);
    
    void clickSystemButton();
    
    void clear();
    
    void loadTexturtes();
    void releaseTextures();
};

#endif /* defined(__PopulationTest__SystemMenu__) */