//
//  NotificationString.cpp
//  PopulationTest
//
//  Created by admin on 21/4/14.
//
//

#include "NotificationString.h"
#include "GameHUD.h"

NotificationString* NotificationString::create(std::string input, float borderY)
{
    NotificationString* pRet = new NotificationString(input, borderY);
    if(pRet)
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

NotificationString::NotificationString(std::string input, float borderY)
{
    borderX = 200;
    this->borderY = borderY;
    this->targetY = borderY;
    
    isUp = false;
    
    notificationLabel = CCLabelTTF::create(input.c_str(), "Arial", 16);
    notificationLabel->setAnchorPoint(ccp(0, 0));
    notificationLabel->setOpacity((GLubyte) 0);
    GameHUD::getThis()->addChild(notificationLabel);
}

NotificationString::~NotificationString()
{
    
}

void NotificationString::scheduleSlideIn()
{
    notificationLabel->setPosition(ccp(0 - borderX, 0));
    notificationLabel->setOpacity((GLubyte) 0);
}

void NotificationString::scheduleSlideUp()
{
    
}

void NotificationString::scheduleSlideOut()
{
    
}

void NotificationString::slideIn(float dt)
{
    float posX = notificationLabel->getPositionX();
    posX += 20;
    
    if(posX >= 0)
    {
        posX = 0;
        GameHUD::getThis()->slideIn = false;
    }
    
    float opacity = 255.0f * ((borderX + posX) / borderX);
    
    notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
    notificationLabel->setOpacity((GLubyte) opacity);
}

void NotificationString::slideUp(float dt, int id)
{
    float posY = notificationLabel->getPositionY();
    posY += 10;
    
    if(posY >= targetY)
    {
        posY = targetY;
        isUp = false;
    }
    else
    {
        isUp = true;
    }
    
    notificationLabel->setPositionY(posY);
}

bool NotificationString::slideOut(float dt)
{
    float posX = notificationLabel->getPositionX();
    posX -= 20;
    
    if(posX <= 0 - borderX)
    {
        posX = 0 - borderX;
        
        float opacity = 255.0f * ((0 - posX) / borderX);
        
        notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
        notificationLabel->setOpacity((GLubyte) opacity);
        
        GameHUD::getThis()->slideOut = false;
        
        GameHUD::getThis()->removeChild(notificationLabel);
        GameHUD::getThis()->eventLabels->removeObject(this);
        return true;
    }
    else
    {
        float opacity = 255.0f * ((0 - posX) / borderX);
    
        notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
        notificationLabel->setOpacity((GLubyte) opacity);
        return false;
    }
}

void NotificationString::update(float dt)
{
    cumulativeTime += dt;
    if(cumulativeTime > 30)
    {
        if(!GameHUD::getThis()->slideUp && !GameHUD::getThis()->slideOut)
        {
            GameHUD::getThis()->slideOut = true;
        }
    }
}