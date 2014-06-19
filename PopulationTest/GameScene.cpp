//
//  GameScene.cpp
//  PopulationTest
//
//  Created by Serious Games on 7/3/13.
//
//

#include "GameScene.h"
#include "GameHUD.h"
#include "Senario.h"
#include "PopupMenu.h"
#include "Sprite.h"
#include "SoundtrackManager.h"
#include "GameDefaults.h"
#include "SpriteInfoMenu.h"
#include "SelectPopulation.h"
#include "GameManager.h"
#include "BuildingInfoMenu.h"
#include "BuildScroll.h"
#include "ReputationOrb.h"
#include "SystemMenu.h"
#include "TutorialManager.h"
#include "BuildingCard.h"
#include "MainMenuScene.h"
#include "ScoreMenu.h"
#include "GlobalHelper.h"

#include <cmath>

GameScene* GameScene::SP;

GameScene::GameScene()
{
    screenCenter = CCNode::create();
    mapHandler = new MapHandler();
    
    globalOutcomeModifier = new GlobalOutcomeModifier();
    
    cumulatedTime = 0;
    
    configSettings = new ConfigSettings();
    settingsLevel = new SettingsLevel();
    systemConfig = new SystemConfig();
    
    switch (GameManager::getThis()->getLevel())
    {
        case 0:
            CCLog("l0");
            settingsLevel->setLevel0();
            systemConfig->skipSenario = systemConfig->skipSenario_tutorial;
            systemConfig->skipTutorial = systemConfig->skipTutorial_tutorial;
            break;
        case 1:
            CCLog("l1");
            settingsLevel->setLevel0();
            systemConfig->skipSenario = systemConfig->skipSenario_level1;
            systemConfig->skipTutorial = systemConfig->skipTutorial_level1;
            break;
        case 2:
            CCLog("l2");
            settingsLevel->setLevel0();
            systemConfig->skipSenario = systemConfig->skipSenario_level2;
            systemConfig->skipTutorial = systemConfig->skipTutorial_level2;
            break;
        default:
            break;
    }
    
    hasBeenDragged = false;
    isInDeccelerating = false;
    scrollDistance = CCPointZero;
    
    tapped = false;
    isSwipe = false;
    
    teachBuildRoadCheckTime = 0;
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    this->initSharedTextureCache();
    
    /* loading screen module */
    loadingScreen = CCSprite::create("loading screen.png");
    loadingScreen->setScale(screenSize.width / loadingScreen->boundingBox().size.width);
    loadingScreen->setAnchorPoint(ccp(0.5, 0.5));
    loadingScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(loadingScreen, 10);
    loadingScreen->setVisible(false);
    
    loadingLabel = CCSprite::create("loading.png");
    loadingLabel->setAnchorPoint(ccp(0.5, 0.5));
    loadingLabel->setScale(0.5f);
    loadingLabel->setPosition(ccp(screenSize.width / 2.0f + 20.0f, screenSize.height / 2.0f - 120.0f));
    this->addChild(loadingLabel, 11);
    loadingLabel->setVisible(false);
    
    targetBuilding = NULL;
    
    clearCacheTime = 0;
    clearCacheTimeLimit = 10;
    
    mGameGuardTowerScore = 0;
}

GameScene::~GameScene()
{
    CCLog("GameScene Deleted");
    mapHandler->UnBuildEndGame();
    
    GameManager::getThis()->EndGameData();
    
    this->unschedule(schedule_selector(GameScene::update));
    this->removeAllChildren();
    
    delete mapHandler;
    delete buildingHandler;
    delete spriteHandler;
    delete jobCollection;
    delete constructionHandler;
    
    GameScene::SP=NULL;
}

CCScene* GameScene::scene()
{
    
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    SP = layer;
    
    scene->addChild(layer, 0);
    
    Senario* senlayer = Senario::create();
    //std::string filename = "senario_h.xml";
    
    ObjectiveHandler* objectiveHandler = ObjectiveHandler::create();
    objectiveHandler->loadObjective();
    
    TutorialManager* tm = TutorialManager::create();
    CCLog("Level is %d", GameManager::getThis()->getLevel());
    
    std::string filename = "senario_h.xml";
    
    if(GameManager::getThis()->getLevel() == 0)
    {
        filename = "introduction.xml";
        senlayer->scenarioState = Introduction;
        senlayer->playSenario(filename.c_str());
    }
    else if(GameManager::getThis()->getLevel() == 2)
    {
        filename = "scenario2.xml";
        senlayer->scenarioState = Scenario2;
        senlayer->playSenario(filename.c_str());
    }
    else
    {
        senlayer->scenarioState = Scenario1;
        tm->setupForTutorial();
        //objectiveHandler->playObjective(true);
    }
    
    scene->addChild(senlayer, 1);
    scene->addChild(tm, 1);
    scene->addChild(objectiveHandler, 1);
     
    return scene;
}

GameScene* GameScene::getThis()
{
    return SP;
}

bool GameScene::init()
{
    //The following are compulsory for all scenes!
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    lastTilePosPreview = ccp(INT_MAX, INT_MAX);
    firstPathPosPreview = ccp(INT_MAX, INT_MAX);
    lastPathPosPreview = ccp(INT_MAX, INT_MAX);

    //this->CCLayer::setTouchEnabled(true);
    currScale = 1.0f;
    
    setupScene();
    
    mapHandler->centerMap();
    
    return true;
}

void GameScene::setupScene()
{
    /* load game maps */
    int level = GameManager::getThis()->getLevel();
    if(level == 0)
    {
        mapHandler->initTiles("DemoScene.tmx");
        GameManager::getThis()->gameMap = "DemoScene.tmx";
    }
    else if(level == 1)
    {
        mapHandler->initTiles("DemoScene.tmx");
        GameManager::getThis()->gameMap = "DemoScene.tmx";
        // mapHandler->initTiles("DemoSceneEmpty.tmx");
        // GameManager::getThis()->gameMap = "DemoSceneEmpty.tmx";
    }
    else
    {
        mapHandler->initTiles("ScenarioScene2.tmx");
        GameManager::getThis()->gameMap = "ScenarioScene2.tmx";
    }
        
    if (mapHandler->getMap())
    {
        jobCollection = new JobCollection();
        buildingHandler = new BuildingHandler();
        buildingHandler->init(mapHandler->getMap(), jobCollection);
        
        screenCenter = CCNode::create();
        
        screenCenter->setAnchorPoint(ccp(0.5, 0.5));
        
        screenCenter->addChild(mapHandler->getScaleLayer(), -1);
        
        mapHandler->getScaleLayer()->setPosition(CCPointZero);
        
        mapHandler->getScaleLayer()->addChild(mapHandler->getMap(), 0);
        this->addChild(screenCenter);
        
        mapHandler->originateMapToTile();
    }
    
    spriteHandler = new SpriteHandler();
    constructionHandler = new ConstructionHandler();
    banditsAttackHandler = new BanditsAttackHandler();
    
    GlobalHelper::clearCache();
    
    initOrientationChange();
}

/*
 * Move function is the function used to handle the camera movement in the GameScene.
 */
void GameScene::move(float time)
{
    // get the screen dimension.
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the centre of the screen
    float xPos = screenCenter->getPositionX();
    float yPos = screenCenter->getPositionY();
    
    // get the horizontal and vertical length difference between the screen centre and the target position!
    float xDiff = screenSize.width / 2.0f - xPos;
    float yDiff = screenSize.height / 2.0f - yPos;
    
    // set the horizontal speed and proportionate vertical speed, so that the object will move to the target at the same time both horizontally or vertically!
    float xSpeed = 32.0f;
    float ySpeed = 32.0f * (yDiff / xDiff);
    
    
    
    // update the position of the screen centre.
    screenCenter->setPosition(xPos + xSpeed, yPos + ySpeed);
    
    if((xPos + xSpeed) > (screenSize.width / 2.0f))
    {
        Senario::getThis()->active = true;
        screenCenter->setPosition(screenSize.width / 2.0f, screenSize.height / 2.0f);
        this->unschedule(schedule_selector(GameScene::move));
    }
    
    mapHandler->rescaleScrollLimits();
    mapHandler->moveMapBy(0.0f, 0.0f);
}

void GameScene::initOrientationChange()
{
   // CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    //screenCenter->setPosition(screenSize.width * -3.0f, screenSize.height * 1.2f);
    mapHandler->rescaleScrollLimits();
    mapHandler->moveMapBy(0.0f, 0.0f);
}

void GameScene::onOrientationChanged()
{
    //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    //screenCenter->setPosition(screenSize.width * -4.0f, screenSize.height * 1.2f);
    mapHandler->rescaleScrollLimits();
    mapHandler->moveMapBy(0.0f, 0.0f);
}

void GameScene::enableTouch()
{
    this->CCLayer::setTouchEnabled(true);
    TutorialManager::getThis()->unlockAll();
    //GameHUD* hudlayer = GameHUD::create();
    //this->addChild(hudlayer, 1);
    //this->scheduleOnce(schedule_selector( GameScene::FirstRunPopulate) , 0.1f);
    //SoundtrackManager::PlayBGM("in-game_1.wav");
}

void GameScene::ccTouchesBegan(CCSet *touches, CCEvent *pEvent)
{
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    if(GameHUD::getThis() != NULL)
    {
        if(GameHUD::getThis()->moneyIcon->boundingBox().containsPoint(touchLoc))
        {
            CCLog("I clicked the money icon.");
        }
    }
    
    if(BuildingInfoMenu::getThis() != NULL)
    {
        if(BuildingInfoMenu::getThis()->selectWorkerButton->boundingBox().containsPoint(touchLoc) && BuildingInfoMenu::getThis()->selectWorkerButton->isVisible())
        {
            CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage("allocatebtn-press.png");
            BuildingInfoMenu::getThis()->selectWorkerButton->setTexture(tex);
        }
    }
}

