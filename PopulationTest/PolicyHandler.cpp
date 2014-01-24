//
//  PolicyHandler.cpp
//  PopulationTest
//
//  Created by Serious Games on 8/5/13.
//
//

#include "PolicyHandler.h"
#include "GameTimer.h"
#include "GameScene.h"
#include "GameManager.h"
#include "GameHUD.h"
#include "GameDefaults.h"
#include "Sprite.h"
#include "GlobalHelper.h"

PolicyHandler::PolicyHandler()
{
    monthly_localBirthRate = default_birth_rate;
    monthly_alienImportRate = 0;
    
    last_frame_month = this_frame_month = 0;
    last_frame_day = this_frame_day = 0;
    
    if (monthly_localBirthRate <= 0)
        next_local_birth_in = -1;
    else
        next_local_birth_in = int (default_birth_rate_days/monthly_localBirthRate);
    
    if (monthly_alienImportRate <= 0)
        next_alien_arrives_in = -1;
    else
        next_alien_arrives_in = int (default_birth_rate_days/monthly_alienImportRate);
    //I assume a month is 28 days, as in this game we assume 4 weeks a month and 7 days a week
    
    int gender = rand() % 2;
    int aliengender = rand() % 2;
    if (gender > 0)
    {
    //    nextMayanSpawn = M_MAYAN_FARMER;
    }
    else
      //  nextMayanSpawn = F_MAYAN_FARMER;
    
    if (aliengender > 0)
    {
       // nextAlienSpawn = M_ALIEN_CITIZEN;
    }
    else
    //    nextAlienSpawn = F_ALIEN_CITIZEN;
    
    pauseAutoSpawning = true;
    
    maxAlienPopulation = default_alien_population_allowed;
    
    percentTax = default_tax_rate_per_building;
    percentCulturalExchangeRate = default_cultural_exchange_rate_percent;
    
    policy_active_br = false;
    policy_active_ce = false;
    birthRatePolicy_months = culturalExchangePolicy_months = 0;

    isFirstMonth = true;
    isFirstYear = true;
}

bool PolicyHandler::isImplementingBirthRatePolicy()
{
    return policy_active_br;
}

bool PolicyHandler::isImplementingCulturalExchangePolicy()
{
    return policy_active_ce;
}

void PolicyHandler::setBirthRatePolicy(int deltaBirthRate, int duration)
{
    monthly_localBirthRate += deltaBirthRate;
    birthRatePolicy_months = duration;
    policy_active_br = true;
    
    GameHUD::getThis()->showHint("Your birth rate policy is in effect. \n It'll start from the next citizen born onwards.");
}


int PolicyHandler::getBirthRateDurationLeft()
{
    return birthRatePolicy_months;
    
}

int PolicyHandler::getCulturalExchangeDurationLeft()
{
    return culturalExchangePolicy_months;
}

void PolicyHandler::BirthNewAlien()
{
    SpriteHandler* sh = GameScene::getThis()->spriteHandler;
    if (sh)
    {
        CCPoint target;
        
        BuildingHandler* bh = GameScene::getThis()->buildingHandler;
        
        if (bh->specialOnMap->count() > 0)
        {
            target = ((Building*)bh->specialOnMap->objectAtIndex(0))->getWorldPosition();
            target = GameScene::getThis()->mapHandler->tilePosFromLocation(target);
        }
        else
        {
        
            MapHandler* mh = GameScene::getThis()->mapHandler;
            target = mh->getRandomPathTile();
        }
        
        
        sh->addSpriteToMap(target, nextAlienSpawn);
        
        int aliengender = rand() % 2;
        
        if (aliengender > 0)
        {
          //  nextAlienSpawn = M_ALIEN_CITIZEN;
        }
        else {}
            //nextAlienSpawn = F_ALIEN_CITIZEN;
    }
    GameManager::getThis()->UpdateUnlocks();
}

void PolicyHandler::BirthNewLocal()
{
    
    SpriteHandler* sh = GameScene::getThis()->spriteHandler;
    if (sh)
    {
        CCPoint target;
        
        BuildingHandler* bh = GameScene::getThis()->buildingHandler;
        
        if (bh->specialOnMap->count() > 0)
        {
            target = ((Building*)bh->specialOnMap->objectAtIndex(0))->getWorldPosition();
            target = GameScene::getThis()->mapHandler->tilePosFromLocation(target);
        }
        else
        {
            
            MapHandler* mh = GameScene::getThis()->mapHandler;
            target = mh->getRandomPathTile();
        }
        
        sh->addSpriteToMap(target, nextMayanSpawn);
        
        
        int gender = rand() % 2;
        if (gender > 0)
        {
           // nextMayanSpawn = M_MAYAN_FARMER;
        }
        else {}
           // nextMayanSpawn = F_MAYAN_FARMER;
    }
    GameManager::getThis()->UpdateUnlocks();
   // ImportForeignersInBuilding((Building*)GameScene::getThis()->buildingHandler->allBuildingsOnMap->objectAtIndex(0), 3);
    
}


