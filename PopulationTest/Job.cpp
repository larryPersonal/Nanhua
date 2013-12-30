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
    exp_mod = energy_mod = loyalty_mod = happiness_mod =0;
    
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
        pCopy->energy_mod = this->energy_mod;
        pCopy->exp_mod = this->exp_mod;
        pCopy->loyalty_mod = this->loyalty_mod;
        pCopy->happiness_mod = this->happiness_mod;
        
        pCopy->job_targetEducationLevel = this->job_targetEducationLevel;
        pCopy->job_targetSpriteType = this->job_targetSpriteType;
        pNewZone = new CCZone(pCopy);
        
    }
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

bool Job::isJobBetter(int qualLvl, int targetrank)
{
    //if (targetrank >= job_targetSpriteType_rank) return false;
    
//    if (job_targetEducationLevel <= qualLvl)  return false; //qualifications to be reenabled later.
    
    return true;
    
    
}


int Job::isCandidateQualified(int qualLvl, std::string spritetype, bool ignoreQual)
{
    int retval = 0;
    
    if (job_targetSpriteType.compare("all") == 0) //SPRITETYPE_END
    {
        /*
        if (!ignoreQual)
        {
            if (job_targetEducationLevel < qualLvl)
                retval = 2;
        }*/
        if (job_targetEducationLevel > qualLvl)
            retval = 1;
    }
    else
    {
        if (job_targetSpriteType.compare(spritetype) == 0)
        {
           /*
            if (!ignoreQual)
            {
                if (job_targetEducationLevel < qualLvl)
                    retval = 2;
            }*/
            if (job_targetEducationLevel > qualLvl)
                retval = 1;
        }
        else
        {
            retval = 3;
        }
        
      //  return (job_targetEducationLevel == qualLvl && job_targetSpriteType.compare(spritetype) == 0);
    }
    
    return retval;
}




void Job::signUpJob()
{
    job_isTaken = true;
}

void Job::leaveJob()
{
    job_isTaken = false;
}

void Job::ModifyStats(GameSprite* sp)
{
    if (sp==NULL) return;
    sp->increasePossession(STATS_ENERGY, energy_mod);
    sp->increasePossession(STATS_EXP, exp_mod);
    sp->increasePossession(STATS_HAPPINESS, happiness_mod);
    sp->increasePossession(STATS_LOYALTY, loyalty_mod);
}

