//
//  Sprite.cpp
//  PopulationTest
//
//  Created by Serious Games on 25/3/13.
//
//

#include "Sprite.h"
#include "GameScene.h"
#include "Selector.h"
#include "GameManager.h"
#include "GameHUD.h"
#include "PopulationMenu.h"
#include "GameDefaults.h"
#include "NameGenerator.h"
#include "BuildingHandler.h"
#include "GlobalHelper.h"
#include "ReputationOrb.h"

#include <sstream>

GameSprite::GameSprite()
{
    spriteW = 128;
    spriteH = 128;
    spriteSpeed = 50;
    currAction = IDLE;
    nextAction = IDLE;
    
    futureAction1 = IDLE;
    futureAction2 = IDLE;
    
    config_doc = "";
    shouldStopNextSquare = false;
    speechBubble = NULL;
    isFollowingMoveInstruction = false;
    currTile = NULL;
    shouldSetVisibleNextFrame = false;
    
    cumulativeTime = 0.0f;
    cumulativeTime_happiness = 0.0f;
    cumulativeTime_attack = 0.0f;
    
    path = NULL;//CCArray::create();
    //path->retain();
    
    gender = 'n';
    race = 'n';
    
    wanderFlag = false;
    isLeavingNextUpdate = false;
    
    isInBuilding = false;
    
    idleDelay = 0.0f;
    
    // Jerry added
    jobLocation = NULL;
    job = NONE;
    isDoingJob = false;
    
    foodCarried = 0;
    current_money_rob = 0;
    current_food_rob = 0;
    
    tryEscape = false;
    stopAction = false;
    
    targetLocation = CCPointZero;
    nextTile = CCPointZero;
    
    mGameWarMode = false;
    
    barHP = new ProgressBar();
    barHP->createProgressBar(CCRectMake(0, 0, 80, 20),
                             CCRectMake(5, 5, 70, 10),
                             "Energy_brown bar.png",
                             "NONE",
                             "NONE",
                             "Energybar.png", true);
    barHP->setAnchorPoint(ccp(0.5, 0.5));
    
    hpLabels = CCArray::create();
    hpLabels->retain();
    
    combatState = C_IDLE;
    
    isMoving = false;
    
    enermy = NULL;
    preEnermy = NULL;
    
    currentTile = CCPointZero;
}


void GameSprite::initAI(bool isUpgrade)
{
    // common stats
    bool parsingSuccessful = false;
    
    if(!isUpgrade)
    {
        loadSpritePossessions();
    }
    
    // specific stats
    parsingSuccessful = loadClassSetup();
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse defaults doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    else
    {
        loadClassPossessions();
    }
    
    clearSetup();
}

bool GameSprite::loadClassSetup()
{
    if (config_doc.length() == 0)
    {
        std::cout << "no configuration loaded!" << std::endl;
        return false;
    }
    
    // this is the main method that starts off all the calculations
    //initialize behavior tree
    bool parsingSuccessful = reader.parse( config_doc, root );
    if ( !parsingSuccessful )
    {
        CCLog("%s",this->spriteClass.c_str());
        std::cout  << "Failed to parse config doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    std::string encoding = root.get("encoding", "UTF-8" ).asString();
    
    behaviorTree = buildTreeWithJsonValue(root);
    
    
    parsingSuccessful = reader.parse(defaults_doc, defaultsRoot);
    
    return parsingSuccessful;
}

void GameSprite::loadSpritePossessions()
{
    possessions->default_work_rate = GameScene::getThis()->configSettings->default_work_rate;
    
    possessions->happinessRating = GameScene::getThis()->configSettings->default_hapiness;
    possessions->loyaltyRating = GameScene::getThis()->configSettings->default_loyalty;
    
    possessions->currentHungry = GameScene::getThis()->configSettings->default_current_hungry;
    possessions->energyRating = GameScene::getThis()->configSettings->default_current_energy;
    
    possessions->default_hapiness_limit = GameScene::getThis()->configSettings->default_hapiness_limit;
    possessions->default_loyalty_limit = GameScene::getThis()->configSettings->default_loyalty_limit;
}

void GameSprite::loadClassPossessions()
{
    possessions->default_work_unit_per_day = atof( defaultsRoot["default_work_unit_per_day"].asString().c_str());
    possessions->default_movement_range = atoi( defaultsRoot["default_movement_range"].asString().c_str());
    possessions->default_movement_speed = atof( defaultsRoot["default_movement_speed"].asString().c_str());
    possessions->default_animate_speed = atof( defaultsRoot["default_animate_speed"].asString().c_str());
    
    possessions->default_hungry_limit = atoi(defaultsRoot["default_hungry_limit"].asString().c_str());
    
    possessions->default_food_carriage_limit = atoi(defaultsRoot["default_food_carriage_limit"].asString().c_str());
    possessions->default_energy_limit = atoi(defaultsRoot["default_energy_limit"].asString().c_str());
    
    possessions->max_endurance = atoi(defaultsRoot["default_starting_endurance"].asString().c_str());
    possessions->current_endurance = possessions->max_endurance;
    
    possessions->attack_cooldown = atof( defaultsRoot["default_attacking_cooldown_time"].asString().c_str() );
    possessions->attack_power_min = atoi( defaultsRoot["default_attack_power_min"].asString().c_str() );
    possessions->attack_power_max = atoi( defaultsRoot["default_attack_power_max"].asString().c_str() );
}

void GameSprite::clearSetup()
{
    //  std::cout<<"created tree\n\n";
    //always reinitialize the behavior tree to point the current child back to the first left child
    
    behaviorTree->onInitialize();
    
    defaultsRoot.clear();
    root.clear();
}


Behavior* GameSprite::buildTreeWithJsonValue(Json::Value json)
{
    //find out if its a behaviour/sequence/selector
    //note: for each behavior item, I need to set the pointer to possessions. 
    if (json["type"]=="selector")
    {
        
        Selector* root = new Selector();
        root->setSprite(this);
        for (int i = 0; i<json["items"].size(); i++) {
            root->m_Children.push_back(buildTreeWithJsonValue(json["items"].operator[](i)));
        }
        return root;
        
        
    }else if (json["type"]=="sequence")
    {
        Sequence* rootseq = new Sequence();
        rootseq->setSprite(this);
        for (int i = 0; i<json["items"].size(); i++) {
            rootseq->m_Children.push_back(buildTreeWithJsonValue(json["items"].operator[](i)));
        }
        return rootseq;
        
    }else
    {
        // ** Map string name to function names, add more statements when adding more functions ** //
        
        Behavior* b = new Behavior();
        b->setSprite(this);
        b->file_name = json["file_name"].asString();
        b->func_name = json["func_name"].asString();
        b->name = json["name"].asString();
        b->BehaviorInit();
        //retStatus should be dynamically set in the behaviorTree

        return b;
    }
    
}

GameSprite::~GameSprite()
{
    //I shouldn't have to do anything here, but watch for instanced variables like spriteName.
  
    if (path != NULL)
    {
        path->removeAllObjects();
        path->release();
    }
}

GameSprite* GameSprite::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    GameSprite* pCopy = NULL;
    
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (GameSprite*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = GameSprite::create();
        pCopy->batchLayerIndex = this->batchLayerIndex;
        pCopy->idleFrameCount = this->idleFrameCount;
        pCopy->walkingFrameCount = this->walkingFrameCount;
        pCopy->spriteName = this->spriteName;
        pCopy->spriteClass = this->spriteClass;
        pCopy->currAction = IDLE;
        pCopy->config_doc = this->config_doc;
        pCopy->defaults_doc = this->defaults_doc;
        pCopy->gender = this->gender;
        pCopy->race = this->race;
        pCopy->villagerClass = this->villagerClass;
        pNewZone = new CCZone(pCopy);
    }
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

GameSprite* GameSprite::create()
{
    GameSprite *pRet = new GameSprite();
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

 void GameSprite::makeSprite(CCPoint* tilePos)
 {
     possessions = new Possessions();
     spriteRep = CCSprite::create();
     
     std::string initName = spriteName.c_str();
     initName+= "_IDL001.png";
     spriteRep->initWithSpriteFrameName(initName.c_str());
     spriteRep->setAnchorPoint(ccp(0.5, 0.75));

     spriteRep->setScale(0.5f);
    // spriteRep->retain();
     CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(tilePos);
 
     spriteRep->setPosition(target);
     
     setAction(IDLE);
     idleDelay = 0.0f;
     
     initAI(false);
     
     changeAnimation("DL");
     currentDir = "DL";
     
     // Progressive bar
     barHP->setValue((float) getPossessions()->current_endurance / (float) getPossessions()->max_endurance);
     barHP->setPosition(ccp(spriteRep->boundingBox().size.width * 0.5f, spriteRep->boundingBox().size.height * 1.0f));
     spriteRep->addChild(barHP);
     
     mGameCurrentEndurance = getPossessions()->current_endurance;
     mGameMaxEndurance = getPossessions()->max_endurance;
     
     if (villagerClass != V_BANDIT && villagerClass != V_SOLDIER)
     {
         barHP->setVisible(false);
     }
     else
     {
         barHP->setVisible(true);
     }
     
     //Speech bubble
     speechBubble = new SpeechBubble();
     speechBubble->createSpeechBubble();
     
     speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 0.8f,
                                   spriteRep->boundingBox().size.height * 1.5f));
     spriteRep->addChild(speechBubble);

     GameScene::getThis()->mapHandler->getMap()->addChild(spriteRep,
                                                          GameScene::getThis()->mapHandler->calcZIndex(*tilePos));
     //speechBubble->autorelease();
     
     
     behaviorTree->onInitialize();
     behaviorTree->update();
     
     spriteDisplayedName = NameGenerator::GenerateName(race, gender);

     currPos = getWorldPosition();
     currPos = GameScene::getThis()->mapHandler->tilePosFromLocation(currPos);
     
     currTile = GameScene::getThis()->mapHandler->getTileAt(currPos.x, currPos.y);
 }

