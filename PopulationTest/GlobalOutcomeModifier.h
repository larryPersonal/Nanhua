//
//  GlobalOutcomeModifier.h
//  PopulationTest
//
//  Created by admin on 3/4/14.
//
//

#ifndef PopulationTest_GlobalOutcomeModifier_h
#define PopulationTest_GlobalOutcomeModifier_h

#include "cocos2d.h"

using namespace cocos2d;

class GlobalOutcomeModifier
{
    static GlobalOutcomeModifier* SP;
public:
    int banditsModifier;
    int refugeesModifier;
    int goldModifier;
    int foodModifier;
    int populationModifier;
    
public:
    GlobalOutcomeModifier();
    ~GlobalOutcomeModifier();
    
    static GlobalOutcomeModifier* getThis();
};

#endif
