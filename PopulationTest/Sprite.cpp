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

#include <sstream>

GameSprite::GameSprite()
{
    spriteW = 128;
    spriteH = 128;
    spriteSpeed = 50;
    currAction = IDLE;
    nextAction = IDLE;
    config_doc = "";
    shouldStopNextSquare = false;
    speechBubble = NULL;
    isFollowingMoveInstruction = false;
    currTile = NULL;
    shouldSetVisibleNextFrame = false;
 
    spawncost = 0;
    path = NULL;//CCArray::create();
    //path->retain();
    
    gender = 'n';
    race = 'n';
    
    wanderFlag = false;
    justSoldHouse = false;
    justQuitJob = false;
    isLeavingNextUpdate = false;
    
    shouldUpgrade = false;
    
    isInteractingSocial = false;
    
    isInBuilding = false;
    
    idleDelay = 0.0f;
    fdaysLeft = 0.0f;
    
}


void GameSprite::initAI(bool isUpgrade)
{
    if (config_doc.length() == 0)
    {
        std::cout << "no configuration loaded!" << std::endl;
        return;
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
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse defaults doc\n" << reader.getFormatedErrorMessages() << std::endl;
        
    }
    else
    {
        if (!isUpgrade)
        {
        std::string encoding = defaultsRoot.get("encoding", "UTF-8" ).asString();
        
        possessions->happinessRating = atoi(defaultsRoot["default_happiness"].asString().c_str());
        possessions->loyaltyRating = atoi(defaultsRoot["default_loyalty"].asString().c_str());
        possessions->movementRange =atoi( defaultsRoot["default_move_range"].asString().c_str());
        possessions->movementSpeed = atof( defaultsRoot["default_movement_speed"].asString().c_str());
        possessions->animateSpeed = atof( defaultsRoot["default_animate_speed"].asString().c_str());

        possessions->energyRating = atoi(defaultsRoot["default_energy"].asString().c_str());
        possessions->defaultEnergy = possessions->energyRating;
        
        if (defaultsRoot["spawn_cost"] != NULL)
        {
            spawncost = atoi(defaultsRoot["spawn_cost"].asString().c_str());
        }
        }
        
        
        if (isUpgrade)
            possessions->expToLevel->removeAllObjects();
        
        for (int i = 1; i <= 20; i++)
        {
            std::ostringstream s;
            s << "default_exp_level" << i;
            if (defaultsRoot[s.str()].isNull())
                break;
            
            CCInteger* intval = CCInteger::create(atoi(defaultsRoot[s.str()].asString().c_str()));
            
            possessions->expToLevel->addObject(intval);
            s.clear();
        
        }
    }
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
            pCopy->type = this->type;
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
    // spriteRep->retain();
     CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(tilePos);
 
     spriteRep->setPosition(target);
     
     setAction(IDLE);
     idleDelay = 0.0f;
     
     initAI();
     
     changeAnimation("DL");
     currentDir = "DL";
     
     //Speech bubble
     speechBubble = new SpeechBubble();
     speechBubble->createSpeechBubble();
     
     speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 0.5f,
                                   spriteRep->boundingBox().size.height * 1.5f));
     spriteRep->addChild(speechBubble);

     GameScene::getThis()->mapHandler->getMap()->addChild(spriteRep,
                                                          GameScene::getThis()->mapHandler->calcZIndex(*tilePos));
     //speechBubble->autorelease();
     
     
     behaviorTree->onInitialize();
     behaviorTree->update();
     
     spriteDisplayedName = NameGenerator::GenerateName(race, gender);

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
    
    //destroy the AI
    delete possessions;
    possessions = NULL;
    delete behaviorTree;
    behaviorTree = NULL;
}



 void GameSprite::unmakeSprite()
 {
     LeaveJob();
     LeaveHouse();
     delete possessions;
     possessions= NULL;
     delete behaviorTree;
     behaviorTree = NULL;
     spriteRep->stopAllActions();
     
   //  CC_SAFE_RELEASE(callback);
     
     //spriteRep->removeChild(speechBubble, true);
    
     // (( CCSpriteBatchNode* ) GameScene::getThis()->spriteHandler->allSpriteSheets->objectAtIndex(this->batchLayerIndex))->removeChild(spriteRep, true);
     if (GameScene::getThis()->mapHandler->getMap()->getChildren()->containsObject(spriteRep))
         GameScene::getThis()->mapHandler->getMap()->removeChild(spriteRep);
     
     delete speechBubble;
 
     //spriteAnimAction->release();
   //  animation->release();
   //  spriteAnimAction->release();
    // spriteRunAction->release();
 //    spriteRep->release();
     if (path != NULL)
     {
         path->removeAllObjects();
     }
     //destroy the AI

 }




