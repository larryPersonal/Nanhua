//
//  Slide.h
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#ifndef PopulationTest_Slide_h
#define PopulationTest_Slide_h

#include "Element.h"
using namespace cocos2d;
using namespace std;

class Slide: public CCObject
{
public:
    CCArray* elementList;
    
    bool isScene;
    std::string scene_src;
    
    bool hasVideo;
    std::string video_clip;
    
    bool clickToNext;
    
public:
    Slide();
    ~Slide();
    
    static Slide* create();
    
    virtual Slide* copyWithZone(CCZone *pZone);
    
    void addElement(Element*);
    CCObject* getElement(int index);
};



#endif
