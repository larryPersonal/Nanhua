//
//  AnimatedSprite.h
//  PopulationTest
//
//  Created by admin on 27/3/14.
//
//

#ifndef PopulationTest_AnimatedSprite_h
#define PopulationTest_AnimatedSprite_h

#include "cocos2d.h"

using namespace cocos2d;

class AnimatedSprite : public CCObject
{
public:
    CCSprite* sprite;
    float cumulativeTime;
    int currentAlpha;
    bool fadeIn;
    bool fadeOut;
    bool stopFade;
    float fadeInSpeed;
    float fadeOutSpeed;
    bool hasFadeInAnimation;
    bool hasFadeOutAnimation;
public:
    static AnimatedSprite* create(std::string, bool, bool, bool = false);
    AnimatedSprite(std::string, bool, bool, bool);
    ~AnimatedSprite();
    void setup(std::string, bool);
    void update(float);
    CCSprite* getSprite();
    void triggerFadeOut();
};


#endif
