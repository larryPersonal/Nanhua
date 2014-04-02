//
//  Narrator.h
//  PopulationTest
//
//  Created by admin on 2/4/14.
//
//

#ifndef PopulationTest_Narrator_h
#define PopulationTest_Narrator_h

#include "cocos2d.h"

using namespace cocos2d;

enum Narrator_State
{
    NS_START = 0, C1T1, C1T2, C1T3, NS_END = 4
};

class Narrator
{
public:
    CCSprite* textBackground;
    
    Narrator_State ns;
    
    float startX;
    float startY;
    
    float offX;
    float offY;
    
    CCArray* animatedStringList;
    
    float cumulativeTime;
    
public:
    Narrator();
    ~Narrator();
    
    void display();
    void finishDisplay();
    void update(float);
    
    void activateSprite(float dt);
};

#endif
