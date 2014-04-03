
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
#include "GlobalHelper.h"
#include "ReputationOrb.h"

#include <json/json.h>

SpriteHandler::SpriteHandler()
{
    tokensOnMap = CCArray::create();
    tokensOnMap->retain();
    
    //create and add all spritesheets first.
    allSprites = CCArray::create();
    allSprites->retain();
    
    allSpriteSheets = CCArray::create();
    allSpriteSheets->retain();
    
    spritesOnMap = CCArray::create();
    spritesOnMap->retain();
    
    allSpriteClass = CCArray::create();
    allSpriteClass->retain();
    
    
}

SpriteHandler::~SpriteHandler()
{
    allSpriteClass->removeAllObjects();
    CC_SAFE_RELEASE(allSpriteClass);
    
    tokensOnMap->removeAllObjects();
    CC_SAFE_RELEASE(tokensOnMap);
    
    allSpriteSheets->removeAllObjects();
    CC_SAFE_RELEASE(allSpriteSheets);
    
    for (int i = 0; i < spritesOnMap->count(); ++i)
    {
        ((GameSprite*)spritesOnMap->objectAtIndex(i))->unmakeSpriteEndGame();
        spritesOnMap->objectAtIndex(i)->release();
    }
    
    spritesOnMap->removeAllObjects();
    CC_SAFE_RELEASE(spritesOnMap);
    
    allSprites->removeAllObjects();
    CC_SAFE_RELEASE(allSprites);
    
    /*
    if (allClassRequirements != NULL)
    {
    
        allClassRequirements->removeAllObjects();
        CC_SAFE_RELEASE(allClassRequirements);
    }*/
    CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
}

void SpriteHandler::initialize()
{
    cumulatedTime = 0.0f;
    
    NameGenerator::init();
    
    int targetIdleF = 0;
    int targetWalkF = 0;
    
    //here I assume there are 10 enums.
    for (int i = 0; i < SPRITE_APPEARANCE_TYPE_END; i++)
    {
        std::string fileName;
        std::string configContent;
        std::string defaultContent;
        std::string targetClass;
        std::string spriteContent;
        char targetGender;
        VillagerClass villagerClass;
        SpriteAppearanceType sat;
        
        //randomly spawn as male or female.
      
        //it's always a human now.
        char targetRace = 'h';
        
        switch (i)
        {
            case 0:
                targetGender = 'm';
                fileName = "MaleVillager";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = M_A_VILLAGER;
                break;
            case 1:
                targetGender = 'f';
                fileName = "FemaleVillager";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = F_A_VILLAGER;
                break;
            case 2:
                targetGender = 'm';
                fileName = "MaleYoungVillager";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = M_A_YOUNG_VILLAGER;
                break;
            case 3:
                targetGender = 'f';
                fileName = "YoungGirl";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = F_A_YOUNG_GIRL;
                break;
            case 4:
                targetGender = 'f';
                fileName = "Girl";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = F_A_GIRL;
                break;
            case 5:
                targetGender = 'f';
                fileName = "FemaleFarmer";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = F_A_FARMER;
                break;
            case 6:
                targetGender = 'm';
                fileName = "MaleFarmer";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = M_A_FARMER;
                break;
            case 7:
                targetGender = 'm';
                fileName = "MaleOldMan";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_REFUGEE;
                sat = M_A_OLDMAN;
                break;
            case 8:
                targetGender = 'm';
                fileName = "Soldier";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_SOLDIER;
                sat = M_A_SOLDIER;
                break;
            case 9:
                targetGender = 'f';
                fileName = "FMW";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_SOLDIER;
                sat = F_A_SOLDIER;
                break;
            case 10:
                targetGender = 'm';
                fileName = "MaleBandit";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_BANDIT;
                sat = M_A_BANDIT;
                break;
            case 11:
                targetGender = 'f';
                fileName = "FemaleBandit";
                targetIdleF = 3;
                targetWalkF = 4;
                villagerClass = V_BANDIT;
                sat = F_A_BANDIT;
                break;
        }
        
        configContent = refugeeConfig;
        defaultContent = refugeeDefaults;
        targetClass = "refugee";
        
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
        gs->setAIConfig(configContent);
        gs->setDefaultsConfig(defaultContent);
        gs->villagerClass = villagerClass;
        gs->appearanceType = sat;
        
        allSprites->addObject(gs);
        configContent = "";
    }
    
    for(int i = 0; i < V_CLASS_END; i++)
    {
        std::string configContent;
        std::string defaultContent;
        std::string targetClass;
        VillagerClass villagerClass;
        
        switch (i)
        {
            case 0:
                configContent = refugeeConfig;
                defaultContent = refugeeDefaults;
                targetClass = "refugee";
                villagerClass = V_REFUGEE;
                break;
            case 1:
                configContent = citizenConfig;
                defaultContent = citizenDefaults;
                targetClass = "citizen";
                villagerClass = V_CITIZEN;
                break;
            case 2:
                configContent = farmerConfig;
                defaultContent = farmerDefaults;
                targetClass = "farmer";
                villagerClass = V_FARMER;
                break;
            case 3:
                configContent = builderConfig;
                defaultContent = builderDefaults;
                targetClass = "builder";
                villagerClass = V_BUILDER;
                break;
            case 4:
                configContent = soldierConfig;
                defaultContent = soldierDefaults;
                targetClass = "soldier";
                villagerClass = V_SOLDIER;
                break;
            case 5:
                configContent = banditConfig;
                defaultContent = banditDefaults;
                targetClass = "bandit";
                villagerClass = V_BANDIT;
                break;
            default:
                break;
        }
        
        SpriteClass* spriteClass = new SpriteClass();
        spriteClass->configContent = configContent;
        spriteClass->defaultContent = defaultContent;
        spriteClass->targetClass = targetClass;
        spriteClass->villagerClass = villagerClass;
        allSpriteClass->addObject(spriteClass);
    }
}

