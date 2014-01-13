//
//  Job.cpp
//  PopulationTest
//
//  Created by Serious Games on 30/4/13.
//
//

#include "Job.h"
#include "GameScene.h"

Job::Job()
{
    //job_targetEducationLevel = job_energyCost = job_monthlyPay = job_expGain = 0;
    
    job_isTaken = false;
}

Job::~Job()
{
    
}

Job* Job::create()
{
    Job *pRet = new Job();
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
}
/*copy constructor*/
Job* Job::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    Job* pCopy = NULL;
    
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (Job*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy =Job::create();
        pCopy->jobName = this->jobName;
        pCopy->job_isTaken = false;
        
        pCopy->job_targetSpriteType = this->job_targetSpriteType;
        pNewZone = new CCZone(pCopy);
        
    }
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}


void Job::signUpJob()
{
    job_isTaken = true;
}

void Job::leaveJob()
{
    job_isTaken = false;
}

