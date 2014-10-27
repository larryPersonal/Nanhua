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
#include "GlobalHelper.h"
#include "UIButtonControl.h"

SystemMenu* SystemMenu::SP;

SystemMenu* SystemMenu::getThis()
{
    return SP;
}

SystemMenu* SystemMenu::create()
{
    SystemMenu* sm = new SystemMenu();
    
    if(sm)
    {
        sm->autorelease();
        return sm;
    }
    else
    {
        CC_SAFE_DELETE(sm);
        return NULL;
    }
}

SystemMenu::SystemMenu()
{
    SystemMenu::SP = this;
    menuItems = CCArray::create();
    menuItems->retain();
    
    show = false;
    hide = false;
    isActive = false;
    
    goingToEndGame = false;
}

SystemMenu::~SystemMenu()
{
    SystemMenu::SP = NULL;
    menuItems->removeAllObjects();
    CC_SAFE_RELEASE(menuItems);
}

void SystemMenu::createUI()
{
    loadTexturtes();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    blackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    blackScreen->setOpacity((GLubyte) 0);
    this->addChild(blackScreen, 31);
    
    systemMenu_background = CCSprite::createWithSpriteFrameName("PauseMenu.png");
    systemMenu_background->setScale(1.0f);
    systemMenu_background->setAnchorPoint(ccp(0.5, 0.5));
    systemMenu_background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(systemMenu_background, 32);
    
    systemMenu_resumeButton = CCMenuItemImage::create("resumebtn.png", "resumepressbtn.png", this, menu_selector(SystemMenu::clickResumeButton));
    systemMenu_resumeButton = CCMenuItemImage::create("resumebtn.png", "resumepressbtn.png", this, menu_selector( SystemMenu::clickResumeButton ));
    systemMenu_resumeButton->setScale((systemMenu_background->boundingBox().size.width / systemMenu_resumeButton->boundingBox().size.width) * 0.6f, systemMenu_background->boundingBox().size.height / systemMenu_resumeButton->boundingBox().size.height * 0.15f);
    systemMenu_resumeButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_resumeButton->setPosition(ccp(systemMenu_background->boundingBox().size.width / 2.0f, systemMenu_background->boundingBox().size.height / 2.0f + systemMenu_resumeButton->boundingBox().size.height * 2));
    
    menuItems->addObject(systemMenu_resumeButton);
    
    systemMenu_restartButton = CCMenuItemImage::create("system_load.png", "system_load_press.png", this, menu_selector( SystemMenu::clickRestartButton ));
    systemMenu_restartButton->setScale(systemMenu_resumeButton->boundingBox().size.width / systemMenu_restartButton->boundingBox().size.width, systemMenu_resumeButton->boundingBox().size.height / systemMenu_restartButton->boundingBox().size.height);
    systemMenu_restartButton->setAnchorPoint(ccp(0.5, 1));
    systemMenu_restartButton->setPosition(ccp(systemMenu_background->boundingBox().size.width / 2.0f, systemMenu_background->boundingBox().size.height / 2.0f + systemMenu_resumeButton->boundingBox().size.height));
    
    menuItems->addObject(systemMenu_restartButton);
    
    systemMenu_optionButton = CCMenuItemImage::create("system_save.png", "system_save_press.png", this, menu_selector( SystemMenu::clickOptionButton ));
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
}

void SystemMenu::clickResumeButton()
{
    GlobalHelper::resumeAllVillagers();
    scheduleHideSystemMenu();
}

void SystemMenu::clickOptionButton()
{
    CCLog("Option Button: Under Construction!");
    // 0 means auto save, 1 means fixed save, 2 means custom save
    GameManagement::saveGameToFile(2);
}

void SystemMenu::clickExitButton()
{
    GameScene::getThis()->setTouchEnabled(false);
    
    CCArray* allBuildings = BuildingHandler::getThis()->allBuildingsOnMap;
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* bui = (Building*) allBuildings->objectAtIndex(i);
        bui->stopUpdate();
    }
    
    CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
    for(int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
        gs->stopAction = true;
        gs->StopMoving();
    }
    
    GameScene::getThis()->isEndingGame = true;
    GameScene::getThis()->unschedule(schedule_selector(GameScene::scrollToCenter));
    GameScene::getThis()->unschedule(schedule_selector(GameScene::decceleratingDragging));
    
    goingToEndGame = true;
    
    scheduleHideSystemMenu();
}

void SystemMenu::clickRestartButton()
{
    CCLog("Restart Button: Under Construction!");
    GameManagement::loadGameFile();
}

void SystemMenu::scheduleShowSystemMenu()
{
    if(!show && !hide)
    {
        isActive = true;
        show = true;
        createUI();
        this->schedule(schedule_selector(SystemMenu::showSystemMenu), 1.0f/120.0f);
    }
}

void SystemMenu::scheduleHideSystemMenu()
{
    if(!show && !hide)
    {
        isActive = false;
        hide = true;
        this->schedule(schedule_selector(SystemMenu::hideSystemMenu), 1.0f/120.0f);
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
        this->unschedule(schedule_selector(SystemMenu::showSystemMenu));
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
        this->unschedule(schedule_selector(SystemMenu::hideSystemMenu));
        
        UIButtonControl::resumeGame();
        
        if(SystemMenu::getThis()->goingToEndGame)
        {
            SystemMenu::getThis()->goingToEndGame = false;
            GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::stopGame), 0.1f);
        }
        
        float opacity = 255.0f * (scale / 1.0f);
        
        SystemMenu::getThis()->blackScreen->setOpacity((GLubyte) opacity);
        SystemMenu::getThis()->systemMenu_background->setScale(scale);
        
        clear();
    }
    else
    {
        float opacity = 255.0f * (scale / 1.0f);
        
        SystemMenu::getThis()->blackScreen->setOpacity((GLubyte) opacity);
        SystemMenu::getThis()->systemMenu_background->setScale(scale);
    }
}

void SystemMenu::clickSystemButton()
{
    if(isActive)
    {
        scheduleHideSystemMenu();
        UIButtonControl::resumeGame();
    }
    else
    {
        scheduleShowSystemMenu();
        UIButtonControl::pauseGame();
    }
}

void SystemMenu::clear()
{
    menuItems->removeAllObjects();
    SystemMenu::getThis()->newMenu->removeChild(SystemMenu::getThis()->systemMenu_resumeButton, true);
    SystemMenu::getThis()->newMenu->removeChild(SystemMenu::getThis()->systemMenu_restartButton, true);
    SystemMenu::getThis()->newMenu->removeChild(SystemMenu::getThis()->systemMenu_optionButton, true);
    SystemMenu::getThis()->newMenu->removeChild(SystemMenu::getThis()->systemMenu_exitButton, true);
    
    SystemMenu::getThis()->systemMenu_background->removeChild(SystemMenu::getThis()->newMenu, true);
    
    this->removeChild(SystemMenu::getThis()->blackScreen, true);
    
    this->removeChild(SystemMenu::getThis()->systemMenu_background, true);
    
    SystemMenu::getThis()->systemMenu_background = NULL;
    SystemMenu::getThis()->blackScreen = NULL;
    
    releaseTextures();
    
    GlobalHelper::clearCache();
}

void SystemMenu::loadTexturtes()
{
    systemMenuNode = CCSpriteBatchNode::create("SystemMenu.png");
    this->addChild(systemMenuNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SystemMenu.plist");
}

void SystemMenu::releaseTextures()
{
    this->removeChild(systemMenuNode, true);
    SystemMenu::getThis()->systemMenuNode = NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("SystemMenu.plist");
}