void GameScene::ccTouchesMoved(CCSet *touches, CCEvent *pEvent){
    if(tapped)
    {
        tapped = false;
    }
    
    isSwipe = true;
    
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    // then check the tutorial manager
    if(TutorialManager::getThis()->active)
    {
        bool skip = false;
        
        if(TutorialManager::getThis()->inDisplay)
        {
            if(TutorialManager::getThis()->inText)
            {
                TutorialManager::getThis()->narrator->finishDisplay();
            }
            else
            {
                TutorialManager::getThis()->narrator->display();
            }
        }
        
        if(TutorialManager::getThis()->lockMap)
        {
            skip = true;
        }
        
        if(skip)
        {
            return;
        }
    }
    
    // the first priority for dragging on the screen is to check whether it is for the senario stage
    if(Senario::getThis()->active)
    {
        return;
    }
    // the second priority for draggin on the screen is to check whether it is for the system menu;
    else if(SystemMenu::getThis() != NULL && !(TutorialManager::getThis()->lockSystemButton))
    {
        if(SystemMenu::getThis()->systemMenu_background->boundingBox().containsPoint(touchLoc))
        {
            return;
        }
        else
        {
            SystemMenu::getThis()->scheduleHideSystemMenu();
        }
    }
    // the second priority for dragging on the screen is to check whether it is for the GameHUD layer
    else
    {
        bool skip = false;
        // check whether the dragging is on the buttons (GameHUD)
        if(GameHUD::getThis()->objectiveButton != NULL && !(TutorialManager::getThis()->lockObjectiveButton))
        {
            if(GameHUD::getThis()->objectiveButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->objectiveMenu != NULL && !(TutorialManager::getThis()->lockObjectiveButton))
        {
            if(GameHUD::getThis()->objectiveMenu->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->peaceButton != NULL)
        {
            if(GameHUD::getThis()->peaceButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->warButton != NULL)
        {
            if(GameHUD::getThis()->warButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->stickHappinessButton != NULL)
        {
            if(GameHUD::getThis()->stickHappinessButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->resumeHappinessButton != NULL)
        {
            if(GameHUD::getThis()->resumeHappinessButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->pauseButton != NULL)
        {
            if(GameHUD::getThis()->pauseButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->resumeButton != NULL)
        {
            if(GameHUD::getThis()->resumeButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->systemButton != NULL && !(TutorialManager::getThis()->lockSystemButton))
        {
            if(GameHUD::getThis()->systemButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->buildButton != NULL && !(TutorialManager::getThis()->lockBuildButton))
        {
            if(GameHUD::getThis()->buildButton->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        if(ScoreMenu::getThis() != NULL && ScoreMenu::getThis()->background != NULL)
        {
            if(ScoreMenu::getThis()->background->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                ScoreMenu::getThis()->scheduleHideScoreMenu();
            }
        }
        
        if(GameHUD::getThis()->buildScroll != NULL && GameHUD::getThis()->buildScroll->buildMenu != NULL && !(TutorialManager::getThis()->lockBuildScroll || TutorialManager::getThis()->lockScroll))
        {
            if(GameHUD::getThis()->buildScroll->buildMenu->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                GameHUD::getThis()->buildScroll->scheduleScrollOut();
            }
        }
        
        if(GameHUD::getThis()->timeMenu != NULL && !(TutorialManager::getThis()->lockTimeGroup))
        {
            if(GameHUD::getThis()->timeMenu->boundingBox().containsPoint(touchLoc))
            {
                if(GameHUD::getThis()->scrolled_in)
                {
                    GameHUD::getThis()->scheduleScrollOut();
                } else {
                    GameHUD::getThis()->scheduleScrollIn();
                }
            }
            else
            {
                GameHUD::getThis()->scheduleScrollOut();
            }
        }
        
        // building info menu
        if(BuildingInfoMenu::getThis() != NULL && BuildingInfoMenu::getThis()->spriteBackground != NULL)
        {
            if(BuildingInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                if(!TutorialManager::getThis()->lockScroll)
                {
                    PopupMenu::closeAllPopupMenu();
                }
            }
        }
        
        if(SelectPopulation::getThis() != NULL && SelectPopulation::getThis()->spriteBackground != NULL)
        {
            if(SelectPopulation::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                if(!TutorialManager::getThis()->lockScroll)
                {
                    PopupMenu::closeAllPopupMenu();
                }
            }
        }
        
        if(SpriteInfoMenu::getThis() != NULL && SpriteInfoMenu::getThis()->spriteBackground != NULL)
        {
            if(SpriteInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                if(!TutorialManager::getThis()->lockScroll)
                {
                    PopupMenu::closeAllPopupMenu();
                }
            }
        }
        
        if(skip)
        {
            return;
        }
        
    }
    
    if(TutorialManager::getThis()->lockScroll)
    {
        return;
    }
    
    CCSetIterator it;
    CCTouch* touchOne;
    CCTouch* touchTwo;
    
    /*
    // handle the build button
    if(BuildScroll::getThis() != NULL)
    {
        BuildScroll::getThis()->closeMenu(true);
    }
    */
    
            //Zoom
            if (touches->count() == 2)
            {
                for ( it = touches->begin(); it != touches->end(); it++) {
                    if (it == touches->begin()) {
                        touchOne = (CCTouch*)*it;
                    } else {
                        touchTwo = (CCTouch*)*it;
                    }
                }
                
                CCDirector* dir = CCDirector::sharedDirector();
                
                CCPoint touchLocationOne = dir->convertToGL(touchOne->getLocationInView());
                CCPoint touchLocationTwo = dir->convertToGL(touchTwo->getLocationInView());
                
                CCPoint previousLocationOne = dir->convertToGL(touchOne->getPreviousLocationInView());
                CCPoint previousLocationTwo = dir->convertToGL(touchTwo->getPreviousLocationInView());
                
                float currentDistance = sqrt(pow(touchLocationOne.x - touchLocationTwo.x, 2.0f) +
                                             pow(touchLocationOne.y - touchLocationTwo.y, 2.0f));
                
                float previousDistance = sqrt(pow(previousLocationOne.x - previousLocationTwo.x, 2.0f) +
                                              pow(previousLocationOne.y - previousLocationTwo.y, 2.0f));
                
                //   float distanceDelta = currentDistance - previousDistance;
                float oldScale = currScale;
                float factor =  (currentDistance == 0 || previousDistance == 0)? 0.0f: currentDistance/previousDistance;
                currScale= clampf(oldScale * factor, 0.5f, 1.0f);
                
              //  CCPoint scaleCenter = ccpMidpoint(touchLocationOne, touchLocationTwo);
               // scaleCenter = mapHandler->pointOnMapFromTouchLocation(scaleCenter);
                
                mapHandler->scaleTo(currScale);
                
            }
            //Map dragging
            else
            {
                CCTouch* touch = (CCTouch*)*touches->begin();
                CCPoint moveDistance;
                
                moveDistance = ccpSub(touch->getLocation(), touch->getPreviousLocation());
                float dis = 0.0f;
                
                dis = sqrtf(moveDistance.x * moveDistance.x + moveDistance.y * moveDistance.y);
                
                if (!hasBeenDragged && fabs(convertDistanceFromPointToInch(dis)) < MOVE_INCH )
                {
                    return;
                }
                
                if (!hasBeenDragged)
                {
                    moveDistance = CCPointZero;
                }
                
                hasBeenDragged = true;
                
                float moveX = moveDistance.x;
                float moveY = moveDistance.y;
                
                scrollDistance = moveDistance;
                
                mapHandler->moveMapBy(moveX, moveY);
            }
    
    isThisTapCounted = false;
}

void GameScene::ccTouchesEnded(CCSet *touches, CCEvent *pEvent)
{
    /*
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the current position
    float xPos = - mapHandler->getMap()->getPositionX() + screenSize.width / 2.0f;
    float yPos = - mapHandler->getMap()->getPositionY() + screenSize.height / 2.0f;
    CCLog("%f, %f", xPos, yPos);
    */
    
    if(tapped)
    {
        tapped = false;
        return;
    }
    
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->miniDragon != NULL)
    {
        if(TutorialManager::getThis()->clickable && TutorialManager::getThis()->miniDragon->clickToNext && !TutorialManager::getThis()->miniDragon->lockClick)
        {
            TutorialManager::getThis()->miniDragon->clickNext();
            TutorialManager::getThis()->clickable = false;
        }
        else
        {
            TutorialManager::getThis()->miniDragon->showAllSpeech();
        }
    }
    
    // then check the tutorial manager
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->narrator != NULL)
    {
        bool skip = false;
        
        if(TutorialManager::getThis()->inDisplay)
        {
            if(TutorialManager::getThis()->inText)
            {
                TutorialManager::getThis()->narrator->finishDisplay();
            }
            else
            {
                if(TutorialManager::getThis()->narrator->clickToNext)
                {
                    TutorialManager::getThis()->narrator->display();
                }
            }
        }
        
        if(TutorialManager::getThis()->lockMap)
        {
            skip = true;
        }
        
        if(skip)
        {
            return;
        }
    }
    
    // the first priority for clicking on the screen is to check whether it is for the senario stage
    if(Senario::getThis()->active)
    {
        if(Senario::getThis()->inOption)
        {
            return;
        }
        
        if(Senario::getThis()->skipButton != NULL)
        {
            if(Senario::getThis()->skipButton->boundingBox().containsPoint(touchLoc))
            {
                Senario::getThis()->nextButtonPressed(true);
            }
            else
            {
                if(((Slide*)Senario::getThis()->slidesList->objectAtIndex(Senario::getThis()->curSlide))->clickToNext)
                {
                    Senario::getThis()->nextButtonPressed(false);
                }
            }
        }
        
        return;
    }
    // system menu
    else if(SystemMenu::getThis() != NULL && !(TutorialManager::getThis()->lockSystemButton))
    {
        if(!hasBeenDragged)
        {
            if(SystemMenu::getThis()->systemMenu_background->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
            else
            {
                SystemMenu::getThis()->scheduleHideSystemMenu();
            }
        }
    }
    else
    // the second priority for clicking on the screen is to check whether it is for the GameHUD layer;
    {
        if(!hasBeenDragged)
        {
            bool skip = false;
            // check whether the clicking is on the objective button (GameHUD)
            if(GameHUD::getThis()->objectiveButton != NULL && !(TutorialManager::getThis()->lockObjectiveButton))
            {
                if(GameHUD::getThis()->objectiveButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->clickObjectiveButton();
                    if(GameHUD::getThis()->showObjectiveNotification)
                    {
                        GameHUD::getThis()->scheduleHideNewObjectiveNotification();
                    }
                    skip = true;
                }
            }
            
            // check whether the clicking is on the objective menu (GameHUD)
            if(GameHUD::getThis()->objectiveMenu != NULL && !(TutorialManager::getThis()->lockObjectiveButton))
            {
                if(GameHUD::getThis()->objectiveMenu->boundingBox().containsPoint(touchLoc))
                {
                    skip = true;
                }
            }
            
            // check whether the clicking is on the objectiveNotification
            if(GameHUD::getThis()->objectiveNotificationLabel != NULL && GameHUD::getThis()->showObjectiveNotification)
            {
                if(GameHUD::getThis()->objectiveNotificationLabel->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->clickObjectiveButton();
                    GameHUD::getThis()->scheduleHideNewObjectiveNotification();
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->peaceButton != NULL && GameHUD::getThis()->warButton != NULL)
            {
                if(GameHUD::getThis()->peaceButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->banditsAttack();
                    skip = true;
                }
                else if(GameHUD::getThis()->warButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->banditsAttack();
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->stickHappinessButton != NULL && GameHUD::getThis()->resumeHappinessButton != NULL)
            {
                if(GameHUD::getThis()->stickHappinessButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->stickGameHappiness();
                    skip = true;
                }
                else if(GameHUD::getThis()->resumeHappinessButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->stickGameHappiness();
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->pauseButton != NULL && GameHUD::getThis()->resumeButton != NULL)
            {
                if(GameHUD::getThis()->pauseButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->pauseGame();
                    skip = true;
                }
                else if(GameHUD::getThis()->resumeButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->pauseGame();
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->scoreButton != NULL)
            {
                if(GameHUD::getThis()->scoreButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->clickScoreButton();
                    skip = true;
                }
            }
            
            if(ScoreMenu::getThis() != NULL && ScoreMenu::getThis()->background != NULL)
            {
                if(ScoreMenu::getThis()->background->boundingBox().containsPoint(touchLoc))
                {
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->systemButton != NULL && !(TutorialManager::getThis()->lockSystemButton))
            {
                if(GameHUD::getThis()->systemButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->clickSystemButton();
                    skip = true;
                }
            }
            
            //click the build button in the bottom right corner to open the build scroll if it is not disable in the tutorial mode
            if(GameHUD::getThis()->buildButton != NULL && !(TutorialManager::getThis()->lockBuildButton))
            {
                //the click point must locate in the actual build button
                if(GameHUD::getThis()->buildButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->clickBuildButton();
                    //do not trigger map interaction. (return before that stage)
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->buildScroll != NULL && GameHUD::getThis()->buildScroll->buildMenu != NULL)
            {
                if(GameHUD::getThis()->buildScroll->buildMenu->boundingBox().containsPoint(touchLoc))
                {
                    skip = true;
                }
                else
                {
                    if(!TutorialManager::getThis()->lockBuildScroll && !TutorialManager::getThis()->lockScroll)
                    {
                        GameHUD::getThis()->buildScroll->scheduleScrollOut();
                    }
                }
            }
            
            if(GameHUD::getThis()->timeMenu != NULL && !(TutorialManager::getThis()->lockTimeGroup))
            {
                if(GameHUD::getThis()->timeMenu->boundingBox().containsPoint(touchLoc))
                {
                    if(GameHUD::getThis()->scrolled_in)
                    {
                        GameHUD::getThis()->scheduleScrollOut();
                    } else {
                        GameHUD::getThis()->scheduleScrollIn();
                    }
                    skip = true;
                }
                else
                {
                    GameHUD::getThis()->scheduleScrollOut();
                }
            }
            
            if(BuildingInfoMenu::getThis() != NULL)
            {
                if(BuildingInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
                {
                    // check whether has clicked the close button
                    if(BuildingInfoMenu::getThis()->buttonClose->boundingBox().containsPoint(touchLoc))
                    {
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && !TutorialManager::getThis()->lockButtonClose))
                        {
                            BuildingInfoMenu::getThis()->closeMenu(true);
                        }
                    }
                    else if(BuildingInfoMenu::getThis()->selectWorkerButton->boundingBox().containsPoint(touchLoc) && BuildingInfoMenu::getThis()->selectWorkerButton->isVisible())
                    {
                        CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage("allocatebtn.png");
                        BuildingInfoMenu::getThis()->selectWorkerButton->setTexture(tex);
                        BuildingInfoMenu::getThis()->selectPop();
                    }
                    else if(BuildingInfoMenu::getThis()->upgradeButton->boundingBox().containsPoint(touchLoc) && BuildingInfoMenu::getThis()->upgradeButton->isVisible())
                    {
                        BuildingInfoMenu::getThis()->upgrade();
                    }
                    else if(BuildingInfoMenu::getThis()->cancelUpgradeButton->boundingBox().containsPoint(touchLoc) && BuildingInfoMenu::getThis()->cancelUpgradeButton->isVisible())
                    {
                        BuildingInfoMenu::getThis()->upgrade();
                    }
                    
                    skip = true;
                }
                else
                {
                    if(!TutorialManager::getThis()->lockScroll)
                    {
                        PopupMenu::closeAllPopupMenu();
                    }
                    skip = true;
                }
            }
            
            if(SelectPopulation::getThis() != NULL && SelectPopulation::getThis()->spriteBackground != NULL)
            {
                if(SelectPopulation::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
                {
                    // check whether has clicked the close button
                    if(SelectPopulation::getThis()->buttonClose->boundingBox().containsPoint(touchLoc))
                    {
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && !TutorialManager::getThis()->lockButtonClose))
                        {
                            SelectPopulation::getThis()->closeMenu(true);
                        }
                    }
                    else if(SelectPopulation::getThis()->buttonOk->boundingBox().containsPoint(touchLoc) && SelectPopulation::getThis()->buttonOk->isVisible())
                    {
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && !TutorialManager::getThis()->lockButtonOk))
                        {
                            if(TutorialManager::getThis()->active && (TutorialManager::getThis()->teachBuildHouse) && TutorialManager::getThis()->miniDragon != NULL)
                            {
                                TutorialManager::getThis()->miniDragon->clickNext();
                            }
                            SelectPopulation::getThis()->performTask();
                        }
                    }
                    else if(SelectPopulation::getThis()->buttonCancel->boundingBox().containsPoint(touchLoc))
                    {
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && !TutorialManager::getThis()->lockButtonCancel))
                        {
                            SelectPopulation::getThis()->cancelTask();
                        }
                    }
                    
                    skip = true;
                }
                else
                {
                    if(!TutorialManager::getThis()->lockScroll)
                    {
                        PopupMenu::closeAllPopupMenu();
                    }
                    skip = true;
                }
            }
            
            if(SpriteInfoMenu::getThis() != NULL && SpriteInfoMenu::getThis()->spriteBackground != NULL)
            {
                if(SpriteInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
                {
                    // check whether has clicked the close button
                    if(SpriteInfoMenu::getThis()->buttonClose->boundingBox().containsPoint(touchLoc))
                    {
                        SpriteInfoMenu::getThis()->closeMenu(true);
                    }
                    skip = true;
                }
                else
                {
                    if(!TutorialManager::getThis()->lockScroll)
                    {
                        PopupMenu::closeAllPopupMenu();
                    }
                    skip = true;
                }
            }
            
            // check the info label
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->moneyIcon != NULL && GameHUD::getThis()->moneyBackground != NULL)
            {
                if(GameHUD::getThis()->moneyIcon->boundingBox().containsPoint(touchLoc) || GameHUD::getThis()->moneyBackground->boundingBox().containsPoint(touchLoc))
                {
                    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockGoldLabel)
                    {
                        
                    }
                    else
                    {
                        GameHUD::getThis()->clickMoneyLabel();
                    }
                    skip = true;
                }
            }
            
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->foodIcon != NULL && GameHUD::getThis()->foodBackground != NULL)
            {
                if(GameHUD::getThis()->foodIcon->boundingBox().containsPoint(touchLoc) || GameHUD::getThis()->foodBackground->boundingBox().containsPoint(touchLoc))
                {
                    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockFoodLabel)
                    {
                        
                    }
                    else
                    {
                        GameHUD::getThis()->clickFoodLabel();
                    }
                    skip = true;
                }
            }
            
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->populationIcon != NULL && GameHUD::getThis()->populationBackground != NULL)
            {
                if(GameHUD::getThis()->populationIcon->boundingBox().containsPoint(touchLoc) || GameHUD::getThis()->populationBackground->boundingBox().containsPoint(touchLoc))
                {
                    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockPopulationLabel)
                    {
                        
                    }
                    else
                    {
                        GameHUD::getThis()->clickPopulationLabel();
                    }
                    skip = true;
                }
            }
            
            if(skip)
            {
                return;
            }
        }
    }
    
    if(TutorialManager::getThis()->lockScroll)
    {
        return;
    }
    
    hasBeenDragged = false;
    
    //If dragged screen, don't count it as a tap
    if (!isThisTapCounted) {
        isThisTapCounted = true;
        
        if ((touches->count() == 1 && !hasBeenDragged) && !isInDeccelerating)
        {
            this->schedule(schedule_selector(GameScene::decceleratingDragging));
            isInDeccelerating = true;
        }
        else if(touches->count() == 0)
        {
            isInDeccelerating = false;
            hasBeenDragged = false;
        }
        
        return;
    }
    
    // it's a tap on the screen of the touch device
    int tapMode = GameHUD::getThis()->getTapMode();
    
    CCPoint tilePos = mapHandler->tilePosFromTouchLocation(touchLoc);
    
    if (mapHandler->isTilePosWithinBounds(tilePos))
    {
        switch (tapMode)
        {
            //Buildpath second tap, modify preview path, or confirm buildpath.
            case 5:
            {
                if (tilePos.x == lastPathPosPreview.x &&
                    tilePos.y == lastPathPosPreview.y)
                {
                    // confirm
                    mapHandler->UnPathPreview();
                    mapHandler->PathLine(firstPathPosPreview, lastPathPosPreview);
                    GameHUD::getThis()->buyBuilding(buildPathDistance * 10);
                    
                    SoundtrackManager::PlaySFX("construction.mp3");
                    
                    GameHUD::getThis()->closeAllMenuAndResetTapMode();
                    
                    if(TutorialManager::getThis()->active && (TutorialManager::getThis()->miniDragon->connectHouse || TutorialManager::getThis()->miniDragon->connectGranary || TutorialManager::getThis()->miniDragon->connectFarm))
                    {
                        checkTeachBuildRoad();
                    }
                }
                else
                {
                    // modify
                    mapHandler->UnPathPreviewLineExtend();
                    lastPathPosPreview = mapHandler->PathPreviewLineExtend(touchLoc, 10);
                    
                    // Manhattan dist, as it's always one-directional
                    setBuildPathDistance(1 + abs((lastPathPosPreview.x - firstPathPosPreview.x) +
                                                 (lastPathPosPreview.y - firstPathPosPreview.y)));
                }
                
                
            }
                break;
                
            //Unbuild path to disallow unpath from mixing with building.
            case 4:
            {
                MapTile* selectedTile = mapHandler->getTileAt(tilePos.x, tilePos.y);
                
                if (selectedTile->isPath)
                {
                    mapHandler->UnPath(tilePos);
                    //GameHUD::getThis()->closeAllMenuAndResetTapMode();
                    
                }
                
            }
                break;
                
                
            //Buildpath first tap, show preview path.
            case 3:
            {
                MapTile* selectedTile = mapHandler->getTileAt(tilePos.x, tilePos.y);
                if (!selectedTile->hasBuilding() &&!selectedTile->isOccupied() && !selectedTile->isPath)
                {
                    mapHandler->PathPreview(tilePos);
                    GameHUD::getThis()->setTapMode(5);
                    firstPathPosPreview = tilePos;
                    lastPathPosPreview = tilePos;
                    setBuildPathDistance(1);
                }
            }
                break;
            //Unbuild
            case 2:
            {
                MapTile* selectedTile = mapHandler->getTileAt(tilePos.x, tilePos.y);
                if (selectedTile->hasBuilding())
                {
                    // buildingHandler->buildingsOnMap->removeObject(selectedTile->building);
                    Building* bui = selectedTile->building;
                    if(bui == NULL)
                    {
                        bui = selectedTile->master->building;
                    }
                    
                    if(bui->buildingType == SPECIAL)
                    {
                        return;
                    }
                    
                    CCArray* allSprites = spriteHandler->spritesOnMap;
                    for (int i = 0; i < allSprites->count(); i++)
                    {
                        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                        if(gs->getTargetLocation() == bui)
                        {
                            gs->setTargetLocation(NULL);
                            gs->currAction = IDLE;
                        }
                        
                        if(gs->getJobLocation() == bui)
                        {
                            gs->setJobLocation(NULL);
                            gs->changeToCitizen();
                            gs->currAction = IDLE;
                        }
                        
                        if(gs->getHome() == bui)
                        {
                            gs->setHome(NULL);
                            SpriteClass* sc = new SpriteClass();
                            gs->changeClassTo(sc);
                            gs->currAction = IDLE;
                        }
                    }
                    
                    bui->memberSpriteList->removeAllObjects();
                    
                    mapHandler->UnBuild(tilePos);
                    GameHUD::getThis()->closeAllMenuAndResetTapMode();
                }
             
            }
                break;
                
                //Build 
            case 1:
            {
                Building* newBuilding = (Building*)buildingHandler->selectedBuilding;
                if (newBuilding == NULL)
                {
                    CCLog("Warning, no building selected");
                    break;
                }
                           
                if (tilePos.x == lastTilePosPreview.x &&
                    tilePos.y == lastTilePosPreview.y)
                {
                    //build
                    //allow a building to be built on top of a path.
                    mapHandler->UnPath(tilePos);
                    if (mapHandler->Build(tilePos, newBuilding, false, "", true))
                    {
                        GameHUD::getThis()->closeAllMenuAndResetTapMode();
                        lastTilePosPreview.x = INT_MAX;
                        lastTilePosPreview.y = INT_MAX;
                    }
                }
                else
                {
                    if(TutorialManager::getThis()->active && (TutorialManager::getThis()->teachBuildHouse || TutorialManager::getThis()->teachBuildGranary))
                    {
                        if(mapHandler->currBuildingPreview == NULL)
                        {
                            TutorialManager::getThis()->miniDragon->clickNext();
                        }
                    }
                    
                    mapHandler->UnBuildPreview();

                    if (mapHandler->BuildPreview(tilePos, newBuilding))
                    {
                        lastTilePosPreview = tilePos;
                    }
                    else
                    {
                        lastTilePosPreview.x = INT_MAX;
                        lastTilePosPreview.y = INT_MAX;
                    }
                }
            }
            break;
            
            /* touch sprite function has been disabled! */
            //Check if clicking on building/sprite/tokens
            default:
            {
                if (!banditsAttackHandler->warMode)
                {
                    if (!handleTouchTokens(touchLoc))
                    {
                        // if touched tokens, don't check for sprite and building
                        //if (!handleTouchSprite(touchLoc))
                        //{
                            // if touched sprite, dont check for building
                            if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && !TutorialManager::getThis()->lockPopup))
                            {
                                handleTouchBuilding(touchLoc, tilePos);
                            }
                        //}
                    }
                }
                else
                {
                    //handleTouchSprite(touchLoc);
                }
            }
                break;
        }
    }
}

void GameScene::centerCamera(Building* b, bool instant)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the currentPosition
    float xPos = - mapHandler->getMap()->getPositionX() + screenSize.width / 2.0f;
    float yPos = - mapHandler->getMap()->getPositionY() + screenSize.height / 2.0f;
    
    // get the horizontal and vertical length difference between the screen centre and the target position!
    float xDiff = b->buildingRep->getPositionX() - xPos;
    float yDiff = b->buildingRep->getPositionY() - yPos;
    
    if(instant)
    {
        mapHandler->moveMapBy(-xDiff, -yDiff);
        CCLog("%f, %f", xPos + xDiff, yPos + yDiff);
    }
    else
    {
        targetBuilding = b;
        this->schedule(schedule_selector(GameScene::scrollToCenter), 1.0f/120.0f);
    }
}

void GameScene::scrollToCenter(float dt)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the current position
    float xPos = - mapHandler->getMap()->getPositionX() + screenSize.width / 2.0f;
    float yPos = - mapHandler->getMap()->getPositionY() + screenSize.height / 2.0f;
    
    // get the horizontal and vertical length difference between the screen centre and the target position!
    float xDiff = targetBuilding->buildingRep->getPositionX() - xPos;
    float yDiff = targetBuilding->buildingRep->getPositionY() - yPos;
    
    float xDis = 0;
    float yDis = 0;
    
    if(xDiff == 0 && yDiff == 0)
    {
        this->unschedule(schedule_selector(GameScene::scrollToCenter));
    }
    else if(xDiff == 0)
    {
        xDis = 0;
        if(yDis >= 0)
        {
            yDis = 10;
            if(yPos + yDis >= targetBuilding->buildingRep->getPositionY())
            {
                xDis = 0;
                yDis = yDiff;
                this->unschedule(schedule_selector(GameScene::scrollToCenter));
            }
        }
        else
        {
            yDis = -10;
            if(yPos + yDis <= targetBuilding->buildingRep->getPositionY())
            {
                xDis = 0;
                yDis = yDiff;
                this->unschedule(schedule_selector(GameScene::scrollToCenter));
            }
        }
    }
    else if(yDiff == 0)
    {
        yDis = 0;
        if(xDis >= 0)
        {
            xDis = 10;
            if(xPos + xDis >= targetBuilding->buildingRep->getPositionX())
            {
                xDis = xDiff;
                yDis = 0;
                this->unschedule(schedule_selector(GameScene::scrollToCenter));
            }
        }
        else
        {
            xDis = -10;
            if(xPos + xDis <= targetBuilding->buildingRep->getPositionX())
            {
                xDis = xDiff;
                yDis = 0;
                this->unschedule(schedule_selector(GameScene::scrollToCenter));
            }
        }
    }
    else
    {
        // x^2 + y^2 = 10^2, x/y = xDiff/yDiff => x^2 = 10^2 * xDiff^2 / (xDiff^2 + yDiff^2), y^2 = 10^2 * yDiff^2 / (xDiff^2 + yDiff^2)
        xDis = sqrt(100.0f * xDiff * xDiff / (xDiff * xDiff + yDiff * yDiff));
        
        if(xDiff < 0)
        {
            xDis = -xDis;
        }
        
        yDis = xDis * yDiff / xDiff;
        
        if(xDiff >= 0)
        {
            if(xPos + xDis >= targetBuilding->buildingRep->getPositionX())
            {
                xDis = xDiff;
                yDis = yDiff;
                this->unschedule(schedule_selector(GameScene::scrollToCenter));
            }
        }
        else
        {
            if(xPos + xDis <= targetBuilding->buildingRep->getPositionX())
            {
                xDis = xDiff;
                yDis = yDiff;
                this->unschedule(schedule_selector(GameScene::scrollToCenter));
            }
        }
    }
    
    mapHandler->moveMapBy(-xDis, -yDis);
}

// going to finish deccelerating dragging.
void GameScene::decceleratingDragging(float dt)
{
    if(hasBeenDragged)
    {
        this->unschedule(schedule_selector(GameScene::decceleratingDragging));
        hasBeenDragged = false;
        isInDeccelerating = false;
    }
    
    scrollDistance = ccpMult(scrollDistance, SCROLL_DEACCEL_RATE);
    mapHandler->moveMapBy(scrollDistance.x, scrollDistance.y);
    
    if ((fabsf(scrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         fabsf(scrollDistance.y) <= SCROLL_DEACCEL_DIST))
    {
        this->unschedule(schedule_selector(GameScene::decceleratingDragging));
        isInDeccelerating = false;
        hasBeenDragged = false;
    }

}

/*we can only start adding sprites to the gamescene after init(), so putting buildings dynamically on the map has to be done here. */
void GameScene::FirstRunPopulate()
{
    if (GameManager::getThis()->getLoadedGame())
    {
        CCLog("loading game");
    }
    else
    {
        mapHandler->Populate(buildingHandler->allBuildingLayers);
    }
    return;
    spriteHandler->initialize();

    GameManager::getThis()->initGameData();
    
    if(GameManager::getThis()->getLevel() == 1 && false)
    {
        loadData();
    }
    else
    {
        if (GameManager::getThis()->getLoadedGame())
        {
            CCLOG("GameManager::getLoadedGame is true!");
        }
        else
        {
            CCLOG("GameManager::getLoadedGame is false!");
            CCPoint target = CCPointMake(24,23);
            
            spriteHandler->addSpriteToMap(target, V_REFUGEE);
            
            target.x += 1;
            spriteHandler->addSpriteToMap(target, V_REFUGEE);
            
            target.x += 1;
            spriteHandler->addSpriteToMap(target, V_REFUGEE);
            
            target.x += 1;
            spriteHandler->addSpriteToMap(target, V_REFUGEE);
        }
    }
    
    CCLog("There are %d sprites on the map!", spriteHandler->spritesOnMap->count());
    for (int i = 0; i < spriteHandler->spritesOnMap->count(); ++i)
    {
        GameSprite* sp = (GameSprite*) spriteHandler->spritesOnMap->objectAtIndex(i);
        sp->setAction(IDLE);
        
    }
    
    // set up the animated rain!
    //animatedRain = AnimatedRain::create();
    
     
    //check if tutorial mode is on.
    
    if (GameManager::getThis()->getTutorialMode())
    {
        //tutorialHandler->BeginTutorial();
    }
    else
    {
        //tutorialHandler->SetActive(false);
        if (!testMode)
            GameManager::getThis()->UpdateUnlocks();
        else
            GameManager::getThis()->UnlockAll();
    }
    this->schedule(schedule_selector(GameScene::update), 1.0f/60.0f);
    
    mapHandler->rescaleScrollLimits();
}

void GameScene::update(float time)
{
    if(!GameHUD::getThis()->pause)
    {
        for (int i = 0; i < spriteHandler->spritesOnMap->count(); i++)
        {
            GameSprite* sp = (GameSprite*) spriteHandler->spritesOnMap->objectAtIndex(i);
            sp->updateSprite(time);
        }
        
        for (int i = 0; i < buildingHandler->allBuildingsOnMap->count(); i++)
        {
            Building* b = (Building*) buildingHandler->allBuildingsOnMap->objectAtIndex(i);
            b->updateBuilding(time);
        }
        
        for (int i = 0; i < buildingHandler->allBuildingsGhostOnMap->count(); i++)
        {
            Building* b = (Building*) buildingHandler->allBuildingsGhostOnMap->objectAtIndex(i);
            b->updateBuilding(time);
        }
        
        constructionHandler->update(time);
        GameHUD::getThis()->update(time);
    
        spriteHandler->update(time);
        banditsAttackHandler->update(time);
        
        mapHandler->update(time);
        if(ObjectiveHandler::getThis() != NULL)
        {
            ObjectiveHandler::getThis()->update(time);
        }
        
        clearCacheTime += time;
        if(clearCacheTime >= clearCacheTimeLimit)
        {
            GlobalHelper::clearCache();
            clearCacheTime = 0;
        }
        
        if(GameManager::getThis()->getLevel() == 2)
        {
            float totalScore = 0;
            CCArray* soldiersOnMap = spriteHandler->spritesOnMap;
            for (int i = 0; i < soldiersOnMap->count(); i++)
            {
                GameSprite* gs = (GameSprite*)soldiersOnMap->objectAtIndex(i);
                if(gs->villagerClass == V_SOLDIER)
                {
                    totalScore += 100.0f;
                }
            }
            
            if(mGameGuardTowerScore != totalScore)
            {
                mGameGuardTowerScore = totalScore;
                GameHUD::getThis()->guardTowerScore = totalScore;
                GameHUD::getThis()->guardTowerScoreBar->setValue(((GameHUD::getThis()->guardTowerScore > 1000.0f) ? 1000.0: GameHUD::getThis()->guardTowerScore) / 1000.0);
                
                stringstream ss;
                if(GameHUD::getThis()->guardTowerScore > 1000)
                {
                    ss << "1000";
                }
                else
                {
                    ss << GameHUD::getThis()->guardTowerScore;
                }
                ss << "/1000";
                
                
                GameHUD::getThis()->guardTowerScoreLabel->setString(ss.str().c_str());
                
            }
        }
    }
    
    if(TutorialManager::getThis()->active)
    {
        if(TutorialManager::getThis()->miniDragon != NULL && TutorialManager::getThis()->miniDragon->waitForVillager)
        {
            CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
            for(int i = 0; i < allSprites->count(); i++)
            {
                GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_REFUGEE && gs->villagerClass != V_CLASS_END)
                {
                    TutorialManager::getThis()->miniDragon->waitForVillager = false;
                    TutorialManager::getThis()->miniDragon->clickNext();
                    break;
                }
            }
        }
        
        if(TutorialManager::getThis()->miniDragon != NULL && TutorialManager::getThis()->miniDragon->finalObjective)
        {
            CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
            int temp = 0;
            for(int i = 0; i < allSprites->count(); i++)
            {
                GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_REFUGEE && gs->villagerClass != V_CLASS_END)
                {
                    temp++;
                }
            }
            
            if(temp >= 8)
            {
                TutorialManager::getThis()->miniDragon->clickNext();
            }
        }
    }
    
    /* check for teach fighting */
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachFighting)
    {
        CCArray* allSpritesOnMap = spriteHandler->spritesOnMap;
        bool teachFightingSuccess = true;
        
        for(int i = 0; i < allSpritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) allSpritesOnMap->objectAtIndex(i);
            if(gs->villagerClass == V_BANDIT && !gs->tryEscape)
            {
                teachFightingSuccess = false;
            }
        }
        
        if(teachFightingSuccess)
        {
            if(TutorialManager::getThis()->miniDragon != NULL)
            {
                GameHUD::getThis()->banditsAttack();
                TutorialManager::getThis()->miniDragon->clickNext();
            }
        }
    }
    
    
    //GameScene::getThis()->animatedRain->update(time);
}

