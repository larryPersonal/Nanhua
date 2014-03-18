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
    
    inProgress = false;
    
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
    
    upgrade_unit_max = 0;
    current_upgrade_unit = 0;
    
    memberSpriteList = CCArray::create();
    memberSpriteList->retain();
    
    ID = 0;
    currentExp = 0;
    currentLevel = 0;
    maintCost = 0;
    
    levelGainVacancy = NULL;
    levelGainPopOverload = NULL;
    levelGainPrice = NULL;
    
    expToLevel = CCArray::create();
    expToLevel->retain();
    
    unlocked = false;
    
    isCurrentConstructing = false;
    isCurrentUpgrading = false;
    isCurrentHarvest = false;
    
    required_building_count = required_capita = required_population = 0;
    
    cumulatedTimeUpgrading = 0;
}
Building::~Building()
{
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
        levelGainPrice->release();
    }
    
    currPopulation.clear();
    
    memberSpriteList->removeAllObjects();
    memberSpriteList->release();
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
        pCopy->upgrade_unit_max = this->upgrade_unit_max;
        pCopy->researchCost = this->researchCost;
        
        pCopy->build_uint_required = this->build_uint_required;
        pCopy->build_uint_current = 0;
        
        pCopy->work_unit_required = this->work_unit_required;
        pCopy->work_unit_current = 0;
        
        pCopy->buildingRect = this->buildingRect;
        pCopy->populationLimit = this->populationLimit;
        pCopy->builderLimit = this->builderLimit;
        
        pCopy->number_of_jobs = this->number_of_jobs;
        
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
        pCopy->baseGID = this->baseGID;   // Copy the target GUID too
        pCopy->maxGID = this->maxGID;
        pCopy->currGID = pCopy->baseGID;
        pCopy->animframe_count = this->animframe_count;
        
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
    
}

int Building::getExpToLevel()
{
    return ((CCInteger*)expToLevel->objectAtIndex(currentLevel - 1))->getValue();
}

/*use this to determine what happens when a sprite enters the building.*/
void Building::ArriveHandler(GameSprite* sp)
{
    if (!sp) return;

    sp->setAction(sp->nextAction);
    
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
            int workval = sp->getPossessions()->PerformTask(dt, GameScene::getThis()->configSettings->secondToDayRatio, GameScene::getThis()->configSettings->default_work_rate);
            
            // workval > 0 means the construction is successful, so update the construction scale and the energy scale
            if(workval > 0)
            {
                this->build_uint_current += workval;
                
                sp->updateIdleDelay(0.2f);
            }
            else
            // workval = 0, means the the villager is out of energy
            {
                for (int i = 0; i< memberSpriteList->count(); i++)
                {
                    GameSprite* gameS = (GameSprite*) memberSpriteList->objectAtIndex(i);
                    
                    if(gameS->getPossessions()->energyRating <= 0)
                    {
                        gameS->setTargetLocation(gameS->getHome());
                        gameS->GoRest(gameS->getHome());
                    }
                }
            }
        }
        else
        // the building has finished the construction process, the villagers will leave the building and become idle
        {
            this->isCurrentConstructing = false;
            for(int i = 0; i < memberSpriteList->count(); i++)
            {
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
                    //this->currentStorage -= food_consumption_rate;
                    sp->getPossessions()->currentHungry += gainVal;
                    if(sp->getPossessions()->currentHungry > sp->getPossessions()->default_hungry_limit + sp->getPossessions()->bonus_hungry_limit)
                    {
                        sp->getPossessions()->currentHungry = sp->getPossessions()->default_hungry_limit + sp->getPossessions()->bonus_hungry_limit;
                    }
                    sp->updateIdleDelay(0.1f);
                }
            }
            else
                // the sprite has finished eating the food, the villagers will leave the building and become idle
            {
                //sp->getPossessions()->energyRating = sp->getPossessions()->targetHungry;
                this->currentStorage--;
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
    else if(sp->currAction == FARMING && sp->getTargetLocation() == this && sp->isFarmer())
    // farming
    {
        // if the farming process not finished!
        if(!isCurrentHarvest && farmState == FARM && currentStorage <= 0)
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
                    isCurrentHarvest = true;
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
    else if(sp->currAction == ROB && sp->getTargetLocation() == this)
    {
        // if the building to be rob is a town hall, rob the money;
        if(build_uint_required >= 10000)
        {
            if(sp->current_money_rob < GameScene::getThis()->settingsLevel->max_money_rob && GameHUD::getThis()->money > 0)
            {
                GameHUD::getThis()->money--;
                sp->current_money_rob++;
                
                if(GameHUD::getThis()->money < 0)
                {
                    GameHUD::getThis()->money = 0;
                    sp->current_money_rob--;
                    sp->currAction = IDLE;
                    sp->nextAction = IDLE;
                    sp->setTargetLocation(NULL);
                }
            }
            else
            {
                sp->currAction = IDLE;
                sp->nextAction = IDLE;
                sp->setTargetLocation(NULL);
            }
        }
        else
        {
            // if the building to rob is a granary, rob the food.
            if(currentStorage > 0)
            {
                this->currentStorage--;
                sp->current_food_rob++;
            }
            else
            {
                sp->currAction = IDLE;
                sp->nextAction = IDLE;
                sp->setTargetLocation(NULL);
            }
        }
    }
    else if (buildingType == HOUSING)
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
                    sp->getPossessions()->energyRating += sp->getPossessions()->Rest();
                    if(sp->getPossessions()->energyRating >= sp->getPossessions()->default_energy_limit)
                    {
                        sp->getPossessions()->energyRating = sp->getPossessions()->default_energy_limit;
                    }
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
                    int workval = sp->getPossessions()->PerformTask(dt, GameScene::getThis()->configSettings->secondToDayRatio, GameScene::getThis()->configSettings->default_work_rate);
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
        sp->currAction = IDLE;
        sp->nextAction = IDLE;
        sp->setTargetLocation(NULL);
        sp->setIsFollowingMovementInstruction(false);
        sp->futureAction1 = IDLE;
        sp->futureAction2 = IDLE;
    }
}

