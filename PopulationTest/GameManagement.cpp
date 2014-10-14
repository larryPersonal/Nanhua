//
//  GameManagement.cpp
//  PopulationTest
//
//  Created by admin on 28/5/14.
//
//

#include "GameManagement.h"
#include "GameScene.h"
#include "GameManager.h"
#include "GameHUD.h"
#include "ReputationOrb.h"
#include "UserProfile.h"
#include "TutorialManager.h"

void GameManagement::saveGameToFile(int type)
{
    CCLog("try saving......");
    saveData(type);
    CCLog("saving success!");
}

void GameManagement::saveBuildingData(int type)
{
    // CCLog("trying to save building data");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    CCArray* allBuildingsOnMap = BuildingHandler::getThis()->allBuildingsOnMap;
    
    // total number of buildings
    
    int buildingIndex = -1;
    
    for (int i = 0; i < allBuildingsOnMap->count(); i++)
    {
        Building* bui = (Building*) allBuildingsOnMap->objectAtIndex(i);
        
        if(bui->buildingType != HOUSING && bui->buildingType != AMENITY && bui->buildingType != GRANARY && bui->buildingType != MARKET && bui->buildingType != MILITARY && bui->buildingType != SPECIAL && bui->buildingType != EDUCATION && bui->buildingType != SOCIAL)
        {
            continue;
        }
        
        buildingIndex++;
        
        CCArray* allbuildings = BuildingHandler::getThis()->allBuildings;
        
        int tag = 0;
        
        for (int j = 0; j < allbuildings->count(); j++)
        {
            Building* tempBui = (Building*) allbuildings->objectAtIndex(j);
            if (bui->buildingType == tempBui->buildingType)
            {
                tag = tempBui->ID;
                break;
            }
        }
        
        // CCLog("****** the current tag is %d", tag);
        
        // building unique id
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_uniqueID";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->uniqueID);
        
        // building id
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_id";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), tag);
        
        // CCLog("****** the id of the building is %d", tag);
        
        // building in progress
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_inProgress";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->inProgress);
        
        // building type
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_buildingType";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) bui->buildingType);
        
        // building farm state
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_farmState";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) bui->farmState);
        
        // building population limit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_populationLimit";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->populationLimit);
        
        // building builder limit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_builderLimit";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->builderLimit);
        
        // building task type
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_taskType";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) bui->taskType);
        
        // building target work unit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_tagetWorkUnit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->targetWorkUnit);
        
        // building current storage
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currentStorage";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->currentStorage);
        
        // building storage limit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_storageLimit";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->storageLimit);
        
        // building build unit required
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_buildUnitRequired";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->build_uint_required);
        
        // building build unit current
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_buildUnitCurrent";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->build_uint_current);
        
        // building work unit required
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_workUnitRequired";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->work_unit_required);
        
        // building work unit current
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_workUnitCurrent";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->work_unit_current);
        
        // building recovery rate
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_recoveryRate";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->recovery_rate);
        
        // building food consumption rate
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_foodConsumptionRate";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->food_consumption_rate);
        
        // building baseGID
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_baseGID";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->baseGID);
        
        // building maxGID
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_maxGID";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->maxGID);
        
        // building currGID
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currGID";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->currGID);
        
        // building lastGID
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_lastGID";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->lastGID);
        
        // building anim frame count
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_animFrameCount";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->animframe_count);
        
        // building anim random
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_animRandom";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->anim_random);
        
        // building anim random chance
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_animRandomChance";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->anim_random_chance);
        
        // building anim curr chance
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_animCurrChance";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->anim_curr_chance);
        
        // building anim triggered
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_animTriggered";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->anim_triggered);
        
        // building width
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_width";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->width);
        
        // building height
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_height";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->height);
        
        // building current level
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currentLevel";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->currentLevel);
        
        // building cost
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_cost";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->buildingCost);
        
        // building memeber sprite lists
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_memberNumber";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->memberSpriteList->count());
        
        CCArray* allSpritesOnMap = SpriteHandler::getThis()->spritesOnMap;
        
        for (int j = 0; j < bui->memberSpriteList->count(); j++)
        {
            GameSprite* gs = (GameSprite*) bui->memberSpriteList->objectAtIndex(j);
            
            int gsID = -1;
            
            for (int k = 0; k < allSpritesOnMap->count(); k++)
            {
                GameSprite* temp = (GameSprite*) allSpritesOnMap->objectAtIndex(k);
                
                if(temp == gs)
                {
                    gsID = temp->uniqueID;
                    break;
                }
            }
            
            ss.str(std::string());
            ss << username << "_building_" << buildingIndex << "_member_" << j;
            CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gsID);
        }
        
        // building upgrade unit max
        ss.str(std::string());
        ss << username << "_buidling_" << buildingIndex << "_upgradeUnitMax";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->upgrade_unit_max);
        
        // building current upgrade unit
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_currentUpgradeUnit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->current_upgrade_unit);
        
        // building is current harvest
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentHarvest";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentHarvest);
        
        // buildnig is current constructing
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentConstructing";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentConstructing);
        
        // building is current working
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentWorking";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentWorking);
        
        // building is current upgrading
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_isCurrentUpgrading";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), bui->isCurrentUpgrading);
        
        // building number of jobs
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_numberOfJobs";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), bui->number_of_jobs);
        
        // building cumulativeTimeUpgrading
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_cumulativeTimeUpgrading";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->cumulatedTimeUpgrading);
        
        // building cumulativeTimeResting
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_cumulativeTimeResting";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), bui->cumulatedTimeResting);
        
        CCPoint tPos = bui->getWorldPosition();
        tPos = MapHandler::getThis()->tilePosFromLocation(tPos);
        
        // building position x
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_positionX";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tPos.x);
        
        // building position y
        ss.str(std::string());
        ss << username << "_building_" << buildingIndex << "_positionY";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tPos.y);
    }
    
    ss.str(std::string());
    ss << username << "_numberOfBuildings";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), buildingIndex + 1);
}

void GameManagement::saveSystemData(int type)
{
    // CCLog("### trying to save system data......");
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    // CCLog("### Username of the account is : %s", username.c_str());
    
    // save the load game status
    ss.str(std::string());
    ss << username << "_hasSavedGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    // CCLog("### has saved game : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
    
    // save the level number
    ss.str(std::string());
    ss << username << "_levelNumber";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), UserProfile::getThis()->gameLevel);
    
    // CCLog("### level number : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    // save the town hall level
    ss.str(std::string());
    ss << username << "_townhallLevel";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameManager::getThis()->town_hall_level);
    
    // save the current gold
    ss.str(std::string());
    ss << username << "_currentGold";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->money);
    
    // CCLog("### current gold : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    // save the target gold
    ss.str(std::string());
    ss << username << "_targetGold";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->targetMoney);
    
    // CCLog("### target gold : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    // save the reputation
    ss.str(std::string());
    ss << username << "_currentReputation";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->reputation);
    
    // CCLog("### current reputation : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    // save the target reputation
    ss.str(std::string());
    ss << username << "_targetReputation";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->targetReputation);
    
    // CCLog("### target reputation : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    // save the date
    ss.str(std::string());
    ss << username << "_date";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), GameHUD::getThis()->getNumberOfDays());
    
    // CCLog("### current date : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    // ignore objective first, deal it later!!!
    
    // ignore all the reputation orbs first!!!
}

