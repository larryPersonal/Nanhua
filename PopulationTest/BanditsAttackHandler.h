//
//  BanditsAttackHandler.h
//  PopulationTest
//
//  Created by admin on 5/3/14.
//
//

#ifndef PopulationTest_BanditsAttackHandler_h
#define PopulationTest_BanditsAttackHandler_h

#include "cocos2d.h"
#include "Sprite.h"


class BanditsAttackHandler
{
    static BanditsAttackHandler* SP;
    
public:
    int requiredPopulation;
    int requiredGranary;
    
    int banditsLeft;
    float cumulativeTime;
    float tresholdTime;
    
    bool warMode;
    
public:
    BanditsAttackHandler();
    ~BanditsAttackHandler();
    
    static BanditsAttackHandler* getThis();
    
    void initialize();
    
    void update (float);
    
    void addBandit();
    
    void startWar();
    void stopWar();
    
    void alertAttack();
};

#endif
