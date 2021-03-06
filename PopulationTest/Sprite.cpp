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
#include "GameDefaults.h"
#include "NameGenerator.h"
#include "BuildingHandler.h"
#include "GlobalHelper.h"
#include "ReputationOrb.h"
#include "BattleIcon.h"
#include "TutorialManager.h"
#include "SoundtrackManager.h"
#include "UserProfile.h"

#include <cmath>
#include <sstream>

GameSprite::GameSprite()
{
    spriteW = 128;
    spriteH = 128;
    currAction = IDLE;
    nextAction = IDLE;
    
    futureAction1 = IDLE;
    futureAction2 = IDLE;
    
    config_doc = "";
    shouldStopNextSquare = false;
    speechBubble = NULL;
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
    barHP->createProgressBar(CCRectMake(0, 0, 76, 16),
                             CCRectMake(3, 3, 70, 10),
                             "Energy_brown bar.png",
                             "NONE",
                             "NONE",
                             "Energybarblue.png", true);
    barHP->setAnchorPoint(ccp(0.5, 0.5));
    
    hpLabels = CCArray::create();
    hpLabels->retain();
    
    combatState = C_IDLE;
    
    isMoving = false;
    
    enermy = NULL;
    preEnermy = NULL;
    
    currentTile = CCPointZero;
    
    movementSpeed = 0;
    
    isInAttackAction = false;
    frameWidth = 64;
    frameHeight = 64;
    currentFrameNumber = 0;
    maxFrameNumber = 0;
    xOffset = 0;
    yOffset = 0;
    
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    isInCombat = false;
    justMoveOneTile = false;
    
    battleIconArray = CCArray::create();
    battleIconArray->retain();
    
    goToTownHall = true;
    
    hasAssigned = false;
    cumulativeCheckTime = 0;
    
    targetHungry = 0;
    targetEnergy = 0;
    targetHappiness = 0;
    
    uniqueID = -1;
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
    movementSpeed = getPossessions()->default_movement_speed;
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
    possessions->default_work_rate = UserProfile::getThis()->configSettings->default_work_rate;
    
    possessions->happinessRating = UserProfile::getThis()->configSettings->default_hapiness;
    possessions->loyaltyRating = UserProfile::getThis()->configSettings->default_loyalty;
    
    possessions->currentHungry = UserProfile::getThis()->configSettings->default_current_hungry;
    possessions->energyRating = UserProfile::getThis()->configSettings->default_current_energy;
    
    possessions->default_hapiness_limit = UserProfile::getThis()->configSettings->default_hapiness_limit;
    possessions->default_loyalty_limit = UserProfile::getThis()->configSettings->default_loyalty_limit;
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
        pCopy->baseSpriteName = this->baseSpriteName;
        pCopy->spriteClass = this->spriteClass;
        pCopy->currAction = IDLE;
        pCopy->config_doc = this->config_doc;
        pCopy->defaults_doc = this->defaults_doc;
        pCopy->gender = this->gender;
        pCopy->race = this->race;
        pCopy->villagerClass = this->villagerClass;
        pCopy->uniqueID = this->uniqueID;
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
     CCPoint target = MapHandler::getThis()->locationFromTilePos(tilePos);
 
     spriteRep->setPosition(target);
     
     setAction(IDLE);
     idleDelay = 0.0f;
     
     initAI(false);
     
     changeAnimation("DL");
     currentDir = "DL";
     
     // Progressive bar
     barHP->setValue((float) getPossessions()->current_endurance / (float) getPossessions()->max_endurance);
     barHP->setPosition(ccp(spriteRep->boundingBox().size.width * 1.0f, spriteRep->boundingBox().size.height * 2.0f));
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
     
     speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 1.2f,
                                   spriteRep->boundingBox().size.height * 1.5f));
     spriteRep->addChild(speechBubble);
     
     BattleIcon* battleIcon = BattleIcon::create(this);
     battleIconArray->addObject(battleIcon);

     MapHandler::getThis()->getMap()->addChild(spriteRep,
                                                          MapHandler::getThis()->calcZIndex(*tilePos));
     //speechBubble->autorelease();
     
     behaviorTree->onInitialize();
     behaviorTree->update();
     
     spriteDisplayedName = NameGenerator::GenerateName(race, gender);

     currPos = getWorldPosition();
     currPos = MapHandler::getThis()->tilePosFromLocation(currPos);
     
     currTile = MapHandler::getThis()->getTileAt(currPos.x, currPos.y);
 }

void GameSprite::unmakeSpriteEndGame()
{
    spriteRep->stopAllActions();
    
    delete speechBubble;
    battleIconArray->removeAllObjects();
    CC_SAFE_RELEASE(battleIconArray);
    
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
    
    if (MapHandler::getThis()->getMap()->getChildren()->containsObject(spriteRep))
    {
        MapHandler::getThis()->getMap()->removeChild(spriteRep);
    }
    
    delete speechBubble;
    battleIconArray->removeAllObjects();
    CC_SAFE_RELEASE(battleIconArray);
    
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
    
    bool isCombatingSprite = false;
    if(villagerClass == V_SOLDIER || villagerClass == V_BANDIT)
    {
        isCombatingSprite = true;
    }
    
    path = p->makePath(&from, &to, isCombatingSprite);

    if (path->count() == 0)
    {
        //CCLog("Warning! no path from %f, %f to %f, %f - next closest node used", from.x, from.y, to.x, to.y);
        saySpeech(STUCK, 2.0f);
        path = p->makePath(&from, &p->closest, isCombatingSprite);
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
        
        bool isCombatingSprite = false;
        if(villagerClass == V_SOLDIER || villagerClass == V_BANDIT)
        {
            isCombatingSprite = true;
        }
        
        tempPath = p->makePath(&from, &to, isCombatingSprite);
        
        int result = tempPath->count();
        
        tempPath->removeAllObjects();
        tempPath->release();
        
        return result;
    }
}

