//
//  AnimatedSprite.cpp
//  PopulationTest
//
//  Created by admin on 27/3/14.
//
//

#include "AnimatedSprite.h"
#include "Senario.h"

AnimatedSprite* AnimatedSprite::create(std::string spritePicture, bool hasFadeIn, bool hasFadeOut, bool fromCache)
{
    AnimatedSprite* pRet = new AnimatedSprite(spritePicture, hasFadeIn, hasFadeOut, fromCache);
    if(pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_RELEASE(pRet);
        return NULL;
    }
}

AnimatedSprite::AnimatedSprite(std::string spritePicture, bool hasFadeIn, bool hasFadeOut, bool fromCache)
{
    cumulativeTime = 0;
    hasFadeInAnimation = hasFadeIn;
    hasFadeOutAnimation = hasFadeOut;
    
    fadeInSpeed = 20;
    fadeOutSpeed = 20;
    
    fadeIn = false;
    fadeOut = false;
    stopFade = false;
    
    currentAlpha = 255;
    
    setup(spritePicture, fromCache);
}

AnimatedSprite::~AnimatedSprite()
{
    
}

void AnimatedSprite::setup(std::string spritePicture, bool fromCache)
{
    if (fromCache)
        sprite = CCSprite::createWithSpriteFrameName(spritePicture.c_str());
    
    else
        sprite = CCSprite::create(spritePicture.c_str());
    if(hasFadeInAnimation)
    {
        currentAlpha = 0;
        sprite->setOpacity((GLubyte) 0);
        fadeIn = true;
    }
}

void AnimatedSprite::update(float dt)
{
    if(!stopFade)
    {
        if(fadeIn){
            currentAlpha += fadeInSpeed;
            if(currentAlpha > 255)
            {
                currentAlpha = 255;
                fadeIn = false;
            }
            sprite->setOpacity((GLubyte) currentAlpha);
        }
        
        if(fadeOut){
            currentAlpha -= fadeOutSpeed;
            if(currentAlpha < 0)
            {
                currentAlpha = 0;
                sprite->setOpacity((GLubyte) currentAlpha);
                fadeOut = false;
                Senario::getThis()->goNextSlide();
            } else {
                sprite->setOpacity((GLubyte) currentAlpha);
            }
        }
    }
}

void AnimatedSprite::triggerFadeOut()
{
    if(hasFadeOutAnimation)
    {
        fadeOut = true;
    }
}

CCSprite* AnimatedSprite::getSprite()
{
    return sprite;
}