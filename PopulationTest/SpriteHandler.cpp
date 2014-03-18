
//
//  SpriteHandler.cpp
//  PopulationTest
//
//  Created by Serious Games on 25/3/13.
//
//

#include "SpriteHandler.h"
#include "GameScene.h"
#include "GameHUD.h"

//Config headers
#include "CitizenCFG.h"
#include "FarmerCFG.h"
#include "SoldierCFG.h"
#include "RefugeeCFG.h"
#include "BuilderCFG.h"
#include "BanditCFG.h"

#include "NameGenerator.h"

#include <json/json.h>


SpriteHandler::~SpriteHandler()
{
    tokensOnMap->removeAllObjects();
    CC_SAFE_RELEASE(tokensOnMap);
    
    allSpriteSheets->removeAllObjects();
    CC_SAFE_RELEASE(allSpriteSheets);
    
    
    aliensOnMap->removeAllObjects();
    CC_SAFE_RELEASE(aliensOnMap);
    
    localsOnMap->removeAllObjects();
    CC_SAFE_RELEASE(localsOnMap);
    
    for (int i = 0; i < spritesOnMap->count(); ++i)
    {
        ((GameSprite*)spritesOnMap->objectAtIndex(i))->unmakeSpriteEndGame();
        spritesOnMap->objectAtIndex(i)->release();
    }
    
    spritesOnMap->removeAllObjects();
    CC_SAFE_RELEASE(spritesOnMap);
    
    allSprites->removeAllObjects();
    CC_SAFE_RELEASE(allSprites);
    
    
    allClassRequirements->removeAllObjects();
    CC_SAFE_RELEASE(allClassRequirements);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
    
}

void SpriteHandler::initialize()
{
    cumulatedTime = 0.0f;
    
    NameGenerator::init();
    
    tokensOnMap = CCArray::create();
    tokensOnMap->retain();
    
    //create and add all spritesheets first.
    allSprites = CCArray::create();
    allSprites->retain();
    
    allSpriteSheets = CCArray::create();
    allSpriteSheets->retain();
    
    spritesOnMap = CCArray::create();
    spritesOnMap->retain();
    
    
    aliensOnMap = CCArray::create();
    aliensOnMap->retain();
    
    localsOnMap = CCArray::create();
    localsOnMap->retain();
    
     int targetIdleF = 0;
    int targetWalkF = 0;
    
    //here I assume there are 10 enums.
    for (int i = 0; i < SPRITETYPE_END; ++i )
    {
        std::string fileName;
        std::string configContent;
        std::string defaultContent;
        std::string targetClass;
        std::string spriteContent;
        char targetGender;
        
        //randomly spawn as male or female.
      
        //it's always a human now.
        char targetRace = 'h';
        
        switch (i)
        {
            case 0:
                targetGender = 'm';
                fileName = "MMR";
                targetIdleF = 4;
                targetWalkF = 4;
                configContent = refugeeConfig;
                defaultContent = refugeeDefaults;
                targetClass = "refugee";

                break;
            case 1:
                targetGender = 'f';
                fileName = "FMR";
                targetIdleF = 4;
                targetWalkF = 4;
            
                configContent = refugeeConfig;
                defaultContent = refugeeDefaults;
                targetClass = "refugee";
                break;
            case 2:
                targetGender = 'm';
                fileName = "MMC";
                targetIdleF = 4;
                targetWalkF = 4;
            
                configContent = citizenConfig;
                defaultContent = citizenDefaults;
                targetClass = "citizen";
                break;
            case 3:
                targetGender = 'f';
                fileName = "FMC";
                targetIdleF = 4;
                targetWalkF = 4;
            
                configContent = citizenConfig;
                defaultContent = citizenDefaults;
                targetClass = "citizen";
                break;
            case 4:
                targetGender = 'm';
                fileName = "MMC";
                targetIdleF = 4;
                targetWalkF = 4;
            
                configContent = builderConfig;
                defaultContent = builderDefaults;
            
                targetClass = "builder";
                break;
            case 5:
                targetGender = 'f';
                fileName = "FMC";
                targetIdleF = 4;
                targetWalkF = 4;
            
                configContent = builderConfig;
                defaultContent = builderDefaults;
            
                targetClass = "builder";
            
                break;
            
            case 6:
                targetGender = 'm';
                fileName = "MMF";
                targetIdleF = 3;
                targetWalkF = 4;
                configContent = farmerConfig;
                defaultContent = farmerDefaults;
                targetClass = "farmer";
            
                break;
            
            case 7:
                targetGender = 'f';
                fileName = "FMF";
                targetIdleF = 3;
                targetWalkF = 4;
                configContent = farmerConfig;
                defaultContent = farmerDefaults;
                targetClass = "farmer";
            
                break;
            case 8:
                targetGender = 'm';
                fileName = "MMW";
          
                targetIdleF = 3;
                targetWalkF = 4;
                
                configContent = soldierConfig;
                defaultContent = soldierDefaults;
                targetClass = "soldier";

                break;
            case 9:
                targetGender = 'f';
                fileName = "FMW";
                targetIdleF = 3;
                targetWalkF = 4;
            
                configContent = soldierConfig;
                defaultContent = soldierDefaults;
                targetClass = "soldier";
                break;
            case 10:
                targetGender = 'm';
                fileName = "MMB";
            
                targetIdleF = 3;
                targetWalkF = 4;
            
                configContent = banditConfig;
                defaultContent = banditDefaults;
                targetClass = "bandit";
            
                break;
            case 11:
                targetGender = 'f';
                fileName = "FMB";
                targetIdleF = 3;
                targetWalkF = 4;
            
                configContent = banditConfig;
                defaultContent = banditDefaults;
                targetClass = "bandit";
                break;
        }
        std::string filepath = fileName + ".png";
        std::string plistName = fileName +".plist";
     
        CCSpriteBatchNode* spriteSheet = CCSpriteBatchNode::create(filepath.c_str());
        
        /*all spritesheets must be a child of the map*/
        AddToCache(NULL, fileName);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName.c_str());//  plistName.c_str());
        allSpriteSheets->addObject(spriteSheet);
        
        /*1 sprite per char. */
        GameSprite* gs = GameSprite::create();
        gs->gender = targetGender;
        gs->race = targetRace;
        gs->spriteName = fileName;
        gs->setFrameCount(targetIdleF, targetWalkF);
        gs->batchLayerIndex = i;
        gs->spriteClass = targetClass;
        gs->type = SpriteType (i);
        gs->setAIConfig(configContent);
        gs->setDefaultsConfig(defaultContent);
        
        allSprites->addObject(gs);
        configContent = "";
    }
}