void GameManagement::saveSpritesData(int type)
{
    // CCLog("trying to save sprite data");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
    
    ss.str(std::string());
    ss << username << "_numberOfGameSprites";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), spritesOnMap->count());
    
    // CCLog("### number of sprites : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        // CCLog ("#####################################################");
        // CCLog ("### sprite : %d", i);
        
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        
        // sprite unique id
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_uniqueID";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->uniqueID);
        
        // sprite displayed name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteDisplayedName";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->spriteDisplayedName);
        
        // CCLog ("### sprite displayed name : %s", CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str()).c_str());
        
        // sprite name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteName";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->spriteName);
        
        // CCLog ("### sprite name : %s", CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str()).c_str());
        
        // spriteRep visibility
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteRepVisible";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->spriteRep->isVisible());
        
        // sprite positions
        CCPoint tilePos = gs->getWorldPosition();
        tilePos = MapHandler::getThis()->tilePosFromLocation(tilePos);
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tilePos.x);
        
        // CCLog ("### sprite position x is : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tilePos.y);
        
        // CCLog ("### sprite position y is : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite foodCarried
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_foodCarried";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->foodCarried);
        
        // CCLog ("### sprite food carried : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite current direction
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentDirection";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->currentDir);
        
        // CCLog ("### sprite current direction : %s", CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str()).c_str());
        
        // sprite cumulative time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeTime);
        
        // CCLog ("### sprite cumulative time : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite cumulative time for happiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_happiness";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeTime_happiness);
        
        // CCLog ("### sprite cumulative time happiness : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite cumulative time for attack
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_attack";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeTime_attack);
        
        // CCLog ("### sprite cumulative time attack : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite is token drop cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isTokenDropCooldown";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->is_token_drop_cooldown);
        
        // CCLog ("### sprite is token drop down : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite token drop cooldown time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tokenDropCooldownTime";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->token_drop_cooldown_time);
        
        // CCLog ("### sprite token drop cooldown time : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite go to town hall
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gotoTownHall";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->goToTownHall);
        
        // CCLog ("### sprite go to town hall : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite has assigned
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_hasAssigned";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->hasAssigned);
        
        // CCLog ("### sprite has assigned : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite cumulative check time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeCheckTime";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->cumulativeCheckTime);
        
        // CCLog ("### sprite cumulative check time : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite targetHungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetHungry);
        
        // CCLog ("### sprite target hungry : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite targetEnergy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetEnergy";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetEnergy);
        
        // CCLog ("### sprite target energy : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite targetHappiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHappiness";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetHappiness);
        
        // CCLog ("### sprite target happiness : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite villager class
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_villagerClass";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->villagerClass);
        
        // CCLog ("### sprite villager class : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite enermy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_enermy";
        string key = ss.str();
        
        int enermyUniqueID = -1;
        if (gs->enermy == NULL)
        {
            enermyUniqueID = -1;
        }
        else
        {
            enermyUniqueID = gs->enermy->uniqueID;
        }
        CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), enermyUniqueID);
        
        // CCLog ("### sprite enermy : %s", CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str()).c_str());
        
        // sprite pre-enermy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_preEnermy";
        key = ss.str();
        
        int preEnermyUniqueID = -1;
        if (gs->preEnermy == NULL)
        {
            preEnermyUniqueID = -1;
        }
        else
        {
            preEnermyUniqueID = gs->preEnermy->uniqueID;
        }
        CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), preEnermyUniqueID);
        
        // CCLog ("### sprite pre enermy : %s", CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str()).c_str());
        
        // sprite nextTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->nextTile.x);
        
        // CCLog ("### sprite next tile x : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->nextTile.y);
        
        // CCLog ("### sprite next tile y : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite currentTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->currentTile.x);
        
        // CCLog ("### sprite current tile x : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->currentTile.y);
        
        // CCLog ("### sprite current tile y : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite target location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetLocation.x);
        
        // CCLog ("### sprite target location x : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->targetLocation.y);
        
        // CCLog ("### sprite target location y : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite stop action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_stopAction";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->stopAction);
        
        // CCLog ("### sprite stop action : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite is moving
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isMoving";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isMoving);
        
        // CCLog ("### sprite is moving : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite try escape
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tryEscape";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->tryEscape);
        
        // CCLog ("### sprite try escape : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite mGameCurrentEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameCurrentEndurance";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->mGameCurrentEndurance);
        
        // CCLog ("### sprite game current endurance : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite mGameMaxEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameMaxEndurance";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->mGameMaxEndurance);
        
        // CCLog ("### sprite game max endurance : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite mGameWarMode
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameWarMode";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->mGameWarMode);
        
        // CCLog ("### sprite game war mode : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite current money rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentMoneyRob";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->current_money_rob);
        
        // CCLog ("### sprite current money rob : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite current food rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentFoodRob";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->current_food_rob);
        
        // CCLog ("### sprite current food rob : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite should stop next square
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldStopNextSquare";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->shouldStopNextSquare);
        
        // CCLog ("### sprite should stop next square : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite wander flag
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_wanderFlag";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->wanderFlag);
        
        // CCLog ("### sprite wander flag : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite should set visible next frame
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldSetVisibleNextFrame";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->shouldSetVisibleNextFrame);
        
        // CCLog ("### sprite should set visible next frame : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite job location
        CCArray* allBuildings = BuildingHandler::getThis()->allBuildingsOnMap;
        int index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_jobLocation";
        
        if(gs->getJobLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getJobLocation())
                {
                    index = gs->getJobLocation()->uniqueID;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // CCLog ("### sprite job location : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_job";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->job);
        
        // CCLog ("### sprite job : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite is doing job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isDoingJob";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isDoingJob);
        
        // CCLog ("### sprite is doing job : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite combat state
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_combatState";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->combatState);
        
        // CCLog ("### sprite combat state : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite is leaving next update
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isLeavingNextUpdate";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isLeavingNextUpdate);
        
        // CCLog ("### sprite is leaving next update : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite is in building
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInBuilding";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isInBuilding);
        
        // CCLog ("### sprite is in building : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite gender
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gender";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->gender);
        
        // CCLog ("### sprite gender : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite spriteclass
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteClass";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), gs->spriteClass);
        
        // CCLog ("### sprite sprite class : %s", CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str()).c_str());
        
        // sprite movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_movementSpeed";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->movementSpeed);
        
        // CCLog ("### sprite movement speed : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite last frame action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_lastFrameAction";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->lastFrameAction);
        
        // CCLog ("### sprite last frame action : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite current action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentAction";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->currAction);
        
        // CCLog ("### sprite current action : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite next action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextAction";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->nextAction);
        
        // CCLog ("### sprite next action : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite future action 1
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction1";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->futureAction1);
        
        // CCLog ("### future action 1: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite future action 2
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction2";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->futureAction2);
        
        // CCLog ("### future action 2: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite batch layer index
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_batchLayerIndex";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), (int) gs->batchLayerIndex);
        
        // CCLog ("### batch layer index: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite barHp value
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_barHPValue";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->barHP->getValue());
        
        // CCLog ("### sprite bar hp value: %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite is in combat
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInCombat";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isInCombat);
        
        // CCLog ("### sprite is in combat: %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite is in attack action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInAttackAction";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->isInAttackAction);
        
        // CCLog ("### sprite is in attack action : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite just move one tile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_justMoveOneTile";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), gs->justMoveOneTile);
        
        // CCLog ("### sprite just move one tile : %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()) ? "true" : "false");
        
        // sprite frame Width
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameWidth";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->frameWidth);
        
        // CCLog ("### sprite frame width : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite frame Height
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameHeight";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->frameHeight);
        
        // CCLog("### sprite frame height : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession happiness rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_happinessRating";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->happinessRating);
        
        // CCLog("### sprite happiness rating : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession current hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentHungry";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->currentHungry);
        
        // CCLog("### sprite current hungry : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession target hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->targetHungry);
        
        // CCLog("### sprite target hungry : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession energy rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_energyRating";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->energyRating);
        
        // CCLog("### sprite energy rating : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default work rate
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkRate";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_work_rate);
        
        // CCLog("### sprite default work rate : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default happiness limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHappinessLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_hapiness_limit);
        
        // CCLog("### sprite default happiness limit : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default work unit per day
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkUnitPerDay";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_work_unit_per_day);
        
        // CCLog("### sprite default work unit per day : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultMovementSpeed";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_movement_speed);
        
        // CCLog("### sprite default movement speed : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default animated speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultAnimatedSpeed";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_animate_speed);
        
        // CCLog("### sprite default animated speed : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default hungry limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHungryLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_hungry_limit);
        
        // CCLog("### sprite default hungry limit : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default food carriage limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultFoodCarriageLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_food_carriage_limit);
        
        // CCLog("### sprite default food carriage limit : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession default energy limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultEnergyLimit";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->default_energy_limit);
        
        // CCLog("### sprite default energy limit : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession attack cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackCooldown";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->attack_cooldown);
        
        // CCLog("### sprite attack cooldown : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession attack power max
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMax";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->attack_power_max);
        
        // CCLog("### sprite attack power max : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession attack power min
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMin";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->attack_power_min);
        
        // CCLog("### sprite attack power min : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession current endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentEndurance";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->current_endurance);
        
        // CCLog("### sprite current endurance : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite possession max endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_maxEndurance";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->getPossessions()->max_endurance);
        
        // CCLog("### sprite max endurance : %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str()));
        
        // sprite idle delay
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_idleDelay";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), gs->idleDelay);
        
        // sprite possession home location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_homeLocation";
        
        if(gs->getHome() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getHome())
                {
                    index = gs->getHome()->uniqueID;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // CCLog("### sprite home location : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite possession job location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_possessionJobLocation";
        
        if(gs->getJobLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getPossessions()->jobLocation)
                {
                    index = gs->getPossessions()->jobLocation->uniqueID;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // CCLog("### sprite possession job location : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite possession target location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation";
        
        if(gs->getTargetLocation() == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getTargetLocation())
                {
                    index = gs->getTargetLocation()->uniqueID;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // CCLog("### sprite target location : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // sprite possession target home location
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHomeLocation";
        
        if(gs->getPossessions()->targetHomeLocation == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->getPossessions()->targetHomeLocation)
                {
                    index = gs->getPossessions()->targetHomeLocation->uniqueID;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        // CCLog("### sprite target home location : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // CCLog ("#####################################################");
        
        // sprite last target
        index = 0;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_lastTarget";
        
        if(gs->lastTarget == NULL)
        {
            index = -1;
        }
        else
        {
            for (int j = 0; j < allBuildings->count(); j++)
            {
                Building* bui = (Building*) allBuildings->objectAtIndex(j);
                if (bui == gs->lastTarget)
                {
                    index = gs->lastTarget->uniqueID;
                    break;
                }
            }
        }
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), index);
        
        /*
         * we must restore the path of the game sprites when saving so that we can resuming their path of moving during ruture game loading.
         */
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_numberOfPathTiles";
        
        if(gs->path == NULL)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 0);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), gs->path->count());
        }
        
        for (int j = 0; j < gs->path->count(); j++)
        {
            ss.str(std::string());
            ss << username << "_sprite_" << i << "_path_" << j << "posX";
            
            PathfindingNode* node = (PathfindingNode*) gs->path->objectAtIndex(j);
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), node->tilepos.x);
            
            ss.str(std::string());
            ss << username << "_sprite_" << i << "_path_" << j << "posY";
            
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), node->tilepos.y);
        }
    }
}

