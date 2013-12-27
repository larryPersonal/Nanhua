//
//  Behavior.cpp
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#include "Behavior.h"
#include "Sprite.h"
#include "Building.h"
#include "GameScene.h"
//#include "cocos2d.h"

/*Glue functions. Defined here to avoid double include problem existing in Sprite.h*/
static GameSprite* sprite = NULL;

/*
static int GetRatioSocialInteraction_Glue(lua_State *L)
{
    lua_pushinteger(L, GameScene::getThis()->policyHandler->getCulturalExchangeRatio());
    
    return 1;
}

static int TriggerSocialInteraction_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        
        BuildingHandler* bh = GameScene::getThis()->buildingHandler;
        if (bh->socialOnMap->count() == 0)
        {
            lua_pushboolean(L, false); //no social buildings
        }
        else
        {
            SpriteHandler* sh = GameScene::getThis()->spriteHandler;
            if (sh->aliensOnMap->count() == 0)
            {
                lua_pushboolean(L, false); //no aliens
            }
            else
            {
                //find the first alien that isn't interacting yet.
                int foundAlien = false;
                for (int i = 0; i < sh->aliensOnMap->count(); ++i)
                {
                    if (sprite->isInteractingSocial) continue;
                    GameSprite* s = (GameSprite*)sh->aliensOnMap->objectAtIndex(i);
                    if (!s->isInteractingSocial && !sprite->isInteractingSocial)
                    {
                        //find a random social building ID.
                        int randomIndex = rand()%bh->socialOnMap->count();
                        
                        Building *b = (Building*)bh->socialOnMap->objectAtIndex(randomIndex);
                       
                        sprite->PathToBuildingOverride(b->ID);
                        
                        s->PathToBuildingOverride(b->ID);
                        //if both are available, make both THIS sprite and the alien go there.
                        //mark both as Interacting so we can deal with that in the social building itself.
                        
                        sprite->isInteractingSocial = true;
                        s->isInteractingSocial = true;
                   }
                }
                
                
                if (foundAlien == false)
                {
                    lua_pushboolean(L, false); //all aliens are interacting already
                }
                
            }
            
            
        }
    }
    
    return 1;
}*/

static int IsAtMaxLevel_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        lua_pushboolean(L, sprite->getPossessions()->isAtMaxLevel());
    }
    
    return 1;
}

static int IsAtAbsoluteMaxLevel_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        lua_pushboolean(L, sprite->getPossessions()->isAtAbsoluteMax());
    }
    return 1;
}

/*
static int HasMetUpgradeCriteria_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        lua_pushboolean(L, sprite->CheckUpgradeReq());
    }
    return 1;
}*/

/*
static int UpgradeClass_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        sprite->PathToHighTemple(); //which will set shouldUpgrade to true;
        sprite->saySpeech("I'm due for an upgrade.", 2.0f);
        lua_pushboolean(L, true);
    }
    return 1;
}*/

/*
static int ConvertToLocal_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        sprite->PathToHighTemple(); //which will set shouldUpgrade to true;
        sprite->saySpeech("Trading in passport.", 2.0f);
        lua_pushboolean(L, true);
    }
    return 1;
}*/

/* calls the current sprite set in the global to Go Home.*/
static int GoHome_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        lua_pushboolean(L, sprite->PathToHome());
        
    }
    return 1;
}

//REWRITE. Larry

/*
//Note: to actually BUY a house please don't check if a better house is available. Otherwise the sprite won't squeeze.
//This function specifically avoids putting a sprite in a squeezy house.
static int IsBetterHouseAvailable_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        
 
        //I won't move house if mine isn't overpopulated.
        if (sprite->getPossessions()->hasHouse && !sprite->getPossessions()->isSqueezing)
        {
            lua_pushboolean(L, false);
            return 1;
            
        }
        
        
        BuildingHandler* bh = GameScene::getThis()->buildingHandler;
        if (bh == NULL)
        {
            lua_pushboolean(L, false);
            return 1;
        }

        
        if (bh->housingOnMap->count() == 0)
        {
            lua_pushboolean(L, false);
            return 1;
        }
        
        for (int i = 0; i < bh->housingOnMap->count(); ++i)
        {
            Building * b = (Building*)bh->housingOnMap->objectAtIndex(i);
            if (!b->isOverpopulated() && b->populationLimit > b->getPopulationCount())// && b->populationLimit
            {
                //sprite will move to a more expensive or cheaper house if it means he doesn't have to squeeze.
                if (sprite->getPossessions()->isSqueezing == true)
                {
                    lua_pushboolean(L, true);
                    return 1;
                }
                else if( sprite->getPossessions()->hasHouse == false)
                {
                    lua_pushboolean(L, true);
                    return 1;
                    
                }
                else if (b->buildingBuyPrice > sprite->getPossessions()->homeLocation->buildingBuyPrice)
                {
                        lua_pushboolean(L, true);
                        return 1;
                }
                
                
                
            }
            
        }
        
        lua_pushboolean(L, false);
    }

    return 1;
}
*/

