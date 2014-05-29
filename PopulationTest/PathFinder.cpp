//
//  PathFinder.cpp
//  PopulationTest
//
//  Created by Serious Games on 3/4/13.
//
//

#include "PathFinder.h"
#include "GameScene.h"
#include "GlobalHelper.h"
#include <cmath>

PathFinder::PathFinder()
{
    lowest_h = 9999;
    openList = CCArray::create();
    openList->retain();
    closedList = CCArray::create();
    closedList->retain();
    
    tileOpenList = CCArray::create();
    tileClosedList = CCArray::create();
}

PathFinder::~PathFinder()
{
    openList->removeAllObjects();
    CC_SAFE_RELEASE(openList);
    closedList->removeAllObjects();
    CC_SAFE_RELEASE(closedList);
    tileOpenList->removeAllObjects();
    CC_SAFE_RELEASE(tileOpenList);
    tileClosedList->removeAllObjects();
    CC_SAFE_RELEASE(tileClosedList);
}

float PathFinder::manhattanDist(CCPoint* from, CCPoint* to)
{
    return ( abs(from->x - to->x) + abs(from->y - to->y));
}

//IMPORTANT NOTE: source and destination may likely change on the fly. Therefore DO NOT USE REFERENCES, I cannot guarantee thread safety. 
void PathFinder::setSource(cocos2d::CCPoint src)
{
    source = src;
}

void PathFinder::setDestination(cocos2d::CCPoint dest)
{
    destination = dest;
}

PathfindingNode* PathFinder::isOnList(CCPoint tilePos, CCArray* list)
{
    if (!list || list->count() == 0){
        return NULL;
    }
    
    PathfindingNode* currNode = NULL;
    for (int i = 0; i < list->count(); ++i)
    {
		currNode = (PathfindingNode*)list->objectAtIndex(i);
        if ((currNode->tilepos.x == tilePos.x) && (currNode->tilepos.y == tilePos.y))
        {
			return currNode;
		}
	}
	return NULL;
}

