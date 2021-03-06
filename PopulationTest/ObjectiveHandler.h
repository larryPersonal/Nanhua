//
//  ObjectiveHandler.h
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#ifndef PopulationTest_ObjectiveHandler_h
#define PopulationTest_ObjectiveHandler_h

#include "cocos2d.h"
#include "Objective.h"
using namespace cocos2d;

class ObjectiveHandler : public CCLayer
{
public:
    CCArray* objectives;
    
    int currID;
    int nextID;
    
    int progressNumber;
    
    Objective* obj;
    
    int startPopulation;
    int targetPopulation;
    
    static ObjectiveHandler* SP;
public:
    ObjectiveHandler();
    ~ObjectiveHandler();
    
    static ObjectiveHandler* create();
    static ObjectiveHandler* getThis();
    
    void loadObjective();
    void playObjective(bool showNotification = true);
    void playObjectiveWithObjectiveID(int objectiveID, bool showNotification = true);
    
    void update(float);
    
};


#endif