//No longer automatically looks for job.

/*
static int IsBetterJobAvailable_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        if (sprite->getPossessions()->hasJob == false)
        {
            lua_pushboolean(L, true);
            return 1;
            //anything is better than no job.
        }
        
        //I'll not find a better job if I'm satisfied with my current one.
        //also, "all" counts as general purpose jobs, but they are treated as if they were farmer jobs.
        if (sprite->getPossessions()->jobClass.compare("all") == 0 &&
            (sprite->spriteClass.compare("farmer") == 0 || sprite->spriteClass.compare("citizen")))
        {
              lua_pushboolean(L, false);
              return 1;
              
        }
        
        //my job completely matches my class. 
        if (sprite->getPossessions()->jobClass.compare(sprite->spriteClass) == 0)
        {
            lua_pushboolean(L, false);
            return 1;
        }
        
        
        BuildingHandler *bh = GameScene::getThis()->buildingHandler; //no buildings = of course no job
        if (bh == NULL)
        {
            lua_pushboolean(L, false);
            return 1;
        }
        
        Job* spriteJob = (Job*) sprite->getPossessions()->jobLocation->getJobsAvailable()->objectAtIndex(sprite->getPossessions()->jobIndex);
        if (bh->amenityOnMap->count() > 0)
        {
        
            for (int i = 0; i < bh->amenityOnMap->count(); ++i)
            {
                Building * b = (Building*)bh->amenityOnMap->objectAtIndex(i);
                
                for (int j = 0; j < b->getJobsAvailable()->count(); ++j)
                {
                    Job * job = (Job*)b->getJobsAvailable()->objectAtIndex(j);
                    if (job->job_isTaken == false)
                    {
                        if (job->isJobBetter(spriteJob->job_targetEducationLevel, spriteJob->job_targetSpriteType_rank ))
                        {
                        
                            if (job->isCandidateQualified(sprite->getPossessions()->educationLevel, sprite->spriteClass))
                            {
                                lua_pushboolean(L, true);
                                return 1;
                            }
                        }
                    }
                
                }
            
            }
        }
        
        if (bh->commerceOnMap->count() > 0)
        {
            
            for (int i = 0; i < bh->commerceOnMap->count(); ++i)
            {
                Building * b = (Building*)bh->commerceOnMap->objectAtIndex(i);
                
                for (int j = 0; j < b->getJobsAvailable()->count(); ++j)
                {
                    Job * job = (Job*)b->getJobsAvailable()->objectAtIndex(j);
                    if (job->job_isTaken == false)
                    {
                        if (job->isJobBetter(spriteJob->job_targetEducationLevel, spriteJob->job_targetSpriteType_rank ))
                        {
                        
                            if (job->isCandidateQualified(sprite->getPossessions()->educationLevel, sprite->spriteClass) == 0)
                            {
                                lua_pushboolean(L, true);
                                return 1;
                            }
                        }
                    }
                
                }
            }
            
        }
        
        if (bh->educationOnMap->count() > 0)
        {
            for (int i = 0; i < bh->educationOnMap->count(); ++i)
            {
                Building * b = (Building*)bh->educationOnMap->objectAtIndex(i);
            
                for (int j = 0; j < b->getJobsAvailable()->count(); ++j)
                {
                    Job * job = (Job*)b->getJobsAvailable()->objectAtIndex(j);
                    if (job->job_isTaken == false)
                    {
                        if (job->isJobBetter(spriteJob->job_targetEducationLevel, spriteJob->job_targetSpriteType_rank ))
                        {
                            if (job->isCandidateQualified(sprite->getPossessions()->educationLevel, sprite->spriteClass))
                            {
                                lua_pushboolean(L, true);
                                return 1;
                            }
                        }
                    }
                
                }
            
            }
        }
        
        if (bh->socialOnMap->count() > 0)
        {
        
            for (int i = 0; i < bh->socialOnMap->count(); ++i)
            {
                Building * b = (Building*)bh->socialOnMap->objectAtIndex(i);
            
                for (int j = 0; j < b->getJobsAvailable()->count(); ++j)
                {
                    Job * job = (Job*)b->getJobsAvailable()->objectAtIndex(j);
                    if (job->job_isTaken == false)
                    {
                        if (job->isJobBetter(spriteJob->job_targetEducationLevel, spriteJob->job_targetSpriteType_rank ))
                        {

                            if (job->isCandidateQualified(sprite->getPossessions()->educationLevel, sprite->spriteClass))
                            {
                            
                                lua_pushboolean(L, true);
                                return 1;
                            }
                        }
                    }
                
                }
            }
        }
        
        if (bh->militaryOnMap->count() > 0)
        {
            for (int i = 0; i < bh->militaryOnMap->count(); ++i)
            {
                Building * b = (Building*)bh->militaryOnMap->objectAtIndex(i);
            
                for (int j = 0; j < b->getJobsAvailable()->count(); ++j)
                {
                    Job * job = (Job*)b->getJobsAvailable()->objectAtIndex(j);
                    if (job->job_isTaken == false)
                    {
                        if (job->isJobBetter(spriteJob->job_targetEducationLevel, spriteJob->job_targetSpriteType_rank ))
                        {
 
                            if (job->isCandidateQualified(sprite->getPossessions()->educationLevel, sprite->spriteClass))
                            {
                            
                                lua_pushboolean(L, true);
                                return 1;
                            }
                        }
                    }
                
                }
            }
        
        }
        lua_pushboolean(L, false);

        
    }
    return 1;
}
*/
 
