//
//  SpeechBubble.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 7/5/13.
//
//

#include "SpeechBubble.h"

SpeechBubble::SpeechBubble()
{
    background = NULL;
    contentNode = NULL;
    
    isHideScheduled = false;
    
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    frameHeight = 64;
    frameWidth = 64;
    
    CCSprite* rawSprite = CCSprite::createWithSpriteFrameName("emotionicon_anima.png");
    bubbleTexture = rawSprite->getTexture();
    // bubbleTexture = CCTextureCache::sharedTextureCache()->addImage("emotionicon_anima.png");
    
    x_frameno = 0;
    x_maxframeno = 1;
    
    numberOfElementInOneRow = 0;
    startElementId = 0;
    
    x_offset = 0;
    y_offset = 0;
}

void SpeechBubble::createSpeechBubble()
{
    background = CCSprite::createWithSpriteFrameName("bubble.png");
    
    orig_sizeX = background->getContentSize().width;
    background->setAnchorPoint(ccp(0.5, 0));
    
    contentNode = CCNode::create();
    
    // float bbWidth = background->boundingBox().getMaxX() - background->boundingBox().getMinX();
    float bbHeight = background->getContentSize().height;
    
    contentNode->setPositionY(bbHeight * 0.55f);
    
    this->addChild(background);
    this->addChild(contentNode);
    
    this->setVisible(false);
}

//Probably not be using this anymore, but keeping it around.
void SpeechBubble::addContent(CCNode* node, CCPoint offset)
{
    node->setPosition(offset);
    node_sizeX = node->getContentSize().width;
    contentNode->addChild(node);
    
    hasAnimation = false;
    
    emotionSprite = NULL;
    
    rescale();
}

void SpeechBubble::addContent(std::string texture, CCPoint offset, int maxFrameNo, int numberOfFrame, int startID, float frameW, float frameH)
{
    CCSprite* rawSprite = CCSprite::createWithSpriteFrameName(texture.c_str());
    bubbleTexture = rawSprite->getTexture();
    // bubbleTexture = CCTextureCache::sharedTextureCache()->addImage(texture.c_str());
    
    x_maxframeno = maxFrameNo;
    numberOfElementInOneRow = numberOfFrame;
    startElementId = startID;
    
    frameWidth = frameW;
    frameHeight = frameH;
    
    x_frameno = 0;
    hasAnimation = true;
    
    x_offset = startElementId % numberOfElementInOneRow;
    y_offset = startElementId / numberOfElementInOneRow;
    
    bubbleRect = CCRectMake(0, 0, frameWidth, frameHeight);
    
    emotionSprite = CCSprite::createWithTexture(bubbleTexture, bubbleRect);
    emotionSprite->setPosition(offset);
    node_sizeX = emotionSprite->getContentSize().width;
    contentNode->addChild(emotionSprite);
    
    rescale();
    
    this->setVisible(true);
    this->schedule(schedule_selector(SpeechBubble::update), 1.0f/120.0f);
}

void SpeechBubble::clearContent()
{
    if(hasAnimation)
    {
        this->unschedule(schedule_selector(SpeechBubble::update));
    }
    contentNode->removeAllChildren();
}

void SpeechBubble::show(float time)
{
    if (isHideScheduled)
    {
        this->unschedule(schedule_selector(SpeechBubble::hide));
    }
    
    this->setVisible(true);
    this->scheduleOnce(schedule_selector(SpeechBubble::hide), time);
    isHideScheduled = true;
}

void SpeechBubble::hide()
{
    isHideScheduled = false;
    this->setVisible(false);
}

void SpeechBubble::rescale()
{
    background->setScaleX(node_sizeX * 1.5f /orig_sizeX);
    //background->setContentSize(((CCNode*)contentNode->getChildren()->objectAtIndex(0))->boundingBox().getWidth);
   
    // float bbWidth = background->boundingBox().getMaxX() - background->boundingBox().getMinX();
    
    // contentNode->setPositionX(bbWidth * 0.5f);
}

void SpeechBubble::update(float dt)
{
    if(hasAnimation)
    {
        if (delay_curr > 0)
        {
            delay_curr -= dt;
        }
        else
        {
            x_frameno++;
            if (x_frameno >= x_maxframeno)
            {
                x_frameno = 0;
            }
            
            x_offset = (x_frameno + startElementId) % numberOfElementInOneRow;
            y_offset = (x_frameno + startElementId) / numberOfElementInOneRow;
            
            bubbleRect.setRect(x_offset * frameWidth, y_offset * frameHeight, frameWidth, frameHeight);
            emotionSprite->setTextureRect(bubbleRect);
            
            delay_curr = delay_animFrame;
        }
    }
}