//
//  NanhuaGameStaticAPI.h
//  PopulationTest
//
//  Created by admin on 27/8/14.
//
//

#ifndef PopulationTest_NanhuaGameStaticAPI_h
#define PopulationTest_NanhuaGameStaticAPI_h

#include "cocos2d.h"

using namespace cocos2d;

class NanhuaGameStaticAPI
{
public:
    
    
public:
    // population
    static int getCurrentCitizenNumber(); // get the number of villagers that are citizens in the village.
    
    // get average happiness from all the population in the village.
    static float getAverageHappiness();
    
    // get the number of villagers in the village;
    static int getNumberOfVillagers();
    
    // get the number of citizens in the village;
    static int getNumberOfCitizens();
};

#endif