/*Asks if the current sprite set in the global if it already has a house*/
static int HasHouse_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        lua_pushboolean(L, sprite->getPossessions()->hasHouse);
    }
    
    return 1;
}

/*Buys a house. ID to be supplied from the LUA side.*/
static int BuyHouseWithID_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        if (sprite->getPossessions()->homeLocation != NULL)
        {
            lua_pushboolean(L, false); //already lives somewhere;
            
            return 1;
        }
        //  CCLog(sprite->spriteName->getCString());
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        int targetID = lua_tonumber(L, 1);
        
        if (targetID < 0)
        {
            lua_pushboolean(L, false); //invalidID;
            return 1;
        }
        
        lua_pushboolean(L, sprite->BuyHouse(targetID));
    
    }
    return 1;
}

/*
static int RentHouseWithID_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        if (sprite->getPossessions()->homeLocation != NULL)
        {
            lua_pushboolean(L, false); //already lives somewhere;
            
            return 1;
        }
        //  CCLog(sprite->spriteName->getCString());
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        int targetID = lua_tonumber(L, 1);
        
        if (targetID < 0)
        {
            lua_pushboolean(L, false); //invalidID;
            return 1;
        }
        
        lua_pushboolean(L, sprite->RentHouse(targetID));
        
    }
    return 1;
}
*/

/*
static int BuySqueezeHouse_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        if (sprite->getPossessions()->homeLocation != NULL)
        {
            lua_pushboolean(L, false); //already lives somewhere;
            
            return 1;
        }
        //  CCLog(sprite->spriteName->getCString());
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        int targetID = lua_tonumber(L, 1);
        
        if (targetID < 0)
        {
            lua_pushboolean(L, false); //invalidID;
            return 1;
        }
        
        lua_pushboolean(L, sprite->BuySqueezeHouse(targetID));
        
    }
    return 1;

}
*/
/*gets a list of IDs that belong to the buildingType HOUSING, and sends it to Lua*/
static int GetAllHomes_Glue(lua_State *L)
{
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    int index = 0;
    lua_newtable(L);
    for (int i = 0; i < bh->housingOnMap->count(); ++i)
    {
        ++index;
        lua_pushnumber(L, index);
        lua_pushnumber(L, ((Building*)(bh->housingOnMap->objectAtIndex(i)))->ID);
        lua_rawset(L, -3);
    }
    lua_setglobal(L, "homes");
    
    return 0;
}

/*gets a list of IDs that do not belong to the buildingType HOUSING, and sends it to Lua*/
/*here I assume everythign that isn't a HOUSING is open for employment.*/
static int GetAllJobs_Glue(lua_State *L)
{
    /*gets an array of all possible job locations on the map. These are generally locations that aren't residences.*/
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    lua_newtable(L);
    int index = 0;
    for (int i = 0; i < bh->allBuildingsOnMap->count(); ++i)
    {
        Building* b = (Building*) bh->allBuildingsOnMap->objectAtIndex(i);
        if (b->buildingType != HOUSING)
        {
            ++index;
            lua_pushnumber(L, index);
            CCLog("%d",b->ID);
            lua_pushnumber(L, b->ID);
            lua_rawset(L, -3);
        }
    }
    
    lua_setglobal(L, "job_locations");
    return 0;
}

