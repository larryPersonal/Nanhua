 //
//  Building.cpp
//  PopulationTest
//
//  Created by Serious Games on 13/3/13.
//
//

#include "Building.h"
#include "GameScene.h"
#include "Sprite.h"
#include "GameHUD.h"
#include "InfoMenu.h"
#include <iterator>

Building::Building()
{
    buildingValue = 0;
    populationLimit = 0;
    
    // for storage.
    currentStorage = 0;
    storageLimit = 0;
    
    build_uint_required = 0;
    
   // populationOverload = populationOverloadLimit = 0;
    buildingBuyPrice = 0;
    constructionEndTime = 0.0f;
    constructionTime = 10.0f;
    researchCost = 100.0f;
    researchTime = 10.0f;
    //jobsInthisBuilding = CCArray::create();
    //jobsInthisBuilding->retain();
    
    currVisitors = CCArray::create();
    currVisitors->retain();
    
    ID = 0;
    /*
    exp_mod = 0;
    cash_mod = 0;
    social_mod = 0;
    happiness_mod = 0;
    energy_mod = 0;
    loyalty_mod = 0;
    int_mod = 0;
    */
    currentExp = 0;
    currentLevel = 1;
    maintCost = 0;
    
    levelGainVacancy = NULL;
    levelGainPopOverload = NULL;
    levelGainPrice = NULL;
    
  //  expToLevel = NULL;
    expToLevel = CCArray::create();
    expToLevel->retain();
    //  pCopy->expToLevel->retain();
    
    
    unlocked = false;
    
    required_building_count = required_capita = required_population = 0;
}
Building::~Building()
{
    EndAnim();
    
    //buildingTexture->release();
    /*
    
    for (int i = 0; i < jobsInthisBuilding->count(); ++i)
    {
        delete jobsInthisBuilding->objectAtIndex(i);
    }*/
    /*
    jobsInthisBuilding->removeAllObjects();
    jobsInthisBuilding->release();
    */
   // if (expToLevel != NULL)
   // {
       /*
        for (int i = 0; i < expToLevel->count(); ++i)
        {
            expToLevel->objectAtIndex(i)->release();
        }*/
    expToLevel->removeAllObjects();
    expToLevel->release();
    // }
    
    if (levelGainVacancy)
    {
        levelGainVacancy->removeAllObjects();
        levelGainVacancy->release();
        
    }
    
    if (levelGainPopOverload)
    {
        levelGainPopOverload->removeAllObjects();
        levelGainPopOverload->release();

    }
    
    if (levelGainPrice)
    {
        levelGainPrice->removeAllObjects();
        levelGainPrice->release();    }
    
    currPopulation.clear();
   
    
    currVisitors->removeAllObjects();
//    currVisitors->release();
    
    currVisitors->release();
}

