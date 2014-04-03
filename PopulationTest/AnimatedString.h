//
//  AnimatedString.h
//  PopulationTest
//
//  Created by admin on 21/3/14.
//
//

#ifndef PopulationTest_AnimatedString_h
#define PopulationTest_AnimatedString_h

#include "cocos2d.h"

using namespace cocos2d;

class AnimatedString : public CCObject
{
public:
    CCLabelTTF* label;
    float cumulativeTime;
    float triggerTime;
    int currentAlpha;
    bool flashIn;
    bool stopFade;
    float fadeInSpeed;
public:
    static AnimatedString* create(std::string, float, std::string, float, float);
    AnimatedString(std::string, float, std::string, float, float);
    ~AnimatedString();
    void setup(std::string, std::string, float);
    void update(float);
    CCLabelTTF* getLabel();
};


#endif