/*note: this function doesn't care what class the character is at the moment. */
static int SetClassLevel_Glue(lua_State *L)
{
    if (sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        int args = lua_gettop(L);
        if (args == 0 || args > 1)
        {
            lua_pushboolean(L, false);
            return 1;
        }
        
        int targetLevel = lua_tonumber(L,1);
        
        sprite->getPossessions()->setClassLevel(targetLevel);
        lua_pushboolean(L, true);
        
    }
    
    return 1;
}

static int SetEducationLevel_Glue(lua_State*L)
{
    if (sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        int args = lua_gettop(L);
        if (args == 0 || args > 1)
        {
            lua_pushboolean(L, false);
            return 1;
        }
        
        int targetLevel = lua_tonumber(L,1);
        
        sprite->getPossessions()->SetEducationLevel(targetLevel);
        lua_pushboolean(L, true);
        
    }
    
    return 1;
    
}

/*
Assigns a job to the current sprite occupied by the pointer. Pass the building ID from Lua.
static int AssignJobWithID_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        if (sprite->getPossessions()->jobLocation != NULL)
        {
            lua_pushboolean(L, false);  
            return 1; //already have a job. Don't check.
        }
        
        int args = lua_gettop(L);
        if (args == 0 || args > 1)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
     
        int targetID = lua_tonumber(L, 1);
        
        if (targetID < 0)
        {
            lua_pushboolean(L, false); //invalidID;
            return 1;
        }
       
        
        //Note: find job at buildingID is automated. It will find the first available job for the sprite's qualification in that building. The sprite takes the job there and then.
        //If this is false, the sprite didn't manage to find a job in that building.
        lua_pushboolean(L, sprite->FindJobAt(targetID));
        
    }
    return 1;
}*/

/*
static int AssignLowerJobWithID_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        if (sprite->getPossessions()->jobLocation != NULL)
        {
            CCLog("Got job already");
            lua_pushboolean(L, false);
            return 1; //already have a job. Don't check.
        }
        
        int args = lua_gettop(L);
        if (args == 0 || args > 1)
        {
            CCLog("Invalid ID");
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        
        int targetID = lua_tonumber(L, 1);
        
        if (targetID < 0)
        {
            CCLog("Invalid targetID");
            lua_pushboolean(L, false); //invalidID;
            return 1;
        }
        
        
        //Note: find job at buildingID is automated. It will find the first available job for the sprite's qualification in that building. The sprite takes the job there and then.
        //If this is false, the sprite didn't manage to find a job in that building.
        lua_pushboolean(L, sprite->FindLowerLevelJobAt(targetID));
        
    }
    return 1;
}
*/

/*causes the sprite to quit its current job.*/
/*
static int QuitJob_Glue(lua_State *L)
{
    if (!sprite)
        lua_pushboolean(L, false); //no sprite
    
   
    lua_pushboolean(L, sprite->QuitJob());
  
    
    return 1;
}*/

/*causes the sprite to sell its house.*/
static int SellHouse_Glue(lua_State* L)
{
    if (!sprite)
        lua_pushboolean(L, false); //no sprite
    
    
    lua_pushboolean(L, sprite->SellHouse());
    
    
    return 1;
    
}

/*checks if a building is fully populated. This can be used to check for job vacacies as well as residence vacancies.*/
static int IsBuildingFullyOccupied_Glue(lua_State *L)
{
    int args = lua_gettop(L);
    if (args == 0)
    {
        lua_pushboolean(L, false); //invalid ID;
        return 1;
    }
    int targetID = lua_tonumber(L, 1);

    if (targetID < 0)
    {
        lua_pushboolean(L, false); //invalidID;
        return 1;
    }
    
    
    Building* b = (Building*)GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(targetID);
    lua_pushboolean(L, b->getPopulationCount() >= b->populationLimit);
    
    return 1;
}


/*calls the current sprite set in the global to path to work.*/
static int GoToWork_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L,false);
    }
    else
    {
        lua_pushboolean(L, sprite->PathToWork());
    }
    return 1;
}

/*checks if the current sprite set in the global has a job.*/
/*
static int HasWork_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L,false);
    }
    else
    {
        lua_pushboolean(L, sprite->getPossessions()->hasJob);
    }
    return 1;
}

static int IsJobSuitable_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
        
    }
    else
    {
        lua_pushboolean(L, (sprite->getPossessions()->hasJob && ((sprite->getPossessions()->jobClass.compare(sprite->spriteClass) == 0)
                                                                 ||
                                                                 (sprite->getPossessions()->jobClass.compare("all") == 0 && (sprite->spriteClass.compare("farmer") == 0 || sprite->spriteClass.compare("citizen") == 0)))));

    }
    return 1;
}

static int IsHomeSuitable_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        lua_pushboolean(L, (!sprite->getPossessions()->homeLocation->isOverpopulated()));
    }
    return 1;
}*/

