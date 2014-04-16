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
public:
    // dirty attributes
    int population;
    bool isUnderConstruction;
    bool isCurrentlyUnderConstruction;
    bool isPerformingTask;
    int num_of_citizens;
    
    // Background Rect
    Rect* background_rect;
    
    // Graphical Items
    Building* building;
    
    CCSprite* buttonClose;
    CCMenuItemImage* buttonOk;
    CCMenuItemImage* buttonCancel;
    
    CCSprite* spriteBackground;
    
    CCMenuItemSprite* spriteBuilding;
    
    // the array to store all the villagers that are not belongs to this building
    CCArray* spriteRowArray;
    CCArray* emptySpaceArray;
    
    CCArray* memberArray;
    CCArray* memberRowArray;
    CCArray* memberRowBackgroundArray;
    CCArray* memberMenuArray;
    
    // labels
    CCLabelTTF* labelBuildingName;
    
    CCLabelTTF* workerLabel;
    CCLabelTTF* taskLabel;
    
    CCMenuItemImage* sortButton;
    CCMenuItemImage* sortHappinessButton;
    
    ScrollArea* scrollArea;
    
    CCMenu* menu;
    
    bool isSorted;
    bool isSortedByHappiness;
    bool happinessIncre;
    bool energyIncre;
    
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
    
    void selectSprite(GameSprite*, SpriteRow*);
    void unselectSprite(GameSprite*, SpriteRow*);
    
    void cancelSprite(CCObject*);
    
    void scheduleConstruction();
    
    void refreshUI();
    
    //Update
    virtual void update(float deltaTime);
    
    void prepareJob(GameSprite*);
    
    void cancelTask();
    void performTask();
    void scheduleGuardTower();
    void scheduleFarming();
    
    void clickSortButton();
    void clickSortHappinessButton();
    
    void adjustZIndex(bool);
    
    CCArray* getSpriteList();
    CCArray* sortByHappiness();
    CCArray* sortByEnergy();
    GameSprite* getSpriteWithHappiness(CCArray*, bool);
    GameSprite* getSpriteWithEnergy(CCArray*, bool);
};


#endif
