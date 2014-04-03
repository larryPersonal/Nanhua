//
//  PopupMenu.h
//  PopulationTest
//
//  Created by Serious Games on 14/3/13.
//
//

#ifndef __PopulationTest__PopupMenu__
#define __PopulationTest__PopupMenu__

#include "cocos2d.h"
#include "Building.h"
#include "Sprite.h"
using namespace cocos2d;

enum PopupMenuType {
    SpInfoMenu = 0, BuInfoMenu, SePopulationMenu = 2
};

struct PreviousMenu
{
    PopupMenuType pmt;
    Building* b;
    GameSprite* gs;
    
    PreviousMenu(PopupMenuType v_pmt, Building* v_b)
    {
        pmt = v_pmt;
        b = v_b;
        gs = NULL;
    }
    
    PreviousMenu(PopupMenuType v_pmt, GameSprite* v_gs)
    {
        pmt = v_pmt;
        b = NULL;
        gs = v_gs;
    }
};

//Do not try to instantiate this. Instantiate subclasses
class PopupMenu:public cocos2d::CCLayer
{ 
protected:
    
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
    
    //Override anything that is = 0 in subclass please
    //This is protected, only call useAsPopupMenu() or useOverPopupMenu().
    virtual void createMenuItems() = 0;
    
public:
    
    //Needs to be virtual to clean up after itself from the subclass.
    PopupMenu();
    
    ~PopupMenu();
    virtual void cleanup();
    //pure virtual cannot be instantiated. Leaving alone.
   // virtual PopupMenu* create();
    
    //The subclasses will also contain these attributes so you don't have to declare them again.
    CCArray* menuItems;
    CCPointArray* menuItemPositions;
    bool canPressButtons;
    
    //Usage helper functions
    static void backupCurrentPopupMenu();          // Opens if there is no other active popupMenu
    void useAsExclusivePopupMenu();     // Closes all active popupMenu and open this
    void useAsTopmostPopupMenu();       // Opens on top of other popupMenu
    static int openPopupCount();
    
    static void closeAllPopupMenu();
    
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //There is only supposed to be ONE callback. See how I deal with it in BuildMenu for an idea of how its dealt with in other menus.
    //if you need other callbacks, feel free to add in the subclass.
    virtual void onMenuItemSelected(CCObject* pSender) = 0;
    
    //Close menu instantly, if there is no animation
    virtual void closeMenu(bool);
    
    //Override this if you do not want your menu to scroll!
    //virtual void scrollY(float scrollBy);
    
    //PopupMenu to swallow touches
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

};


#endif /* defined(__PopulationTest__PopupMenu__) */