int GameSprite::getDistance(CCPoint from, CCPoint to)
{
    return (abs(from.x - to.x) + abs(from.y - to.y));
}

void GameSprite::followPath(bool moveOneTile)
{
    isMoving = true;
    justMoveOneTile = moveOneTile;
    
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
            MapTile* tile = MapHandler::getThis()->getTileAt(node->tilepos.x, node->tilepos.y);
            if (tile->hasBuilding())
            {
                if (spriteRep != NULL && villagerClass != V_BANDIT && villagerClass != V_SOLDIER)
                {
                    spriteRep->setVisible(false);
                }
            }
            else
            {
                if (isInBuilding)
                {
                    CCPoint spritePos = getWorldPosition();
                    spritePos = MapHandler::getThis()->tilePosFromLocation(spritePos);
                    MapTile* tile = MapHandler::getThis()->getTileAt(spritePos.x, spritePos.y );
                    
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
            CCPoint nextPos = MapHandler::getThis()->locationFromTilePos(&(node->tilepos));
            this->nextTile = nextPos;
            
            if(!isInCombat || true)
            {
                // if the soldier's next target tile is already taken by the bandits, stop moving and prepare to attack the bandit
                if(villagerClass == V_SOLDIER && !tryEscape){
                    CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
                    CCPoint myTile = MapHandler::getThis()->locationFromTilePos(&(currPos));
                    
                    for(int i = 0; i < allSprites->count(); i++){
                        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                        if(gs->villagerClass == V_BANDIT && gs->combatState == C_IDLE && !gs->tryEscape){
                            CCPoint gsTile = MapHandler::getThis()->locationFromTilePos(&(gs->currPos));
                            if(GlobalHelper::compareCCPoint(gs->nextTile, nextTile) || GlobalHelper::compareCCPoint(nextTile, gsTile) || GlobalHelper::compareCCPoint(gs->nextTile, myTile) || GlobalHelper::compareCCPoint(myTile, gsTile))
                            {
                                if(!gs->isInCombat)
                                {
                                    //nextTile = currentTile;
                                    stopAction = true;
                                    gs->stopAction = true;
                                    isMoving = false;
                                    preEnermy = gs;
                                    
                                    //BattleManager::getThis()->addBattlePair(this, gs);
                                    
                                    return;
                                }
                            }
                        }
                    }
                }
                
                // if the bandit's next target tile is already taken by the soldiers, stop moving and prepare to attack the soldier
                if(villagerClass == V_BANDIT && !tryEscape){
                    CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
                    CCPoint myTile = MapHandler::getThis()->locationFromTilePos(&(currPos));
                    
                    for(int i = 0; i < allSprites->count(); i++){
                        // get the location of that
                        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
                        if(gs->villagerClass == V_SOLDIER && gs->combatState == C_IDLE && !gs->tryEscape){
                            CCPoint gsTile = MapHandler::getThis()->locationFromTilePos(&(gs->currPos));
                            // if next tile has been pre-assigned by a soldier or the next tile has a soldier
                            if(GlobalHelper::compareCCPoint(gs->nextTile, nextTile) || GlobalHelper::compareCCPoint(nextTile, gsTile) || GlobalHelper::compareCCPoint(myTile, gs->nextTile) || GlobalHelper::compareCCPoint(myTile, gsTile))
                            {
                                if(!gs->isInCombat)
                                {
                                    //nextTile = currentTile;
                                    isMoving = false;
                                    stopAction = true;
                                    gs->stopAction = true;
                                    preEnermy = gs;
                                    
                                    //BattleManager::getThis()->addBattlePair(gs, this);
                                    
                                    return;
                                }
                            }
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
    
    for(int i = 0; i < battleIconArray->count(); i++)
    {
        BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(i);
        bi->adjustPosition();
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
        
        updateZIndex();
    }
    else if (diff.x < 0 && diff.x < 0)
    {
        changeAnimation("DL");
        
        updateZIndex();
    }

    callback = CCCallFuncN::create(pSender, callfuncN_selector(GameSprite::moveComplete));
    callback->retain();
    spriteRunAction = CCSequence::createWithTwoActions(CCMoveBy::create(50.0f / movementSpeed, diff), callback);

    spriteRep->runAction(spriteRunAction);
    
    CC_SAFE_RELEASE(callback);

}

void GameSprite::moveComplete(cocos2d::CCObject *pSender)
{
    // Because moveComplete is called instantly after sequential action and
    // updateSprite is called at intervals of approx 16ms,
    // we need to calculate the newest currPos right now.
    currPos = getWorldPosition();
    currPos = MapHandler::getThis()->tilePosFromLocation(currPos);
    
    currTile = MapHandler::getThis()->getTileAt(currPos.x, currPos.y);
    
    // after calculate the newest currPos, update the z order of the sprite
    updateZIndex();
    
    // each time after moveComplete has been triggered, the soldier will check the bandit's position again, so that they can adjust their path to the capture the bandit.
    if (villagerClass == V_SOLDIER)
    {
        CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
        
        CCPoint nearestTarget = CCPointZero;
        int nearestDistance = 999999;
        
        // get the current position of the soldier
        CCPoint sPos = getWorldPosition();
        sPos = MapHandler::getThis()->tilePosFromLocation(sPos);
        
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            
            // check the bandits, get the nearest bandit to the soldier, as well as the distance.
            if (gs->villagerClass == V_BANDIT && gs->possessions->current_endurance > 0 && !gs->tryEscape)
            {
                CCPoint bPos = gs->getWorldPosition();
                bPos = MapHandler::getThis()->tilePosFromLocation(bPos);
                
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
                GoLocation(nearestTarget);
                isMoving = false;
                return;
            }
        }
    }

    if(justMoveOneTile)
    {
        isMoving = false;
        return;
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
    if(GameHUD::getThis()->pause)
    {
        return false;
    }
    
    if(TutorialManager::getThis()->active)
    {
        if(villagerClass != V_BANDIT && villagerClass != V_CLASS_END && goToTownHall)
        {
            goToTownHall = false;
            Building* th = (Building*) BuildingHandler::getThis()->specialOnMap->objectAtIndex(0);
            hasAssigned = true;
            setTargetLocation(th);
            GoBuilding(th);
            return true;
        }
    }
    
    if(hasAssigned)
    {
        saySpeech(STUCK, 5.0f);
        return true;
    }
    
    // if the sprite is in combat, the combat manager will take over to control the sprite.
    if(isInCombat)
    {
        return true;
    }
    
    if(tryEscape)
    {
        return true;
    }
    
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
    
    wanderFlag = !wanderFlag;
    if (wanderFlag)
    {
        if(villagerClass == V_REFUGEE)
        {
            saySpeech(HOMELESS, 5.0f);
        }
        else
        {
            saySpeech(IDLING, 5.0f);
        }
        
        CCPoint tgt = MapHandler::getThis()->getRandomPathTile();//CCPointMake( rand() % 40, rand() % 40);
        if (tgt.x == -1 && tgt.y == -1)
        {
            setAction(IDLE); //no path, can't
            return false;
        }
        
        CCPoint startPos = getWorldPosition();
        startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
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

void GameSprite::updateSprite(float dt)
{
    // updateZIndex();
    /* if one villager has been assigned to one target building and he cannot reach there, keep trying to there before he finally reach the target */
    if(hasAssigned)
    {
        if(currAction == IDLE || currAction == GET_HOME)
        {
            cumulativeCheckTime += dt;
            if(cumulativeCheckTime > 1)
            {
                if(getTargetLocation() != NULL)
                {
                    GoBuilding(getTargetLocation());
                }
                cumulativeCheckTime = 0;
            }
        }
        return;
    }
    
    /* In attack, change the animation sprite of bandits and soldiers */
    if(isInAttackAction)
    {
        if (delay_curr > 0)
        {
            delay_curr -= dt;
        }
        else
        {
            currentFrameNumber++;
            if (currentFrameNumber >= maxFrameNumber)
            {
                currentFrameNumber = 0;
                isInAttackAction = false;
            }
            
            xOffset = currentFrameNumber % 4;
            yOffset = currentFrameNumber / 4;
            
            spriteRect.setRect(xOffset * frameWidth, yOffset * frameHeight, frameWidth, frameHeight);
            spriteRep->setTextureRect(spriteRect);
            
            delay_curr = delay_animFrame;
        }
    }
    
    lastFrameAction = currAction;
    
    /* a bandit will try to attack the village when he is in idle state (always) */
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
    
    // in combat mode, the soldiers and bandits will follow own strategic plan
    if(BanditsAttackHandler::getThis()->warMode)
    {
        if(combatState == C_COMBAT)
        {
            // bandit is the fighting controller.
            if(villagerClass == V_BANDIT)
            {
                if(enermy != NULL && enermy->enermy == this && enermy->combatState == C_COMBAT)
                {
                    CCPoint myPos = this->getWorldPosition();
                    myPos = MapHandler::getThis()->tilePosFromLocation(myPos);
                    
                    CCPoint enermyPos = this->getWorldPosition();
                    enermyPos = MapHandler::getThis()->tilePosFromLocation(enermyPos);
                    
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
                        enermy->stopAction = false;
                        enermy->followPath();
                        
                        this->enermy = NULL;
                        this->combatState = C_IDLE;
                        this->currAction = IDLE;
                        this->stopAction = false;
                        this->followPath();
                        return;
                    }
                }
                else if(enermy == NULL || (enermy != NULL && enermy->enermy != this))
                {
                    this->enermy = NULL;
                    this->combatState = C_IDLE;
                    this->currAction = IDLE;
                    this->stopAction = false;
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
                CCArray* allSpritesOnMap = SpriteHandler::getThis()->spritesOnMap;
                
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
                        // no enermy in all the directions, resume find enermy.
                        currAction = IDLE;
                        combatState = C_IDLE;
                        stopAction = false;
                        if(enermy != NULL)
                        {
                            enermy->currAction = IDLE;
                            enermy->combatState = C_IDLE;
                            enermy->stopAction = false;
                            enermy->followPath();
                            enermy = NULL;
                        }
                        this->followPath();
                        return;
                    }
                }
            }
            
            if ((villagerClass == V_BANDIT) && !tryEscape)
            {
                CCArray* allSpritesOnMap = SpriteHandler::getThis()->spritesOnMap;
                
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
                        // not enermy in all directions, resume the finding enermy process
                        stopAction = false;
                        currAction = IDLE;
                        combatState = C_IDLE;
                        if(enermy != NULL)
                        {
                            enermy->stopAction = false;
                            enermy->currAction = IDLE;
                            enermy->combatState = C_IDLE;
                            enermy->followPath();
                        }
                        this->followPath();
                        return;
                    }
                }
            }
            
            // only soldiers and bandits are involved in the combat.
            if((villagerClass == V_SOLDIER || villagerClass == V_BANDIT) && !tryEscape && combatState == C_COMBAT)
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
                
                if(villagerClass == V_BANDIT)
                {
                    if(battleIconArray->count() > 0)
                    {
                        BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(0);
                        bi->update(dt);
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
                        
                        playAttackAction();
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
                bPos = MapHandler::getThis()->tilePosFromLocation(bPos);
                
                // check whether the bandits meet the soldiers
                CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
                
                bool stop = false;
                GameSprite* opponent = NULL;
                // check all the soldiers, if any free soldier is adjacent to the free bandit and the bandit is not trying to escape, stop moving and prepare to attack the soldier
                for (int i = 0; i < spritesOnMap->count(); i++)
                {
                    GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
                    
                    if (gs->villagerClass == V_SOLDIER && gs->combatState == C_IDLE && !gs->tryEscape)
                    {
                        CCPoint sPos = gs->getWorldPosition();
                        sPos = MapHandler::getThis()->tilePosFromLocation(sPos);
                        int tempDistance = getDistance(currPos, sPos);
                        if(tempDistance <= 1 && !tryEscape && gs->combatState == C_IDLE && gs->enermy == NULL)
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
                        
                        if(battleIconArray->count() > 0)
                        {
                            BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(0);
                            bi->show();
                        }
                        
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
            }
            
            if (villagerClass == V_SOLDIER)
            {
                if(currAction == IDLE)
                {
                    GoBuilding(getJobLocation());
                }
            }
        }
    }
    else
    {
        if(villagerClass == V_SOLDIER)
        {
            if(stopAction)
            {
                stopAction = false;
                GoBuilding(possessions->jobLocation);
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
            SpriteHandler::getThis()->removeSpriteFromMap(this);
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
    
    if(villagerClass != V_BANDIT && villagerClass != V_SOLDIER)
    {
        //spriteRep->setZOrder( MapHandler::getThis()->calcZIndex(currPos, 0, true, this) );
        //speechBubble->setZOrder( MapHandler::getThis()->calcZIndex(currPos, 0, true, this) );
        spriteRep->setZOrder( 99999 );
        speechBubble->setZOrder( 99999 );
    }
    else
    {
        spriteRep->setZOrder( 99999 );
        speechBubble->setZOrder( 99999 );
    }
    /*
    for(int i = 0; i < battleIconArray->count(); i++)
    {
        BattleIcon* battleIcon = (BattleIcon*) battleIconArray->objectAtIndex(i);
        if(currentDir == "UL" || currentDir == "UR")
        {
            battleIcon->battleSprite->setZOrder( GameScene::getThis()->mapHandler->calcZIndex(currPos, 0, true, this) - 2 );
        }
        else
        {
            battleIcon->battleSprite->setZOrder( GameScene::getThis()->mapHandler->calcZIndex(currPos, 0, true, this) );
        }
    }
    */
}

CCPoint GameSprite::getWorldPosition()
{
    if (!spriteRep) return CCPointMake(-1,-1);
    //good thing the scale's on another layer, so I can safely do this now.
    
    return ccpAdd(spriteRep->getPosition(), MapHandler::getThis()->getMap()->getPosition());
    
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
    speechBubble->show(timeInSeconds);
}


void GameSprite::saySpeech(SpeechMood s, float timeInSeconds)
{
    speechBubble->clearContent();
    
    CCSprite* label = NULL;
    bool show = true;
    
    switch (s)
    {
        case IDLING:
            speechBubble->addContent("emotionicon_anima.png", CCPointZero, 1, 4, 1);
            show = false;
            break;
        case HAPPY:
            speechBubble->addContent("emotionicon_anima.png", CCPointZero, 3, 4, 0);
            show = false;
            break;
        case HUNGRY:
            speechBubble->addContent("emotionicon_anima.png", CCPointZero, 4, 4, 14);
            show = false;
            break;
        case TIRED:
            speechBubble->addContent("emotionicon_anima.png", CCPointZero, 3, 4, 5);
            show = false;
            break;
        case UNHAPPY:
            speechBubble->addContent("emotionicon_anima.png", CCPointZero, 2, 4, 18);
            show = false;
            break;
        case STUCK:
            speechBubble->addContent("unabletogo.png", CCPointZero, 10, 4, 0);
            show = false;
            break;
        case STUCK_FOOD:
            speechBubble->addContent("nofood_64.png", CCPointZero, 10, 4, 0);
            show = false;
            break;
        case FIND_HOME:
            label = CCSprite::create("idle2.png");
            
            break;
        case HOMELESS:
            speechBubble->addContent("homeless_anim.png", CCPointZero, 10, 4, 0);
            show = false;
            break;
        case FIND_BANDIT:
            speechBubble->addContent("findingbandit.png", CCPointZero, 10, 4, 0);
            show = false;
            break;
        case BATTLE:
            speechBubble->addContent("defending.png", CCPointZero, 16, 4, 0);
            show = false;
            break;
        case GUARD_EMOTION:
            speechBubble->addContent("defending.png", CCPointZero, 16, 4, 0);
            show = false;
            break;
        case BUILDER_EMOTION:
            speechBubble->addContent("building_safety helmet2.png", CCPointZero, 5, 2, 0);
            show = false;
            break;
        case FARMER_EMOTION:
            speechBubble->addContent("goingToWork.png", CCPointZero, 4, 2, 0);
            show = false;
            break;
        case TRANSPORT_EMOTION:
            speechBubble->addContent("emotionicon_anima.png", CCPointZero, 2, 4, 3);
            show = false;
            break;
        case STEAL_MONEY:
            speechBubble->addContent("thiefStealingMoney.png", CCPointZero, 14, 4, 0);
            show = false;
            break;
        case STEAL_FOOD:
            speechBubble->addContent("thiefStealingFood.png", CCPointZero, 13, 4, 0);
            show = false;
            break;
        default:
            break;
            
    }
    
    if(show){
        if (label == NULL) return;
        speechBubble->addContent(label, CCPointZero );
    }
    
    speechBubble->show(timeInSeconds * 2);
}

/* paths to a target building. */
bool GameSprite::GoBuilding(Building* b)
{
    lastTarget = b;

    CCPoint startPos = getWorldPosition();
    CCPoint endPos = b->getWorldPosition();
    
    startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
    endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
        followPath();
    }
    
    return hasPath;
}

/* paths to a target location. */
bool GameSprite::GoLocation(CCPoint endPos)
{
    CCPoint startPos = getWorldPosition();
    
    startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
    
    int tempDistance = getPathDistance(startPos, endPos);
    
    if(tempDistance <= 0)
    {
        return false;
    }
    
    bool hasPath = CreatePath(startPos, endPos);
    
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

void GameSprite::changeSpriteRepWhenLoadingGame()
{
    CCPoint pos = spriteRep->getPosition();
    speechBubble->retain();
    spriteRep->removeChild(speechBubble, false);
    
    for (int i = 0; i < battleIconArray->count(); i++)
    {
        BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(i);
        spriteRep->removeChild(bi->battleSprite, true);
    }
    battleIconArray->removeAllObjects();
    
    spriteRep->removeChild(barHP, true);
    delete barHP;
    
    MapHandler::getThis()->getMap()->removeChild(spriteRep);
    
    std::string initName = spriteName;
    initName+= "_IDL001.png";
    
    spriteRep = CCSprite::createWithSpriteFrameName(initName.c_str());
    spriteRep->setAnchorPoint(ccp(0.5, 0.75));
    spriteRep->setScale(0.5f);
    
    spriteRep->setPosition(pos);
    
    spriteRep->addChild(speechBubble);
    
    BattleIcon* battleIcon = BattleIcon::create(this);
    battleIconArray->addObject(battleIcon);
    
    barHP = new ProgressBar();
    barHP->createProgressBar(CCRectMake(0, 0, 76, 16),
                             CCRectMake(3, 3, 70, 10),
                             "Energy_brown bar.png",
                             "NONE",
                             "NONE",
                             "Energybarblue.png", true);
    barHP->setAnchorPoint(ccp(0.5, 0.5));
    
    barHP->setValue((float) getPossessions()->current_endurance / (float) getPossessions()->max_endurance);
    barHP->setPosition(ccp(spriteRep->boundingBox().size.width * 1.0f, spriteRep->boundingBox().size.height * 2.0f));
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
    
    MapHandler::getThis()->getMap()->addChild(spriteRep,
                                                         MapHandler::getThis()->calcZIndex(pos));
}

void GameSprite::ReplaceSpriteRep()
{
    CCPoint pos = spriteRep->getPosition();
    spriteRep->stopAllActions();
    
    delete behaviorTree;
    spriteRep->removeChild(speechBubble, true);
    delete speechBubble;
    for(int i = 0; i < battleIconArray->count(); i++)
    {
        BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(i);
        spriteRep->removeChild(bi->battleSprite, true);
    }
    battleIconArray->removeAllObjects();
    
    spriteRep->removeChild(barHP, true);
    delete barHP;
    
    MapHandler::getThis()->getMap()->removeChild(spriteRep);
    
    
    std::string initName = spriteName;
    initName+= "_IDL001.png";
    
    spriteRep = CCSprite::createWithSpriteFrameName(initName.c_str());
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
    
    
    speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 1.2f,
                                  spriteRep->boundingBox().size.height * 1.5f));
    
    spriteRep->addChild(speechBubble);
    
    BattleIcon* battleIcon = BattleIcon::create(this);
    battleIconArray->addObject(battleIcon);
    
    barHP = new ProgressBar();
    barHP->createProgressBar(CCRectMake(0, 0, 76, 16),
                             CCRectMake(3, 3, 70, 10),
                             "Energy_brown bar.png",
                             "NONE",
                             "NONE",
                             "Energybarblue.png", true);
    barHP->setAnchorPoint(ccp(0.5, 0.5));
    
    barHP->setValue((float) getPossessions()->current_endurance / (float) getPossessions()->max_endurance);
    barHP->setPosition(ccp(spriteRep->boundingBox().size.width * 1.0f, spriteRep->boundingBox().size.height * 2.0f));
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
    
    MapHandler::getThis()->getMap()->addChild(spriteRep,
                                                        MapHandler::getThis()->calcZIndex(pos));
    behaviorTree->onInitialize();
    behaviorTree->update();
     
   // updateZIndex();
  
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
    setJob(NONE);
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
    CCArray* buildingsOnMap = BuildingHandler::getThis()->allBuildingsOnMap;
    Building* nearestGranary = NULL;
    int distance = 999999;
    
    for(int i = 0; i < buildingsOnMap->count(); i++)
    {
        Building* bui = (Building*) buildingsOnMap->objectAtIndex(i);
        
        if(isDeliveringFood)
        {
            if(bui->buildingType != GRANARY || bui->currentStorage >= bui->storageLimit)
            {
                continue;
            }
        }
        else
        {
            if(bui->buildingType != GRANARY || bui->currentStorage <= 0)
            {
                continue;
            }
        }
        
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = bui->getWorldPosition();
        
        startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
        endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
        
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
            
            startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
            endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
            
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
        saySpeech(STUCK_FOOD, 5.0f);
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
        saySpeech(STUCK_FOOD, 5.0f);
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
        
        startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
        endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
        
        if(startPos.equals(endPos))
        {
            saySpeech(TIRED, 5.0f);
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
        
        startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
        endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
        
        if(startPos.equals(endPos))
        {
            saySpeech("Yeh! I have a home!", 5.0f);
            setHome(b);
            changeToCitizen();
        }
        else
        {
            saySpeech(HOMELESS, 5.0f);
            setTargetLocation(b);
            hasAssigned = true;
            GoHome(b);
        }
        isDoingJob = false;
    }
}

bool GameSprite::hasEmptyHouse()
{
    CCArray* allHousing = BuildingHandler::getThis()->housingOnMap;
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
    Building* nearestHome = findNearestEmptyHouse();
    
    if(nearestHome != NULL)
    {
        possessions->targetHomeLocation = nearestHome;
        goToOccupyHome(nearestHome);
    }
    
    return true;
}

Building* GameSprite::findNearestHouse()
{
    CCArray* allHousing = BuildingHandler::getThis()->housingOnMap;
    Building* nearestHome = NULL;
    int distance = 999999;
    
    for(int i = 0; i < allHousing->count(); i++)
    {
        Building* b = (Building*)allHousing->objectAtIndex(i);
        
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = b->getWorldPosition();
        
        startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
        endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
        
        int tempDistance = getDistance(startPos, endPos);
        
        if(tempDistance < distance)
        {
            distance = tempDistance;
            nearestHome = b;
        }
    }
    
    return nearestHome;
}

Building* GameSprite::findNearestEmptyHouse()
{
    CCArray* allHousing = BuildingHandler::getThis()->housingOnMap;
    Building* nearestHome = NULL;
    int distance = 999999;
    
    for(int i = 0; i < allHousing->count(); i++)
    {
        Building* b = (Building*)allHousing->objectAtIndex(i);
        
        CCPoint startPos = getWorldPosition();
        CCPoint endPos = b->getWorldPosition();
        
        startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
        endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
        
        int tempDistance = getDistance(startPos, endPos);
        
        if(tempDistance < distance && b->memberSpriteList->count() < b->populationLimit)
        {
            distance = tempDistance;
            nearestHome = b;
        }
    }
    
    return nearestHome;
}

bool GameSprite::hasValidTarget()
{
    Building* nearestTarget;
    if(current_money_rob < UserProfile::getThis()->settingsLevel->max_money_rob && current_food_rob < UserProfile::getThis()->settingsLevel->max_food_rob)
    {
        // check either food or money
        nearestTarget = checkTarget(1);
    }
    else if(current_money_rob < UserProfile::getThis()->settingsLevel->max_money_rob)
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
            saySpeech(STEAL_MONEY, 5.0f);
        }
        else
        {
            saySpeech(STEAL_FOOD, 5.0f);
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
    CCArray* allGranary = BuildingHandler::getThis()->granaryOnMap;
    CCArray* allSpecial = BuildingHandler::getThis()->specialOnMap;
    
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
    
    startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
    endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
    
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
    
    if (mGameWarMode != BanditsAttackHandler::getThis()->warMode)
    {
        mGameWarMode = BanditsAttackHandler::getThis()->warMode;
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
        factor = 1.0f / UserProfile::getThis()->configSettings->default_homeless_happiness_drop_multiplier;
    }
    /*
     * if the sprite is hungry (currentHungry <= 0), drop happiness!
     */
    if(possessions->happinessRating >= 80.0f)
    {
        // the sprite is in very happy state
        if (cumulativeTime_happiness >= 1.0f / (UserProfile::getThis()->settingsLevel->hungry_happiness_veryHappy_decay / ((float) UserProfile::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else if(possessions->happinessRating >= 60.0f)
    {
        // the sprite is in happy state
        if (cumulativeTime_happiness >= 1.0f / (UserProfile::getThis()->settingsLevel->hungry_happiness_happy_decay / ((float) UserProfile::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else if(possessions->happinessRating >= 40.0f)
    {
        // the sprite is in normal state
        if (cumulativeTime_happiness >= 1.0f / (UserProfile::getThis()->settingsLevel->hungry_happiness_normal_decay / ((float) UserProfile::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else if(possessions->happinessRating >= 10.0f)
    {
        // the sprite is in unhappy state
        if (cumulativeTime_happiness >= 1.0f / (UserProfile::getThis()->settingsLevel->hungry_happiness_unhappy_decay / ((float) UserProfile::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    else
    {
        // the sprite is in angry state
        if (cumulativeTime_happiness >= 1.0f / (UserProfile::getThis()->settingsLevel->hungry_happiness_angry_decay / ((float) UserProfile::getThis()->configSettings->secondToDayRatio)) * factor)
        {
            possessions->happinessRating--;
            cumulativeTime_happiness = 0.0f;
        }
    }
    
    if(possessions->happinessRating < 0)
    {
        possessions->happinessRating = 0;
        
        // when energy <= 0, the villager will leave the village.
        if(villagerClass == V_REFUGEE)
        {
            if(!tryEscape)
            {
                tryEscape = true;
                stringstream ss;
                ss << spriteDisplayedName << " will leave the village!";
                GameHUD::getThis()->addNewNotification(ss.str());
                
                CCPoint target = CCPointMake(29,33);
                if(GoLocation(target))
                {
                    isLeavingNextUpdate = true;
                }
            }
        }
        else if(villagerClass != V_BANDIT && villagerClass != V_CLASS_END)
        {
            if(!BanditsAttackHandler::getThis()->warMode)
            {
                if(villagerClass != V_CITIZEN)
                {
                    if(possessions->jobLocation != NULL)
                    {
                        possessions->jobLocation->memberSpriteList->removeObject(this);
                        possessions->jobLocation = NULL;
                    }
                }
                
                getHome()->memberSpriteList->removeObject(this);
                possessions->homeLocation = NULL;
                
                changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_REFUGEE));
                tryEscape = true;
                stringstream ss;
                ss << spriteDisplayedName << " will leave the village!";
                GameHUD::getThis()->addNewNotification(ss.str());
                
                CCPoint target = CCPointMake(29, 33);
                if(GoLocation(target))
                {
                    isLeavingNextUpdate = true;
                }
            }
        }
    }
}

void GameSprite::scheduleToken(float dt)
{
    if(TutorialManager::getThis()->lockDropTokens)
    {
        return;
    }
    
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
    if(mAverageHappiness >= 80)
    {
        timeDiffer = UserProfile::getThis()->configSettings->token_drop_treshold_time_veryHappy_max - UserProfile::getThis()->configSettings->token_drop_treshold_time_veryHappy_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = UserProfile::getThis()->configSettings->token_drop_treshold_time_veryHappy_min + randomTime;
    }
    else if(mAverageHappiness >= 60)
    {
        timeDiffer = UserProfile::getThis()->configSettings->token_drop_treshold_time_happy_max - UserProfile::getThis()->configSettings->token_drop_treshold_time_happy_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = UserProfile::getThis()->configSettings->token_drop_treshold_time_happy_min + randomTime;
    }
    else if(mAverageHappiness >= 40)
    {
        timeDiffer = UserProfile::getThis()->configSettings->token_drop_treshold_time_normal_max - UserProfile::getThis()->configSettings->token_drop_treshold_time_normal_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = UserProfile::getThis()->configSettings->token_drop_treshold_time_normal_min + randomTime;
    }
    else if(mAverageHappiness >= 10)
    {
        timeDiffer = UserProfile::getThis()->configSettings->token_drop_treshold_time_unhappy_max - UserProfile::getThis()->configSettings->token_drop_treshold_time_unhappy_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = UserProfile::getThis()->configSettings->token_drop_treshold_time_unhappy_min + randomTime;
    }
    else
    {
        timeDiffer = UserProfile::getThis()->configSettings->token_drop_treshold_time_angry_max - UserProfile::getThis()->configSettings->token_drop_treshold_time_angry_min;
        int randomTime = rand() % timeDiffer;
        token_drop_cooldown_treshold = UserProfile::getThis()->configSettings->token_drop_treshold_time_angry_min + randomTime;
    }
}

void GameSprite::dropToken(bool tutorial)
{
    checkDropRate();
    
    int random_number = rand() % 100 + 1;
    if(random_number <= token_drop_rate || tutorial)
    {
        saySpeech(HAPPY, 1.0f);
        ReputationOrb* ro = ReputationOrb::create("REN", UserProfile::getThis()->configSettings->token_disappear_time);
        CCSprite* newToken = ro->getSprite();
        newToken->setAnchorPoint(ccp(0.5, 0.5));
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        CCSize spriteSize = newToken->getContentSize();
        
        newToken->setScale(screenSize.width / spriteSize.width * 0.05f);
        
        SpriteHandler::getThis()->tokensOnMap->addObject(ro);
        
        MapHandler::getThis()->getMap()->addChild(newToken, 99999);
        
        CCPoint targetPos = MapHandler::getThis()->getNearestNoneBuildingTile(currPos);
        
        CCPoint target = MapHandler::getThis()->locationFromTilePos(&targetPos);
        
        newToken->setPosition(target);
    }
}

void GameSprite::checkDropRate()
{
    float mAverageHappiness = GameHUD::getThis()->average_happiness;
    if(mAverageHappiness >= 80)
    {
        token_drop_rate = UserProfile::getThis()->configSettings->token_drop_rate_veryHappy;
    }
    else if(mAverageHappiness >= 60)
    {
        token_drop_rate = UserProfile::getThis()->configSettings->token_drop_rate_happy;
    }
    else if(mAverageHappiness >= 40)
    {
        token_drop_rate = UserProfile::getThis()->configSettings->token_drop_rate_normal;
    }
    else if(mAverageHappiness >= 10)
    {
        token_drop_rate = UserProfile::getThis()->configSettings->token_drop_rate_unhappy;
    }
    else
    {
        token_drop_rate = UserProfile::getThis()->configSettings->token_drop_rate_angry;
    }
}

bool GameSprite::escape()
{
    CCPoint target = CCPointMake(39,60);
    if(!GoLocation(target))
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
        stopAction = false;
        if(enermy != NULL)
        {
            if(enermy->enermy != NULL)
            {
                enermy->enermy = NULL;
            }
            enermy = NULL;
        }
        
        for(int i = 0; i < battleIconArray->count(); i++)
        {
            BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(i);
            bi->hide();
        }
        
        SoundtrackManager::PlaySFX("hurt sfx.wav");
    }
    else
    {
        
        SoundtrackManager::PlaySFX("hit sfx.wav");
        
    }
}

bool GameSprite::hasBandit(CCArray* spritesOnMap, CCPoint checkTile)
{
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        CCPoint gsTile = MapHandler::getThis()->locationFromTilePos(&checkTile);
        
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
        CCPoint gsTile = MapHandler::getThis()->locationFromTilePos(&checkTile);
        
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
    
    startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
    endPos = MapHandler::getThis()->tilePosFromLocation(endPos);
    
    bool hasPath = CreatePath(startPos, endPos);
    if (hasPath)
    {
        isFollowingMoveInstruction = true;
    }
}

void GameSprite::updatePath(CCPoint endPos)
{
    CCPoint startPos = getWorldPosition();
    startPos = MapHandler::getThis()->tilePosFromLocation(startPos);
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

void GameSprite::playAttackAction()
{
    CCPoint pos = spriteRep->getPosition();
    CCSize spriteSize = spriteRep->boundingBox().size;
    spriteRep->stopAllActions();
    
    delete behaviorTree;
    spriteRep->removeChild(speechBubble, true);
    delete speechBubble;
    for(int i = 0; i < battleIconArray->count(); i++)
    {
        BattleIcon* bi = (BattleIcon*) battleIconArray->objectAtIndex(i);
        spriteRep->removeChild(bi->battleSprite, true);
    }
    battleIconArray->removeAllObjects();
    
    spriteRep->removeChild(barHP, true);
    delete barHP;
    
    frameWidth = 128;
    frameHeight = 128;
    
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    MapHandler::getThis()->getMap()->removeChild(spriteRep);
    
    if(villagerClass == V_BANDIT)
    {
        if(currentDir == "UL")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("banditAttackLeftUp.png");
            maxFrameNumber = 11;
            currentFrameNumber = 0;
        }
        else if(currentDir == "DL")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("banditAttackLeftDown.png");
            maxFrameNumber = 15;
            currentFrameNumber = 0;
        }
        else if(currentDir == "UR")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("banditAttackRightUp.png");
            maxFrameNumber = 11;
            currentFrameNumber = 0;
        }
        else if(currentDir == "DR")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("banditAttackRightDown.png");
            maxFrameNumber = 14;
            currentFrameNumber = 0;
        }
    }
    else if(villagerClass == V_SOLDIER)
    {
        if(currentDir == "UL")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("soldierAnimationUL.png");
            maxFrameNumber = 9;
            currentFrameNumber = 0;
        }
        else if(currentDir == "DL")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("soldierAnimationDL.png");
            maxFrameNumber = 11;
            currentFrameNumber = 0;
        }
        else if(currentDir == "UR")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("soldierAnimationUR.png");
            maxFrameNumber = 9;
            currentFrameNumber = 0;
        }
        else if(currentDir == "DR")
        {
            spriteTexture = CCTextureCache::sharedTextureCache()->addImage("soldierAnimationDR.png");
            maxFrameNumber = 11;
            currentFrameNumber = 0;
        }
    }
    
    spriteRect = CCRectMake(0, 0,  frameWidth, frameHeight);
    
    spriteRep = CCSprite::createWithTexture(spriteTexture, spriteRect);
    spriteRep->setAnchorPoint(ccp(0.5, 0.75));
    spriteRep->cocos2d::CCNode::setScale(spriteSize.width / spriteRep->boundingBox().size.width, spriteSize.height / spriteRep->boundingBox().size.height);
    
    // common stats
    bool parsingSuccessful = false;
    
    // specific stats
    parsingSuccessful = loadClassSetup();
    
    clearSetup();
    movementSpeed = getPossessions()->default_movement_speed;
    
    spriteRep->setPosition(pos);
    
    //Speech bubble
    speechBubble = new SpeechBubble();
    speechBubble->createSpeechBubble();
    
    
    speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 1.2f,
                                  spriteRep->boundingBox().size.height * 1.5f));
    
    spriteRep->addChild(speechBubble);
    
    BattleIcon* battleIcon = BattleIcon::create(this);
    if(villagerClass == V_BANDIT)
    {
        battleIcon->show();
    }
    battleIconArray->addObject(battleIcon);
    
    barHP = new ProgressBar();
    barHP->createProgressBar(CCRectMake(0, 0, 76, 16),
                             CCRectMake(3, 3, 70, 10),
                             "Energy_brown bar.png",
                             "NONE",
                             "NONE",
                             "Energybarblue.png", true);
    barHP->setAnchorPoint(ccp(0.5, 0.5));
    
    barHP->setValue((float) getPossessions()->current_endurance / (float) getPossessions()->max_endurance);
    barHP->setPosition(ccp(spriteRep->boundingBox().size.width * 1.0f, spriteRep->boundingBox().size.height * 2.0f));
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
    
    MapHandler::getThis()->getMap()->addChild(spriteRep,
                                                         MapHandler::getThis()->calcZIndex(pos));
    behaviorTree->onInitialize();
    behaviorTree->update();
    
    isInAttackAction = true;
}