void SpriteHandler::AddToCache(cocos2d::CCSpriteBatchNode *spritesheet, std::string animName)
{
    std::string plistName = animName + ".plist";
   
    
}
GameSprite* SpriteHandler::getSpriteByType(SpriteType type)
{
    //return NULL;
    if (!allSprites) return NULL;
    if (allSprites->count() == 0) return NULL;
    
    for (int i = 0; i < allSprites->count(); ++i)
    {
        GameSprite* tgt = (GameSprite*) allSprites->objectAtIndex(i);
        if (tgt->type == type) return tgt;
    }
    return NULL;
    
}

GameSprite* SpriteHandler::getSpriteTemplate(const char *classname, char gender, char race)
{
    if (!allSprites) return NULL;
    if (allSprites->count() == 0) return NULL;
    
    /*
    if (strcmp(classname, "chief") == 0)
    {
        if (race == 'a')
            return getSpriteByType(ALIEN_CHIEF);
        else
            return getSpriteByType(MAYAN_CHIEF);
    }
    else
    {
        GameSprite* currsprite;

        for (int i = 0; i < allSprites->count(); ++i)
        {
            currsprite = (GameSprite*)allSprites->objectAtIndex(i);
            if (currsprite->spriteClass.compare(classname) == 0 &&
                currsprite->gender == gender &&
                currsprite->race == race)
                return currsprite;
        }
        
        return NULL;
    }*/
    
    return NULL;
}



void SpriteHandler::addSpriteToMap(cocos2d::CCPoint &tilePos, SpriteType type)
{
    GameSprite* targetSprite = getSpriteByType(type);
    
    if (!targetSprite) return;
    
    GameSprite* newSprite = (GameSprite*)targetSprite->copy();
    newSprite->retain();
    
    newSprite->defaults_doc = targetSprite->defaults_doc;
    
    newSprite->makeSprite(&tilePos);
    
    spritesOnMap->addObject(newSprite);
    
    GameHUD::getThis()->onSpriteAddedToMap(newSprite);
    
}

