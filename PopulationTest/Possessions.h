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
     int happinessRating;
    int loyaltyRating;
   
    int movementRange;
    
    float movementSpeed;
    float animateSpeed;
    
    int defaultEnergy;
    int energyRating;
    
    int classLevel;
    
    int expRating;
    CCArray* expToLevel;
    
    Building* homeLocation;
   // bool isRentalProperty;
  //  bool isSqueezing;
   // int rentalVisitsLeft;
    
    Building* jobLocation;
   // std::string jobClass; //check for this == to sprite class to know if sprite has taken the correct job
   // int jobIndex;
    //I think I'll store the index instead of the job pointer.
    //call jobLocation->getJobsAvailable()->objectAtIndex(jobIndex) and cast to a Job*
    
    Possessions()
    {
       homeLocation = NULL;
        jobLocation = NULL;
      
        loyaltyRating = happinessRating = 50;
        //note: movement range is range from a sprite's HOME. If the sprite has no home, this value will be ignored.
        movementRange = 1;
        energyRating = defaultEnergy = 100;
        
        expRating = 0;    
        expToLevel = CCArray::create();
        expToLevel->retain();
    }
    
    ~Possessions()
    {
        
        expToLevel->removeAllObjects();
            expToLevel->release();
    }
    
    int getExpToLevel()
    {
        if ((classLevel-1) >= expToLevel->count()) return 0;
        
        return ((CCInteger*)expToLevel->objectAtIndex(classLevel-1))->getValue();
    }
    
    bool isAtMaxLevel()
    {
        return classLevel == expToLevel->count();
    }
    
    bool isAtAbsoluteMax()
    {
        return classLevel >= 5;
    }
    
    
    void setClassLevel(int targetLevel)
    {
        if (targetLevel < 1)
        {
            classLevel = 1;
            return;
        }
        if (targetLevel > expToLevel->count())
        {
            classLevel = expToLevel->count();
            return;
        }
        
        classLevel = targetLevel;

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
    
    int Rest()
    {
        ++energyRating;
        if (energyRating > defaultEnergy)
        {
            energyRating = defaultEnergy;
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
