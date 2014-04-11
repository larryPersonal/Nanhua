//
//  TutorialSlide.cpp
//  PopulationTest
//
//  Created by admin on 10/4/14.
//
//

#include "TutorialSlide.h"

TutorialSlide::TutorialSlide()
{
    speech = "";
    cameraMove = false;
    target = CCPointZero;
    dragonMove = false;
    moveVector = CCPointZero;
    
    highlight = false;
    deHighlight = false;
    highlightBuilding = "";
    deHighlightBuilding = "";
    
    fade_in = false;
    fade_out = false;
    target_opacity = 0;
    
    clickToNext = false;
    offY = 0;
    addVillager = 0;
    
    hide = false;
    show = false;
    
    triggerDropToken = false;
    unlockAll = false;
    hideBubble = false;
}

TutorialSlide::~TutorialSlide()
{
    
}

TutorialSlide* TutorialSlide::create()
{
    TutorialSlide *pRet = new TutorialSlide();
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}