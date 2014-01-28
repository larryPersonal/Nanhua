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
#include "BuildingInfoMenu.h"
#include <iterator>

Building::Building()
{
    farmState = WAITING;
    
    buildingValue = 0;
    populationLimit = 0;
    
    // for storage.
    currentStorage = 0;
    storageLimit = 0;
    
    build_uint_required = 0;
    build_uint_current = 0;
    
    work_unit_current = 0;
    work_unit_required = 0;
    
    recovery_rate = 0;
    food_consumption_rate = 0;

    buildingBuyPrice = 0;
    constructionEndTime = 0.0f;
    constructionTime = 10.0f;
   
    researchCost = 100.0f;
    researchTime = 10.0f;
    
    jobsInthisBuilding = CCArray::create();
    jobsInthisBuilding->retain();
    
    
    currVisitors = CCArray::create();
    currVisitors->retain();
    
    memberSpriteList = CCArray::create();
    memberSpriteList->retain();
    
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
    
    
    memberSpriteList->removeAllObjects();
    memberSpriteList->release();
    
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
        
        pCopy->work_unit_required = this->work_unit_required;
        pCopy->work_unit_current = 0;
        
        pCopy->buildingRect = this->buildingRect;
        pCopy->populationLimit = this->populationLimit;
        
        pCopy->jobs = this->jobs;
        
        
        // for storage.
        pCopy->currentStorage = this->currentStorage;
        pCopy->storageLimit = this->storageLimit;
        
        pCopy->build_uint_required = this->build_uint_required;
        
        pCopy->food_consumption_rate = this->food_consumption_rate;
        pCopy->recovery_rate = this->recovery_rate;
        
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
    
    if(sp->currAction == RESTING)
    {
        
    }
    
    if(sp->currAction == EATING && sp->getTargetLocation() == this)
    {
        //sp->setTargetHungry(sp->getPossessions()->energyRating + 15 > sp->getPossessions()->defaultEnergy ? sp->getPossessions()->defaultEnergy : sp->getPossessions()->energyRating + 15);
    }
    
    if(sp->getJobLocation() == this)
    {
        sp->setIsDoingJob(true);
    }
}