void PolicyHandler::SpawnImmediately(SpriteType sp)
{
    SpriteHandler* sh = GameScene::getThis()->spriteHandler;
    CCPoint target;
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    if (bh->specialOnMap->count() > 0)
    {
        target = ((Building*)bh->specialOnMap->objectAtIndex(0))->getWorldPosition();
        target = GameScene::getThis()->mapHandler->tilePosFromLocation(target);
    }
    else
    {
        
        MapHandler* mh = GameScene::getThis()->mapHandler;
        target = mh->getRandomPathTile();
    }
    
    sh->addSpriteToMap(target, sp);
    GameManager::getThis()->UpdateUnlocks();
}

void PolicyHandler::ImportForeignersInBuilding(Building *b, int amt)
{
    if (amt > 0)
    {
        CCPoint position = b->getWorldPosition();
        position = GameScene::getThis()->mapHandler->tilePosFromLocation(position);
        
       
        for (int i = 0; i < amt; ++i)
        {
            int gender = rand() % 2;
            if (gender > 0) {}
                 //GameScene::getThis()->spriteHandler->addSpriteToMap(position, M_ALIEN_CITIZEN);
            else {}
                 //GameScene::getThis()->spriteHandler->addSpriteToMap(position, F_ALIEN_CITIZEN);
          
        }
        
    }
    
    
}


void PolicyHandler::CollectTaxes()
{
    GameManager* gm = GameManager::getThis();
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    for (int i = 0; i < bh->allBuildingsOnMap->count(); ++i)
    {
        //taxes are currently recorded as buildingCost.
        Building* b = (Building*)bh->allBuildingsOnMap->objectAtIndex(i);
        
        gm->currentMoney += ((percentTax * 0.01) * b->buildingCost);
    }
    
    
    GameHUD::getThis()->updateMoneyLabel();
    
}

void PolicyHandler::YearlyMaintenance()
{
    GameManager* gm = GameManager::getThis();
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    
    for (int i = 0; i < bh->allBuildingsOnMap->count(); ++i)
    {
        //taxes are currently recorded as maintCost;
        Building* b = (Building*)bh->allBuildingsOnMap->objectAtIndex(i);
        
        gm->currentMoney -= b->maintCost;
        
    }
    
    
    GameHUD::getThis()->updateMoneyLabel();

}

void PolicyHandler::MonthlyMaintenance()
{

    
    GameManager* gm = GameManager::getThis();
    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    bool hasUnoccupied = false;
    
    int totalMaint = 0;
    
    
    for (int i = 0; i < bh->allBuildingsOnMap->count(); ++i)
    {
        //taxes are currently recorded as buildingCost.
        Building* b = (Building*)bh->allBuildingsOnMap->objectAtIndex(i);
        
        int unoccCount = b->getUnoccupiedCount();
        int cost = 0;
        if (unoccCount > 0)
        {
            hasUnoccupied = true;
            if (b->buildingType == HOUSING)
            {
                cost = unoccCount * maint_per_unoccupied_residence;
            }
            else
            {
                cost = unoccCount * maint_per_unoccupied_job;
            }
            
            totalMaint += cost;
            
        }
    }
   
    if (hasUnoccupied)
    {
        GameHUD::getThis()->showHint("You have buildings that aren't filled to capacity.\nThe empty space costs you "+ GlobalHelper::Convert(totalMaint)  +" G" );
        gm->currentMoney -= totalMaint;

        GameHUD::getThis()->updateMoneyLabel();

    }
    
}


void PolicyHandler::UpdateFrames()
{
   last_frame_day = this_frame_day;
  //  last_frame_week = this_frame_week;
    last_frame_month = this_frame_month;
    
    this_frame_day = 0;
 //   this_frame_week = GameTimer::getThis()->getCurrentWeek();
    this_frame_month = GameTimer::getThis()->getCurrentMonth();
    
    
    if (this_frame_day != last_frame_day)
    {
        next_alien_arrives_in -= 1;
        next_local_birth_in -= 1;
        
        SpriteHandler* sh = GameScene::getThis()->spriteHandler;
        for (int i = 0; i < sh->aliensOnMap->count(); ++i)
        {
            ((GameSprite*)sh->aliensOnMap->objectAtIndex(i))->CallbackDayPassed();
        }
      //  CCLog("Next local birth in %d", next_local_birth_in);
    }
    
    if (this_frame_month != last_frame_month)
    {
        if (isImplementingCulturalExchangePolicy())
        {
            --culturalExchangePolicy_months;
            if (culturalExchangePolicy_months <= 0)
            {
                culturalExchangePolicy_months = 0;
                policy_active_ce = false;
                //set the percentage back to default
                percentCulturalExchangeRate = default_cultural_exchange_rate_percent;
                GameHUD::getThis()->showHint("Your cultural exchange policy is over.");
            }
            
        
        }
        
        if (isImplementingBirthRatePolicy())
        {
            --birthRatePolicy_months;
            if (birthRatePolicy_months <= 0)
            {
                birthRatePolicy_months = 0;
                policy_active_br = false;
                monthly_localBirthRate = default_birth_rate;
                
                GameHUD::getThis()->showHint("Your birth rate boosting policy is over.");
            }
        }
    }
    
    

}


