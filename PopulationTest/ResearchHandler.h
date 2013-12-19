//
//  ResearchHandler.h
//  PopulationTest
//
//  Created by Ong Heng Le on 3/5/13.
//
//

#ifndef PopulationTest_ResearchHandler_h
#define PopulationTest_ResearchHandler_h

class ResearchHandler
{
    
private:
    
    float sleepTimeLeft;
    
    int researching_count;
    
public:
    
    ResearchHandler();
    ~ResearchHandler();
    virtual void update(float deltaTime);
    
    void addResearchingBuilding(int buildingID, float endTime);
    void completeResearchingBuilding(int buildingID);
    int getResearchingBuildingCount();
    float getResearchingBuildingTimeLeft(int buildingID);
    bool isAlreadyResearching(int buildingID);
    
};

#endif
