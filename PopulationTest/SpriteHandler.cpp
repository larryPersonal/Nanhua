
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
#include "GameSpriteCFG.h"
#include "CitizenCFG.h"
#include "FarmerCFG.h"
#include "SoldierCFG.h"
#include "RefugeeCFG.h"
#include "BuilderCFG.h"
#include "BanditCFG.h"

#include "GlobalSettings.h"

#include "NameGenerator.h"

#include "GameDefaults.h"

#include <json/json.h>


SpriteHandler::~SpriteHandler()
{
    
    allSpriteSheets->removeAllObjects();
    CC_SAFE_RELEASE(allSpriteSheets);
    
//    allSpriteSheets->release();
    
    aliensOnMap->removeAllObjects();
//    aliensOnMap->release();
    
    CC_SAFE_RELEASE(aliensOnMap);
    
    localsOnMap->removeAllObjects();
//    localsOnMap->release();
    CC_SAFE_RELEASE(localsOnMap);
    
    for (int i = 0; i < spritesOnMap->count(); ++i)
    {
        ((GameSprite*)spritesOnMap->objectAtIndex(i))->unmakeSpriteEndGame();
        spritesOnMap->objectAtIndex(i)->release();
    }
    
    spritesOnMap->removeAllObjects();
    CC_SAFE_RELEASE(spritesOnMap);
    
    /*
    for (int i = 0; i < allSprites->count(); ++i)
    {
        allSprites->objectAtIndex(i)->release();
    }*/
    allSprites->removeAllObjects();
//    allSprites->release();
    CC_SAFE_RELEASE(allSprites);
    
    
    allClassRequirements->removeAllObjects();
//    allClassRequirements->release();
    CC_SAFE_RELEASE(allClassRequirements);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
    
}

void SpriteHandler::initialize()
{
    cumulatedTime = 0.0f;
    
    NameGenerator::init();
    
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
        spriteContent = gameSpriteDefaults;
        
        switch (i)
        {
            case 0:
                targetGender = 'm';
                fileName = "MMC";
                targetIdleF = 4;
                targetWalkF = 4;
                configContent = refugeeConfig;
                defaultContent = refugeeDefaults;
                targetClass = "refugee";

                break;
            case 1:
                targetGender = 'f';
                fileName = "FMC";
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

            case 10:
                targetGender = 'm';
                fileName = "MMW";
            
                targetIdleF = 3;
                targetWalkF = 4;
            
                configContent = banditConfig;
                defaultContent = banditDefaults;
                targetClass = "bandit";
            
                break;
            case 11:
                targetGender = 'f';
                fileName = "FMW";
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
        /*
        map->addChild(spriteSheet, map->layerNamed("Ground_1")->getZOrder()); 
         //basket, zindex is based on batchnode zindex, which means the sprites will always be lower than the buildings! disabled.
        */
        /*all spritesheets must be a child of the map*/
        AddToCache(NULL, fileName);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName.c_str());//  plistName.c_str());
        allSpriteSheets->addObject(spriteSheet);
        
      //  CCLog("spritesheet has zorder %i", spriteSheet->getZOrder());
        
        /*1 sprite per char. */
        GameSprite* gs = GameSprite::create();
        gs->gender = targetGender;
        gs->race = targetRace;
        gs->spriteName = fileName;
      //  gs->spriteName->retain();
        gs->setFrameCount(targetIdleF, targetWalkF);
        gs->batchLayerIndex = i;
        gs->spriteClass = targetClass;
      //  gs->spriteClass->retain();
        gs->type = SpriteType (i);
        gs->setAIConfig(configContent);
        gs->setDefaultsConfig(defaultContent);
        gs->setSpriteConfig(spriteContent);
        
     //   gs->setRequirementsConfig(reqContent);
        //set default animation here.
        
        
        
        
        allSprites->addObject(gs);
        configContent = "";
    
    }
    
    initClassRequirements();
}

void SpriteHandler::initClass(std::string classConfig)
{
    Json::Value defaultsRoot;
    Json::Reader reader;
    
    
    bool parsingSuccessful = reader.parse(classConfig, defaultsRoot);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse requirements doc\n" << reader.getFormatedErrorMessages() << std::endl;
        
    }
    else
    {
        Requirements* requirements = new Requirements();
        requirements->autorelease();
        std::string encoding = defaultsRoot.get("encoding", "UTF-8" ).asString();
        requirements->className = defaultsRoot["name"].asString();
        
        requirements->required_intelligence = atoi(defaultsRoot["required_int"].asString().c_str());
        requirements->required_loyalty = atoi(defaultsRoot["required_loyalty"].asString().c_str());
        requirements->required_social = atoi(defaultsRoot["required_social"].asString().c_str());
        requirements->required_education_level = atoi(defaultsRoot["required_education_level"].asString().c_str());
        
        allClassRequirements->addObject(requirements);
    }
    //  std::cout<<"created tree\n\n";
    //always reinitialize the behavior tree to point the current child back to the first left child
    
    defaultsRoot.clear();
    
}

void SpriteHandler::initClassRequirements()
{
    allClassRequirements = CCArray::create();
    allClassRequirements->retain();
    
    initClass(citizenRequirements);
    initClass(farmerRequirements);
    //initClass(banditRequirements);
    //initClass(refugeeRequirements);
    //initClass(builderRequirements);
    //initClass(soldierRequirements);
    
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
    newSprite->sprite_doc = targetSprite->sprite_doc;
    
    newSprite->makeSprite(&tilePos);
    
    spritesOnMap->addObject(newSprite);
    
    if (newSprite->race == 'a')
    {
        newSprite->fdaysLeft = alien_visa_duration_months * 28;
        aliensOnMap->addObject(newSprite);
    }
    else
        localsOnMap->addObject(newSprite);
    
    GameHUD::getThis()->onSpriteAddedToMap(newSprite);
    
    if (TutorialHandler::getThis() == NULL) return;
    if (TutorialHandler::getThis()->IsActive())
    {
        stringstream ss;
        ss << spritesOnMap->count();
        TutorialHandler::getThis()->ReportAction("count", "population", ss.str().c_str());
        if (newSprite->race == 'a')
            TutorialHandler::getThis()->ReportAction("spawncomplete", "alien", newSprite->spriteClass );
        else
            TutorialHandler::getThis()->ReportAction("spawncomplete", "mayan", newSprite->spriteClass );

    }
}

void SpriteHandler::loadSpriteToMap(cocos2d::CCPoint &tilePos, GameSprite *sp, std::string details)
{
    GameSprite* newSprite = (GameSprite*)sp->copy();
    newSprite->retain();
    newSprite->makeSprite(&tilePos);
    newSprite->internal_rank = getRank(newSprite->spriteClass);
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
            newSprite->getPossessions()->classLevel = atoi(tokens[8].c_str());
            
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
    // this is the section to detect the global hungry decay
    cumulatedTime += dt;
    
    if(cumulatedTime >= (1.0f / GlobalSettings::hungry_decay))
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

    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = ((GameSprite*) spritesOnMap->objectAtIndex(i));
        
        // check whether a sprite is hungry, if hungry, ask them to eat food
        if(gs->getPossessions()->currentHungry <= GlobalSettings::find_food_hungry_treshold && gs->currAction != EATING && gs->nextAction != EATING && gs->currAction && gs->getTargetLocation() != gs->getHome())
        {
            gs->goToEat();
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