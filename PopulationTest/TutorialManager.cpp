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
#include "FloatingArraw.h"

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
    lockSpriteInfo = true;
    lockBuildingInfo = true;
    lockGoldLabel = true;
    lockFoodLabel = true;
    lockPopulationLabel = true;
    lockDropTokens = true;
    
    teachBuildButton = false;
    teachBuildHouse = false;
    teachFarming = false;
    teachBuildRoad = false;
    teachBuildFarm = false;
    
    clickable = false;
    highlightedBuilding = NULL;
    
    villagersToBeAdded = 0;
    cumulativeTime = 0;
    
    show = false;
    hide = false;
    
    goNarr = false;
    
    freeBuilding = false;
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
    lockSpriteInfo = false;
    lockGoldLabel = false;
    lockFoodLabel = false;
    lockPopulationLabel = false;
    lockBuildingInfo = false;
    lockDropTokens = false;
    
    teachBuildButton = false;
    teachBuildHouse = false;
    teachFarming = false;
    teachBuildGranary = false;
    teachBuildGuardTower = false;
    teachSoldier = false;
    teachFighting = false;
    
    freeBuilding = true;
}

void TutorialManager::lockAll()
{
    lockBuildButton = true;
    lockObjectiveButton = true;
    lockScroll = true;
    lockSystemButton = true;
    lockTimeGroup = true;
    lockBuildScroll = true;
    lockBuildingButton = true;
    lockPopup = true;
    lockButtonOk = true;
    lockButtonCancel = true;
    lockButtonClose = true;
    lockManpowerSelect = true;
    lockMap = true;
    lockVillager = true;
    lockSpriteInfo = true;
    lockGoldLabel = true;
    lockFoodLabel = true;
    lockPopulationLabel = true;
    lockBuildingInfo = true;
    lockDropTokens = true;
    
    freeBuilding = false;
}

void TutorialManager::setupForTutorial()
{
    active = true;
    lockDropTokens = true;
    
    GameHUD* hudlayer = GameHUD::create();
    GameScene::getThis()->setTouchEnabled(true);
    GameScene::getThis()->addChild(hudlayer, 1);
    SoundtrackManager::PlayBGM("in-game_1.wav");
    
    if(GameManager::getThis()->getLevel() == 2)
    {
        GameHUD::getThis()->setupForGuardTowerBar();
    }
    
    if(GameScene::getThis()->systemConfig->skipTutorial)
    {
        GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::FirstRunPopulate), 0.1f);
        unlockAll();
        
        if(!GameScene::getThis()->systemConfig->skipSenario && Senario::getThis()->scenarioState != Scenario1)
        {
            GameHUD::getThis()->pause = true;
            this->scheduleOnce(schedule_selector(TutorialManager::setupForScenario), 1.0f);
        }
        else
        {
            active = false;
        }
        
        ObjectiveHandler::getThis()->playObjective();
        
        return;
    }
    
    // animated rain is not used for this time, maybe in later time?
    //GameScene::getThis()->animatedRain = AnimatedRain::create();
    
    GameScene::getThis()->mapHandler->Populate(GameScene::getThis()->buildingHandler->allBuildingLayers);
    
    GameScene::getThis()->spriteHandler->initialize();
    
    GameManager::getThis()->initGameData();
    
    GameManager::getThis()->UpdateUnlocks();
    GameScene::getThis()->schedule(schedule_selector(GameScene::update), 1.0f/60.0f);
    GameScene::getThis()->mapHandler->rescaleScrollLimits();
    GameHUD::getThis()->update(0.1f);
    GameHUD::getThis()->pause = true;
    //this->schedule(schedule_selector( TutorialManager::moveCamera ), 1.0f / 120.0f);
    setupMiniDragon();
    
    FloatingArraw* fa = new FloatingArraw();
}

void TutorialManager::setupForScenario()
{
    GameManager::getThis()->setLevel(1);
    
    lockAll();
    target = ccp(4128.0f, 1800.0f);
    this->schedule(schedule_selector(TutorialManager::moveCamera), 1.0f / 120.0f);
    
    spritesArray = CCArray::create();
    spritesArray->retain();
    
    CCPoint target = CCPointMake(30,39);
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.y += 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x -= 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x -= 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x -= 1;
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE, true);
    
    goNarr = true;
    active = false;
}

void TutorialManager::addVillagers(int num)
{
    villagersToBeAdded = num;
    this->schedule(schedule_selector(TutorialManager::scheduleVillagers), 1.0f/120.0f);
}