void GameSprite::unmakeSpriteEndGame()
{
    spriteRep->stopAllActions();
    
   // CC_SAFE_RELEASE(callback);
   //spriteRep->removeChild(speechBubble, true);
    delete speechBubble;
    //  animation->release();
  //  spriteRep->release();
    if (path != NULL)
        path->removeAllObjects();
    
    if (hpLabels != NULL)
    {
        hpLabels->removeAllObjects();
        CC_SAFE_RELEASE(hpLabels);
    }
    
    //destroy the AI
    delete possessions;
    possessions = NULL;
    delete behaviorTree;
    behaviorTree = NULL;
}

void GameSprite::unmakeSprite()
{
    delete possessions;
    possessions= NULL;
    delete behaviorTree;
    behaviorTree = NULL;
    spriteRep->stopAllActions();
    
    if (GameScene::getThis()->mapHandler->getMap()->getChildren()->containsObject(spriteRep))
    {
        GameScene::getThis()->mapHandler->getMap()->removeChild(spriteRep);
    }
    
    delete speechBubble;
    
    if (path != NULL)
    {
        path->removeAllObjects();
    }
    
    if (hpLabels != NULL)
    {
        hpLabels->removeAllObjects();
        CC_SAFE_RELEASE(hpLabels);
    }
}

bool GameSprite::CreatePath(CCPoint from, CCPoint to)
{
    if (currAction != IDLE){
        setAction(IDLE);
    }
    
    if (from.equals(to))
    {
        isFollowingMoveInstruction = false;
        return false;
    }
    
    PathFinder *p = new PathFinder();
    p->setDestination(to);
    p->setSource(from);
    if (path != NULL)
    {
        if (path->count() > 0)
        {
            path->removeAllObjects();
            path->release();
            path = NULL;
        }
    }
    
    path = p->makePath(&from, &to);

    if (path->count() == 0)
    {
        //CCLog("Warning! no path from %f, %f to %f, %f - next closest node used", from.x, from.y, to.x, to.y);
        saySpeech(STUCK, 2.0f);
        path = p->makePath(&from, &p->closest);
    }
    path->retain();
    
    delete p;
    return true;
 }

bool GameSprite::CreatePathEscape(CCPoint from, CCPoint to)
{
    if (currAction != IDLE)
    {
        setAction(IDLE);
    }
    
    if (from.equals(to))
    {
        isFollowingMoveInstruction = false;
        return false;
    }
    
    PathFinder *p = new PathFinder();
    
    if (path != NULL)
    {
        if (path->count() > 0)
        {
            path->removeAllObjects();
            path->release();
            path = NULL;
        }
    }
    
    path = p->makePath(&from, &to);
    
    if (path->count() == 0)
    {
        //CCLog("Warning! no path from %f, %f to %f, %f - next closest node used", from.x, from.y, to.x, to.y);
        saySpeech(STUCK, 2.0f);
        path = p->makePath(&from, &p->closest);
    }
    path->retain();
    
    delete p;
    return true;

}

int GameSprite::getPathDistance(CCPoint from, CCPoint to)
{
    if(from.equals(to))
    {
        return 0;
    }
    else
    {
        PathFinder* p = new PathFinder();
        p->setDestination(to);
        p->setSource(from);
        CCArray* tempPath = CCArray::create();
        tempPath->retain();
        
        tempPath = p->makePath(&from, &to);
        
        int result = tempPath->count();
        
        tempPath->removeAllObjects();
        tempPath->release();
        
        return result;
    }
}




void GameSprite::followPath()
{
    isMoving = true;
    
    if(GameHUD::getThis()->pause)
    {
        isMoving = false;
        return;
    }
    
    if(stopAction)
    {
        isMoving = false;
        return;
    }
    
    int squares = path->count();
    if (squares > 0)
    {
        
        if (squares > 1)
        {
            setAction(WALKING);
            idleDelay = 0;
            int targetindex = squares - 2;
            PathfindingNode* node = (PathfindingNode*)path->objectAtIndex(targetindex);
            path->removeLastObject();
            MapTile* tile = GameScene::getThis()->mapHandler->getTileAt(node->tilepos.x, node->tilepos.y);
            if (tile->hasBuilding())
            {
                if (spriteRep != NULL)
                    spriteRep->setVisible(false);
            }
            else
            {
                if (isInBuilding)
                {
                    CCPoint spritePos = getWorldPosition();
                    spritePos = GameScene::getThis()->mapHandler->tilePosFromLocation(spritePos);
                    MapTile* tile = GameScene::getThis()->mapHandler->getTileAt(spritePos.x, spritePos.y );
                    
                    if (tile->hasBuilding())
                    {
                        if (tile->building == NULL)
                            tile->master->building->Leavehandler(this);
                        else
                            tile->building->Leavehandler(this);
                        isInBuilding = false;
                    
                    }
                }
                
                shouldSetVisibleNextFrame = true;
            }
            
            currentTile = nextTile;
            CCPoint nextPos = GameScene::getThis()->mapHandler->locationFromTilePos(&(node->tilepos));
            this->nextTile = nextPos;
            
            // if the soldier's next target tile is already taken by the bandits, stop moving and prepare to attack the bandit
            if(villagerClass == V_SOLDIER){
                CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
                CCPoint myTile = GameScene::getThis()->mapHandler->locationFromTilePos(&(currPos));
                
                for(int i = 0; i < allSprites->count(); i++){
                    GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                    if(gs->villagerClass == V_BANDIT && gs->combatState == C_IDLE && !gs->tryEscape){
                        CCPoint gsTile = GameScene::getThis()->mapHandler->locationFromTilePos(&(gs->currPos));
                        if(GlobalHelper::compareCCPoint(gs->nextTile, nextTile) || GlobalHelper::compareCCPoint(nextTile, gsTile) || GlobalHelper::compareCCPoint(gs->nextTile, myTile) || GlobalHelper::compareCCPoint(myTile, gsTile))
                        {
                            //nextTile = currentTile;
                            stopAction = true;
                            isMoving = false;
                            preEnermy = gs;
                            return;
                        }
                    }
                }
            }
            
            // if the bandit's next target tile is already taken by the soldiers, stop moving and prepare to attack the soldier
            if(villagerClass == V_BANDIT){
                CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
                CCPoint myTile = GameScene::getThis()->mapHandler->locationFromTilePos(&(currPos));
                
                for(int i = 0; i < allSprites->count(); i++){
                    // get the location of that
                    GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                    if(gs->villagerClass == V_SOLDIER && gs->combatState == C_IDLE && !gs->tryEscape){
                        CCPoint gsTile = GameScene::getThis()->mapHandler->locationFromTilePos(&(gs->currPos));
                        // if next tile has been pre-assigned by a soldier or the next tile has a soldier
                        if(GlobalHelper::compareCCPoint(gs->nextTile, nextTile) || GlobalHelper::compareCCPoint(nextTile, gsTile) || GlobalHelper::compareCCPoint(myTile, gs->nextTile) || GlobalHelper::compareCCPoint(myTile, gsTile))
                        {
                            //nextTile = currentTile;
                            isMoving = false;
                            stopAction = true;
                            preEnermy = gs;
                            return;
                        }
                    }
                }
            }
            
            moveSpritePosition(nextPos, this);
        }
        else
        {
            idleDelay = 5.0f;
        }
    }
}

void GameSprite::setFrameCount(int idleFrames, int walkFrames)
{
    idleFrameCount = idleFrames;
    walkingFrameCount = walkFrames;
}

void GameSprite::setAction(SpriteAction action)
{
    currAction = action;
}

void GameSprite::changeAnimation(std::string dir)
{
    if (currentDir.compare(dir) == 0 && lastFrameAction == currAction)
    {
        return;
    }
    
    currentDir = dir;
    spriteRep->stopAllActions();
    
    
    CCArray *animFrames = CCArray::create();
    std::string frameName = spriteName.c_str();
    int frameCount = 0;
    if (currAction == WALKING)
    {
        frameName += "_W";
        frameCount = walkingFrameCount;
    }
    else
    {
        frameName += "_I";
        frameCount = idleFrameCount;
    }
    
    frameName += dir;
    
    std::string strNum;
    std::string frameNum = "001.png";
    
    //set the sprite rep to use the very first frame in the very first animation

    for (int i = 1; i <= frameCount; i++)
    {
       std::stringstream ss;

        ss << i;
        strNum = ss.str();
        
        frameNum = "00" + strNum +".png";
        CCSpriteFrame* fr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((frameName + frameNum).c_str());
        
        if (fr != NULL)
        {
            animFrames->addObject(fr);
        }	
        
    }
    
    animation = CCAnimation::createWithSpriteFrames(animFrames, 50.0f / possessions->default_animate_speed * 0.2f);
    
    animation->setRestoreOriginalFrame(false);
    spriteAnimAction = CCRepeatForever::create(CCAnimate::create(animation));
    
    spriteRep->runAction(spriteAnimAction);
    
    
}