// when a villager performs a task in a building, this will direct the villager's behavior
void Building::StickAroundHandler(GameSprite *sp, float dt)
{
    // defensive programming (no villager)
    if (!sp) return;
    
    // when villager's action is build and villager has a job of builder and villager is doing his/her job, trigger for the behavior
    if(sp->currAction == BUILD && sp->getIsDoingJob() && sp->getJob() == BUILDER && sp->spriteClass.compare("builder") == 0)
    {
        // if the building is under construction, construct the building
        if (isUnderConstruction())
        {
            int workval = sp->getPossessions()->PerformTask();
            
            // workval > 0 means the construction is successful, so update the construction scale and the energy scale
            if(workval > 0)
            {
                this->build_uint_current += workval;
                
                // update the building infor menu.
                //if(GameScene::buildingInfoMenu != NULL && GameScene::buildingInfoMenu->getBuilding() == this)
                //{
                    //GameScene::buildingInfoMenu->updateConstructionStatus();
                //}
                
                
                
                sp->updateIdleDelay(0.2f);
            }
            else
            // workval = 0, means the the villager is out of energy
            {
                //sp->currAction = IDLE;
                //sp->nextAction = IDLE;
                //sp->setTargetLocation(NULL);
               // sp->setIsFollowingMovementInstruction(false);
                //
                ///sp->setTargetLocation(sp->getHome());
                //sp->GoRest(sp->getHome());
            }
        }
        else
        // the building has finished the construction process, the villagers will leave the building and become idle
        {
            for(int i = 0; i < memberSpriteList->count(); i++)
            {
                CCLog("test4");
                GameSprite* gameS = (GameSprite*) memberSpriteList->objectAtIndex(i);
                
                gameS->currAction = IDLE;
                gameS->nextAction = IDLE;
                gameS->setIsDoingJob(false);
                gameS->spriteClass = "citizen";
                gameS->setJob(NONE);
                gameS->setJobLocation(NULL);
                gameS->setTargetLocation(NULL);
                gameS->changeToCitizen();
            }
            memberSpriteList->removeAllObjects();
        }
    }
    else if(sp->currAction == GET_HOME && sp->getTargetLocation() == this && sp->getPossessions()->targetHomeLocation == this)
    {
        sp->currAction = IDLE;
        sp->nextAction = IDLE;
        sp->futureAction1 = IDLE;
        sp->futureAction2 = IDLE;
        sp->setTargetLocation(NULL);
        sp->getPossessions()->targetHomeLocation = NULL;
        if(memberSpriteList->count() < populationLimit)
        {
            
            sp->setHome(this);
            sp->setIsDoingJob(false);
            sp->spriteClass = "citizen";
            sp->changeToCitizen();
            memberSpriteList->addObject(sp);
        }
    }
    else if(sp->currAction == EATING && sp->getTargetLocation() == this)
    // eating
    {
        // if there is still food in the granary
        if(hasFood())
        {
            // if the sprite is still hungry
            if (sp->isHungry())
            {
                int gainVal = sp->getPossessions()->EatFood();
                
                if(gainVal > 0)
                {
                    this->currentStorage -= food_consumption_rate;
                    sp->getPossessions()->currentHungry += gainVal;
                    sp->updateIdleDelay(0.2f);
                }
            }
            else
                // the sprite has finished eating the food, the villagers will leave the building and become idle
            {
                //sp->getPossessions()->energyRating = sp->getPossessions()->targetHungry;
                leaveGranuary(sp);
            }
        }
        else
        // no food anymore sprite will leave the granary (probably with penalty? I guess!)
        {
            leaveGranuary(sp);
            // add the penalty here!
        }
    }
    else if(sp->currAction == FARMING && sp->getTargetLocation() == this && sp->getJob() == FARMER)
    // farming
    {
        // if the farming process not finished!
        if(notHavest() && farmState == FARM && currentStorage <= 0)
        {
            // a farmer will never quit a farming job naturally
            //int workval = sp->getPossessions()->PerformTask();
            
            // workval > 0 means the farming process is successful, so update the farming scale and the energy scale
            if(sp->getPossessions()->energyRating >= 0)
            {
                sp->setCumulativeTime(sp->getCumulativeTime() + dt);
                if(sp->getCumulativeTime() >= 0.33333333f)
                {
                    sp->getPossessions()->energyRating -= 0.1f;
                    work_unit_current += sp->getPossessions()->default_work_unit_per_day / 12.0f;
                    sp->setCumulativeTime(0.0f);
                }
                
                if(work_unit_current >= work_unit_required)
                {
                    work_unit_current = 0;
                    currentStorage = storageLimit;
                }
                
                if(sp->getPossessions()->energyRating < 0)
                {
                    sp->getPossessions()->energyRating = 0;
                }
                sp->updateIdleDelay(0.2f);
            }
            else
                // workval = 0, means the the villager is out of energy
            {
                //sp->currAction = IDLE;
                //sp->nextAction = IDLE;
                //sp->setTargetLocation(NULL);
                //sp->setIsFollowingMovementInstruction(false);
                
                //sp->setTargetLocation(sp->getHome());
                //sp->GoRest(sp->getHome());
            }
        }
        else
        // the farming process has been finished, the farm goes into harvest state and it will not cumulate farm point any more.
        {
            // if the state of the farm is farm, then the farm finishes farming process, so the state changes to harvest.
            if(farmState == FARM)
            {
                farmState = HARVEST;
            }
            
            // if the state of the farm is harvest, trigger the sprites (farmers to havest the food)
            if(farmState == HARVEST)
            {
                sp->setJob(DELIVERER);
                if(currentStorage <= sp->getPossessions()->default_food_carriage_limit)
                {
                    sp->setFoodCarried(currentStorage);
                    currentStorage = 0;
                    farmState = FARM;
                    
                    // finish farming process....
                    for(int i = 0; i < memberSpriteList->count(); i++)
                    {
                        GameSprite* gameS = (GameSprite*) memberSpriteList->objectAtIndex(i);
                        if(gameS->getJob() == FARMER || (gameS->getJob() == DELIVERER && gameS->getFoodCarried() <= 0))
                        {
                            // all farmer and delivery without carring food will leave the job
                            // if has scheduled task, do the task, otherwise, leave the building
                            gameS->currAction = IDLE;
                            gameS->nextAction =IDLE;
                            gameS->setIsDoingJob(false);
                            gameS->spriteClass = "citizen";
                            gameS->setJob(NONE);
                            gameS->setJobLocation(NULL);
                            gameS->setTargetLocation(NULL);
                            gameS->changeToCitizen();
                        }
                    }
                }
                else
                {
                    sp->setFoodCarried(sp->getPossessions()->default_food_carriage_limit);
                    currentStorage -= sp->getFoodCarried();
                }
                
                Building* gran = sp->findNearestGranary(true);
                if(gran != NULL)
                {
                    sp->setTargetLocation(gran);
                    sp->GoGranary(gran);
                }
            }
        }

    }
    else if(sp->currAction == STORING && sp->getTargetLocation() == this && sp->getJob() == DELIVERER)
    // the delivery reach the granary and store the food
    {
        // if the granary can only store some of the food
        if(currentStorage + sp->getFoodCarried() > storageLimit)
        {
            currentStorage = storageLimit;
            sp->setFoodCarried(currentStorage + sp->getFoodCarried() - storageLimit);
            // TODO: go next granary
        }
        else
        // all food has been stored into the granary, go back to the farm
        {
            currentStorage += sp->getFoodCarried();
            sp->setFoodCarried(0);
            
            // check the whether the job location still has food to be transported
            if(sp->getJobLocation()->currentStorage > 0)
            {
                sp->setJob(FARMER);
                sp->setTargetLocation(sp->getJobLocation());
                sp->GoFarming(sp->getJobLocation());
            }
            else
            // no more food already, job has been completed, quit the job.
            {
                    sp->currAction = IDLE;
                    sp->nextAction = IDLE;
                    sp->setIsDoingJob(false);
                    sp->spriteClass = "citizen";
                    sp->setJob(NONE);
                    sp->setJobLocation(NULL);
                    sp->setTargetLocation(NULL);
                    sp->changeToCitizen();
            }
        }
    }
    else if (this->recovery_rate > 0 && this->food_consumption_rate == 0)
    // if the house is a place for resting
    {
        // if the sprite is resting now and the sprite is not a refugee, update the resting process
        if (sp->currAction == RESTING)
        {
            // if it is the home house of the sprite, recharge energy
            if(sp->getHome() == this)
            {
                // if the sprite is fully recharged it will leave the home
                if(sp->getPossessions()->energyRating >= sp->getPossessions()->default_energy_limit)
                {
                    leaveHouse(sp);
                }
                else
                    // if the sprite is not fully recharged, it will recharge energy.
                {
                    sp->getPossessions()->Rest();
                }
            }
            else
                // if it is not the home house, leave the house
            {
                // TODO::because the house is not sprite's home, so the spirte still does not have been recharged, to have enough energy to do other tasks, it needs to be recharged first, so try to find his home in other place, if no home, just wander around....
                leaveHouse(sp);
            }
        }
    }
    else
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
}

