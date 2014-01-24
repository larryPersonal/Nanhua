//
//  SelectPopulation.h
//  PopulationTest
//
//  Created by admin on 10/1/14.
//
//

#ifndef PopulationTest_SelectPopulation_h
#define PopulationTest_SelectPopulation_h

#include "CCScrollView.h"

using namespace cocos2d::extension;

#include "PopupMenu.h"
#include "ProgressBar.h"
#include "ScrollArea.h"
#include "Building.h"
#include "SpriteRow.h"

using namespace std;

class SelectPopulation: public PopupMenu
{

private:
    // dirty attributes
    int mBuildingUnitCurrent;
    int mBuildingUnitRequired;
    int mBuildingMemberCurrent;
    int mBuildingPopulationLimit;
    int mBuildingBuilderCurrent;
    int mBuildingBuilderLimit;
    
    // Background Rect
    Rect* background_rect;
    
    // Graphical Items
    Building* building;
    
    CCMenuItemImage* buttonClose;
    
    CCSprite* spriteBackground;
    
    CCSprite* spriteBuilding;
    
    // the array to store all the villagers that are belongs to this building
    CCArray* memberSpriteRowArray;
    
    // the array to store all the villagers that are not belongs to this building
    CCArray* spriteRowArray;
    
    // stats graphics
    CCSprite* spLoy;
    CCSprite* spHap;
    
    // labels
    CCLabelTTF* labelBuildingName;
    CCLabelTTF* labelBuildingLevel;
    
    //CCLabelTTF* labelTextInt;
    CCLabelTTF* labelTextLoy;
    //CCLabelTTF* labelTextSoc;
    CCLabelTTF* labelTextHap;
    
    // sprite row header
    CCLabelTTF* labelSRImage;
    CCLabelTTF* labelSRMember;
    CCLabelTTF* labelSRName;
    
    ScrollArea* memberScrollArea;
    ScrollArea* scrollArea;
    
    ProgressBar* progressBar;
    CCLabelTTF* progressBarLabel;
    
    CCLabelTTF* memberLabel;
    CCLabelTTF* builderLabel;
    
    CCMenu* menu;
    
protected:
    virtual void createMenuItems();
    
public:
    static SelectPopulation* SP;
    static SelectPopulation* create(Building* building);
    static SelectPopulation* getThis();
    virtual void cleanup();
    
    virtual ~SelectPopulation();
    // constructor with building object
    SelectPopulation(Building* building);
    
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //General menu callback
    virtual void onMenuItemSelected(CCObject* pSender);
    
    //Public function
    void reposition();
    void refreshAllMenuItemValues();
    
    void addMemberRow(GameSprite*, SpriteRow*);
    void addVillagerRow(GameSprite*, SpriteRow*);
    
    //Update
    virtual void update(float deltaTime);
};


#endif