void GameSprite::moveSpritePosition(CCPoint target, cocos2d::CCObject *pSender)
{
    if (spriteRep == NULL){
        isMoving = false;
        return;
    }
    
    CCPoint diff = ccpSub(target, spriteRep->getPosition());
    
    if (spriteRep->numberOfRunningActions() > 1)
    {
        spriteRep->stopAction(spriteRunAction);
    }
    
    if (diff.x > 0 && diff.y > 0)
    {
        changeAnimation("UR");
    }
    else if (diff.x < 0 && diff.y > 0)
    {
        changeAnimation("UL");
    }
    else if (diff.x > 0 && diff.y < 0)
    {
        changeAnimation("DR");
    }
    else if (diff.x < 0 && diff.x < 0)
    {
        changeAnimation("DL");
    }

    callback = CCCallFuncN::create(pSender, callfuncN_selector(GameSprite::moveComplete));
    callback->retain();
    spriteRunAction = CCSequence::createWithTwoActions(CCMoveBy::create(50.0f / possessions->default_movement_speed, diff), callback);

    spriteRep->runAction(spriteRunAction);
    
    CC_SAFE_RELEASE(callback);

}

void GameSprite::moveComplete(cocos2d::CCObject *pSender)
{
    // Because moveComplete is called instantly after sequential action and
    // updateSprite is called at intervals of approx 16ms,
    // we need to calculate the newest currPos right now.
    currPos = getWorldPosition();
    currPos = GameScene::getThis()->mapHandler->tilePosFromLocation(currPos);
    
    currTile = GameScene::getThis()->mapHandler->getTileAt(currPos.x, currPos.y);
    
    // each time after moveComplete has been triggered, the soldier will check the bandit's position again, so that they can adjust their path to the capture the bandit.
    if (villagerClass == V_SOLDIER)
    {
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        
        CCPoint nearestTarget = CCPointZero;
        int nearestDistance = 999999;
        
        // get the current position of the soldier
        CCPoint sPos = getWorldPosition();
        sPos = GameScene::getThis()->mapHandler->tilePosFromLocation(sPos);
        
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            
            // check the bandits, get the nearest bandit to the soldier, as well as the distance.
            if ((villagerClass == V_BANDIT) && gs->possessions->current_endurance > 0)
            {
                CCPoint bPos = gs->getWorldPosition();
                bPos = GameScene::getThis()->mapHandler->tilePosFromLocation(bPos);
                
                int tempDistance = getPathDistance(bPos, sPos);
                if(tempDistance < nearestDistance)
                {
                    nearestTarget = bPos;
                    nearestDistance = tempDistance;
                }
            }
        }
        
        // if the nearest bandit has a distance more than 2 (not in adjacent tile) from the soldier, the soldier will target the position of the nearest bandit and go there.
        if (nearestDistance < 999999 && nearestDistance > 2)
        {
            if(targetLocation.x != nearestTarget.x || targetLocation.y != nearestTarget.y)
            {
                targetLocation = nearestTarget;
                GoLocation(nearestTarget, false);
                isMoving = false;
                return;
            }
        }
    }

    
    
    if (path == NULL){
        isMoving = false;
        return;
    }
    
    if (path->count() > 1 && !shouldStopNextSquare) //where 1 is the position the sprite is standing on
    {
        if (shouldSetVisibleNextFrame)
        {
            spriteRep->setVisible(true);
            shouldSetVisibleNextFrame = false;
        }
        
        followPath();
        
    }
    else
    {
        shouldStopNextSquare = false;
        isFollowingMoveInstruction = false;
        setAction(IDLE);
        idleDelay = 5.0f;
        changeAnimation(currentDir);
        isMoving = false;
        
        /* trigger the event that should occur when the player arrives at his destination*/
        /* but only if a building is present.*/
        /*note: for multitile buildings the destination is where the building's main section is, the master* will never be used for that purpose.*/
        if (currTile)
            if (currTile->building)
            {
                isInBuilding = true;
                currTile->building->ArriveHandler(this);
            }
       // if (behaviorTree != NULL)
      //  {
            behaviorTree->onInitialize();
            behaviorTree->update();
            
       // }
    }
    
    //for the AI to run the decision tree must be reinited every frame.
    /*note: I have no idea if this must be run every frame or not. This sort of algo unravels the entire tree each frame!*/


}

bool GameSprite::Wander()
{
    // The very first action for a refugee is alway trying to find a house, he/she will lose happiness continually.
    if(spriteClass.compare("refugee") == 0 && hasEmptyHouse())
    {
        return findNearestHome();
    }
    
    if(futureAction1 == EATING)
    {
        goToEat();
        return false;
    }
    
    if(futureAction1 == RESTING)
    {
        goToSleep();
        return false;
    }
    
    //Note: the random nature of Wander means there is no need to set isFollowingMoveInstruction to true. Wander should be interruptable at any time.
    wanderFlag = !wanderFlag;
    if (wanderFlag)
    {
        saySpeech(IDLING, 5.0f);
        
        CCPoint tgt = GameScene::getThis()->mapHandler->getRandomPathTile();//CCPointMake( rand() % 40, rand() % 40);
        if (tgt.x == -1 && tgt.y == -1)
        {
            setAction(IDLE); //no path, can't
            return false;
        }
        
        CCPoint startPos = getWorldPosition();
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        if (CreatePath(startPos, tgt))
        {
            setAction(WALKING);
        
            followPath();
            return true;
        }
        else
        {
            setAction(IDLE);
            changeAnimation(currentDir);
            
            return false;
        }
    }
    else
    {
        setAction(IDLE);
        idleDelay = 5.0f;
        
        changeAnimation(currentDir);
        
        return true;
    }
    
   // wanderFlag = !wanderFlag;
}

bool GameSprite::attack()
{
    // check the tryEscape flag, if true, run away    
    if(tryEscape)
    {
        return escape();
    }
    
    // The very first action for a bandit is always trying to robe the nearest granary for food or town hall for money.
    if (spriteClass.compare("bandit") == 0 && hasValidTarget())
    {
        return true;
    }
    else
    {
        saySpeech("Enough already, go back loh!", 5.0f);
        tryEscape = true;
        return false;
    }
}

void GameSprite::StopMoving()
{
    if (currAction == IDLE && shouldStopNextSquare) return;
    
    shouldStopNextSquare = true;
    /*I think its better to stop on the very next square rather than just stopping while the damn thing is moving. */
    
}

SpriteAction GameSprite::getAction()
{
    return currAction;
}


bool GameSprite::PathToResources()
{
    Building* b = GameScene::getThis()->buildingHandler->getNearestStorage(this);
    if (b == NULL) return false;
    
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    
    CCPoint endPos =b->getWorldPosition();
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    
    
    return hasPath;
}

