//
//  BattleManager.h
//  PopulationTest
//
//  Created by admin on 23/4/14.
//
//

#ifndef PopulationTest_BattleManager_h
#define PopulationTest_BattleManager_h


#include "cocos2d.h"
#include "BattlePair.h"

using namespace cocos2d;

class BattleManager
{
public:
    CCArray* battlePairs;
    
public:
    BattleManager();
    ~BattleManager();
    
    void addBattlePair(GameSprite* theSoldier, GameSprite* theBandit);
    
    void update(float deltaTime);
};

#endif
