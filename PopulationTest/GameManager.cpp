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
#include "GameTimer.h"
#include "GameHUD.h"
#include <json/json.h> //for the area unlock section
//#include <sstream>
//#include <iostream>
//#include <iterator>
#include <cstring>


GameManager* GameManager::SP;


GameManager::GameManager()
{
    SP = this;
    currentMoney = 0;
    currentSecsElapsed = 0;
    totalHappiness = 0;
    loadedGame = false;
    unlockedBuildings = CCArray::create();
    unlockedBuildings->retain();
    lockedBuildings = CCArray::create();
    lockedBuildings->retain();
    
    researchableBuildings = CCArray::create();
    researchableBuildings->retain();
    
    areaUnlockIndex = 0;
    
    tutorialMode = false;
    
    loadedGameArea = CCArray::create();
    loadedGameArea->retain();
    
    init();
}

GameManager::~GameManager()
{
    SP = NULL;
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
    
    firstPlay = CCUserDefault::sharedUserDefault()->getBoolForKey("firstPlay", true);
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
            if (found==std::string::npos) continue;
           
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
            //rootseq->m_Children.push_back(buildTreeWithJsonValue(json["items"].operator[](i)));
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
            //rootseq->m_Children.push_back(buildTreeWithJsonValue(json["items"].operator[](i)));
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
   // if (!loadedGame)
        NewGameUnlocks();
   // else
//        loadGameData();

    GameHUD::getThis()->updateMoneyLabel();

    // Init arrays of locked and unlocked buildings
    for (list<int>::iterator it = unlockedBuildingIDs.begin(); it != unlockedBuildingIDs.end(); it++)
    {
        CCLog("%d", (*it));
        unlockedBuildings->addObject(GameScene::getThis()->buildingHandler->getBuilding(*it));
    }
    for (list<int>::iterator it = researchableBuildingIDs.begin(); it != researchableBuildingIDs.end(); it++)
        researchableBuildings->addObject(GameScene::getThis()->buildingHandler->getBuilding(*it));
    
    
    lockedBuildings->initWithArray(GameScene::getThis()->buildingHandler->allBuildings);
    lockedBuildings->removeObjectsInArray(unlockedBuildings);
    lockedBuildings->removeObjectsInArray(researchableBuildings);
}

bool GameManager::checkGameDataExists(){
    
    std::string test = CCFileUtils::sharedFileUtils()->getWriteablePath();
    test += "mayansave.txt";
    std::ifstream readFile(test.c_str());
    
    if (readFile)
    {
        readFile.close();
        return true;
    }
    return false;
}

