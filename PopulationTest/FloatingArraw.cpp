//
//  FloatingArraw.cpp
//  PopulationTest
//
//  Created by admin on 21/4/14.
//
//

#include "FloatingArraw.h"
#include "TutorialManager.h"
#include "BuildScroll.h"

FloatingArraw* FloatingArraw::SP;

FloatingArraw* FloatingArraw::getThis()
{
    return SP;
}

FloatingArraw::FloatingArraw()
{
    FloatingArraw::SP = this;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    frameHeight = 64;
    frameWidth = 64;
    
    arrowTexture = CCTextureCache::sharedTextureCache()->addImage("arrowupanddown.png");
    
    y_offset = 0;
    int x_offset = 0;
    
    //assume left to right. ALWAYS make the leftmost frame the first frame.
    //hopefully the artist will not try to make
    x_frameno = 0;
    x_maxframeno = 11;
    
    layer = 0;
    
    y_offset = x_frameno / 4.0f;
    x_offset = ((int)x_frameno) % 4;
    
    arrowRect = CCRectMake(x_offset * frameWidth, y_offset * frameHeight,  frameWidth, frameHeight);
    
    //set the thing to the first frame.
    arrow = CCSprite::createWithTexture(arrowTexture, arrowRect);
    arrow->setAnchorPoint(ccp(0.5, 0.5));
    arrow->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    arrow->setVisible(false);
    arrow->retain();
    
    TutorialManager::getThis()->addChild(arrow, 5);
    TutorialManager::getThis()->schedule(schedule_selector(FloatingArraw::update), 1.0f/120.0f);
}

FloatingArraw::~FloatingArraw()
{
    FloatingArraw::SP = NULL;
    CC_SAFE_RELEASE(arrow);
}

void FloatingArraw::update(float dt)
{
    if (FloatingArraw::getThis()->delay_curr > 0)
    {
        FloatingArraw::getThis()->delay_curr -= dt;
    }
    else
    {
        FloatingArraw::getThis()->x_frameno++;
        if (FloatingArraw::getThis()->x_frameno >= FloatingArraw::getThis()->x_maxframeno)
        {
            FloatingArraw::getThis()->x_frameno = 0;
        }
        
        FloatingArraw::getThis()->y_offset = FloatingArraw::getThis()->x_frameno / 4.0f;
        int x_offset = ((int)FloatingArraw::getThis()->x_frameno) % 4;
        
        FloatingArraw::getThis()->arrowRect.setRect(x_offset * FloatingArraw::getThis()->frameWidth, FloatingArraw::getThis()->y_offset * FloatingArraw::getThis()->frameHeight, FloatingArraw::getThis()->frameWidth, FloatingArraw::getThis()->frameHeight);
        FloatingArraw::getThis()->arrow->setTextureRect(FloatingArraw::getThis()->arrowRect);
        
        FloatingArraw::getThis()->delay_curr = FloatingArraw::getThis()->delay_animFrame;
    }
}

void FloatingArraw::showArrow(CCPoint pos, float ang, float scale, int arrowLayer)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
    arrow->setPosition(ccp(hw + pos.x, hh + pos.y));
    arrow->setRotation(ang);
    arrow->setScale(scale);
    arrow->setVisible(true);
    
    if(arrowLayer == 1)
    {
        if(BuildScroll::getThis() != NULL)
        {
            if(layer == 0)
            {
                TutorialManager::getThis()->removeChild(arrow);
                BuildScroll::getThis()->scrollArea->addItem(arrow, ccp(pos.x, pos.y));
            }
            else
            {
                BuildScroll::getThis()->scrollArea->removeI(arrow);
                BuildScroll::getThis()->scrollArea->addItem(arrow, ccp(pos.x, pos.y));
            }
            layer = arrowLayer;
        }
    }
    else if(arrowLayer == 0)
    {
        if(layer == 1)
        {
            if(BuildScroll::getThis() != NULL)
            {
                BuildScroll::getThis()->scrollArea->removeI(arrow);
                TutorialManager::getThis()->addChild(arrow);
            }
            /*
            else
            {
                CCLog("test4");
                //set the thing to the first frame.
                arrow = CCSprite::createWithTexture(arrowTexture, arrowRect);
                arrow->setAnchorPoint(ccp(0.5, 0.5));
                arrow->setPosition(ccp(hw + pos.x, hh + pos.y));
                arrow->setRotation(ang);
                arrow->retain();
                
                TutorialManager::getThis()->addChild(arrow, 5);
                TutorialManager::getThis()->schedule(schedule_selector(FloatingArraw::update), 1.0f/120.0f);
            }
            */
            layer = arrowLayer;
        }
    }
}

void FloatingArraw::hideArrow()
{
    arrow->setVisible(false);
}