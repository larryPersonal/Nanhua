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
using namespace cocos2d;

class ObjectiveHandler
{
public:
    CCArray* objectives;
public:
    ObjectiveHandler();
    ~ObjectiveHandler();
    
    void loadObjective();
    
    CCObject* getObjective(int index);
};


#endif
