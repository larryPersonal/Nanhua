//
//  BattleIcon.cpp
//  PopulationTest
//
//  Created by admin on 25/4/14.
//
//

#include "BattleIcon.h"

BattleIcon* BattleIcon::create(GameSprite* parentGS)
{
    BattleIcon* pRet = new BattleIcon(parentGS);
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

BattleIcon::BattleIcon(GameSprite* parentGS)
{
    frameHeight = 64;
    frameWidth = 128;
    
    x_frameno = 0;
    x_maxframeno = 4;
    x_offset = 0;
    y_offset = 0;
    
    delay_animFrame = 0.1f;
    delay_curr = 0.1f;
    
    parent = parentGS;
    battleIconTexture = CCTextureCache::sharedTextureCache()->addImage("swordAnimation.png");
    battleIconRect = CCRectMake(0, 0, frameWidth, frameHeight);
    battleSprite = CCSprite::createWithTexture(battleIconTexture, battleIconRect);
    adjustPosition();
    battleSprite->setVisible(false);
    parent->spriteRep->addChild(battleSprite);
}

BattleIcon::~BattleIcon()
{
}

void BattleIcon::update(float deltaTime)
{
    if (delay_curr > 0)
    {
        delay_curr -= deltaTime;
    }
    else
    {
        x_frameno++;
        if (x_frameno >= x_maxframeno)
        {
            x_frameno = 0;
        }
        
        x_offset = 0;
        y_offset = x_frameno;
        
        battleIconRect.setRect(x_offset * frameWidth, y_offset * frameHeight, frameWidth, frameHeight);
        battleSprite->setTextureRect(battleIconRect);
        
        delay_curr = delay_animFrame;
    }
}

void BattleIcon::show()
{
    battleSprite->setVisible(true);
}

void BattleIcon::hide()
{
    battleSprite->setVisible(false);
}

void BattleIcon::adjustPosition()
{
    if(parent->currentDir == "UL")
    {
        battleSprite->setPosition(ccp(0, 80));
    }
    else if(parent->currentDir == "DL")
    {
        battleSprite->setPosition(ccp(0, 40));
    }
    else if(parent->currentDir == "UR")
    {
        battleSprite->setPosition(ccp(115, 80));
    }
    else if(parent->currentDir == "DR")
    {
        battleSprite->setPosition(ccp(115, 40));
    }
}