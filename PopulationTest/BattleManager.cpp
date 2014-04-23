//
//  BattleManager.cpp
//  PopulationTest
//
//  Created by admin on 23/4/14.
//
//

#include "BattleManager.h"
#include "GameScene.h"

BattleManager::BattleManager()
{
    battlePairs = CCArray::create();
    battlePairs->retain();
}

BattleManager::~BattleManager()
{
    battlePairs->removeAllObjects();
    CC_SAFE_RELEASE(battlePairs);
}

void BattleManager::addBattlePair(GameSprite *theSoldier,  GameSprite *theBandit)
{
    battlePairs->addObject(BattlePair::create(theSoldier, theBandit));
}

void BattleManager::update(float deltaTime)
{
    /*
    for (int i = 0; i < battlePairs->count(); i++)
    {
        // get the battle pair first
        BattlePair* bp = (BattlePair*) battlePairs->objectAtIndex(i);
        
        // each battle pair will check the behaviour
        GameSprite* soldier = bp->soldier;
        GameSprite* bandit = bp->bandit;
        
        CCPoint soldierPos = soldier->getWorldPosition();
        soldierPos = GameScene::getThis()->mapHandler->tilePosFromLocation(soldierPos);
        
        CCPoint banditPos = bandit->getWorldPosition();
        banditPos = GameScene::getThis()->mapHandler->tilePosFromLocation(banditPos);
        
        int tempDistance = bandit->getPathDistance(banditPos, soldierPos);
        
        if(tempDistance <= 2)
        {
            bandit->stopAction = true;
            soldier->stopAction = true;
        }
        else
        {
            bandit->enermy = NULL;
            bandit->combatState = C_IDLE;
            
        }
            
            // base on the direction of the sprite, get the check attacking area of that sprite.
            CCPoint temp = CCPointZero;
            CCPoint infront = CCPointZero;
            CCPoint back = CCPointZero;
            CCPoint left = CCPointZero;
            CCPoint right = CCPointZero;
            if(currentDir.compare("DR") == 0){
                temp.x = currPos.x + 1;
                temp.y = currPos.y;
                
                back.x = currPos.x -1;
                back.y = currPos.y;
                
                left.x = currPos.x;
                left.y = currPos.y - 1;
                
                right.x = currPos.x;
                right.y = currPos.y + 1;
            } else if(currentDir.compare("UR") == 0){
                temp.x = currPos.x;
                temp.y = currPos.y - 1;
                
                back.x = currPos.x;
                back.y = currPos.y + 1;
                
                left.x = currPos.x - 1;
                left.y = currPos.y;
                
                right.x = currPos.x + 1;
                right.y = currPos.y;
            } else if(currentDir.compare("DL") == 0){
                temp.x = currPos.x;
                temp.y = currPos.y + 1;
                
                back.x = currPos.x;
                back.y = currPos.y - 1;
                
                left.x = currPos.x + 1;
                left.y = currPos.y;
                
                right.x = currPos.x - 1;
                right.y = currPos.y;
            } else if(currentDir.compare("UL") == 0){
                temp.x = currPos.x - 1;
                temp.y = currPos.y;
                
                back.x = currPos.x + 1;
                back.y = currPos.y;
                
                left.x = currPos.x;
                left.y = currPos.y + 1;
                
                right.x = currPos.x;
                right.y = currPos.y - 1;
            }
            infront = temp;
            
            if((villagerClass == V_SOLDIER) && !tryEscape)
            {
                CCArray* allSpritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
                
                if(!hasBandit(allSpritesOnMap, infront))
                {
                    if(hasBandit(allSpritesOnMap, left))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DL");
                        }
                    }
                    else if(hasBandit(allSpritesOnMap, right))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("UR");
                        }
                    }
                    else if(hasBandit(allSpritesOnMap, back))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DR");
                        }
                    }
                    else
                    {
                        // no enermy in all the directions, resume find enermy.
                        currAction = IDLE;
                        combatState = C_IDLE;
                        stopAction = false;
                        return;
                    }
                }
            }
            
            if ((villagerClass == V_BANDIT) && !tryEscape)
            {
                CCArray* allSpritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
                
                if(!hasSoldier(allSpritesOnMap, infront))
                {
                    if(hasSoldier(allSpritesOnMap, left))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DL");
                        }
                    }
                    else if(hasSoldier(allSpritesOnMap, right))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DR");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("UR");
                        }
                    }
                    else if(hasSoldier(allSpritesOnMap, back))
                    {
                        if(currentDir.compare("DR") == 0)
                        {
                            changeAnimation("UL");
                        }
                        else if(currentDir.compare("UR") == 0)
                        {
                            changeAnimation("DL");
                        }
                        else if(currentDir.compare("DL") == 0)
                        {
                            changeAnimation("UR");
                        }
                        else if(currentDir.compare("UL") == 0)
                        {
                            changeAnimation("DR");
                        }
                    }
                    else
                    {
                        // not enermy in all directions, resume the finding enermy process
                        stopAction = false;
                        combatState = C_IDLE;
                        if(enermy != NULL && enermy->enermy != this)
                        {
                            enermy = NULL;
                            currAction = IDLE;
                        }
                        return;
                    }
                }
            }
            
            // only soldiers and bandits are involved in the combat.
            if((villagerClass == V_SOLDIER || villagerClass == V_BANDIT) && !tryEscape)
            {
                bool hasEnermy = false;
                
                if(enermy != NULL)
                {
                    CCPoint enermyTile = enermy->currPos;
                    if(temp.x == enermyTile.x && temp.y == enermyTile.y)
                    {
                        hasEnermy = true;
                    }
                }
                
                if(hasEnermy)
                {
                    cumulativeTime_attack += dt;
                }
                else
                {
                    cumulativeTime_attack = 0;
                }
                
                if(cumulativeTime_attack >= possessions->attack_cooldown)
                {
                    int diff = possessions->attack_power_max - possessions->attack_power_min;
                    
                    if(temp.x == enermy->currPos.x && temp.y == enermy->currPos.y)
                    {
                        int random_number = rand() % diff;
                        int damage = possessions->attack_power_min + random_number;
                        enermy->damaged(damage);
                        
                        if(villagerClass == V_BANDIT)
                        {
                            playAttackAction();
                        }
                    }
                    
                    cumulativeTime_attack = 0;
                }
                
            }
        }
    }
    */
}