/*
void GameScene::checkPathValid(float time)
{
    //CCLog("test my bad");
    // check for tutorial build road to connect town hall to house and farm.
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachBuildRoad)
    {
        if(teachBuildRoadCheckTime >= 1.0f)
        {
            teachBuildRoadCheckTime = 0;
            checkTeachBuildRoad();
        }
        else
        {
            teachBuildRoadCheckTime += time;
        }
    }
}
*/

void GameScene::checkTeachBuildRoad()
{
    Building* house = NULL;
    Building* granary = NULL;
    Building* farm = NULL;
    Building* townHall = (Building*) buildingHandler->specialOnMap->objectAtIndex(0);
    
    CCPoint housePos = CCPointZero;
    CCPoint granaryPos = CCPointZero;
    CCPoint farmPos = CCPointZero;
    CCPoint townHallPos = CCPointZero;
    
    if(TutorialManager::getThis()->miniDragon->connectHouse)
    {
        house = (Building*) buildingHandler->housingOnMap->objectAtIndex(0);
        housePos = house->getWorldPosition();
        housePos = GameScene::getThis()->mapHandler->tilePosFromLocation(housePos);
    }
    else if(TutorialManager::getThis()->miniDragon->connectGranary)
    {
        granary = (Building*) buildingHandler->granaryOnMap->objectAtIndex(0);
        granaryPos = granary->getWorldPosition();
        granaryPos = GameScene::getThis()->mapHandler->tilePosFromLocation(granaryPos);
    }
    else if(TutorialManager::getThis()->miniDragon->connectFarm)
    {
        farm = (Building*) buildingHandler->amenityGhostOnMap->objectAtIndex(0);
        farmPos = farm->getWorldPosition();
        farmPos = GameScene::getThis()->mapHandler->tilePosFromLocation(farmPos);
    }
    
    townHallPos = townHall->getWorldPosition();
    townHallPos = GameScene::getThis()->mapHandler->tilePosFromLocation(townHallPos);
    
    
    Building* granGhost = NULL;
    CCPoint granGPos = CCPointZero;
    if(TutorialManager::getThis()->miniDragon->notFirst)
    {
        granGhost = (Building*) buildingHandler->granaryGhostOnMap->objectAtIndex(0);
        granGPos = granGhost->getWorldPosition();
        granGPos = GameScene::getThis()->mapHandler->tilePosFromLocation(granGPos);
    }
    
    PathFinder *p = new PathFinder();
    p->setDestination(granaryPos);
    p->setSource(townHallPos);
    
    bool valid = true;
    
    if(TutorialManager::getThis()->miniDragon->connectHouse)
    {
        valid = p->hasPath(&townHallPos, &housePos);
    }
    else if(TutorialManager::getThis()->miniDragon->connectGranary)
    {
        valid = p->hasPath(&townHallPos, &granaryPos);
    }
    else if(TutorialManager::getThis()->miniDragon->connectFarm)
    {
        valid = p->hasPath(&townHallPos, &farmPos);
    }
    else
    {
        valid = (p->hasPath(&townHallPos, &granGPos));
    }
    
    if(valid)
    {
        TutorialManager::getThis()->miniDragon->clickNext();
        TutorialManager::getThis()->miniDragon->notFirst = false;
        
        GameHUD::getThis()->setTapMode(0);
    }
    
    delete p;
}
                         
