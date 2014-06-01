//
//  AnimatedDialogue.cpp
//  PopulationTest
//
//  Created by admin on 1/6/14.
//
//

#include "AnimatedDialogue.h"
#include "Senario.h"

AnimatedDialogue* AnimatedDialogue::create(std::string spritePicture, bool hasFadeIn, bool hasFadeOut)
{
    AnimatedDialogue* pRet = new AnimatedDialogue(spritePicture, hasFadeIn, hasFadeOut);
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

AnimatedDialogue::AnimatedDialogue(std::string spritePicture, bool hasFadeIn, bool hasFadeOut)
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
    
    setup(spritePicture);
}

AnimatedDialogue::~AnimatedDialogue()
{
    
}

void AnimatedDialogue::setup(std::string spritePicture)
{
    sprite = CCSprite::create(spritePicture.c_str());
    if(hasFadeInAnimation)
    {
        currentAlpha = 0;
        sprite->setOpacity((GLubyte) 0);
        fadeIn = true;
    }
}

void AnimatedDialogue::update(float dt)
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

void AnimatedDialogue::triggerFadeOut()
{
    if(hasFadeOutAnimation)
    {
        fadeOut = true;
    }
}

CCSprite* AnimatedDialogue::getSprite()
{
    return sprite;
}