//
//  BuildScroll.h
//  PopulationTest
//
//  Created by admin on 13/2/14.
//
//

#ifndef PopulationTest_BuildScroll_h
#define PopulationTest_BuildScroll_h

#include "CCScrollView.h"

using namespace cocos2d::extension;

#include "PopupMenu.h"
#include "ScrollArea.h"
#include "Building.h"

using namespace std;

class BuildScroll: public PopupMenu
{
private:
    // Background Rect
    Rect* background_rect;
    
    CCMenu* menu;
    
    CCSprite* buildMenu;
    
    CCArray* buildingCards;
    
    ScrollArea* scrollArea;
    
protected:
    virtual void createMenuItems();

public:
    static BuildScroll* SP;
    static BuildScroll* create();
    static BuildScroll* getThis();
    virtual void cleanup();
    
    virtual ~BuildScroll();
    // constructor with building object
    BuildScroll();
    
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