void GameManagement::saveRoadData(int type)
{
    // CCLog("trying to save road data");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    CCArray* allPath = MapHandler::getThis()->pathTiles;
    
    ss.str(std::string());
    ss << username << "_numberOfPathTiles";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), allPath->count());
    
    // CCLog("### number of paths : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    for (int i = 0; i < allPath->count(); i++)
    {
        // CCLog ("#####################################################");
        // CCLog ("### path : %d", i);
        
        MapTile* mt = (MapTile*) allPath->objectAtIndex(i);
        
        // path position
        ss.str(std::string());
        ss << username << "_path_" << i << "_posX";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), mt->xpos);
        
        // CCLog ("### path position x : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        ss.str(std::string());
        ss << username << "_path_" << i << "_posY";
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), mt->ypos);
        
        // CCLog ("### path position y : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
        
        // CCLog ("#####################################################");
    }
}

void GameManagement::saveReputationOrbData(int type)
{
    // CCLog("trying to save reputation orb data");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    CCArray* allReputationOrbs = SpriteHandler::getThis()->tokensOnMap;
    
    ss.str(std::string());
    ss << username << "_numberOfReputationOrbs";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), allReputationOrbs->count());
    
    // CCLog("### number of reputation orbs : %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str()));
    
    for (int i = 0; i < allReputationOrbs->count(); i++)
    {
        // CCLog ("#####################################################");
        // CCLog ("### reputation orb : %d", i);
        
        ReputationOrb* ro = (ReputationOrb*) allReputationOrbs->objectAtIndex(i);
        
        if(ro->collected)
        {
            return;
        }
        
        CCPoint tilePos = ccpAdd(ro->orbSprite->getPosition(), MapHandler::getThis()->getMap()->getPosition());
        tilePos = MapHandler::getThis()->tilePosFromLocation(tilePos);
        
        // orb position
        ss.str(std::string());
        ss << username << "_orb_" << i << "_position_x";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tilePos.x);
        
        ss.str(std::string());
        ss << username << "_orb_" << i << "_position_y";
        CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), tilePos.y);
        
        // CCLog("tile pos is: (%f, %f)", tilePos.x, tilePos.y);
    }
    
}