void GameScene::lostGame(cocos2d::CCObject *psender)
{
    GameManager::getThis()->firstPlay = false;
    CCDirector::sharedDirector()->popScene();
    CCLog("here!");
}

bool GameScene::handleTouchTokens(CCPoint touchLoc)
{
    CCArray* allTokens = spriteHandler->tokensOnMap;
    if(allTokens == NULL)
    {
        return false;
    }
    
    if (allTokens->count() == 0)
    {
        return false;
    }
    
    for (int i = 0; i < allTokens->count(); i++)
    {
        ReputationOrb* ob = (ReputationOrb*) allTokens->objectAtIndex(i);
        if (ob == NULL)
        {
            continue;
        }
        
        if (!ob->getSprite()->isVisible())
        {
            continue;
        }
        
        if (ob->getSprite()->boundingBox().containsPoint(mapHandler->pointOnMapFromTouchLocation(touchLoc)))
        {
            if(ob->orbStatus != EXIST)
            {
                return false;
            }
            
            ob->collect(touchLoc);
            
            GameHUD::getThis()->scheduleAddReputation(5);
            return true;
        }
    }
    return false;
}

bool GameScene::handleTouchSprite(CCPoint touchLoc)
{
    CCArray* gameSprites = spriteHandler->spritesOnMap;
    if(gameSprites == NULL){
        return false;
    }
    if (gameSprites->count() == 0) return false;
    for (int i = 0; i < gameSprites->count(); i++)
    {
        GameSprite* gameSprite = static_cast<GameSprite*>(gameSprites->objectAtIndex(i));
        if (gameSprite == NULL)
            continue;
        
        // Skip if it is not visible (inside buildings)
        if (!gameSprite->spriteRep->isVisible())
            continue;
        
        if (gameSprite->spriteRep->boundingBox().containsPoint(mapHandler->pointOnMapFromTouchLocation(touchLoc)))
        {
            if (!banditsAttackHandler->warMode)
            {
                PopupMenu::backupCurrentPopupMenu();
                SpriteInfoMenu* spriteInfoMenu = SpriteInfoMenu::create(gameSprite);
                spriteInfoMenu->useAsTopmostPopupMenu();
            }
            else
            {
                // in war mode which means bandits attack the village, disable all the popup windows. The only task for player is to tap the bandits to let bandits lose endrance. When endurance is less than 0, bandits will escape.
                if (gameSprite->villagerClass == V_BANDIT)
                {
                    gameSprite->getPossessions()->current_endurance -= 200;
                    if (gameSprite->getPossessions()->current_endurance <= 0)
                    {
                        gameSprite->getPossessions()->current_endurance = 0;
                        gameSprite->tryEscape = true;
                        gameSprite->escape();
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool GameScene::handleTouchBuilding(CCPoint touchLoc, CCPoint tilePos)
{
    MapTile* selectedTile = mapHandler->getTileAt(tilePos.x, tilePos.y);
    if (selectedTile == NULL) return false;
    // Firstly, check by tile
    if (selectedTile->hasBuilding())
    {
        // Check if it is multitile building
        if (selectedTile->master)
            selectedTile = selectedTile->master;
        
        if (selectedTile->building && selectedTile->building->buildingType != DECORATION && selectedTile->building->buildingType != RIVER)
        {
            if(selectedTile->building->isUnderConstruction())
            {
                if((TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachBuildHouse && selectedTile->building->buildingType == HOUSING) || TutorialManager::getThis()->freeBuilding)) || !TutorialManager::getThis()->active)
                {
                    PopupMenu::backupCurrentPopupMenu();
                    
                    if(selectedTile->building->isCurrentConstructing)
                    {
                        BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(selectedTile->building);
                        buildingInfoMenu->useAsTopmostPopupMenu();
                    }
                    else
                    {
                        SelectPopulation* selectPopulation = SelectPopulation::create(selectedTile->building);
                        selectPopulation->useAsTopmostPopupMenu();
                        
                        if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachBuildHouse)
                        {
                            TutorialManager::getThis()->miniDragon->clickNext();
                            selectPopulation->setZOrder(35);
                            TutorialManager::getThis()->lockPopup = true;
                        }
                    }
                }
            }
            else
            {
                if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachFarming && selectedTile->building->buildingType == AMENITY) || TutorialManager::getThis()->freeBuilding)))
                {
                    PopupMenu::backupCurrentPopupMenu();
                    BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(selectedTile->building);//new BuildingInfoMenu(selectedTile->building);
                    buildingInfoMenu->useAsTopmostPopupMenu();
                }
            }
            
            centerCamera(selectedTile->building, false);
            
            return true;
        }
    }
    else
    {
        // Checks (n) number of tiles southwards to see if it is occupied.
        // If so, check if the buildingRep is touched.
        const int numberOfTilesToCheck = 6;
        CCPoint touchWorldLoc = mapHandler->pointOnMapFromTouchLocation(touchLoc);
        
        bool isRightwards = touchWorldLoc.x >= mapHandler->locationFromTilePos(&tilePos).x ? true : false;
        
        for (int i = 0; i < numberOfTilesToCheck; i++)
        {
            if ((i % 2 == 0) ^ isRightwards)    // Bitwise XOR
                tilePos.y++;
            else
                tilePos.x++;
            
            selectedTile = mapHandler->getTileAt(tilePos.x, tilePos.y);
            if (!selectedTile) continue;
            
            if (selectedTile->hasBuilding())
            {
                // Check if it is multitile building
                if (selectedTile->master)
                    selectedTile = selectedTile->master;
                
                if (selectedTile->building &&
                    selectedTile->building->buildingRep->boundingBox().containsPoint(touchWorldLoc) &&
                    selectedTile->building->buildingType != DECORATION &&
                    selectedTile->building->buildingType != RIVER)
                {
                    if(selectedTile->building->isUnderConstruction())
                    {
                        if((TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachBuildHouse && selectedTile->building->buildingType == HOUSING) || TutorialManager::getThis()->freeBuilding)) || !TutorialManager::getThis()->active)
                        {
                            PopupMenu::backupCurrentPopupMenu();
                            
                            if(selectedTile->building->isCurrentConstructing)
                            {
                                BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(selectedTile->building);
                                buildingInfoMenu->useAsTopmostPopupMenu();
                            }
                            else
                            {
                                SelectPopulation* selectPopulation = SelectPopulation::create(selectedTile->building);
                                selectPopulation->useAsTopmostPopupMenu();
                                
                                if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachBuildHouse)
                                {
                                    TutorialManager::getThis()->miniDragon->clickNext();
                                    selectPopulation->setZOrder(35);
                                    TutorialManager::getThis()->lockPopup = true;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachFarming && selectedTile->building->buildingType == AMENITY) || TutorialManager::getThis()->freeBuilding)))
                        {
                            PopupMenu::backupCurrentPopupMenu();
                            BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(selectedTile->building);//new BuildingInfoMenu(selectedTile->building);
                            buildingInfoMenu->useAsTopmostPopupMenu();
                        }
                    }
                    
                    centerCamera(selectedTile->building, false);
                    
                    return true;
                }
            }
        }
    }
    return false;
}

