//
//  BanditsAttackHandler.cpp
//  PopulationTest
//
//  Created by admin on 6/3/14.
//
//

#include "BanditsAttackHandler.h"
#include "GameHUD.h"

BanditsAttackHandler* BanditsAttackHandler::SP;

BanditsAttackHandler::BanditsAttackHandler()
{
    BanditsAttackHandler::SP = this;
    initialize();
}

BanditsAttackHandler::~BanditsAttackHandler()
{
    BanditsAttackHandler::SP = NULL;
}

BanditsAttackHandler* BanditsAttackHandler::getThis()
{
    return SP;
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
    CCPoint target = CCPointMake(39,60);
    
    SpriteHandler::getThis()->addSpriteToMap(target, V_BANDIT);

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
    
    CCArray* allSprites = SpriteHandler::getThis()->allSprites;
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