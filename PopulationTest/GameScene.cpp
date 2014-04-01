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
#include "AlertBox.h"
#include "SoundtrackManager.h"
#include "GameDefaults.h"
#include "SpriteInfoMenu.h"
#include "SelectPopulation.h"
#include "GameManager.h"
#include "BuildingInfoMenu.h"
#include "BuildScroll.h"
#include "ReputationOrb.h"
#include "SystemMenu.h"

#include <cmath>

GameScene* GameScene::SP;

GameScene::GameScene()
{
    screenCenter = CCNode::create();
    mapHandler = new MapHandler();
    objectiveHandler = new ObjectiveHandler();
    objectiveHandler->loadObjective();
    
    _currentTime = 0;
    _previousTime = 0;
    
    cumulatedTime = 0;
    
    _post_drag_effect = false;
    
    configSettings = new ConfigSettings();
    settingsLevel = new SettingsLevel();
    
    switch (GameManager::getThis()->getLevel())
    {
        case 0:
            settingsLevel->setLevel0();
            break;
        case 1:
            settingsLevel->setLevel0();
            break;
        default:
            break;
    }
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
    
    //delete screenCenter;
    
    GameScene::SP=NULL;
    
    //map = NULL;
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
    std::string filename = "tutorial.xml";
    
    senlayer->playSenario(filename.c_str());
    scene->addChild(senlayer, 1);
    
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
   // this->schedule(schedule_selector( GameScene::move ), 1/60.0f);
    return true;
}



