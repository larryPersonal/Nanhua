//
//  UIButtonControl.cpp
//  PopulationTest
//
//  Created by admin on 20/8/14.
//
//

#include "UIButtonControl.h"
#include "GameHUD.h"
#include "GameScene.h"
#include "Sprite.h"
#include "NotificationPopup.h"

/* handle pause button: (GameHUD.h)*/
void UIButtonControl::pauseGame()
{
    if(!GameHUD::getThis()->pause)
    {
        GameHUD::getThis()->pause = true;
        GameHUD::getThis()->pauseButton->setVisible(false);
        GameHUD::getThis()->pauseButton->setPosition(ccp(GameHUD::getThis()->pauseButton->getPositionX(), GameHUD::getThis()->pauseButton->getPositionY() - 500));
        GameHUD::getThis()->resumeButton->setVisible(true);
        GameHUD::getThis()->resumeButton->setPosition(ccp(GameHUD::getThis()->resumeButton->getPositionX(), GameHUD::getThis()->resumeButton->getPositionY() + 500));
    }
}

void UIButtonControl::resumeGame()
{
    if(GameHUD::getThis()->pause)
    {
        GameHUD::getThis()->pause = false;
        GameHUD::getThis()->pauseButton->setVisible(true);
        GameHUD::getThis()->pauseButton->setPosition(ccp(GameHUD::getThis()->pauseButton->getPositionX(), GameHUD::getThis()->pauseButton->getPositionY() + 500));
        GameHUD::getThis()->resumeButton->setVisible(false);
        GameHUD::getThis()->resumeButton->setPosition(ccp(GameHUD::getThis()->resumeButton->getPositionX(), GameHUD::getThis()->resumeButton->getPositionY() - 500));
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
            sp->followPath();
        }
    }

}

void UIButtonControl::clickPauseButton()
{
    if(GameHUD::getThis()->pause)
    {
        resumeGame();
    }
    else
    {
        pauseGame();
    }
}

void UIButtonControl::clickBuildHospitalButton()
{
    if(GameHUD::getThis()->targetMoney >= 1000 && GameHUD::getThis()->targetFood >= 0)
    {
        GameHUD::getThis()->scheduleAddMoney(-1000);
        GameHUD::getThis()->scheduleAddFood(0);
        GameManager::getThis()->number_of_hospital++;
        
        if(GameManager::getThis()->number_of_hospital == 2)
        {
            NotificationPopup::getThis()->showScenario3Congratulations();
            // GameHUD::getThis()->scheduleOnce(schedule_selector(GameHUD::clickScoreButton), 5.0f);
        }
    }
}