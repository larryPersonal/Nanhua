//
//  BuildingHandler.h
//  PopulationTest
//
//  Created by Serious Games on 12/3/13.
//
//

#ifndef PopulationTest_BuildingHandler_h
#define PopulationTest_BuildingHandler_h

#include "Building.h"
#include "cocos2d.h"
#include "JobCollection.h"
/*this class must be initialized before the map!*/

class BuildingHandler
{
private:
    Building* BinarySearch(CCArray* targetArray, int targetID);
public:
    BuildingHandler();
    ~BuildingHandler();
    Building* selectedBuilding; //if in build mode, pointer to target building.
    
    Building* mainBuilding;
    
    CCArray * allBuildings;
    //for setup only
    CCArray * allBuildingLayers;
    
    //Ok, I think its simpler if I also had this so i can get the pointer immediately from ID.
    /*update: getters implemented due to possibility of demolishing buildings screwing with array order*/
    CCArray * allBuildingsOnMap;
    
    /*it'd probably be easier on the search and the head if I kept them as separate CCArrays.*/
    CCArray* housingOnMap;
    CCArray* amenityOnMap;
    CCArray* commerceOnMap;
    CCArray* militaryOnMap;
    CCArray* educationOnMap;
    CCArray* socialOnMap;
    
    CCArray* specialOnMap;
    
    /*Searches for anything that says "onMap"*/
    /* //I probably don't need these, but we'll see.
    Building* getHousingOnMapWithID(int withID);
    Building* getAmenityOnMapWithID(int withID);
    Building* getCommerceOnMapWithID(int withID);
    Building* getMilitaryOnMapWithID(int withID);
    Building* getEducationOnMapWithID(int withID);
    Building* getSocialOnMapWithID(int withID);
    */
    
    Building* getBuildingOnMapWithID(int withID);
    Building* getBuildingOnMapWithName(std::string name);
    
    /*This function makes sure to clean out the job and home flags before deleting the building.*/
    void EmptyBuildingFirst(Building* b);
    
    /*Searches in allBuildings*/
    Building* getBuilding(int withID);
    Building* getBuilding(std::string& buildingName);
    Building* getFirstBuildingOfCategory(BuildingCategory bc);
    Building* getBuildingWithGID(int GID);
    
    void init(CCTMXTiledMap* mapPtr, JobCollection* jc);
    
//    CCArray* getAllBuildingsWithCategory(std::string category);
//    CCArray* getUnoccupiedResidences();
//    CCArray* getJobsWithVacancies();
    
    void removeBuildingFromMap(Building* b);
    void addBuildingToMap(Building* b);
    
    int getHighestBuildingID();
    
    
    
    
    
};



#endif