void Building::leaveHouse(GameSprite* sp)
{
    /*
    if(sp->getJobLocation() != NULL)
    // if the sprite has a job
    {
        sp->setTargetLocation(sp->getJobLocation());
        // if the job is builder
        if(sp->getJobLocation()->isUnderConstruction())
        {
            sp->GoBuild(sp->getJobLocation());
        }
        else if(sp->getJobLocation()->storageLimit > 0 && sp->getJobLocation()->food_consumption_rate == 0)
        // if the job is farmer
        {
            // farmer will always go to the farm to continue work!
            sp->GoFarming(sp->getJobLocation());
        }
        else
        {
            // for some one time job, when finishes the task, the sprite will change back to citizen!
            sp->currAction = IDLE;
            sp->nextAction = IDLE;
            sp->setIsDoingJob(false);
            sp->setJob(NONE);
            sp->spriteClass = "citizen";
            sp->setJob(NONE);
            sp->setJobLocation(NULL);
            sp->setTargetLocation(NULL);
            sp->changeToCitizen();
        }
        sp->setIsFollowingMovementInstruction(false);
    }
    else
    // the sprite does not have a job, just leave the hosue
    {
    */
        sp->currAction = IDLE;
        sp->nextAction = IDLE;
        sp->setTargetLocation(NULL);
        sp->setIsFollowingMovementInstruction(false);
        sp->futureAction1 = IDLE;
        sp->futureAction2 = IDLE;
    /*}*/
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
    /*
    if (buildingName.compare("Farm") == 0)
        {
            GUID_offset = targetGUID;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Building::AnimUpdate), this, 10.0f, false);
            
           
        }
    */
}


void Building::EndAnim()
{
    /*
    if (buildingName.compare("Farm") == 0)
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Building::AnimUpdate), this);
        }
*/
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

bool Building::isUnderConstruction()
{
    return build_uint_current < build_uint_required;
}

bool Building::hasFood()
{
    return currentStorage > 0;
}

bool Building::notHavest()
{
    return work_unit_current < work_unit_required;
}

void Building::leaveGranuary(GameSprite* sp)
{
    if(sp->getJob() == FARMER)
    {
        //sp->getPossessions()->energyRating = sp->getPossessions()->targetHungry;
        sp->setTargetLocation(sp->jobLocation);
        sp->GoFarming(sp->jobLocation);
    }
    else if(sp->getJob() == BUILDER)
    {
        CCPoint startPos = sp->getWorldPosition();
        CCPoint endPos = sp->jobLocation->getWorldPosition();
        
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        endPos = GameScene::getThis()->mapHandler->tilePosFromLocation(endPos);
        
        if(startPos.equals(endPos))
        {
            currVisitors->addObject(sp);
            sp->currAction = BUILD;
            sp->setTargetLocation(sp->jobLocation);
            sp->isDoingJob = true;
        }
        else
        {
            sp->setTargetLocation(sp->jobLocation);
            sp->GoBuild(sp->jobLocation);
        }
    }
    else
    {
        sp->setTargetLocation(NULL);
        if(sp->futureAction1 == RESTING || sp->futureAction2 == RESTING)
        {
            sp->futureAction1 = RESTING;
            sp->futureAction2 = IDLE;
        }
        else
        {
            sp->futureAction1 = IDLE;
            sp->futureAction2 = IDLE;
        }
        sp->currAction = IDLE;
        sp->nextAction = IDLE;
    }
}