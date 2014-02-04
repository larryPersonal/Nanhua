//
//  PathFinder.h
//  PopulationTest
//
//  Created by Serious Games on 3/4/13.
//
//

#ifndef __PopulationTest__PathFinder__
#define __PopulationTest__PathFinder__

#include "cocos2d.h"
#include "PathfindingNode.h"


class PathFinder
{
    CCArray* openList;
    CCArray* closedList;
    
    CCPoint* destination;
    CCPoint* source;
    
    
public:
    CCPoint closest;
    int lowest_h;
    
    PathFinder();
    ~PathFinder();
    float manhattanDist(CCPoint* from, CCPoint* to);
    
    CCArray* getReachableTiles(PathfindingNode* fromTile, CCPoint* toTile);
    
    CCArray* makePath(cocos2d::CCPoint *fromTile, cocos2d::CCPoint *toTile);
    
    PathfindingNode* isOnList(CCPoint tilePos, CCArray* list);
    
    void initializeWithLowestCostNode(CCPoint target);
    
    bool isReachable(CCPoint* tilePos);
};

#endif /* defined(__PopulationTest__PathFinder__) */
