//
//  ResearchHandler.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 3/5/13.
//
//

#include "ResearchHandler.h"
#include "GameManager.h"
#include "PopupMenu.h"
#include "AlertBox.h"
#include "GameHUD.h"

ResearchHandler::ResearchHandler()
{
    sleepTimeLeft = 0;
}

ResearchHandler::~ResearchHandler()
{
}

void ResearchHandler::update(float deltaTime)
{
    if (sleepTimeLeft > 0)
    {
        sleepTimeLeft -= deltaTime;
        return;
    }
    
    float shortestTimeDiff = INT_MAX;
    map<int, float>::iterator it = GameManager::getThis()->buildingIdResearchEndTime.begin();
    while (it != GameManager::getThis()->buildingIdResearchEndTime.end())
    {
        if (GameManager::getThis()->getCurrentTime() >= it->second)
        {
            completeResearchingBuilding(it->first);
            GameManager::getThis()->buildingIdResearchEndTime.erase(it++);
        }
        else
        {
            float diff = it->second - GameManager::getThis()->getCurrentTime();
            if (diff < shortestTimeDiff)
                shortestTimeDiff = diff;
            ++it;
        }
    }
    sleepTimeLeft = shortestTimeDiff;
}

void ResearchHandler::addResearchingBuilding(int buildingID, float time)
{
    if (getResearchingBuildingCount() == 1)
    {
        GameHUD::getThis()->showHint("You cannot research more than 1 building at once");
        return;
    }
    GameManager::getThis()->buildingIdResearchEndTime[buildingID] = GameManager::getThis()->getCurrentTime() + time;
    if (time < sleepTimeLeft)
        sleepTimeLeft = time;
    
}

void ResearchHandler::completeResearchingBuilding(int buildingID)
{
    CCLog("Researched BuildingID: %i", buildingID);
    
    GameManager::getThis()->unlockedBuildingIDs.push_back(buildingID);
    GameManager::getThis()->researchableBuildingIDs.remove(buildingID);
    
    Building* building = GameScene::getThis()->buildingHandler->getBuilding(buildingID);
    GameManager::getThis()->unlockedBuildings->addObject(building);
    GameManager::getThis()->researchableBuildings->removeObject(building);
    /*inform the tutorial that a building of type X has been researched*/
    if (TutorialHandler::getThis()->IsActive())
    {
        std::string bType;
        if (building->buildingType == HOUSING) bType = "HOUSING";
        if (building->buildingType == COMMERCE) bType = "COMMERCE";
        if (building->buildingType == AMENITY) bType = "AMENITY";
        if (building->buildingType == MILITARY) bType = "MILITARY";
        if (building->buildingType == EDUCATION) bType = "EDUCATION";
        if (building->buildingType == SOCIAL) bType = "SOCIAL";
        
        TutorialHandler::getThis()->ReportAction("researchcomplete", "building", bType);
    }
    
    //Visual feedback
    PopupMenu* newAlert = new AlertBox();
    newAlert->useAsExclusivePopupMenu();
    string text = building->buildingName;
    text += " unlocked!";
    ((AlertBox*)newAlert)->setDisplayText(text);
    ((AlertBox*)newAlert)->addButton(0, "Ok");
    
}

int ResearchHandler::getResearchingBuildingCount()
{
    return GameManager::getThis()->buildingIdResearchEndTime.size();
}

float ResearchHandler::getResearchingBuildingTimeLeft(int buildingID)
{
    if (!isAlreadyResearching(buildingID))
        return 0.0f;
    
    return GameManager::getThis()->buildingIdResearchEndTime.at(buildingID) - GameManager::getThis()->getCurrentTime();
}

bool ResearchHandler::isAlreadyResearching(int buildingID)
{
    return GameManager::getThis()->buildingIdResearchEndTime.count(buildingID) > 0;
}


