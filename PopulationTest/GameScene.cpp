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
#include "SenarioChooseScene.h"
#include "ScoreMenu.h"
#include "GlobalHelper.h"
#include "RandomEventManager.h"
#include "SanGuoXiaoXueTang.h"
#include "UIButtonControl.h"
#include "GameManagement.h"
#include "NotificationPopup.h"
#include "UserProfile.h"
#include "NanhuaGameStaticAPI.h"

#include <cmath>

GameScene* GameScene::SP;

GameScene::GameScene()
{
    screenCenter = CCNode::create();
    constructGameScene();
}

void GameScene::constructGameScene()
{
    hasBeenDragged = false;
    isInDeccelerating = false;
    scrollDistance = CCPointZero;
    
    tapped = false;
    isInGame = false;
    
    targetBuilding = NULL;
    
    clearCacheTime = 0;
    clearCacheTimeLimit = 10;
    
    autoSaveTimeInterval = 60;
    autoSaveTimeLeft = autoSaveTimeInterval;
    
    randomEventTresholdTimeMax = 240;
    randomEventTresholdTimeMin = 120;
    
    randomEventTresholdTime = 0;
    randomEventCumulativeTime = 0;
    
    isEndingGame = false;
}

GameScene::~GameScene()
{
    GameScene::SP = NULL;
}

CCScene* GameScene::scene()
{
    // 'scene' is an auto release object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    SP = layer;
    
    scene->addChild(layer, 0);
    
    // add MainMenuScene layer as a child to scene
    MainMenuScene* mmslayer = MainMenuScene::getThis();
    
    // add SenarioChooseScene layer as a child to scene
    SenarioChooseScene* scslayer = SenarioChooseScene::getThis();
    
    // add SanGuoXiaoXueTang layer as a child to scene
    SanGuoXiaoXueTang* sanlayer = SanGuoXiaoXueTang::getThis();
    
    // add scenario layer as a child to scene
    Senario* senlayer = Senario::getThis();
    
    // add tutorial layer as a child to scene
    TutorialManager* tutlayer = TutorialManager::getThis();
    
    // add game hud layer as a child to scene
    GameHUD* hudlayer = GameHUD::getThis();
    
    // add game objective handler layer as a child to scene
    ObjectiveHandler* objlayer = ObjectiveHandler::getThis();
    
    // add notification popup layer as a child to scene
    NotificationPopup* notlayer = NotificationPopup::getThis();
    
    // add random event manager layer as a child to scene
    RandomEventManager* ranlayer = RandomEventManager::getThis();
    
    // add system menu layer as a child to scene
    SystemMenu* syslayer = SystemMenu::getThis();
    
    layer->addChild(mmslayer, 10);
    layer->addChild(scslayer, 9);
    layer->addChild(sanlayer, 8);
    layer->addChild(senlayer, 7);
    layer->addChild(syslayer, 6);
    layer->addChild(tutlayer, 5);
    layer->addChild(hudlayer, 4);
    layer->addChild(objlayer, 3);
    layer->addChild(notlayer, 2);
    layer->addChild(ranlayer, 1);
    
    MainMenuScene::getThis()->loadTeacherManagementTextures();
    MainMenuScene::getThis()->loadMainMenuTextures();
    MainMenuScene::getThis()->init();
    GameManager::getThis()->enableMainMenuScene();
    
    SoundtrackManager::PlayBGM("Ishikari Lore.mp3");
    
    return scene;
}

GameScene* GameScene::getThis()
{
    return SP;
}

void GameScene::configSkipData()
{
    // load the data that defines whether the current level will skip the scenario and tutorial, the data is stored in 'GameConfig.h'
    int level = UserProfile::getThis()->gameLevel;
    
    if(level == 0)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_tutorial;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_tutorial;
    }
    else if(level == 1)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_level1;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_level1;
    }
    else if(level == 2)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_level2;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_level2;
    }
    else if(level == 3)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_level3;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_level3;
    }
    else if(level == 4)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_level4;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_level4;
    }
    else if(level == 5)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_level5;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_level5;
    }
    else if(level == 6)
    {
        UserProfile::getThis()->settingsLevel->setLevel0();
        UserProfile::getThis()->systemConfig->skipSenario = UserProfile::getThis()->systemConfig->skipSenario_level6;
        UserProfile::getThis()->systemConfig->skipTutorial = UserProfile::getThis()->systemConfig->skipTutorial_level6;
    }
}

void GameScene::configLevelData()
{
    int level = UserProfile::getThis()->gameLevel;
    
    std::string filename = "senario_h.xml";
    
    if(level == 0)
    {
        filename = "introduction.xml";
        Senario::getThis()->scenarioState = Introduction;
        Senario::getThis()->playSenario(filename.c_str());
    }
    else if(level == 1)
    {
        Senario::getThis()->scenarioState = Scenario1;
        TutorialManager::getThis()->active = false;
        TutorialManager::getThis()->unlockAll();
        ObjectiveHandler::getThis()->playObjective();
    }
    else if(level == 2)
    {
        SanGuoXiaoXueTang::getThis()->theState = Part_2;
        SanGuoXiaoXueTang::getThis()->showUI();
    }
    else if(level == 3)
    {
        filename = "senario_h3.xml";
        Senario::getThis()->scenarioState = Scenario3;
        Senario::getThis()->playSenario(filename.c_str());
        ObjectiveHandler::getThis()->playObjective();
    }
    else if(level == 4)
    {
        SanGuoXiaoXueTang::getThis()->theState = Part_3;
        SanGuoXiaoXueTang::getThis()->showUI();
    }
    else if(level == 5)
    {
        SanGuoXiaoXueTang::getThis()->theState = Part_4;
        SanGuoXiaoXueTang::getThis()->showUI();
    }
    else if(level == 6)
    {
        filename = "senario_h6.xml";
        Senario::getThis()->scenarioState = Scenario6;
        Senario::getThis()->playSenario(filename.c_str());
        ObjectiveHandler::getThis()->playObjective();
    }
}

void GameScene::reSetupLevel(bool newGame)
{
    ObjectiveHandler::getThis()->loadObjective();
    configSkipData();
    if(!newGame)
    {
        // 1 means fixed save
        GameManagement::saveGameToFile(1);
    }
    configLevelData();
}

