//
//  GameManager.cpp
//  PopulationTest
//
//  Created by admin on 29/4/13.
//
//

#include "GameManager.h"
#include "GameUnlocks.h"
#include "GameDefaults.h"
#include "Possessions.h"
#include "GlobalHelper.h"
#include "GameHUD.h"
#include "HousingLimit.h"
#include <json/json.h> //for the area unlock section
//#include <sstream>
//#include <iostream>
//#include <iterator>
#include <cstring>

#include "MainMenuScene.h"
#include "SenarioChooseScene.h"
#include "GameScene.h"
#include "Senario.h"
#include "TutorialManager.h"
#include "SanGuoXiaoXueTang.h"
#include "ObjectiveHandler.h"
#include "NotificationPopup.h"
#include "RandomEventManager.h"

GameManager* GameManager::SP;

GameManager::GameManager()
{
    GameManager::SP = this;
    unlockedBuildings = CCArray::create();
    unlockedBuildings->retain();
    lockedBuildings = CCArray::create();
    lockedBuildings->retain();
    
    researchableBuildings = CCArray::create();
    researchableBuildings->retain();
    
    areaUnlockIndex = 0;
    
    town_hall_level = 0;
    
    housingLimitation = new HousingLimit();
    parseHousingLimitation();
    
    init();
}

GameManager::~GameManager()
{
    GameManager::SP = NULL;
    //PLEASE empty all arrays before use. RELEASING arrays does not necessarily empty them first!
    unlockedBuildings->removeAllObjects();
    lockedBuildings->removeAllObjects();
    researchableBuildings->removeAllObjects();
    unlockedBuildings->release();
    lockedBuildings->release();
    researchableBuildings->release();
    
    unlockedBuildingIDs.clear();
    researchableBuildingIDs.clear();
    
    areaUnlocks.clear();
}

GameManager* GameManager::getThis()
{
    return SP;
}

void GameManager::init()
{
    initMapUnlocks();
    initMenuUnlocks();
}

void GameManager::initMapUnlocks()
{
    bool parsingSuccessful = reader.parse(Unlocks,root);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse defaults doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    else
    {
        //std::string encoding = root.get("encoding", "UTF-8" ).asString();
        for (int i = 0; i<root["items"].size(); i++)
        {
            std::string name = root["items"].operator[](i)["unlock_name"].asString();
            std::size_t found = name.find("area_");
            if (found==std::string::npos){
                continue;
            }
           
            AreaUnlock a;
            a.min = ccp(root["items"].operator[](i)["unlock_min_x"].asInt(), root["items"].operator[](i)["unlock_min_y"].asInt());
            a.max = ccp(root["items"].operator[](i)["unlock_max_x"].asInt(), root["items"].operator[](i)["unlock_max_y"].asInt());
            a.name = name;
            a.prereqs = root["items"].operator[](i)["unlock_prereq"].asString();
            a.researchtime = root["items"].operator[](i)["research_time"].asDouble();
            a.researchcost = root["items"].operator[](i)["research_cost"].asInt();
            a.required_cash =root["items"].operator[](i)["unlock_cash"].asInt() ;
            a.required_bcount = root["items"].operator[](i)["unlock_bcount"].asInt();
            a.required_popn = root["items"].operator[](i)["unlock_popn"].asInt();
            areaUnlocks.push_back(a);
        }
    }
    
    root.clear();
}

void GameManager::initMenuUnlocks()
{
    bool parsingSuccessful = reader.parse(Unlocks,root);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse defaults doc\n" << reader.getFormatedErrorMessages() << std::endl;
        
    }
    else
    {
        //std::string encoding = root.get("encoding", "UTF-8" ).asString();
        for (int i = 0; i<root["items"].size(); i++)
        {
            std::string name = root["items"].operator[](i)["unlock_name"].asString();
            std::size_t found = name.find("menu_");
            if (found==std::string::npos) continue;
            
            MenuItemUnlock a;
            a.name = name;
            a.prereqs = root["items"].operator[](i)["unlock_prereq"].asString();
            a.required_cash =root["items"].operator[](i)["unlock_cash"].asInt() ;
            a.required_bcount = root["items"].operator[](i)["unlock_bcount"].asInt();
            a.required_popn = root["items"].operator[](i)["unlock_popn"].asInt();
            
            gameMenuUnlocks.push_back(a);
        }
    }
    
    root.clear();
}

