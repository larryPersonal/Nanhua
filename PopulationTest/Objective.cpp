//
//  Objective.cpp
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#include "Objective.h"

Objective::Objective()
{
    oType = DefaultGoal;
    
    oid = 0;
    value = 0;
    strValue = "";
    
    goldReward = 0;
    foodReward = 0;
    populationReward = 0;
    reputationReward = 0;
    
    active = false;
}

Objective::~Objective()
{
    
}

Objective* Objective::create()
{
    Objective *pRet = new Objective();
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