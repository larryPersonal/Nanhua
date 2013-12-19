//
//  SpriteSolidCircle.h
//  PopulationTest
//
//  Created by Ong Heng Le on 14/5/13.
//
//

#ifndef __PopulationTest__SpriteSolidCircle__
#define __PopulationTest__SpriteSolidCircle__

#include "cocos2d.h"
using namespace cocos2d;

class SpriteSolidCircle:public cocos2d::CCNode
{
    
private:
    
    float radius;
    float scaleX;
    float scaleY;
    ccColor4F color;
    
public:
    
    SpriteSolidCircle();
    virtual void draw();
    
    void setRadius(float radius);
    void setScale(float scaleX, float scaleY);
    void setColor(ccColor4F color);
    
};

#endif /* defined(__PopulationTest__SpriteSolidCircle__) */