void GameScene::resetBuildMode()
{
    mapHandler->UnBuildPreview();
    mapHandler->UnPathPreview();
    
    lastTilePosPreview.x = INT_MAX;
    lastTilePosPreview.y = INT_MAX;
    firstPathPosPreview.x = INT_MAX;
    firstPathPosPreview.y = INT_MAX;
    lastPathPosPreview.x = INT_MAX;
    lastPathPosPreview.y = INT_MAX;
    
    setBuildPathDistance(0);
}

void GameScene::setBuildPathDistance(int dist)
{
    buildPathDistance = dist;
    GameHUD::getThis()->updateBuildCostLabel(10, dist);
}

void GameScene::enableLoadingScreen()
{
    loadingScreen->setVisible(true);
    loadingLabel->setVisible(true);
    
    GameScene::getThis()->mapHandler->UnBuildEndGame();
    GameScene::getThis()->unschedule(schedule_selector(GameScene::update));
    
    GlobalHelper::clearCache();
    this->scheduleOnce(schedule_selector(GameScene::goToMainMenu), 0.1f);
}

void GameScene::goToMainMenu()
{
    CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

/*Initialized all packed textures here.
 since they're in the shared texture cache, there's no issue calling them from child objects or layers while In Game. This includes the Tutorials. The cache will work as long as the scene is GameScene, and as long as PurgeSharedTextureCache isn't called.*/

void GameScene::initSharedTextureCache()
{
    CCSpriteBatchNode* backgroundBgNode = CCSpriteBatchNode::create("packedportraits.png");
    this->addChild(backgroundBgNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("packedportraits.plist");
    
    CCSpriteBatchNode* HUDNode = CCSpriteBatchNode::create("GameHUD.png");
    this->addChild(HUDNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameHUD.plist");

    CCSpriteBatchNode* ScrollNode = CCSpriteBatchNode::create("BuildScrollUI.png");
    this->addChild(ScrollNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BuildScrollUI.plist");
}

void GameScene::saveBuildingData()
{
    CCLog("trying to save building data");
    
    std::string username = GameManager::getThis()->username;
    
    CCArray* allBuildingsOnMap = buildingHandler->allBuildingsOnMap;
    
    // total number of buildings
    stringstream ss;
    
    int buildingIndex = -1;
    
    for (int i = 0; i < allBuildingsOnMap->count(); i++)
    {
        Building* bui = (Building*) allBuildingsOnMap->objectAtIndex(i);
        
        if(bui->buildingType != HOUSING && bui->buildingType != AMENITY && bui->buildingType != GRANARY && bui->buildingType != MARKET && bui->buildingType != MILITARY && bui->buildingType != SPECIAL && bui->buildingType != EDUCATION)
        {
            continue;
        }
        
        buildingIndex++;
        
        CCArray* allbuildings = buildingHandler->allBuildings;
        
        int tag = 0;
        
        for (int j = 0; j < allbuildings->count(); j++)
        {
            Building* tempBui = (Building*) allbuildings->objectAtIndex(j);
            if (bui->buildingType == tempBui->buildingType)
            {
                tag = tempBui->ID;
                break;
            }
        }
        
        CCLog("****** the current tag is %d", tag);
        
        // building id
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_id";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), tag);
        
        // building in progress
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_inProgress";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->inProgress);
        
        // building type
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_buildingType";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) bui->buildingType);
        
        // building farm state
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_farmState";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) bui->farmState);
        
        // building population limit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_populationLimit";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->populationLimit);
        
        // building builder limit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_builderLimit";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->builderLimit);
        
        // building task type
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_taskType";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) bui->taskType);
        
        // building target work unit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_tagetWorkUnit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->targetWorkUnit);
        
        // building current storage
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currentStorage";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->currentStorage);
        
        // building storage limit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_storageLimit";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->storageLimit);
        
        // building build unit required
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_buildUnitRequired";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->build_uint_required);
        
        // building build unit current
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_buildUnitCurrent";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->build_uint_current);
        
        // building work unit required
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_workUnitRequired";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->work_unit_required);
        
        // building work unit current
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_workUnitCurrent";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->work_unit_current);
        
        // building recovery rate
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_recoveryRate";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->recovery_rate);
        
        // building food consumption rate
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_foodConsumptionRate";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->food_consumption_rate);
        
        // building width
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_width";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->width);
        
        // building height
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_height";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->height);
        
        // building current level
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currentLevel";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->currentLevel);
        
        // building cost
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_cost";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->buildingCost);
        
        // building memeber sprite lists
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_memberNumber";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->memberSpriteList->count());
        
        CCArray* allSpritesOnMap = spriteHandler->spritesOnMap;
        
        for (int j = 0; j < bui->memberSpriteList->count(); j++)
        {
            GameSprite* gs = (GameSprite*) bui->memberSpriteList->objectAtIndex(j);
            
            int gsIndex = -1;
            
            for (int k = 0; k < allSpritesOnMap->count(); k++)
            {
                GameSprite* temp = (GameSprite*) allSpritesOnMap->objectAtIndex(k);
                
                if(temp == gs)
                {
                    gsIndex = k;
                    break;
                }
            }
            
            ss.str(std::string());
            ss << username << "_building_" << buildingIndex << "_member_" << j;
            CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gsIndex);
        }
        
        // building upgrade unit max
        ss.str(std::string());
        ss << username << "_buidling_" << buildingIndex << "_upgradeUnitMax";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->upgrade_unit_max);
        
        // building current upgrade unit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currentUpgradeUnit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->current_upgrade_unit);
        
        // building is current harvest
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentHarvest";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentHarvest);
        
        // buildnig is current constructing
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentConstructing";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentConstructing);
        
        // building is current working
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentWorking";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentWorking);
        
        // building is current upgrading
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentUpgrading";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentUpgrading);
        
        // building number of jobs
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_numberOfJobs";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->number_of_jobs);
        
        // building cumulativeTimeUpgrading
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_cumulativeTimeUpgrading";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->cumulatedTimeUpgrading);
        
        // building cumulativeTimeResting
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_cumulativeTimeResting";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->cumulatedTimeResting);
        
        CCPoint tPos = bui->getWorldPosition();
        tPos = mapHandler->tilePosFromLocation(tPos);
        
        // building position x
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_positionX";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tPos.x);
        
        // building position y
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_positionY";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tPos.y);
    }
    
    ss.str(std::string());
    ss << username << "_numberOfBuildings";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), buildingIndex + 1);
}

