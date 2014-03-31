//
//  SystemMenu.cpp
//  PopulationTest
//
//  Created by Serious Games on 27/5/13.
//
//

#include "SystemMenu.h"
#include "GameHUD.h"
#include "MainMenuScene.h"

SystemMenu* SystemMenu::SP;
SystemMenu* SystemMenu::create(CCLayer* layer)
{
    SystemMenu *pRet = new SystemMenu(layer);
    if (pRet && pRet->init(layer))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

SystemMenu* SystemMenu::getThis()
{
    return SP;
}

SystemMenu::SystemMenu(CCLayer* layer)
{
    SystemMenu::SP = this;
    menuItems = CCArray::create();
    menuItems->retain();
}

SystemMenu::~SystemMenu()
{
    SystemMenu::SP = NULL;
    menuItems->removeAllObjects();
    CC_SAFE_RELEASE(menuItems);
}

bool SystemMenu::init(CCLayer* layer)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    blackScreen = CCSprite::create("blackscreen.png");
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    layer->addChild(blackScreen, 5);
    
    systemMenu_background = CCSprite::create("PauseMenu.png");
    systemMenu_background->setScale(1.0f);
    systemMenu_background->setAnchorPoint(ccp(0.5, 0.5));
    systemMenu_background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    layer->addChild(systemMenu_background, 6);
    
    systemMenu_resumeButton = CCMenuItemImage::create("resumebtn.png", "resumepressbtn.png", this, menu_selector( SystemMenu::clickResumeButton ));
    systemMenu_resumeButton->setScale((systemMenu_background->boundingBox().size.width / systemMenu_resumeButton->boundingBox().size.width) * 0.6f, systemMenu_background->boundingBox().size.height / systemMenu_resumeButton->boundingBox().size.height * 0.15f);
    systemMenu_resumeButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_resumeButton->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + systemMenu_resumeButton->boundingBox().size.height * 2));
    
    menuItems->addObject(systemMenu_resumeButton);
    
    systemMenu_restartButton = CCMenuItemImage::create("restartbtn.png", "restartpressbtn.png", this, menu_selector( SystemMenu::clickRestartButton ));
    systemMenu_restartButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_restartButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_restartButton->boundingBox().size.height);
    systemMenu_restartButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_restartButton->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + systemMenu_resumeButton->boundingBox().size.height));
    
    menuItems->addObject(systemMenu_restartButton);
    
    systemMenu_optionButton = CCMenuItemImage::create("optionsbtn.png", "optionspressbtn", this, menu_selector( SystemMenu::clickOptionButton ));
    systemMenu_optionButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_optionButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_optionButton->boundingBox().size.height);
    systemMenu_optionButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_optionButton->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    menuItems->addObject(systemMenu_optionButton);
    
    systemMenu_exitButton = CCMenuItemImage::create("exitbtn.png", "exitpressbtn.png", this, menu_selector( SystemMenu::clickExitButton ));
    systemMenu_exitButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_exitButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_exitButton->boundingBox().size.height);
    systemMenu_exitButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_exitButton->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - systemMenu_resumeButton->boundingBox().size.height));
    
    menuItems->addObject(systemMenu_exitButton);
    
    newMenu = CCMenu::createWithArray(menuItems);
    newMenu->setPosition(CCPointZero);
    layer->addChild(newMenu, 7);
    
    return true;
}

void SystemMenu::clickResumeButton()
{
    GameHUD::getThis()->pause = false;
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
        sp->followPath();
    }
    releaseAll();
}

void SystemMenu::clickOptionButton()
{
    CCLog("Option Button: Under Construction!");
}

void SystemMenu::clickExitButton()
{
    GameScene::getThis()->mapHandler->UnBuildEndGame();
    CCDirector::sharedDirector()->pushScene(MainMenuScene::scene());
}

void SystemMenu::clickRestartButton()
{
    CCLog("Restart Button: Under Construction!");
}

void SystemMenu::releaseAll()
{
    GameHUD::getThis()->removeChild(blackScreen);
    GameHUD::getThis()->removeChild(systemMenu_background);
    GameHUD::getThis()->removeChild(newMenu);
    CC_SAFE_RELEASE(this);
}