void GameManager::loadGameData()
{
    CCLog("Loading game data");
    
    
    std::string test = CCFileUtils::sharedFileUtils()->getWriteablePath();
    test += "mayansave.txt";
    std::ifstream readFile;
    std::string out;

    readFile.open(test.c_str());
    
    int mode = 0;
    
    
    if (readFile.is_open())
    {
        while (!readFile.eof())
        {
            std::getline(readFile, out);
            
            if (out.compare("[GAMEMANAGER]") == 0)
            {
                mode = 1;
                continue;
            }
            if (out.compare("[PEOPLE]") == 0)
            {
                mode = 2;
                continue;
            }
            if (out.compare("[BUILDING]")==0)
            {
                mode = 3;
                continue;
            }
            if (out.compare("[MAP]")==0)
            {
                mode = 4;
                continue;
            }
            if (out.compare("[UNLOCKS]")==0)
            {
                mode = 5;
                continue;
            }
            if (out.compare("[RESEARCHS]")==0)
            {
                mode = 6;
                continue;
            }
            if (out.compare("[MENUITEMUNLOCKS]")==0)
            {
                mode = 7;
                continue;
            }


            std::vector<std::string> tokens;
            std::stringstream ss(out);
            std::string buffer;
            
            
            switch (mode)
            {
                case 1:
                {
                    if (out.length() == 0) break;
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                        
                    }
                    
                    currentMoney = atoi(tokens[0].c_str());
                   areaUnlockIndex = atoi(tokens[1].c_str());
                                    
                    
                    
                    GameTimer::getThis()->setTime(atof(tokens[2].c_str()), atof(tokens[3].c_str()));
                    
            
                    
                    //birth rate restore
                    int isImplementingPolicy = atoi(tokens[4].c_str());
                    if (isImplementingPolicy == 1)
                    {
                        GameScene::getThis()->policyHandler->setBirthRatePolicy(atoi(tokens[6].c_str()) - 1, atoi(tokens[5].c_str()));
                    }
                    isImplementingPolicy = atoi(tokens[7].c_str());
                    if (isImplementingPolicy == 1)
                    {
                        GameScene::getThis()->policyHandler->setCulturalExchangePolicy(atoi(tokens[9].c_str())-1, atoi(tokens[8].c_str()));
                    }
                    GameScene::getThis()->policyHandler->maxAlienPopulation = atoi(tokens[10].c_str());
                    
                    GameScene::getThis()->policyHandler->percentTax = atoi(tokens[11].c_str());
                    
                    //tutorial restore
                    TutorialHandler::getThis()->setSequenceIndex(atoi(tokens[12].c_str()));
                }
                    break;
                case 2:
                {
                    if (out.length() == 0) break;
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                      
                    }
                    GameSprite* sp = GameScene::getThis()->spriteHandler->getSpriteTemplate(tokens[0].c_str(), tokens[1].c_str()[0], tokens[2].c_str()[0]);
                    if (sp != NULL)
                    {
                        CCPoint target = ccp(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));
                        GameScene::getThis()->spriteHandler->loadSpriteToMap(target, sp, tokens[5]);
                    }
                    
                    
                }
                break;
                case 3:
                {
                    if (out.length() == 0) break;
                    
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                                
                    }
                    Building* b = GameScene::getThis()->buildingHandler->getBuilding(tokens[0]);
                    if (b != NULL)
                    {
                        CCPoint target = ccp(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()));
                        
                        GameScene::getThis()->mapHandler->Build(target, b, true, tokens[3]);
                        
                    }
                    
               
                    
                    break;
                }
                case 4:
                {
                    if (out.length() == 0) break;
                    
                    std::string buffer;
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                    }
                    
                    if (atoi(tokens[2].c_str()) == 1)
                    {
                    
                        CCPoint target = ccp(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()));
                        MapTile* targetTile = GameScene::getThis()->mapHandler->getTileAt(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()));
                        targetTile->pathHere();
                        GameScene::getThis()->mapHandler->pathTiles->addObject(targetTile);
                        CCTMXLayer* groundzero = GameScene::getThis()->mapHandler->getMap()->layerNamed("Ground_0");
                        groundzero->setTileGID(3, target); //exact GID of map tile. Count from L to R, U to Down in TMX tile map, starting from 1.
                    }
                    
                    
                    break;
                }
                case 5:
                {
                    if (out.length() == 0) break;
                    std::string buffer;
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                    }
                    
                    for (int i = 0; i < tokens.size(); ++i)
                    {
                        int targetID = atoi(tokens[i].c_str());
                        unlockedBuildingIDs.push_back(targetID);
                        ((Building*)GameScene::getThis()->buildingHandler->allBuildings->objectAtIndex(targetID))->unlockToBuild();
                        
                        
                    }

                    
                    
                    break;
                }
                case 6:
                {
                    if (out.length() == 0) break;
                    std::string buffer;
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                    }
                    
                    for (int i = 0; i < tokens.size(); ++i)
                    {
                        int targetID = atoi(tokens[i].c_str());
                        researchableBuildingIDs.push_back(targetID);
                        ((Building*)GameScene::getThis()->buildingHandler->allBuildings->objectAtIndex(targetID))->unlockToResearch();
                        
                        
                    }
                    break;
                }
                case 7:
                {
                    if (out.length() == 0) break;
                    std::string buffer;
                    while (ss.peek() != EOF)
                    {
                        std::getline(ss, buffer, ':' );
                        
                        tokens.push_back(buffer);
                        
                        buffer = "";
                    }
                    
                    for (int i = 0; i < tokens.size(); ++i)
                    {
                        fliplockMenuItem(i, atoi(tokens[i].c_str()));
                                                
                        
                    }
                    break;

                }
            }
            
            
            CCLog(out.c_str());
        }
    }
    
    readFile.close();
    //PushScene mainscene is NOT supposed to be here!!!!
}

void GameManager::newGameData()
{
    CCLog("Creating new data..");
    //playarea_min = ccp(1, 10);
    //playarea_max = ccp(16, 25);
    areaUnlockIndex = 0;
    currentMoney = starting_capital;
}

