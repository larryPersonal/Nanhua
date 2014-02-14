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
    CCMenuItemSprite* buildingImage;
    CCMenuItemImage* buildingInfoButton;
    
    CCSprite* costImage;
    CCLabelTTF* costLabel;
    
    CCSprite* populationImage;
    CCLabelTTF* populationLabel;
    
    CCSprite* buildingTimeImage;
    CCLabelTTF* buildingTimeLabel;
    
    int index;
    
    CCMenu* menu;
    CCArray* menuItemsArray;
    
public:
    BuildingCard(Building*, ScrollArea*, int);
    ~BuildingCard();
    
    static BuildingCard* create(Building*, ScrollArea*, int);
    
    void init();
    
    void refreshAllMenuItems();
    
    void showBuildingInfo();
    
    void onMenuItemSelected();
};

#endif