void GameSprite::updateSprite(float dt)
{
    updateZIndex();
    
    lastFrameAction = currAction;
    
    if(villagerClass == V_BANDIT)
    {
        if(currAction == IDLE)
        {
            attack();
        }
    }
    
    /****************************************************
     * This part is only for the combating in the game! *
     ****************************************************/
    if(GameScene::getThis()->banditsAttackHandler->warMode)
    {
        // move and fade out the damage label
        for (int i = 0; i < hpLabels->count(); i++)
        {
            CCLabelTTF* label = (CCLabelTTF*) hpLabels->objectAtIndex(i);
            label->setPosition(ccp(label->getPosition().x, label->getPosition().y + 1));
            
            int labelOpacity = (int)label->getOpacity();
            int newOpacity = labelOpacity - 10;
            int limitOpacity = 0;
            
            if(newOpacity < limitOpacity)
            {
                newOpacity = limitOpacity;
                spriteRep->removeChild(label, true);
                hpLabels->removeObject(label);
            }
            
            label->setOpacity(newOpacity);
        }
        
        if(combatState == C_COMBAT)
        {
            // currently only apply this to bandit but not the soldiers
            if(villagerClass == V_BANDIT)
            {
                if(enermy != NULL && enermy->enermy == this && enermy->combatState == C_COMBAT)
                {
                    CCPoint myPos = this->getWorldPosition();
                    myPos = GameScene::getThis()->mapHandler->tilePosFromLocation(myPos);
                    
                    CCPoint enermyPos = this->getWorldPosition();
                    enermyPos = GameScene::getThis()->mapHandler->tilePosFromLocation(enermyPos);
                    
                    int tempDistance = getPathDistance(myPos, enermyPos);
                    
                    if(tempDistance <= 2)
                    {
                        this->stopAction = true;
                        enermy->stopAction = true;
                    }
                    else
                    {
                        enermy->enermy = NULL;
                        enermy->combatState = C_IDLE;
                        enermy->currAction = IDLE;
                        enermy->followPath();
                        
                        this->enermy = NULL;
                        this->combatState = C_IDLE;
                        this->currAction = IDLE;
                        this->followPath();
                        return;
                    }
                }
                else if(enermy == NULL || (enermy != NULL && enermy->enermy != this))
                {
                    this->enermy = NULL;
                    this->combatState = C_IDLE;
                    this->currAction = IDLE;
                    this->followPath();
                    return;
                }
            }
            
            // base on the direction of the sprite, get the check attacking area of that sprite.
            CCPoint temp = CCPointZero;
            CCPoint infront = CCPointZero;
            CCPoint back = CCPointZero;
            CCPoint left = CCPointZero;
            CCPoint right = CCPointZero;
            if(currentDir.compare("DR") == 0){
                temp.x = currPos.x + 1;
                temp.y = currPos.y;
                
                back.x = currPos.x -1;
                back.y = currPos.y;
                
                left.x = currPos.x;
                left.y = currPos.y - 1;
                
                right.x = currPos.x;
                right.y = currPos.y + 1;
            } else if(currentDir.compare("UR") == 0){
                temp.x = currPos.x;
                temp.y = currPos.y - 1;
                
                back.x = currPos.x;
                back.y = currPos.y + 1;
                
                left.x = currPos.x - 1;
                left.y = currPos.y;
                
                right.x = currPos.x + 1;
                right.y = currPos.y;
            } else if(currentDir.compare("DL") == 0){
                temp.x = currPos.x;
                temp.y = currPos.y + 1;
                
                back.x = currPos.x;
                back.y = currPos.y - 1;
                
                left.x = currPos.x + 1;
                left.y = currPos.y;
                
                right.x = currPos.x - 1;
                right.y = currPos.y;
            } else if(currentDir.compare("UL") == 0){
                temp.x = currPos.x - 1;
                temp.y = currPos.y;
                
                back.x = currPos.x + 1;
                back.y = currPos.y;
                
                left.x = currPos.x;
                left.y = currPos.y + 1;
                
                right.x = currPos.x;
                right.y = currPos.y - 1;
            }
            infront = temp;
            
            if((villagerClass == V_SOLDIER) && !tryEscape)
            {
                CCArray* allSpritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
                
                if(!hasBandit(allSpritesOnMap, infront))
                {
                    if(hasBandit(allSpritesOnMap, left))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DL");
                        }
                    }
                    else if(hasBandit(allSpritesOnMap, right))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("UR");
                        }
                    }
                    else if(hasBandit(allSpritesOnMap, back))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DR");
                        }
                    }
                    else
                    {
                        currAction = IDLE;
                        combatState = C_IDLE;
                        stopAction = false;
                        return;
                    }
                }
            }
            
            if ((villagerClass == V_BANDIT) && !tryEscape)
            {
                CCArray* allSpritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
                
                if(!hasSoldier(allSpritesOnMap, infront))
                {
                    if(hasSoldier(allSpritesOnMap, left))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DL");
                        }
                    }
                    else if(hasSoldier(allSpritesOnMap, right))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("UR");
                        }
                    }
                    else if(hasSoldier(allSpritesOnMap, back))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DR");
                        }
                    }
                    else
                    {
                        stopAction = false;
                        combatState = C_IDLE;
                        if(enermy != NULL && enermy->enermy != this)
                        {
                            enermy = NULL;
                            currAction = IDLE;
                        }
                        return;
                    }
                }
            }
            
            // only soldiers and bandits are involved in the combat.
            if((villagerClass == V_SOLDIER || villagerClass == V_BANDIT) && !tryEscape)
            {
                bool hasEnermy = false;
                
                if(enermy != NULL)
                {
                    CCPoint enermyTile = enermy->currPos;
                    if(temp.x == enermyTile.x && temp.y == enermyTile.y)
                    {
                        hasEnermy = true;
                    }
                }

                if(hasEnermy)
                {
                    cumulativeTime_attack += dt;
                }
                else
                {
                    cumulativeTime_attack = 0;
                }
                
                if(cumulativeTime_attack >= possessions->attack_cooldown)
                {
                    int diff = possessions->attack_power_max - possessions->attack_power_min;
                    
                    if(temp.x == enermy->currPos.x && temp.y == enermy->currPos.y)
                    {
                        int random_number = rand() % diff;
                        int damage = possessions->attack_power_min + random_number;
                        enermy->damaged(damage);
                    }
                    
                    cumulativeTime_attack = 0;
                }
            
            }
        }
    
        // the sate is used for the bandits and soldiers when they either get enough resources and try to run out or they lose all endurance so that they cannot do task and combat any more.
        if(combatState == C_ESCAPE)
        {
            if(villagerClass == V_BANDIT)
            {
                if(!isMoving)
                {
                    tryEscape = true;
                    escape();
                    stopAction = false;
                }
            }
        }
        else if(combatState == C_IDLE)
        {
            // free means sprites that are not in battle and are not escaping
            // free bandits will be stop by free soldiers to a 1:1 ratio and become non-free bandits. They will escape after losing all endurance
            if (villagerClass == V_BANDIT)
            {
                // get the current position of the soldier
                CCPoint bPos = getWorldPosition();
                bPos = GameScene::getThis()->mapHandler->tilePosFromLocation(bPos);
                
                // check whether the bandits meet the soldiers
                CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
                
                bool stop = false;
                GameSprite* opponent = NULL;
                // check all the soldiers, if any free soldier is adjacent to the free bandit and the bandit is not trying to escape, stop moving and prepare to attack the soldier
                for (int i = 0; i < spritesOnMap->count(); i++)
                {
                    GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
                    
                    if (gs->villagerClass == V_SOLDIER && gs->combatState == C_IDLE && !gs->tryEscape)
                    {
                        CCPoint sPos = gs->getWorldPosition();
                        sPos = GameScene::getThis()->mapHandler->tilePosFromLocation(sPos);
                        int tempDistance = getPathDistance(currPos, sPos);
                        if(tempDistance <= 2 && !tryEscape && gs->combatState == C_IDLE && gs->enermy == NULL)
                        {
                            stop = true;
                            opponent = gs;
                            break;
                        }
                    }
                }
                
                if(stop){
                    if(opponent != NULL)
                    {
                        //stopAction = true;
                        enermy = opponent;
                        combatState = C_COMBAT;
                        currAction = FIGHTING;
                        
                        //enermy->stopAction = true;
                        enermy->enermy = this;
                        enermy->combatState = C_COMBAT;
                        enermy->currAction = FIGHTING;
                        
                        for(int i = 0; i < spritesOnMap->count(); i++)
                        {
                            GameSprite* gs = (GameSprite*)spritesOnMap->objectAtIndex(i);
                            
                            if(gs != this && gs->villagerClass == V_BANDIT && gs->combatState == C_IDLE)
                            {
                                if(gs->stopAction && gs->preEnermy == enermy)
                                {
                                    gs->stopAction = false;
                                    gs->followPath();
                                    gs->currAction = WALKING;
                                    gs->preEnermy = NULL;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(!isMoving)
                    {
                        //stopAction = false;
                        //attack();
                    }
                }
            }
            
            // solders will actively to block bandits!
            if (villagerClass == V_SOLDIER)
            {
                CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
                
                CCPoint nearestTarget = CCPointZero;
                int nearestDistance = 999999;
                
                // get the current position of the soldier
                CCPoint sPos = getWorldPosition();
                sPos = GameScene::getThis()->mapHandler->tilePosFromLocation(sPos);
                GameSprite* opponent = NULL;
                
                for (int i = 0; i < spritesOnMap->count(); i++)
                {
                    GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
                    
                    // check the bandits, get the nearest bandit to the soldier, as well as the distance.

                    if ((gs->villagerClass == V_BANDIT) && gs->possessions->current_endurance > 0 && !gs->tryEscape && gs->combatState == C_IDLE)
                    {
                        CCPoint bPos = gs->getWorldPosition();
                        bPos = GameScene::getThis()->mapHandler->tilePosFromLocation(bPos);
                        
                        int tempDistance = getPathDistance(bPos, sPos);
                        if(tempDistance < nearestDistance)
                        {
                            nearestTarget = bPos;
                            nearestDistance = tempDistance;
                            opponent = gs;
                        }
                    }
                }
                
                // if the nearest bandit has a distance more than 2 (not in adjacent tile) from the soldier, the soldier will target the position of the nearest bandit and go there.
                if (nearestDistance < 999999 && nearestDistance > 2)
                {
                    targetLocation = nearestTarget;
                    
                    if(!isMoving)
                    {
                        //stopAction = false;
                        GoLocation(nearestTarget, false);
                    }
                }
                // if the nearest bandit is adjacent to the soldier, stop moving further and prepare to attact the bandit.
                else if(nearestDistance <= 2)
                {
                     if(opponent != NULL)
                     {
                         //stopAction = true;
                     }
                }
                else
                {
                    if(stopAction)
                    {
                        //stopAction = false;
                        GoBuilding(jobLocation);
                    }
                }
            }
        }

    }

    //After this part, only functions relating to IDLE will be called. This is because WALKING already has its own function calls.
    
    // check the sprite interacts with the building
    if(currTile != NULL)
    {
        if(currTile->building)
        {
            currTile->building->StickAroundHandler(this, dt);
        }
    }

    if (idleDelay > 0.0f)
    {
        if (idleDelay > dt)
            idleDelay -= dt;
        else
            idleDelay = 0;
        return;
    }
    else
    {
        
        if (currAction != IDLE) return;
     
        
        if (isLeavingNextUpdate)
        {
            GameHUD::getThis()->showHint(spriteDisplayedName +" has left the town.");
            GameScene::getThis()->spriteHandler->removeSpriteFromMap(this);
            
            return;
        }
        
        if (possessions->loyaltyRating <= 0)
        {
           // saySpeech(UNHAPPY, 2.0f);
            isLeavingNextUpdate = true;
            
        }
        
        //restore pathing
        if (nextAction == BUILD)
        {
            GoBuild(lastTarget);
        }
        if (nextAction == EATING)
        {
            GoEat(lastTarget);
            
        }
        if (nextAction == FARMING)
        {
            GoRest(lastTarget);
        }
        
        /*kick starts the sprite's AI, because idle doesn't do this normally*/
        behaviorTree->onInitialize();
        behaviorTree->update();
    }
}


void GameSprite::updateZIndex()
{
    if (!spriteRep) return;
    if (!spriteRep->isVisible()) return;
    if (path == NULL) return; //not necessary to update Z if there isn't a path, implies sprite isn't moving
    if (path->count() == 0) return; //same reason
    // needs to be in world space
    // CCPoint position = getWorldPosition();//GameScene::getThis()->mapHandler->getMap()->convertToWorldSpace(spriteRep->getPosition()); // .ConvertToWorldSpace(startPos);
    
    
    //position = GameScene::getThis()->mapHandler->tilePosFromLocation(position);
    spriteRep->setZOrder( GameScene::getThis()->mapHandler->calcZIndex(currPos, 0, true, this) );
    speechBubble->setZOrder( GameScene::getThis()->mapHandler->calcZIndex(currPos, 0, true, this) );
}

CCPoint GameSprite::getWorldPosition()
{
    if (!spriteRep) return CCPointMake(-1,-1);
    //good thing the scale's on another layer, so I can safely do this now.
    
    return ccpAdd(spriteRep->getPosition(), GameScene::getThis()->mapHandler->getMap()->getPosition());
    
    //GameScene::getThis()->mapHandler->getMap()->convertToWorldSpace(spriteRep->getPosition());
}

/*default setters for spritehandler*/
void GameSprite::setAIConfig(std::string config)
{
    config_doc = config;
}

void GameSprite::setDefaultsConfig(std::string config)
{
    defaults_doc = config;
}

Possessions* GameSprite::getPossessions()
{
    return possessions;
}

void GameSprite::increasePossession(PossessionStats statType, int value)
{
    /*
    // Init our vars
    if (possessions == NULL) return;
    
    int* pStat;
    int statCap = 1000;
    std::string statName = "Exp";
    switch (statType)
    {

        case STATS_HAPPINESS:
            pStat = &possessions->happinessRating;
            statName = "Hap";
            break;
        case STATS_ENERGY:
            pStat = &possessions->energyRating;
            statCap = possessions->default_energy_limit;
            statName = "Energy";
            break;
        case STATS_EXP:
            statCap = possessions->getExpToLevel();
            statName = "Exp";
            break;
        default:
            return;
    }
    
    // Increase stat while clamping, store useful vars
    int initial = *pStat;
    int overflow = 0;
    *pStat += value;
    if (*pStat < 0)
    {
        overflow = *pStat;
        *pStat = 0;
    }
    else if (*pStat > statCap)
    {
        overflow = *pStat - statCap;
        *pStat = statCap;
    }
    int diff = *pStat - initial;
    
    // Say speech if difference is not 0
    if (diff != 0)
    {
        if (diff > 0)
            statName += " +";
        else
            statName += " ";
        char numstr[21]; // enough to hold all numbers up to 64-bits
      //  sprintf(numstr, "%i", diff);
        statName = statName + numstr;
        saySpeech(statName.c_str(), 3.0f);
    }
    
    // Do aftermath
    switch (statType)
    {

        case STATS_HAPPINESS:
            GameManager::getThis()->totalHappiness += diff;
            GameHUD::getThis()->updateAvgHapLabel();
            break;
        case STATS_EXP:
            if (possessions->classLevel < possessions->expToLevel->count() &&
                *pStat >= statCap)
            {
                //level up!
                *pStat = 0;
                // Add overflow recursively for multi-leveling
                increasePossession(STATS_EXP, overflow);
                
                if (possessions->isAtMaxLevel())
                {
                    if (possessions->isAtAbsoluteMax())
                        saySpeech("MAX ABSOLUTE LEVEL!", 3.0f);
                    
                    
                    else
                        saySpeech("MAX CLASS LEVEL!", 3.0f);
                }
                else
                {
                    saySpeech("LEVEL UP!", 3.0f);
                    
                    //possessions->educationLevel++;
                    possessions->classLevel++;
                }
            }
            break;
        default:
            return;
    }
    */
}

void GameSprite::saySpeech(const char* text, float timeInSeconds)
{
    speechBubble->clearContent();
    CCLabelTTF* label = CCLabelTTF::create(text, "Shojumaru-Regular", 18);
    
    label->setColor(ccc3(81, 77, 2));
   
    speechBubble->addContent(label, CCPointZero);
    //speechBubble->show(timeInSeconds);
    speechBubble->show(10000);
}


void GameSprite::saySpeech(SpeechMood s, float timeInSeconds)
{
    speechBubble->clearContent();
    
    CCSprite* label = NULL;
    switch (s)
    {
        case IDLING:
            label = CCSprite::create("idle.png");
            break;
        case HAPPY:
            label = CCSprite::create("happy.png");

            break;
        case HUNGRY:
            label = CCSprite::create("hungry.png");

            break;
        case TIRED:
            label = CCSprite::create("tired.png");

            break;
        case UNHAPPY:
            label = CCSprite::create("unhappy.png");

            break;
        case STUCK:
            label = CCSprite::create("stuck.png");

            break;
        case GUARD_EMOTION:
            label = CCSprite::create("happy1.png");
            
            break;
        case BUILDER_EMOTION:
            label = CCSprite::create("happy2.png");
            
            break;
        case FARMER_EMOTION:
            label = CCSprite::create("idle1.png");
            
            break;
        case STUCK_FOOD:
            label = CCSprite::create("stuck1.png");
            
            break;
        case FIND_HOME:
            label = CCSprite::create("idle2.png");
            
            break;
        case HOMELESS:
            label = CCSprite::create("stuck2.png");
            
            break;
        default:
            break;
            
    }
    if (label == NULL) return;
    speechBubble->addContent(label, CCPointZero );
    //speechBubble->show(timeInSeconds);
    speechBubble->show(200);
}



/*pathing*/
/*paths back home, if possible. Ignores range for obvious reasons.*/
bool GameSprite::PathToHome()
{
    
    if (isFollowingMoveInstruction)
    {
        CCLog("still moving. Can't obey.");
        return false;
    }
    if (possessions->homeLocation == NULL)
    {
       //we don't do this anymoe
        //CCLog("No house?");
    //    saySpeech("Homeless..", 3.0f);
     //   increasePossession(STATS_HAPPINESS, happiness_mod_homeless); //note actually subtracts
        
        return false; // no home
    }
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    
    CCPoint endPos = possessions->homeLocation->getWorldPosition();
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    return hasPath;
}

/*paths to an existing workplace, if it does. Ignores range, because it is assumed that in this case the person cannot change where he works.*/
bool GameSprite::PathToWork()
{

    if (isFollowingMoveInstruction)
    {
        CCLog("still moving. Can't obey.");
        return false;
    }
    /*
    if (possessions->hasJob == false){
     
        //we don't do this here anymore, as the hasjob check prevents this from running.
     //   saySpeech("Jobless..", 3.0f);
     //   increasePossession(STATS_HAPPINESS, happiness_mod_jobless); //note actually subtracts
        return false;
    }*/
    
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    bool hasPath = false; //temporary TODO
   // CCPoint endPos = possessions->jobLocation->getWorldPosition();
   // endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    /*
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }*/
    return hasPath;
}

bool GameSprite::PathToBuildingOverride(int building_id)
{
    //move instruction should be interruptable
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    Building* b = bh->getBuildingOnMapWithID(building_id);
    
    if (b == NULL)
    {
        CCLog("Building ain't in map anymore");
        return false;
    }


    
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    CCPoint endPos = b->getWorldPosition();
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    
    return hasPath;
}


bool GameSprite::PathToBuilding(int building_id)
{
    
    if (isFollowingMoveInstruction)
    {
        CCLog("Still moving. Can't obey.");
        return false;
    }
    
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    Building* b = bh->getBuildingOnMapWithID(building_id);
    
    if (b == NULL)
    {
        CCLog("Building ain't in map anymore");
        return false;
    }
    
    
    if (race == 'a' && b->buildingType == MILITARY)
    {
        CCLog("Aliens cannot visit Military buildings");
        return false;
    }
    
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    
    CCPoint endPos = b->getWorldPosition();
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    return hasPath;

    
}

/* paths to a target building. */
bool GameSprite::GoBuilding(Building* b)
{
    lastTarget = b;

    CCPoint startPos = getWorldPosition();
    CCPoint endPos = b->getWorldPosition();
    
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    
    return hasPath;
}

/* paths to a target location. */
bool GameSprite::GoLocation(CCPoint endPos, bool tryEscape)
{
    CCPoint startPos = getWorldPosition();
    
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    
    int tempDistance = getPathDistance(startPos, endPos);
    
    if(tempDistance <= 0)
    {
        return true;
    }
    
    bool hasPath;
    if(tryEscape)
    {
        hasPath = CreatePathEscape(startPos, endPos);
    }
    else
    {
        hasPath = CreatePath(startPos, endPos);
    }
    
    if(hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
        return true;
    } else {
        return false;
    }
}

/*Paths to a building under construction. Fails if there isn't a building under construction.*/
bool GameSprite::GoBuild(Building *b)
{
     if (b->build_uint_current >= b->build_uint_required) return false; //already built
    
    this->nextAction = BUILD;
    
    return GoBuilding(b);
}

// farming
bool GameSprite::GoFarming(Building *b)
{
   
    this->nextAction = FARMING;
    
    return GoBuilding(b);
}

bool GameSprite::GoRest(Building* b)
{
   
    // if it is not a residential house or the house does not belong to this sprite, return false -> cannot rest.
    if(b->buildingType != HOUSING || possessions->homeLocation != b)
    {
        return false;
    }
    
    this->nextAction = RESTING;
    
    return GoBuilding(b);
}

bool GameSprite::GoEat(Building* b)
{
   
    this->nextAction = EATING;
    
    return GoBuilding(b);
}

bool GameSprite::GoGranary(Building* b)
{
    this->nextAction = STORING;
    
    return GoBuilding(b);
}

bool GameSprite::returnFarm(Building* b)
{
    this->nextAction = FARMING;
    
    return GoBuilding(b);
}

bool GameSprite::GoHome(Building* b)
{
    this->nextAction = GET_HOME;
    
    return GoBuilding(b);
}

bool GameSprite::PathToBuild()
{
    if (GameScene::getThis()->constructionHandler->getConstructingBuildingCount() == 0) return false; //no buildings under construction, stop.
    
    Building* target = NULL;
    CCPoint startPos = getWorldPosition();
    CCPoint endPos;
    
    float shortestDist = 9999.0f;
    
    if (GameScene::getThis()->constructionHandler->getConstructingBuildingCount() == 1)
    {
        target = (Building*)GameScene::getThis()->constructionHandler->getConstructingBuildings()->objectAtIndex(0);
        endPos = target->getWorldPosition();
    }
    else
    {
        for (int i = 0; i <GameScene::getThis()->constructionHandler->getConstructingBuildingCount(); ++i)
        {
            target =(Building*)GameScene::getThis()->constructionHandler->getConstructingBuildings()->objectAtIndex(i);
            CCPoint tEndPos = target->getWorldPosition();
            
            float dist = ccpDistanceSQ(startPos, tEndPos);
            if (dist < shortestDist)
            {
                shortestDist = dist;
                endPos = tEndPos;
            }
            
            
        }
    }
    
    
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    
    return hasPath;
    
}


bool GameSprite::PathToExit()
{
    
    
    
    return false;
}


/*checks if destination is within range of home. If any.*/
bool GameSprite::isDestinationInRange(int destinationID)
{
    /* note: destination doesn't necessarily exist as the sprite's home or workplace. This is why the Building ID is supplied. */
    /*this makes sure the character can only path a certain distance from his own home. Applies only if the character has a home, otherwise the
     character has infinite range.*/
    Building* destination = (Building*)GameScene::getThis()->buildingHandler->allBuildingsOnMap->objectAtIndex(destinationID);
    
    if (possessions->homeLocation != NULL) return true;
    
    /*create a Path first before using by calling CreatePath */
    if (!path) return false;
    if (path->count() == 0) return false;
     
    //CCArray *pathFromHome = CCArray::create();
    //no need to retain. It loses its scope after this function.
        
    CCPoint homePos = possessions->homeLocation->getWorldPosition();
    homePos = GameScene::getThis()->mapHandler->tilePosFromLocation(homePos);
    
    CCPoint to = destination->getWorldPosition();
    to = GameScene::getThis()->mapHandler->tilePosFromLocation(to);
    
    PathFinder *p = new PathFinder();
    CCArray* pathFromHome = p->makePath(&homePos, &to);
    p->setDestination(to);
    p->setSource(homePos);
    if (pathFromHome->count() > possessions->default_movement_range)
    {
        delete p;
        return false;
    }
    delete p;
    return true;
}

/*changes the gamesprite's appearance.*/
/*does not change stats.*/
void GameSprite::changeSpriteTo(GameSprite *sp, SpriteClass* sc)
{
    villagerClass = sc->villagerClass;
    batchLayerIndex = sp->batchLayerIndex;
    idleFrameCount = sp->idleFrameCount;
    walkingFrameCount = sp->walkingFrameCount;
    spriteName = sp->spriteName;
    spriteClass = sc->targetClass;
    config_doc = sc->configContent;
    defaults_doc = sc->defaultContent;
    
    bool parsingSuccessful = reader.parse(defaults_doc, defaultsRoot);
    
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse defaults doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    else
    {
        loadClassPossessions();
    }

    ReplaceSpriteRep();
}

void GameSprite::changeClassTo(SpriteClass* sc)
{
    config_doc = sc->configContent;
    defaults_doc = sc->defaultContent;
    spriteClass = sc->targetClass;
    villagerClass = sc->villagerClass;
    
    bool parsingSuccessful = reader.parse(defaults_doc, defaultsRoot);
    
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse defaults doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    else
    {
        loadClassPossessions();
    }
}

void GameSprite::ReplaceSpriteRep()
{
    CCPoint pos = spriteRep->getPosition();
    spriteRep->stopAllActions();
    
    delete behaviorTree;
    spriteRep->removeChild(speechBubble, true);
    delete speechBubble;
    spriteRep->removeChild(barHP, true);
    delete barHP;
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(spriteRep);
    
    spriteRep = CCSprite::create();
    
    std::string initName = spriteName;
    initName+= "_IDL001.png";
    spriteRep->initWithSpriteFrameName(initName.c_str());
    spriteRep->setAnchorPoint(ccp(0.5, 0.75));
    spriteRep->setScale(0.5f);

    initAI(true);
    
    spriteRep->setPosition(pos);
    setAction(IDLE);
    idleDelay = 0.0f;
    
    changeAnimation("DL");
    currentDir = "DL";

    // behaviorTree->BehaviorInit();
    
    //Speech bubble
    speechBubble = new SpeechBubble();
    speechBubble->createSpeechBubble();
    
    
    speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 0.8f,
                                  spriteRep->boundingBox().size.height * 1.5f));
    
    spriteRep->addChild(speechBubble);
    
    barHP = new ProgressBar();
    barHP->createProgressBar(CCRectMake(0, 0, 80, 20),
                             CCRectMake(5, 5, 70, 10),
                             "Energy_brown bar.png",
                             "NONE",
                             "NONE",
                             "Energybar.png", true);
    barHP->setAnchorPoint(ccp(0.5, 0.5));
    
    barHP->setValue((float) getPossessions()->current_endurance / (float) getPossessions()->max_endurance);
    barHP->setPosition(ccp(spriteRep->boundingBox().size.width * 0.5f, spriteRep->boundingBox().size.height * 1.0f));
    spriteRep->addChild(barHP);
    
    mGameCurrentEndurance = getPossessions()->current_endurance;
    mGameMaxEndurance = getPossessions()->max_endurance;
    
    if (villagerClass != V_BANDIT && villagerClass != V_SOLDIER)
    {
        barHP->setVisible(false);
    }
    else
    {
        barHP->setVisible(true);
    }
    
    GameScene::getThis()->mapHandler->getMap()->addChild(spriteRep,
                                                        GameScene::getThis()->mapHandler->calcZIndex(pos));
    behaviorTree->onInitialize();
    behaviorTree->update();
     
   // updateZIndex();
  
}