bool GameManager::saveGameData()
{
    CCLog("Saving data");
    /*
    //Flag for tutorial
    CCUserDefault::sharedUserDefault()->setBoolForKey   ("firstPlay", firstPlay);
    
    std::string test = CCFileUtils::sharedFileUtils()->getWriteablePath();
    test += "mayansave.txt";
    
    std::ofstream writeFile;
    writeFile.open(test.c_str());
    
    writeFile << "[GAMEMANAGER]\n";
    writeFile << GlobalHelper::Convert(currentMoney) +":";
    writeFile << GlobalHelper::Convert(areaUnlockIndex) +":";
    writeFile << GlobalHelper::Convert(GameTimer::getThis()->getTimeElapsed()) +":";
    writeFile << GlobalHelper::Convert(GameTimer::getThis()->getLastTime()) +":";
    
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->isImplementingBirthRatePolicy()) +":";
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->getBirthRateDurationLeft()) +":";
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->monthly_localBirthRate) +":";
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->isImplementingCulturalExchangePolicy()) +":";
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->getCulturalExchangeDurationLeft())+":";
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->getCulturalExchangeRatio())+":"; //cultural exchange rate
    
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->maxAlienPopulation) +":";
    
    writeFile << GlobalHelper::Convert(GameScene::getThis()->policyHandler->percentTax)+":";
    
    writeFile << GlobalHelper::Convert(TutorialHandler::getThis()->getSequenceIndex())+":";
    
    
    
    
    writeFile << "\n";
    
    //TODO: building locks and unlocks
    writeFile << "[UNLOCKS]\n";
    
    for (int i = 0; i < unlockedBuildings->count(); ++i)
    {
        writeFile << GlobalHelper::Convert(((Building*)unlockedBuildings->objectAtIndex(i))->ID);
        if (i != (unlockedBuildings->count()-1)) writeFile << ":";
        
        
    }
    writeFile << "\n";
    writeFile << "[MENUITEMUNLOCKS]\n";
    
    for (int i = 0; i < gameMenuUnlocks.size(); ++i)
    {
        writeFile << GlobalHelper::Convert(gameMenuUnlocks[i].isUnlocked);
        if (i != (gameMenuUnlocks.size() - 1)) writeFile << ":";
        
    }
    
    //TODO: research locks and unlocks
    
    writeFile << "\n";
   
    writeFile << "[RESEARCHS]\n";
    for (int i = 0; i < researchableBuildings->count(); ++i)
    {
        writeFile << GlobalHelper::Convert(((Building*)researchableBuildings->objectAtIndex(i))->ID);
        if (i < researchableBuildings->count() -1) writeFile << ":";
    }
    writeFile << "\n";

    BuildingHandler* bh = GameScene::getThis()->buildingHandler;
    writeFile << "[BUILDING]\n";
    for (int i = 0; i < bh->allBuildingsOnMap->count(); ++i)
    {
        Building* b = (Building*) bh->allBuildingsOnMap->objectAtIndex(i);
        if (b != NULL)
        {
            
 
            CCPoint bPosition = b->getWorldPosition();
            bPosition = GameScene::getThis()->mapHandler->tilePosFromLocation(bPosition);
            
            
            writeFile << b->buildingName +":" + GlobalHelper::Convert(bPosition.x) +":" + GlobalHelper::Convert(bPosition.y) +":" +GlobalHelper::Convert(b->currentExp) +"|" + GlobalHelper::Convert(b->currentLevel) + "|" + GlobalHelper::Convert(b->ID) + "\n";
            
        }
            
    }
 
    writeFile << "[PEOPLE]\n";
    SpriteHandler* sh = GameScene::getThis()->spriteHandler;
    for (int i = 0; i < sh->spritesOnMap->count(); ++i)
    {
        GameSprite* sp = (GameSprite*)sh->spritesOnMap->objectAtIndex(i);
        if (sp != NULL)
        {
            CCPoint sPosition = sp->getWorldPosition();
            sPosition = GameScene::getThis()->mapHandler->tilePosFromLocation(sPosition);
            
            
            int homeID = 0, workID = 0;
            if (sp->getPossessions()->hasHouse)
                homeID = sp->getPossessions()->homeLocation->ID;
            else
                homeID = -1;
            
            if (sp->getPossessions()->hasJob)
                workID = sp->getPossessions()->jobLocation->ID;
            else
                workID = -1;
            
            writeFile << sp->spriteClass +":" +sp->gender +":" + sp->race + ":" +GlobalHelper::Convert(sPosition.x)+ ":" +GlobalHelper::Convert(sPosition.y) +":" +"|" + GlobalHelper::Convert(sp->getPossessions()->happinessRating) +"|" + GlobalHelper::Convert(sp->getPossessions()->loyaltyRating) +"|" + GlobalHelper::Convert(sp->getPossessions()->movementRange) + "|" + GlobalHelper::Convert(sp->getPossessions()->energyRating) +"|" +
            GlobalHelper::Convert(sp->getPossessions()->classLevel) + "|" + GlobalHelper::Convert(sp->getPossessions()->expRating) + "|" + GlobalHelper::Convert(sp->getPossessions()->hasHouse) + "|"+ GlobalHelper::Convert(homeID) + "|" + GlobalHelper::Convert(workID) +
            "\n";
        }
        
        
    }

    
    writeFile << "[MAP]\n";
    MapHandler* mh = GameScene::getThis()->mapHandler;
    for (int i = 0; i < mh->getMap()->getMapSize().width; ++i)
    {
        for (int j = 0; j < mh->getMap()->getMapSize().height; ++j)
        {
            MapTile* tile = mh->getTileAt(i, j);
            if (tile->isPath)
            {
                writeFile << GlobalHelper::Convert(i) +":" + GlobalHelper::Convert(j) +":" + GlobalHelper::Convert(1) +"\n";
            }
            else
            {
                writeFile << GlobalHelper::Convert(i) +":" + GlobalHelper::Convert(j) +":" + GlobalHelper::Convert(0) +"\n";
                
            }
            
            
        }
    }
    
    
    
    writeFile.close();

    */
    
    CCLog("Saved data");
    return true;
}


