//
//  BattlePair.h
//  PopulationTest
//
//  Created by admin on 23/4/14.
//
//

#ifndef PopulationTest_BattlePair_h
#define PopulationTest_BattlePair_h


#include "cocos2d.h"
#include "sprite.h"

using namespace cocos2d;

class BattlePair : public CCObject
{
public:
    GameSprite* soldier;
    GameSprite* bandit;
    
public:
    static BattlePair* create(GameSprite* theSoldier, GameSprite* theBandit);
    BattlePair(GameSprite* theSoldier, GameSprite* theBandit);
    ~BattlePair();
};


#endif
