//
//  GameManager.h
//  PopulationTest
//
//  Created by Serious Games on 29/4/13.
//
//

#ifndef __PopulationTest__GameManager__
#define __PopulationTest__GameManager__

#include "cocos2d.h"
#include "GameScene.h"
#include <map.h>
#include <iterator>
#include <vector>

using namespace cocos2d;
using namespace std;


//I can't find a better way to integrate all the unlocks, unfortunately.
struct AreaUnlock
{
    CCPoint min;
    CCPoint max;
    
    std::string name;
    std::string prereqs;
    
    bool isUnlocked;
    float researchtime;
    int researchcost;
    
    int required_popn;
    int required_bcount;
    int required_cash;
    
    
    AreaUnlock()
    {
        required_bcount = required_cash = required_popn = 0;
        
        isUnlocked = false;
        prereqs = "";
        researchtime = 0.0f;
        researchcost = 0;
    }
    /*order of declaration means I can't use GameManager::getThis() here*/
    bool hasMetCriteria(int currCash)
    {
        if (prereqs.length() != 0)
        {
            istringstream iss(prereqs);
            vector<std::string> tokens;
            std::string token;
            
            while(std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            
            int truecount = 0;
            for (int i = 0; i < tokens.size(); ++i)
            {
                if (GameScene::getThis()->buildingHandler->getBuildingOnMapWithName(tokens[i]) != NULL)
                {
                    ++truecount;
                }
            }
            
            
            if (truecount != tokens.size()) return false;
        }
        //if prereqs.length = 0 assume true at this point.
        
        if (required_bcount > GameScene::getThis()->buildingHandler->allBuildingsOnMap->count()) return false;
        if (required_popn > GameScene::getThis()->spriteHandler->spritesOnMap->count()) return false;
        if (required_cash >currCash) return false;
        return true;
    }
    
};

struct MenuItemUnlock
{
    std::string name;
    std::string prereqs;
    
    bool isUnlocked;
    
    int required_popn;
    int required_bcount;
    int required_cash;
    
    MenuItemUnlock()
    {
        prereqs = "";
        isUnlocked = false;
        
        required_bcount = required_cash = required_popn = 0;
    }
    
    bool hasMetCriteria(int currCash)
    {
        if (required_bcount == -1 && required_cash == -1 && required_popn == -1) return false; //to prevent a menu item from ever being unlocked automatically, set these 3 to -1.
        //if this is the case it shouldn't ever check to see if the population amount matches. 
        
        if (required_bcount > GameScene::getThis()->buildingHandler->allBuildingsOnMap->count()) return false;
        if (required_popn > GameScene::getThis()->spriteHandler->spritesOnMap->count()) return false;
        if (required_cash >currCash) return false; 
        
        
        if (prereqs.length() == 0) return true;
        istringstream iss(prereqs);
        vector<std::string> tokens;
        std::string token;
        
        while(std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        
      
        
        int truecount = 0;
        for (int i = 0; i < tokens.size(); ++i)
        {
            if (GameScene::getThis()->buildingHandler->getBuildingOnMapWithName(tokens[i]) != NULL)
            {
                ++truecount;
            }
        }
        
        if (truecount == tokens.size()) return true;
        
        return false;

    
    }
};

struct HousingLimit
{
    int maximum_level;
    vector<int> gold_required;
    vector<int> food_required;
    vector<int> housing_limits;
    vector<int> granary_limits;
    vector<int> market_limits;
    vector<int> farm_limits;
    vector<int> guard_tower_limits;
    
    HousingLimit()
    {
        maximum_level = 0;
    }
};

class GameManager
{
public:
    static GameManager* SP;
    
    /* section that will consider to retain or discard */
    bool loadedGame;
    bool tutorialMode;
    
    
    bool UnlockConditionsMet(Building* b);
    bool ResearchConditionsMet(Building* b);
    void NewGameUnlocks();
    
    std::vector<AreaUnlock> areaUnlocks;
    std::vector<MenuItemUnlock> gameMenuUnlocks; //for in game menu only!
    
    int areaUnlockIndex;
    Json::Value root;
    Json::Reader reader;
    void initMapUnlocks();
    void initMenuUnlocks();
    
    int level;
    std::string gameMap;
    
public:
    void UnlockAll();
    
    GameManager();
    ~GameManager();
    static GameManager* getThis();
    void init();
    void initGameData();
    void EndGameData();
    
    bool checkGameDataExists();
    void loadGameData();
    void newGameData();
    bool saveGameData();
    
    
    // Game values
    bool firstPlay;
    CCArray* loadedGameArea;
    
    // Research building list
    list<int> unlockedBuildingIDs;
    list<int> researchableBuildingIDs;
    map<int, float> buildingIdResearchEndTime;
    
    CCArray* unlockedBuildings;
    CCArray* researchableBuildings;
    CCArray* lockedBuildings;
    
    
   
    void UpdateUnlocks();
    
    void fliplockMenuItem(int arrayIndex, bool locked);
    
    
    bool isMenuItemUnlocked(std::string itemName);
    
    CCPoint getMinArea();
    CCPoint getMaxArea();
    
    AreaUnlock* getNextAreaUnlock();
    void triggerNextAreaUnlock();
    
    void setTutorialMode(bool mode);
    bool getTutorialMode();
    
    
    void setLoadedGame(bool mode);
    bool getLoadedGame();
    
    bool hasLostGame();
    
    void setLevel(int);
    int getLevel();
    
    /* Jerry added */
    int town_hall_level;
    
    Json::Value hl;
    HousingLimit* housingLimitation;
    void parseHousingLimitation();
    bool parseHousingLimit(string);
    void loadHousingLimit();
};



#endif /* defined(__PopulationTest__GameManager__) */