void Building::Leavehandler(GameSprite *sp)
{
    CCLog("Leave handler called");
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
        
            //increase stats here **
            Building* buildingData = GameScene::getThis()->buildingHandler->getBuildingWithGID(baseGID);
        
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

bool Building::isUnderConstruction()
{
    return build_uint_current < build_uint_required;
}

bool Building::hasFood()
{
    return currentStorage > 0;
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

/*Building Animation Is Here! */
//changes the building's appearance only. The building's current stats must not be changed!
//ideally, other appearances should be implemented as ignore_this on the TMX file, right next to the actual building record.
//(TODO)to completely change the building's anim sequence, throw the should_completely_change_anim flag, supplying the new Building through the first parameter.
//If the animation sequence is changed the pointer supplies the new default and max frames BUUUUUT nothing else changes.(/TODO)
void Building::ChangeAppearance(Building *b, bool should_completely_change_anim)
{
    if (b==NULL) return;
    CCSprite* newRep = CCSprite::create();
    newRep->initWithTexture(b->buildingTexture, b->buildingRect);
    CCPoint tilePos = buildingRep->getPosition();
    float z = buildingRep->getZOrder();
    newRep->setPosition(tilePos);
    
    //NEED TO REMOVE THE OLD SPRITE BEFORE CREATING THE NEW ONE.
    
    
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(buildingRep);
    //Because ::Create was used the old one should garbage collect on its own. TO MONITOR.
    buildingRep = newRep;
    
    
    GameScene::getThis()->mapHandler->getMap()->addChild(buildingRep, z);
    
}

void Building::BeginAnim()
{
    if (animframe_count > 1)
    {
    
        currGID = baseGID;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Building::AnimUpdate), this, 1.0f, false);
        
        
    }
    
}


void Building::EndAnim()
{
   if (animframe_count > 1) //there will only be an animation IF there i more than one frame.
       CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Building::AnimUpdate), this);
    
}

//Anim is currently automatic with no control. Todo: anim based on progress etc.

void Building::AnimUpdate()
{
    if (buildingRep == NULL) return;
    if (!GameScene::getThis()) return;
    if (currGID < maxGID)
        ++currGID;
    else
        currGID = baseGID;
    ChangeAppearance(GameScene::getThis()->buildingHandler->getBuildingWithGID(currGID)); //do NOT call true here, otherwise the number of frames will update. We don't want that. - Larry
    
}

void Building::updateBuilding(float dt)
{
    if(isCurrentUpgrading)
    {
        cumulatedTimeUpgrading += dt;
        
        if(cumulatedTimeUpgrading > 0.12f)
        {
            current_upgrade_unit++;
            if(current_upgrade_unit > upgrade_unit_max)
            {
                current_upgrade_unit = 0;
                GameManager::getThis()->town_hall_level++;
                isCurrentUpgrading = false;
            }
        }
    }
    
    if(inProgress != (memberSpriteList->count() > 0))
    {
        inProgress = (memberSpriteList->count() > 0);
    }
}