void TutorialManager::scheduleVillagers(float dt)
{
    cumulativeTime += dt;
    
    if(villagersToBeAdded <= 0)
    {
        this->unschedule(schedule_selector(TutorialManager::scheduleVillagers));
    }
    
    if(cumulativeTime >= 1.0f)
    {
        cumulativeTime = 0;
        CCPoint location = CCPointMake(29, 33);
        GameScene::getThis()->spriteHandler->addSpriteToMap(location, V_REFUGEE);
        villagersToBeAdded--;
    }
}

void TutorialManager::moveCamera(float dt)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the currentPosition
    float xPos = - GameScene::getThis()->mapHandler->getMap()->getPositionX() + screenSize.width / 2.0f;
    float yPos = - GameScene::getThis()->mapHandler->getMap()->getPositionY() + screenSize.height / 2.0f;
    
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
    
    xPos = - GameScene::getThis()->mapHandler->getMap()->getPositionX() + screenSize.width / 2.0f;
    yPos = - GameScene::getThis()->mapHandler->getMap()->getPositionY() + screenSize.height / 2.0f;
    
    if(xSpeed > 0)
    {
        if(xPos >= TutorialManager::getThis()->target.x)
        {
            GameScene::getThis()->mapHandler->getMap()->setPosition(-TutorialManager::getThis()->target.x + screenSize.width / 2.0f, -TutorialManager::getThis()->target.y + screenSize.height / 2.0f);
            this->unschedule(schedule_selector( TutorialManager::moveCamera ));
            
            if(TutorialManager::getThis()->miniDragon != NULL)
            {
                if(TutorialManager::getThis()->miniDragon->lockClick)
                {
                    TutorialManager::getThis()->miniDragon->lockClick = false;
                }
                
                if(!miniDragon->debug)
                {
                    if(TutorialManager::getThis()->miniDragon->slidesList != NULL && TutorialManager::getThis()->miniDragon->curSlide < TutorialManager::getThis()->miniDragon->slidesList->count())
                    {
                        TutorialSlide* ts = (TutorialSlide*)TutorialManager::getThis()->miniDragon->slidesList->objectAtIndex(TutorialManager::getThis()->miniDragon->curSlide);
                        if(ts->highlight)
                        {
                            TutorialManager::getThis()->miniDragon->highlightBuilding(ts->highlightBuilding);
                        }
                    }
                }
            }
            
            if(goNarr)
            {
                setupNarrator();
                goNarr = false;
            }
        }
    }
    else
    {
        if(xPos <= TutorialManager::getThis()->target.x)
        {
            GameScene::getThis()->mapHandler->getMap()->setPosition(-TutorialManager::getThis()->target.x + screenSize.width / 2.0f, -TutorialManager::getThis()->target.y + screenSize.height / 2.0f);
            this->unschedule(schedule_selector( TutorialManager::moveCamera ));
            
            if(TutorialManager::getThis()->miniDragon != NULL)
            {
                if(TutorialManager::getThis()->miniDragon->lockClick)
                {
                    TutorialManager::getThis()->miniDragon->lockClick = false;
                }
                
                if(!miniDragon->debug)
                {
                    if(TutorialManager::getThis()->miniDragon->slidesList != NULL && TutorialManager::getThis()->miniDragon->curSlide < TutorialManager::getThis()->miniDragon->slidesList->count())
                    {
                        TutorialSlide* ts = (TutorialSlide*)TutorialManager::getThis()->miniDragon->slidesList->objectAtIndex(TutorialManager::getThis()->miniDragon->curSlide);
                        if(ts->highlight)
                        {
                            TutorialManager::getThis()->miniDragon->highlightBuilding(ts->highlightBuilding);
                        }
                    }
                }
            }
            
            if(goNarr)
            {
                setupNarrator();
                goNarr = false;
            }
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
    inText = true;
    narrator->display();
}

void TutorialManager::setupMiniDragon()
{
    miniDragon = new MiniDragon();
    inDraggon = true;
    miniDragon->playMiniDraggon();
}

void TutorialManager::clearSprites()
{
    for(int i = 0; i < spritesArray->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesArray->objectAtIndex(i);
        GameScene::getThis()->spriteHandler->removeSpriteFromMap(gs);
    }
    spritesArray->removeAllObjects();
    CC_SAFE_RELEASE(spritesArray);
}

void TutorialManager::scheduleForScenario(float time)
{
    this->scheduleOnce(schedule_selector(TutorialManager::setupForScenario), time);
}
