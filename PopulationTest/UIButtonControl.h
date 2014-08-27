//
//  UIButtonControl.h
//  PopulationTest
//
//  Created by admin on 20/8/14.
//
//

#ifndef PopulationTest_UIButtonControl_h
#define PopulationTest_UIButtonControl_h

#include "cocos2d.h"

using namespace cocos2d;

class UIButtonControl
{
public:
    static void pauseGame();                // pause the game play
    static void resumeGame();               // resume the game play (also resume the sprites' behaviours
    static void clickPauseButton();         // event listener for pauseButton
    
    static void clickBuildHospitalButton();
};

#endif