void SpriteHandler::AddToCache(cocos2d::CCSpriteBatchNode *spritesheet, std::string animName)
{
    std::string plistName = animName + ".plist";
}

/*
 * Only refugee and bandits can be added to the game without having a house.
 */
GameSprite* SpriteHandler::getSpriteByVillagerClass(VillagerClass villagerClass)
{
    //return NULL;
    if (!allSprites)
    {
        return NULL;
    }
    
    if (allSprites->count() == 0)
    {
        return NULL;
    }
    
    vector<int> idList = vector<int>();
    VillagerClass searchClass = V_REFUGEE;
    
    if(villagerClass == V_BANDIT)
    {
        searchClass = V_BANDIT;
    }
    
    for(int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
        if(gs->villagerClass == searchClass)
        {
            idList.push_back(i);
        }
    }
    
    if(idList.size() <= 0)
    {
        return NULL;
    }
    
    int temp = rand() % idList.size();
    return (GameSprite*) allSprites->objectAtIndex(idList.at(temp));
}

void SpriteHandler::addSpriteToMap(cocos2d::CCPoint &tilePos, VillagerClass villagerClass)
{
    GameSprite* targetSprite = getSpriteByVillagerClass(villagerClass);
    SpriteClass* spriteClass = GlobalHelper::getSpriteClassByVillagerClass(villagerClass);
    
    if (!targetSprite) return;
    
    GameSprite* newSprite = (GameSprite*)targetSprite->copy();
    newSprite->retain();
    
    newSprite->defaults_doc = spriteClass->defaultContent;
    newSprite->config_doc = spriteClass->configContent;
    newSprite->spriteClass = spriteClass->targetClass;
    newSprite->villagerClass = villagerClass;
    
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
    spritesOnMap->removeObject(sprite);
    
    //Order of execution! 
    GameHUD::getThis()->onSpriteRemovedFromMap(sprite);
    sprite->unmakeSprite();
    sprite->release();
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
     * The energy of villages will also decay, from 100 to 0.
     */
    cumulatedTime_energy += dt;
    
    if(cumulatedTime_energy >= 0.1f)
    {
        for(int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = ((GameSprite*) spritesOnMap->objectAtIndex(i));
            gs->getPossessions()->energyRating -= 0.05f;
            
            if(gs->getPossessions()->energyRating < 0)
            {
                gs->getPossessions()->energyRating = 0;
            }
        }
        
        cumulatedTime_energy = 0.0f;
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
    if(!GameScene::getThis()->banditsAttackHandler->warMode)
    {
        for(int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = ((GameSprite*) spritesOnMap->objectAtIndex(i));
            
            gs->scheduleToken(dt);
        }
    }
    
    /*
     * schedule the ren tokens, see whether it will start to fade out.
     */
    for (int i = 0; i < tokensOnMap->count(); i++)
    {
        ReputationOrb* ro = ((ReputationOrb*) tokensOnMap->objectAtIndex(i));
        if(ro != NULL)
        {
            ro->update(dt);
        }
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