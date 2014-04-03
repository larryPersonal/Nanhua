//
//  Objective.h
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#ifndef PopulationTest_Objective_h
#define PopulationTest_Objective_h

#include <vector.h>
#include "cocos2d.h"
using namespace cocos2d;

enum ObjectiveType {
    DefaultGoal, PopulationGoal, ReputationGoal, BuildBuilding, GoldGoal, FoodGoal
};

class Objective : public CCObject
{
public:
    ObjectiveType oType;
    
    int oid;
    int value;
    std::string strValue;
    
    int goldReward;
    int foodReward;
    int populationReward;
    int reputationReward;
    
    bool active;
    bool complete;
    
    vector<int> preConditions;
    
public:
    static Objective* create();
    
    Objective();
    ~Objective();
    
    std::string getObjectiveString();
};

#endif