void GameManagement::saveObjectiveData(int type)
{
    // CCLog("#############################################");
    // CCLog("## save Objective");
    // CCLog("#############################################");
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    // CCLog("## username: %s", username.c_str());
    
    // objective current id
    ss.str(std::string());
    ss << username << "_objective_current_id";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), ObjectiveHandler::getThis()->currID);
    
    // CCLog("## current id: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0));
    
    // objective next id
    ss.str(std::string());
    ss << username << "_objective_next_id";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), ObjectiveHandler::getThis()->nextID);
    
    // CCLog("## next id: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0));
    
    // objective progress number
    ss.str(std::string());
    ss << username << "_objective_progress_number";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), ObjectiveHandler::getThis()->progressNumber);
    
    // CCLog("## progress number: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0));
    
    // objective start population
    ss.str(std::string());
    ss << username << "_objective_start_population";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), ObjectiveHandler::getThis()->startPopulation);
    
    // CCLog("## start population: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0));
    
    // objective target population
    ss.str(std::string());
    ss << username << "_objective_target_population";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), ObjectiveHandler::getThis()->targetPopulation);
    
    // CCLog("## target population: %d", CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0));
    
    // objective has timer
    ss.str(std::string());
    ss << username << "_objective_hasTimer";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), GameHUD::getThis()->hasTimer);
    
    // CCLog("## has timer: %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str(), false) ? "true" : "false");
    
    // objective target time
    ss.str(std::string());
    ss << username << "_objective_targetTime";
    CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), GameHUD::getThis()->targetTime);
    
    // CCLog("## target time: %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0));
    
    // objective current time
    ss.str(std::string());
    ss << username << "_objective_currentTime";
    CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), GameHUD::getThis()->currentTime);
    
    // CCLog("## current time: %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0));
    
    // objective scenario time
    ss.str(std::string());
    ss << username << "_objective_scenarioTime";
    CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), GameHUD::getThis()->scenarioTime);
    
    // CCLog("## scenario time: %f", CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0));
    
    // objective has scenario
    ss.str(std::string());
    ss << username << "_objective_hasScenario";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), GameHUD::getThis()->hasScenario);
    
    // CCLog("## has scenario: %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str(), false) ? "true" : "false");
    // CCLog("#############################################");
}

void GameManagement::saveTutorialData(int type)
{
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    // save the status weather it is in tutorial
    ss.str(std::string());
    ss << username << "_tutorial_isInTutorial";
    bool isInTutorial = false;
    if(UserProfile::getThis()->gameLevel != 0 && UserProfile::getThis()->gameLevel != 2)
    {
        isInTutorial = false;
    }
    else
    {
        isInTutorial = TutorialManager::getThis()->active;
    }
    
    // CCLog("** save tutorial data - key: %s", ss.str().c_str());
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), isInTutorial);
    // CCLog("** isInTutorial: %s", CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str(), false) ? "true" : "false");
}

// try this save data first for testing purpose
void GameManagement::saveData(int type)
{
    saveSystemData(type);
    saveSpritesData(type);
    saveBuildingData(type);
    saveRoadData(type);
    saveReputationOrbData(type);
    saveObjectiveData(type);
    saveTutorialData(type);
}

void GameManagement::loadGameFile()
{
    CCLog("loading file......");
    int level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", -1);
    std::string map = CCUserDefault::sharedUserDefault()->getStringForKey("map", "");
    int time = CCUserDefault::sharedUserDefault()->getIntegerForKey("time", -1);
    int gold = CCUserDefault::sharedUserDefault()->getIntegerForKey("gold", -1);
    int reputation = CCUserDefault::sharedUserDefault()->getIntegerForKey("reputation", -1);
    
    if(level >= 0)
    {
        CCLog("level is: %d", level);
    }
    else
    {
        CCLog("level is: no data");
    }
    
    if(map.compare("") != 0)
    {
        CCLog("map is: %s", map.c_str());
    }
    else
    {
        CCLog("map is: no data");
    }
    
    if(time >= 0)
    {
        CCLog("time is: %d", time);
    }
    else
    {
        CCLog("time is: no data");
    }
    
    if(gold >= 0)
    {
        CCLog("gold is: %d", gold);
    }
    else
    {
        CCLog("gold is: no data");
    }
    
    if(reputation >= 0)
    {
        CCLog("reputation is: %d", reputation);
    }
    else
    {
        CCLog("reputation is: no data");
    }
    CCLog("loading finish!");
}

void GameManagement::loadReputationOrbData(int type)
{
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    ss.str(std::string());
    ss << username << "_numberOfReputationOrbs";
    int orbNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    for (int i = 0; i < orbNumber; i++)
    {
        ss.str(std::string());
        ss << username << "_orb_" << i << "_position_x";
        float positionX = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << username << "_orb_" << i << "_position_y";
        float positionY = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        ReputationOrb* ro = ReputationOrb::create("REN", UserProfile::getThis()->configSettings->token_disappear_time);
        CCSprite* newToken = ro->getSprite();
        newToken->setAnchorPoint(ccp(0.5, 0.5));
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        CCSize spriteSize = newToken->getContentSize();
        
        newToken->setScale(screenSize.width / spriteSize.width * 0.05f);
        
        SpriteHandler::getThis()->tokensOnMap->addObject(ro);
        
        MapHandler::getThis()->getMap()->addChild(newToken, 99999);
        
        CCPoint targetPos = ccp(positionX, positionY);
        
        CCPoint target = MapHandler::getThis()->locationFromTilePos(&targetPos);
        
        newToken->setPosition(target);
    }
}