CCPoint GameManager::getMinArea()
{
    return areaUnlocks[areaUnlockIndex].min;
    
}

CCPoint GameManager::getMaxArea()
{
    return areaUnlocks[areaUnlockIndex].max;
}


int GameManager::getCurrentMoney()
{
    return currentMoney;
}

float GameManager::getCurrentTime()
{
    return currentSecsElapsed;
}

int GameManager::getAverageHappiness()
{
    return (int)(totalHappiness / (float)GameScene::getThis()->spriteHandler->spritesOnMap->count()) /10;
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
    
    return b->hasMetUnlockCriteria();
    
}


void GameManager::UpdateUnlocks()
{
    //check map area unlocks
    GameScene* scene = GameScene::getThis();
    
    if (scene->buildingHandler->allBuildingsOnMap == NULL || scene->spriteHandler->spritesOnMap == NULL) return;
   
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
   
    if (TutorialHandler::getThis()->IsActive()) return; //do not update unlocks when tutorial is active.
    
    //menuitem unlocks only run outside of the tutorial.
    for (int i = 0; i < gameMenuUnlocks.size(); ++i)
    {
        if (gameMenuUnlocks[i].hasMetCriteria(currentMoney))
        {
            fliplockMenuItem(i, true);
        }
    }

}

void GameManager::NewGameUnlocks()
{
    /*builds the unlock ID list for a new game. This should not be called when loading from a save*/
    CCArray* allBuildings = GameScene::getThis()->buildingHandler->allBuildings;
    
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
    
    CCArray* allBuildings = GameScene::getThis()->buildingHandler->allBuildings;
    
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
                b->unlockToResearch();
                researchableBuildingIDs.push_back(b->ID);
            }
            else
            {
                b->unlockToBuild();
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
    GameScene::getThis()->mapHandler->updatePlayArea(areaUnlocks[areaUnlockIndex].min , areaUnlocks[areaUnlockIndex].max);
    
}

void GameManager::setTutorialMode(bool mode)
{
    tutorialMode = mode;
}

bool GameManager::getTutorialMode()
{
    return tutorialMode;
}


void GameManager::setLoadedGame(bool mode)
{
    loadedGame = mode;
}

bool GameManager::getLoadedGame()
{
    return loadedGame;
}


bool GameManager::hasLostGame()
{
    //check lose condition
    if (getCurrentMoney() <= lose_condition_money ||
        getAverageHappiness() <= lose_condition_average_happiness)
        
        
        
        return true;
    
    return false;

}

void GameManager::addMaxStorageValue(Building *justbuilt)
{
    maxStorageVal += justbuilt->storageLimit;
}

void GameManager::subtractMaxStorageValue(Building *justdestroyed)
{
    maxStorageVal -= justdestroyed->storageLimit;
    //additional food over limit is wasted.
    if (currStorageVal > maxStorageVal)
        currStorageVal = maxStorageVal;
}

void GameManager::storage_store(int amt)
{
    currStorageVal += amt;
    //additional food over limit is wasted.
    if (currStorageVal > maxStorageVal)
        currStorageVal = maxStorageVal;
}

void GameManager::storage_consume(int amt)
{
    currStorageVal -= amt;
    if (currStorageVal < 0) currStorageVal = 0;
}
bool GameManager::hasStorageLeft()
{
    return (currStorageVal < maxStorageVal);
}