/*Makes the sprite go to a building. ID will be supplied.*/
static int GoToBuilding_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    
    }
    else
    {
        int args = lua_gettop(L);        
        if (args == 0 || args > 1)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        
        int targetID = lua_tonumber(L, 1);
        if (targetID < 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;

        }

        
        lua_pushboolean(L, sprite->PathToBuilding(targetID));
    }
    return 1;
}
 

/*makes the global sprite follow a path*/
static int FollowPath_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L,false);
        
    }
    else
    {
        if (sprite->path->count() > 0)
        {
            sprite->followPath();
            lua_pushboolean(L, true);
        }
        else
        {
            lua_pushboolean(L, false);
        }
    }
    return 1;
}


/*makes the global sprite stop moving upon reaching the next square*/
static int StopMoving_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushboolean(L,false);
        
    }
    else
    {
        sprite->StopMoving();
        lua_pushboolean(L, true);
    }
    return 1;
}

/*gets the cost of a building. ID supplied from the LUA side.*/
static int GetBuildingCost_Glue(lua_State *L)
{
    int args = lua_gettop(L);
    if (args == 0)
    {
        lua_pushinteger(L, -1); //invalid ID;
        return 1;
    }

    /*Note: I checked the array by breakpointing, and apparently the very first building added to the map has an ID of 1 due to how I did it.
     therefore the object we want is always at index -1. */
    int targetID = lua_tonumber(L, 1);
    if (targetID < 0)
    {
        lua_pushinteger(L, -1); //invalidID;
        return 1;
    }
    
    Building *targetBuilding = (Building*) (GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(targetID));
  //  CCLog("target building name is %s", targetBuilding->buildingName.c_str());
  //  CCLog("target building cost is %i", targetBuilding->buildingBuyPrice);

    if (targetBuilding == NULL)
    {
        lua_pushinteger(L, -1); //invalidID;
    }
    else
    {
        lua_pushinteger(L, targetBuilding->buildingBuyPrice);
    }
    
    return 1;
}


/*gets the rent of the building by selected buildingID*/
/*
static int GetBuildingRent_Glue(lua_State* L)
{
    int args = lua_gettop(L);
    if (args == 0)
    {
        lua_pushinteger(L, -1); //invalid ID;
        return 1;
    }
    

    int targetID = lua_tonumber(L, 1);
    if (targetID < 0)
    {
        lua_pushinteger(L, -1); //invalidID;
        return 1;
    }
 
    
    Building *targetBuilding = (Building*) (GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(targetID));
   // CCLog("target building name is %s", targetBuilding->buildingName->getCString());
   // CCLog("target building cost is %i", targetBuilding->buildingRentPrice);
    
    if (targetBuilding == NULL)
    {
        lua_pushinteger(L, -1); //invalidID;
    }
    else
    {
        lua_pushinteger(L, targetBuilding->buildingRentPrice);
    }
    
    return 1;
}*/


/*gets the current sprite's cash on hand.*/
static int GetCashOnHand_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushinteger(L, -1); //invalid sprite
    }
    else
    {
        lua_pushinteger(L, sprite->getPossessions()->cashOnHand);
    }
    return 1;
}

static int GetLevel_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushinteger(L, -1); //invalid sprite

    }
    else
    {
        lua_pushinteger(L, sprite->getLevel());
    }
    return 1;
}


static int GetEnergy_Glue(lua_State* L)
{
    if (!sprite)
    {
        lua_pushinteger(L, -1);//invalid sprite
    }
    else
    {
        lua_pushinteger(L, sprite->getPossessions()->energyRating);
    }
    return 1;
}


static int IsIdle_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
        lua_pushboolean(L, sprite->getAction() == IDLE);
    }
    return 1;
}

/*forces the sprite to pick a random destination. Range ignored! Valid destination ignored!*/
static int PickRandomDestination_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
       // CCLog(sprite->spriteName->getCString());
        lua_pushboolean(L, sprite->Wander());
    }
    return 1;
}


static int IsDestinationInRange_Glue(lua_State *L)
{
    if (!sprite)
        lua_pushboolean(L, false);
    else
    {
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        else
        {
            
            int targetID = lua_tonumber(L, 1);
            if (targetID < 0)
            {
                lua_pushboolean(L, false); //invalidID;
                return 1;
            }
                
            lua_pushboolean(L, sprite->isDestinationInRange(targetID));
        
        }

    }
    
    return 1;
    
}

