//
//  Job.h
//  PopulationTest
//
//  Created by Serious Games on 30/4/13.
//
//

#ifndef __PopulationTest__Job__
#define __PopulationTest__Job__
#include "cocos2d.h"
#include "Sprite.h"
using namespace cocos2d;

class Job: public cocos2d::CCObject
{
public:
    
    virtual Job* copyWithZone(CCZone *pZone);
    
    static Job* create();
    
    std::string jobName;
    
    int job_targetEducationLevel;
    std::string job_targetSpriteType;
    
    int happiness_mod;
    int loyalty_mod;
    
    int energy_mod;
    
    int exp_mod;
    
    bool job_isTaken;
       
    Job();
    virtual ~Job();
    
    //TODO: check for sprite type.
    bool isJobBetter(int qualLvl, int targetrank);
    int isCandidateQualified(int qualLvl, std::string spritetype, bool ignoreQual = false);
    
    //note: it's still up to the sprite to clean up after itself, so this simply adds to and subtracts the vacancies for the current job.
    void signUpJob();
    void leaveJob(); 
 
    
   void ModifyStats(GameSprite* sp);
};

#endif /* defined(__PopulationTest__Job__) */