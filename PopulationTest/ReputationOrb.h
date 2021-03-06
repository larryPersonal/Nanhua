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

enum OrbStatus
{
    APPEAR, EXIST, DISAPPEAR
};

class ReputationOrb : public CCObject
{
public:
    int frameHeight;
    int frameWidth;
    
    CCTexture2D *orbTexture;
    CCRect orbRect;
    int x_frameno;
    int x_maxframeno;
    int y_offset;

    float delay_animFrame;
    float delay_curr;
    
    CCSprite* orbSprite;
    
    float opacity;
    float targetOpacity;
    float fadeSpeed;
    
    float cumulativeTime;
    float triggerTime;
    bool disappear;
    bool collected;
    bool stopAnimation;
    
    OrbStatus orbStatus;
    
public:
    static ReputationOrb* create(std::string, float);
    ReputationOrb(std::string, float);
    ~ReputationOrb();
    
    CCSprite* getSprite();
    virtual void update(float);
    
    void collect(CCPoint touchLoc);
    void collectComplete();
    
    void appearAnimation(float deltaTime);
    void disappearAnimation(float deltaTime);
};

#endif
