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
    build_uint_current = 0;
    

    buildingBuyPrice = 0;
    constructionEndTime = 0.0f;
    constructionTime = 10.0f;
   
    researchCost = 100.0f;
    researchTime = 10.0f;
    
    jobsInthisBuilding = CCArray::create();
    jobsInthisBuilding->retain();
    
    
    currVisitors = CCArray::create();
    currVisitors->retain();
    
    ID = 0;
    currentExp = 0;
    currentLevel = 1;
    maintCost = 0;
    
    levelGainVacancy = NULL;
    levelGainPopOverload = NULL;
    levelGainPrice = NULL;
    
    expToLevel = CCArray::create();
    expToLevel->retain();
    
    
    
    
    unlocked = false;
    
    required_building_count = required_capita = required_population = 0;
}
Building::~Building()
{
    EndAnim();
    
      expToLevel->removeAllObjects();
    expToLevel->release();
    
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
   
    
    jobsInthisBuilding->removeAllObjects();
    jobsInthisBuilding->release();
    
    
    currVisitors->removeAllObjects();
   
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
        
        
        pCopy->build_uint_required = this->build_uint_required;
        pCopy->build_uint_current = 0;
        
        pCopy->buildingRect = this->buildingRect;
        pCopy->populationLimit = this->populationLimit;
        
        pCopy->jobs = this->jobs;
        
        
        // for storage.
        pCopy->currentStorage = this->currentStorage;
        pCopy->storageLimit = this->storageLimit;
        
        pCopy->build_uint_required = this->build_uint_required;
        
        pCopy->buildingType = this->buildingType;
        pCopy->buildingValue = this->buildingValue;
        pCopy->buildingBuyPrice = this->buildingBuyPrice;
        pCopy->ID = -1;  /*IDs will be assigned only after construction completes! *///GameScene::getThis()->buildingHandler->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
        pCopy->targetGUID = this->targetGUID;   // Copy the target GUID too
        pCopy->currentExp = this->currentExp;
        pCopy->expToLevel->initWithArray(this->expToLevel);
        
        pNewZone = new CCZone(pCopy);
    }
    
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

    
    sp->setAction(sp->nextAction);

    
    
    //if this isn't the character's job, home or an upgrade location, modify all stats based on building stats.
    //ModifyStats(sp);
    this->gainExp(10);
    
    
   // sp->ChangeSpriteTo(GameScene::getThis()->spriteHandler->getSpriteByType(M_MAYAN_FARMER));
}

/* use this to determine what happens when a sprite spends an update cycle IDLE in the building. */
void Building::StickAroundHandler(GameSprite *sp)
{
    if (!sp) return;
    if (this->build_uint_required > this->build_uint_current)
    {
        
        if (sp->spriteClass.compare("builder") == 0)
        {
            
            int workval = sp->getPossessions()->PerformTask();
            if (workval > 0)
       
                this->build_uint_current += workval;
            else
            {
                //TODO: sprite has run out of energy.
                
                //what do I want to do here? Meanwhile he keeps working even if he has no energy. Must go home and rest I think.
            }
            
        }
        
        
        return;
        
    }
    
    
    if (buildingType != HOUSING)
    {
        if (buildingType == AMENITY) //where Amenity == House that creates stuff. A Farm is one of them.
        {
            if (sp->spriteClass.compare("farmer") == 0)
            {
                if (sp->getPossessions()->jobLocation == this && sp->getAction() == FARMING)
                {
                    int workval = sp->getPossessions()->PerformTask();
                    if (workval > 0)
                    {
                        
                    }
                    else
                    {
                        //TODO: sprite has run out of energy.
                        
                        //what do I want to do here? Meanwhile he keeps working even if he has no energy. Must go home and rest I think.
                    }
                }
            }
        }
        
        
    }
    else
    {
        //is a house. Rest?
        if (sp->getPossessions()->homeLocation == this) //if this is my home
        {
          
            sp->getPossessions()->Rest(); //recharge energy
        }
    }
}


void Building::Leavehandler(GameSprite *sp)
{
    CCLog("Leave handler called");
    currVisitors->removeObject(sp);
}
//Doing it in the copy constructor unfortunately fails. Doing this in a separate function instead.

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