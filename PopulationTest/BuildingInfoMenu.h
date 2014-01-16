//
//  BuildingInfoMenu.h
//  PopulationTest
//
//  Created by Ong Heng Le on 19/4/13.
//
//

#ifndef PopulationTest_BuildingInfoMenu_h
#define PopulationTest_BuildingInfoMenu_h

#include "CCScrollView.h"
using namespace cocos2d::extension;

#include "PopupMenu.h"
#include "ProgressBar.h"
#include "ScrollArea.h"
#include "Building.h"

using namespace std;

class BuildingInfoMenu:public PopupMenu
{
    
private:
    
    // Building variables
    int mBuildingExp;
    int mBuildingExpMax;
    int mBuildingLevel;
    int mBuildingPrice;
    int mBuildingVacancy;
    int mBuildingOverload;
    int mBuildingCurrPopulation;
    
    // Background Rect
    Rect* background_rect;
    
    // Graphical items
    Building* building;
    
    CCSprite* spriteBackground;
    CCSprite* spriteBackgroundInner;
    
    CCSprite* spriteBuilding;
    
    vector<CCSprite*> spritePopulationSlot;
    vector<CCSprite*> spritePopulationOverloadSlot;
    vector<CCMenuItemImage*> spritePopulation;
    
    CCSprite* spPrice;
    CCSprite* spLoy;
    CCSprite* spHap;
    CCSprite* spCash;
    
    CCLabelTTF* labelLevel;
    CCLabelTTF* labelExp;
/*
    CCLabelTTF* labelPrice;
 
    CCLabelTTF* labelLoy;
    CCLabelTTF* labelHap;
  */  
    CCLabelTTF* textName;
    CCLabelTTF* textPrice;
    CCLabelTTF* textExp;

    ProgressBar* barExp;
    ProgressBar* unitBar;
    
    CCLabelTTF* textLoy;
    CCLabelTTF* textHap;
    
    CCLabelTTF* textCashMod;
    
    CCLabelTTF* labelStatus;
    CCLabelTTF* unitLabel;
    
    CCMenuItemImage* buttonClose;
    
    CCMenu* menu;
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    static BuildingInfoMenu* create(Building* building);
    virtual void cleanup();
    
    
    virtual ~BuildingInfoMenu() {}
    // Constructor with Building object
    BuildingInfoMenu(Building* building);
    
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //General menu callback
    virtual void onMenuItemSelected(CCObject* pSender);
    
    //Public function
    void reposition();
    void refreshAllMenuItemValues();
    
    //Update
    virtual void update(float deltaTime);
};

#endif
