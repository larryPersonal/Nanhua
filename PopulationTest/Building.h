//
//  Building.h
//  PopulationTest
//
//  Created by Serious Games on 13/3/13.
//
//

#ifndef PopulationTest_Building_h
#define PopulationTest_Building_h

#include "cocos2d.h"
//#include "SimpleAudioEngine.h"

using namespace cocos2d;
//using namespace CocosDenshion;
using namespace std;

class GameSprite;

enum BuildingCategory {
    HOUSING = 0,
    GRANARY,
    AMENITY,
    COMMERCE,
    MILITARY,
    EDUCATION,
    SOCIAL,
    SPECIAL,
    BUILDINGCATEGORYMAX = 8
};

enum FarmState {
    WAITING,
    FARM,
    HARVEST
};


class Building: public CCObject
{
    vector<GameSprite*> currPopulation;
    
public:
    Building();
   ~Building();
    
    static Building* create();
    
    virtual Building* copyWithZone(CCZone *pZone);
    
    int ID;
    
    bool inProgress;
    
    CCSprite* buildingRep;
    BuildingCategory buildingType;
    FarmState farmState;
    int populationLimit;
    int builderLimit;
    
    // for food, one for the current storage amount, the other for the limitation of storage.
    int currentStorage;
    int storageLimit;
    
    int build_uint_required;
    int build_uint_current;
    
    int work_unit_required;
    float work_unit_current;
    
    int recovery_rate;
    int food_consumption_rate;
    
    int buildingValue;
    int buildingBuyPrice;
    //int buildingRentPrice;
     
    int targetGUID;
    int GUID_offset;
    
    int width;
    int height;
    
    /*unlock requirements*/
    /*unlock requirements will only exist in allBuildings. Anything on the map will have these values set to 0.*/
    int required_population;
    int required_capita;
    int required_building_count;
    bool unlocked;
    /*end unlock requirements*/
    
   
    //Building exp
    int currentExp;
    int currentLevel;
    CCArray* expToLevel;
    int getExpToLevel();
    void gainExp(int expToGain);
    
    //Level up stat gain (This is stored in the real building, not in copies)
    CCDictionary* levelGainVacancy;
    CCDictionary* levelGainPopOverload;
    CCDictionary* levelGainPrice;
    
    //Building desc
    std::string buildingName;
    std::string buildingDesc;
    std::string targetLayerName;
    std::string prereq;
   
    CCTexture2D * buildingTexture;
    CCRect buildingRect;
    
    int buildingCost;
    int researchCost;
    int maintCost;
    //and probably other stuff here.
    float researchTime;
    float constructionTime;
    float constructionEndTime;
    
    
    // jerry added
    CCArray* memberSpriteList;
    
    CCPoint getWorldPosition();
    
    /*jobs*/
    int number_of_jobs;
    
    /*called when the sprite arrives in the building.*/
    void ArriveHandler(GameSprite* sp);
    /*called when the sprite hangs around in the building.*/
    void StickAroundHandler(GameSprite* sp, float time);
    /*called when the sprite leaves the building it was in*/
    void Leavehandler(GameSprite* sp);
    
    void ChangeAppearance(Building* b);
    
    void leaveHouse(GameSprite*);
    
    bool isUnderConstruction();
    bool hasFood();
    bool notHavest();
    
    void leaveGranuary(GameSprite*);
};

#endif
