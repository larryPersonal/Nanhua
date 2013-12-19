//
//  ProgressBar.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 17/4/13.
//
//

#include "ProgressBar.h"

ProgressBar::~ProgressBar()
{
}

void ProgressBar::createProgressBar(CCRect bgBodyRect, CCRect barOffsetRect,
                                    const char* bgBodyFileName,
                                    const char* bgLeftFileName,
                                    const char* bgRightFileName,
                                    const char* barFileName)
{
    // ProgressBar is the parent node, set its position/size
    this->setPosition(bgBodyRect.origin);
    this->setContentSize(bgBodyRect.size);
    barFillOffsetRect = barOffsetRect;
    
    // GL_TEX_PARAMS for repeating textures
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    
    // Create bgBody sprite, which is repeated and scaled
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(bgBodyFileName);
    texture->setTexParameters(&params);
    bgBody = CCSprite::createWithTexture(texture);
    bgBody->setAnchorPoint(CCPointZero);
    bgBody->setContentSize(bgBodyRect.size);
    bgBody->setTextureRect(CCRectMake(0, 0,
                                      bgBodyRect.size.width * (texture->getPixelsHigh() / bgBodyRect.size.height),
                                      texture->getPixelsHigh()));
    bgBody->setScale(bgBodyRect.size.height / texture->getPixelsHigh());
    
    // Create bgLeft, which is stretched and scaled
    bgLeft = CCSprite::create(bgLeftFileName);
    bgLeft->setAnchorPoint(CCPointZero);
    bgLeft->setScale(bgBodyRect.size.height / bgLeft->boundingBox().size.height);
    
    // Create bgRight, which is stretched and scaled
    bgRight = CCSprite::create(bgRightFileName);
    bgRight->setAnchorPoint(CCPointZero);
    bgRight->setScale(bgBodyRect.size.height / bgRight->boundingBox().size.height);
    
    // Create barFill, which is repeated and scaled
    texture = CCTextureCache::sharedTextureCache()->addImage(barFileName);
    texture->setTexParameters(&params);
    barFill = CCSprite::createWithTexture(texture);
    barFill->setAnchorPoint(CCPointZero);
    barFill->setContentSize(barFillOffsetRect.size);
    barFillTexSize = texture->getContentSizeInPixels();
    barFill->setScale(barFillOffsetRect.size.height / barFillTexSize.height);
    barFillScale = barFillTexSize.height / barOffsetRect.size.height;
    
    // Add them to this object
    this->addChild(bgBody);
    
    this->addChild(bgLeft);
    this->addChild(bgRight);
    this->addChild(barFill);
    
    // Initialize
    setValue(0.0f);
    reposition();
}

void ProgressBar::reposition()
{
    // Set positions relative to this object
    bgBody->setPosition(this->getPosition());
    bgLeft->setPosition(this->getPosition());
    bgRight->CCNode::setPosition(this->getContentSize().width - bgRight->boundingBox().size.width,
                                 this->getPositionY());
    
    barFill->CCNode::setPosition(this->getPositionX() + barFillOffsetRect.origin.x,
                                 this->getPositionY() + barFillOffsetRect.origin.y);
}

float ProgressBar::getValue()
{
    return currentValue;
}

void ProgressBar::setValue(float value)
{
    // Clamp between 0.0f and 1.0f
    if (value < 0.0f)
        value = 0.0f;
    else if (value > 1.0f)
        value = 1.0f;
    currentValue = value;
    
    // Set the fill rect, relative to its own texture size,
    // as barFill already has its own scale
    float newFillWidth = currentValue * barFillOffsetRect.size.width;
    barFill->setTextureRect(CCRectMake(0, 0,
                                       newFillWidth * barFillScale,
                                       barFillTexSize.height));
}



ProgressBar* ProgressBar::create()
{
    ProgressBar *pRet = new ProgressBar();
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


