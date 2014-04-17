//
//  BanditsAttackHandler.cpp
//  PopulationTest
//
//  Created by admin on 6/3/14.
//
//

#include "BanditsAttackHandler.h"
#include "GameHUD.h"

BanditsAttackHandler::BanditsAttackHandler()
{
    initialize();
}

BanditsAttackHandler::~BanditsAttackHandler()
{
}

void BanditsAttackHandler::initialize()
{
    warMode = false;
    banditsLeft = 0;
    cumulativeTime = 0;
    tresholdTime = 0;
}

void BanditsAttackHandler::update(float dt)
{
    if(warMode)
    {
        if (banditsLeft <= 0)
        {
            return;
        }
        
        cumulativeTime += dt;
        
        if(cumulativeTime > tresholdTime)
        {
            cumulativeTime = 0;
            addBandit();
            tresholdTime = ((float) (rand() % banditsLeft + 1)) / 10.0f;
            banditsLeft--;
        }
    }
}

void BanditsAttackHandler::addBandit(){
    CCPoint target = CCPointMake(29,33);
    
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_BANDIT);

    /*
    int isMale = rand() % 2;
    
    if(isMale)
    {
        GameScene::getThis()->spriteHandler->addSpriteToMap(target, M_BANDIT);
    }
    else
    {
        GameScene::getThis()->spriteHandler->addSpriteToMap(target, F_BANDIT);
    }
    */
}

void BanditsAttackHandler::startWar()
{
    warMode = true;
    banditsLeft = 1;
    cumulativeTime = 0;
    tresholdTime = 0;
}

void BanditsAttackHandler::stopWar()
{
    warMode = false;
    banditsLeft = 0;
    cumulativeTime = 0;
    tresholdTime = 0;
    
    CCArray* allSprites = GameScene::getThis()->spriteHandler->allSprites;
    for (int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
        if(gs->villagerClass == V_BANDIT)
        {
            gs->tryEscape = true;
            //gs->escape();
        }
    }
}

void BanditsAttackHandler::alertAttack()
{
    
}