/*will override all current move instructions.*/
void GameSprite::PathToHighTemple()
{
    
    
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    if (bh->specialOnMap->count() == 0) return;
    
    Building* b = (Building*)bh->specialOnMap->objectAtIndex(0);
    
    if (b == NULL)
    {
        CCLog("Building ain't in map anymore");
        return;
    }
    
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    
    CCPoint endPos = b->getWorldPosition();
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    return;

}

/*
 * Jerry Added these functions
 */

Building* GameSprite::getHome()
{
    return possessions->homeLocation;
}

void GameSprite::setHome(Building* home)
{
    this->possessions->homeLocation = home;
}

SpriteJob GameSprite::getJob()
{
    return job;
}

void GameSprite::setJob(SpriteJob job)
{
    this->job = job;
}

Building* GameSprite::getJobLocation()
{
    return jobLocation;
}

void GameSprite::setJobLocation(Building* building)
{
    jobLocation = building;
    possessions->jobLocation = building;
}

bool GameSprite::getIsDoingJob()
{
    return isDoingJob;
}

void GameSprite::setIsDoingJob(bool isDoingJob)
{
    this->isDoingJob = isDoingJob;
}

void GameSprite::updateIdleDelay(float delay)
{
    this->idleDelay = delay;
}

void GameSprite::setIsFollowingMovementInstruction(bool flag)
{
    this->isFollowingMoveInstruction = flag;
}

