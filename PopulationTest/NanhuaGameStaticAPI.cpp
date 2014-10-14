//
//  NanhuaGameStaticAPI.cpp
//  PopulationTest
//
//  Created by admin on 27/8/14.
//
//

#include "NanhuaGameStaticAPI.h"
#include "GameScene.h"
#include "GameHUD.h"

int NanhuaGameStaticAPI::getCurrentCitizenNumber()
{
    return 0;
}

float NanhuaGameStaticAPI::getAverageHappiness()
{
    CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
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
    CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
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
    CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
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

// bandtis attack
void NanhuaGameStaticAPI::banditsAttack(int banditsNo)
{
    if(!GameHUD::getThis()->isAlerting && !GameHUD::getThis()->isAlertingText)
    {
        GameHUD::getThis()->redMask->setOpacity((GLubyte) 0);
        GameHUD::getThis()->redMask->setVisible(true);
        GameHUD::getThis()->alertText->setScale(0);
        GameHUD::getThis()->alertText->setVisible(true);
        GameHUD::getThis()->isAlertingText = true;
        GameHUD::getThis()->alertTextFadeIn = true;
        GameHUD::getThis()->alertCumulativeTime = 0;
        GameHUD::getThis()->schedule(schedule_selector(GameHUD::alertBanditsAttackText), 1.0f / 120.0f);
        
        GameHUD::getThis()->peaceButton->setVisible(true);
        GameHUD::getThis()->warButton->setVisible(false);
        BanditsAttackHandler::getThis()->startWar();
        BanditsAttackHandler::getThis()->banditsLeft = banditsNo;
        
        // all villages will find a place to hide when bandits are comming....
        CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
        for (int i = 0; i < allSprites->count(); i++)
        {
            GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
            if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_CLASS_END)
            {
                if(gs->villagerClass == V_REFUGEE)
                {
                    Building* b = gs->findNearestHouse();
                    gs->GoBuilding(b);
                }
                else
                {
                    gs->GoBuilding(gs->getHome());
                }
            }
        }
    }
}

// bandits end attack
void NanhuaGameStaticAPI::banditsEndAttack()
{
    if(!GameHUD::getThis()->isAlerting && !GameHUD::getThis()->isAlertingText)
    {
        GameHUD::getThis()->peaceButton->setVisible(false);
        GameHUD::getThis()->warButton->setVisible(true);
        BanditsAttackHandler::getThis()->stopWar();
        
        // resume all villages after the bandits attack
        CCArray* allSprites = SpriteHandler::getThis()->spritesOnMap;
        for(int i = 0; i < allSprites->count(); i++)
        {
            GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
            if(gs->villagerClass != V_CLASS_END)
            {
                if(gs->villagerClass == V_BANDIT)
                {
                    gs->tryEscape = true;
                    gs->escape();
                }
                else if(gs->villagerClass == V_REFUGEE || gs->villagerClass == V_CITIZEN)
                {
                    gs->StopMoving();
                    gs->currAction = IDLE;
                }
                else
                {
                    gs->GoBuilding(gs->getPossessions()->jobLocation);
                }
            }
        }

    }
}