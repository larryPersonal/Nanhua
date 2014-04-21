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
    posX -= 10;
    
    if(posX <= screenSize.width)
    {
        posX = screenSize.width;
    }
    
    float opacity = 255.0f * ((borderX + screenSize.width - posX) / borderX);
    
    notificationLabel->setPosition(ccp(posX, notificationLabel->getPositionY()));
    notificationLabel->setOpacity((GLubyte) opacity);
}

void NotificationString::slideUp(float dt)
{
    
}

void NotificationString::slideOut(float dt)
{
    
}