void GameManager::EndGameData()
{
    unlockedBuildings->removeAllObjects();
    lockedBuildings->removeAllObjects();
    researchableBuildings->removeAllObjects();
}

// this method is only called when GameScene is set up
void GameManager::initGameData()
{
    unlockedBuildingIDs.clear();
    researchableBuildingIDs.clear();

    if (unlockedBuildings->count() > 0)
    unlockedBuildings->removeAllObjects();
    if (lockedBuildings->count() > 0)
    lockedBuildings->removeAllObjects();
    researchableBuildings->removeAllObjects();

    //Loaded game must be set from the main menu, if player chooses to continue game instead of starting a new game.
    NewGameUnlocks();

    // Init arrays of locked and unlocked buildings
    for (list<int>::iterator it = unlockedBuildingIDs.begin(); it != unlockedBuildingIDs.end(); it++)
    {
        CCLog("%d", (*it));
        unlockedBuildings->addObject(BuildingHandler::getThis()->getBuilding(*it));
    }
    for (list<int>::iterator it = researchableBuildingIDs.begin(); it != researchableBuildingIDs.end(); it++)
        researchableBuildings->addObject(BuildingHandler::getThis()->getBuilding(*it));
    
    
    lockedBuildings->initWithArray(BuildingHandler::getThis()->allBuildings);
    lockedBuildings->removeObjectsInArray(unlockedBuildings);
    lockedBuildings->removeObjectsInArray(researchableBuildings);
}

bool GameManager::checkGameDataExists(){
    
    std::string test = CCFileUtils::sharedFileUtils()->getWritablePath();
    test += "mayansave.txt";
    std::ifstream readFile(test.c_str());
    
    if (readFile)
    {
        readFile.close();
        return true;
    }
    return false;
}

CCPoint GameManager::getMinArea()
{
    return areaUnlocks[areaUnlockIndex].min;
    
}

CCPoint GameManager::getMaxArea()
{
    return areaUnlocks[areaUnlockIndex].max;
}

/*Note: once unlocked, facilities will not be re-locked.*/
/*facilities will appear in research menu first before being unlocked.*/
bool GameManager::UnlockConditionsMet(Building *b)
{
    if (b->buildingName.length() == 0 || b->buildingName.compare("No Name") == 0)
    {
        return false;
    }
    return (b->unlocked);
}

bool GameManager::ResearchConditionsMet(Building* b)
{
    
    if (b->buildingName.length() == 0 || b->buildingName.compare("No Name") == 0)
    {
        return false;
    }
    return true;
    
}


void GameManager::UpdateUnlocks()
{
    if (BuildingHandler::getThis()->allBuildingsOnMap == NULL || SpriteHandler::getThis()->spritesOnMap == NULL) return;
   
    /*update building unlocks*/
    if (lockedBuildings->count() == 0) return; //don't bother if everything's been unlocked already
    
    CCArray* toRemove = CCArray::create();
    for (int i = 0; i < lockedBuildings->count(); ++i)
    {
        Building* b = ((Building*)lockedBuildings->objectAtIndex(i));
        if (UnlockConditionsMet(b))
        {
            toRemove->addObject(b);
            unlockedBuildings->addObject(b);
            
        }
        else
        {
            if (ResearchConditionsMet(b))
            {
                toRemove->addObject(b);
                researchableBuildings->addObject(b);
            }
        }
        
        
    }
    
    
    
    if (toRemove->count() > 0)
    {
        lockedBuildings->removeObjectsInArray(toRemove);
    }
   
    //if (TutorialHandler::getThis()->IsActive()) return; //do not update unlocks when tutorial is active.
    
    //menuitem unlocks only run outside of the tutorial.
    for (int i = 0; i < gameMenuUnlocks.size(); ++i)
    {
        /*
        if (gameMenuUnlocks[i].hasMetCriteria(currentMoney))
        {
            fliplockMenuItem(i, true);
        }
        */
    }

}

