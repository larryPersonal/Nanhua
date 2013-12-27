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
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

class GameSprite;

enum BuildingCategory {
    HOUSING = 0,
    AMENITY,
    COMMERCE,
    MILITARY,
    EDUCATION,
    SOCIAL,
    SPECIAL,
    BUILDINGCATEGORYMAX = 8
};


class Building: public CCObject
{
  //  CCArray* jobsInthisBuilding;
    CCArray* currVisitors;
    
    vector<GameSprite*> currPopulation;
    
    void ModifyStats(GameSprite* sp);
    
public:
    Building();
   ~Building();
    
    static Building* create();
    
    virtual Building* copyWithZone(CCZone *pZone);
    
    int ID;
    
    CCSprite* buildingRep;
     
    BuildingCategory buildingType;
    int populationLimit;
    //int populationOverload;
    //int populationOverloadLimit;
    
    int buildingValue;
    int buildingBuyPrice;
    //int buildingRentPrice;
     
    int targetGUID;
    int GUID_offset;
    
    int width;
    int height;
    
    /*modifications to anyone entering building*/
    //int happiness_mod;
    //int loyalty_mod;
    //int social_mod;
    //int int_mod;
    
    //int energy_mod;
    //int cash_mod;
   
    //int exp_mod;
    /*end modifications to anyone entering building*/
    
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
    
    CCPoint getWorldPosition();
    
    /*jobs*/
 //   std::string jobs;
  //  void initializeJobs();
    
    
  //  CCArray* getJobsAvailable();
   // bool isQualifiedForJob(GameSprite* sp, int jobindex);
    
    /*called when the sprite arrives in the building.*/
    void ArriveHandler(GameSprite* sp);
    /*called when the sprite hangs around in the building.*/
    void StickAroundHandler(GameSprite* sp);
    /*called when the sprite leaves the building it was in*/
    void Leavehandler(GameSprite* sp);
    
    // Population helper funcs
    void addPopulation(GameSprite* sprite);
    void removePopulation(GameSprite* sprite);
    int getPopulationCount();
    GameSprite* getPopulationAt(int index);
    
    
    int getLevel();
    
    void unlockToResearch();
    void unlockToBuild();
    
    bool hasMetUnlockCriteria();
    bool isOverpopulated();
    
    void BeginAnim();
    void EndAnim();
    void AnimUpdate();
    
    void ChangeAppearance(Building* b);
    
    int getOverpopulateCount();
    int getAlienPopulationCount();
    
    int getUnoccupiedCount();

};

#endif
