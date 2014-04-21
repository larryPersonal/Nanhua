//
//  FloatingArraw.cpp
//  PopulationTest
//
//  Created by admin on 21/4/14.
//
//

#include "FloatingArraw.h"

FloatingArraw* FloatingArraw::create()
{
    FloatingArraw *pRet = new FloatingArraw();
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

FloatingArraw::FloatingArraw()
{
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    frameHeight = 64;
    frameWidth = 64;
    
    arrowTexture = CCTextureCache::sharedTextureCache()->addImage("arrowupanddown.png");
    
    y_offset = 0;
    int x_offset = 0;
    
    //assume left to right. ALWAYS make the leftmost frame the first frame.
    //hopefully the artist will not try to make
    x_frameno = 0;
    x_maxframeno = 8;
    
    y_offset = x_frameno / 4.0f;
    x_offset = x_frameno % 4.0f;
    
    orbRect = CCRectMake(0, y_offset * frameHeight,  frameWidth, frameHeight);
    
    //set the thing to the first frame.
    orbSprite = CCSprite::createWithTexture(orbTexture, orbRect);
    
    
    targetOpacity = 0;
    fadeSpeed = 5;
    cumulativeTime = 0;
    triggerTime = tTime;
    disappear = false;
    stopAnimation = false;
}

FloatingArraw::~FloatingArraw()
{
    
}

void FloatingArraw::displayArraw(float dt)
{
    y_offset = 0;
    
    x_frameno = 0;
    x_maxframeno = 8;
    
    arrowRect = CCRectMake(0, y_offset * frameHeight,  frameWidth, frameHeight);
    
    CCSprite* arrowSprite = CCSprite::createWithTexture(arrowTexture, arrowRect);
}