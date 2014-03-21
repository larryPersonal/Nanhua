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
    
    CCPoint destination;
    CCPoint source;
    
    
public:
    CCPoint closest;
    int lowest_h;
    
    
    
    PathFinder();
    ~PathFinder();
    float manhattanDist(CCPoint* from, CCPoint* to);
    void setDestination(CCPoint dest);
    void setSource(CCPoint src);
    
    CCArray* getReachableTiles(PathfindingNode* fromTile, CCPoint* toTile, bool tryEscape);
    
    CCArray* makePath(cocos2d::CCPoint *fromTile, cocos2d::CCPoint *toTile);
    CCArray* makePathEscape(cocos2d::CCPoint *fromTile, cocos2d::CCPoint *toTile);
    
    PathfindingNode* isOnList(CCPoint tilePos, CCArray* list);
    
    void initializeWithLowestCostNode(CCPoint target, bool tryEscape);
    
    bool isReachable(CCPoint* tilePos, bool tryEscape);
};

#endif /* defined(__PopulationTest__PathFinder__) */
