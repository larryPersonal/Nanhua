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
#include "BuildingInfoMenu.h"
#include "GlobalHelper.h"
#include <iterator>
#include "TutorialManager.h"

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
    anim_curr_chance = 0.0f;
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
    isCurrentWorking = false;
    
    required_building_count = required_capita = required_population = 0;
    
    cumulatedTimeUpgrading = 0;
    
    anim_random = false;
    anim_random_chance = 1.0f; //1.0 is 100%, 0 is 0%;
    
    anim_check_time = 1.0f;
    curr_anim_check_time = 1.0f;
    
    anim_triggered = true;
    cumulatedTimeResting = 0;
    
    taskType = NoActivity;
    
    progressBar = NULL;
    
    targetWorkUnit = 0;
    
    labelArray = CCArray::create();
    labelArray->retain();
    
    /* fish animation */
    fishAnimationFrameWidth = 64;
    fishAnimationFrameHeight = 64;
    
    fishAnimationArea = CCRect(220, -120, 480, 230);
    fishAnimationCheckTime = 0;
    fishAnimationCheckTimeMax = 2; //2
    fishAnimationCheckTimeMin = 1; //1
    fishAnimationCumulativeTime = 0;
    
    fishAnimationRate = 50; // 50
    
    isInAnimation = false;
    
    delay_animFrame = 0.1;
    delay_curr = 0.1;
    
    x_frameNo = 0;
    x_maxFrameNo = 8;
    
    xOffset = 0;
    yOffset = 0;
    
    fishAnim = NULL;
    
    buildingRep = NULL;
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
    
    labelArray->removeAllObjects();
    CC_SAFE_RELEASE(labelArray);
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
        
        pCopy->taskType = this->taskType;
      
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
        pCopy->lastGID = pCopy->lastGID;
        pCopy->animframe_count = this->animframe_count;
        pCopy->anim_random = this->anim_random;
        pCopy->anim_random_chance = this->anim_random_chance;
        pCopy->currentExp = this->currentExp;
        pCopy->expToLevel->initWithArray(this->expToLevel);
        
        pCopy->fishAnim = this->fishAnim;
        pCopy->fishAnimationArea = this->fishAnimationArea;
        pCopy->fishAnimationCheckTime = this->fishAnimationCheckTime;
        pCopy->fishAnimationCheckTimeMax = this->fishAnimationCheckTimeMax;
        pCopy->fishAnimationCheckTimeMin = this->fishAnimationCheckTimeMin;
        pCopy->fishAnimationCumulativeTime = this->fishAnimationCumulativeTime;
        pCopy->fishAnimationRate = this->fishAnimationRate;
        pCopy->isInAnimation = this->isInAnimation;
        pCopy->fishAnimationFrameWidth = this->fishAnimationFrameWidth;
        pCopy->fishAnimationFrameHeight = this->fishAnimationFrameHeight;
        pCopy->delay_animFrame = this->delay_animFrame;
        pCopy->delay_curr = this->delay_curr;
        pCopy->x_frameNo = this->x_frameNo;
        pCopy->x_maxFrameNo = this->x_maxFrameNo;
        pCopy->xOffset = this->xOffset;
        pCopy->yOffset = this->yOffset;
        pCopy->fishAnimationTexture = this->fishAnimationTexture;
        pCopy->fishAnimationRect = this->fishAnimationRect;
        pCopy->fishAnim = this->fishAnim;
        
        pNewZone = new CCZone(pCopy);
    }
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCPoint Building::getWorldPosition()
{
    if (!buildingRep)
    {
        return CCPointMake(-1, -1);
    }
    else
    {
        return ccpAdd(buildingRep->getPosition(), GameScene::getThis()->mapHandler->getMap()->getPosition());
    }
    
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
    
    if(sp->getTargetLocation() == this)
    {
        sp->hasAssigned = false;
    }
    
    if(sp->getJobLocation() == this)
    {
        sp->setIsDoingJob(true);
    }
    
    if(sp->currAction == EATING && sp->getTargetLocation() == this)
    {
        if(this->buildingType == GRANARY)
        {
            sp->targetEnergy = (sp->getPossessions()->energyRating + 20 + rand() % 20);
            sp->targetEnergy = sp->targetEnergy > 100 ? 100 : sp->targetEnergy;
            sp->targetHungry = (sp->getPossessions()->currentHungry + 60 + rand() % 20);
            sp->targetHungry = sp->targetHungry > 100 ? 100 : sp->targetHungry;
            sp->targetHappiness = (sp->getPossessions()->happinessRating + 5 + rand() % 5);
            sp->targetHappiness = sp->targetHappiness > 100 ? 100 : sp->targetHappiness;
        }
        else if(this->buildingType == MARKET)
        {
            sp->targetEnergy = (sp->getPossessions()->energyRating + 30 + rand() % 30);
            sp->targetEnergy = sp->targetEnergy > 100 ? 100 : sp->targetEnergy;
            sp->targetHungry = (sp->getPossessions()->currentHungry + 80 + rand() % 20);
            sp->targetHungry = sp->targetHungry > 100 ? 100 : sp->targetHungry;
            sp->targetHappiness = (sp->getPossessions()->happinessRating + 10 + rand() % 10);
            sp->targetHappiness = sp->targetHappiness > 100 ? 100 : sp->targetHappiness;
        }
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
                if(TutorialManager::getThis()->active)
                {
                    this->build_uint_current += workval * 10;
                }
                else
                {
                    this->build_uint_current += workval;
                }
                
                if(this->build_uint_current > this->build_uint_required)
                {
                    this->build_uint_current = this->build_uint_required;
                }
                
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
            if (sp->getPossessions()->currentHungry < sp->getPossessions()->targetHungry || sp->getPossessions()->energyRating < sp->targetEnergy || sp->getPossessions()->happinessRating < sp->targetHappiness)
            {
                if(sp->getPossessions()->currentHungry < sp->getPossessions()->targetHungry)
                {
                    sp->getPossessions()->currentHungry++;
                    sp->getPossessions()->currentHungry = sp->getPossessions()->currentHungry > 100 ? 100 : sp->getPossessions()->currentHungry;
                }
                
                if(sp->getPossessions()->energyRating < sp->targetEnergy)
                {
                    sp->getPossessions()->energyRating++;
                    sp->getPossessions()->energyRating = sp->getPossessions()->energyRating > 100 ? 100 : sp->getPossessions()->energyRating;
                }
                
                if(sp->getPossessions()->happinessRating < sp->targetHappiness)
                {
                    sp->getPossessions()->happinessRating++;
                    sp->getPossessions()->happinessRating = sp->getPossessions()->happinessRating > 100 ? 100 : sp->getPossessions()->happinessRating;
                }
                
                sp->updateIdleDelay(0.1f);
            }
            else
            // the sprite has finished eating the food, the villagers will leave the building and become idle
            {
                this->currentStorage -= 20;
                if(this->currentStorage < 0)
                {
                    this->currentStorage = 0;
                }
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
            // if the sprite still has energy to do farming task
            if(sp->getPossessions()->energyRating >= 0)
            {
                sp->setCumulativeTime(sp->getCumulativeTime() + dt);
                
                if(work_unit_current < targetWorkUnit)
                {
                    work_unit_current += 1.0f;
                    
                    if(work_unit_current > targetWorkUnit)
                    {
                        work_unit_current = targetWorkUnit;
                    }
                }
                
                if(sp->getCumulativeTime() >= 0.33333333f)
                {
                    sp->getPossessions()->energyRating -= 0.1f;
                    sp->setCumulativeTime(0.0f);
                    if(TutorialManager::getThis()->active)
                    {
                        targetWorkUnit += sp->getPossessions()->default_work_unit_per_day / 12.0f * 10;
                    }
                    else
                    {
                        targetWorkUnit += sp->getPossessions()->default_work_unit_per_day / 12.0f;
                    }
                }
                
                if(work_unit_current >= work_unit_required)
                {
                    work_unit_current = 0;
                    currentStorage = storageLimit;
                    isCurrentHarvest = true;
                    taskType = NoActivity;
                    targetWorkUnit = 0;
                    removeProgressiveBar();
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
                sp->saySpeech(TRANSPORT_EMOTION, 20000);
                sp->movementSpeed = sp->getPossessions()->default_movement_speed * 0.5f;
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
                            gameS->movementSpeed = gameS->getPossessions()->default_movement_speed;
                        }
                    }
                    memberSpriteList->removeAllObjects();
                    
                    this->isCurrentWorking = false;
                    this->isCurrentHarvest = false;
                }
                else
                {
                    sp->setFoodCarried(sp->getPossessions()->default_food_carriage_limit);
                    currentStorage -= sp->getFoodCarried();
                }
                
                Building* gran = sp->findNearestGranary(true);
                if(gran != NULL)
                // if has valid granary, deliver food to there
                {
                    sp->setTargetLocation(gran);
                    sp->GoGranary(gran);
                }
                else
                // if no valid granary available, discard all food and unjob
                {
                    sp->setFoodCarried(0);
                    isCurrentWorking = false;
                    sp->currAction = IDLE;
                    sp->nextAction = IDLE;
                    sp->setIsDoingJob(false);
                    sp->spriteClass = "citizen";
                    sp->setJob(NONE);
                    sp->setJobLocation(NULL);
                    sp->setTargetLocation(NULL);
                    sp->changeToCitizen();
                    sp->movementSpeed = sp->getPossessions()->default_movement_speed;
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
            Building* gran = sp->findNearestGranary(true);
            if(gran != NULL)
            // if still got other granary with storage capacity, go there
            {
                sp->setTargetLocation(gran);
                sp->GoGranary(gran);
            }
            else
            // if no valid granary is available, discard all the food and unjob.
            {
                sp->setFoodCarried(0);
                isCurrentWorking = false;
                sp->currAction = IDLE;
                sp->nextAction = IDLE;
                sp->setIsDoingJob(false);
                sp->spriteClass = "citizen";
                sp->getJobLocation()->memberSpriteList->removeObject(sp);
                sp->setJob(NONE);
                sp->setJobLocation(NULL);
                sp->setTargetLocation(NULL);
                sp->changeToCitizen();
                sp->movementSpeed = sp->getPossessions()->default_movement_speed;
                
                if(TutorialManager::getThis()->active)
                {
                    if(TutorialManager::getThis()->teachFarming && TutorialManager::getThis()->miniDragon != NULL)
                    {
                        TutorialManager::getThis()->miniDragon->clickNext();
                    }
                }
            }
        }
        else
        // all food has been stored into the granary, if still has job, means the delivering process is not finished, go back to the farm, otherwise, the delivering process has been finished, unjob.
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
                isCurrentWorking = false;
                sp->currAction = IDLE;
                sp->nextAction = IDLE;
                sp->setIsDoingJob(false);
                sp->spriteClass = "citizen";
                sp->getJobLocation()->memberSpriteList->removeObject(sp);
                sp->setJob(NONE);
                sp->setJobLocation(NULL);
                sp->setTargetLocation(NULL);
                sp->changeToCitizen();
                sp->movementSpeed = sp->getPossessions()->default_movement_speed;
                
                if(TutorialManager::getThis()->active)
                {
                    if(TutorialManager::getThis()->teachFarming && TutorialManager::getThis()->miniDragon != NULL)
                    {
                        TutorialManager::getThis()->miniDragon->clickNext();
                    }
                }
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
                    cumulatedTimeResting = 0;
                    leaveHouse(sp);
                }
                else
                    // if the sprite is not fully recharged, it will recharge energy.
                {
                    cumulatedTimeResting += dt * 3;
                    
                    if(cumulatedTimeResting >= (1.0f / GameScene::getThis()->configSettings->default_energy_recovery))
                    {
                        sp->getPossessions()->energyRating += sp->getPossessions()->Rest();
                        if(sp->getPossessions()->energyRating >= sp->getPossessions()->default_energy_limit)
                        {
                            sp->getPossessions()->energyRating = sp->getPossessions()->default_energy_limit;
                        }
                        cumulatedTimeResting = 0;
                    }
                }
            }
            else
                // if it is not the home house, leave the house
            {
                // TODO::because the house is not sprite's home, so the spirte still does not have been recharged, to have enough energy to do other tasks, it needs to be recharged first, so try to find his home in other place, if no home, just wander around....
                cumulatedTimeResting = 0;
                leaveHouse(sp);
            }
        }
    }
    else if (buildingType == MILITARY && sp->currAction == GUARD)
    {
        if(sp->villagerClass == V_CITIZEN)
        {
            sp->changeSpriteTo(GlobalHelper::getSpriteByVillagerClass(sp, V_SOLDIER), GlobalHelper::getSpriteClassByVillagerClass(V_SOLDIER));
            
            if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachSoldier)
            {
                if(TutorialManager::getThis()->miniDragon != NULL)
                {
                    TutorialManager::getThis()->miniDragon->clickNext();
                }
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
    
    if (should_completely_change_anim) //if the anim changes, change the whole lot. GIDs are different per anim.
    {
        animframe_count = b->animframe_count;
        anim_triggered = false;
        baseGID = b->baseGID;
        currGID = b->baseGID;
        maxGID = b->maxGID;
    }
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(buildingRep);
    
    //Because ::Create was used the old one should garbage collect on its own. TO MONITOR.
    buildingRep = newRep;
    
    if(!isUnderConstruction())
    {
        if(taskType == FarmActivity)
        {
            addProgressiveBar();
        }
    }
    
    GameScene::getThis()->mapHandler->getMap()->addChild(buildingRep, z);
    
}

void Building::BeginAnim()
{
    if (animframe_count > 1 && buildingType != AMENITY)
    {
    
        currGID = baseGID;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Building::AnimUpdate), this, 0.1f, false);
        
        
    }
    
}