Building* Building::create()
{
    Building *pRet = new Building();
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

Building* Building::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    Building* pCopy = NULL;
    
    if(pZone)
    {
        //in case of being called at sub class
        pCopy = (Building*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = Building::create();
        pCopy->buildingName = this->buildingName;
        pCopy->buildingDesc = this->buildingDesc;
         pCopy->targetLayerName = this->targetLayerName;
      
        pCopy->width = this->width;
        pCopy->height = this->height;
        pCopy->buildingCost = this->buildingCost;
        pCopy->buildingTexture = this->buildingTexture;
  
        //pCopy->buildingTexture->retain();
        pCopy->constructionTime = this->constructionTime;
        pCopy->researchTime = this->researchTime;
        pCopy->researchCost = this->researchCost;
        
        pCopy->buildingRect = this->buildingRect;
        pCopy->populationLimit = this->populationLimit;
        
        // for storage.
        pCopy->currentStorage = this->currentStorage;
        pCopy->storageLimit = this->storageLimit;
        
        pCopy->build_uint_required = this->build_uint_required;
        
        pCopy->buildingType = this->buildingType;
        pCopy->buildingValue = this->buildingValue;
        pCopy->buildingBuyPrice = this->buildingBuyPrice;
     //   pCopy->buildingRentPrice = this->buildingRentPrice;
        pCopy->ID = -1;  /*IDs will be assigned only after construction completes! *///GameScene::getThis()->buildingHandler->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
        pCopy->targetGUID = this->targetGUID;   // Copy the target GUID too
       // pCopy->jobs = this->jobs;
       // pCopy->populationOverload = this->populationOverload;
       // pCopy->populationOverloadLimit = this->populationLimit + this->populationOverload;
        
        //unlocking doesn't matter, that's for the benefit of the build menu and research menu only.
        
        //Mods
        /*
        pCopy->exp_mod = this->exp_mod;
        pCopy->cash_mod = this->cash_mod;
        pCopy->social_mod = this->social_mod;
        pCopy->happiness_mod = this->happiness_mod;
        pCopy->energy_mod = this->energy_mod;
        pCopy->loyalty_mod = this->loyalty_mod;
        pCopy->int_mod = this->int_mod;
        */
        pCopy->currentExp = this->currentExp;
   //    pCopy->expToLevel = CCArray::create();
        pCopy->expToLevel->initWithArray(this->expToLevel);
    //    pCopy->retain();
        
        pNewZone = new CCZone(pCopy);    }
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCPoint Building::getWorldPosition()
{
    if (!buildingRep) return CCPointMake(-1, -1);
    return ccpAdd(buildingRep->getPosition(), GameScene::getThis()->mapHandler->getMap()->getPosition());
    
    //return GameScene::getThis()->mapHandler->getMap()->convertToWorldSpace(buildingRep->getPosition());
}

int Building::getExpToLevel()
{
    return ((CCInteger*)expToLevel->objectAtIndex(currentLevel - 1))->getValue();
}
/*
CCArray* Building::getJobsAvailable()
{
    return jobsInthisBuilding;
}
*/
/*use this to determine what happens when a sprite enters the building.*/
void Building::ArriveHandler(GameSprite* sp)
{
    if (!sp) return;
    
    currVisitors->addObject(sp);

    
    
    /*deal with the case where a building is the guy's home first.*/
    /* A guy can't possibly work in his home, so.*/
    if (buildingType == HOUSING)
    {
        if (sp->getPossessions()->homeLocation)
        {
            if (sp->getPossessions()->homeLocation == this)
            {
                CCLog("reached home. Energy reset.");
                sp->increasePossession(STATS_ENERGY, sp->getPossessions()->defaultEnergy);
                
                CCLog("taking stock of day spent.");
                sp->TakeStockOfDay();
            }
        }
        return;
    }
    

        
     
        
    
    
    /*if the building happens to be the job location*/
    /*
    if (sp->getPossessions()->jobLocation)
    {
        if (sp->getPossessions()->jobLocation == this)
        {
            CCLog("reached workplace.");
            Job* j = (Job*)getJobsAvailable()->objectAtIndex(sp->getPossessions()->jobIndex);
            j->ModifyStats(sp);
            //sp->increasePossession(STATS_ENERGY, -j->energy_mod);
            //sp->increasePossession(STATS_EXP, j->exp_mod);
            
            //todo: check if payday. If it is,
            //sp->getPossessions()->cashOnHand += j->job_monthlyPay;
           
        }
         return;
    }
    */
    /*
    if the building is neither a home nor a job location, check if building is an upgrade location.
    sp->CheckUpgrade();
    */
    
    
    //only commerce buildings are shoppping locations. Note that at this point I've already determined that it's not the sprite's job location.
    
    /*shopping should only be a one time cost*/
    /*
    if (buildingType == COMMERCE)
    {
        sp->increasePossession(STATS_CASHONHAND, cash_mod * currentLevel);
        
        GameManager::getThis()->currentMoney += (abs(cash_mod) * currentLevel);
        GameHUD::getThis()->updateMoneyLabel();
    
    }*/
    
    
    
    //if this isn't the character's job, home or an upgrade location, modify all stats based on building stats.
    ModifyStats(sp);
    this->gainExp(10);
    
    
   // sp->ChangeSpriteTo(GameScene::getThis()->spriteHandler->getSpriteByType(M_MAYAN_FARMER));
}

/* use this to determine what happens when a sprite spends an update cycle IDLE in the building. */
void Building::StickAroundHandler(GameSprite *sp)
{
    if (!sp) return;
    if (buildingType != HOUSING)
        ModifyStats(sp);
    
    
    //social interaction disabled
    /*
    if (buildingType == SOCIAL && sp->isInteractingSocial)
    {
        if (currVisitors->count() > 0)
        {
            for (int i = 0; i < currVisitors->count(); ++i)
            {
                GameSprite* spr = (GameSprite*)currVisitors->objectAtIndex(i);
                
                if (spr != sp)
                {
                    if (spr->isInteractingSocial)
                    {
                        if ((sp->race == 'a' && spr->race == 'h') ||
                            (sp->race == 'h' && spr->race == 'a'))
                        {
                            sp->getPossessions()->socialRating += 10;
                            spr->getPossessions()->socialRating += 10;
                            
                            spr->isInteractingSocial = false;
                            sp->isInteractingSocial = false;
                        }
                    }
                }
            }
        }
    }*/
}


void Building::Leavehandler(GameSprite *sp)
{
    CCLog("Leave handler called");
    currVisitors->removeObject(sp);
}
//Doing it in the copy constructor unfortunately fails. Doing this in a separate function instead.
/*
void Building::initializeJobs()
{
    if (jobs.length() == 0) return;
 
    JobCollection* jc = GameScene::getThis()->jobCollection;
     std::istringstream ss(jobs);
     do
     {
         std::string substr;
         ss >> substr;
     
     
         Job* j = jc->getJobWithName(substr.c_str());
         if (j != NULL)
         {
             
             jobsInthisBuilding->addObject(j);
         }
     }
     while (ss);
    
}
*/

void Building::ModifyStats(GameSprite *sp)
{
    //note: it is possible to supply negatives to this.
    if (sp == NULL) return;
    
    /*
    if (buildingType == COMMERCE)
    {
        GameManager::getThis()->currentMoney += (abs(cash_mod) * currentLevel);
        GameHUD::getThis()->updateMoneyLabel();
    }*/
    
    /*
    sp->increasePossession(STATS_ENERGY, energy_mod);
    sp->increasePossession(STATS_HAPPINESS, happiness_mod);
    sp->increasePossession(STATS_INTELLIGENCE, int_mod);
    sp->increasePossession(STATS_SOCIAL, social_mod);
    sp->increasePossession(STATS_LOYALTY, loyalty_mod);
    */
    /*
    if (sp->race != 'a' && !sp->getPossessions()->isAtAbsoluteMax() && !sp->getPossessions()->isAtMaxLevel()) //aliens don't "learn" unless conversion happens.
        sp->increasePossession(STATS_EXP, exp_mod);
    */
}

void Building::gainExp(int expToGain)
{
    currentExp += expToGain;
    
    if (this->currentExp >= getExpToLevel())
    {
        int overflow = currentExp - getExpToLevel();
        this->currentExp = getExpToLevel();
        
        //Level up if my level isn't max yet
        if (this->currentLevel < expToLevel->count())
        {
            this->currentLevel++;
            this->currentExp = 0;
            
            // Update GameHUD
            if (GameHUD::getThis()->getMenuMode() == 6)
            {
                if (InfoMenu* menu = dynamic_cast<InfoMenu*>(PopupMenu::getBottommostPopupMenu()))
                    menu->updateItemLevelLabel(this);
            }
        
            //increase stats here **
            Building* buildingData = GameScene::getThis()->buildingHandler->getBuildingWithGID(targetGUID);
        
            CCInteger* gain = (CCInteger*)buildingData->levelGainVacancy->objectForKey(currentLevel);
            if (gain != NULL)
            {
                this->populationLimit += gain->getValue();
             //   this->populationOverloadLimit = this->populationLimit + this->populationOverload;
            }
            
            gain = (CCInteger*)buildingData->levelGainPopOverload->objectForKey(currentLevel);
            if (gain != NULL)
            {
               // this->populationOverload += gain->getValue();
               // this->populationOverloadLimit = this->populationLimit + this->populationOverload;
            }
        
            gain = (CCInteger*)buildingData->levelGainPrice->objectForKey(currentLevel);
            if (gain != NULL)
                this->buildingCost += gain->getValue();
            //** end increase stats
            
            // recursive call for multi-leveling
            gainExp(overflow);
        }
    }
}

void Building::addPopulation(GameSprite* sprite)
{
    if (sprite)
        currPopulation.push_back(sprite);
}

void Building::removePopulation(GameSprite* sprite)
{
    currPopulation.erase(std::remove(currPopulation.begin(), currPopulation.end(), sprite), currPopulation.end());
}

int Building::getPopulationCount()
{
    return currPopulation.size();
}

GameSprite* Building::getPopulationAt(int index)
{
    return currPopulation.at(index);
}

int Building::getLevel()
{
    return currentLevel;
}

void Building::unlockToResearch()
{
    required_building_count = 0;
    required_capita = 0;
    required_population = 0;
    prereq = "";
    
    //and the updateUnlocks in game manager will deal with this
    //note: only difference is the unlocked flag. Flipping that puts the building in the BUILD immediately.
    GameManager::getThis()->UpdateUnlocks();
}

void Building::unlockToBuild()
{
    required_building_count = 0;
    required_capita = 0;
    required_population = 0;
    prereq = "";
    unlocked = true;
    //and the updateUnlocks in game manager will deal with this
    GameManager::getThis()->UpdateUnlocks();

}

bool Building::hasMetUnlockCriteria()
{
    if (prereq.length() != 0)
    {
        istringstream iss(prereq);
        vector<std::string> tokens;
        std::string token;
        
        while(std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        
        int truecount = 0;
        for (int i = 0; i < tokens.size(); ++i)
        {
            if (GameScene::getThis()->buildingHandler->getBuildingOnMapWithName(tokens[i]) != NULL)
            {
                ++truecount;
            }
        }
    
        if (truecount != tokens.size()) return false;
    }
    //if prereqs.length = 0 assume true at this point.
    
    if (required_building_count > GameScene::getThis()->buildingHandler->allBuildingsOnMap->count()) return false;
    if (required_population > GameScene::getThis()->spriteHandler->spritesOnMap->count()) return false;
    if (required_capita > GameManager::getThis()->getCurrentMoney()) return false;
    
    return true;
}

bool Building::isOverpopulated()
{
    return (currPopulation.size() > populationLimit);
}


//changes the building's appearance only. The building's current stats must not be changed!
//ideally, other appearances should be implemented as Dummys on the TMX file, right next to the actual building record. This is
//so the GIDs can be 
//currently only used for Farms.
void Building::ChangeAppearance(Building *b)
{
    if (b==NULL) return;
    CCSprite* newRep = CCSprite::create();
    newRep->initWithTexture(b->buildingTexture, b->buildingRect);
  //  newRep->retain();
    CCPoint tilePos = buildingRep->getPosition();
    float z = buildingRep->getZOrder();
    newRep->setPosition(tilePos);
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(buildingRep);
    
  //  buildingRep->release();
    
    buildingRep = newRep;
    GameScene::getThis()->mapHandler->getMap()->addChild(buildingRep, z);
    
}

void Building::BeginAnim()
{
        if (buildingName.compare("Farm") == 0)
        {
            GUID_offset = targetGUID;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Building::AnimUpdate), this, 10.0f, false);
            
           
        }
    
}


void Building::EndAnim()
{
        if (buildingName.compare("Farm") == 0)
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Building::AnimUpdate), this);
        }

}

void Building::AnimUpdate()
{
    if (buildingRep == NULL) return;
    if (!GameScene::getThis()) return;
    if (GUID_offset < (targetGUID +3))
        ++GUID_offset;
    else
        GUID_offset = targetGUID;
    ChangeAppearance(GameScene::getThis()->buildingHandler->getBuildingWithGID(GUID_offset));
    
}

int Building::getOverpopulateCount()
{
    return (currPopulation.size() - populationLimit);
}

int Building::getAlienPopulationCount()
{
    int count = 0;
    for (int i = 0; i < currPopulation.size(); ++i)
    {
        if (currPopulation[i]->race == 'a') ++count;
    }
    return count;
}

int Building::getUnoccupiedCount()
{
    //Only residences count from population
    //if (buildingType == HOUSING)
    return (populationLimit - currPopulation.size());
    /*
    else
    {
        int count = 0;
     
        for (int i = 0; i < jobsInthisBuilding->count(); ++i)
        {
            if (((Job*) jobsInthisBuilding->objectAtIndex(i))->job_isTaken == false)
            {
                ++count;
            }
        }
        return count;
     
    }*/
    
}