//
//  GameManagement.h
//  PopulationTest
//
//  Created by admin on 28/5/14.
//
//

#ifndef PopulationTest_GameManagement_h
#define PopulationTest_GameManagement_h

#include "cocos2d.h"
#include "CCUserDefault.h"

using namespace cocos2d;

class GameManagement
{
public:
    static void saveGameToFile(int type);
    static void saveData(int type);
    static void saveSpritesData(int type);
    static void saveBuildingData(int type);
    static void saveSystemData(int type);
    static void saveRoadData(int type);
    static void saveReputationOrbData(int type);
    static void saveObjectiveData(int type);
    static void saveTutorialData(int type);
    
    static bool loadData(int type);
    static void loadSystemData(int type);
    static void loadBuildingData(int type);
    static void loadSpritesData(int type);
    static void loadSpritesLinks(int type);
    static void loadRoadData(int type);
    static void loadReputationOrbData(int type);
    static void loadObjectiveData(int type);
    static void loadTutorialData(int type);
    
    static void loadGameFile();
};

#endif
