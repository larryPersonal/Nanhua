//
//  ConstructionHandler.h
//  PopulationTest
//
//  Created by Ong Heng Le on 13/5/13.
//
//

#ifndef __PopulationTest__ConstructionHandler__
#define __PopulationTest__ConstructionHandler__

#include "cocos2d.h"
#include "Building.h"
using namespace cocos2d;

class ConstructionHandler
{
    static ConstructionHandler* SP;
    
private:
    
    CCArray* constructingBuildings;
    
public:
    ConstructionHandler();
    ~ConstructionHandler();
    
    static ConstructionHandler* getThis();
    
    virtual void update(float deltaTime);
    
    void addConstructingBuilding(Building* building);
    void removeConstructingBuilding(Building* building);
    void completeConstructingBuilding(Building* building);
    int getConstructingBuildingCount();
    
    CCArray* getConstructingBuildings();
    
    void clear();
};

#endif /* defined(__PopulationTest__ConstructionHandler__) */
