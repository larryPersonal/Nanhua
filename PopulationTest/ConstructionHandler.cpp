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

static const int PROGRESSBAR_TAG = (int)"constructionProgressBar";

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
        return;
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
    }*/
    
    for (int i = getConstructingBuildingCount() - 1; i >= 0; i-- )
    {
        Building* building = (Building*)constructingBuildings->objectAtIndex(i);
       if (building->build_uint_current >= building->build_uint_required || building->build_uint_required == 0)
       {
           completeConstructingBuilding(building);
       }
       else
       {
           ProgressBar* progressBar = (ProgressBar*)building->buildingRep->getChildByTag(PROGRESSBAR_TAG);
           if (progressBar != NULL)
               progressBar->setValue(building->build_uint_current/building->build_uint_required); //TODO: get building construct time
           
       }
    }
    
}

void ConstructionHandler::addConstructingBuilding(Building* building)
{
    ProgressBar* progressBar = ProgressBar::create();
    progressBar->createProgressBar(CCRectMake(0, 0, building->buildingRep->boundingBox().size.width, 20),
                                   CCRectMake(5, 5, building->buildingRep->boundingBox().size.width-10, 10),
                                   "loadingbar-empty.png",
                                   "loadingbar-left.png",
                                   "loadingbar-right.png",
                                   "loadingbar-full.png");
    progressBar->setTag(PROGRESSBAR_TAG);
    building->buildingRep->addChild(progressBar);
    progressBar->setPosition(0, building->buildingRep->boundingBox().size.height);
    
    building->buildingRep->setOpacity(150);
   
    /*
    building->constructionEndTime = GameManager::getThis()->getCurrentTime() + building->constructionTime; //TODO: get building construct time*/
    
    
    constructingBuildings->addObject(building);
    
}

void ConstructionHandler::removeConstructingBuilding(Building *building)
{
    constructingBuildings->removeObject(building);
}

void ConstructionHandler::completeConstructingBuilding(Building* building)
{
    building->buildingRep->removeChildByTag(PROGRESSBAR_TAG);
    building->buildingRep->setOpacity(255);
    building->ID = GameScene::getThis()->buildingHandler->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.

    GameScene::getThis()->buildingHandler->addBuildingToMap(building);
    constructingBuildings->removeObject(building);
    
    
    if (TutorialHandler::getThis()->IsActive())
    {
        std::string bType;
        if (building->buildingType == HOUSING) bType = "HOUSING";
        if (building->buildingType == COMMERCE) bType = "COMMERCE";
        if (building->buildingType == AMENITY) bType = "AMENITY";
        if (building->buildingType == MILITARY) bType = "MILITARY";
        if (building->buildingType == EDUCATION) bType = "EDUCATION";
        if (building->buildingType == SOCIAL) bType = "SOCIAL";
        
        TutorialHandler::getThis()->ReportAction("constructioncomplete", "building", bType);
        
    }
    
    //handled in AI
    /*
    if (building->buildingType == HOUSING)
    {
        GameScene::getThis()->spriteHandler->NewHousingLocationCreated();
    
    }
    else
    {
        GameScene::getThis()->spriteHandler->NewJobLocationCreated();
    }*/
    
}

int ConstructionHandler::getConstructingBuildingCount()
{
    return constructingBuildings->count();
}




