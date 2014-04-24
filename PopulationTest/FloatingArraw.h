//
//  FloatingArraw.h
//  PopulationTest
//
//  Created by admin on 21/4/14.
//
//

#ifndef PopulationTest_FloatingArraw_h
#define PopulationTest_FloatingArraw_h

#include "cocos2d.h"
using namespace cocos2d;

class FloatingArraw
{
public:
    static FloatingArraw* SP;
    
    CCSprite* arrow;
    
    // this part is for the frame animation
    int frameHeight;
    int frameWidth;
    
    CCTexture2D* arrowTexture;
    CCRect arrowRect;
    int x_frameno;
    int x_maxframeno;
    int y_offset;
    
    float delay_animFrame;
    float delay_curr;
    
    int layer;

public:
    FloatingArraw();
    ~FloatingArraw();
    
    static FloatingArraw* getThis();
    
    void update(float);
    
    void showArrow(CCPoint, float, float, int arrowLayer = 0);
    void hideArrow();
};

#endif
