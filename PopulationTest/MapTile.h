//
//  MapTile.h
//  PopulationTest
//
//  Created by Serious Games on 7/3/13.
//
//

#ifndef PopulationTest_MapTile_h
#define PopulationTest_MapTile_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Building.h"

using namespace cocos2d;
using namespace CocosDenshion;

class MapTile: public CCObject
{
public:

    MapTile();
    ~MapTile();
    //position isn't actually needed. In MapHandler, the tile's x and y index in the array is also its position.
    //Debug
    int xpos;
    int ypos;
    
    bool isPath;
    bool isEnvironment;
    bool isInCombat;
    
    // 0 - Cannot build anything
    // 1 - Can build anything
    int buildAllowType;
    
    // 0 - can't be a destination or walked.
    // 1 - can be a destination but not walkable.
    // 2 - is walkable and can be a destination.
    int walkAllowType;
   
    int tileGID;
    
    MapTile* master; //for multitile buildings, will always point to the bottom left corner. All clicks will point to this.
    
    //pointer to built object
    Building* building;
    
    //pointer to environment sprite
    CCSprite* environment;

    bool isOccupied();
    bool hasBuilding();
    bool isBuildingAllowed(Building *b);
    
    
    void init();
  
    void pathHere();
    void unpathHere();
    
    void Build(Building* b);
    void UnBuild();
    void setMaster(MapTile* mapTile);
    void setEnvironment(CCSprite* sprite);
    
};
#endif
