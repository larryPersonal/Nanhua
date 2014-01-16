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

using namespace std;

class SelectPopulation: public PopupMenu
{

private:
    // Background Rect
    Rect* background_rect;
    
    // Graphical Items
    Building* building;
    
    CCMenuItemImage* buttonClose;
    
    CCSprite* spriteBackground;
    
    CCSprite* spriteBuilding;
    
    CCSprite* customSprite;
    CCArray* spriteRowArray;
    
    // stats graphics
    //CCSprite* spInt;
    CCSprite* spLoy;
    //CCSprite* spSoc;
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
    CCLabelTTF* labelSRName;
    
    ScrollArea* scrollArea;
    
    ProgressBar* progressBar;
    CCLabelTTF* progressBarLabel;
    
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
    //void refreshAllMenuItemValues();
    
    //Update
    virtual void update(float deltaTime);
};


#endif
