//
//  AnimatedRain.h
//  PopulationTest
//
//  Created by GAO YU on 27/4/14.
//
//

#ifndef PopulationTest_AnimatedRain_h
#define PopulationTest_AnimatedRain_h

#include "cocos2d.h"

using namespace cocos2d;

class AnimatedRain : public cocos2d::CCNode
{
public:
    int frameWidth;
    int frameHeight;
    
    CCTexture2D* rainTexture;
    CCRect rainRect;
    
    int frameNo;
    int maxFrameNo;
    
    int xOffset;
    int yOffset;
    
    float delay_offset;
    float delay;
    
    bool isMoving;
    bool isInAction;
    
    CCArray* path;
    CCSprite* rainSprite;
    
    CCPoint nextPos;
    
    float movementSpeed;
    
public:
    static AnimatedRain* create();
    AnimatedRain();
    ~AnimatedRain();
    
    virtual void update(float deltaTime);
    
    CCPoint getWorldPosition();
    bool CreatePath(CCPoint fromPos, CCPoint toPos);
    
    void move(float deltaTime);
};



#endif
