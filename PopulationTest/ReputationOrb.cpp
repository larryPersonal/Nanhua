//
//  ReputationOrb.cpp
//  PopulationTest
//
//  Created by admin on 31/3/14.
//
//

#include "ReputationOrb.h"
#include "GameScene.h"


//REPUTATION ORB CLASS
ReputationOrb* ReputationOrb::create(std::string spriteStr, float tTime)
{
    ReputationOrb *pRet = new ReputationOrb(spriteStr, tTime);
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

ReputationOrb::ReputationOrb(std::string spriteStr, float tTime)
{
    if (spriteStr.compare("REN"))
        y_offset = 0;
        
    if (spriteStr.compare("XUN"))
        y_offset = 1;
    
        
    if (spriteStr.compare("ZHONG"))
        y_offset = 2;
        
    if (spriteStr.compare("AI"))
        y_offset = 3;
    
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    opacity = 255;
    
    //change frame here
    frameHeight = 64;
    frameWidth = 64;
    
    
    // orbSprite = CCSprite::create(spriteStr.c_str());
    orbTexture = CCTextureCache::sharedTextureCache()->addImage("tokenanim.png");
    
    //assume left to right. ALWAYS make the leftmost frame the first frame.
    //hopefully the artist will not try to make
    x_frameno = 0;
    x_maxframeno = orbTexture->getPixelsWide() / frameWidth;
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

ReputationOrb::~ReputationOrb()
{
    
}

CCSprite* ReputationOrb::getSprite()
{
    return orbSprite;
}

void ReputationOrb::update(float dt)
{
    if(!stopAnimation)
    {
        if(!disappear)
        {
            if(cumulativeTime >= triggerTime)
            {
                cumulativeTime = 0;
                disappear = true;
            }
        }
        else
        {
            if(cumulativeTime >= 0.01f)
            {
                cumulativeTime = 0;
                opacity = opacity - fadeSpeed;
                if(opacity <= targetOpacity)
                {
                    opacity = 0;
                    stopAnimation = true;
                    GameScene::getThis()->spriteHandler->tokensOnMap->removeObject(this);
                    GameScene::getThis()->mapHandler->getMap()->removeChild(orbSprite);
                }
                else {
                    orbSprite->setOpacity((GLubyte) opacity);
                }
            }
        }
        cumulativeTime += dt;
    }
    
    if (opacity == 0)
    {
       // CC_SAFE_RELEASE(orbSprite);
       // CC_SAFE_RELEASE(orbTexture);
    }
    else
    {
        if (delay_curr > 0)
        {
            delay_curr -= dt;
        }
        else
        {
            ++x_frameno;
            if (x_frameno >= x_maxframeno)
            {
                x_frameno = 0;
            }
            orbRect.setRect(x_frameno * frameWidth, y_offset * frameHeight, frameWidth, frameHeight);
            orbSprite->setTextureRect(orbRect);
            
            delay_curr = delay_animFrame;
        }
        
    }
    
}