void GameManagement::loadSpritesData(int type)
{
    // CCLog("trying to load sprites data......");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    ss.str(std::string());
    ss << username << "_numberOfGameSprites";
    int game_sprites_number = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    /* load all the sprites without mutual links first */
    for (int i = 0; i < game_sprites_number; i++)
    {
        // sprite villager class
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_villagerClass";
        int vcNum = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        VillagerClass vc = (VillagerClass) vcNum;
        
        // sprite positions
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_x";
        float posX = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_position_y";
        float posY = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        CCPoint tilePos = ccp(posX, posY);
        
        // CCLog("****** sprite tilePos is: (%f, %f)", tilePos.x, tilePos.y);
        
        GameSprite* gameSprite = SpriteHandler::getThis()->getSpriteToMap(tilePos, vc);
        
        // sprite unique id
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_uniqueID";
        int uniID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->uniqueID = uniID;
        
        // sprite displayed name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteDisplayedName";
        std::string name = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->spriteDisplayedName = name;
        
        // sprite name
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteName";
        std::string sname = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->spriteName = sname;
        // CCLog("********** sprite name is: %s", gameSprite->spriteName.c_str());
        gameSprite->changeSpriteRepWhenLoadingGame();
        
        // sprite rep visibility
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteRepVisible";
        bool tempVisible = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->spriteRep->setVisible(tempVisible);
        
        // sprite foodCarried
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_foodCarried";
        int foodC = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->foodCarried = foodC;
        
        // sprite current direction
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentDirection";
        std::string currDir = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->currentDir = currDir;
        
        // sprite cumulative time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime";
        float cumuTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeTime = cumuTime;
        
        // sprite cumulative time for happiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_happiness";
        float cumuHappiness = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeTime_happiness = cumuHappiness;
        
        // sprite cumulative time for attack
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeTime_attack";
        float cumuTimeAttack = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeTime_attack = cumuTimeAttack;
        
        // sprite is token drop cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isTokenDropCooldown";
        bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->is_token_drop_cooldown = flag;
        
        // sprite token drop cooldown time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tokenDropCooldownTime";
        float tokenDropTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->token_drop_cooldown_time = tokenDropTime;
        
        // sprite go to town hall
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gotoTownHall";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->goToTownHall = flag;
        
        // sprite has assigned
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_hasAssigned";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->hasAssigned = flag;
        
        // sprite cumulative check time
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_cumulativeCheckTime";
        float cumuCheckTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->cumulativeCheckTime = cumuCheckTime;
        
        // sprite targetHungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        float targetHungry = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetHungry = targetHungry;
        
        // sprite targetEnergy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetEnergy";
        float targetEnergy = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetEnergy = targetEnergy;
        
        // sprite targetHappiness
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHappiness";
        float targethappiness = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetHappiness = targethappiness;
        
        // sprite enermy && sprite pre-enermy -> these two parts can be only done after all game sprites have already been loaded.
        
        // sprite nextTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_x";
        float value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->nextTile.x = value;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextTile_y";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->nextTile.y = value;
        
        // sprite currentTile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_x";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->currentTile.x = value;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentTile_y";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->currentTile.y = value;
        
        // sprite target location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_x";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetLocation.x = value;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation_y";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->targetLocation.y = value;
        
        /*
         // sprite stop action
         ss.str(std::string());
         ss << username << "_sprite_" << i << "_stopAction";
         flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
         gameSprite->stopAction = flag;
         */
        
        // sprite is moving
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isMoving";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isMoving = flag;
        
        // sprite try escape
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_tryEscape";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->tryEscape = flag;
        
        // sprite mGameCurrentEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameCurrentEndurance";
        int val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->mGameCurrentEndurance = val;
        
        // sprite mGameMaxEndurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameMaxEndurance";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->mGameMaxEndurance = val;
        
        // sprite mGameWarMode
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_mGameWarMode";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->mGameWarMode = flag;
        
        // sprite current money rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentMoneyRob";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->current_money_rob = val;
        
        // sprite current food rob
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentFoodRob";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->current_food_rob = val;
        
        // sprite should stop next square
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldStopNextSquare";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->shouldStopNextSquare = flag;
        
        // sprite wander flag
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_wanderFlag";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->wanderFlag = flag;
        
        // sprite should set visible next frame
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_shouldSetVisibleNextFrame";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->shouldSetVisibleNextFrame = flag;
        
        // sprite job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_job";
        SpriteJob sj = (SpriteJob) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->job = sj;
        
        // sprite is doing job
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isDoingJob";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isDoingJob = flag;
        
        // sprite combat state
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_combatState";
        SpriteCombatState scs = (SpriteCombatState) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->combatState = scs;
        
        // sprite is leaving next update
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isLeavingNextUpdate";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isLeavingNextUpdate = flag;
        
        // sprite is in building
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInBuilding";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isInBuilding = flag;
        
        // sprite gender
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_gender";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->gender = (char) val;
        
        // sprite spriteclass
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteClass";
        std::string tempStr = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        gameSprite->spriteClass = tempStr;
        
        // sprite movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_movementSpeed";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->movementSpeed = value;
        
        // sprite last frame action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_lastFrameAction";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->lastFrameAction = (SpriteAction) val;
        
        // sprite current action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentAction";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->currAction = (SpriteAction) val;
        
        // sprite next action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_nextAction";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->nextAction = (SpriteAction) val;
        
        // sprite future action 1
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction1";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->futureAction1 = (SpriteAction) val;
        
        // sprite future action 2
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_futureAction2";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->futureAction2 = (SpriteAction) val;
        
        // sprite batch layer index
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_batchLayerIndex";
        val = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        gameSprite->batchLayerIndex = val;
        
        // sprite barHp value
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_barHPValue";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->barHP->setValue(value);
        
        // Note, after load bar hp value, it may be required to adjust the progress bar (hp bar) value of that sprite.
        
        // sprite is in combat
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInCombat";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isInCombat = flag;
        
        // sprite is in attack action
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_isInAttackAction";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->isInAttackAction = flag;
        
        // sprite just move one tile
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_justMoveOneTile";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        gameSprite->justMoveOneTile = flag;
        
        // sprite frame Width
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameWidth";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->frameWidth = value;
        
        // sprite frame Height
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_frameHeight";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->frameHeight = value;
        
        // sprite possession happiness rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_happinessRating";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->happinessRating = value;
        
        // sprite possession current hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentHungry";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->currentHungry = value;
        
        // sprite possession target hungry
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHungry";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->targetHungry = value;
        
        // sprite possession energy rating
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_energyRating";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->energyRating = value;
        
        // sprite possession default work rate
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkRate";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_work_rate = value;
        
        // sprite possession default happiness limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHappinessLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_hapiness_limit = value;
        
        // sprite possession default work unit per day
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultWorkUnitPerDay";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_work_unit_per_day = value;
        
        // sprite possession default movement speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultMovementSpeed";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_movement_speed = value;
        
        // sprite possession default animated speed
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultAnimatedSpeed";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_animate_speed = value;
        
        // sprite possession default hungry limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultHungryLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_hungry_limit = value;
        
        // sprite possession default food carriage limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultFoodCarriageLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_food_carriage_limit = value;
        
        // sprite possession default energy limit
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_defaultEnergyLimit";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->default_energy_limit = value;
        
        // sprite possession attack cooldown
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackCooldown";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->attack_cooldown = value;
        
        // sprite possession attack power max
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMax";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->attack_power_max = value;
        
        // sprite possession attack power min
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_attackPowerMin";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->attack_power_min = value;
        
        // sprite possession current endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_currentEndurance";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->current_endurance = value;
        
        // sprite possession max endurance
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_maxEndurance";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->getPossessions()->max_endurance = value;
        
        // sprite idle delay
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_idleDelay";
        value = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        gameSprite->idleDelay = value;
        
        /*
         * we must load the path of the game sprites
         */
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_numberOfPathTiles";
        int numberOfTiles = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
        
        gameSprite->path = CCArray::create();
        gameSprite->path->retain();
        
        if(numberOfTiles > 0)
        {
            for (int j = 0; j < numberOfTiles; j++)
            {
                ss.str(std::string());
                ss << username << "_sprite_" << i << "_path_" << j << "posX";
                
                float pX = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0);
                
                ss.str(std::string());
                ss << username << "_sprite_" << i << "_path_" << j << "posY";
                
                float pY = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0);
                
                PathfindingNode* node = new PathfindingNode();
                node->tilepos = ccp(pX, pY);
                node->parent = NULL;
                node->F = 0;
                node->G = 0;
                node->H = 0;
                
                gameSprite->path->addObject(node);
            }
        }
        
        if(gameSprite->villagerClass == V_SOLDIER || gameSprite->villagerClass == V_BANDIT)
        {
            gameSprite->spriteRep->setVisible(true);
            gameSprite->spriteRep->setZOrder( 99999 );
            gameSprite->speechBubble->setZOrder( 99999 );
        }
    }
    
    // all the game sprites have been loaded, then load the links between these gamesprites!
    // currently only enermy and pre-enermy are required to be loaded.
    for (int i = 0; i < game_sprites_number; i++)
    {
        // get the related game sprite first
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_uniqueID";
        int uniID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        GameSprite* theSprite = NULL;
        
        if(uniID < 0)
        {
            theSprite = NULL;
        }
        else
        {
            for (int j = 0; j < SpriteHandler::getThis()->spritesOnMap->count(); j++)
            {
                GameSprite* gs = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(j);
                
                if(gs->uniqueID == uniID)
                {
                    theSprite = gs;
                    break;
                }
            }
        }
        
        if(theSprite == NULL)
        {
            continue;
        }
        
        // then update his enermy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_enermy";
        int enermyID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        GameSprite* enermySprite = NULL;
        for (int j = 0; j < SpriteHandler::getThis()->spritesOnMap->count(); j++)
        {
            GameSprite* gs = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(j);
            
            if(gs->uniqueID == enermyID)
            {
                enermySprite = gs;
                break;
            }
        }
        
        theSprite->enermy = enermySprite;
        
        // finally, update his pre-enermy
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_preEnermy";
        int preEnermyID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        GameSprite* preEnermySprite = NULL;
        for (int j = 0; j < SpriteHandler::getThis()->spritesOnMap->count(); j++)
        {
            GameSprite* gs = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(j);
            
            if(gs->uniqueID == preEnermyID)
            {
                preEnermySprite = gs;
                break;
            }
        }
        
        theSprite->preEnermy = preEnermySprite;
    }
}

