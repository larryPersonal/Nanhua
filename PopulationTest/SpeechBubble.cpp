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
}

void SpeechBubble::createSpeechBubble()
{
    background = CCSprite::create("bubble.png");
    background->setScaleY(200 / background->getContentSize().width);
    
    orig_sizeX = background->getContentSize().width;
    background->setAnchorPoint(ccp(0.5, 0));
    
    contentNode = CCNode::create();
    
  //  float bbWidth = background->boundingBox().getMaxX() - background->boundingBox().getMinX();
    float bbHeight = background->getContentSize().height;
    
    contentNode->setPositionY(bbHeight * 0.025f);
    
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

void SpeechBubble::addContent(SpeechMood s, CCPoint offset)
{
    
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
    
 //   contentNode->setPositionX(bbWidth * 0.5f);
    
    
}