void GameManager::NewGameUnlocks()
{
    /*builds the unlock ID list for a new game. This should not be called when loading from a save*/
    CCArray* allBuildings = BuildingHandler::getThis()->allBuildings;
    
    unlockedBuildingIDs.clear();
    researchableBuildingIDs.clear();
    
    
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        Building* b = ((Building*)allBuildings->objectAtIndex(i));
        if (b->unlocked == true)
            unlockedBuildingIDs.push_back(b->ID);
        else
            if (ResearchConditionsMet(b))
                researchableBuildingIDs.push_back(b->ID);
        
        /*
        if (UnlockConditionsMet(b))
        {
            unlockedBuildingIDs.push_back(b->ID);
        }*/
        
    }
}

void GameManager::UnlockAll()
{
    for (int i = 0; i < gameMenuUnlocks.size(); ++i)
    {
        gameMenuUnlocks[i].isUnlocked = true;
    }
    
    for (int i = 0; i < areaUnlocks.size(); ++i)
    {
        areaUnlocks[i].isUnlocked = true;
    }
    
    CCArray* allBuildings = BuildingHandler::getThis()->allBuildings;
    
    unlockedBuildingIDs.clear();
    researchableBuildingIDs.clear();
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        Building* b = ((Building*)allBuildings->objectAtIndex(i));
        if (b->unlocked == true)
            unlockedBuildingIDs.push_back(b->ID);
        else
        {
            if (b->researchCost > 0)
            {
                researchableBuildingIDs.push_back(b->ID);
            }
            else
            {
                unlockedBuildingIDs.push_back(b->ID);
            }
        }
    }
    
}

void GameManager::fliplockMenuItem(int arrayIndex, bool locked)
{
    if (gameMenuUnlocks.size() == 0) return;
    if (arrayIndex >= gameMenuUnlocks.size()) return;
    
    
    gameMenuUnlocks[arrayIndex].isUnlocked = locked;
    
}

bool GameManager::isMenuItemUnlocked(std::string itemName)
{
    if (gameMenuUnlocks.size() == 0) return true; //safe fail, allowing all menu items to be unlocked if there's no unlocks defined.
    for (int i = 0; i < gameMenuUnlocks.size(); ++i)
    {
        if (gameMenuUnlocks[i].name.compare(itemName) == 0)
        {
            if (gameMenuUnlocks[i].isUnlocked) return true;
            else return false;
        }
    }
    return true;
}


AreaUnlock* GameManager::getNextAreaUnlock()
{
    if (areaUnlocks.size() == 0) return NULL;
    if (areaUnlockIndex + 1 >= (areaUnlocks.size() - 1))
    {
        return NULL;
    }
    else return &(areaUnlocks[areaUnlockIndex + 1]);
}

void GameManager::triggerNextAreaUnlock()
{
    ++areaUnlockIndex;
    if (areaUnlockIndex > (areaUnlocks.size() -1))
    {
        areaUnlockIndex = areaUnlocks.size() -1;
    }
    MapHandler::getThis()->updatePlayArea(areaUnlocks[areaUnlockIndex].min , areaUnlocks[areaUnlockIndex].max);
    
}

bool GameManager::hasLostGame()
{
    //check lose condition
        
        return true;
    
    return false;

}

void GameManager::parseHousingLimitation()
{
    std::string content = contentJson;
    
    // specific stats
    bool parsingSuccessful = parseHousingLimit(content);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse housing limit jason doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    else
    {
        loadHousingLimit();
    }
    
    hl.clear();
}

bool GameManager::parseHousingLimit(string doc)
{
    if (doc.length() == 0)
    {
        std::cout << "no housing limit file loaded!" << std::endl;
        return false;
    }
    
    // this is the main method that starts off all the calculations
    //initialize behavior tree
    bool parsingSuccessful = reader.parse( doc, hl );
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse config doc\n" << reader.getFormatedErrorMessages() << std::endl;
    }
    std::string encoding = root.get("encoding", "UTF-8" ).asString();
    
    return parsingSuccessful;
}

