//
//  AnimatedDialogue.h
//  PopulationTest
//
//  Created by admin on 1/6/14.
//
//

#ifndef PopulationTest_AnimatedDialogue_h
#define PopulationTest_AnimatedDialogue_h

#include "cocos2d.h"

using namespace cocos2d;

class AnimatedDialogue : public CCObject
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
    static AnimatedDialogue* create(std::string, bool, bool);
    AnimatedDialogue(std::string, bool, bool);
    ~AnimatedDialogue();
    void setup(std::string);
    void update(float);
    CCSprite* getSprite();
    void triggerFadeOut();
};

#endif
