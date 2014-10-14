//
//  ConstructionHandler.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 13/5/13.
//
//

#include "ConstructionHandler.h"
#include "GameScene.h"
#include "GameManager.h"
#include "ProgressBar.h"
#include "TutorialManager.h"

ConstructionHandler* ConstructionHandler::SP;

ConstructionHandler* ConstructionHandler::getThis()
{
    return SP;
}

ConstructionHandler::ConstructionHandler()
{
    ConstructionHandler::SP = this;
    
    constructingBuildings = CCArray::create();
    constructingBuildings->retain();
}

ConstructionHandler::~ConstructionHandler()
{
    ConstructionHandler::SP = NULL;
    
    constructingBuildings->removeAllObjects();
    constructingBuildings->release();
}

void ConstructionHandler::update(float deltaTime)
{
    if (constructingBuildings->count() <= 0)
    {
        return;
    }
    
    /*
    float currentTime = GameManager::getThis()->getCurrentTime();
    
    for (int i = getConstructingBuildingCount() - 1; i >= 0; i--)   //reverse iterate due to removals
    {
        Building* building = (Building*)constructingBuildings->objectAtIndex(i);
        
        if (currentTime >= building->constructionEndTime)
            completeConstructingBuilding(building);
        else
        {
            ProgressBar* progressBar = (ProgressBar*)building->buildingRep->getChildByTag(PROGRESSBAR_TAG);
            if (progressBar != NULL)
            progressBar->setValue(1.0f - ((building->constructionEndTime - currentTime) /building->constructionTime)); //TODO: get building construct time
        }
    }
    */
    
    for (int i = getConstructingBuildingCount() - 1; i >= 0; i-- )
    {

        Building* building = (Building*)constructingBuildings->objectAtIndex(i);
        if (building->build_uint_current >= building->build_uint_required || building->build_uint_required == 0)
       {
           CCLog("%i", building->build_uint_current);
           completeConstructingBuilding(building);
       }
    }
    
}

void ConstructionHandler::addConstructingBuilding(Building* building)
{
    building->taskType = Construction;
    
    building->buildingRep->setOpacity(150);
    
    constructingBuildings->addObject(building);
    
    if(building->buildingType == HOUSING)
    {
        BuildingHandler::getThis()->housingGhostOnMap->addObject(building);
    }
    else if(building->buildingType == GRANARY)
    {
        BuildingHandler::getThis()->granaryGhostOnMap->addObject(building);
    }
    else if(building->buildingType == MARKET)
    {
        BuildingHandler::getThis()->marketGhostOnMap->addObject(building);
    }
    else if(building->buildingType == AMENITY)
    {
        BuildingHandler::getThis()->amenityGhostOnMap->addObject(building);
    }
    else if(building->buildingType == MILITARY)
    {
        BuildingHandler::getThis()->militaryGhostOnMap->addObject(building);
    }
    else if(building->buildingType == COMMERCE)
    {
        BuildingHandler::getThis()->commerceGhostOnMap->addObject(building);
    }
    else if(building->buildingType == EDUCATION)
    {
        BuildingHandler::getThis()->educationGhostOnMap->addObject(building);
    }
    else if(building->buildingType == SOCIAL)
    {
        BuildingHandler::getThis()->socialGhostOnMap->addObject(building);
    }
    else if(building->buildingType == SPECIAL)
    {
        BuildingHandler::getThis()->specialGhostOnMap->addObject(building);
    }
    else if(building->buildingType == DECORATION)
    {
        BuildingHandler::getThis()->decorationGhostOnMap->addObject(building);
    }
    
    BuildingHandler::getThis()->allBuildingsGhostOnMap->addObject(building);
}

void ConstructionHandler::removeConstructingBuilding(Building *building)
{
    constructingBuildings->removeObject(building);
    
    if(building->buildingType == HOUSING)
    {
        BuildingHandler::getThis()->housingGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == GRANARY)
    {
        BuildingHandler::getThis()->granaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MARKET)
    {
        BuildingHandler::getThis()->marketGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == AMENITY)
    {
        BuildingHandler::getThis()->amenityGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MILITARY)
    {
        BuildingHandler::getThis()->militaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == COMMERCE)
    {
        BuildingHandler::getThis()->commerceGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == EDUCATION)
    {
        BuildingHandler::getThis()->educationGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SOCIAL)
    {
        BuildingHandler::getThis()->socialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SPECIAL)
    {
        BuildingHandler::getThis()->specialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == DECORATION)
    {
        BuildingHandler::getThis()->decorationGhostOnMap->removeObject(building);
    }
    
    BuildingHandler::getThis()->allBuildingsGhostOnMap->removeObject(building);
}

void ConstructionHandler::completeConstructingBuilding(Building* building)
{
    building->taskType = NoActivity;
    building->removeProgressiveBar();
    
    building->buildingRep->setOpacity(255);
    building->ID = BuildingHandler::getThis()->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
 
    BuildingHandler::getThis()->addBuildingToMap(building);
    constructingBuildings->removeObject(building);
    
    if(building->buildingType == HOUSING)
    {
        BuildingHandler::getThis()->housingGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == GRANARY)
    {
        BuildingHandler::getThis()->granaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MARKET)
    {
        BuildingHandler::getThis()->marketGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == AMENITY)
    {
        BuildingHandler::getThis()->amenityGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MILITARY)
    {
        BuildingHandler::getThis()->militaryGhostOnMap->removeObject(building);
        
        if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachBuildGuardTower)
        {
            if(TutorialManager::getThis()->miniDragon != NULL)
            {
                TutorialManager::getThis()->miniDragon->clickNext();
            }
        }
    }
    else if(building->buildingType == COMMERCE)
    {
        BuildingHandler::getThis()->commerceGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == EDUCATION)
    {
        BuildingHandler::getThis()->educationGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SOCIAL)
    {
        BuildingHandler::getThis()->socialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SPECIAL)
    {
        BuildingHandler::getThis()->specialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == DECORATION)
    {
        BuildingHandler::getThis()->decorationGhostOnMap->removeObject(building);
    }
    
    BuildingHandler::getThis()->allBuildingsGhostOnMap->removeObject(building);

    if(TutorialManager::getThis()->active)
    {
        if(TutorialManager::getThis()->teachBuildHouse && building->buildingType == HOUSING)
        {
            TutorialManager::getThis()->miniDragon->clickNext();
            TutorialManager::getThis()->teachBuildHouse = false;
        }
        
        if(TutorialManager::getThis()->teachBuildFarm && building->buildingType == AMENITY)
        {
            TutorialManager::getThis()->miniDragon->clickNext();
        }
    }
    
}

int ConstructionHandler::getConstructingBuildingCount()
{
    return constructingBuildings->count();
}


CCArray* ConstructionHandler::getConstructingBuildings()
{
    return constructingBuildings;
}

void ConstructionHandler::clear()
{
    constructingBuildings->removeAllObjects();
}