void GameScene::saveSystemData()
{
    CCLog("trying to save system data......");
    
    std::string username = GameManager::getThis()->username;
    
    stringstream ss;
    
    // save the level number
    ss.str(std::string());
    ss << username << "_levelNumber";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameManager::getThis()->getLevel());
    
    // save the current gold
    ss.str(std::string());
    ss << username << "_currentGold";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->money);
    
    // save the target gold
    ss.str(std::string());
    ss << username << "_targetGold";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->targetMoney);
    
    // save the reputation
    ss.str(std::string());
    ss << username << "_currentReputation";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->reputation);
    
    // save the target reputation
    ss.str(std::string());
    ss << username << "_targetReputation";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->targetReputation);
    
    // save the date
    ss.str(std::string());
    ss << username << "_date";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->getNumberOfDays());
    
    // ignore objective first, deal it later!!!
    
    // ignore all the reputation orbs first!!!
}

void GameScene::saveSpritesData()
{
    CCLog("trying to save sprite data");
    
    std::string username = GameManager::getThis()->username;
    
    CCArray* spritesOnMap = spriteHandler->spritesOnMap;
    
    stringstream ss;
    ss << username << "_numberOfGameSprites";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), spritesOnMap->count());
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        
        // sprite displayed name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteDisplayedName";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->spriteDisplayedName);
        
        // sprite name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteName";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->spriteName);
        
        // sprite positions
        CCPoint tilePos = gs->getWorldPosition();
        tilePos = mapHandler->tilePosFromLocation(tilePos);
        
        CCLog("save tilePos is (%f, %f)", tilePos.x, tilePos.y);
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tilePos.x);
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tilePos.y);
        
        // sprite foodCarried
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_foodCarried";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->foodCarried);
        
        // sprite current direction
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentDirection";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->currentDir);
        
        // sprite cumulative time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeTime);
        
        // sprite cumulative time for happiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_happiness";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeTime_happiness);
        
        // sprite cumulative time for attack
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_attack";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeTime_attack);
        
        // sprite is token drop cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isTokenDropCooldown";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->is_token_drop_cooldown);
        
        // sprite token drop cooldown time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tokenDropCooldownTime";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->token_drop_cooldown_time);
        
        // sprite go to town hall
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gotoTownHall";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->goToTownHall);
        
        // sprite has assigned
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_hasAssigned";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->hasAssigned);
        
        // sprite cumulative check time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeCheckTime";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeCheckTime);
        
        // sprite targetHungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetHungry);
        
        // sprite targetEnergy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetEnergy";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetEnergy);
        
        // sprite targetHappiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHappiness";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetHappiness);
        
        // sprite villager class
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_villagerClass";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->villagerClass);
        
        // sprite enermy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_enermy";
        string key = ss.str();
        
        ss.str(std::string());
        if (gs->enermy == NULL)
        {
            ss << "NULL";
        }
        else
        {
            ss << gs->enermy->spriteDisplayedName;
        }
        CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), ss.str());
        
        // sprite pre-enermy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_preEnermy";
        key = ss.str();
        
        ss.str(std::string());
        if (gs->preEnermy == NULL)
        {
            ss << "NULL";
        }
        else
        {
            ss << gs->preEnermy->spriteDisplayedName;
        }
        CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), ss.str());
        
        // sprite nextTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->nextTile.x);
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->nextTile.y);
        
        // sprite currentTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->currentTile.x);
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->currentTile.y);
        
        // sprite target location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetLocation.x);
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetLocation.y);
        
        // sprite stop action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_stopAction";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->stopAction);
        
        // sprite is moving
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isMoving";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isMoving);
        
        // sprite try escape
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tryEscape";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->tryEscape);
        
        // sprite mGameCurrentEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameCurrentEndurance";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->mGameCurrentEndurance);
        
        // sprite mGameMaxEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameMaxEndurance";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->mGameMaxEndurance);
        
        // sprite mGameWarMode
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameWarMode";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->mGameWarMode);
        
        // sprite current money rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentMoneyRob";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->current_money_rob);
        
        // sprite current food rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentFoodRob";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->current_food_rob);
        
        // sprite should stop next square
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldStopNextSquare";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->shouldStopNextSquare);
        
        // sprite wander flag
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_wanderFlag";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->wanderFlag);
        
        // sprite should set visible next frame
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldSetVisibleNextFrame";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->shouldSetVisibleNextFrame);
        
        // sprite job location
        CCArray* allBuildings = buildingHandler->allBuildingsOnMap;
        int index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_jobLocation";
        
        if(gs->getJobLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getJobLocation())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_job";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->job);
        
        // sprite is doing job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isDoingJob";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isDoingJob);
        
        // sprite combat state
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_combatState";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->combatState);
        
        // sprite is leaving next update
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isLeavingNextUpdate";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isLeavingNextUpdate);
        
        // sprite is in building
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInBuilding";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isInBuilding);
        
        // sprite gender
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gender";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->gender);
        
        // sprite spriteclass
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteClass";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->spriteClass);
        
        // sprite movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_movementSpeed";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->movementSpeed);
        
        // sprite last frame action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_lastFrameAction";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->lastFrameAction);
        
        // sprite current action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentAction";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->currAction);
        
        // sprite next action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextAction";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->nextAction);
        
        // sprite future action 1
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction1";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->futureAction1);
        
        // sprite future action 2
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction2";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->futureAction2);
        
        // sprite batch layer index
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_batchLayerIndex";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->batchLayerIndex);
        
        // sprite barHp value
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_barHPValue";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->barHP->getValue());
        
        // sprite is in combat
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInCombat";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isInCombat);
        
        // sprite is in attack action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInAttackAction";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isInAttackAction);
        
        // sprite just move one tile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_justMoveOneTile";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->justMoveOneTile);
        
        // sprite frame Width
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameWidth";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->frameWidth);
        
        // sprite frame Height
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameHeight";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->frameHeight);
        
        // sprite possession happiness rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_happinessRating";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->happinessRating);
        
        // sprite possession current hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentHungry";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->currentHungry);
        
        // sprite possession target hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->targetHungry);
        
        // sprite possession energy rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_energyRating";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->energyRating);
        
        // sprite possession default work rate
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkRate";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_work_rate);
        
        // sprite possession default happiness limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHappinessLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_hapiness_limit);
        
        // sprite possession default work unit per day
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkUnitPerDay";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_work_unit_per_day);
        
        // sprite possession default movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultMovementSpeed";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_movement_speed);
        
        // sprite possession default animated speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultAnimatedSpeed";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_animate_speed);
        
        // sprite possession default hungry limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHungryLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_hungry_limit);
        
        // sprite possession default food carriage limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultFoodCarriageLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_food_carriage_limit);
        
        // sprite possession default energy limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultEnergyLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_energy_limit);
        
        // sprite possession attack cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackCooldown";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->attack_cooldown);
        
        // sprite possession attack power max
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMax";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->attack_power_max);
        
        // sprite possession attack power min
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMin";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->attack_power_min);
        
        // sprite possession current endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentEndurance";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->current_endurance);
        
        // sprite possession max endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_maxEndurance";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->max_endurance);
        
        // sprite possession home location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_homeLocation";
        
        if(gs->getHome() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getHome())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite possession job location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_possessionJobLocation";
        
        if(gs->getJobLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getJobLocation())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite possession target location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation";
        
        if(gs->getTargetLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getTargetLocation())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite possession target home location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHomeLocation";
        
        if(gs->getPossessions()->targetHomeLocation == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getPossessions()->targetHomeLocation)
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
    }
}

