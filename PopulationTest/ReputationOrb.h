//
//  ReputationOrb.h
//  PopulationTest
//
//  Created by admin on 31/3/14.
//
//

#include "cocos2d.h"

using namespace cocos2d;

#ifndef PopulationTest_ReputationOrb_h
#define PopulationTest_ReputationOrb_h

class ReputationOrb : public CCObject
{
public:
    CCSprite* orbSprite;
    
    float opacity;
    float targetOpacity;
    float fadeSpeed;
    
    float cumulativeTime;
    float triggerTime;
    bool disappear;
    bool stopAnimation;
    
public:
    static ReputationOrb* create(std::string, float);
    ReputationOrb(std::string, float);
    ~ReputationOrb();
    
    CCSprite* getSprite();
    void update(float);
};

#endif
