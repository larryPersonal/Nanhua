//
//  InGameMenu.h
//  PopulationTest
//
//  Created by Serious Games on 15/3/13.
//
//

#ifndef __PopulationTest__InGameMenu__
#define __PopulationTest__InGameMenu__

#include "PopupMenu.h"

class InGameMenu:public PopupMenu
{
    CCMenuItemSprite* menuItemsBuild;
    CCMenuItemSprite* menuItemsResearch;
    CCMenuItemSprite* menuItemsPopulation;
    CCMenuItemSprite* menuItemsInfo;
    CCMenuItemSprite* menuItemsPolicy;
    CCMenuItemSprite* menuItemsSystem;
    CCMenuItemSprite* menuItemsExit;
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    /*look, inheritence means I need to override the destructor ok. You think I am doing this for nothing?*/
    virtual ~InGameMenu();
    virtual void cleanup();
    
    static InGameMenu* create();
    
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    
    virtual void onOrientationChanged();
//    virtual void onAnimComplete();
  
    //virtual void scrollY(float scrollBy);
    
    void exitGame(CCObject* pSender);
};
#endif /* defined(__PopulationTest__InGameMenu__) */
