//
//  AnimatedSprite.cpp
//  PopulationTest
//
//  Created by admin on 27/3/14.
//
//

#include "AnimatedSprite.h"
#include "Senario.h"

AnimatedSprite* AnimatedSprite::create(std::string spritePicture, bool hasFadeIn, bool hasFadeOut)
{
    AnimatedSprite* pRet = new AnimatedSprite(spritePicture, hasFadeIn, hasFadeOut);
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

AnimatedSprite::AnimatedSprite(std::string spritePicture, bool hasFadeIn, bool hasFadeOut)
{
    cumulativeTime = 0;
    hasFadeInAnimation = hasFadeIn;
    hasFadeOutAnimation = hasFadeOut;
    
    fadeInSpeed = 10;
    fadeOutSpeed = 10;
    
    fadeIn = false;
    fadeOut = false;
    stopFade = false;
    
    currentAlpha = 255;
    
    setup(spritePicture);
}

AnimatedSprite::~AnimatedSprite()
{
    
}

void AnimatedSprite::setup(std::string spritePicture)
{
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