//
//  NanhuaGameStaticAPI.cpp
//  PopulationTest
//
//  Created by admin on 27/8/14.
//
//

#include "NanhuaGameStaticAPI.h"
#include "GameScene.h"

int NanhuaGameStaticAPI::getCurrentCitizenNumber()
{
    
}

float NanhuaGameStaticAPI::getAverageHappiness()
{
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    float totalHappiness = 0;
    int villagerCount = 0;
    
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        if(gs->villagerClass != V_BANDIT)
        {
            totalHappiness += gs->getPossessions()->happinessRating;
            villagerCount++;
        }
    }
    float average_happiness = totalHappiness / (float) villagerCount;
        
    return average_happiness;
}

int NanhuaGameStaticAPI::getNumberOfVillagers()
{
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    int temp = 0;
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_CLASS_END)
        {
            temp++;
        }
    }
    return temp;
}

int NanhuaGameStaticAPI::getNumberOfCitizens()
{
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    int tempCitizen = 0;
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        if(gs->getHome() != NULL)
        {
            tempCitizen++;
        }
    }
    return tempCitizen;
}