static int GetBuildingLevel_Glue(lua_State *L)
{
    int args = (lua_gettop(L));
    if (args == 0 || args > 1)
    {
        lua_pushinteger(L, -1);
        return 1;
    }
    
    int targetID = lua_tonumber(L, 1);
    
    if (targetID < 0)
    {
        lua_pushinteger(L, -1); //invalidID;
        return 1;
    }
    
    
    Building *targetBuilding = (Building*) (GameScene::getThis()->buildingHandler->getBuildingOnMapWithID(targetID));
    
    lua_pushinteger(L, targetBuilding->getLevel() );
    
    /*gets the current level of the building. Assume buildingID is supplied.*/
    return 1;
}

static int GetHappiness_Glue(lua_State * L)
{
    if (!sprite)
        lua_pushinteger(L, -1);
    else
        lua_pushinteger(L, sprite->getPossessions()->happinessRating);
    
        
    return 1;
}

static int GetLoyalty_Glue(lua_State *L)
{
    if (!sprite)
        lua_pushinteger(L, -1);
    else
        lua_pushinteger(L, sprite->getPossessions()->loyaltyRating);
    
    
    return 1;
    
}


static int GetSocial_Glue(lua_State *L)
{
    if (!sprite)
        lua_pushinteger(L, -1);
    else
        lua_pushinteger(L, sprite->getPossessions()->socialRating);
    
    
    return 1;
    
}

static int GetIntelligence_Glue(lua_State*L)
{
    if (!sprite)
        lua_pushinteger(L, -1);
    else
        lua_pushinteger(L, sprite->getPossessions()->intelligenceRating);
    
    
    return 1;
    
}

static int ModifyHappiness_Glue(lua_State* L)
{
    
    if (!sprite)
        lua_pushboolean(L, false);
    else
    {
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        else
        {
            
            int val = lua_tonumber(L, 1);
            sprite->getPossessions()->happinessRating += val;
            lua_pushboolean(L, true);
            
        }
        
    }

    
    return 1;
}

static int ModifyIntelligence_Glue(lua_State* L)
{
    if (!sprite)
        lua_pushboolean(L, false);
    else
    {
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        else
        {
            
            int val = lua_tonumber(L, 1);
            sprite->getPossessions()->intelligenceRating += val;
            lua_pushboolean(L, true);
            
        }
        
    }
    
    
    return 1;

}

static int ModifyLoyalty_Glue(lua_State* L)
{
    
    if (!sprite)
        lua_pushboolean(L, false);
    else
    {
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        else
        {
            
            int val = lua_tonumber(L, 1);
            sprite->getPossessions()->loyaltyRating += val;
            lua_pushboolean(L, true);
            
        }
        
    }
    
    
    return 1;
}


static int ModifySocial_Glue(lua_State* L)
{
    
    if (!sprite)
        lua_pushboolean(L, false);
    else
    {
        int args = lua_gettop(L);
        if (args == 0)
        {
            lua_pushboolean(L, false); //invalid ID;
            return 1;
        }
        else
        {
            
            int val = lua_tonumber(L, 1);
            sprite->getPossessions()->socialRating += val;
            lua_pushboolean(L, true);
            
        }
        
    }
    
    
    return 1;
}

static int GetAllCommerce_Glue(lua_State *L)
{
     BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    lua_newtable(L);
    for (int i = 0; i < bh->commerceOnMap->count(); ++i)
    {
        lua_pushnumber(L, i);
        lua_pushnumber(L, ((Building*)(bh->commerceOnMap->objectAtIndex(i)))->ID);
        lua_rawset(L, -3);
    }
    lua_setglobal(L, "returned_commerce");
    return 1;
}


