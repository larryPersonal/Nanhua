//
//  TutorialManager.cpp
//  PopulationTest
//
//  Created by admin on 1/4/14.
//
//

#include "TutorialManager.h"
#include "GameHUD.h"
#include "SoundTrackManager.h"
#include "GameScene.h"
#include "GameManager.h"
#include "Senario.h"

TutorialManager* TutorialManager::SP;

TutorialManager* TutorialManager::create()
{
    TutorialManager *pRet = new TutorialManager();
    if (pRet)
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


TutorialManager* TutorialManager::getThis()
{
    return SP;
}

TutorialManager::TutorialManager()
{
    TutorialManager::SP = this;
    lockMap = false;
    lockVillager = false;
    miniDragon = NULL;
    narrator = NULL;
    active = false;
    inText = false;
    inDisplay = false;
    inDraggon = false;
    
    lockBuildButton = true;
    lockObjectiveButton = true;
    lockScroll = true;
    lockSystemButton = false;
    lockTimeGroup = true;
    lockBuildScroll = true;
    lockBuildingButton = true;
    lockPopup = true;
    lockButtonOk = true;
    lockButtonCancel = true;
    lockButtonClose = true;
    lockManpowerSelect = true;
    
    teachBuildButton = false;
    teachBuildHouse = false;
    teachFarming = false;
}

TutorialManager::~TutorialManager()
{
     if(miniDragon != NULL)
    {
        delete miniDragon;
    }
    TutorialManager::SP = NULL;
    
}

void TutorialManager::unlockAll()
{
    lockBuildButton = false;
    lockObjectiveButton = false;
    lockScroll = false;
    lockSystemButton = false;
    lockTimeGroup = false;
    lockBuildScroll = false;
    lockBuildingButton = false;
    lockPopup = false;
    lockButtonOk = false;
    lockButtonCancel = false;
    lockButtonClose = false;
    lockManpowerSelect = false;
    lockMap = false;
    lockVillager = false;
    active = false;
    
    teachBuildButton = false;
    teachBuildHouse = false;
    teachFarming = false;
}

void TutorialManager::setupForTutorial()
{
    active = true;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    GameHUD* hudlayer = GameHUD::create();
    GameScene::getThis()->setTouchEnabled(true);
    GameScene::getThis()->addChild(hudlayer, 1);
    SoundtrackManager::PlayBGM("Ishikari Lore.mp3");
    
    if(GameScene::getThis()->systemConfig->skipTutorial)
    {
        GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::FirstRunPopulate), 0.1f);
        unlockAll();
        if(!GameScene::getThis()->systemConfig->skipSenario)
        {
            GameHUD::getThis()->pause = true;
            std::string filename = "tutorial.xml";
            Senario::getThis()->playSenario(filename.c_str());
        }
        return;
    }
    
    GameScene::getThis()->mapHandler->Populate(GameScene::getThis()->buildingHandler->allBuildingLayers);
    
    GameScene::getThis()->spriteHandler->initialize();
    
    GameManager::getThis()->initGameData();
    
    CCPoint location = CCPointMake(28, 36);
    
    GameScene::getThis()->spriteHandler->addSpriteToMap(location, V_REFUGEE);
    
    location.x += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(location, V_REFUGEE);
    
    location.x += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(location, V_REFUGEE);
    
    location.y += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(location, V_REFUGEE);
    
    location.x -= 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(location, V_REFUGEE);
    
    this->target = CCPointMake(32,46);
    this->target = GameScene::getThis()->mapHandler->locationFromTilePos(&target);
    
    lockVillager = true;
    lockMap = false;
    GameManager::getThis()->UpdateUnlocks();
    GameScene::getThis()->schedule(schedule_selector(GameScene::update), 1.0f/60.0f);
    GameScene::getThis()->mapHandler->rescaleScrollLimits();
    GameHUD::getThis()->update(0.1f);
    GameHUD::getThis()->pause = true;
    this->schedule(schedule_selector( TutorialManager::moveCameraToGate ), 1.0f / 120.0f);
}

