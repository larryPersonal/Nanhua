//
//  SpriteSolidPoly.h
//  PopulationTest
//
//  Created by Ong Heng Le on 14/5/13.
//
//

#ifndef __PopulationTest__SpriteSolidPoly__
#define __PopulationTest__SpriteSolidPoly__

#include "cocos2d.h"
using namespace cocos2d;

class SpriteSolidPoly:public cocos2d::CCNode
{
    
private:
    
    std::vector<CCPoint> points;
    ccColor4F color;
    
public:
    
    SpriteSolidPoly();
    virtual void draw();
    
    void addVertex(CCPoint point);
    void setVertexAt(int index, float x, float y);
    void setColor(ccColor4F color);
    
};

#endif /* defined(__PopulationTest__SpriteSolidPoly__) */