void GameManagement::loadSpritesLinks(int type)
{
    // CCLog("trying to load sprites links data......");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    ss.str(std::string());
    ss << username << "_numberOfGameSprites";
    int game_sprites_number = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    /* load all the sprites' mutual links */
    for (int i = 0; i < game_sprites_number; i++)
    {
        // we use the sprite displayed name to get the game sprite first
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_spriteDisplayedName";
        std::string name = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        
        GameSprite* theGameSprite = NULL;
        
        for (int j = 0; j < SpriteHandler::getThis()->spritesOnMap->count(); j++)
        {
            GameSprite* gs = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(j);
            
            if(gs->spriteDisplayedName.compare(name.c_str()) == 0)
            {
                theGameSprite = gs;
                break;
            }
        }
        
        // the sprite cannot be null when loading the links
        if(theGameSprite == NULL)
        {
            continue;
        }
        
        // loading the sprite job location.
        CCArray* allBuildings = BuildingHandler::getThis()->allBuildingsOnMap;
        
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_jobLocation";
        
        int uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        if(uniqueID < 0)
        {
            theGameSprite->setJobLocation(NULL);
        }
        else
        {
            for (int k = 0; k < allBuildings->count(); k++)
            {
                Building* tempBuilding = (Building*) allBuildings->objectAtIndex(k);
                if(tempBuilding->uniqueID == uniqueID)
                {
                    theGameSprite->setJobLocation(tempBuilding);
                    break;
                }
            }
        }
        
        // sprite possession home location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_homeLocation";
        
        uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        if(uniqueID < 0)
        {
            theGameSprite->setHome(NULL);
        }
        else
        {
            for (int k = 0; k < allBuildings->count(); k++)
            {
                Building* tempBuilding = (Building*) allBuildings->objectAtIndex(k);
                if(tempBuilding->uniqueID == uniqueID)
                {
                    
                    theGameSprite->setHome(tempBuilding);
                    break;
                }
            }
        }
        
        // sprite possession job location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_possessionJobLocation";
        
        uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        if(uniqueID < 0)
        {
            theGameSprite->getPossessions()->jobLocation = NULL;
        }
        else
        {
            for (int k = 0; k < allBuildings->count(); k++)
            {
                Building* tempBuilding = (Building*) allBuildings->objectAtIndex(k);
                if(tempBuilding->uniqueID == uniqueID)
                {
                    theGameSprite->getPossessions()->jobLocation = tempBuilding;
                    break;
                }
            }
        }
        
        // sprite possession target location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetLocation";
        
        uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        if(uniqueID < 0)
        {
            theGameSprite->setTargetLocation(NULL);
        }
        else
        {
            for (int k = 0; k < allBuildings->count(); k++)
            {
                Building* tempBuilding = (Building*) allBuildings->objectAtIndex(k);
                if(tempBuilding->uniqueID == uniqueID)
                {
                    theGameSprite->setTargetLocation(tempBuilding);
                    break;
                }
            }
        }
        
        // sprite possession target home location
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_targetHomeLocation";
        
        uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        if(uniqueID < 0)
        {
            theGameSprite->getPossessions()->targetHomeLocation = NULL;
        }
        else
        {
            for (int k = 0; k < allBuildings->count(); k++)
            {
                Building* tempBuilding = (Building*) allBuildings->objectAtIndex(k);
                if(tempBuilding->uniqueID == uniqueID)
                {
                    theGameSprite->getPossessions()->targetHomeLocation = tempBuilding;
                    break;
                }
            }
        }
        
        // sprite last target
        ss.str(std::string());
        ss << username << "_sprite_" << i << "_lastTarget";
        
        uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        if(uniqueID < 0)
        {
            theGameSprite->lastTarget = NULL;
        }
        else
        {
            for (int k = 0; k < allBuildings->count(); k++)
            {
                Building* tempBuilding = (Building*) allBuildings->objectAtIndex(k);
                if(tempBuilding->uniqueID == uniqueID)
                {
                    theGameSprite->lastTarget = tempBuilding;
                    break;
                }
            }
        }
    }
}

void GameManagement::loadSystemData(int type)
{
    // CCLog("trying to load system data......");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    //  get the level number first
    ss.str(std::string());
    ss << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    UserProfile::getThis()->gameLevel = levelNumber;
    
    // load the town hall level
    ss.str(std::string());
    ss << username << "_townhallLevel";
    int townhallLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameManager::getThis()->town_hall_level = townhallLevel;
    
    // load the current money the user have
    ss.str(std::string());
    ss << username << "_currentGold";
    int currentGold = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->money = currentGold;
    
    // load the target money the user have
    ss.str(std::string());
    ss << username << "_targetGold";
    int targetGold = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->targetMoney = targetGold;
    
    // load the current reputation
    ss.str(std::string());
    ss << username << "_currentReputation";
    int currentReputation = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->reputation = currentReputation;
    
    // load the target reputation
    ss.str(std::string());
    ss << username << "_targetReputation";
    int targetReputation = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->targetReputation = targetReputation;
    
    // load the number of days
    ss.str(std::string());
    ss << username << "_date";
    int numberOfDays = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameHUD::getThis()->setNumberOfDays(numberOfDays);
}

