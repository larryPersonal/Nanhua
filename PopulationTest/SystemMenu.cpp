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
#include "GameManagement.h"

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
    
    show = false;
    hide = false;
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
    blackScreen->setOpacity((GLubyte) 0);
    layer->addChild(blackScreen, 31);
    
    systemMenu_background = CCSprite::create("PauseMenu.png");
    systemMenu_background->setScale(1.0f);
    systemMenu_background->setAnchorPoint(ccp(0.5, 0.5));
    systemMenu_background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    layer->addChild(systemMenu_background, 32);
    
    systemMenu_resumeButton = CCMenuItemImage::create("resumebtn.png", "resumepressbtn.png", this, menu_selector( SystemMenu::clickResumeButton ));
    systemMenu_resumeButton->setScale((systemMenu_background->boundingBox().size.width / systemMenu_resumeButton->boundingBox().size.width) * 0.6f, systemMenu_background->boundingBox().size.height / systemMenu_resumeButton->boundingBox().size.height * 0.15f);
    systemMenu_resumeButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_resumeButton->setPosition(ccp(systemMenu_background->boundingBox().size.width / 2.0f, systemMenu_background->boundingBox().size.height / 2.0f + systemMenu_resumeButton->boundingBox().size.height * 2));
    
    menuItems->addObject(systemMenu_resumeButton);
    
    systemMenu_restartButton = CCMenuItemImage::create("restartbtn.png", "restartpressbtn.png", this, menu_selector( SystemMenu::clickRestartButton ));
    systemMenu_restartButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_restartButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_restartButton->boundingBox().size.height);
    systemMenu_restartButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_restartButton->setPosition(ccp(systemMenu_background->boundingBox().size.width / 2.0f, systemMenu_background->boundingBox().size.height / 2.0f + systemMenu_resumeButton->boundingBox().size.height));
    
    menuItems->addObject(systemMenu_restartButton);
    
    systemMenu_optionButton = CCMenuItemImage::create("optionsbtn.png", "optionspressbtn.png", this, menu_selector( SystemMenu::clickOptionButton ));
    systemMenu_optionButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_optionButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_optionButton->boundingBox().size.height);
    systemMenu_optionButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_optionButton->setPosition(ccp(systemMenu_background->boundingBox().size.width / 2.0f, systemMenu_background->boundingBox().size.height / 2.0f));
    
    menuItems->addObject(systemMenu_optionButton);
    
    systemMenu_exitButton = CCMenuItemImage::create("exitbtn.png", "exitpressbtn.png", this, menu_selector( SystemMenu::clickExitButton ));
    systemMenu_exitButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_exitButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_exitButton->boundingBox().size.height);
    systemMenu_exitButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_exitButton->setPosition(ccp(systemMenu_background->boundingBox().size.width / 2.0f, systemMenu_background->boundingBox().size.height / 2.0f - systemMenu_resumeButton->boundingBox().size.height));
    
    menuItems->addObject(systemMenu_exitButton);
    
    newMenu = CCMenu::createWithArray(menuItems);
    newMenu->setPosition(CCPointZero);
    newMenu->setAnchorPoint(ccp(0.5f, 0.5f));
    
    systemMenu_background->addChild(newMenu);
    
    systemMenu_background->setScale(0);
    
    return true;
}

void SystemMenu::clickResumeButton()
{
    GameHUD::getThis()->pause = false;
    GameScene::getThis()->tapped = true;
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
        sp->followPath();
    }
    
    scheduleHideSystemMenu();
    //releaseAll();
}

void SystemMenu::clickOptionButton()
{
    CCLog("Option Button: Under Construction!");
    GameManagement::saveGameToFile();
}

void SystemMenu::clickExitButton()
{
    /*
    GameScene::getThis()->mapHandler->UnBuildEndGame();
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
    CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
    */
    GameScene::getThis()->enableLoadingScreen();
}

void SystemMenu::clickRestartButton()
{
    CCLog("Restart Button: Under Construction!");
    GameManagement::loadGameFile();
}

void SystemMenu::releaseAll()
{
    GameHUD::getThis()->removeChild(blackScreen);
    GameHUD::getThis()->removeChild(systemMenu_background);
    CC_SAFE_RELEASE(this);
}

void SystemMenu::scheduleShowSystemMenu()
{
    if(!show && !hide)
    {
        show = true;
        GameHUD::getThis()->schedule(schedule_selector(SystemMenu::showSystemMenu), 1.0f/120.0f);
    }
}

void SystemMenu::scheduleHideSystemMenu()
{
    if(!show && !hide)
    {
        hide = true;
        GameHUD::getThis()->schedule(schedule_selector(SystemMenu::hideSystemMenu), 1.0f/120.0f);
    }
}

void SystemMenu::showSystemMenu(float dt)
{
    float scale = SystemMenu::getThis()->systemMenu_background->getScale();
    scale += 0.1f;
    if(scale >= 1.0f)
    {
        scale = 1.0f;
        SystemMenu::getThis()->show = false;
        GameHUD::getThis()->unschedule(schedule_selector(SystemMenu::showSystemMenu));
    }
    
    float opacity = 255.0f * (scale / 1.0f);
    
    SystemMenu::getThis()->blackScreen->setOpacity((GLubyte) opacity);
    SystemMenu::getThis()->systemMenu_background->setScale(scale);
}

void SystemMenu::hideSystemMenu(float dt)
{
    float scale = SystemMenu::getThis()->systemMenu_background->getScale();
    scale -= 0.1f;
    if(scale <= 0)
    {
        scale = 0;
        SystemMenu::getThis()->hide = false;
        GameHUD::getThis()->unschedule(schedule_selector(SystemMenu::hideSystemMenu));
        SystemMenu::getThis()->removeSystemMenu();
    }
    else
    {
        float opacity = 255.0f * (scale / 1.0f);
        
        SystemMenu::getThis()->blackScreen->setOpacity((GLubyte) opacity);
        SystemMenu::getThis()->systemMenu_background->setScale(scale);
    }
}

void SystemMenu::removeSystemMenu()
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