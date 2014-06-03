//
//  TutorialSlide.h
//  PopulationTest
//
//  Created by admin on 10/4/14.
//
//

#ifndef PopulationTest_TutorialSlide_h
#define PopulationTest_TutorialSlide_h

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class TutorialSlide : public CCObject
{
public:
    string speech;
    vector<string> commands;
    CCPoint target;
    bool cameraMove;
    CCPoint moveVector;
    bool dragonMove;
    
    bool highlight;
    bool deHighlight;
    string highlightBuilding;
    string deHighlightBuilding;
    
    bool fade_out;
    bool fade_in;
    float target_opacity;
    
    bool clickToNext;
    bool lockClick;
    
    float offY;
    int addVillager;
    int addBandit;
    
    bool hide;
    bool show;
    
    bool triggerDropToken;
    bool unlockAll;
    bool hideBubble;
    
    bool showArrow;
    bool hideArrow;
    
    float arrowX;
    float arrowY;
    float arrowAngle;
    float arrowScale;
    int arrowLayer;
    
    float contentOffX;
    float contentOffY;
    
    bool stopScroll;
    bool resumeScroll;
    int hideScroll;
    int showScroll;
    
    bool checkGranary;
    bool checkFarm;
    bool notFirst;
    
    bool showObjective;
    bool hideObjective;
    
    bool connectHouse;
    bool connectGranary;
    bool connectFarm;
    
    bool waitForVillager;
    
    float delay;
    
    bool clear;
    
public:
    TutorialSlide();
    ~TutorialSlide();
    
    static TutorialSlide* create();
};

#endif
