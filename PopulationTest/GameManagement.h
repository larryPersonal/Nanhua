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
    static void saveGameToFile();
    static void loadGameFile();
};

#endif