void GameScene::setupScene()
{
    int level = GameManager::getThis()->getLevel();
    if(level == 0)
    {
        mapHandler->initTiles("DemoScene.tmx");
    }
    else
    {
        mapHandler->initTiles("Full_Scene.tmx");
    }
        
        if (mapHandler->getMap())
        {
            
            jobCollection = new JobCollection();
            buildingHandler = new BuildingHandler();
            buildingHandler->init(mapHandler->getMap(), jobCollection);
    
            
        //    gameBG = CCSprite::create("background.jpg");
         //   gameBG->setAnchorPoint(ccp(0,0));
           // CCSize spriteSize = gameBG->getContentSize();
           // CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

          //  gameBG->setScaleX(screenSize.width / spriteSize.width * 10) ;
          //  gameBG->setScaleY(screenSize.height / spriteSize.height * 10) ;
           // CCSize newSize = gameBG->boundingBox().size;
            
       ///     gameBG->setPosition(ccp(mapHandler->getMap()->getPositionX() +spriteSize.width +screenSize.width,
          //                          mapHandler->getMap()->getPositionY() +spriteSize.height/2));
            
         //   mapHandler->getMap()->addChild(gameBG, -3);
            
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
    //mapHandler->rescaleScrollLimits();
    //mapHandler->moveMapBy(0.0f, 0.0f);
}

void GameScene::enableTouch()
{
    this->CCLayer::setTouchEnabled(true);
    GameHUD* hudlayer = GameHUD::create();
    this->addChild(hudlayer, 1);
    this->scheduleOnce(schedule_selector( GameScene::FirstRunPopulate) , 0.1f);
    SoundtrackManager::PlayBGM("Ishikari Lore.mp3");
}

void GameScene::ccTouchesMoved(CCSet *touches, CCEvent *pEvent){
    
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    // the first priority for dragging on the screen is to check whether it is for the senario stage
    if(Senario::getThis()->active)
    {
        return;
    }
    else if(SystemMenu::getThis() != NULL)
    {
        if(SystemMenu::getThis()->systemMenu_background->boundingBox().containsPoint(touchLoc))
        {
            return;
        }
        else
        {
            GameHUD::getThis()->pause = false;
            CCArray* spritesOnMap = spriteHandler->spritesOnMap;
            
            for (int i = 0; i < spritesOnMap->count(); i++)
            {
                GameSprite* sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
                sp->followPath();
            }
            SystemMenu::getThis()->releaseAll();
        }
    }
    // the second priority for dragging on the screen is to check whether it is for the GameHUD layer
    else
    {
        // check whether the dragging is on the buttons (GameHUD)
        if(GameHUD::getThis()->objectiveButton != NULL)
        {
            if(GameHUD::getThis()->objectiveButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->objectiveMenu != NULL)
        {
            if(GameHUD::getThis()->objectiveMenu->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->peaceButton != NULL)
        {
            if(GameHUD::getThis()->peaceButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->warButton != NULL)
        {
            if(GameHUD::getThis()->warButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->stickHappinessButton != NULL)
        {
            if(GameHUD::getThis()->stickHappinessButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->resumeHappinessButton != NULL)
        {
            if(GameHUD::getThis()->resumeHappinessButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->pauseButton != NULL)
        {
            if(GameHUD::getThis()->pauseButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->resumeButton != NULL)
        {
            if(GameHUD::getThis()->resumeButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->systemButton != NULL)
        {
            if(GameHUD::getThis()->systemButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->buildButton != NULL)
        {
            if(GameHUD::getThis()->buildButton->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->buildScroll != NULL && GameHUD::getThis()->buildScroll->buildMenu != NULL)
        {
            if(GameHUD::getThis()->buildScroll->buildMenu->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
            else
            {
                GameHUD::getThis()->buildScroll->scheduleScrollOut();
            }
        }
        
        if(GameHUD::getThis()->timeMenu != NULL)
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
                return;
            }
            else
            {
                BuildingInfoMenu::getThis()->closeMenu();
            }
        }
        
        if(SelectPopulation::getThis() != NULL && SelectPopulation::getThis()->spriteBackground != NULL)
        {
            if(SelectPopulation::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
            else
            {
                SelectPopulation::getThis()->closeMenu();
            }
        }
        
        if(SpriteInfoMenu::getThis() != NULL && SpriteInfoMenu::getThis()->spriteBackground != NULL)
        {
            if(SpriteInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
            else
            {
                SpriteInfoMenu::getThis()->closeMenu();
            }
        }
        
        
        
    }
    
    CCSetIterator it;
    CCTouch* touchOne;
    CCTouch* touchTwo;
    
    // store the starting time of dragging;
    if(_previousTime == 0)
    {
        // stop previous _post_drag_effect
        _speedX = 0;
        _speedY = 0;
        _post_drag_effect = false;
        this->unschedule(schedule_selector(GameScene::postDrag));
        
        _previousTime = CCTime::gettimeofdayCocos2d(&now, NULL);
        _previousTime = (now.tv_sec * 1000 + now.tv_usec / 1000);
        
        CCTouch* touch = (CCTouch*)*touches->begin();
        _previous_pos_x = touch->getLocation().x;
        _previous_pos_y = touch->getLocation().y;
    }
    
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
                float moveX = touch->getLocation().x - touch->getPreviousLocation().x;
                float moveY = touch->getLocation().y - touch->getPreviousLocation().y;
                
                _current_pos_x = touch->getLocation().x;
                _current_pos_y = touch->getLocation().y;
                
                mapHandler->moveMapBy(moveX, moveY);
            }
    
    isThisTapCounted = false;
}

void GameScene::postDrag(float time)
{
    mapHandler->moveMapBy(_speedX * 10, _speedY * 10);
    if(cumulatedTime > 0.05f)
    {
        _speedX *= 0.1f;
        _speedY *= 0.1f;
        cumulatedTime = 0;
    }
    cumulatedTime += time;
    if(fabsf(_speedX) <= 0.001f && fabsf(_speedY) <= 0.001f)
    {
        _speedX = 0;
        _speedY = 0;
        _post_drag_effect = false;
        this->unschedule(schedule_selector(GameScene::postDrag));
    }
}

void GameScene::ccTouchesEnded(CCSet *touches, CCEvent *pEvent)
{
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
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
                Senario::getThis()->nextButtonPressed(false);
            }
        }
        
        return;
    }
    else if(SystemMenu::getThis() != NULL)
    {
        if(SystemMenu::getThis()->systemMenu_background->boundingBox().containsPoint(touchLoc))
        {
            return;
        }
        else
        {
            GameHUD::getThis()->pause = false;
            CCArray* spritesOnMap = spriteHandler->spritesOnMap;
            
            for (int i = 0; i < spritesOnMap->count(); i++)
            {
                GameSprite* sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
                sp->followPath();
            }
            SystemMenu::getThis()->releaseAll();
        }
    }
    else
    // the second priority for clicking on the screen is to check whether it is for the GameHUD layer;
    {
        // check whether the clicking is on the objective button (GameHUD)
        if(GameHUD::getThis()->objectiveButton != NULL)
        {
            if(GameHUD::getThis()->objectiveButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->clickObjectiveButton();
                return;
            }
        }
        
        // check whether the clicking is on the objective menu (GameHUD)
        if(GameHUD::getThis()->objectiveMenu != NULL)
        {
            if(GameHUD::getThis()->objectiveMenu->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
        }
        
        if(GameHUD::getThis()->peaceButton != NULL)
        {
            if(GameHUD::getThis()->peaceButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->banditsAttack();
                return;
            }
        }
        
        if(GameHUD::getThis()->warButton != NULL)
        {
            if(GameHUD::getThis()->warButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->banditsAttack();
                return;
            }
        }
        
        if(GameHUD::getThis()->stickHappinessButton != NULL)
        {
            if(GameHUD::getThis()->stickHappinessButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->stickGameHappiness();
                return;
            }
        }
        
        if(GameHUD::getThis()->resumeHappinessButton != NULL)
        {
            if(GameHUD::getThis()->resumeHappinessButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->stickGameHappiness();
                return;
            }
        }
        
        if(GameHUD::getThis()->pauseButton != NULL)
        {
            if(GameHUD::getThis()->pauseButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->pauseGame();
                return;
            }
        }
        
        if(GameHUD::getThis()->resumeButton != NULL)
        {
            if(GameHUD::getThis()->resumeButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->pauseGame();
                return;
            }
        }
        
        if(GameHUD::getThis()->systemButton != NULL)
        {
            if(GameHUD::getThis()->systemButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->clickSystemButton();
                return;
            }
        }

        if(GameHUD::getThis()->buildButton != NULL)
        {
            if(GameHUD::getThis()->buildButton->boundingBox().containsPoint(touchLoc))
            {
                GameHUD::getThis()->clickBuildButton();
                return;
            }
        }
        
        if(GameHUD::getThis()->buildScroll != NULL && GameHUD::getThis()->buildScroll->buildMenu != NULL)
        {
            if(GameHUD::getThis()->buildScroll->buildMenu->boundingBox().containsPoint(touchLoc))
            {
                return;
            }
            else
            {
                GameHUD::getThis()->buildScroll->scheduleScrollOut();
            }
        }
        
        if(GameHUD::getThis()->timeMenu != NULL)
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
        
        if(BuildingInfoMenu::getThis() != NULL && BuildingInfoMenu::getThis()->spriteBackground != NULL)
        {
            if(BuildingInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                // check whether has clicked the close button
                /*
                if(BuildingInfoMenu::getThis()->buttonClose->boundingBox().containsPoint(touchLoc))
                {
                    BuildingInfoMenu::getThis()->closeMenu();
                }*/ //close button no longer implemented
                return;
            }
            else
            {
                BuildingInfoMenu::getThis()->closeMenu();
            }
        }
        
        if(SelectPopulation::getThis() != NULL && SelectPopulation::getThis()->spriteBackground != NULL)
        {
            if(SelectPopulation::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                // check whether has clicked the close button
                /*
                if(SelectPopulation::getThis()->buttonClose->boundingBox().containsPoint(touchLoc))
                {
                    SelectPopulation::getThis()->closeMenu();
                }*/ //close button no longer implemented
                return;
            }
            else
            {
                SelectPopulation::getThis()->closeMenu();
            }
        }
        
        if(SpriteInfoMenu::getThis() != NULL && SpriteInfoMenu::getThis()->spriteBackground != NULL)
        {
            if(SpriteInfoMenu::getThis()->spriteBackground->boundingBox().containsPoint(touchLoc))
            {
                // check whether has clicked the close button
                /*
                if(SpriteInfoMenu::getThis()->buttonClose->boundingBox().containsPoint(touchLoc))
                {
                    SpriteInfoMenu::getThis()->closeMenu();
                }*/ //close button no longer implemented
                return;
            }
            else
            {
                SpriteInfoMenu::getThis()->closeMenu();
            }
        }
    }
    
    //If dragged screen, don't count it as a tap
    if (!isThisTapCounted) {
        isThisTapCounted = true;
        
        // calculate the draggin speed
        _currentTime = CCTime::gettimeofdayCocos2d(&now, NULL);
        _currentTime = (now.tv_sec * 1000 + now.tv_usec / 1000);
        float time_elapse = _currentTime - _previousTime;
        
        if(time_elapse < 0.1f)
        {
            time_elapse = 0.1f;
        }
        
        _speedX = (_current_pos_x - _previous_pos_x) / time_elapse * 1.5f;
        _speedY = (_current_pos_y - _previous_pos_y) / time_elapse * 1.5f;
        
        if(_speedX > 2)
        {
            _speedX = 2;
        }
        else if(_speedX < -2)
        {
            _speedX = -2;
        }
        
        if(_speedY > 2)
        {
            _speedY = 2;
        }
        else if(_speedY < -2)
        {
            _speedY = -2;
        }

        
        // enable post drag effect
        _post_drag_effect = true;
        this->schedule(schedule_selector( GameScene::postDrag ), 1/120.0f);
        _currentTime = 0;
        _previousTime = 0;
        _previous_pos_x = 0;
        _previous_pos_y = 0;
        _current_pos_x = 0;
        _current_pos_y = 0;
        
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
                    mapHandler->UnPathPreview();
                    mapHandler->PathLine(firstPathPosPreview, lastPathPosPreview);
                    GameHUD::getThis()->buyBuilding(buildPathDistance * 10);
                    
                    GameHUD::getThis()->closeAllMenuAndResetTapMode();
                }
                else
                {
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
                    if (mapHandler->Build(tilePos, newBuilding))
                    {
                        GameHUD::getThis()->closeAllMenuAndResetTapMode();
                        lastTilePosPreview.x = INT_MAX;
                        lastTilePosPreview.y = INT_MAX;
                    }
                }
                else
                {
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
                            handleTouchBuilding(touchLoc, tilePos);
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
    spriteHandler->initialize();

    GameManager::getThis()->initGameData();
    
    if (GameManager::getThis()->getLoadedGame())
    {
        CCLOG("GameManager::getLoadedGame is true!");
    }
    else
    {
        CCLOG("GameManager::getLoadedGame is false!");
        CCPoint target = CCPointMake(25,23);
        
        spriteHandler->addSpriteToMap(target, V_REFUGEE);
        
        target.x += 1;
        spriteHandler->addSpriteToMap(target, V_REFUGEE);
        
        target.x += 1;
        spriteHandler->addSpriteToMap(target, V_REFUGEE);

        
    }
    
    CCLog("There are %d sprites on the map!", spriteHandler->spritesOnMap->count());
    for (int i = 0; i < spriteHandler->spritesOnMap->count(); ++i)
    {
        GameSprite* sp = (GameSprite*) spriteHandler->spritesOnMap->objectAtIndex(i);
        sp->setAction(IDLE);
        
    }
    
     
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
        
        for (int i = 0; i < buildingHandler->specialOnMap->count(); i++)
        {
            Building* b = (Building*) buildingHandler->specialOnMap->objectAtIndex(i);
            b->updateBuilding(time);
        }
        
        for (int i = 0; i < buildingHandler->amenityOnMap->count(); i++)
        {
            Building* b = (Building*) buildingHandler->amenityOnMap->objectAtIndex(i);
            b->updateBuilding(time);
        }
        
        constructionHandler->update(time);
        GameHUD::getThis()->update(time);
    
        spriteHandler->update(time);
        banditsAttackHandler->update(time);
        
        mapHandler->update(time);
    }
    
    // check lose game
    /*
    if (GameManager::getThis()->hasLostGame())
    {
        this->unschedule(schedule_selector(GameScene::update));
        
        
        AlertBox* alert = AlertBox::create();
        alert->useAsTopmostPopupMenu();
        alert->useAsExclusivePopupMenu();
        alert->setDisplayText("Your people have had enough of your mismanagement! Furious citizens drag you from your office...");
        
        alert->addButton(0, "DOH!", this, callfuncO_selector(GameScene::lostGame), this);
   
        
        
    }
    */
    
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
            mapHandler->getMap()->removeChild(ob->getSprite());
            allTokens->removeObjectAtIndex(i);
            
            GameHUD::getThis()->addReputation(5);
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
                SpriteInfoMenu* spriteInfoMenu = new SpriteInfoMenu(gameSprite);
                spriteInfoMenu->autorelease();
                spriteInfoMenu->useAsBasePopupMenu();
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
        
        if (selectedTile->building && selectedTile->building->buildingType != DECORATION)
        {
            if(selectedTile->building->buildingType == AMENITY || selectedTile->building->buildingType == MILITARY)
            {
                SelectPopulation* selectPopulation = SelectPopulation::create(selectedTile->building);
                selectPopulation->useAsBasePopupMenu();
            }
            else
            {
                BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(selectedTile->building);//new BuildingInfoMenu(selectedTile->building);
                buildingInfoMenu->useAsBasePopupMenu();
            }
            
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
                    selectedTile->building->buildingType != DECORATION)
                {
                    if(selectedTile->building->buildingType == AMENITY || selectedTile->building->buildingType == MILITARY)
                    {
                        SelectPopulation* selectPopulation = SelectPopulation::create(selectedTile->building);
                        selectPopulation->useAsBasePopupMenu();
                    }
                    else
                    {
                        BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(selectedTile->building);//new BuildingInfoMenu(selectedTile->building);
                        buildingInfoMenu->useAsBasePopupMenu();
                    }
                    
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