bool PathFinder::isReachable(cocos2d::CCPoint *tilePos)
{
    MapHandler* handler = GameScene::getThis()->mapHandler;
    if (handler->isTilePosWithinBounds(*tilePos))
    {
        if (destination.x == tilePos->x &&
            destination.y == tilePos->y)
        {
            //the destination must always be reachable, in case someone wants to enter a building.
            MapTile* targetTile = GameScene::getThis()->mapHandler->getTileAt(tilePos->x, tilePos->y);
            if((targetTile->hasBuilding() && (targetTile->building != NULL && targetTile->building->buildingType != DECORATION)) || targetTile->isPath)
            {
                return true;
            }
        }
        
        if (source.x == tilePos->x &&
            source.y == tilePos->y)
        {
            //The source must always be reachable, in case someone wants to move from a building.
            return true;
        }
        
        if (!handler->isTileBlocked(*tilePos))
        {
            MapTile* targetTile = GameScene::getThis()->mapHandler->getTileAt(tilePos->x, tilePos->y);
            
            if(targetTile->hasBuilding())
            {
                if(targetTile->building == NULL)
                {
                    if(targetTile->master == NULL || targetTile->master->building == NULL || (targetTile->master->building->buildingType == DECORATION))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    if(targetTile->building->buildingType == DECORATION)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            else
            {
                return true;
            }
        }
    }
    return false;
    
}

bool PathFinder::hasPath( CCPoint* fromTile, CCPoint* toTile )
{
    // should use greedy algorithms for the path finding!
    
    openList->removeAllObjects();
    CC_SAFE_RELEASE(openList);
    closedList->removeAllObjects();
    CC_SAFE_RELEASE(closedList);
    
    openList = CCArray::create();
    openList->retain();
    closedList = CCArray::create();
    closedList->retain();
    
    closestNode = NULL;
    
    PathfindingNode* n = new PathfindingNode();
    n->autorelease();
    n->tilepos = CCPointMake(fromTile->x, fromTile->y);
    n->parent = NULL;
    n->G = 0;
    n->H = manhattanDist(fromTile, toTile);
    n->F = n->G + n->H;
    openList->addObject(n);
    
    PathfindingNode* targetNode = NULL;
    
    while (openList->count() > 0)
    {
        PathfindingNode *lowestCostNode = NULL;
        PathfindingNode *currNode = NULL;
        
        // get the node with lowest F (F = cost + heuristic)
        for (int i = 0; i < openList->count(); i++)
        {
            currNode = (PathfindingNode*) openList->objectAtIndex(i);
            if (lowestCostNode == NULL)
            {
                lowestCostNode = currNode;
            }
            else
            {
                if (currNode->F < lowestCostNode->F)
                {
                    lowestCostNode = currNode;
                }
                
            }
        }
        
        openList->removeObject(lowestCostNode);
        closedList->addObject(lowestCostNode);
        
        CCArray *reachableTiles = getReachableTiles(lowestCostNode, toTile);
        for (int i = 0; i < reachableTiles->count(); ++i)
        {
            PathfindingNode* reachableTile = (PathfindingNode*)reachableTiles->objectAtIndex(i);
            if(reachableTile->tilepos.x == toTile->x && reachableTile->tilepos.y == toTile->y)
            {
                return true;
            }
            openList->addObject(reachableTile);
        }
        
        if ((toTile->x == lowestCostNode->tilepos.x) && (toTile->y == lowestCostNode->tilepos.y))
        {
            targetNode = lowestCostNode;
        }
        
        reachableTiles->removeAllObjects();
        reachableTiles->release();
    }
    
    return false;
}

CCPoint PathFinder::getNearestNoneBuildingPos( CCPoint* sourcePos )
{
    tileOpenList->removeAllObjects();
    CC_SAFE_RELEASE(tileOpenList);
    tileClosedList->removeAllObjects();
    CC_SAFE_RELEASE(tileClosedList);
    
    tileOpenList = CCArray::create();
    tileOpenList->retain();
    tileClosedList = CCArray::create();
    tileClosedList->retain();
    
    PathfindingNode* n = new PathfindingNode();
    n->autorelease();
    n->tilepos = CCPointMake(sourcePos->x, sourcePos->y);
    n->parent = NULL;
    n->G = 0;
    n->H = 0;
    n->F = n->G + n->H;
    tileOpenList->addObject(n);
    
    while (tileOpenList->count() > 0)
    {
        PathfindingNode* lowestCostNode = NULL;
        PathfindingNode* currNode = NULL;
        for (int i = 0; i < tileOpenList->count(); i++)
        {
            currNode = (PathfindingNode*) tileOpenList->objectAtIndex(i);
            if (lowestCostNode == NULL)
            {
                lowestCostNode = currNode;
            }
            else
            {
                if (currNode->F < lowestCostNode->F)
                {
                    lowestCostNode = currNode;
                }
            }
        }
        
        MapTile* currTile = GameScene::getThis()->mapHandler->getTileAt(lowestCostNode->tilepos.x, lowestCostNode->tilepos.y);
        
        if(currTile->hasBuilding())
        {
            tileOpenList->removeObject(lowestCostNode);
            tileClosedList->removeObject(lowestCostNode);
            
            CCPoint pointsToCheck[4];
            pointsToCheck[0] = CCPointMake(lowestCostNode->tilepos.x -1, lowestCostNode->tilepos.y);
            pointsToCheck[1] = CCPointMake(lowestCostNode->tilepos.x +1, lowestCostNode->tilepos.y);
            pointsToCheck[2] = CCPointMake(lowestCostNode->tilepos.x, lowestCostNode->tilepos.y -1);
            pointsToCheck[3] = CCPointMake(lowestCostNode->tilepos.x, lowestCostNode->tilepos.y +1);
            
            for(int i = 0; i < 4; i++)
            {
                CCPoint adjacentTile = pointsToCheck[i];
                
                PathfindingNode *node = new PathfindingNode();
                node->autorelease();
                node->tilepos = CCPointMake(adjacentTile.x, adjacentTile.y);
                
                tileOpenList->addObject(node);
            }
        }
        else
        {
            CCPoint targetPos = ccp(currTile->xpos, currTile->ypos);
            return targetPos;
        }
    }
    
    return CCPointZero;
}

CCArray* PathFinder::makePath( CCPoint* fromTile, CCPoint* toTile)
{
    openList->removeAllObjects();
    CC_SAFE_RELEASE(openList);
    closedList->removeAllObjects();
    CC_SAFE_RELEASE(closedList);
    
    openList = CCArray::create();
    openList->retain();
    closedList = CCArray::create();
    closedList->retain();
    
    closestNode = NULL;
    
    PathfindingNode* n = new PathfindingNode();
    n->autorelease();
    n->tilepos = CCPointMake(fromTile->x, fromTile->y);
    n->parent = NULL;
    n->G = 0;
    n->H = manhattanDist(fromTile, toTile);
    n->F = n->G + n->H;
    openList->addObject(n);
    
    PathfindingNode* targetNode = NULL;
    
    while (openList->count() > 0)
    {
        PathfindingNode *lowestCostNode = NULL;
        PathfindingNode *currNode = NULL;
        
        // get the node with lowest F (F = cost + heuristic)
        for (int i = 0; i < openList->count(); i++)
        {
            currNode = (PathfindingNode*) openList->objectAtIndex(i);
            if (lowestCostNode == NULL)
            {
                lowestCostNode = currNode;
            }
            else
            {
                if (currNode->F < lowestCostNode->F)
                {
                    lowestCostNode = currNode;
                }
                
            }
        }
        
        openList->removeObject(lowestCostNode);
        closedList->addObject(lowestCostNode);
        
        CCArray *reachableTiles = getReachableTiles(lowestCostNode, toTile);
        for (int i = 0; i < reachableTiles->count(); ++i)
        {
            PathfindingNode* reachableTile = (PathfindingNode*)reachableTiles->objectAtIndex(i);
            // make the search greedy!
            if(reachableTile->tilepos.x == toTile->x && reachableTile->tilepos.y == toTile->y)
            {
                targetNode = reachableTile;
                break;
            }
            openList->addObject(reachableTile);
        }
        
        if ((toTile->x == lowestCostNode->tilepos.x) && (toTile->y == lowestCostNode->tilepos.y))
        {
            targetNode = lowestCostNode;
        }
        
        reachableTiles->removeAllObjects();
        reachableTiles->release();
    }
    
    CCArray* retrievedPath = CCArray::create();
    
    if(targetNode == NULL)
    {
        n = closestNode;
    }
    else
    {
        n = targetNode;
    }
    
    if (n)
    {
        retrievedPath->addObject(n);
        
        PathfindingNode* parent = n->parent;
        while (parent)
        {
            n = parent;
            parent = n->parent;
            retrievedPath->addObject(n);
         
        }
    }
   
    return retrievedPath;
}

CCArray* PathFinder::makeRainPath(CCPoint* fromTile, CCPoint* toTile)
{
    openList = CCArray::create();
    closedList = CCArray::create();
    CCArray* retrievedPath = CCArray::create();
    
    if(fromTile->x > toTile->x)
    {
        for(int i = fromTile->x; i >= toTile->x; i--)
        {
            PathfindingNode* n = new PathfindingNode();
            n->autorelease();
            n->tilepos = CCPointMake(i, fromTile->y);
            retrievedPath->addObject(n);
        }
    }
    else if(fromTile->x < toTile->x)
    {
        for(int i = fromTile->x; i <= toTile->x; i++)
        {
            PathfindingNode* n = new PathfindingNode();
            n->autorelease();
            n->tilepos = CCPointMake(i, fromTile->y);
            retrievedPath->addObject(n);
        }
    }
    else
    {
        PathfindingNode* n = new PathfindingNode();
        n->autorelease();
        n->tilepos = CCPointMake(fromTile->x, fromTile->y);
        retrievedPath->addObject(n);
    }
    
    if(fromTile->y > toTile->y)
    {
        for(int i = fromTile->y - 1; i >= toTile->y; i--)
        {
            PathfindingNode* n = new PathfindingNode();
            n->autorelease();
            n->tilepos = CCPointMake(toTile->x, i);
            retrievedPath->addObject(n);
        }
    }
    else if(fromTile->y < toTile->y)
    {
        for(int i = fromTile->y + 1; i <= toTile->y; i++)
        {
            PathfindingNode* n = new PathfindingNode();
            n->autorelease();
            n->tilepos = CCPointMake(toTile->x, i);
            retrievedPath->addObject(n);
        }
    }
    
    return retrievedPath;
}

/*
CCArray* PathFinder::makePathEscape(CCPoint* fromTile, CCPoint* toTile)
{
   
    openList = CCArray::create();
    closedList = CCArray::create();
    
    PathfindingNode* n = new PathfindingNode();
    n->autorelease();
    n->tilepos = CCPointMake(fromTile->x, fromTile->y);
    n->parent = NULL;
    n->G = 0;
    n->H = 0;
    n->F = 0;
    openList->addObject(n);
    
    initializeWithLowestCostNode(*toTile, true);
    
    CCArray* retrievedPath = CCArray::create();
    //retrievedPath->autorelease();
    n = isOnList(*toTile, closedList);
    if (n)
    {
        retrievedPath->addObject(n);
        
        PathfindingNode* parent = n->parent;
        while (parent)
        {
            n = parent;
            parent = n->parent;
            retrievedPath->addObject(n);
            
        }
    }
    
    return retrievedPath;
}
*/

CCArray* PathFinder::getReachableTiles(PathfindingNode *fromTile,
                                        CCPoint *toTile)
{
    CCArray* reachableTiles = CCArray::create();
    if (fromTile == NULL || toTile == NULL)
    {
        return reachableTiles;
    }

    CCPoint pointsToCheck[4];
    pointsToCheck[0] = CCPointMake(fromTile->tilepos.x -1, fromTile->tilepos.y);
    pointsToCheck[1] = CCPointMake(fromTile->tilepos.x +1, fromTile->tilepos.y);
    pointsToCheck[2] = CCPointMake(fromTile->tilepos.x, fromTile->tilepos.y -1);
    pointsToCheck[3] = CCPointMake(fromTile->tilepos.x, fromTile->tilepos.y +1);
    
    //and no diagonals
    for (int i = 3; i >= 0; i--)
    {
        CCPoint adjacentTile = pointsToCheck[i];
        if (isReachable(&adjacentTile))
        {
            if (!isOnList(adjacentTile, closedList))
            {
                // Either A* search or Dijkstra's algorithm will work for this game, as the distance between two adjacent tiles will not have difference.
                MapTile* targetTile = GameScene::getThis()->mapHandler->getTileAt(adjacentTile.x, adjacentTile.y);
                
                int G = 0;
                if(targetTile->hasBuilding())
                {
                    G = fromTile->G + 10;
                }
                else
                {
                    G = fromTile->G + 1;
                }
                
                int H = manhattanDist(&adjacentTile, toTile);
                int F = G + H;
                
                PathfindingNode* existingNode = isOnList( adjacentTile, openList );
                
                if (existingNode)
                {
                    if (F < existingNode->F)
                    {
                        existingNode->F = F;
                        existingNode->G = G;
                        existingNode->H = H;
                        existingNode->parent = fromTile;
                        //reachableTiles->addObject(existingNode);
                    }
                    
                    if (H < lowest_h)
                    {
                        closestNode = existingNode;
                        closest = fromTile->tilepos;
                        lowest_h = H;
                    }
                }
                else
                {
                    PathfindingNode *node = new PathfindingNode();
                    node->autorelease();
                    node->tilepos = CCPointMake(adjacentTile.x, adjacentTile.y);
                    node->parent = fromTile;
                    node->G = G;
                    node->H = H;
                    node->F = F;
                    reachableTiles->addObject(node);
                    
                    if (H < lowest_h)
                    {
                        closestNode = node;
                        closest = fromTile->tilepos;
                        lowest_h = H;
                    }
                }
            }
        }
    }
    
    return reachableTiles;
}

/*
void PathFinder::initializeWithLowestCostNode(CCPoint target, bool tryEscape)
{
    PathfindingNode *lowestCostNode = NULL;
    PathfindingNode *currNode = NULL;
    for (int i = 0; i < openList->count(); i++)
    {
        currNode = (PathfindingNode*) openList->objectAtIndex(i);
        if (lowestCostNode == NULL)
        {
            lowestCostNode = currNode;
        }
        else
        {
            if (currNode->F < lowestCostNode->F)
            {
                lowestCostNode = currNode;
            }
            
        }
    }
	
    openList->removeObject(lowestCostNode);
    closedList->addObject(lowestCostNode);
    
    CCArray *reachableTiles = getReachableTiles(lowestCostNode, &target, tryEscape);
	for (int i = 0; i < reachableTiles->count(); ++i)
    {
        PathfindingNode* reachableTile = (PathfindingNode*)reachableTiles->objectAtIndex(i);
        openList->addObject(reachableTile);
    }
    
    if ((target.x == lowestCostNode->tilepos.x) && (target.y == lowestCostNode->tilepos.y))
    {
		//CCLog("Path found!!");
	}
    else
    {
		if (openList->count() != 0)
        {
            initializeWithLowestCostNode(target, tryEscape);
        }
	}
    reachableTiles->removeAllObjects();
    reachableTiles->release();
}
*/