void SpriteHandler::loadSpriteToMap(cocos2d::CCPoint &tilePos, GameSprite *sp, std::string details)
{
    GameSprite* newSprite = (GameSprite*)sp->copy();
    newSprite->retain();
    newSprite->makeSprite(&tilePos);
    if (details.length() > 0)
    {
        stringstream ss(details);
        std::string buffer;
        std::vector<std::string>tokens;
        while (ss.peek() != EOF)
        {
            std::getline(ss, buffer, '|' );
            
            tokens.push_back(buffer);
            
            buffer = "";
            
        }
        
        //       +GlobalHelper::Convert(sp->getPossessions()->cashOnHand) +"|" + GlobalHelper::Convert(sp->getPossessions()->happinessRating) +"|" + GlobalHelper::Convert(sp->getPossessions()->loyaltyRating) +"|" + GlobalHelper::Convert(sp->getPossessions()->educationLevel) +"|" + GlobalHelper::Convert(sp->getPossessions()->intelligenceRating) +"|" + GlobalHelper::Convert(sp->getPossessions()->socialRating) + "|" + GlobalHelper::Convert(sp->getPossessions()->movementRange) + "|" + GlobalHelper::Convert(sp->getPossessions()->energyRating) +"|" + GlobalHelper::Convert(sp->getPossessions()->classLevel) + "|" + GlobalHelper::Convert(sp->getPossessions()->expRating) + "|" + GlobalHelper::Convert(sp->getPossessions()->hasHouse) + "|"+ GlobalHelper::Convert(homeID) + "|"+ GlobalHelper::Convert(sp->getPossessions()->isRentalProperty) + "|"+ GlobalHelper::Convert(sp->getPossessions()->isSqueezing) + "|"+ GlobalHelper::Convert(sp->getPossessions()->rentalVisitsLeft) + "|"+ GlobalHelper::Convert(sp->getPossessions()->hasJob) + "|"+ GlobalHelper::Convert(workID) + "|"+ sp->getPossessions()->jobClass + GlobalHelper::Convert(sp->getPossessions()->jobIndex) +
    
        if (tokens.size() == 19)
        {
       //     newSprite->getPossessions()->cashOnHand = atoi(tokens[0].c_str());
            newSprite->getPossessions()->happinessRating = atoi(tokens[1].c_str());
            newSprite->getPossessions()->loyaltyRating = atoi(tokens[2].c_str());
         //   newSprite->getPossessions()->educationLevel = atoi(tokens[3].c_str());
          //  newSprite->getPossessions()->intelligenceRating = atoi(tokens[4].c_str());
           // newSprite->getPossessions()->socialRating = atoi(tokens[5].c_str());
            //newSprite->getPossessions()->default_move = atoi(tokens[6].c_str());
            newSprite->getPossessions()->energyRating = atoi(tokens[7].c_str());
            
            //house
            
            //newSprite->getPossessions()->hasHouse = false;//!!atoi(tokens[10].c_str());
            /*
            int homeID = atoi(tokens[11].c_str());
                newSprite->getPossessions()->homeLocation = GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(homeID);
                
               // newSprite->getPossessions()->homeLocation->addPopulation(newSprite);
                newSprite->getPossessions()->isRentalProperty = !!atoi(tokens[12].c_str());
                newSprite->getPossessions()->isSqueezing = !!atoi(tokens[13].c_str());
                newSprite->getPossessions()->rentalVisitsLeft = atoi(tokens[14].c_str());
                
             
            }*/
            /*
            newSprite->getPossessions()->hasJob = !!atoi(tokens[15].c_str());
            if (newSprite->getPossessions()->hasJob)
            {
                int jobID = atoi(tokens[16].c_str());
               // newSprite->getPossessions()->jobLocation = GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(jobID);
              /
               // newSprite->getPossessions()->jobLocation->addPopulation(newSprite);
                
               // newSprite->getPossessions()->jobClass = tokens[17];
               // newSprite->getPossessions()->jobIndex = atoi(tokens[18].c_str());
                
               // ((Job*)newSprite->getPossessions()->jobLocation->getJobsAvailable()->objectAtIndex(newSprite->getPossessions()->jobIndex))->signUpJob();
                
            }*/
            
            
        }
        
        
        
    }
    
    
    spritesOnMap->addObject(newSprite);
    
    GameHUD::getThis()->onSpriteAddedToMap(newSprite);
}

