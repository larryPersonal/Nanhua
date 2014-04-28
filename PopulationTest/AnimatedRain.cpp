//
//  AnimatedRain.cpp
//  PopulationTest
//
//  Created by GAO YU on 27/4/14.
//
//

#include "AnimatedRain.h"
#include "GameScene.h"

AnimatedRain* AnimatedRain::create()
{
    AnimatedRain* ar = new AnimatedRain;
    if(ar)
    {
        ar->autorelease();
        return ar;
    }
    else
    {
        CC_SAFE_DELETE(ar);
        return NULL;
    }
}

AnimatedRain::AnimatedRain()
{
    frameWidth = 164;
    frameHeight = 164;
    
    frameNo = 0;
    maxFrameNo = 2;
    
    xOffset = 0;
    yOffset = 0;
    
    delay_offset = 10.0f;
    delay = 10.0f;
    
    rainTexture = CCTextureCache::sharedTextureCache()->addImage("raining.png");
    rainRect = CCRectMake(0, 0, frameWidth, frameHeight);
    
    rainSprite = CCSprite::createWithTexture(rainTexture, rainRect);
    rainSprite->setScale(1.0f);
    rainSprite->setAnchorPoint(ccp(0.5, 0.5));
    
    CCPoint rainPos = CCPointMake(30, 23);
    rainPos = GameScene::getThis()->mapHandler->locationFromTilePos(&rainPos);
    
    GameScene::getThis()->mapHandler->getMap()->addChild(rainSprite, 999999);
    rainSprite->setPosition(rainPos);
    
    path = CCArray::create();
    path->retain();
}

AnimatedRain::~AnimatedRain()
{
    path->removeAllObjects();
    CC_SAFE_RELEASE(path);
}

void AnimatedRain::update(float deltaTime)
{
    // change appearance
    if(delay > 0)
    {
        delay -= deltaTime;
    }
    else
    {
        frameNo++;
        if(frameNo >= maxFrameNo)
        {
            frameNo = 0;
        }
        
        xOffset = 0;
        yOffset = frameNo;
        
        rainRect.setRect(xOffset * frameWidth, yOffset * frameHeight, frameWidth, frameHeight);
        rainSprite->setTextureRect(rainRect);
        
        delay = delay_offset;
    }
    
    
}