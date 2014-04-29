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
#include "TutorialReader.h"
#include "Building.h"

using namespace cocos2d;
using namespace std;

enum Dragon_State {
    Dragon_Start = 0, D1T1=1, D1T2=2, D1T3=3, D1T4=4, D1T5=5, D2T1=6, D2T2=7, D2T3=8, D2T4=9, D2T5=10, D2T6=11, D2T7=12, D2T8=13,
    D3T1=14, D3T2=15, D3T3=16, D3T4=17, D3T5=18, D3T6=19, D3T7 =20, D3T8=21,
    D4T1=22, D4T2=23, D4T3=24, Dragon_End = 25
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
    
    float delay;
    
    float cumulativeTime;
    
    CCArray* slidesList;
    int curSlide;
    
    bool clickToNext;
    bool lockClick;
    
public:
    MiniDragon();
    ~MiniDragon();
    
    void finishDisplay();
    void update(float);
    
    void scheduleSenario();
    
    void move(CCPoint);
    
    void readTutorialFile();
    void playMiniDraggon();
    void clickNext();
    bool constructTutorialSlide();
    void moveCamera(CCPoint);
    
    void fadeInScreen(float);
    void fadeOutScreen(float);
    
    void highlightBuilding(string);
    void deHighlightBuilding(string);
    
    void showAllSpeech();
    void hideDragonGroup();
};

#endif