void TutorialManager::moveCameraToGate(float dt)
{
    // get the screen dimension.
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the currentPosition
    float xPos = - GameScene::getThis()->mapHandler->getMap()->getPositionX();
    float yPos = - GameScene::getThis()->mapHandler->getMap()->getPositionY();
    
    // get the horizontal and vertical length difference between the screen centre and the target position!
    float xDiff = target.x - xPos;
    float yDiff = target.y - yPos;
    
    // set the horizontal speed and proportionate vertical speed, so that the object will move to the target at the same time both horizontally or vertically!
    float xSpeed = 0;
    if(xDiff < 0)
    {
        xSpeed = -10.0f;
    }
    else
    {
        xSpeed = 10.0f;
    }
    
    float ySpeed = xSpeed * (yDiff / xDiff);
    
    // update the position of the screen centre.
    GameScene::getThis()->mapHandler->moveMapBy(-xSpeed, -ySpeed);
    
    if(xSpeed > 0)
    {
        if(-GameScene::getThis()->mapHandler->getMap()->getPositionX() >= TutorialManager::getThis()->target.x)
        {
            GameScene::getThis()->mapHandler->getMap()->setPosition(-TutorialManager::getThis()->target.x, -TutorialManager::getThis()->target.y);
            this->unschedule(schedule_selector( TutorialManager::moveCameraToGate ));
            scheduleFadeOut(255, 5);
        }
    }
    else
    {
        if(-GameScene::getThis()->mapHandler->getMap()->getPositionX() <= TutorialManager::getThis()->target.x)
        {
            GameScene::getThis()->mapHandler->getMap()->setPosition(-TutorialManager::getThis()->target.x, -TutorialManager::getThis()->target.y);
            this->unschedule(schedule_selector( TutorialManager::moveCameraToGate ));
            scheduleFadeOut(255, 5);
        }
    }
    
    GameScene::getThis()->mapHandler->rescaleScrollLimits();
    GameScene::getThis()->mapHandler->moveMapBy(0.0f, 0.0f);
}

void TutorialManager::fadeOut(float dt)
{
    CCSprite* mask = GameHUD::getThis()->mask;
    float currentOpacity = (float) mask->getOpacity();
    currentOpacity += fadeSpeed;
    if(currentOpacity >= targetOpacity)
    {
        currentOpacity = targetOpacity;
        this->unschedule(schedule_selector( TutorialManager::fadeOut ));
        if(narrator == NULL)
        {
            setupNarrator();
        }
    }
    mask->setOpacity((GLubyte) currentOpacity);
}

void TutorialManager::fadeIn(float dt)
{
    CCSprite* mask = GameHUD::getThis()->mask;
    float currentOpacity = (float) mask->getOpacity();
    currentOpacity -= fadeSpeed;
    if(currentOpacity <= targetOpacity)
    {
        currentOpacity = targetOpacity;
        this->unschedule(schedule_selector( TutorialManager::fadeIn ));
    }
    mask->setOpacity((GLubyte) currentOpacity);
}

void TutorialManager::scheduleFadeIn(float tOpacity, float speed)
{
    fadeSpeed = speed;
    targetOpacity = tOpacity;
    this->schedule(schedule_selector( TutorialManager::fadeIn ), 1.0f / 120.0f);
}

void TutorialManager::scheduleFadeOut(float tOpacity, float speed)
{
    fadeSpeed = speed;
    targetOpacity = tOpacity;
    this->schedule(schedule_selector( TutorialManager::fadeOut ), 1.0f / 120.0f);
}

void TutorialManager::setupNarrator()
{
    narrator = new Narrator();
    inDisplay = true;
    narrator->display();
}

void TutorialManager::setupMiniDragon()
{
    miniDragon = new MiniDragon();
    inDraggon = true;
    miniDragon->display();
}


