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
    
    int bonus_work_rate;
    int bonus_hapiness_limit;
    int bonus_loyalty_limit;
    
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
    
    Building* homeLocation;
    Building* jobLocation;
    Building* targetLocation;
    Building* targetHomeLocation;
    
    float cumulativeTime;
    
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
        
        bonus_work_rate = 0;
        bonus_hapiness_limit = 0;
        bonus_loyalty_limit = 0;
        
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
        
        homeLocation = NULL;
        jobLocation = NULL;
        targetLocation = NULL;
        
        cumulativeTime = 0.0f;
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
   
    int PerformTask(float dt, int secToDay, float workRate)
    {
        cumulativeTime += dt;
        if(cumulativeTime >= 1.0f / (2.0f * workRate / 100.0f))
        {
            energyRating -= 25.0f / workRate;
            if(energyRating < 0)
            {
                energyRating = 0;
                return 0;
            }
            return 1;
        }
        return 0;
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
