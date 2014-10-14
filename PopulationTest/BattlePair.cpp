//
//  BattlePair.cpp
//  PopulationTest
//
//  Created by admin on 23/4/14.
//
//

#include "BattleManager.h"
#include "BattlePair.h"
#include "GameScene.h"

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

void BattlePair::update(float deltaTime)
{
    CCPoint soldierPos = soldier->getWorldPosition();
    soldierPos = MapHandler::getThis()->tilePosFromLocation(soldierPos);
    
    CCPoint banditPos = bandit->getWorldPosition();
    banditPos = MapHandler::getThis()->tilePosFromLocation(banditPos);
    
    int tempDistance = bandit->getPathDistance(banditPos, soldierPos);
    
    if(tempDistance == 3)
    {
        
    }
    else if(tempDistance == 2)
    {
        if(!soldier->isMoving && !bandit->isMoving)
        {
            // startBattle;
            battle();
        }
    }
    else if(tempDistance < 2)
    {
        if(!soldier->isMoving && !bandit->isMoving)
        {
            // get the adjacent four tiles and let the soldier to go to a valid path adjacent tile
            CCPoint pointsToCheck[4];
            pointsToCheck[0] = CCPointMake(soldierPos.x - 1, soldierPos.y);
            pointsToCheck[1] = CCPointMake(soldierPos.x + 1, soldierPos.y);
            pointsToCheck[2] = CCPointMake(soldierPos.x, soldierPos.y - 1);
            pointsToCheck[3] = CCPointMake(soldierPos.x, soldierPos.y + 1);
            
            MapTile* validTile = NULL;
            
            for(int i = 0; i < 4; i++)
            {
                CCPoint adjacentTile = pointsToCheck[i];
                MapTile* targetTile = MapHandler::getThis()->getTileAt(adjacentTile.x, adjacentTile.y);
                if(targetTile->isPath)
                {
                    if(validTile == NULL)
                    {
                        validTile = targetTile;
                    }
                    else
                    {
                        int randomNumber = rand() % 2;
                        if(randomNumber == 1)
                        {
                            validTile = targetTile;
                        }
                    }
                }
            }
            
            if(soldier->CreatePath(soldierPos, ccp(validTile->xpos, validTile->ypos)))
            {
                soldier->followPath(true);
            }
        }
    }
    else
    {
        if(!soldier->isMoving && !bandit->isMoving)
        {
            if(soldier->CreatePath(soldierPos, banditPos) && bandit->CreatePath(banditPos, soldierPos))
            {
                soldier->followPath(true);
                bandit->followPath(true);
            }
            else if(soldier->CreatePath(soldierPos, banditPos))
            {
                soldier->followPath(true);
            }
            else if(bandit->CreatePath(banditPos, soldierPos))
            {
                bandit->followPath(true);
            }
            else
            {
                soldier->isInCombat = false;
                bandit->isInCombat = false;
                BattleManager::getThis()->battlePairs->removeObject(this);
            }
        }
        else if(!soldier->isMoving)
        {
            soldier->followPath(true);
        }
        else if(!bandit->isMoving)
        {
            bandit->followPath(true);
        }
    }
}

void BattlePair::battle()
{
    CCPoint soldierPos = soldier->getWorldPosition();
    soldierPos = MapHandler::getThis()->tilePosFromLocation(soldierPos);
    
    CCPoint banditPos = bandit->getWorldPosition();
    banditPos = MapHandler::getThis()->tilePosFromLocation(banditPos);
    
    if(soldierPos.x < banditPos.x && soldierPos.y == banditPos.y)
    {
        soldier->changeAnimation("DR");
        bandit->changeAnimation("UL");
    }
    else if(soldierPos.x > banditPos.x && soldierPos.y == banditPos.y)
    {
        soldier->changeAnimation("UL");
        bandit->changeAnimation("DR");
    }
    else if(soldierPos.x == banditPos.x && soldierPos.y < banditPos.y)
    {
        soldier->changeAnimation("DL");
        bandit->changeAnimation("UR");
    }
    else
    {
        soldier->changeAnimation("UR");
        bandit->changeAnimation("DL");
    }
}