void GameSprite::changeToCitizen()
{
    changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_CITIZEN));
}

void GameSprite::setTargetLocation(Building* b)
{
    possessions->targetLocation = b;
}

Building* GameSprite::getTargetLocation()
{
    return possessions->targetLocation;
}

int GameSprite::getTargetHungry()
{
    return possessions->targetHungry;
}

void GameSprite::setTargetHungry(int target)
{
    possessions->targetHungry = target;
}

void GameSprite::setFoodCarried(int food)
{
    foodCarried = food;
}

int GameSprite::getFoodCarried()
{
    return foodCarried;
}

Building* GameSprite::findNearestGranary(bool isDeliveringFood)
{
    CCArray* buildingsOnMap = GameScene::getThis()->buildingHandler->allBuildingsOnMap;
    Building* nearestGranary = NULL;
    int distance = 999999;
    
    for(int i = 0; i < buildingsOnMap->count(); i++)
    {
        Building* bui = (Building*) buildingsOnMap->objectAtIndex(i);
        
        if(bui->buildingType != GRANARY || bui->currentStorage >= bui->storageLimit)
        {
            continue;
        }
        
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = bui->getWorldPosition();
        
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
        
        int tempDistance = getPathDistance(startPos, endPos);
        
        if(tempDistance < distance)
        {
            distance = tempDistance;
            nearestGranary = bui;
        }
    }
    
    return nearestGranary;
}

