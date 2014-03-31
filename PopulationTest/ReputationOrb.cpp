//
//  ReputationOrb.cpp
//  PopulationTest
//
//  Created by admin on 31/3/14.
//
//

#include "ReputationOrb.h"
#include "GameScene.h"

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
    orbSprite = CCSprite::create(spriteStr.c_str());
    opacity = 255;
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
}