void GameManagement::loadBuildingData(int type)
{
    // CCLog("trying to load building data......");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    // get the number of Buildings
    ss.str(std::string());
    ss << username << "_numberOfBuildings";
    int number_of_buildings = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    // loop to load all the buildings in the database
    for (int i = 0; i < number_of_buildings; i++)
    {
        // load the building id of the current building
        ss.str(std::string());
        ss << username << "_building_" << i << "_id";
        int id = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        // CCLog("****** the id of the building is %d", id);
        
        // get the source image copy of the building want to load
        Building* buildingToBuy = BuildingHandler::getThis()->getBuilding(id);
        
        // load the positionX and positionY of the building
        ss.str(std::string());
        ss << username << "_building_" << i << "_positionX";
        float posX = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << username << "_building_" << i << "_positionY";
        float posY = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        
        CCPoint tilePos = ccp(posX, posY);
        
        // build the building on the map
        Building* bui = MapHandler::getThis()->BuildOnMap(tilePos, buildingToBuy);
        
        /* buildings are set up, load the stats of them */
        if(bui == NULL)
        {
            continue;
        }
        
        // load building unique id
        ss.str(std::string());
        ss << username << "_building_" << i << "_uniqueID";
        int uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->uniqueID = uniqueID;
        
        // building id
        bui->ID = id;
        
        // building in progress
        ss.str(std::string());
        ss << username << "_building_" << i << "_inProgress";
        bool inProgress = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->inProgress = inProgress;
        
        // building type
        ss.str(std::string());
        ss << username << "_building_" << i << "_buildingType";
        BuildingCategory bc = (BuildingCategory) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->buildingType = bc;
        
        // building farm state
        ss.str(std::string());
        ss << username << "_building_" << i << "_farmState";
        FarmState fs = (FarmState) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->farmState = fs;
        
        // building population limit
        ss.str(std::string());
        ss << username << "_building_" << i << "_populationLimit";
        int populationLimit = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->populationLimit = populationLimit;
        
        // building builder limit
        ss.str(std::string());
        ss << username << "_building_" << i << "_builderLimit";
        int builderLimit = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->builderLimit = builderLimit;
        
        // building task type
        ss.str(std::string());
        ss << username << "_building_" << i << "_taskType";
        TaskType tt = (TaskType) CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->taskType = tt;
        
        // building target work unit
        ss.str(std::string());
        ss << username << "_building_" << i << "_tagetWorkUnit";
        float targetWorkUnit = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->targetWorkUnit = targetWorkUnit;
        
        // building current storage
        ss.str(std::string());
        ss << username << "_building_" << i << "_currentStorage";
        int currentStorage = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->currentStorage = currentStorage;
        
        // building storage limit
        ss.str(std::string());
        ss << username << "_building_" << i << "_storageLimit";
        int storageLimit = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->storageLimit = storageLimit;
        
        // building build unit required
        ss.str(std::string());
        ss << username << "_building_" << i << "_buildUnitRequired";
        int build_unit_required = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->build_uint_required = build_unit_required;
        
        // building build unit current
        ss.str(std::string());
        ss << username << "_building_" << i << "_buildUnitCurrent";
        int build_unit_current = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->build_uint_current = build_unit_current;
        
        // building work unit required
        ss.str(std::string());
        ss << username << "_building_" << i << "_workUnitRequired";
        int work_unit_required = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->work_unit_required = work_unit_required;
        
        // building work unit current
        ss.str(std::string());
        ss << username << "_building_" << i << "_workUnitCurrent";
        float work_unit_current = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->work_unit_current = work_unit_current;
        
        // building recovery rate
        ss.str(std::string());
        ss << username << "_building_" << i << "_recoveryRate";
        int recoveryRate = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->recovery_rate = recoveryRate;
        
        // building food consumption rate
        ss.str(std::string());
        ss << username << "_building_" << i << "_foodConsumptionRate";
        int foodConsumptionRate = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->food_consumption_rate = foodConsumptionRate;
        
        // building baseGID
        ss.str(std::string());
        ss << username << "_building_" << i << "_baseGID";
        int baseGID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->baseGID = baseGID;
        
        // building maxGID
        ss.str(std::string());
        ss << username << "_building_" << i << "_maxGID";
        int maxGID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->maxGID = maxGID;
        
        // building currGID
        ss.str(std::string());
        ss << username << "_building_" << i << "_currGID";
        int currGID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->currGID = currGID;
        
        // building lastGID
        ss.str(std::string());
        ss << username << "_building_" << i << "_lastGID";
        int lastGID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->lastGID = lastGID;
        
        // building anim frame count
        ss.str(std::string());
        ss << username << "_building_" << i << "_animFrameCount";
        int animframe_count = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->animframe_count = animframe_count;
        
        // building anim random
        ss.str(std::string());
        ss << username << "_building_" << i << "_animRandom";
        bool anim_random = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->anim_random = anim_random;
        
        // building anim random chance
        ss.str(std::string());
        ss << username << "_building_" << i << "_animRandomChance";
        float anim_random_chance = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->anim_random_chance = anim_random_chance;
        
        // building anim curr chance
        ss.str(std::string());
        ss << username << "_building_" << i << "_animCurrChance";
        float anim_curr_chance = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->anim_curr_chance = anim_curr_chance;
        
        // building anim triggered
        ss.str(std::string());
        ss << username << "_building_" << i << "_animTriggered";
        float anim_triggered = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->anim_triggered = anim_triggered;
        
        // building width
        ss.str(std::string());
        ss << username << "_building_" << i << "_width";
        int buildingWidth = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->width = buildingWidth;
        
        // building height
        ss.str(std::string());
        ss << username << "_building_" << i << "_height";
        int buildingHeight = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->height = buildingHeight;
        
        // building current level
        ss.str(std::string());
        ss << username << "_building_" << i << "_currentLevel";
        int currentLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->currentLevel = currentLevel;
        
        // building cost
        ss.str(std::string());
        ss << username << "_building_" << i << "_cost";
        int buildingCost = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->buildingCost = buildingCost;
        
        // load the all the members belonging to this building
        ss.str(std::string());
        ss << username << "_building_" << i << "_memberNumber";
        int memberNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        bui->memberSpriteList->removeAllObjects();
        CC_SAFE_RELEASE(bui->memberSpriteList);
        bui->memberSpriteList = CCArray::create();
        bui->memberSpriteList->retain();
        
        for (int j = 0; j < memberNumber; j++)
        {
            ss.str(std::string());
            ss << username << "_building_" << i << "_member_" << j;
            int spriteID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
            
            if(spriteID >= 0)
            {
                for (int k = 0; k < SpriteHandler::getThis()->spritesOnMap->count(); k++)
                {
                    GameSprite* gs = (GameSprite*) SpriteHandler::getThis()->spritesOnMap->objectAtIndex(k);
                    if(gs->uniqueID == spriteID)
                    {
                        bui->memberSpriteList->addObject(gs);
                    }
                }
            }
        }
        
        // building upgrade unit max
        ss.str(std::string());
        ss << username << "_buidling_" << i << "_upgradeUnitMax";
        float upgradeUnitMax = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->upgrade_unit_max = upgradeUnitMax;
        
        // building current upgrade unit
        ss.str(std::string());
        ss << username << "_building_" << i << "_currentUpgradeUnit";
        float current_upgrade_unit = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->current_upgrade_unit = current_upgrade_unit;
        
        // building is current harvest
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentHarvest";
        bool isCurrentHarvest = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentHarvest = isCurrentHarvest;
        
        // buildnig is current constructing
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentConstructing";
        bool isCurrentConstructing = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentConstructing = isCurrentConstructing;
        
        // building is current working
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentWorking";
        bool isCurrentWorking = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentWorking = isCurrentWorking;
        
        // building is current upgrading
        ss.str(std::string());
        ss << username << "_building_" << i << "_isCurrentUpgrading";
        bool isCurrentUpgrading = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        bui->isCurrentUpgrading = isCurrentUpgrading;
        
        // building number of jobs
        ss.str(std::string());
        ss << username << "_building_" << i << "_numberOfJobs";
        int numberOfJobs = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        bui->number_of_jobs = numberOfJobs;
        
        // building cumulativeTimeUpgrading
        ss.str(std::string());
        ss << username << "_building_" << i << "_cumulativeTimeUpgrading";
        float cumulatedTimeUpgrading = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->cumulatedTimeUpgrading = cumulatedTimeUpgrading;
        
        // building cumulativeTimeResting
        ss.str(std::string());
        ss << username << "_building_" << i << "_cumulativeTimeResting";
        float cumulatedTimeResting = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        bui->cumulatedTimeResting = cumulatedTimeResting;
        
        bui->updateFrame();
    }
}

