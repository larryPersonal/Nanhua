//
//  BattlePair.cpp
//  PopulationTest
//
//  Created by admin on 23/4/14.
//
//

#include "BattlePair.h"

BattlePair* BattlePair::create(GameSprite* theSoldier, GameSprite* theBandit)
{
    BattlePair* pRet = new BattlePair(theSoldier, theBandit);
    
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

BattlePair::BattlePair(GameSprite* theSoldier, GameSprite* theBandit)
{
    soldier = theSoldier;
    bandit = theBandit;
}

BattlePair::~BattlePair()
{
    
}