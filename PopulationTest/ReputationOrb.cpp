//
//  ReputationOrb.cpp
//  PopulationTest
//
//  Created by admin on 31/3/14.
//
//

#include "ReputationOrb.h"
#include "GameScene.h"
#include "GameHUD.h"
#include "SoundtrackManager.h"
#include "TutorialManager.h"

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
    {
        y_offset = 3;
    }
        
    if (spriteStr.compare("XUN"))
    {
        y_offset = 2;
    }
    
    if (spriteStr.compare("ZHONG"))
    {
        y_offset = 1;
    }
        
    if (spriteStr.compare("AI"))
    {
        y_offset = 0;
    }
    
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    opacity = 255;
    
    //change frame here
    frameHeight = 128;
    frameWidth = 64;
    
    // orbSprite = CCSprite::create(spriteStr.c_str());
    //orbTexture = CCTextureCache::sharedTextureCache()->addImage("tokenanim.png");
    orbTexture = CCTextureCache::sharedTextureCache()->addImage("appear.png");
    
    //assume left to right. ALWAYS make the leftmost frame the first frame.
    //hopefully the artist will not try to make
    x_frameno = 0;
    x_maxframeno = 8;
    orbRect = CCRectMake(0, ((int)(x_frameno / 4)) * frameHeight,  frameWidth, frameHeight);
    
    //set the thing to the first frame.
    orbSprite = CCSprite::createWithTexture(orbTexture, orbRect);
    
    targetOpacity = 0;
    fadeSpeed = 5;
    cumulativeTime = 0;
    triggerTime = tTime;
    disappear = false;
    stopAnimation = false;
    collected = false;
    
    orbStatus = APPEAR;
}

ReputationOrb::~ReputationOrb()
{
    
}

CCSprite* ReputationOrb::getSprite()
{
    return orbSprite;
}

void ReputationOrb::appearAnimation(float deltaTime)
{
    if (delay_curr > 0)
    {
        delay_curr -= deltaTime;
    }
    else
    {
        ++x_frameno;
        if (x_frameno >= x_maxframeno)
        {
            x_frameno = 0;
            
            frameWidth = 64;
            frameHeight = 64;
            
            orbTexture = CCTextureCache::sharedTextureCache()->addImage("tokenanim.png");
            x_maxframeno = 4;
            orbRect.setRect(0, y_offset * frameHeight, frameWidth, frameHeight);
            
            orbSprite->setTexture(orbTexture);
            orbSprite->setTextureRect(orbRect);
            
            orbStatus = EXIST;
            delay_curr = delay_animFrame;
        }
        else
        {
            orbRect.setRect(x_frameno * frameWidth, y_offset * frameHeight, frameWidth, frameHeight);
            orbSprite->setTextureRect(orbRect);
        
            delay_curr = delay_animFrame;
        }
    }
}

void ReputationOrb::disappearAnimation(float deltaTime)
{
    if (delay_curr > 0)
    {
        delay_curr -= deltaTime;
    }
    else
    {
        ++x_frameno;
        if (x_frameno >= x_maxframeno)
        {
            GameScene::getThis()->spriteHandler->tokensOnMap->removeObject(this);
            GameScene::getThis()->mapHandler->getMap()->removeChild(orbSprite);
            GameHUD::getThis()->removeChild(orbSprite);
        }
        else
        {
            orbRect.setRect(x_frameno * frameWidth, y_offset * frameHeight, frameWidth, frameHeight);
            orbSprite->setTextureRect(orbRect);
            
            delay_curr = delay_animFrame;
        }
    }
}

void ReputationOrb::update(float dt)
{
    if(orbStatus == APPEAR)
    {
        appearAnimation(dt);
    }
    else if(orbStatus == DISAPPEAR)
    {
        disappearAnimation(dt);
    }
    else
    {
        if(!stopAnimation && (!TutorialManager::getThis()->active || (TutorialManager::getThis()->miniDragon != NULL && !TutorialManager::getThis()->miniDragon->dropToken)))
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
                x_frameno = 0;
                
                frameWidth = 64;
                frameHeight = 128;
                
                orbTexture = CCTextureCache::sharedTextureCache()->addImage("disappear.png");
                x_maxframeno = 8;
                orbRect.setRect(0, ((int)(x_frameno / 4)) * frameHeight, frameWidth, frameHeight);
                
                orbSprite->setTexture(orbTexture);
                orbSprite->setTextureRect(orbRect);
                
                collected = true;
                stopAnimation = true;
                
                orbStatus = DISAPPEAR;
                delay_curr = delay_animFrame;
            }
            cumulativeTime += dt;
        }
        
        if (opacity == 0)
        {
            if (orbSprite)
            {
                orbSprite = NULL;
            }
            
            if (orbTexture)
            {
                orbTexture = NULL;
            }
        }
        else
        {
            if (!collected)
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
    }
}

void ReputationOrb::collectComplete()
{
    x_frameno = 0;
    
    frameWidth = 64;
    frameHeight = 128;
    
    orbTexture = CCTextureCache::sharedTextureCache()->addImage("disappear.png");
    x_maxframeno = 8;
    orbRect.setRect(0, ((int)(x_frameno / 4)) * frameHeight, frameWidth, frameHeight);
    
    orbSprite->setTexture(orbTexture);
    orbSprite->setTextureRect(orbRect);
    
    collected = true;
    stopAnimation = true;
    
    orbStatus = DISAPPEAR;
    delay_curr = delay_animFrame;
    
}

void ReputationOrb::collect(CCPoint touchLoc)
{
    collected = true;
    stopAnimation = true;
    
    orbSprite->retain();
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(orbSprite);
    GameHUD::getThis()->addChild(orbSprite);
    orbSprite->setPosition(touchLoc);
    orbSprite->autorelease();
    
    CCCallFuncN* callback = CCCallFuncN::create(this, callfuncN_selector(ReputationOrb::collectComplete));
    callback->retain();
    
    CCPoint diff = ccpSub(ccp(100, 686), touchLoc);
    float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
    float movementSpeed = 600.0f;
    
    CCSequence* runAction = CCSequence::createWithTwoActions(CCMoveBy::create(dist / movementSpeed, diff), callback);
    runAction->retain();
    orbSprite->runAction(runAction);
    CC_SAFE_RELEASE(runAction);
    CC_SAFE_RELEASE(callback);
    
    SoundtrackManager::PlaySFX("coin pickup.wav");
    //CC_SAFE_RELEASE(bezier);
}