/*gets all buildings of the type stated in the lua file.*/
static int GetBuildingsOfType_Glue(lua_State *L)
{
    int args = lua_gettop(L);
    
    
    if (args == 0 || args > 1)
    {
        lua_pushinteger(L, -1); //invalid entry;
        return 1;
    }
    /*gets an array of all possible job locations on the map. These are generally locations that aren't residences.*/
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    std::string btype = lua_tostring(L, -1);
    if (btype.compare("housing") == 0)
    {
        lua_newtable(L);
        for (int i = 0; i < bh->housingOnMap->count(); ++i)
        {
            lua_pushnumber(L, i+1);
            lua_pushnumber(L, ((Building*)(bh->housingOnMap->objectAtIndex(i)))->ID);
            lua_rawset(L, -3);
        }
        lua_setglobal(L, "returned_buildings");
    
    
    }
    else if (btype.compare("amenity") == 0)
    {
        lua_newtable(L);
        for (int i = 0; i < bh->amenityOnMap->count(); ++i)
        {
            lua_pushnumber(L, i+1);
            lua_pushnumber(L, ((Building*)(bh->amenityOnMap->objectAtIndex(i)))->ID);
            lua_rawset(L, -3);
        }
        lua_setglobal(L, "returned_buildings");
    }
    else if (btype.compare("commerce")==0)
    {
        lua_newtable(L);
        for (int i = 0; i < bh->commerceOnMap->count(); ++i)
        {
            lua_pushnumber(L, i+1);
            lua_pushnumber(L, ((Building*)(bh->commerceOnMap->objectAtIndex(i)))->ID);
            lua_rawset(L, -3);
        }
        lua_setglobal(L, "returned_buildings");
    }
    else if (btype.compare("military")==0)
    {
        lua_newtable(L);
        for (int i = 0; i < bh->militaryOnMap->count(); ++i)
        {
            lua_pushnumber(L, i+1);
            lua_pushnumber(L, ((Building*)(bh->militaryOnMap->objectAtIndex(i)))->ID);
            lua_rawset(L, -3);
        }
        lua_setglobal(L, "returned_buildings");
    
    
    }
    else if (btype.compare("education")==0)
    {
        lua_newtable(L);
        for (int i = 0; i < bh->educationOnMap->count(); ++i)
        {
            lua_pushnumber(L, i+1);
            lua_pushnumber(L, ((Building*)(bh->educationOnMap->objectAtIndex(i)))->ID);
            lua_rawset(L, -3);
        }
        lua_setglobal(L, "returned_buildings");
    
    }
        
    else if (btype.compare("social")==0)
    {
        lua_newtable(L);
        for (int i = 0; i < bh->socialOnMap->count(); ++i)
        {
            lua_pushnumber(L, i+1);
            lua_pushnumber(L, ((Building*)(bh->socialOnMap->objectAtIndex(i)))->ID);
            lua_rawset(L, -3);
        }
        lua_setglobal(L, "returned_buildings");
    
    }
    else
    {
        CCLog ("Invalid building type! (Small caps pls, or check spelling)");
    }
        
    return 0;
}






/*behavior functions*/
Behavior::Behavior()
{

    L = luaL_newstate();
    
}

Behavior::~Behavior()
{
    if (sp != NULL)
    {
      //  sp->release(); //NOT HERE
        
        
        
        sp = NULL;
    }
    lua_close(L);

}

Behavior::Behavior(Status (*food)(std::string))
{
    foo = food;
    
}

void Behavior::BehaviorInit()
{
    luaL_openlibs(L);
    
    registerFunctions(L);
    
    path = CCFileUtils::sharedFileUtils()->fullPathForFilename(file_name.c_str());
    
    int result = luaL_dofile(L, path.c_str());
    if (result != 0)
    {
        CCLog("%s", lua_tostring(L, -1));
        CCLog("something went wrong.");
    }
    

}

void Behavior::setSprite(GameSprite *ptrSprite)
{
    sp = ptrSprite;
   // sp->retain();
}

void Behavior::lua_pushvariables(const char* func_name)
{
        //not sure if I wanna put things in here. We'll see.
}


int  Behavior::lua_processfunc(lua_State* L)
{
    int retVal = 0;
  //  CCLog(file_name.c_str());
  //  CCLog(func_name.c_str());
       
    //set the correct GameSprite pointer for LuaBindFunctions to operate on.
    sprite = sp;
    lua_getglobal(L, func_name.c_str());
    //lua_pushvariables(func_name.c_str());
    
    lua_call(L, 0, 1);
   // CCLog("Lua called");
    retVal = (int)lua_tointeger(L, -1);
    lua_pop(L,1);
    return retVal;
}

Status Behavior::update()
{
  return BH_SUCCESS;

}

void Behavior::onTerminate(Status)
{
    
    int result = lua_processfunc(L);
    
    if (result == 0)
        retStatus = BH_SUCCESS;
    else
        retStatus = BH_FAILURE;
    
   
    m_eStatus = retStatus;
}

Status Behavior::tick()
{
   if (m_eStatus == BH_INVALID)
   {
       onInitialize();
   }
   m_eStatus = update();
   
   if (m_eStatus != BH_RUNNING)
   {
       onTerminate(m_eStatus);
   }
   return m_eStatus;
}

void Behavior::onInitialize()
{
    
}

