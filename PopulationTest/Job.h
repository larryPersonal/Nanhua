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

//Note that a building under construction does NOT actually have a job record.
class Job: public cocos2d::CCObject
{
public:
    
    virtual Job* copyWithZone(CCZone *pZone);
    static Job* create();
    std::string jobName;
    std::string job_targetSpriteType;
    
    int job_workvalreq; //this value shows how much you need to complete this job.
    int job_currworkval;
    int job_loymod;
    int job_hapmod;
    int job_expmod;
    
    bool job_isTaken;
       
    Job();
    virtual ~Job();
    
    //note: it's still up to the sprite to clean up after itself, so this simply adds to and subtracts the vacancies for the current job.
    void signUpJob();
    void leaveJob(); 
 
    int PerformTask(GameSprite* sp);
    
    
   //void ModifyStats(GameSprite* sp);
};

#endif /* defined(__PopulationTest__Job__) */