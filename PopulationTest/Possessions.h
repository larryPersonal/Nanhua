//
//  Possessions.h
//  PopulationTest//
//  Created by Serious Games on 15/4/13.
//
//

#ifndef __PopulationTest__Possessions__
#define __PopulationTest__Possessions__

#include <iostream>
#include "Building.h"
#include "Sprite.h"
#include "Behavior.h"
#include "Job.h"

/* contains the sprite's possessions at the moment*/
/* this is contained as a private member inside Sprite and used as a private pointer inside Behavior.*/
struct Possessions
{
    /* game sprite common stats */
    int happinessRating;
    int loyaltyRating;
    
    int currentHungry;
    int targetHungry;
    float energyRating;
    
    int default_work_rate;
    int default_hapiness_limit;
    int default_loyalty_limit;
    int default_exp_level1;
    int default_exp_level2;
    int default_exp_level3;
    int default_exp_level4;
    int default_exp_level5;
    int default_spawn_cost;
    
    int bonus_work_rate;
    int bonus_hapiness_limit;
    int bonus_loyalty_limit;
    int bonus_exp_level1;
    int bonus_exp_level2;
    int bonus_exp_level3;
    int bonus_exp_level4;
    int bonus_exp_level5;
    int bonus_spawn_cost;
    
    /* special stats */
    float default_work_unit_per_day;
    int default_movement_range;
    int default_movement_speed;
    int default_animate_speed;
    int default_hungry_limit;
    int default_food_carriage_limit;
    int default_energy_limit;
    
    float bonus_work_unit_per_day;
    int bonus_movement_range;
    int bonus_movement_speed;
    int bonus_animate_speed;
    int bonus_hungry_limit;
    int bonus_food_carriage_limit;
    int bonus_energy_limit;
    
    /* variable stats */
    int current_farmer_exp;
    int current_sodier_exp;
    
    int current_farmer_level;
    int current_sodier_level;
    
    int classLevel;
    
    Building* homeLocation;
    Building* jobLocation;
    Building* targetLocation;
    Building* targetHomeLocation;
    
    Possessions()
    {
        /* common stats */
        loyaltyRating = 0;
        happinessRating = 0;
        
        currentHungry = 0;
        targetHungry = 0;
        energyRating = 0;
        
        default_work_rate = 0;
        default_hapiness_limit = 0;
        default_loyalty_limit = 0;
        default_exp_level1 = 0;
        default_exp_level2 = 0;
        default_exp_level3 = 0;
        default_exp_level4 = 0;
        default_exp_level5 = 0;
        default_spawn_cost = 0;
        
        bonus_work_rate = 0;
        bonus_hapiness_limit = 0;
        bonus_loyalty_limit = 0;
        bonus_exp_level1 = 0;
        bonus_exp_level2 = 0;
        bonus_exp_level3 = 0;
        bonus_exp_level4 = 0;
        bonus_exp_level5 = 0;
        bonus_spawn_cost = 0;
        
        /* special stats */
        default_work_unit_per_day = 0;
        default_movement_range = 0;
        default_movement_speed = 0;
        default_animate_speed = 0;
        default_hungry_limit = 0;
        default_food_carriage_limit = 0;
        default_energy_limit = 0;
        
        bonus_work_unit_per_day = 0;
        bonus_movement_range = 0;
        bonus_movement_speed = 0;
        bonus_animate_speed = 0;
        bonus_hungry_limit = 0;
        bonus_food_carriage_limit = 0;
        bonus_energy_limit = 0;
        
        /* variable stats */
        current_farmer_exp = 0;
        current_sodier_exp = 0;
        
        current_farmer_level = 0;
        current_sodier_level = 0;
        
        classLevel = 0;
        
        homeLocation = NULL;
        jobLocation = NULL;
        targetLocation = NULL;
    }
    
    ~Possessions()
    {
    }
    
    int getExpToLevel()
    {
        return 0;
    }
    
    bool isAtMaxLevel()
    {
        return false;
    }
    
    bool isAtAbsoluteMax()
    {
        return false;
    }
    
    
    void setClassLevel(int targetLevel)
    {
    }
   
    int PerformTask()
    {
        --energyRating;
        if (energyRating < 0)
        {
            energyRating = 0;
            CCLog("out of energy");
            return 0;
        }
        //the rate of
        return 1;
        
    }
    
    int EatFood()
    {
        currentHungry++;
        if(currentHungry > default_hungry_limit + bonus_hungry_limit)
        {
            currentHungry = default_hungry_limit + bonus_hungry_limit;
            CCLog("finish eating food!");
            return 0;
        }
        
        return 1;
    }
    
    int Rest()
    {
        energyRating++;
        if (energyRating > default_energy_limit + bonus_energy_limit)
        {
            energyRating = default_energy_limit + bonus_energy_limit;
            return 0;
        }
        return 1;
    }
    
    bool setJob(Building* jobl)
    {
        if (jobl->buildingType == HOUSING) return false;
        jobLocation = jobl;
        return true;
    }
    
    //you can normally setJob(anotherJob) to get a new job. BUT if you just want to leave the job because the building is deconstructed...
    void LeaveJob()
    {
        jobLocation = NULL;
    }

    
    bool setHome(Building* homel)
    {
        if (homel->buildingType != HOUSING) return false;
        homeLocation = homel;
        return true;
    }
    
    //see LeaveJob();
    void LeaveHome()
    {
        homeLocation = NULL;
    }

};

enum PossessionStats
{
    STATS_CASHONHAND = 0,
    STATS_HAPPINESS,
    STATS_LOYALTY,
    STATS_INTELLIGENCE,
    STATS_SOCIAL,
    STATS_ENERGY,
    STATS_EXP
};


#endif /* defined(__PopulationTest__Possessions__) */
