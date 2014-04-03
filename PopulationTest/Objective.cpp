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
    complete = false;
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

std::string Objective::getObjectiveString()
{
    std::stringstream ss;
    if(oType == DefaultGoal)
    {
        ss << "No goals currently!";
    }
    else if(oType == PopulationGoal)
    {
        ss << "Raise the population of your village to " << value << "!";
    }
    else if(oType == ReputationGoal)
    {
        ss << "Raise your reputation larger than " << value << "!";
    }
    else if(oType == BuildBuilding)
    {
        ss << "Build an additional " << strValue << "!";
    }
    else if(oType == GoldGoal)
    {
        ss << "Manage to obtain gold of " << value << "!";
    }
    else if(oType == FoodGoal)
    {
        ss << "Manage to obtain food of " << value << "!";
    }
    else
    {
        ss << "unknow reason!";
    }
    
    return ss.str();
}