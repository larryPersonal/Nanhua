//
//  NotificationString.cpp
//  PopulationTest
//
//  Created by admin on 21/4/14.
//
//

#include "NotificationString.h"
#include "GameHUD.h"

NotificationString* NotificationString::create(std::string input, float startY)
{
    NotificationString* pRet = new NotificationString(input, startY);
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

NotificationString::NotificationString(std::string input, float startY)
{
    borderX = 200;
    borderY = 200;
    this->startY = borderY + startY;
    
    isUp = false;
    
    notificationLabel = CCLabelTTF::create(input.c_str(), "Arial", 24);
    notificationLabel->setAnchorPoint(ccp(1, 1));
    notificationLabel->setOpacity((GLubyte) 0);
    GameHUD::getThis()->addChild(notificationLabel);
}

NotificationString::~NotificationString()
{
    
}

void NotificationString::scheduleSlideIn()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    notificationLabel->setPosition(ccp(screenSize.width + borderX, screenSize.height - startY));
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
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float posX = notificationLabel->getPositionX();
    posX -= 20;
    
    if(posX <= screenSize.width)
    {
        posX = screenSize.width;
        GameHUD::getThis()->slideIn = false;
    }
    
    float opacity = 255.0f * ((borderX + screenSize.width - posX) / borderX);
    
    notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
    notificationLabel->setOpacity((GLubyte) opacity);
}

void NotificationString::slideUp(float dt, int id)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    CCArray* eventLabels = GameHUD::getThis()->eventLabels;
    float height = 0;
    
    for (int i = 0; i < id; i++)
    {
        NotificationString* ns = (NotificationString*) eventLabels->objectAtIndex(i);
        height += ns->notificationLabel->boundingBox().size.height;
    }
    
    float posY = notificationLabel->getPositionY();
    posY += 10;
    
    if(posY >= screenSize.height - borderY - height)
    {
        posY = screenSize.height - borderY - height;
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
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float posX = notificationLabel->getPositionX();
    posX += 20;
    
    if(posX >= screenSize.width + borderX)
    {
        posX = screenSize.width + borderX;
        
        float opacity = 255.0f * ((borderX + screenSize.width - posX) / borderX);
        
        notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
        notificationLabel->setOpacity((GLubyte) opacity);
        
        GameHUD::getThis()->slideUp = true;
        GameHUD::getThis()->slideOut = false;
        
        GameHUD::getThis()->removeChild(notificationLabel);
        GameHUD::getThis()->eventLabels->removeObject(this);
        return true;
    }
    else
    {
        float opacity = 255.0f * ((borderX + screenSize.width - posX) / borderX);
    
        notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
        notificationLabel->setOpacity((GLubyte) opacity);
        return false;
    }
}

void NotificationString::update(float dt)
{
    CCLog("test my bad: %f", cumulativeTime);
    cumulativeTime += dt;
    if(cumulativeTime > 30)
    {
        if(!GameHUD::getThis()->slideUp && !GameHUD::getThis()->slideOut)
        {
            GameHUD::getThis()->slideOut = true;
        }
    }
}