bool GameSprite::CreatePath(CCPoint from, CCPoint to)
{
    if (currAction != IDLE) setAction(IDLE);
        
    
        if (from.equals(to))
        {
            CCLog("I'm moving to the same position ma");
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
            
            CCLog("Warning! no path from %f, %f to %f, %f", from.x, from.y, to.x, to.y);
            delete p;
            path = NULL;
            return false;
        }
        path->retain();
    
      //  followPath();
    
        delete p;
        return true;
 }


void GameSprite::followPath()
{
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
            
            
            
            CCPoint nextPos = GameScene::getThis()->mapHandler->locationFromTilePos(&(node->tilepos));
           
            
            
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
    if (currentDir.compare(dir) == 0 && lastFrameAction == currAction) return;
    // animation->release();
    currentDir = dir;
    //if (spriteRep->numberOfRunningActions() > 0)
      //  spriteRep->stopAction(spriteAnimAction);
    spriteRep->stopAllActions();
    
    
    CCArray *animFrames = CCArray::create();
  //  animFrames->retain();
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
    //spriteRep->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((frameName + frameNum).c_str()));

    for (int i = 1; i <= frameCount; ++i)
    {
       std::stringstream ss;

        ss << i;
        strNum = ss.str();
        
        frameNum = "00" + strNum +".png";
        CCSpriteFrame* fr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((frameName + frameNum).c_str());
        
        if (fr != NULL)
            animFrames->addObject(fr);
        
    }
    
    animation = CCAnimation::createWithSpriteFrames(animFrames, 50.0f / possessions->animateSpeed * 0.2f);
 //   animFrames->removeAllObjects();
 //   animFrames->release();
    animation->setRestoreOriginalFrame(false);
    spriteAnimAction = CCRepeatForever::create(CCAnimate::create(animation));
    
    spriteRep->runAction(spriteAnimAction);
    
    
}

void GameSprite::moveSpritePosition(CCPoint target, cocos2d::CCObject *pSender)
{
    if (spriteRep == NULL) return;
    
    CCPoint diff = ccpSub(target, spriteRep->getPosition());
    
    if (spriteRep->numberOfRunningActions() > 1)
    {
        spriteRep->stopAction(spriteRunAction);
        // spriteRunAction->release();

    }
    if (diff.x > 0 && diff.y > 0)
            changeAnimation("UR");
    else if (diff.x < 0 && diff.y > 0)
        
        changeAnimation("UL");
    else if (diff.x > 0 && diff.y < 0)
        changeAnimation("DR");
    else if (diff.x < 0 && diff.x < 0)
        changeAnimation("DL");

    callback = CCCallFuncN::create(pSender, callfuncN_selector(GameSprite::moveComplete));
    callback->retain();
    spriteRunAction = CCSequence::createWithTwoActions(CCMoveBy::create(50.0f / possessions->movementSpeed, diff), callback);
    //spriteRunAction = CCSequence::createWithTwoActions(CCMoveBy::create(50.0f, diff), callback);

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
    
    if (path == NULL) return;
    
    
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
        changeAnimation(currentDir);
        
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
    if (isInteractingSocial) return false;
    
    //Note: the random nature of Wander means there is no need to set isFollowingMoveInstruction to true. Wander should be interruptable at any time.
    wanderFlag = !wanderFlag;
    if (wanderFlag)
    {
   
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
    currPos = getWorldPosition();
    currPos = GameScene::getThis()->mapHandler->tilePosFromLocation(currPos);
    
    updateZIndex();
    
    lastFrameAction = currAction;
  
    //After this part, only functions relating to IDLE will be called. This is because WALKING already has its own function calls.

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
        
        if (possessions->happinessRating < happiness_level_before_loyalty_affected && possessions->loyaltyRating > 0)
        {
            //  saySpeech("Patience wearing thin.",2.0f);
            increasePossession(STATS_LOYALTY, loyalty_mod_if_below_happiness_threshold);
            
        }
        
        if (possessions->loyaltyRating <= 0)
        {
            saySpeech("I'm going elsewhere.", 2.0f);
            isLeavingNextUpdate = true;
            
        }
        //todo: make sure this is not called every frame the sprite remains idle in the building.
        if (currTile != NULL)
        {
            if (currTile->building)
            {
                currTile->building->StickAroundHandler(this);
           }
            
        
        
        }
      

        
        //modify loyalty based on happiness;
        
        
        /*kick starts the sprite's AI, because idle doesn't do this normally*/
        behaviorTree->onInitialize();
        behaviorTree->update();
        
        
        //I didn't want hasJob to trigger if there is no house.
        if (possessions->homeLocation == NULL && !justSoldHouse)
        {
            saySpeech("Homeless..", 2.0f);
            //I need to do this the manual way. Otherwise the 2nd saySpeech overrides the first.
             if (possessions->happinessRating > 0) possessions->happinessRating += happiness_mod_homeless;
            //increasePossession(STATS_HAPPINESS, happiness_mod_homeless); //note actually subtracts
            return;
        }
        else
        {
            justSoldHouse = false;
            return;
        }
        /*
        if (!possessions->hasJob && !justQuitJob)
        {
            saySpeech("Jobless..", 2.0f);
            if (possessions->happinessRating > 0) possessions->happinessRating += happiness_mod_jobless;
            
            //increasePossession(STATS_HAPPINESS, happiness_mod_jobless); //note actually subtracts
            return;
        }
        else
        {
            justQuitJob = false;
            return;
        }
        */
        
    }
    
}


