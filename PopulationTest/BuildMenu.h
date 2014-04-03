//
//  BuildMenu.h
//  PopulationTest
//
//  Created by Serious Games on 14/3/13.
//
//

#ifndef PopulationTest_BuildMenu_h
#define PopulationTest_BuildMenu_h
#include "PopupMenu.h"
#include "TabbedView.h"
#include "ScrollArea.h"


class BuildMenu:public PopupMenu
{
    
private:
    
    TabbedView* tabbedView;
    
    //    CCMenuItemImage* tabItemBuildings;
    std::vector<CCMenu*> menuArray;
    std::vector<CCMenuItemImage *> tabItemArray;
    
    CCMenuItemImage* tabItemPath;
    
    //    CCLayer* tabLayerBuildings;
    std::vector<CCLayer*> tabLayerArray;
    
    CCLayer* tabLayerPath;
    
    //    ScrollArea* scrollArea;
    
    std::vector<ScrollArea*> scrollAreaArray;
    
    ScrollArea* scrollPathArea;
    
    
    std::vector<CCArray* > menuItemArray;
    
    CCArray* pathMenuItems;
    
    CCLabelTTF* TitleLabel;
    
    float min_tabx;
    float max_tabx;
    
    
    CCPoint touchBegin;
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~BuildMenu();
    //static BuildMenu* create();
    
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    
    virtual void onOrientationChanged();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    

};

#endif