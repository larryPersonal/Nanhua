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
    DefaultGoal, PopulationGoal, ReputationGoal, BuildBuilding, GoldGoal, FoodGoal, DisplayGoal
};

class Objective : public CCObject
{
public:
    ObjectiveType oType;
    
    int oid;
    int nid;
    int value;
    std::string strValue;
    
    int goldReward;
    int foodReward;
    int populationReward;
    int reputationReward;
    
    bool complete;
    
    std::string title;
    std::string content;
    std::string progress;
    
public:
    static Objective* create();
    
    Objective();
    ~Objective();
    
    std::string getObjectiveString();
};

#endif
