//
//  BuildingCard.h
//  PopulationTest
//
//  Created by admin on 13/2/14.
//
//

#ifndef PopulationTest_BuildingCard_h
#define PopulationTest_BuildingCard_h

#include "cocos2d.h"
#include "Building.h"
#include "ScrollArea.h"
using namespace cocos2d;

class BuildingCard: CCObject
{
private:
    Building* building;
    ScrollArea* scrollArea;
    
    CCLabelTTF* buildingNameLabel;
    CCSprite* buildingImage;
    CCMenuItemImage* buildingInfoButton;
    
    CCSprite* costImage;
    CCLabelTTF* costLabel;
    
    CCSprite* populationImage;
    CCLabelTTF* populationLabel;
    
    CCSprite* buildingTimeImage;
    CCLabelTTF* buildingTimeLabel;
    
public:
    BuildingCard(Building*, ScrollArea*);
    ~BuildingCard();
    
    static BuildingCard* create(Building*, ScrollArea*);
    
    void init();
    
    void refreshAllMenuItems();
    
    void showBuildingInfo();
};

#endif