bool GameSprite::isHungry()
{
    return possessions->currentHungry < possessions->default_hungry_limit;
}

float GameSprite::getCumulativeTime()
{
    return cumulativeTime;
}

void GameSprite::setCumulativeTime(float t)
{
    cumulativeTime = t;
}

void GameSprite::goToEat()
{
    // try to find the nearest granary to the sprite
    Building* bui = findNearestGranary(false);
    
    // if the granary has been found and has food, go to eat it!
    if(bui != NULL && bui->currentStorage > 0)
    {
        if(!bui->isUnderConstruction())
        {
            CCPoint startPos = getWorldPosition();
            CCPoint endPos = bui->getWorldPosition();
            
            startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
            endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
            
            if(startPos.equals(endPos))
            {
                //saySpeech("Eating food aiya!", 5.0f);
                setTargetLocation(bui);
                currAction = EATING;
            }
            else
            {
                saySpeech(HUNGRY, 5.0f);
                setTargetLocation(bui);
                GoEat(bui);
            }
            isDoingJob = false;
        }
    }
    // if the granary does not have food, do next scheduled action.
    else if(bui != NULL)
    {
        saySpeech("No food store with food!", 5.0f);
        if(getJob() == NONE && getJobLocation() == NULL)
        {
            if(futureAction1 == RESTING || futureAction2 == RESTING)
            {
                futureAction1 = RESTING;
                futureAction2 = IDLE;
            }
            else
            {
                futureAction1 = futureAction2 = IDLE;
            }
        }
    }
    // if there is no granary found, do next scheduled action.
    else
    {
        saySpeech("No food store!", 5.0f);
        if(futureAction1 == RESTING || futureAction2 == RESTING)
        {
            futureAction1 = RESTING;
            futureAction2 = IDLE;
        }
        else
        {
            futureAction1 = futureAction2 = IDLE;
        }
    }
}

void GameSprite::goToSleep()
{
    if(getHome() == NULL)
    {
        return;
    }
    else
    {
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = getHome()->getWorldPosition();
        
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
        
        if(startPos.equals(endPos))
        {
            saySpeech("Sleep loh!", 5.0f);
            setTargetLocation(getHome());
            currAction = RESTING;
        }
        else
        {
            saySpeech(TIRED, 5.0f);
            setTargetLocation(getHome());
            GoRest(getHome());
        }
        isDoingJob = false;
    }
    futureAction1 = IDLE;
    futureAction2 = IDLE;
}

void GameSprite::goToOccupyHome(Building* b)
{
    if(getHome() != NULL || b == NULL)
    {
        return;
    }
    else
    {
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = b->getWorldPosition();
        
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
        
        if(startPos.equals(endPos))
        {
            saySpeech("Yeh! I have a home!", 5.0f);
            setHome(b);
            changeToCitizen();
        }
        else
        {
            saySpeech("I want to find a home!", 5.0f);
            setTargetLocation(b);
            GoHome(b);
        }
        isDoingJob = false;
    }
}

bool GameSprite::hasEmptyHouse()
{
    CCArray* allHousing = GameScene::getThis()->buildingHandler->housingOnMap;
    for(int i = 0; i < allHousing->count(); i++)
    {
        Building* b = (Building*)allHousing->objectAtIndex(i);
        if(b->memberSpriteList->count() < b->populationLimit)
        {
            return true;
        }
    }
    return false;
}

bool GameSprite::findNearestHome()
{
    CCArray* allHousing = GameScene::getThis()->buildingHandler->housingOnMap;
    Building* nearestHome = NULL;
    int distance = 999999;
    
    for(int i = 0; i < allHousing->count(); i++)
    {
        Building* b = (Building*)allHousing->objectAtIndex(i);
        
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = b->getWorldPosition();
        
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
        
        int tempDistance = getPathDistance(startPos, endPos);
        
        if(tempDistance < distance && b->memberSpriteList->count() < b->populationLimit)
        {
            distance = tempDistance;
            nearestHome = b;
        }
    }
    
    if(nearestHome != NULL)
    {
        possessions->targetHomeLocation = nearestHome;
        goToOccupyHome(nearestHome);
    }
    
    return true;
}

bool GameSprite::hasValidTarget()
{
    Building* nearestTarget;
    if(current_money_rob < GameScene::getThis()->settingsLevel->max_money_rob && current_food_rob < GameScene::getThis()->settingsLevel->max_food_rob)
    {
        // check either food or money
        nearestTarget = checkTarget(1);
    }
    else if(current_money_rob < GameScene::getThis()->settingsLevel->max_money_rob)
    {
        // check money
        nearestTarget = checkTarget(3);
    }
    else
    {
        // check food
        nearestTarget = checkTarget(2);
    }
    
    if (nearestTarget != NULL)
    {
        possessions->targetLocation = nearestTarget;
        nextAction = ROB;
        if(nearestTarget->buildingType == SPECIAL)
        {
            saySpeech("Your money is mine!", 5.0f);
        }
        else
        {
            saySpeech("I want to get food free!", 5.0f);
        }
        GoBuilding(nearestTarget);
        return true;
    }
    else
    {
        return false;
    }
}

Building* GameSprite::checkTarget(int mode)
{
    CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
    CCArray* allSpecial = GameScene::getThis()->buildingHandler->specialOnMap;
    
    Building* nearestTarget = NULL;
    int distance = 999999;
    
    // mode 1: check all
    if(mode == 1)
    {
        for (int i = 0; i < (allGranary->count() + allSpecial->count()); i++)
        {
            Building* b;
            if(i < allGranary->count())
            {
                b = (Building*) allGranary->objectAtIndex(i);
            }
            else
            {
                b = (Building*) allSpecial->objectAtIndex(i - allGranary->count());
            }
            
            int tempDistance = getDistance(b);
            
            if(tempDistance < distance && ((b->currentStorage > 0 && b->buildingType == GRANARY) || (b->buildingType == SPECIAL && GameHUD::getThis()->money > 0)))
            {
                distance = tempDistance;
                nearestTarget = b;
            }
        }
    }
    else if(mode == 2)
    // mode 2: check the granary;
    {
        for (int i = 0; i < allGranary->count(); i++)
        {
            Building* b = (Building*) allGranary->objectAtIndex(i);
            
            int tempDistance = getDistance(b);
            
            if(tempDistance < distance && b->currentStorage > 0)
            {
                distance = tempDistance;
                nearestTarget = b;
            }
        }
    }
    else
    // in other cases: check the town hall
    {
        for (int i = 0; i < allSpecial->count(); i++)
        {
            Building* b = (Building*) allSpecial->objectAtIndex(i);
            
            int tempDistance = getDistance(b);
            
            if(tempDistance < distance && GameHUD::getThis()->money > 0)
            {
                distance = tempDistance;
                nearestTarget = b;
            }
        }
    }
    
    return nearestTarget;
}

int GameSprite::getDistance(Building* b)
{
    CCPoint startPos = getWorldPosition();
    CCPoint endPos = b->getWorldPosition();
    
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    return getPathDistance(startPos, endPos);
}


