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
    lockClick = false;
    
    offY = 0;
    addVillager = 0;
    
    hide = false;
    show = false;
    
    triggerDropToken = false;
    unlockAll = false;
    hideBubble = false;
    
    showArrow = false;
    hideArrow = false;
    
    arrowAngle = 0;
    arrowX = 0;
    arrowY = 0;
    arrowScale = 0;
    arrowLayer = 0;
    
    contentOffX = 0;
    contentOffY = 0;
    
    stopScroll = false;
    resumeScroll = false;
    hideScroll = -1;
    showScroll = -1;
    
    checkGranary = false;
    checkFarm = false;
    notFirst = false;
    
    showObjective = false;
    hideObjective = false;
    
    connectHouse = false;
    connectGranary = false;
    connectFarm = false;
    
    waitForVillager = false;
    
    delay = 0;
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