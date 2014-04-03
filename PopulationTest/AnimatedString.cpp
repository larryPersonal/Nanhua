//
//  AnimatedString.cpp
//  PopulationTest
//
//  Created by admin on 21/3/14.
//
//

#include "AnimatedString.h"

AnimatedString* AnimatedString::create(std::string labelString, float time, std::string font, float fontSize, float speed)
{
    AnimatedString* pRet = new AnimatedString(labelString, time, font, fontSize, speed);
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

AnimatedString::AnimatedString(std::string labelString, float time, std::string font, float fontSize, float speed)
{
    cumulativeTime = 0;
    triggerTime = time;
    currentAlpha = 255;
    flashIn = false;
    fadeInSpeed = speed;
    stopFade = false;
    setup(labelString, font, fontSize);
}

AnimatedString::~AnimatedString()
{
    
}

void AnimatedString::setup(std::string labelStr, std::string font, float fontSize)
{
    label = CCLabelTTF::create(labelStr.c_str(), font.c_str(), fontSize);
    label->setOpacity((GLubyte) 0);
}

void AnimatedString::update(float dt)
{
    if(!flashIn)
    {
        if(cumulativeTime >= triggerTime)
        {
            flashIn = true;
        }
        else
        {
            cumulativeTime += dt;
        }
    }
    else
    {
        if(!stopFade)
        {
            currentAlpha += fadeInSpeed;
            if(currentAlpha > 255)
            {
                currentAlpha = 255;
                stopFade = true;
            }
            label->setOpacity((GLubyte) currentAlpha);
        }
    }
}

CCLabelTTF* AnimatedString::getLabel()
{
    return label;
}