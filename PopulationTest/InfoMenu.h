//
//  InfoMenu.h
//  PopulationTest
//
//  Created by Ong Heng Le on 27/5/13.
//
//

#ifndef __PopulationTest__InfoMenu__
#define __PopulationTest__InfoMenu__

#include "PopupMenu.h"
#include "TabbedView.h"
#include "ScrollArea.h"
#include "Building.h"

class InfoMenu:public PopupMenu
{
    
private:
    
    TabbedView* tabbedView;
    
    CCMenuItemImage* tabItemInfo;
    
    CCLayer* tabLayerInfo;
    
    ScrollArea* scrollArea;
    
    CCMenu* menu;
    
    std::vector<CCMenu*> menuArray;
    std::vector<CCMenuItemImage *> tabItemArray;
    std::vector<CCLayer*> tabLayerArray;
    std::vector<ScrollArea*> scrollAreaArray;
    std::vector<CCArray* > menuItemArray;
        
    float min_tabx;
    float max_tabx;
    
    CCPoint touchBegin;
    
    CCLabelTTF* TitleLabel;
    CCLabelTTF* NameLabel;
    CCLabelTTF* TypeLabel;// = CCLabelTTF::create("NAME", "Droidiga", 30);
    CCLabelTTF* OccLabel;// = CCLabelTTF::create("JOB", "Droidiga", 30);
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~InfoMenu();
    
    virtual void cleanup();
    
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    
    virtual void onOrientationChanged();
    
    void updateItemLevelLabel(Building* building);
    
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__PopulationTest__InfoMenu__) */
