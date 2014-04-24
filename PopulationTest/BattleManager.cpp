//
//  BattleManager.cpp
//  PopulationTest
//
//  Created by admin on 23/4/14.
//
//

#include "BattleManager.h"
#include "GameScene.h"

BattleManager* BattleManager::SP;
BattleManager::BattleManager()
{
    battlePairs = CCArray::create();
    battlePairs->retain();
    BattleManager::SP = this;
}

BattleManager::~BattleManager()
{
    battlePairs->removeAllObjects();
    CC_SAFE_RELEASE(battlePairs);
    BattleManager::SP = NULL;
}

BattleManager* BattleManager::getThis()
{
    return SP;
}

void BattleManager::addBattlePair(GameSprite *theSoldier,  GameSprite *theBandit)
{
    battlePairs->addObject(BattlePair::create(theSoldier, theBandit));
}

void BattleManager::update(float deltaTime)
{
    for (int i = 0; i < battlePairs->count(); i++)
    {
        BattlePair* bp = (BattlePair*) battlePairs->objectAtIndex(i);
        bp->update(deltaTime);
    }
}