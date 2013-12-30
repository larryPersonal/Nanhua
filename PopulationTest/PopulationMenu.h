//
//  PopulationMenu.h
//  PopulationTest
//
//  Created by Ong Heng Le on 27/5/13.
//
//

#ifndef __PopulationTest__PopulationMenu__
#define __PopulationTest__PopulationMenu__

#include "PopupMenu.h"
#include "TabbedView.h"
#include "ScrollArea.h"
#include "Sprite.h"

class PopulationMenu:public PopupMenu
{
    
private:
    
    TabbedView* tabbedView;
    
    CCMenuItemImage* tabItemPopulation;
    
    CCLayer* tabLayerPopulation;
    
    ScrollArea* scrollArea;
    
    CCMenu* menu;
    
    CCLabelTTF* TitleLabel;
    CCLabelTTF* NameLabel;// = CCLabelTTF::create("NAME", "Droidiga", 30);
    CCLabelTTF* ClassLabel;// = CCLabelTTF::create("CLASS", "Droidiga",30 );
    CCLabelTTF* JobLabel;// = CCLabelTTF::create("JOB", "Droidiga", 30);
    CCLabelTTF* HomeLabel;// =CCLabelTTF::create("HOME", "Droidiga", 30);
    CCLabelTTF* CashLabel;// =CCLabelTTF::create("CASH", "Droidiga", 30);
    
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~PopulationMenu();
    
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    void onBuildingItemSelected(CCObject* pSender);
    
    virtual void onOrientationChanged();
    
    void updateItemJobLabel(GameSprite* sprite);
    void updateItemHomeLabel(GameSprite* sprite);
    void updateItemClassLabel(GameSprite* sprite);
    
    void addPopulationToList(GameSprite* sprite);
    void removePopulationFromList(GameSprite* sprite);
};

#endif /* defined(__PopulationTest__PopulationMenu__) */