void Behavior::registerFunctions(lua_State* L)
{
   /* functions are located in LuaBindFunctions class. */
    lua_register(L, "GoHome", GoHome_Glue);
    lua_register(L, "HasHouse", HasHouse_Glue);
    lua_register(L, "GoToWork", GoToWork_Glue);
   // lua_register(L, "HasWork", HasWork_Glue);
    lua_register(L, "GetLevel", GetLevel_Glue);
    lua_register(L, "GoToBuildingWithID", GoToBuilding_Glue);
    
    /*Exposed sprite properties*/
    lua_register(L, "GetCashOnHand", GetCashOnHand_Glue);
    lua_register(L, "GetEnergy", GetEnergy_Glue);
    
    /*makes the person start walking*/
    lua_register(L, "FollowPath", FollowPath_Glue);
    
    lua_register(L, "SetClassLevel", SetClassLevel_Glue);
    lua_register(L, "SetEducationLevel", SetEducationLevel_Glue);
    
    lua_register(L, "IsDestinationInRange", IsDestinationInRange_Glue);
    lua_register(L, "IsIdle", IsIdle_Glue);
    lua_register(L, "StopMoving", StopMoving_Glue);
    lua_register(L, "Wander", PickRandomDestination_Glue);
    
    /*buying or renting house*/
    lua_register(L, "GetCostOfBuildingWithID", GetBuildingCost_Glue);
 //   lua_register(L, "GetRentOfBuildingWithID", GetBuildingRent_Glue);
    lua_register(L, "GetBuildingLevelWithID", GetBuildingLevel_Glue);
    lua_register(L, "IsBuildingWithIDFull", IsBuildingFullyOccupied_Glue);
    lua_register(L, "GetAllHomeIDs", GetAllHomes_Glue);
    lua_register(L, "BuyHouseWithID", BuyHouseWithID_Glue);
  //  lua_register(L, "RentHouseWithID", RentHouseWithID_Glue);
    
   // lua_register(L, "BuySqueezeHouseWithID", BuySqueezeHouse_Glue);
    
    //makes the sprite sell his house. 
    lua_register(L, "SellHouse", SellHouse_Glue);
    
    /*finding jobs*/
    lua_register(L, "GetAllJobIDs", GetAllJobs_Glue);
    lua_register(L, "GetAllCommerceIDs", GetAllCommerce_Glue);
    
    //AssignJobWithID incorporates a vacancy and qualification check.
   // lua_register(L, "AssignJobWithID", AssignJobWithID_Glue);
  //  lua_register(L, "AssignLowerJobWithID", AssignLowerJobWithID_Glue);
    //Makes the sprite quit his job
   // lua_register(L, "IsJobSuitable", IsJobSuitable_Glue);
  //  lua_register(L, "IsHomeSuitable", IsHomeSuitable_Glue);
  //  lua_register(L, "QuitJob", QuitJob_Glue);
    
    lua_register(L, "ModifyHappiness", ModifyHappiness_Glue);
    lua_register(L, "ModifySocial", ModifySocial_Glue);
    lua_register(L, "ModifyLoyalty", ModifyLoyalty_Glue);
    lua_register(L, "ModifyIntelligence", ModifyIntelligence_Glue);
    
    lua_register(L, "GetIntelligence", GetIntelligence_Glue);
    lua_register(L, "GetHappiness", GetHappiness_Glue);
    lua_register(L, "GetSocial", GetSocial_Glue);
    lua_register(L, "GetLoyalty", GetLoyalty_Glue);
    
  //  lua_register(L, "BecomeLocal", ConvertToLocal_Glue);
    
    /*general building type retriever*/
    lua_register(L,"GetBuildingsOfType", GetBuildingsOfType_Glue);
    
    
  //  lua_register(L, "IsBetterHouseAvailable", IsBetterHouseAvailable_Glue);
  //  lua_register(L, "IsBetterJobAvailable", IsBetterJobAvailable_Glue);
    
    /*Checks for upgrade requirements.*/
    lua_register(L, "IsAtMaxClassLevel", IsAtMaxLevel_Glue);
    lua_register(L, "IsAtAbsoluteMaxlevel", IsAtAbsoluteMaxLevel_Glue);
  //  lua_register(L, "HasMetUpgradeCriteria", HasMetUpgradeCriteria_Glue);
  //  lua_register(L, "TriggerUpgrade", UpgradeClass_Glue);
    
  //  lua_register(L, "GetSocialInteractionChance", GetRatioSocialInteraction_Glue);
  //  lua_register(L, "TriggerSocialInteraction", TriggerSocialInteraction_Glue);
  //  CCLog("Functions registered");
}




