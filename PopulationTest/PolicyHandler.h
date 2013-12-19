//
//  PolicyHandler.h
//  PopulationTest
//
//  Created by Serious Games on 8/5/13.
//
//

#ifndef __PopulationTest__PolicyHandler__
#define __PopulationTest__PolicyHandler__

#include <iostream>
/*A policy handler implements game policies, such as taxation and population control.*/
#include "Building.h"
#include "Sprite.h"

class PolicyHandler
{
    int next_alien_arrives_in;
    int next_local_birth_in;

    void BirthNewLocal();
    
    void BirthNewAlien();
    
    /*detecting month, week and day transitions*/
    int last_frame_month;
    int this_frame_month;
  //  int last_frame_week;
  //  int this_frame_week;
    int last_frame_day;
    int this_frame_day;
    
    void UpdateFrames();
    
    SpriteType nextMayanSpawn;
    SpriteType nextAlienSpawn;
    
    bool pauseAutoSpawning;
    bool yearOne;
    
    
    SpriteType toSpawnSprite;
    
    bool isFirstMonth;
    bool isFirstYear;
    
    
public:
    PolicyHandler();
    /*revealed numbers for UI, if any*/
    /*changed numbers will only apply in the next cycle.*/
    int monthly_alienImportRate;
    int monthly_localBirthRate;
    int maxAlienPopulation;
    
    bool policy_active_br;
    bool policy_active_ce;
    int birthRatePolicy_months;
    int culturalExchangePolicy_months;
    
    float percentTax;
    float percentCulturalExchangeRate;
    
    void init();
    
    
    void Update();
    
    
    void ImportForeignersInBuilding(Building* b, int amt);
    
    
    void CollectTaxes();
    void YearlyMaintenance();
    void MonthlyMaintenance();
    
    void SpawnImmediately(SpriteType sp);
    void SetNextSpawn(SpriteType sp);
    void SetNextAlienSpawn(SpriteType sp);
    
    /*allows us to hold and re-enable auto spawning. We may want to hold spawning to prevent the tutorial from being short circuited.*/
    void pauseSpawn();
    void startSpawn();
    
    /*spawns a specific sprite. Note: GameScene decides WHERE to put it.*/
    void setToSpawn(std::string& spriteclass, char race);
    SpriteType getToSpawn();

    void setBirthRatePolicy(int deltaBirthRate, int duration);
    bool isImplementingBirthRatePolicy();
    void setCulturalExchangePolicy(int deltaExchangeRate, int duration) {} //to implement later
    bool isImplementingCulturalExchangePolicy();
    
    int getBirthRateDurationLeft();
    int getCulturalExchangeDurationLeft();
    
    float getCulturalExchangeRatio();
};


#endif /* defined(__PopulationTest__PolicyHandler__) */