bool GameScene::init()
{
    //The following are compulsory for all scenes!
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    lastTilePosPreview = ccp(INT_MAX, INT_MAX);
    firstPathPosPreview = ccp(INT_MAX, INT_MAX);
    lastPathPosPreview = ccp(INT_MAX, INT_MAX);

    currScale = 1.0f;
    
    setupScene();
    
    this->schedule(schedule_selector(GameScene::update), 1.0f/60.0f);
    
    return true;
}

void GameScene::setupScene()
{
    MapHandler::getThis()->initTiles("BaseMap.tmx");
    
    if (MapHandler::getThis()->getMap())
    {
        BuildingHandler::getThis()->init(MapHandler::getThis()->getMap());
        
        screenCenter->setAnchorPoint(ccp(0.5, 0.5));
        
        screenCenter->addChild(MapHandler::getThis()->getScaleLayer(), -1);
        
        MapHandler::getThis()->getScaleLayer()->setPosition(CCPointZero);
        
        MapHandler::getThis()->getScaleLayer()->addChild(MapHandler::getThis()->getMap(), 0);
        this->addChild(screenCenter);
        
        MapHandler::getThis()->originateMapToTile();
        
        MapHandler::getThis()->centerMap();
    }
    
    SpriteHandler::getThis()->initialize();
    
    if (!testMode)
    {
        GameManager::getThis()->UpdateUnlocks();
    }
    else
    {
        GameManager::getThis()->UnlockAll();
    }
    
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
    
    MapHandler::getThis()->rescaleScrollLimits();
    MapHandler::getThis()->moveMapBy(0.0f, 0.0f);
}

void GameScene::initOrientationChange()
{
    MapHandler::getThis()->rescaleScrollLimits();
    MapHandler::getThis()->moveMapBy(0.0f, 0.0f);
}

void GameScene::onOrientationChanged()
{
    MapHandler::getThis()->rescaleScrollLimits();
    MapHandler::getThis()->moveMapBy(0.0f, 0.0f);
}

void GameScene::enableTouch()
{
    GameScene::getThis()->setTouchEnabled(true);
}

void GameScene::ccTouchesBegan(CCSet *touches, CCEvent *pEvent)
{
    if(!isInGame)
    {
        return;
    }
    
    if(isEndingGame)
    {
        return;
    }
    
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    if(BuildingInfoMenu::getThis() != NULL)
    {
        if(BuildingInfoMenu::getThis()->selectWorkerButton->boundingBox().containsPoint(touchLoc) && BuildingInfoMenu::getThis()->selectWorkerButton->isVisible())
        {
            CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("allocatebtn-press.png");
            BuildingInfoMenu::getThis()->selectWorkerButton->setTexture(theFrame->getTexture());
            BuildingInfoMenu::getThis()->selectWorkerButton->setTextureRect(theFrame->getRect());
        }
    }
    
    if(RandomEventManager::getThis() != NULL && RandomEventManager::getThis()->background != NULL)
    {
        if(RandomEventManager::getThis()->okButton != NULL && RandomEventManager::getThis()->okButton->boundingBox().containsPoint(touchLoc) && RandomEventManager::getThis()->okButton->isVisible())
        {
            CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("confirm_press.png");
            RandomEventManager::getThis()->okButton->setTexture(theFrame->getTexture());
            RandomEventManager::getThis()->okButton->setTextureRect(theFrame->getRect());
        }
    }
    
    // check the interactions of san guo xiao xue tang
    if(SanGuoXiaoXueTang::getThis() != NULL && SanGuoXiaoXueTang::getThis()->background != NULL)
    {
        if(SanGuoXiaoXueTang::getThis()->background->boundingBox().containsPoint(touchLoc))
        {
            /* section to handle san guo xiao xue tang interactions */
            if(SanGuoXiaoXueTang::getThis()->buttonAnswer1->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer1->boundingBox().containsPoint(touchLoc))
            {
                CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptybtn_press.png");
                SanGuoXiaoXueTang::getThis()->buttonAnswer1->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer1->setTextureRect(theFrame->getRect());
            }
            else if(SanGuoXiaoXueTang::getThis()->buttonAnswer2->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer2->boundingBox().containsPoint(touchLoc))
            {
                CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptybtn_press.png");
                SanGuoXiaoXueTang::getThis()->buttonAnswer2->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer2->setTextureRect(theFrame->getRect());
            }
            else if(SanGuoXiaoXueTang::getThis()->buttonAnswer3->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer3->boundingBox().containsPoint(touchLoc))
            {
                CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptybtn_press.png");
                SanGuoXiaoXueTang::getThis()->buttonAnswer3->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer3->setTextureRect(theFrame->getRect());
            }
            else if(SanGuoXiaoXueTang::getThis()->buttonAnswer4->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer4->boundingBox().containsPoint(touchLoc))
            {
                CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptybtn_press.png");
                SanGuoXiaoXueTang::getThis()->buttonAnswer4->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer4->setTextureRect(theFrame->getRect());
            }
            else if(SanGuoXiaoXueTang::getThis()->buttonNext->isVisible() && SanGuoXiaoXueTang::getThis()->buttonNext->boundingBox().containsPoint(touchLoc))
            {
                CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptybtn_press.png");
                SanGuoXiaoXueTang::getThis()->buttonNext->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonNext->setTextureRect(theFrame->getRect());
            }
        }
    }
}

