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
    int cashOnHand;
    int happinessRating;
    int loyaltyRating;
    int educationLevel;
    int intelligenceRating;
    int socialRating;
    int movementRange;
    
    int defaultEnergy;
    int energyRating;
    
    int classLevel;
    int expRating;
    CCArray* expToLevel;
    
    bool hasHouse;
    Building* homeLocation;
   // bool isRentalProperty;
  //  bool isSqueezing;
   // int rentalVisitsLeft;
    
   // bool hasJob;
   // Building* jobLocation;
   // std::string jobClass; //check for this == to sprite class to know if sprite has taken the correct job
   // int jobIndex;
    //I think I'll store the index instead of the job pointer.
    //call jobLocation->getJobsAvailable()->objectAtIndex(jobIndex) and cast to a Job*
    
    Possessions()
    {
        cashOnHand = 0;
        hasHouse = false;
        homeLocation = NULL;
      //  isRentalProperty = false;
       // isSqueezing = false;
      //  rentalVisitsLeft = 0;
        
        
      //  jobLocation = NULL;
      //  jobIndex = -1;
      //  jobClass = "";
        
        
        loyaltyRating = happinessRating = 50;
      //  educationLevel = classLevel = 1;
        intelligenceRating = 1;
        socialRating = 1;
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
        return educationLevel == 20;
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
    void SetEducationLevel(int targetLevel)
    {
        if (targetLevel < 1)
        {
            educationLevel = 1;
            return;
        }
        educationLevel = targetLevel;
        //Note: I do NOT check numbers here, please ensure that the value provided is correct! 
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
