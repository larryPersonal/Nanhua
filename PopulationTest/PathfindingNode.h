//
//  PathfindingNode.h
//  PopulationTest
//
//  Created by Serious Games on 21/3/13.
//
//

#ifndef __PopulationTest__PathfindingNode__
#define __PopulationTest__PathfindingNode__

#include "cocos2d.h"

using namespace cocos2d;

class PathfindingNode: public CCObject
{
public:
    CCPoint tilepos;
    PathfindingNode* parent;
    
    int F;
    int G;
    int H;
    
    
};

#endif /* defined(__PopulationTest__PathfindingNode__) */
