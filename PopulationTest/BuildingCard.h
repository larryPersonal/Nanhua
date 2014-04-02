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

class BuildingCard : public CCObject
{
public:
    Building* building;
    ScrollArea* scrollArea;
    
    CCSprite* cardBG;
    CCSprite* cardDetailBG;
    
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
    int type;
    //where 0 is normal,
    //where 1 overrides with Build Path
    //where 2 overrides with Destroy Path
    //Where 3 overrides with Demolish
    
    CCMenu* menu;
    CCArray* menuItemsArray;
    
    CCSprite* mask;
    
public:
    BuildingCard(Building*, ScrollArea*, int, int);
    ~BuildingCard();
    
    static BuildingCard* create(Building*, ScrollArea*, int, int);
    
    void init();
    
    void refreshAllMenuItems();
    
    void showBuildingInfo();
    
    //General menu callback
    void onMenuItemSelected(CCObject*);
    
    void tryToBuild(int);
    
    void setOpacity(GLubyte);
};

#endif