void GameScene::ccTouchesMoved(CCSet *touches, CCEvent *pEvent){
    if(tapped)
    {
        tapped = false;
    }
    
    if(!isInGame)
    {
        return;
    }
    
    if(isEndingGame)
    {
        return;
    }
    
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
    else if(SystemMenu::getThis() != NULL && SystemMenu::getThis()->isActive && !(TutorialManager::getThis()->lockSystemButton))
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
            if(GameHUD::getThis()->peaceButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->peaceButton->isVisible())
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->warButton != NULL)
        {
            if(GameHUD::getThis()->warButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->warButton->isVisible())
            {
                skip = true;
            }
        }
        
        if(RandomEventManager::getThis()->background != NULL)
        {
            if(RandomEventManager::getThis()->background->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                if(RandomEventManager::getThis()->blackScreen->isVisible())
                {
                    RandomEventManager::getThis()->hideUI();
                }
            }
        }
        
        if(GameHUD::getThis()->stickHappinessButton != NULL)
        {
            if(GameHUD::getThis()->stickHappinessButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->stickHappinessButton->isVisible())
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->resumeHappinessButton != NULL)
        {
            if(GameHUD::getThis()->resumeHappinessButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->resumeHappinessButton->isVisible())
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->pauseButton != NULL)
        {
            if(GameHUD::getThis()->pauseButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->pauseButton->isVisible())
            {
                skip = true;
            }
        }
        
        if(GameHUD::getThis()->resumeButton != NULL)
        {
            if(GameHUD::getThis()->resumeButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->resumeButton->isVisible())
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
        
        // san guo xiao xue tang handler
        if(SanGuoXiaoXueTang::getThis() != NULL && SanGuoXiaoXueTang::getThis()->background != NULL)
        {
            if(SanGuoXiaoXueTang::getThis()->background->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
        }
        
        // check the interactions with the notification popup background
        if(NotificationPopup::getThis() != NULL && NotificationPopup::getThis()->background != NULL)
        {
            if(NotificationPopup::getThis()->background->boundingBox().containsPoint(touchLoc))
            {
                skip = true;
            }
            else
            {
                NotificationPopup::getThis()->clickNotificationButton();
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
    CCTouch* touchOne = NULL;
    CCTouch* touchTwo = NULL;
    
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
        for (it = touches->begin(); it != touches->end(); it++) {
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
        
        // float distanceDelta = currentDistance - previousDistance;
        float oldScale = currScale;
        float factor =  (currentDistance == 0 || previousDistance == 0)? 0.0f: currentDistance/previousDistance;
        currScale= clampf(oldScale * factor, 0.5f, 1.0f);
        
        // CCPoint scaleCenter = ccpMidpoint(touchLocationOne, touchLocationTwo);
        // scaleCenter = mapHandler->pointOnMapFromTouchLocation(scaleCenter);
        
        MapHandler::getThis()->scaleTo(currScale);
    }
    // Map dragging
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
        
        MapHandler::getThis()->moveMapBy(moveX, moveY);
    }
    
    isThisTapCounted = false;
}

void GameScene::ccTouchesEnded(CCSet *touches, CCEvent *pEvent)
{
    // CCLog("it is a tap!");
    if(tapped)
    {
        tapped = false;
        return;
    }
    
    if(!isInGame)
    {
        return;
    }
    
    if(isEndingGame)
    {
        return;
    }
    
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    if(BuildingInfoMenu::getThis() != NULL)
    {
        if(BuildingInfoMenu::getThis()->selectWorkerButton->isVisible())
        {
            CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("allocatebtn.png");
            BuildingInfoMenu::getThis()->selectWorkerButton->setTexture(theFrame->getTexture());
            BuildingInfoMenu::getThis()->selectWorkerButton->setTextureRect(theFrame->getRect());
        }
    }
    
    if(RandomEventManager::getThis() != NULL && RandomEventManager::getThis()->background != NULL)
    {
        if(RandomEventManager::getThis()->okButton != NULL && RandomEventManager::getThis()->okButton->isVisible())
        {
            CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("confirm.png");
            RandomEventManager::getThis()->okButton->setTexture(theFrame->getTexture());
            RandomEventManager::getThis()->okButton->setTextureRect(theFrame->getRect());
        }
    }
    
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
    if(Senario::getThis() != NULL && Senario::getThis()->active)
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
    else if(SystemMenu::getThis() != NULL && SystemMenu::getThis()->isActive && !(TutorialManager::getThis()->lockSystemButton))
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
                if(GameHUD::getThis()->peaceButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->peaceButton->isVisible())
                {
                    NanhuaGameStaticAPI::banditsEndAttack();
                    skip = true;
                }
                else if(GameHUD::getThis()->warButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->warButton->isVisible())
                {
                    NanhuaGameStaticAPI::banditsAttack(1);
                    skip = true;
                }
            }
            
            if(RandomEventManager::getThis()->background != NULL)
            {
                if(RandomEventManager::getThis()->background->boundingBox().containsPoint(touchLoc))
                {
                    // click in the background area, nothing happens.
                    skip = true;
                }
                else
                {
                    if(RandomEventManager::getThis()->blackScreen->isVisible())
                    {
                        RandomEventManager::getThis()->hideUI();
                        skip = true;
                    }
                }
            }
            
            if(GameHUD::getThis()->showRandomEventManagerButton != NULL && GameHUD::getThis()->showRandomEventManagerButton->isVisible())
            {
                /*
                if(GameHUD::getThis()->showRandomEventManagerButton->boundingBox().containsPoint(touchLoc))
                {
                    GameHUD::getThis()->clickShowRandomEventManagerButton();
                    skip = true;
                }
                */
                
                if(GameHUD::getThis()->showRandomEventManagerButton->boundingBox().containsPoint(touchLoc))
                {
                    SanGuoXiaoXueTang::getThis()->clickSanGuoXiaoXueTangButton();
                }
                
                /*
                if(GameHUD::getThis()->showRandomEventManagerButton->boundingBox().containsPoint(touchLoc))
                {
                    NotificationPopup::getThis()->showScenario3Congratulations();
                    skip = true;
                }
                */
            }
            
            if(GameHUD::getThis()->stickHappinessButton != NULL && GameHUD::getThis()->resumeHappinessButton != NULL)
            {
                if(GameHUD::getThis()->stickHappinessButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->stickHappinessButton->isVisible())
                {
                    GameHUD::getThis()->stickGameHappiness();
                    skip = true;
                }
                else if(GameHUD::getThis()->resumeHappinessButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->resumeHappinessButton->isVisible())
                {
                    GameHUD::getThis()->stickGameHappiness();
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->pauseButton != NULL && GameHUD::getThis()->resumeButton != NULL)
            {
                if(GameHUD::getThis()->pauseButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->pauseButton->isVisible())
                {
                    UIButtonControl::clickPauseButton();
                    skip = true;
                }
                else if(GameHUD::getThis()->resumeButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->resumeButton->isVisible())
                {
                    UIButtonControl::clickPauseButton();
                    skip = true;
                }
            }
            
            if(GameHUD::getThis()->scoreButton != NULL)
            {
                if(GameHUD::getThis()->scoreButton->boundingBox().containsPoint(touchLoc) && GameHUD::getThis()->scoreButton->isVisible())
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
                    SystemMenu::getThis()->clickSystemButton();
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
            
            // random event manager okButton
            if(RandomEventManager::getThis() != NULL && RandomEventManager::getThis()->background != NULL)
            {
                if(RandomEventManager::getThis()->okButton != NULL && RandomEventManager::getThis()->okButton->boundingBox().containsPoint(touchLoc) && RandomEventManager::getThis()->okButton->isVisible())
                {
                    RandomEventManager::getThis()->hideUI();
                }
            }
            
            // check the building info menu
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
            
            // check the select population menu
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
            
            // check the sprite info menu
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
            
            // check the info labels - money label
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->moneyIcon != NULL && GameHUD::getThis()->moneyBackground != NULL)
            {
                if(GameHUD::getThis()->moneyIcon->boundingBox().containsPoint(touchLoc) || GameHUD::getThis()->moneyBackground->boundingBox().containsPoint(touchLoc))
                {
                    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockGoldLabel)
                    {
                        
                    }
                    else
                    {
                        // GameHUD::getThis()->clickMoneyLabel();
                    }
                    skip = true;
                }
            }
            
            // check the food label
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->foodIcon != NULL && GameHUD::getThis()->foodBackground != NULL)
            {
                if(GameHUD::getThis()->foodIcon->boundingBox().containsPoint(touchLoc) || GameHUD::getThis()->foodBackground->boundingBox().containsPoint(touchLoc))
                {
                    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockFoodLabel)
                    {
                        
                    }
                    else
                    {
                        // GameHUD::getThis()->clickFoodLabel();
                    }
                    skip = true;
                }
            }
            
            // check population label
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->populationIcon != NULL && GameHUD::getThis()->populationBackground != NULL)
            {
                if(GameHUD::getThis()->populationIcon->boundingBox().containsPoint(touchLoc) || GameHUD::getThis()->populationBackground->boundingBox().containsPoint(touchLoc))
                {
                    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockPopulationLabel)
                    {
                        
                    }
                    else
                    {
                        // GameHUD::getThis()->clickPopulationLabel();
                    }
                    skip = true;
                }
            }
            
            // check the interactions of san guo xiao xue tang
            if(SanGuoXiaoXueTang::getThis() != NULL && SanGuoXiaoXueTang::getThis()->background != NULL)
            {
                CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptybtn.png");
                SanGuoXiaoXueTang::getThis()->buttonAnswer1->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer1->setTextureRect(theFrame->getRect());
                
                SanGuoXiaoXueTang::getThis()->buttonAnswer2->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer2->setTextureRect(theFrame->getRect());
                
                SanGuoXiaoXueTang::getThis()->buttonAnswer3->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer3->setTextureRect(theFrame->getRect());
                
                SanGuoXiaoXueTang::getThis()->buttonAnswer4->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonAnswer4->setTextureRect(theFrame->getRect());
                
                SanGuoXiaoXueTang::getThis()->buttonNext->setTexture(theFrame->getTexture());
                SanGuoXiaoXueTang::getThis()->buttonNext->setTextureRect(theFrame->getRect());
                
                if(SanGuoXiaoXueTang::getThis()->background->boundingBox().containsPoint(touchLoc))
                {
                    skip = true;
                    
                    /* section to handle san guo xiao xue tang interactions */
                    if(SanGuoXiaoXueTang::getThis()->buttonAnswer1->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer1->boundingBox().containsPoint(touchLoc))
                    {
                        SanGuoXiaoXueTang::getThis()->onButtonAnswer1Pressed();
                    }
                    else if(SanGuoXiaoXueTang::getThis()->buttonAnswer2->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer2->boundingBox().containsPoint(touchLoc))
                    {
                        SanGuoXiaoXueTang::getThis()->onButtonAnswer2Pressed();
                    }
                    else if(SanGuoXiaoXueTang::getThis()->buttonAnswer3->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer3->boundingBox().containsPoint(touchLoc))
                    {
                        SanGuoXiaoXueTang::getThis()->onButtonAnswer3Pressed();
                    }
                    else if(SanGuoXiaoXueTang::getThis()->buttonAnswer4->isVisible() && SanGuoXiaoXueTang::getThis()->buttonAnswer4->boundingBox().containsPoint(touchLoc))
                    {
                        SanGuoXiaoXueTang::getThis()->onButtonAnswer4Pressed();
                    }
                    else if(SanGuoXiaoXueTang::getThis()->buttonNext->isVisible() && SanGuoXiaoXueTang::getThis()->buttonNext->boundingBox().containsPoint(touchLoc))
                    {
                        SanGuoXiaoXueTang::getThis()->onButtonNextPressed();
                    }
                }
            }
            
            // check the interactions of hospital group
            if(GameHUD::getThis() != NULL && GameHUD::getThis()->buildHospitalButton != NULL)
            {
                if(GameHUD::getThis()->buildHospitalButton->isVisible() && GameHUD::getThis()->buildHospitalButton->boundingBox().containsPoint(touchLoc))
                {
                    skip = true;
                    
                    UIButtonControl::clickBuildHospitalButton();
                }
            }
            
            // check the interactions with the notification popup background
            if(NotificationPopup::getThis() != NULL && NotificationPopup::getThis()->background != NULL)
            {
                if(NotificationPopup::getThis()->background->boundingBox().containsPoint(touchLoc)) //&& !NotificationPopup::getThis()->cancelButton->boundingBox().containsPoint(touchLoc))
                {
                    skip = true;
                }
                else
                {
                    NotificationPopup::getThis()->clickNotificationButton();
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
    
    CCPoint tilePos = MapHandler::getThis()->tilePosFromTouchLocation(touchLoc);
    
    if (MapHandler::getThis()->isTilePosWithinBounds(tilePos))
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
                    MapHandler::getThis()->UnPathPreview();
                    MapHandler::getThis()->PathLine(firstPathPosPreview, lastPathPosPreview);
                    // GameHUD::getThis()->buyBuilding(buildPathDistance * 10);
                    
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
                    MapHandler::getThis()->UnPathPreviewLineExtend();
                    lastPathPosPreview = MapHandler::getThis()->PathPreviewLineExtend(touchLoc, 10);
                    
                    // Manhattan dist, as it's always one-directional
                    setBuildPathDistance(1 + abs((lastPathPosPreview.x - firstPathPosPreview.x) +
                                                 (lastPathPosPreview.y - firstPathPosPreview.y)));
                }
                
                
            }
                break;
                
            //Unbuild path to disallow unpath from mixing with building.
            case 4:
            {
                MapTile* selectedTile = MapHandler::getThis()->getTileAt(tilePos.x, tilePos.y);
                
                if (selectedTile->isPath)
                {
                    MapHandler::getThis()->UnPath(tilePos);
                    //GameHUD::getThis()->closeAllMenuAndResetTapMode();
                    
                }
                
            }
                break;
                
                
            //Buildpath first tap, show preview path.
            case 3:
            {
                MapTile* selectedTile = MapHandler::getThis()->getTileAt(tilePos.x, tilePos.y);
                if (!selectedTile->hasBuilding() &&!selectedTile->isOccupied() && !selectedTile->isPath)
                {
                    MapHandler::getThis()->PathPreview(tilePos);
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
                MapTile* selectedTile = MapHandler::getThis()->getTileAt(tilePos.x, tilePos.y);
                if (selectedTile->hasBuilding())
                {
                    // buildingHandler->buildingsOnMap->removeObject(selectedTile->building);
                    Building* bui = selectedTile->building;
                    if(bui == NULL)
                    {
                        bui = selectedTile->master->building;
                    }
                    
                    // the special building town hall is not allowed to be destroyed!
                    if(bui->buildingType == SPECIAL)
                    {
                        return;
                    }
                    
                    // get the neareast valid path tile
                    CCPoint nearestPathTile = MapHandler::getThis()->getNearestPathTile(bui);
                    
                    // unlink all the villagers that has a relationship with the destroyed building.
                    CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
                    for (int i = 0; i < allSprites->count(); i++)
                    {
                        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                        if(gs->getTargetLocation() == bui)
                        {
                            gs->setTargetLocation(NULL);
                            gs->currAction = IDLE;
                        }
                        
                        // unlink the building with its member sprits who has this building as job location.
                        if(gs->getJobLocation() == bui || gs->getPossessions()->jobLocation == bui)
                        {
                            // CCLog("TEST MY BEST APPLE");
                            gs->setIsDoingJob(false);
                            gs->setJobLocation(NULL);
                            if(gs->villagerClass != V_SOLDIER)
                            {
                                gs->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_CITIZEN));
                            }
                            else
                            {
                                // CCLog("gameSprite name is: %s", gs->spriteName.c_str());
                                gs->changeSpriteTo(GlobalHelper::getSpriteByVillagerClass(gs, V_REFUGEE), GlobalHelper::getSpriteClassByVillagerClass(V_CITIZEN));
                                gs->spriteName = gs->baseSpriteName;
                                gs->changeSpriteRepWhenLoadingGame();
                            }
                            gs->currAction = IDLE;
                            gs->nextAction = IDLE;
                            gs->setIsDoingJob(false);
                            gs->setJob(NONE);
                            gs->path->removeAllObjects();
                            gs->hasAssigned = false;
                        }
                        
                        // if the building is a home, all the member will become a refugee and lose their jobs.
                        if(gs->getHome() == bui)
                        {
                            gs->setHome(NULL);
                            gs->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_REFUGEE));
                            gs->currAction = IDLE;
                            gs->nextAction = IDLE;
                            if(gs->getJobLocation() != NULL)
                            {
                                Building* tempBui = gs->getJobLocation();
                                for(int j = 0; j < tempBui->memberSpriteList->count(); j++)
                                {
                                    GameSprite* gs = (GameSprite*) tempBui->memberSpriteList->objectAtIndex(j);
                                    if(gs->getTargetLocation() == tempBui)
                                    {
                                        gs->setTargetLocation(NULL);
                                        gs->currAction = IDLE;
                                    }
                                    
                                    if(tempBui->buildingType == MILITARY)
                                    {
                                        gs->changeSpriteTo(GlobalHelper::getSpriteByVillagerClass(gs, V_REFUGEE), GlobalHelper::getSpriteClassByVillagerClass(V_REFUGEE));
                                    }
                                    else
                                    {
                                        gs->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_REFUGEE));
                                    }
                                    gs->setJobLocation(NULL);
                                    gs->currAction = IDLE;
                                    gs->nextAction = IDLE;
                                    gs->setIsDoingJob(false);
                                    gs->setJob(NONE);
                                    gs->path->removeAllObjects();
                                    gs->hasAssigned = false;
                                }
                                tempBui->memberSpriteList->removeAllObjects();
                                tempBui->isCurrentWorking = false;
                                
                                gs->setJobLocation(NULL);
                                gs->setJob(NONE);
                            }
                            gs->path->removeAllObjects();
                            gs->hasAssigned = false;
                        }
                        
                        if(MapHandler::getThis()->isSpriteInBuilding(gs, bui) && (nearestPathTile.x >= 0 && nearestPathTile.y >= 0))
                        {
                            CCPoint nearestPathPos = MapHandler::getThis()->locationFromTilePos(&nearestPathTile);
                            gs->spriteRep->setVisible(true);
                            gs->spriteRep->setPosition(nearestPathPos);
                        }
                    }
                    
                    bui->memberSpriteList->removeAllObjects();
                    
                    MapHandler::getThis()->UnBuild(tilePos);
                    GameHUD::getThis()->closeAllMenuAndResetTapMode();
                }
             
            }
                break;
                
                //Build 
            case 1:
            {
                Building* newBuilding = (Building*)BuildingHandler::getThis()->selectedBuilding;
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
                    MapHandler::getThis()->UnPath(tilePos);
                    if (MapHandler::getThis()->Build(tilePos, newBuilding, true, false, "", true))
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
                        if(MapHandler::getThis()->currBuildingPreview == NULL)
                        {
                            TutorialManager::getThis()->miniDragon->clickNext();
                        }
                    }
                    
                    MapHandler::getThis()->UnBuildPreview();

                    if (MapHandler::getThis()->BuildPreview(tilePos, newBuilding))
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
                if (!BanditsAttackHandler::getThis()->warMode)
                {
                    if (!handleTouchTokens(touchLoc))
                    {
                        // if touched sprite, dont check for building
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && !TutorialManager::getThis()->lockPopup))
                        {
                            handleTouchBuilding(touchLoc, tilePos);
                        }
                    }
                }
                else
                {
                    handleTouchBandits(touchLoc);
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
    float xPos = - MapHandler::getThis()->getMap()->getPositionX() + screenSize.width / 2.0f;
    float yPos = - MapHandler::getThis()->getMap()->getPositionY() + screenSize.height / 2.0f;
    
    // get the horizontal and vertical length difference between the screen centre and the target position!
    float xDiff = b->buildingRep->getPositionX() - xPos;
    float yDiff = b->buildingRep->getPositionY() - yPos;
    
    if(instant)
    {
        MapHandler::getThis()->moveMapBy(-xDiff, -yDiff);
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
    if(isEndingGame)
    {
        this->unschedule(schedule_selector(GameScene::scrollToCenter));
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // get the current position
    float xPos = - MapHandler::getThis()->getMap()->getPositionX() + screenSize.width / 2.0f;
    float yPos = - MapHandler::getThis()->getMap()->getPositionY() + screenSize.height / 2.0f;
    
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
    
    MapHandler::getThis()->moveMapBy(-xDis, -yDis);
}

// going to finish deccelerating dragging.
void GameScene::decceleratingDragging(float dt)
{
    if(isEndingGame)
    {
        this->unschedule(schedule_selector(GameScene::decceleratingDragging));
        return;
    }
    
    if(hasBeenDragged)
    {
        this->unschedule(schedule_selector(GameScene::decceleratingDragging));
        hasBeenDragged = false;
        isInDeccelerating = false;
    }
    
    scrollDistance = ccpMult(scrollDistance, SCROLL_DEACCEL_RATE);
    MapHandler::getThis()->moveMapBy(scrollDistance.x, scrollDistance.y);
    
    if ((fabsf(scrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         fabsf(scrollDistance.y) <= SCROLL_DEACCEL_DIST))
    {
        this->unschedule(schedule_selector(GameScene::decceleratingDragging));
        isInDeccelerating = false;
        hasBeenDragged = false;
    }

}

void GameScene::loadLevelBuildings()
{
    int level = UserProfile::getThis()->gameLevel;
    
    MapHandler::getThis()->loadLevelBuildings(level);
}

/*we can only start adding sprites to the gamescene after init(), so putting buildings dynamically on the map has to be done here. */
void GameScene::FirstRunPopulate()
{
    GameManager::getThis()->initGameData();
    
    if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isLoadingGame"))
    {
        MapHandler::getThis()->Populate(BuildingHandler::getThis()->allBuildingLayers);
        loadLevelBuildings();
    }

    if(CCUserDefault::sharedUserDefault()->getBoolForKey("isLoadingGame"))
    {
        MapHandler::getThis()->PopulateForLoadingGame(BuildingHandler::getThis()->allBuildingLayers);
        // load game type: 0 for auto save, 1 for fixed save, 2 for custom save
        int loadGameType = CCUserDefault::sharedUserDefault()->getIntegerForKey("loadingGameType");
        GameManagement::loadData(loadGameType);
        
        for(int i = 0; i < SpriteHandler::getThis()->spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(i);
            // I must restore all the paths from the villagers!
            gs->followPath();
        }
    }
    else
    {
        // new game
        for (int i = 0; i < SpriteHandler::getThis()->spritesOnMap->count(); ++i)
        {
            GameSprite* sp = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(i);
            sp->setAction(IDLE);
        }
    }
    
    // set up the animated rain!
    //animatedRain = AnimatedRain::create();
    
    enableTouch();
    
    isInGame = true;
    
    MapHandler::getThis()->rescaleScrollLimits();
}

void GameScene::startGame()
{
    GameManager::getThis()->enableGameScene();
    reSetupLevel(true);
    GameHUD::getThis()->resetGameHUD();
    FirstRunPopulate();
}

void GameScene::loadGame()
{
    GameManager::getThis()->enableGameScene();
    GameHUD::getThis()->resetGameHUD();
    ObjectiveHandler::getThis()->loadObjective();
    configSkipData();
    FirstRunPopulate();
    
    stringstream ss;
    ss.str(std::string());
    ss << "loadingGameType";
    int type = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    if(type == 2)
    {
        TutorialManager::getThis()->unlockAll();
    }
}

void GameScene::stopGame()
{
    // close all popupMenu
    if(BuildingInfoMenu::getThis() != NULL)
    {
        BuildingInfoMenu::getThis()->closeMenu(false);
    }
    
    if(SpriteInfoMenu::getThis() != NULL)
    {
        SpriteInfoMenu::getThis()->closeMenu(false);
    }
    
    if(SelectPopulation::getThis() != NULL)
    {
        SelectPopulation::getThis()->closeMenu(false);
    }
    
    PopupMenu::clean();
    
    // close random event manager
    if(RandomEventManager::getThis()->active)
    {
        RandomEventManager::getThis()->hideUI();
    }
    
    // clear all fish anims
    CCArray* fishAnimOnMap = SpriteHandler::getThis()->fishAnimOnMap;
    while(fishAnimOnMap->count() > 0)
    {
        CCSprite* sprite = (CCSprite*) fishAnimOnMap->objectAtIndex(0);
        MapHandler::getThis()->getMap()->removeChild(sprite, true);
        fishAnimOnMap->removeObject(sprite);
    }
    
    // remove all buildings on map
    MapHandler::getThis()->UnBuildEndGame();
    MapHandler::getThis()->UnBuildAllPath();
    GameManager::getThis()->EndGameData();
    
    // remove all entries in constructionHandler
    ConstructionHandler::getThis()->clear();
    
    // remove all game sprites on map
    CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
    while (spritesOnMap->count() > 0)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(0);
        SpriteHandler::getThis()->removeSpriteFromMap(gs);
    }
    
    CCArray* tokensOnMap = SpriteHandler::getThis()->tokensOnMap;
    while(tokensOnMap->count() > 0)
    {
        ReputationOrb* ro = (ReputationOrb*) tokensOnMap->objectAtIndex(0);
        SpriteHandler::getThis()->removeTokenFromMap(ro);
    }
    
    // remove all entries in spriteHandler
    SpriteHandler::getThis()->clear();
    
    // remove all entries in buildingHandler
    BuildingHandler::getThis()->clear();
    
    // reset bandits attack handler
    BanditsAttackHandler::getThis()->initialize();
    
    // if it is in tutorial, need to clear tutorial module
    if(TutorialManager::getThis()->active)
    {
        TutorialManager::getThis()->clearSprites();
    }
    
    // reset up GameScene
    this->resetupAllData();
    
    // clear GameHUD when quit game
    GameHUD::getThis()->clearWhenQuitGame();
    
    // reset town hall level
    GameManager::getThis()->town_hall_level = 0;
    
    MainMenuScene::getThis()->loadTeacherManagementTextures();
    MainMenuScene::getThis()->loadMainMenuTextures();
    MainMenuScene::getThis()->init();
    GameManager::getThis()->enableMainMenuScene();
    
    isEndingGame = false;
    isInGame = false;
    
    GlobalHelper::clearCache();
}

void GameScene::update(float time)
{
    if(MainMenuScene::getThis()->isActive())
    {
        MainMenuScene::getThis()->update(time);
        return;
    }
    
    if(SenarioChooseScene::getThis()->isActive())
    {
        return;
    }
    
    if(!isInGame)
    {
        return;
    }
    
    if(isEndingGame)
    {
        return;
    }
    
    if(!GameHUD::getThis()->pause)
    {
        for (int i = 0; i < SpriteHandler::getThis()->spritesOnMap->count(); i++)
        {
            GameSprite* sp = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(i);
            sp->updateSprite(time);
        }
        
        for (int i = 0; i < BuildingHandler::getThis()->allBuildingsOnMap->count(); i++)
        {
            Building* b = (Building*) BuildingHandler::getThis()->allBuildingsOnMap->objectAtIndex(i);
            b->updateBuilding(time);
        }
        
        for (int i = 0; i < BuildingHandler::getThis()->allBuildingsGhostOnMap->count(); i++)
        {
            Building* b = (Building*) BuildingHandler::getThis()->allBuildingsGhostOnMap->objectAtIndex(i);
            b->updateBuilding(time);
        }
        
        ConstructionHandler::getThis()->update(time);
        GameHUD::getThis()->update(time);
    
        SpriteHandler::getThis()->update(time);
        BanditsAttackHandler::getThis()->update(time);
        
        MapHandler::getThis()->update(time);
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
        
        if(!TutorialManager::getThis()->active && !RandomEventManager::getThis()->active)
        {
            // random event;
            if(randomEventTresholdTime < randomEventTresholdTimeMin || randomEventTresholdTime > randomEventTresholdTimeMax)
            {
                randomEventTresholdTime = randomEventTresholdTimeMin + rand() % ((int) (randomEventTresholdTimeMax - randomEventTresholdTimeMin));
            }
            
            randomEventCumulativeTime += time;
            if(randomEventCumulativeTime >= randomEventTresholdTime)
            {
                RandomEventManager::getThis()->showUI();
                randomEventTresholdTime = randomEventTresholdTimeMin + rand() % ((int) (randomEventTresholdTimeMax - randomEventTresholdTimeMin));
                randomEventCumulativeTime = 0;
            }
        }
    }
    
    if(TutorialManager::getThis()->active)
    {
        if(TutorialManager::getThis()->miniDragon != NULL && TutorialManager::getThis()->miniDragon->waitForVillager)
        {
            CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
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
        
        if(TutorialManager::getThis()->miniDragon != NULL && TutorialManager::getThis()->miniDragon->finalObjective && Senario::getThis()->scenarioState == Tutorial)
        {
            CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
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
        CCArray* allSpritesOnMap = SpriteHandler::getThis()->spritesOnMap;
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
                NanhuaGameStaticAPI::banditsEndAttack();
                TutorialManager::getThis()->miniDragon->clickNext();
            }
        }
    }
    
    return;
    
    /* auto save function is currently disabled due to change of main mechanism framework! do it later. */
    
    if(!TutorialManager::getThis()->active)
    {
        checkAutoSave(time);
    }
    
    //GameScene::getThis()->animatedRain->update(time);
}

void GameScene::checkAutoSave(float deltaTime)
{
    autoSaveTimeLeft -= deltaTime;
    
    if(autoSaveTimeLeft <= 0)
    {
        GameManagement::saveGameToFile(0);
        autoSaveTimeLeft = autoSaveTimeInterval;
    }
}

void GameScene::checkTeachBuildRoad()
{
    Building* house = NULL;
    Building* granary = NULL;
    Building* farm = NULL;
    Building* townHall = (Building*) BuildingHandler::getThis()->specialOnMap->objectAtIndex(0);
    
    CCPoint housePos = CCPointZero;
    CCPoint granaryPos = CCPointZero;
    CCPoint farmPos = CCPointZero;
    CCPoint townHallPos = CCPointZero;
    
    if(TutorialManager::getThis()->miniDragon->connectHouse)
    {
        house = (Building*) BuildingHandler::getThis()->housingOnMap->objectAtIndex(0);
        housePos = house->getWorldPosition();
        housePos = MapHandler::getThis()->tilePosFromLocation(housePos);
    }
    else if(TutorialManager::getThis()->miniDragon->connectGranary)
    {
        granary = (Building*) BuildingHandler::getThis()->granaryOnMap->objectAtIndex(0);
        granaryPos = granary->getWorldPosition();
        granaryPos = MapHandler::getThis()->tilePosFromLocation(granaryPos);
    }
    else if(TutorialManager::getThis()->miniDragon->connectFarm)
    {
        farm = (Building*) BuildingHandler::getThis()->amenityGhostOnMap->objectAtIndex(0);
        farmPos = farm->getWorldPosition();
        farmPos = MapHandler::getThis()->tilePosFromLocation(farmPos);
    }
    
    townHallPos = townHall->getWorldPosition();
    townHallPos = MapHandler::getThis()->tilePosFromLocation(townHallPos);
    
    
    Building* granGhost = NULL;
    CCPoint granGPos = CCPointZero;
    if(TutorialManager::getThis()->miniDragon->notFirst)
    {
        granGhost = (Building*) BuildingHandler::getThis()->granaryGhostOnMap->objectAtIndex(0);
        granGPos = granGhost->getWorldPosition();
        granGPos = MapHandler::getThis()->tilePosFromLocation(granGPos);
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

bool GameScene::handleTouchTokens(CCPoint touchLoc)
{
    CCArray* allTokens = SpriteHandler::getThis()->tokensOnMap;
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
        
        if (ob->getSprite()->boundingBox().containsPoint(MapHandler::getThis()->pointOnMapFromTouchLocation(touchLoc)))
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
    CCArray* gameSprites = SpriteHandler::getThis()->spritesOnMap;
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
        
        if (gameSprite->spriteRep->boundingBox().containsPoint(MapHandler::getThis()->pointOnMapFromTouchLocation(touchLoc)))
        {
            if (!BanditsAttackHandler::getThis()->warMode)
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

bool GameScene::handleTouchBandits(CCPoint touchLoc)
{
    CCArray* gameSprites = SpriteHandler::getThis()->spritesOnMap;
    if(gameSprites == NULL){
        return false;
    }
    if (gameSprites->count() == 0) return false;
    
    for (int i = 0; i < gameSprites->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) gameSprites->objectAtIndex(i);
        
        if(gameSprite == NULL)
        {
            continue;
        }
        
        if(!gameSprite->spriteRep->isVisible())
        {
            continue;
        }
        
        if(gameSprite->villagerClass == V_BANDIT && gameSprite->tryEscape)
        {
            continue;
        }
        
        if(gameSprite->spriteRep->boundingBox().containsPoint(MapHandler::getThis()->pointOnMapFromTouchLocation(touchLoc)) && gameSprite->villagerClass == V_BANDIT)
        {
            gameSprite->damaged(50);
            return true;
        }
    }
    return false;
}

bool GameScene::handleTouchBuilding(CCPoint touchLoc, CCPoint tilePos)
{
    MapTile* selectedTile = MapHandler::getThis()->getTileAt(tilePos.x, tilePos.y);
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
                if((TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachBuildHouse && selectedTile->building->buildingType == HOUSING) || TutorialManager::getThis()->freeBuilding || TutorialManager::getThis()->teachBuildGuardTower || TutorialManager::getThis()->teachSoldier)) || !TutorialManager::getThis()->active)
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
                if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachFarming && selectedTile->building->buildingType == AMENITY) || TutorialManager::getThis()->freeBuilding || TutorialManager::getThis()->teachBuildGuardTower || TutorialManager::getThis()->teachSoldier)))
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
        CCPoint touchWorldLoc = MapHandler::getThis()->pointOnMapFromTouchLocation(touchLoc);
        
        bool isRightwards = touchWorldLoc.x >= MapHandler::getThis()->locationFromTilePos(&tilePos).x ? true : false;
        
        for (int i = 0; i < numberOfTilesToCheck; i++)
        {
            if ((i % 2 == 0) ^ isRightwards)    // Bitwise XOR
                tilePos.y++;
            else
                tilePos.x++;
            
            selectedTile = MapHandler::getThis()->getTileAt(tilePos.x, tilePos.y);
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
                        if((TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachBuildHouse && selectedTile->building->buildingType == HOUSING) || TutorialManager::getThis()->freeBuilding || TutorialManager::getThis()->teachBuildGuardTower || TutorialManager::getThis()->teachSoldier)) || !TutorialManager::getThis()->active)
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
                        if(!TutorialManager::getThis()->active || (TutorialManager::getThis()->active && ((TutorialManager::getThis()->teachFarming && selectedTile->building->buildingType == AMENITY) || TutorialManager::getThis()->freeBuilding || TutorialManager::getThis()->teachBuildGuardTower || TutorialManager::getThis()->teachSoldier)))
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
    MapHandler::getThis()->UnBuildPreview();
    MapHandler::getThis()->UnPathPreview();
    
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
    // GameHUD::getThis()->updateBuildCostLabel(10, dist);
}

