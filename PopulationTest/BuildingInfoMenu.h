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

struct Rect
{
    float width;
    float height;
    float left;
    float top;
    
    void ini(float wid, float hei, float lef, float to)
    {
        width = wid;
        height = hei;
        left = lef;
        top = to;
    }
};

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
    //CCSprite* spriteGy;
    
    vector<CCSprite*> spritePopulationSlot;
    vector<CCSprite*> spritePopulationOverloadSlot;
    vector<CCMenuItemImage*> spritePopulation;
    
    CCSprite* spPrice;
    CCSprite* spInt;
    CCSprite* spLoy;
    CCSprite* spSoc;
    CCSprite* spHap;
    CCSprite* spCash;
    
    CCLabelTTF* labelLevel;
    CCLabelTTF* labelExp;
/*
    CCLabelTTF* labelPrice;
 
    
    CCLabelTTF* labelInt;
    CCLabelTTF* labelLoy;
    CCLabelTTF* labelSoc;
    CCLabelTTF* labelHap;
  */  
    CCLabelTTF* textName;
    CCLabelTTF* textPrice;
    CCLabelTTF* textExp;

    ProgressBar* barExp;
    
    
    CCLabelTTF* textInt;
    CCLabelTTF* textLoy;
    CCLabelTTF* textSoc;
    CCLabelTTF* textHap;
    
    CCLabelTTF* textCashMod;
    
    CCMenuItemImage* buttonClose;
    
    ScrollArea* scrollArea;
    
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