void GameSprite::updateZIndex()
{
    if (!spriteRep) return;
    if (!spriteRep->isVisible()) return;
    if (path == NULL) return; //not necessary to update Z if there isn't a path, implies sprite isn't moving
    if (path->count() == 0) return; //same reason
    //needs to be in world space
   // CCPoint position = getWorldPosition();//GameScene::getThis()->mapHandler->getMap()->convertToWorldSpace(spriteRep->getPosition()); // .ConvertToWorldSpace(startPos);
    
    
    //position = GameScene::getThis()->mapHandler->tilePosFromLocation(position);
    spriteRep->setZOrder( GameScene::getThis()->mapHandler->calcZIndex(currPos));
    
    
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
            statCap = possessions->defaultEnergy;
            statName = "Energy";
            break;
        case STATS_EXP:
            pStat = &possessions->expRating;
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
}

void GameSprite::saySpeech(const char* text, float timeInSeconds)
{
    speechBubble->clearContent();
    CCLabelTTF* label = CCLabelTTF::create(text, "Droidiga", 18);
   
    label->setColor(ccc3(81, 77, 2));
   
    speechBubble->addContent(label, CCPointZero);
    speechBubble->show(timeInSeconds);
}

/*transactions*/
bool GameSprite::SetHouse(int instanceID)
{
    if (isInteractingSocial) return false;
    
    if (isFollowingMoveInstruction)
    {
        CCLog("still moving. Can't obey.");
        return false;
    }

     
    Building* b = (Building*)GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(instanceID);
    if (!b)
    {
        CCLog("invalid Instance ID!");
        return false;
    }
    
    if (b->buildingType != HOUSING)
    {
        CCLog("Building isn't a house!");
        return false;
    }
    //looks like I have to do this after all.
    if (b->getPopulationCount() >= b->populationLimit)
    {
        CCLog("Building has reached its population limit!");
        return false;
    }
    
    bool boughtHouse = possessions->setHome(b);
    
    if (boughtHouse)
        b->addPopulation(this);
    

    //house successfully bought
    return boughtHouse;
}

bool GameSprite::LeaveJob()
{
        if (possessions->jobLocation != NULL)
        {
            possessions->LeaveJob();
            
        }
    justQuitJob = true;
    return true;
}