void GameScene::enableLoadingScreen()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    /* loading screen module */
    loadingScreen = CCSprite::create("loading screen.png");
    loadingScreen->setScale(screenSize.width / loadingScreen->boundingBox().size.width);
    loadingScreen->setAnchorPoint(ccp(0.5, 0.5));
    loadingScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(loadingScreen, 10);
    
    loadingLabel = CCSprite::create("loading.png");
    loadingLabel->setAnchorPoint(ccp(0.5, 0.5));
    loadingLabel->setScale(0.5f);
    loadingLabel->setPosition(ccp(screenSize.width / 2.0f + 20.0f, screenSize.height / 2.0f - 120.0f));
    this->addChild(loadingLabel, 11);
}

void GameScene::goToMainMenu()
{
    GlobalHelper::clearCache();
    CCUserDefault::sharedUserDefault()->setBoolForKey("LoadTextureMainMenu", true);
    // CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

void GameScene::resetupAllData()
{
    currScale = 1.0f;
    MapHandler::getThis()->scaleTo(currScale);
    
    hasBeenDragged = false;
    isInDeccelerating = false;
    scrollDistance = CCPointZero;
    
    tapped = false;
    
    targetBuilding = NULL;
    clearCacheTime = 0;
    
    autoSaveTimeLeft = autoSaveTimeInterval;
    randomEventCumulativeTime = 0;
}

bool GameScene::isActive()
{
    return GameHUD::getThis()->isVisible();
}