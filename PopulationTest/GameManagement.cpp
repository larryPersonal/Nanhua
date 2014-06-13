//
//  GameManagement.cpp
//  PopulationTest
//
//  Created by admin on 28/5/14.
//
//

#include "GameManagement.h"
#include "GameScene.h"
#include "GameManager.h"
#include "GameHUD.h"

void GameManagement::saveGameToFile()
{
    CCLog("try saving......");
    GameScene::getThis()->saveData();
    CCLog("saving success!");
}

void GameManagement::loadGameFile()
{
    CCLog("loading file......");
    int level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", -1);
    std::string map = CCUserDefault::sharedUserDefault()->getStringForKey("map", "");
    int time = CCUserDefault::sharedUserDefault()->getIntegerForKey("time", -1);
    int gold = CCUserDefault::sharedUserDefault()->getIntegerForKey("gold", -1);
    int reputation = CCUserDefault::sharedUserDefault()->getIntegerForKey("reputation", -1);
    
    if(level >= 0)
    {
        CCLog("level is: %d", level);
    }
    else
    {
        CCLog("level is: no data");
    }
    
    if(map.compare("") != 0)
    {
        CCLog("map is: %s", map.c_str());
    }
    else
    {
        CCLog("map is: no data");
    }
    
    if(time >= 0)
    {
        CCLog("time is: %d", time);
    }
    else
    {
        CCLog("time is: no data");
    }
    
    if(gold >= 0)
    {
        CCLog("gold is: %d", gold);
    }
    else
    {
        CCLog("gold is: no data");
    }
    
    if(reputation >= 0)
    {
        CCLog("reputation is: %d", reputation);
    }
    else
    {
        CCLog("reputation is: no data");
    }
    CCLog("loading finish!");
}