bool GameSprite::LeaveHouse()
{
    if (possessions->homeLocation != NULL)
    {
        possessions->homeLocation->removePopulation(this);
        possessions->LeaveHome();
    }
    justSoldHouse = true;
    return true;
}
/*pathing*/
/*paths back home, if possible. Ignores range for obvious reasons.*/
bool GameSprite::PathToHome()
{
    
    if (isInteractingSocial)
    {
        CCLog("waiting for godot");
        return false;
    }
    
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
    
    if (isInteractingSocial)
    {
        CCLog("waiting for godot");
        return false;
    }

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
  //  CCPoint endPos = possessions->jobLocation->getWorldPosition();
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
    
    if (isInteractingSocial)
    {
        CCLog("waiting for godot");
        return false;
    }

    
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

/*Paths to a building under construction. Fails if there isn't a building under construction.*/
bool GameSprite::GoBuild(Building *b)
{
    if (b->build_uint_current >= b->build_uint_required) return false; //already built
    
    this->nextAction = BUILD;
    
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
    if (pathFromHome->count() > possessions->movementRange)
    {
        delete p;
        return false;
    }
    delete p;
    return true;
}

/*changes the gamesprite's appearance.*/
/*does not change stats.*/
void GameSprite::ChangeSpriteTo(GameSprite *sp)
{
    type = sp->type;
    batchLayerIndex = sp->batchLayerIndex;
    idleFrameCount = sp->idleFrameCount;
    walkingFrameCount = sp->walkingFrameCount;
    spriteName = sp->spriteName;
    spriteClass = sp->spriteClass;
    internal_rank = GameScene::getThis()->spriteHandler->getRank(spriteClass);
    config_doc = sp->config_doc;
    defaults_doc = sp->defaults_doc;
    
    //Because the class of the character is changed, the class level goes back to 1. 
    possessions->classLevel = 1;
    
    ReplaceSpriteRep();
}


void GameSprite::ReplaceSpriteRep()
{
    CCPoint pos = spriteRep->getPosition();
  //  spriteRunAction->stop();
  //  spriteAnimAction->stop();
   // spriteRep->stopAction(spriteRunAction);
   // spriteRep->stopAction(spriteAnimAction);
    spriteRep->stopAllActions();
    
    delete behaviorTree;
    spriteRep->removeChild(speechBubble, true);
    delete speechBubble;
    
    //destroy the AI
    // (( CCSpriteBatchNode* ) GameScene::getThis()->spriteHandler->allSpriteSheets->objectAtIndex(this->batchLayerIndex))->removeChild(spriteRep, true);
    
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(spriteRep);
    //spriteRunAction->release();
    //spriteAnimAction->release();
    
   // if (spriteRep != NULL)
  //  spriteRep->release();
    
    spriteRep = CCSprite::create();
    
    //std::string initName = spriteName->getCString();// +"_idle_UL_0";
    //initName += "_walk_DL_0";
    
    std::string initName = spriteName;
    initName+= "_IDL001.png";
    spriteRep->initWithSpriteFrameName(initName.c_str());
   // spriteRep->retain();
    
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
   
    speechBubble->setPosition(ccp(spriteRep->boundingBox().size.width * 0.5f,
                                  spriteRep->boundingBox().size.height * 1.5f));
    spriteRep->addChild(speechBubble);
    
    GameScene::getThis()->mapHandler->getMap()->addChild(spriteRep,
                                                        GameScene::getThis()->mapHandler->calcZIndex(pos));
    
    behaviorTree->onInitialize();
    behaviorTree->update();
    
   // updateZIndex();
  
}

int GameSprite::getLevel()
{
    if (!possessions) return -1;
    
    return possessions->classLevel;// educationLevel;
}

void GameSprite::TakeStockOfDay()
{
    //I assume that the sprite has a home.
    if (possessions->homeLocation != NULL)
    {
        if (possessions->homeLocation->isOverpopulated())
        {
            increasePossession(STATS_HAPPINESS, possessions->homeLocation->getOverpopulateCount() * happiness_mod_overcrowded_conditions);
        }
        
        //mistrust factor
        //aliens not affected
        if (race=='h')
        {
            increasePossession(STATS_HAPPINESS, possessions->homeLocation->getAlienPopulationCount() * happiness_mod_per_alien_in_same_house);
        }
        
    }
 
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
        shouldUpgrade = true;
        followPath();
    }
    return;

}

void GameSprite::CallbackDayPassed()
{
    if (race != 'a') return;
    if (fdaysLeft > 0)
        --fdaysLeft;
    else
    {
        saySpeech("My contract's up.",2);
        isLeavingNextUpdate = true;
    }
    
}

//
void GameSprite::CallbackPerformingTask()
{
    
}

