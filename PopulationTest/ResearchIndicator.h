//
//  ResearchIndicator.h
//  PopulationTest
//
//  Created by Ong Heng Le on 10/5/13.
//
//

#ifndef __PopulationTest__ResearchIndicator__
#define __PopulationTest__ResearchIndicator__

#include "cocos2d.h"
#include "ProgressBar.h"

using namespace cocos2d;

/*
 * Because the player can only research one at a time,
 * I just have to fetch the first researching item.
 *
 * Multiple researches are still kept for future extensions.
 *
 */
class ResearchIndicator:public cocos2d::CCLayer
{
private:
    
    CCSprite* buildingIcon;
    ProgressBar* progressBar;
    float researchTimeTotal;
public:
    
    ResearchIndicator();
    virtual ~ResearchIndicator();
    
    void createResearchIndicator();
    
    void updateResearchIndicator();
    
    virtual void update(float deltaTime);
    
};

#endif /* defined(__PopulationTest__ResearchIndicator__) */
