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
#include "UserProfile.h"

/* handle pause button: (GameHUD.h)*/
void UIButtonControl::pauseGame()
{
    if(!GameHUD::getThis()->pause)
    {
        GameHUD::getThis()->pause = true;
        GameHUD::getThis()->pauseButton->setVisible(false);
        GameHUD::getThis()->resumeButton->setVisible(true);
    }
}

void UIButtonControl::resumeGame()
{
    if(GameHUD::getThis()->pause)
    {
        GameHUD::getThis()->pause = false;
        GameHUD::getThis()->pauseButton->setVisible(true);
        GameHUD::getThis()->resumeButton->setVisible(false);
        CCArray* spritesOnMap = SpriteHandler::getThis()->spritesOnMap;
        
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
    if(GameHUD::getThis()->targetMoney >= 2400 && GameHUD::getThis()->targetFood >= 800)
    {
        GameHUD::getThis()->scheduleAddMoney(-2400);
        GameHUD::getThis()->scheduleAddFood(-800);
        UserProfile::getThis()->number_of_hospital++;
        
        if(UserProfile::getThis()->number_of_hospital == 2)
        {
            NotificationPopup::getThis()->showScenario3Congratulations();
            // GameHUD::getThis()->scheduleOnce(schedule_selector(GameHUD::clickScoreButton), 5.0f);
        }
    }
}