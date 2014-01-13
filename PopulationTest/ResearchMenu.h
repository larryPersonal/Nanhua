//
//  ResearchMenu.h
//  PopulationTest
//
//  Created by Ong Heng Le on 30/4/13.
//
//

#ifndef PopulationTest_ResearchMenu_h
#define PopulationTest_ResearchMenu_h

#include "PopupMenu.h"
#include "TabbedView.h"
#include "ScrollArea.h"


class ResearchMenu:public PopupMenu
{
    
private:
    
    bool isUpdateScheduled;
    
    TabbedView* tabbedView;
    
    ScrollArea* scrollArea;
    
    CCArray* timeLabelsToUpdate;
    std::vector<CCMenu*> menuArray;
    std::vector<CCMenuItemImage *> tabItemArray;
    std::vector<CCLayer*> tabLayerArray;
    std::vector<ScrollArea*> scrollAreaArray;
    std::vector<CCArray* > menuItemArray;
    
    
    float min_tabx;
    float max_tabx;
    CCPoint touchBegin;
    
    CCLabelTTF* TitleLabel;
   
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~ResearchMenu();
    
    virtual void cleanup();
    
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    virtual void onOrientationChanged();
    
    virtual void update(float deltaTime);
    void repositionMenuItems();
    void researchBuilding(CCMenuItem* sender, float time);
    void addResearchingLabel(CCLabelTTF* label, float time);
    
    //Confirmation Alert Callback
    void onResearchButtonPressed(CCObject* pSender);
    
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif
