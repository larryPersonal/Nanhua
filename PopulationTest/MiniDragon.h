//
//  MiniDragon.h
//  PopulationTest
//
//  Created by AIXUE on 1/4/14.
//
//

#ifndef PopulationTest_MiniDragon_h
#define PopulationTest_MiniDragon_h

#include "cocos2d.h"
#include <vector.h>
using namespace cocos2d;
using namespace std;

enum Dragon_State {
    Dragon_Start = 0, D1T1, D1T2, D1T3, D1T4, D1T5, D2T1, D2T2, D2T3, D2T4, D2T5, D2T6, D2T7, D2T8,
    D3T1, D3T2, D3T3, D3T4, D3T5, D3T6, D3T7, D3T8,
    D4T1, D4T2, D4T3, Dragon_End
};

class MiniDragon
{
public:
    CCSprite* dragon;
    CCSprite* bubble;
    
    Dragon_State ds;
    
    float startX;
    float startY;
    
    float offX;
    float offY;
    
    CCArray* animatedStringList;
    
    bool autoJump;
    
    float delay;
    
    float cumulativeTime;
    
public:
    MiniDragon();
    ~MiniDragon();
    
    void display();
    void finishDisplay();
    void update(float);
    
    void scheduleSenario(float dt);
    
    void move(CCPoint);
};

#endif
