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
    
    bool fade_out;
    bool fade_in;
    float target_opacity;
    
    bool clickToNext;
    
public:
    TutorialSlide();
    ~TutorialSlide();
    
    static TutorialSlide* create();
};

#endif
