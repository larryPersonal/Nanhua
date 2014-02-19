//
//  CustomMenu.cpp
//  PopulationTest
//
//  Created by admin on 19/2/14.
//
//

#include "CustomMenu.h"

void CustomMenu::registerWithTouchDispatcheer()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 128, false);
}