// try this save data first for testing purpose
void GameScene::saveData()
{
    saveSystemData();
    saveSpritesData();
    saveBuildingData();
}

void GameScene::loadSpritesData()
{
    CCLog("trying to load sprites data......");
    
    std::string username = GameManager::getThis()->username;
    stringstream ss;
    
    ss.str(std::string());
    ss << username << "_numberOfGameSprites";
    int game_sprites_number = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    for (int i = 0; i < game_sprites_number; i++)
    {
        // sprite villager class
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_villagerClass";
        int vcNum = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        VillagerClass vc = (VillagerClass) vcNum;
        
        // sprite positions
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_x";
        float posX = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_y";
        float posY = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        CCPoint tilePos = ccp(posX, posY);
        
        CCLog("****** sprite tilePos is: (%f, %f)", tilePos.x, tilePos.y);
        
        GameSprite* gameSprite = spriteHandler->getSpriteToMap(tilePos, vc);
        
        // sprite displayed name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteDisplayedName";
        std::string name = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->spriteDisplayedName = name;
        
        // sprite name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteName";
        std::string sname = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->spriteName = sname;
        
        // sprite foodCarried
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_foodCarried";
        int foodC = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->foodCarried = foodC;
        
        // sprite current direction
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentDirection";
        std::string currDir = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->currentDir = currDir;
        
        // sprite cumulative time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime";
        float cumuTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeTime = cumuTime;
        
        // sprite cumulative time for happiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_happiness";
        float cumuHappiness = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeTime_happiness = cumuHappiness;
        
        // sprite cumulative time for attack
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_attack";
        float cumuTimeAttack = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeTime_attack = cumuTimeAttack;
        
        // sprite is token drop cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isTokenDropCooldown";
        bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->is_token_drop_cooldown = flag;
        
        // sprite token drop cooldown time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tokenDropCooldownTime";
        float tokenDropTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->token_drop_cooldown_time = tokenDropTime;
        
        // sprite go to town hall
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gotoTownHall";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->goToTownHall = flag;
        
        // sprite has assigned
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_hasAssigned";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->hasAssigned = flag;
        
        // sprite cumulative check time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeCheckTime";
        float cumuCheckTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeCheckTime = cumuCheckTime;
        
        // sprite targetHungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        float targetHungry = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetHungry = targetHungry;
        
        // sprite targetEnergy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetEnergy";
        float targetEnergy = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetEnergy = targetEnergy;
        
        // sprite targetHappiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHappiness";
        float targethappiness = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetHappiness = targethappiness;
        
        // sprite nextTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_x";
        float value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->nextTile.x = value;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_y";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->nextTile.y = value;
        
        // sprite currentTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_x";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->currentTile.x = value;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_y";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->currentTile.y = value;
        
        // sprite target location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_x";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetLocation.x = value;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_y";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetLocation.y = value;
        
        /*
        // sprite stop action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_stopAction";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->stopAction = flag;
        */
        
        // sprite is moving
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isMoving";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isMoving = flag;
        
        // sprite try escape
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tryEscape";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->tryEscape = flag;
        
        // sprite mGameCurrentEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameCurrentEndurance";
        int val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->mGameCurrentEndurance = val;
        
        // sprite mGameMaxEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameMaxEndurance";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->mGameMaxEndurance = val;
        
        // sprite mGameWarMode
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameWarMode";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->mGameWarMode = flag;
        
        // sprite current money rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentMoneyRob";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->current_money_rob = val;
        
        // sprite current food rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentFoodRob";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->current_food_rob = val;
        
        // sprite should stop next square
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldStopNextSquare";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->shouldStopNextSquare = flag;
        
        // sprite wander flag
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_wanderFlag";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->wanderFlag = flag;
        
        // sprite should set visible next frame
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldSetVisibleNextFrame";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->shouldSetVisibleNextFrame = flag;
        
        /*
        // sprite job location
        CCArray* allBuildings = buildingHandler->allBuildingsOnMap;
        int index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_jobLocation";
        
        if(gs->getJobLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getJobLocation())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        */
        
        // sprite job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_job";
        SpriteJob sj = (SpriteJob) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->job = sj;
        
        // sprite is doing job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isDoingJob";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isDoingJob = flag;
        
        // sprite combat state
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_combatState";
        SpriteCombatState scs = (SpriteCombatState) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->combatState = scs;
        
        // sprite is leaving next update
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isLeavingNextUpdate";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isLeavingNextUpdate = flag;
        
        // sprite is in building
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInBuilding";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isInBuilding = flag;
        
        // sprite gender
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gender";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->gender = (char) val;
        
        // sprite spriteclass
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteClass";
        std::string tempStr = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->spriteClass = tempStr;
        
        // sprite movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_movementSpeed";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->movementSpeed = value;
        
        // sprite last frame action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_lastFrameAction";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->lastFrameAction = (SpriteAction) val;
        
        // sprite current action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentAction";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->currAction = (SpriteAction) val;
        
        // sprite next action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextAction";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->nextAction = (SpriteAction) val;
        
        // sprite future action 1
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction1";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->futureAction1 = (SpriteAction) val;
        
        // sprite future action 2
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction2";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->futureAction2 = (SpriteAction) val;
        
        // sprite batch layer index
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_batchLayerIndex";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->batchLayerIndex = val;
        
        // sprite barHp value
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_barHPValue";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->barHP->setValue(value);
        
        // sprite is in combat
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInCombat";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isInCombat = flag;
        
        // sprite is in attack action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInAttackAction";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isInAttackAction = flag;
        
        // sprite just move one tile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_justMoveOneTile";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->justMoveOneTile = flag;
        
        // sprite frame Width
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameWidth";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->frameWidth = value;
        
        // sprite frame Height
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameHeight";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->frameHeight = value;
        
        // sprite possession happiness rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_happinessRating";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->happinessRating = value;
        
        // sprite possession current hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentHungry";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->currentHungry = value;
        
        // sprite possession target hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->targetHungry = value;
        
        // sprite possession energy rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_energyRating";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->energyRating = value;
        
        // sprite possession default work rate
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkRate";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_work_rate = value;
        
        // sprite possession default happiness limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHappinessLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_hapiness_limit = value;
        
        // sprite possession default work unit per day
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkUnitPerDay";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_work_unit_per_day = value;
        
        // sprite possession default movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultMovementSpeed";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_movement_speed = value;
        
        // sprite possession default animated speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultAnimatedSpeed";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_animate_speed = value;
        
        // sprite possession default hungry limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHungryLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_hungry_limit = value;
        
        // sprite possession default food carriage limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultFoodCarriageLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_food_carriage_limit = value;
        
        // sprite possession default energy limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultEnergyLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_energy_limit = value;
        
        // sprite possession attack cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackCooldown";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->attack_cooldown = value;
        
        // sprite possession attack power max
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMax";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->attack_power_max = value;
        
        // sprite possession attack power min
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMin";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->attack_power_min = value;
        
        // sprite possession current endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentEndurance";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->current_endurance = value;
        
        // sprite possession max endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_maxEndurance";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->max_endurance = value;
        
        /*
        // sprite possession home location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_homeLocation";
        
        if(gs->getHome() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getHome())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite possession job location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_possessionJobLocation";
        
        if(gs->getJobLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getJobLocation())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite possession target location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation";
        
        if(gs->getTargetLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getTargetLocation())
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // sprite possession target home location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHomeLocation";
        
        if(gs->getPossessions()->targetHomeLocation == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getPossessions()->targetHomeLocation)
                {
                    index = j;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        */
    }
}