void GameManager::loadHousingLimit()
{
    housingLimitation->maximum_level = atoi(hl["maximum_level"].asString().c_str());
    
    for(int i = 1; i <= housingLimitation->maximum_level; i++)
    {
        stringstream ss;
        ss << "level_" << i;
        
        if(hl[ss.str().c_str()]["levelup_money"].asString().compare("unlimited") == 0)
        {
            housingLimitation->gold_required.push_back(999999);
        }
        else
        {
            housingLimitation->gold_required.push_back(atoi(hl[ss.str().c_str()]["levelup_money"].asString().c_str()));
        }
        
        if(hl[ss.str().c_str()]["levelup_food"].asString().compare("unlimited") == 0)
        {
            housingLimitation->food_required.push_back(999999);
        }
        else
        {
            housingLimitation->food_required.push_back(atoi(hl[ss.str().c_str()]["levelup_food"].asString().c_str()));
        }
        
        if(hl[ss.str().c_str()]["housing"].asString().compare("unlimited") == 0)
        {
            housingLimitation->farm_limits.push_back(999999);
        }
        else
        {
            housingLimitation->farm_limits.push_back(atoi(hl[ss.str().c_str()]["housing"].asString().c_str()));
        }
        
        if(hl[ss.str().c_str()]["granary"].asString().compare("unlimited") == 0)
        {
            housingLimitation->granary_limits.push_back(999999);
        }
        else
        {
            housingLimitation->granary_limits.push_back(atoi(hl[ss.str().c_str()]["granary"].asString().c_str()));
        }
        
        if(hl[ss.str().c_str()]["granary"].asString().compare("unlimited") == 0)
        {
            housingLimitation->market_limits.push_back(999999);
        }
        else
        {
            housingLimitation->market_limits.push_back(atoi(hl[ss.str().c_str()]["market"].asString().c_str()));
        }
        
        if(hl[ss.str().c_str()]["guard_tower"].asString().compare("unlimited") == 0)
        {
            housingLimitation->guard_tower_limits.push_back(999999);
        }
        else
        {
            housingLimitation->guard_tower_limits.push_back(atoi(hl[ss.str().c_str()]["guard_tower"].asString().c_str()));
        }
        
        if(hl[ss.str().c_str()]["housing"].asString().compare("unlimited") == 0)
        {
            housingLimitation->housing_limits.push_back(999999);
        }
        else
        {
            housingLimitation->housing_limits.push_back(atoi(hl[ss.str().c_str()]["housing"].asString().c_str()));
        }
    }
}

void GameManager::enableGameScene()
{
    MainMenuScene::getThis()->setVisible(false);
    SenarioChooseScene::getThis()->setVisible(false);
    SanGuoXiaoXueTang::getThis()->setVisible(true);
    Senario::getThis()->setVisible(true);
    TutorialManager::getThis()->setVisible(true);
    GameHUD::getThis()->setVisible(true);
    ObjectiveHandler::getThis()->setVisible(true);
    NotificationPopup::getThis()->setVisible(true);
    RandomEventManager::getThis()->setVisible(true);
}

void GameManager::enableMainMenuScene()
{
    MainMenuScene::getThis()->setVisible(true);
    SenarioChooseScene::getThis()->setVisible(false);
    SanGuoXiaoXueTang::getThis()->setVisible(false);
    Senario::getThis()->setVisible(false);
    TutorialManager::getThis()->setVisible(false);
    GameHUD::getThis()->setVisible(false);
    ObjectiveHandler::getThis()->setVisible(false);
    NotificationPopup::getThis()->setVisible(false);
    RandomEventManager::getThis()->setVisible(false);
}

void GameManager::enableSenarioChooseScene()
{
    MainMenuScene::getThis()->setVisible(false);
    SenarioChooseScene::getThis()->setVisible(true);
    SanGuoXiaoXueTang::getThis()->setVisible(false);
    Senario::getThis()->setVisible(false);
    TutorialManager::getThis()->setVisible(false);
    GameHUD::getThis()->setVisible(false);
    ObjectiveHandler::getThis()->setVisible(false);
    NotificationPopup::getThis()->setVisible(false);
    RandomEventManager::getThis()->setVisible(false);
}