//
//  RandomEvent.h
//  PopulationTest
//
//  Created by admin on 9/7/14.
//
//

#ifndef PopulationTest_RandomEvent_h
#define PopulationTest_RandomEvent_h

#include "cocos2d.h"

using namespace cocos2d;

class RandomEvent : public CCObject
{
public:
    int event_id;
    std::string event_name;
    std::string event_image;
    std::string event_description;
    std::vector<std::string> event_effects;
    
public:
    RandomEvent();
    ~RandomEvent();
    
    static RandomEvent* create();
};

#endif