void GameScene::loadSystemData()
{
    CCLog("trying to load system data......");
    
    std::string username = GameManager::getThis()->username;
    
    stringstream ss;
    
    //  get the level number first
    ss.str(std::string());
    ss << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    // load the current money the user have
    ss.str(std::string());
    ss << username << "_currentGold";
    int currentGold = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->money = currentGold;
    
    // load the target money the user have
    ss.str(std::string());
    ss << username << "_targetGold";
    int targetGold = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->targetMoney = targetGold;
    
    // load the current reputation
    ss.str(std::string());
    ss << username << "_currentReputation";
    int currentReputation = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->reputation = currentReputation;
    
    // load the target reputation
    ss.str(std::string());
    ss << username << "_targetReputation";
    int targetReputation = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->targetReputation = targetReputation;
    
    // load the number of days
    ss.str(std::string());
    ss << username << "_date";
    int numberOfDays = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->setNumberOfDays(numberOfDays);
}

void GameScene::loadBuildingData()
{
    CCLog("trying to load building data......");
    
    std::string username = GameManager::getThis()->username;
    stringstream ss;
    
    // get the number of Buildings
    ss.str(std::string());
    ss << username << "_numberOfBuildings";
    int number_of_buildings = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    // loop to load all the buildings in the database
    for (int i = 0; i < number_of_buildings; i++)
    {
        // load the building id of the current building
        ss.str(std::string());
        ss << username << "_building_" << i << "_id";
        int id = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        // get the source image copy of the building want to load
        Building* buildingToBuy = GameScene::getThis()->buildingHandler->getBuilding(id);
        
        // load the positionX and positionY of the building
        ss.str(std::string());
        ss << username << "_building_" << i << "_positionX";
        float posX = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << username << "_building_" << i << "_positionY";
        float posY = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        CCPoint tilePos = ccp(posX, posY);
        
        // build the building on the map
        Building* bui = mapHandler->BuildOnMap(tilePos, buildingToBuy);
        
        /* buildings are set up, load the stats of them */
        if(bui == NULL)
        {
            continue;
        }
        
        // building id
        bui->ID = id;
        
        // building in progress
        ss.str(std::string());
        ss << username << "_building_" << i << "_inProgress";
        bool inProgress = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->inProgress = inProgress;
        
        // building type
        ss.str(std::string());
        ss << username << "_building_" << i << "_buildingType";
        BuildingCategory bc = (BuildingCategory) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->buildingType = bc;
        
        // building farm state
        ss.str(std::string());
        ss << username << "_building_" << i << "_farmState";
        FarmState fs = (FarmState) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->farmState = fs;
        
        // building population limit
        ss.str(std::string());
        ss << username << "_building_" << i << "_populationLimit";
        int populationLimit = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->populationLimit = populationLimit;
        
        // building builder limit
        ss.str(std::string());
        ss << username << "_building_" << i << "_builderLimit";
        int builderLimit = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->builderLimit = builderLimit;
        
        // building task type
        ss.str(std::string());
        ss << username << "_building_" << i << "_taskType";
        TaskType tt = (TaskType) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->taskType = tt;
        
        // building target work unit
        ss.str(std::string());
        ss << username << "_building_" << i << "_tagetWorkUnit";
        float targetWorkUnit = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->targetWorkUnit = targetWorkUnit;
        
        // building current storage
        ss.str(std::string());
        ss << username << "_building_" << i << "_currentStorage";
        int currentStorage = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->currentStorage = currentStorage;
        
        // building storage limit
        ss.str(std::string());
        ss << username << "_building_" << i << "_storageLimit";
        int storageLimit = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->storageLimit = storageLimit;
        
        // building build unit required
        ss.str(std::string());
        ss << username << "_building_" << i << "_buildUnitRequired";
        int build_unit_required = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->build_uint_required = build_unit_required;
        
        // building build unit current
        ss.str(std::string());
        ss << username << "_building_" << i << "_buildUnitCurrent";
        int build_unit_current = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->build_uint_current = build_unit_current;
        
        // building work unit required
        ss.str(std::string());
        ss << username << "_building_" << i << "_workUnitRequired";
        int work_unit_required = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->work_unit_required = work_unit_required;
        
        // building work unit current
        ss.str(std::string());
        ss << username << "_building_" << i << "_workUnitCurrent";
        float work_unit_current = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->work_unit_current = work_unit_current;
        
        // building recovery rate
        ss.str(std::string());
        ss << username << "_building_" << i << "_recoveryRate";
        int recoveryRate = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->recovery_rate = recoveryRate;
        
        // building food consumption rate
        ss.str(std::string());
        ss << username << "_building_" << i << "_foodConsumptionRate";
        int foodConsumptionRate = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->food_consumption_rate = foodConsumptionRate;
        
        // building width
        ss.str(std::string());
        ss << username << "_building_" << i << "_width";
        int buildingWidth = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->width = buildingWidth;
        
        // building height
        ss.str(std::string());
        ss << username << "_building_" << i << "_height";
        int buildingHeight = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->height = buildingHeight;
        
        // building current level
        ss.str(std::string());
        ss << username << "_building_" << i << "_currentLevel";
        int currentLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->currentLevel = currentLevel;
        
        // building cost
        ss.str(std::string());
        ss << username << "_building_" << i << "_cost";
        int buildingCost = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->buildingCost = buildingCost;
        
        // building upgrade unit max
        ss.str(std::string());
        ss << username << "_buidling_" << i << "_upgradeUnitMax";
        float upgradeUnitMax = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->upgrade_unit_max = upgradeUnitMax;
        
        // building current upgrade unit
        ss.str(std::string());
        ss << username << "_building_" << i << "_currentUpgradeUnit";
        float current_upgrade_unit = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->current_upgrade_unit = current_upgrade_unit;
        
        // building is current harvest
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentHarvest";
        bool isCurrentHarvest = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentHarvest = isCurrentHarvest;
        
        // buildnig is current constructing
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentConstructing";
        bool isCurrentConstructing = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentConstructing = isCurrentConstructing;
        
        // building is current working
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentWorking";
        bool isCurrentWorking = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentWorking = isCurrentWorking;
        
        // building is current upgrading
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentUpgrading";
        bool isCurrentUpgrading = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentUpgrading = isCurrentUpgrading;
        
        // building number of jobs
        ss.str(std::string());
        ss << username << "_building_" << i << "_numberOfJobs";
        int numberOfJobs = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->number_of_jobs = numberOfJobs;
        
        // building cumulativeTimeUpgrading
        ss.str(std::string());
        ss << username << "_building_" << i << "_cumulativeTimeUpgrading";
        float cumulatedTimeUpgrading = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->cumulatedTimeUpgrading = cumulatedTimeUpgrading;
        
        // building cumulativeTimeResting
        ss.str(std::string());
        ss << username << "_building_" << i << "_cumulativeTimeResting";
        float cumulatedTimeResting = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->cumulatedTimeResting = cumulatedTimeResting;
    }
}

void GameScene::loadData()
{
    CCLog("trying to load the game data......");
    // warning: for loading the game data, always load the system data first (before loading the building and sprites data!);
    loadSystemData();
    loadBuildingData();
    loadSpritesData();
    CCLog("loading data finished......");
}