void Building::EndAnim()
{
   if (animframe_count > 1 && buildingType != AMENITY) //there will only be an animation IF there i more than one frame.
       CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Building::AnimUpdate), this);
    
}

//Anim is currently automatic with no control. Todo: anim based on progress etc.

void Building::AnimUpdate()
{
    if (buildingRep == NULL) return;
    if (!GameScene::getThis()) return;
    
    
    if (anim_random && !anim_triggered)
    {
        if (curr_anim_check_time > 0)
            curr_anim_check_time -= 0.1f;
        else
        {
            anim_curr_chance = float(rand()) / RAND_MAX;
            if (anim_curr_chance >= anim_random_chance)
                anim_triggered = true;
            else
            {
                curr_anim_check_time = anim_check_time;
                anim_triggered = false;
            }
        }
    }
    
    if ((anim_random && anim_triggered) || !anim_random)
    {
        if (currGID < maxGID)
            ++currGID;
        else
        {
            currGID = baseGID;
            curr_anim_check_time = anim_check_time;
            anim_triggered = false;
        }
        ChangeAppearance(GameScene::getThis()->buildingHandler->getBuildingWithGID(currGID)); //do NOT call true here, otherwise the number of frames will update. We don't want that. - Larry
    }
    
}

void Building::updateBuilding(float dt)
{
    if(buildingType == AMENITY)
    {
        //  int number_of_phase = maxGID - baseGID + 1;
        if(work_unit_current == 0)
        {
            currGID = baseGID;
        }
        else if(work_unit_current >= work_unit_required)
        {
            currGID = baseGID + 4;
        }
        else
        {
            currGID = ((int)((int)(work_unit_current * 4)) / work_unit_required) + baseGID + 1;
        }
        
        if(lastGID != currGID)
        {
            lastGID = currGID;
            ChangeAppearance(GameScene::getThis()->buildingHandler->getBuildingWithGID(currGID));
        }
    }
    
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
                taskType = NoActivity;
                removeProgressiveBar();
            }
        }
    }
    
    /*
     * update the side information for all the activities of the building
     */
    if(isUnderConstruction())
    {
        if(taskType == Construction)
        {
            if(progressBar == NULL)
            {
                addProgressiveBar();
            }
            else
            {
                progressBar->setValue((float)build_uint_current / (float)build_uint_required);
            }
        }
    }
    else
    {
        if(taskType == FarmActivity)
        {
            if(progressBar == NULL)
            {
                addProgressiveBar();
            }
            else
            {
                progressBar->setValue((float)work_unit_current / (float)work_unit_required);
            }
        }
        else if(taskType == UpgradeActivity)
        {
            if(progressBar == NULL)
            {
                addProgressiveBar();
            }
            else
            {
                progressBar->setValue((float)current_upgrade_unit / (float) upgrade_unit_max);
            }
        }
    }
    
    if(inProgress != (memberSpriteList->count() > 0))
    {
        inProgress = (memberSpriteList->count() > 0);
    }
    
    /* update for building labels */
    for (int i = 0; i < labelArray->count(); i++)
    {
        CCLabelTTF* label = (CCLabelTTF*) labelArray->objectAtIndex(i);
        label->setPosition(ccp(label->getPositionX(), label->getPositionY() + 1.0f));
        float alpha = (float)label->getOpacity();
        alpha -= 4.0f;
        if(alpha < 0)
        {
            labelArray->removeObject(label);
            buildingRep->removeChild(label, true);
        }
        else
        {
            label->setOpacity((GLubyte) alpha);
        }
    }
    
    /* randomly trigger the fish animation */
    if (buildingType == RIVER)
    {
        if (!isInAnimation)
        {
            if(fishAnimationCheckTime <= 0)
            {
                fishAnimationCheckTime = fishAnimationCheckTimeMin + rand() % ((int) (fishAnimationCheckTimeMax - fishAnimationCheckTimeMin) + 1);
                fishAnimationCumulativeTime = 0;
            }
            else
            {
                fishAnimationCumulativeTime += dt;
                
                if(fishAnimationCumulativeTime >= fishAnimationCheckTime)
                {
                    int randNumber = rand() % 100 + 1;
                    if(randNumber <= fishAnimationRate)
                    {
                        isInAnimation = true;
                        x_frameNo = -1;
                    }
                    fishAnimationCheckTime = 0;
                    fishAnimationCumulativeTime = 0;
                }
            }
        }
        else
        {
            if(delay_curr > 0)
            {
                delay_curr -= dt;
            }
            else
            {
                delay_curr = delay_animFrame;
                
                x_frameNo++;
                
                if(x_frameNo >= x_maxFrameNo)
                {
                    x_frameNo = 0;
                    isInAnimation = false;
                    GameScene::getThis()->mapHandler->getMap()->removeChild(fishAnim);
                    GameScene::getThis()->spriteHandler->fishAnimOnMap->removeObject(fishAnim);
                    fishAnim = NULL;
                }
                else
                {
                    xOffset = x_frameNo % 2;
                    yOffset = x_frameNo / 2;
                    
                    if(x_frameNo == 0)
                    {
                        fishAnimationTexture = CCTextureCache::sharedTextureCache()->addImage("fish.png");
                        
                        fishAnimationRect = CCRectMake(xOffset * fishAnimationFrameWidth, yOffset * fishAnimationFrameHeight, fishAnimationFrameWidth, fishAnimationFrameHeight);
                        fishAnim = CCSprite::createWithTexture(fishAnimationTexture, fishAnimationRect);
                        fishAnim->setAnchorPoint(ccp(0.5, 0.5));
                        fishAnim->setScale( 1.0f );
                        GameScene::getThis()->mapHandler->getMap()->addChild(fishAnim, buildingRep->getZOrder() + 1);
                        GameScene::getThis()->spriteHandler->fishAnimOnMap->addObject(fishAnim);
                        
                        int xPos = ((int) fishAnimationArea.getMinX()) + rand() % ((int) (fishAnimationArea.getMaxX() - fishAnimationArea.getMinX()));
                        int yPos = ((int) fishAnimationArea.getMinY()) + rand() % ((int) (fishAnimationArea.getMaxY() - fishAnimationArea.getMinY()));
                        
                        fishAnim->setPosition(ccp((float) xPos + buildingRep->getPositionX(), (float) yPos + buildingRep->getPositionY()));
                    }
                    else
                    {
                        fishAnimationRect.setRect(xOffset * fishAnimationFrameWidth, yOffset * fishAnimationFrameHeight, fishAnimationFrameWidth, fishAnimationFrameHeight);
                        fishAnim->setTextureRect(fishAnimationRect);
                    }
                }
            }
        }
    }
}