void GameSprite::updateEndurance(float dt)
{
    /*
     * when endurance changed, update the new endurance for the sprite
     */
    if (mGameCurrentEndurance != possessions->current_endurance || mGameMaxEndurance != possessions->max_endurance)
    {
        mGameCurrentEndurance = possessions->current_endurance;
        mGameMaxEndurance = possessions->max_endurance;
        barHP->setValue((float) mGameCurrentEndurance / (float) mGameMaxEndurance);
    }
    
    if (mGameWarMode != GameScene::getThis()->banditsAttackHandler->warMode)
    {
        mGameWarMode = GameScene::getThis()->banditsAttackHandler->warMode;
        if(villagerClass == V_SOLDIER)
        {
            if(mGameWarMode){
                stringstream ss;
                ss << possessions->current_endurance;
             //   CCLog(ss.str().c_str());
                barHP->setVisible(true);
            } else {
                stringstream ss;
                ss << possessions->current_endurance;
              //  CCLog(ss.str().c_str());
                //barHP->setVisible(false);
            }
        }
    }

}

void GameSprite::updateHungry(float dt)
{
    /*
     * if the sprite is not hungry (currentHungry > 0), do nothing.
     */
    if(possessions->currentHungry > 0.0)
    {
        return;
    }
    
    cumulativeTime_happiness += dt;
    float factor = 1.0f;
    if (villagerClass == V_REFUGEE)
    {
        factor = 1.0f / GameScene::getThis()->configSettings->default_homeless_happiness_drop_multiplier;
    }
    /*
     * if the sprite is hungry (currentHungry <= 0), drop happiness!
     */
    if(possessions->happinessRating >= 70.0f)
    {
        // the sprite is in happy state
        if (cumulativeTime_happiness >= 1.0f / (GameScene::getThis()->settingsLevel->hungry_happiness_happy_decay / ((float) GameScene::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else if(possessions->happinessRating >= 40.0f)
    {
        // the sprite is in normal state
        if (cumulativeTime_happiness >= 1.0f / (GameScene::getThis()->settingsLevel->hungry_happiness_normal_decay / ((float) GameScene::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else if(possessions->happinessRating >= 10.0f)
    {
        // the sprite is in unhappy state
        if (cumulativeTime_happiness >= 1.0f / (GameScene::getThis()->settingsLevel->hungry_happiness_unhappy_decay / ((float) GameScene::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else
    {
        // the sprite is in angry state
        if (cumulativeTime_happiness >= 1.0f / (GameScene::getThis()->settingsLevel->hungry_happiness_angry_decay / ((float) GameScene::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    
    if(possessions->happinessRating < 0)
    {
        possessions->happinessRating = 0;
        // when energy <= 0, the villager will leave the village.
    }
}

void GameSprite::scheduleToken(float dt)
{
    if(is_token_drop_cooldown)
    {
        checkDropTresholdTime();
        token_drop_cooldown_time += dt;
        if(token_drop_cooldown_time >= token_drop_cooldown_treshold)
        {
            is_token_drop_cooldown = false;
        }
    }
    else
    {
        
        token_drop_cooldown_time = 0;
        is_token_drop_cooldown = true;
        
        if (villagerClass != V_BANDIT && villagerClass != V_SOLDIER && villagerClass != V_CLASS_END)
        {
            dropToken(false);
        }
    }
}

void GameSprite::checkDropTresholdTime()
{
    float mAverageHappiness = possessions->happinessRating;
    int timeDiffer = 0;
    if(mAverageHappiness >= 70)
    {
        timeDiffer = GameScene::getThis()->configSettings->token_drop_treshold_time_happy_max - GameScene::getThis()->configSettings->token_drop_treshold_time_happy_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = GameScene::getThis()->configSettings->token_drop_treshold_time_happy_min + randomTime;
    }
    else if(mAverageHappiness >= 40)
    {
        timeDiffer = GameScene::getThis()->configSettings->token_drop_treshold_time_normal_max - GameScene::getThis()->configSettings->token_drop_treshold_time_normal_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = GameScene::getThis()->configSettings->token_drop_treshold_time_normal_min + randomTime;
    }
    else if(mAverageHappiness >= 10)
    {
        timeDiffer = GameScene::getThis()->configSettings->token_drop_treshold_time_unhappy_max - GameScene::getThis()->configSettings->token_drop_treshold_time_unhappy_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = GameScene::getThis()->configSettings->token_drop_treshold_time_unhappy_min + randomTime;
    }
    else
    {
        timeDiffer = GameScene::getThis()->configSettings->token_drop_treshold_time_angry_max - GameScene::getThis()->configSettings->token_drop_treshold_time_angry_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = GameScene::getThis()->configSettings->token_drop_treshold_time_angry_min + randomTime;
    }
}

void GameSprite::dropToken(bool tutorial)
{
    checkDropRate();
    
    int random_number = rand() % 100 + 1;
    if(random_number <= token_drop_rate || tutorial)
    {
        saySpeech(HAPPY, 1.0f);
        ReputationOrb* ro = ReputationOrb::create("REN", GameScene::getThis()->configSettings->token_disappear_time);
        CCSprite* newToken = ro->getSprite();
        newToken->setAnchorPoint(ccp(0.5, 0.5));
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        CCSize spriteSize = newToken->getContentSize();
        
        newToken->setScale(screenSize.width / spriteSize.width * 0.05f);
        
        GameScene::getThis()->spriteHandler->tokensOnMap->addObject(ro);
        
        GameScene::getThis()->mapHandler->getMap()->addChild(newToken, 99999);
        
        CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(&currPos);
        
        newToken->setPosition(target);
    }
}

void GameSprite::checkDropRate()
{
    float mAverageHappiness = GameHUD::getThis()->average_happiness;
    if(mAverageHappiness >= 70)
    {
        token_drop_rate = GameScene::getThis()->configSettings->token_drop_rate_happy;
    }
    else if(mAverageHappiness >= 40)
    {
        token_drop_rate = GameScene::getThis()->configSettings->token_drop_rate_normal;
    }
    else if(mAverageHappiness >= 10)
    {
        token_drop_rate = GameScene::getThis()->configSettings->token_drop_rate_unhappy;
    }
    else
    {
        token_drop_rate = GameScene::getThis()->configSettings->token_drop_rate_angry;
    }
}

bool GameSprite::escape()
{
    CCPoint target = CCPointMake(29,33);
    if(GoLocation(target, true))
    {
        isLeavingNextUpdate = true;
        return true;
    }
    return false;
}

void GameSprite::damaged(int damage)
{
    stringstream ss;
    if(possessions->current_endurance < damage)
    {
        ss << possessions->current_endurance;
    }
    else
    {
        ss << damage;
    }
    
    CCLabelTTF* damageLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    damageLabel->setAnchorPoint(ccp(0, 1));
    damageLabel->setPosition(ccp(0, spriteRep->boundingBox().size.height * 1.1f));
    spriteRep->addChild(damageLabel);
    hpLabels->addObject(damageLabel);
    
    possessions->current_endurance -= damage;
    
    if(possessions->current_endurance < 0)
    {
        possessions->current_endurance = 0;
        tryEscape = true;
        combatState = C_ESCAPE;
        if(enermy != NULL)
        {
            if(enermy->enermy != NULL)
            {
                enermy->enermy = NULL;
            }
            enermy = NULL;
        }
    }
}

bool GameSprite::hasBandit(CCArray* spritesOnMap, CCPoint checkTile)
{
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        CCPoint gsTile = GameScene::getThis()->mapHandler->locationFromTilePos(&checkTile);
        
        if(gs->villagerClass == V_BANDIT && gs->combatState == C_COMBAT && gs->enermy == this)
        {
            if(gsTile.x == gs->spriteRep->getPosition().x && gsTile.y == gs->spriteRep->getPosition().y)
            //if(GlobalHelper::compareCCPoint(gsTile, gs->currPos))
            {
                return true;
            }
        }
    }
    return false;
}

bool GameSprite::hasSoldier(CCArray* spritesOnMap, CCPoint checkTile)
{
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        CCPoint gsTile = GameScene::getThis()->mapHandler->locationFromTilePos(&checkTile);
        
        if(gs->villagerClass == V_SOLDIER && gs->combatState == C_COMBAT && gs->enermy == this)
        {
            if(gsTile.x == gs->spriteRep->getPosition().x && gsTile.y == gs->spriteRep->getPosition().y)
            //if(GlobalHelper::compareCCPoint(gsTile, gs->currPos))
            {
                return true;
            }
        }
    }
    return false;
}

void GameSprite::updatePath(Building* b)
{
    lastTarget = b;
    
    CCPoint startPos = getWorldPosition();
    CCPoint endPos = b->getWorldPosition();
    
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
    }
}

void GameSprite::updatePath(CCPoint endPos)
{
    CCPoint startPos = getWorldPosition();
    startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
    }
}

bool GameSprite::isFarmer()
{
    if(villagerClass == V_FARMER)
    {
        return true;
    }
    return false;
}

bool GameSprite::isRefugee()
{
    if(villagerClass == V_REFUGEE)
    {
        return true;
    }
    return false;
}

bool GameSprite::isCitizen()
{
    if(villagerClass == V_CITIZEN)
    {
        return true;
    }
    return false;
}

bool GameSprite::isBuilder()
{
    if(villagerClass == V_BUILDER)
    {
        return true;
    }
    return false;
}

bool GameSprite::isSoldier()
{
    if(villagerClass == V_SOLDIER)
    {
        return true;
    }
    return false;
}

bool GameSprite::isBandit()
{
    if(villagerClass == V_BANDIT)
    {
        return true;
    }
    return false;
}

string GameSprite::getCurrentDir()
{
    return currentDir;
}