void SpriteHandler::removeSpriteFromMap(GameSprite* sprite)
{
    if (sprite->race == 'a')
        aliensOnMap->removeObject(sprite);
    else
        localsOnMap->removeObject(sprite);
    
    spritesOnMap->removeObject(sprite);
    
    
    
    
    //Order of execution! 
    GameHUD::getThis()->onSpriteRemovedFromMap(sprite);
    sprite->unmakeSprite();
    sprite->release();
    
}

/*When a new job location is created, all sprites with an unsuitable job quits and tries to find a new job.*/
//disabled, handled in AI
void SpriteHandler::NewJobLocationCreated()
{
   /*
    GameSprite* sp = NULL;
    for (int i = 0; i < spritesOnMap->count(); ++i)
    {
        sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
        if (sp->getPossessions()->hasJob)
        {
        
            if (sp->getPossessions()->jobClass.compare(sp->spriteClass) != 0)
                sp->QuitJob();
        }
    }*/
}

/*when a new house is created, all sprites with an overpopulated house tries to buy a new house.*/
//disabled, handled in AI

void SpriteHandler::NewHousingLocationCreated()
{
/*
    GameSprite* sp = NULL;
    for (int i = 0; i < spritesOnMap->count(); ++i)
    {
        sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
        
        if (sp->getPossessions()->hasHouse)
        {
            sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
            if (sp->getPossessions()->homeLocation->isOverpopulated())
                sp->SellHouse();
            
        }
    }*/
    
}

int SpriteHandler::getRank(std::string spriteclass)
{
    /*
    if (spriteclass.compare("farmer") ==0|| spriteclass.compare("citizen") == 0) return 0;
    if (spriteclass.compare("merchant")==0 || spriteclass.compare("researcher") == 0) return 1;
    if (spriteclass.compare("warrior") ==0) return 2;
    if (spriteclass.compare("chief")==0) return 3;
    */
    
    //Rank doesn't matter anymore.
    return 0;
}



int SpriteHandler::getAlienCount()
{

    
    return aliensOnMap->count();
}

// Artificial Intelligence Included
void SpriteHandler::update(float dt)
{
    /*
     * the hungry parameter will fade according to time past (fade per second), from 100 (maximun) to 0 (minimum);
     * when the parameter reaches 0, the sprite is in hungry state -> lose happiness and increase the energy usage.
     */
    cumulatedTime += dt;
    
    if(cumulatedTime >= (1.0f / (GameScene::getThis()->settingsLevel->global_hungry_decay / ((float) GameScene::getThis()->configSettings->secondToDayRatio * (float) 4))))
    {
        for(int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = ((GameSprite*) spritesOnMap->objectAtIndex(i));
            gs->getPossessions()->currentHungry--;
            
            if(gs->getPossessions()->currentHungry < 0)
            {
                gs->getPossessions()->currentHungry = 0;
            }
        }
        
        cumulatedTime = 0.0f;
    }
    
    /*
     * if the sprites' happiness has been sticked, set the individual happiness of each sprite to 70 (in happy state). Otherwise, the individual happiness will change through the game.
     */
    if (GameHUD::getThis()->stickHappiness)
    {
        for(int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            gs->getPossessions()->happinessRating = 70;
        }
    }
    else
    {
        for(int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            // if the gamesprite is in hungry state, update the hungry happiness of each sprite
            gs->updateHungry(dt);
        }
    }
    
    /*
     * update the sprites' endurance!
     */
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        gs->updateEndurance(dt);
    }
    
    /*
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = ((GameSprite*) spritesOnMap->objectAtIndex(i));
        
        // check whether a sprite is hungry, if hungry, ask them to eat food
        if(gs->getPossessions()->currentHungry <= GlobalSettings::find_food_hungry_treshold && gs->currAction != EATING && gs->nextAction != EATING && gs->currAction && gs->getTargetLocation() != gs->getHome())
        {
            gs->goToEat();
        }
    }
    */
    
    /*
     * schedule the ren token dropping mechanism. The ren tokens will be dropped by chance.
     */
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = ((GameSprite*) spritesOnMap->objectAtIndex(i));
        
        gs->scheduleToken(dt);
    }
}

float SpriteHandler::getCumulatedTime()
{
    return cumulatedTime;
}

void SpriteHandler::setCumulatedTime(float time)
{
    cumulatedTime = time;
}