void PolicyHandler::Update()
{
    UpdateFrames();
    if (last_frame_month != this_frame_month)
    {
        //stops the game from subtracting cash right off the bat.
        if (isFirstMonth)
        {
            CCLog("this should run only once!");
            isFirstMonth = false;
        }
        else
        {
        
            //monthly maintenance
            MonthlyMaintenance();
            //Beginning of march tax collection
        }
        
        
        
        if (this_frame_month == 3)
        {
            //stops yearly maint and taxing from being applied to the very first year
            if (isFirstYear)
            {
                isFirstYear = false;
            }
            else
            {
            
                CollectTaxes();
                YearlyMaintenance();
            }
        }
    }
    
    if (pauseAutoSpawning) return;
    
    if (next_local_birth_in <= 0)
    {
        
        if (next_local_birth_in > -1)
            BirthNewLocal();
        
        if (monthly_localBirthRate <= 0)
            next_local_birth_in = -1;
        else
            next_local_birth_in = int (28/monthly_localBirthRate);
        
        /*update unlocks on each local birth*/
        GameManager::getThis()->UpdateUnlocks();

    }
    
    if (next_alien_arrives_in <= 0)
    {
        if (next_alien_arrives_in > -1)
            BirthNewAlien();
        
        if (monthly_alienImportRate <= 0)
            next_alien_arrives_in = -1;
        else
            next_alien_arrives_in = int (28/monthly_alienImportRate); //I assume a month is 30 days, as in this game we assume 4 weeks a month and 7 days a week
        
        /*update unlocks in each alien birth*/
        GameManager::getThis()->UpdateUnlocks();

        
    }
    
}

void PolicyHandler::SetNextSpawn(SpriteType sp)
{
    //nextMayanSpawn = sp;
}

void PolicyHandler::SetNextAlienSpawn(SpriteType sp)
{
   // nextAlienSpawn = sp;
}

void PolicyHandler::pauseSpawn()
{
    //pauseAutoSpawning = true;
}

void PolicyHandler::startSpawn()
{
    //pauseAutoSpawning = false;
}

void PolicyHandler::setToSpawn(std::string& spriteclass, char race)
{
    /*
    SpriteType st;
    int gender = rand() % 2;
    if (race == 'a')
    {
        st = M_ALIEN_CITIZEN;
        if (spriteclass.compare("researcher") == 0)
        {
            if (gender == 1)
            {
                st = M_ALIEN_RESEARCHER;
            }
            else
            {
                st = F_ALIEN_RESEARCHER;
            }
        }
        else if (spriteclass.compare("citizen") == 0)
        {
            if (gender == 1)
            {
                st = M_ALIEN_CITIZEN;
            }
            else
            {
                st = F_ALIEN_CITIZEN;
            }
        }
    }
    
    else
    {
        st = M_MAYAN_CITIZEN;
        if (spriteclass.compare("warrior") == 0)
        {
            if (gender == 1)
            {
                st = M_MAYAN_WARRIOR;
            }
            else
            {
                st = F_MAYAN_WARRIOR;
            }
        }
        else if (spriteclass.compare("citizen") == 0)
        {
            if (gender == 1)
            {
                st = M_MAYAN_CITIZEN;
            }
            else
            {
                st = F_MAYAN_CITIZEN;
            }
        }
        else if (spriteclass.compare("farmer") == 0)
        {
            if (gender == 1)
            {
                st = M_MAYAN_FARMER;
            }
            else
            {
                st = F_MAYAN_FARMER;
            }
        }
        else if (spriteclass.compare("merchant") == 0)
        {
            if (gender == 1)
            {
                st = M_MAYAN_MERCHANT;
            }
            else
            {
                st = F_MAYAN_MERCHANT;
            }
        }
    }
    
    
    toSpawnSprite = st;*/
}

SpriteType PolicyHandler::getToSpawn()
{
    return toSpawnSprite;
}


float PolicyHandler::getCulturalExchangeRatio()
{
    return percentCulturalExchangeRate;
}