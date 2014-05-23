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

ConstructionHandler::ConstructionHandler()
{
    constructingBuildings = CCArray::create();
    constructingBuildings->retain();
}

ConstructionHandler::~ConstructionHandler()
{
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
        GameScene::getThis()->buildingHandler->housingGhostOnMap->addObject(building);
    }
    else if(building->buildingType == GRANARY)
    {
        GameScene::getThis()->buildingHandler->granaryGhostOnMap->addObject(building);
    }
    else if(building->buildingType == MARKET)
    {
        GameScene::getThis()->buildingHandler->marketGhostOnMap->addObject(building);
    }
    else if(building->buildingType == AMENITY)
    {
        GameScene::getThis()->buildingHandler->amenityGhostOnMap->addObject(building);
    }
    else if(building->buildingType == MILITARY)
    {
        GameScene::getThis()->buildingHandler->militaryGhostOnMap->addObject(building);
    }
    else if(building->buildingType == COMMERCE)
    {
        GameScene::getThis()->buildingHandler->commerceGhostOnMap->addObject(building);
    }
    else if(building->buildingType == EDUCATION)
    {
        GameScene::getThis()->buildingHandler->educationGhostOnMap->addObject(building);
    }
    else if(building->buildingType == SOCIAL)
    {
        GameScene::getThis()->buildingHandler->socialGhostOnMap->addObject(building);
    }
    else if(building->buildingType == SPECIAL)
    {
        GameScene::getThis()->buildingHandler->specialGhostOnMap->addObject(building);
    }
    else if(building->buildingType == DECORATION)
    {
        GameScene::getThis()->buildingHandler->decorationGhostOnMap->addObject(building);
    }
    
    GameScene::getThis()->buildingHandler->allBuildingsGhostOnMap->addObject(building);
}

void ConstructionHandler::removeConstructingBuilding(Building *building)
{
    constructingBuildings->removeObject(building);
    
    if(building->buildingType == HOUSING)
    {
        GameScene::getThis()->buildingHandler->housingGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == GRANARY)
    {
        GameScene::getThis()->buildingHandler->granaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MARKET)
    {
        GameScene::getThis()->buildingHandler->marketGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == AMENITY)
    {
        GameScene::getThis()->buildingHandler->amenityGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MILITARY)
    {
        GameScene::getThis()->buildingHandler->militaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == COMMERCE)
    {
        GameScene::getThis()->buildingHandler->commerceGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == EDUCATION)
    {
        GameScene::getThis()->buildingHandler->educationGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SOCIAL)
    {
        GameScene::getThis()->buildingHandler->socialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SPECIAL)
    {
        GameScene::getThis()->buildingHandler->specialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == DECORATION)
    {
        GameScene::getThis()->buildingHandler->decorationGhostOnMap->removeObject(building);
    }
    
    GameScene::getThis()->buildingHandler->allBuildingsGhostOnMap->removeObject(building);
}

void ConstructionHandler::completeConstructingBuilding(Building* building)
{
    building->taskType = NoActivity;
    building->removeProgressiveBar();
    
    building->buildingRep->setOpacity(255);
    building->ID = GameScene::getThis()->buildingHandler->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
 
    GameScene::getThis()->buildingHandler->addBuildingToMap(building);
    constructingBuildings->removeObject(building);
    
    if(building->buildingType == HOUSING)
    {
        GameScene::getThis()->buildingHandler->housingGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == GRANARY)
    {
        GameScene::getThis()->buildingHandler->granaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MARKET)
    {
        GameScene::getThis()->buildingHandler->marketGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == AMENITY)
    {
        GameScene::getThis()->buildingHandler->amenityGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == MILITARY)
    {
        GameScene::getThis()->buildingHandler->militaryGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == COMMERCE)
    {
        GameScene::getThis()->buildingHandler->commerceGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == EDUCATION)
    {
        GameScene::getThis()->buildingHandler->educationGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SOCIAL)
    {
        GameScene::getThis()->buildingHandler->socialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == SPECIAL)
    {
        GameScene::getThis()->buildingHandler->specialGhostOnMap->removeObject(building);
    }
    else if(building->buildingType == DECORATION)
    {
        GameScene::getThis()->buildingHandler->decorationGhostOnMap->removeObject(building);
    }
    
    GameScene::getThis()->buildingHandler->allBuildingsGhostOnMap->removeObject(building);

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