void GameManagement::loadRoadData(int type)
{
    CCLog("trying to load road data......");
    
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    // get the number of Buildings
    ss.str(std::string());
    ss << username << "_numberOfPathTiles";
    int number_of_pathTiles = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    
    // loop to load all the buildings in the database
    for (int i = 0; i < number_of_pathTiles; i++)
    {
        ss.str(std::string());
        ss << username << "_path_" << i << "_posX";
        int posX = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        // CCLog ("### path position x : %d", posX);
        
        ss.str(std::string());
        ss << username << "_path_" << i << "_posY";
        int posY = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        // CCLog ("### path position y : %d", posY);
        
        MapTile* mapTile = MapHandler::getThis()->getTileAt(posX, posY);
        
        CCPoint position = ccp(posX, posY);
        
        if(!mapTile->isPath)
        {
            MapHandler::getThis()->PathLine(position, position);
        }
    }
}

void GameManagement::loadObjectiveData(int type)
{
    stringstream ss;
    ss.str(std::string());
    
    // 0 means auto save, 1 means fixed save, others mean custom save
    if (type == 0)
    {
        ss << "autosave_";
    }
    else if(type == 1)
    {
        ss << "fixedsave_";
    }
    else if(type == 2)
    {
        ss << "customsave_";
    }
    else
    {
        ss << "backupsave_";
    }
    
    ss << UserProfile::getThis()->username;
    
    std::string username = ss.str();
    
    // objective current id
    ss.str(std::string());
    ss << username << "_objective_current_id";
    int currID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
    ObjectiveHandler::getThis()->playObjectiveWithObjectiveID(currID);
    
    // objective next id;
    ss.str(std::string());
    ss << username << "_objective_next_id";
    int nextID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
    ObjectiveHandler::getThis()->nextID = nextID;
    
    // objective progress number
    ss.str(std::string());
    ss << username << "_objective_progress_number";
    int progress_number = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
    ObjectiveHandler::getThis()->progressNumber = progress_number;
    
    // objective start population
    ss.str(std::string());
    ss << username << "_objective_start_population";
    int start_population = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
    ObjectiveHandler::getThis()->startPopulation = start_population;
    
    // objective target population
    ss.str(std::string());
    ss << username << "_objective_target_population";
    int target_population = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
    ObjectiveHandler::getThis()->targetPopulation = target_population;
    
    // objective has timer
    ss.str(std::string());
    ss << username << "_objective_hasTimer";
    bool hasTimer = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str(), false);
    GameHUD::getThis()->hasTimer = hasTimer;
    
    // objective target time
    ss.str(std::string());
    ss << username << "_objective_targetTime";
    float targetTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0);
    GameHUD::getThis()->targetTime = targetTime;
    
    // objective current time
    ss.str(std::string());
    ss << username << "_objective_currentTime";
    float currentTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0);
    GameHUD::getThis()->currentTime = currentTime;
    
    // objective scenario time
    ss.str(std::string());
    ss << username << "_objective_scenarioTime";
    float scenarioTime = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str(), 0);
    GameHUD::getThis()->scenarioTime = scenarioTime;
    
    // objective has scenario
    ss.str(std::string());
    ss << username << "_objective_hasScenario";
    bool hasScenario = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str(), false);
    GameHUD::getThis()->hasScenario = hasScenario;
}

void GameManagement::loadTutorialData(int type)
{
    if(type == 1)
    {
        // fixed save
        GameScene::getThis()->reSetupLevel(true);
    }
    else if(type != 0 && type != 1 && type != 2)
    {
        // backup save
        // play scenario already, then start to play tutorial again
        UserProfile::getThis()->systemConfig->skipSenario = true;
        GameScene::getThis()->configLevelData();
    }
}

bool GameManagement::loadData(int type)
{
    std::string username = UserProfile::getThis()->username;
    
    stringstream ss;
    
    if(type == 0)
    {
        // auto save
        ss.str(std::string());
        ss << "autosave_" << username << "_hasSavedGame";
        if(!CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()))
        {
            return false;
        }
    }
    else if(type == 1)
    {
        // fixed save
        ss.str(std::string());
        ss << "fixedsave_" << username << "_hasSavedGame";
        if(!CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()))
        {
            return false;
        }
    }
    else if(type == 2)
    {
        // custom save
        ss.str(std::string());
        ss << "customsave_" << username << "_hasSavedGame";
        if(!CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()))
        {
            return false;
        }
        else
        {
            ss.str(std::string());
            ss << "customsave_";
            ss << UserProfile::getThis()->username;
            
            std::string username1 = ss.str();
            
            // load the status weather it is in tutorial
            ss.str(std::string());
            ss << username1 << "_tutorial_isInTutorial";
            bool isInTutorial = false;
            if(UserProfile::getThis()->gameLevel != 0 && UserProfile::getThis()->gameLevel != 2)
            {
                isInTutorial = false;
            }
            else
            {
                isInTutorial = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str(), false);
            }
            
            if(isInTutorial)
            {
                ss.str(std::string());
                ss << "backupsave_" << username << "_hasSavedGame";
                if(!CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()))
                {
                    return false;
                }
                type = 3;
            }
        }
    }
    else
    {
        // backup save
        ss.str(std::string());
        ss << "backupsave_" << username << "_hasSavedGame";
        if(!CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str()))
        {
            return false;
        }
    }
    
    // CCLog("trying to load the game data......");
    // warning: for loading the game data, always load the system data first (before loading the building and sprites data!);
    loadSystemData(type);
    
    // we need to load all sprites before loading all the buildings since the buildings requires links from sprites.
    loadSpritesData(type);
    loadBuildingData(type);
    
    // then we update the links of sprites from buildings after all buildngs have been loaded.
    loadSpritesLinks(type);
    
    loadRoadData(type);
    loadReputationOrbData(type);
    // CCLog("loading data finished......");
    
    if(type == 0 || type == 2)
    {
        loadObjectiveData(type);
    }
    loadTutorialData(type);
    
    return true;
}