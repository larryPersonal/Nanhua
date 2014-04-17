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
    
    bubbleTexture = CCTextureCache::sharedTextureCache()->addImage("emotionicon_anima.png");
    
    x_frameno = 0;
    x_maxframeno = 1;
}

void SpeechBubble::createSpeechBubble()
{
    background = CCSprite::create("bubble.png");
    
    orig_sizeX = background->getContentSize().width;
    background->setAnchorPoint(ccp(0.5, 0));
    
    contentNode = CCNode::create();
    
  //  float bbWidth = background->boundingBox().getMaxX() - background->boundingBox().getMinX();
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
    
    rescale();
}

void SpeechBubble::clearContent()
{
    contentNode->removeAllChildren();
}

void SpeechBubble::show(float time)
{
    if (isHideScheduled)
        this->unschedule(schedule_selector(SpeechBubble::hide));
    
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

void SpeechBubble::displayTransportBubble()
{
    y_offset = 0;
    
    x_frameno = 0;
    x_maxframeno = 2;
    
    bubbleRect = CCRectMake(0, y_offset * frameHeight,  frameWidth, frameHeight);
    
    CCSprite* bubbleSprite = CCSprite::createWithTexture(bubbleTexture, bubbleRect);
    addContent(bubbleSprite, CCPointZero);
}