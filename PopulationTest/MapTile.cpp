//
//  MapTile.cpp
//  PopulationTest
//
//  Created by Serious Games on 7/3/13.
//
//

#include "MapTile.h"
#include "GameScene.h"

MapTile::MapTile()
{
    xpos = ypos = 0;
    building = NULL;
    environment = NULL;
    master = NULL;
    isPath = false;
    isEnvironment = false;
    isInCombat = false;
    buildAllowType = 0;
    walkAllowType = 0;
}

MapTile::~MapTile()
{
    if (environment != NULL)
    {
        environment->release();
    }
}

void MapTile::init()
{
   // building = new Building();
    buildAllowType = 1;
    walkAllowType = 2;
    isPath = false;
}

bool MapTile::isOccupied()
{
    if (this==NULL){
        return true;
    }
    
    bool occupied = false;
    
    if (isInCombat){
        occupied = true;
    }
    
    bool b = false;//(building != NULL);
    if (building != NULL)
    {
        if (building->buildingName.compare("Wall") == 0 || building->buildingName.compare("Gate") == 0)
        {
            b = true;
        }
    }
    bool m = false;//(master != NULL);
    
    if (master != NULL)
    {
        if (master->building->buildingName.compare("Wall") == 0 || master->building->buildingName.compare("Gate") == 0)
        {
            m = true;
        }

    }
    
    bool e = (environment != NULL);
    if (b || m || e) {
        occupied = true;
    }
    
    return (occupied);
}

bool MapTile::hasBuilding()
{
    return (building != NULL) || (master != NULL);
}

void MapTile::pathHere()
{
    if (isOccupied())
    {
        return;
    }
    isPath = true;
}

void MapTile::unpathHere()
{
    if (isOccupied()){
        return;
    }
    isPath = false;
}


void MapTile::Build(Building *b)
{
    building = b;
}

void MapTile::UnBuild()
{
    master = NULL;
    building = NULL;
}

void MapTile::setMaster(MapTile* mapTile)
{
    master = mapTile;
}

void MapTile::setEnvironment(CCSprite* sprite)
{
    environment = sprite;
    isEnvironment = true;
}