void Building::addNotificationLabel(std::string notificationStr, ccColor3B color)
{
    CCLabelTTF* label = CCLabelTTF::create(notificationStr.c_str(), "GillSansMT", 24);
    label->setPosition(ccp(buildingRep->boundingBox().size.width / 2.0f, buildingRep->boundingBox().size.height / 2.0f));
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setColor(color);
    buildingRep->addChild(label);
    labelArray->addObject(label);
}

void Building::addProgressiveBar()
{
    progressBar = ProgressBar::create();
    progressBar->createProgressBar(CCRectMake(0, 0, buildingRep->boundingBox().size.width + 6, 16),
                                   CCRectMake(3, 3, buildingRep->boundingBox().size.width, 10),
                                   "Energy_brown bar.png",
                                   "NONE",
                                   "NONE",
                                   "Energybarblue.png",
                                   true);
    progressBar->setAnchorPoint(ccp(0, 1));
    buildingRep->addChild(progressBar);
    progressBar->setPosition(0, buildingRep->boundingBox().size.height);
    
    if(taskType == Construction)
    {
        CCSprite* taskIcon = CCSprite::create("main-game-buttons_build.png");
        taskIcon->setAnchorPoint(ccp(0, 1));
        taskIcon->setScale(progressBar->boundingBox().size.height / taskIcon->boundingBox().size.height * 3.0f);
        progressBar->addChild(taskIcon);
        taskIcon->setPosition(ccp(0, progressBar->boundingBox().size.height + taskIcon->boundingBox().size.height * 0.6f));
    }
    else if(taskType == FarmActivity)
    {
        CCSprite* taskIcon = CCSprite::createWithSpriteFrameName("rice_icon.png");
        taskIcon->setAnchorPoint(ccp(0, 1));
        taskIcon->setScale(progressBar->boundingBox().size.height / taskIcon->boundingBox().size.height * 1.5f);
        progressBar->addChild(taskIcon);
        taskIcon->setPosition(ccp(0, progressBar->boundingBox().size.height + taskIcon->boundingBox().size.height * 1.1f));
    }
    else if(taskType == UpgradeActivity)
    {
        CCSprite* taskIcon = CCSprite::create("upgrade.png");
        taskIcon->setAnchorPoint(ccp(0, 1));
        taskIcon->setScale(progressBar->boundingBox().size.height / taskIcon->boundingBox().size.height * 2.0f);
        progressBar->addChild(taskIcon);
        taskIcon->setPosition(ccp(0, progressBar->boundingBox().size.height + taskIcon->boundingBox().size.height * 1.0f));
    }
}

void Building::removeProgressiveBar()
{
    if(progressBar != NULL)
    {
        buildingRep->removeChild(progressBar, true);
        progressBar = NULL;
    }
}
