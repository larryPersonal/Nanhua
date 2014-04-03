//
//  AlertBox.h
//  PopulationTest
//
//  Created by Ong Heng Le on 20/5/13.
//
//

#ifndef __PopulationTest__AlertBox__
#define __PopulationTest__AlertBox__

#include "cocos2d.h"
using namespace cocos2d;

#include <string>
using namespace std;

#include "PopupMenu.h"

class AlertBox:public PopupMenu
{
    
private:
    
    CCSprite* background;
    CCSprite* backgroundOverlay;
    CCLabelTTF* displayText;
    CCMenu* buttonMenu;
    
    bool close_called; //ensures that the tutorialhandler is only informed of each Alertbox ONCE
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    CCCallFuncO* callback;// = CCCallFuncO::create(pSelectorTarget, selector, pSender);
    
public:
    
    // Constructor with text to display
    AlertBox();
    static AlertBox* create();
   // virtual bool init();
    
    virtual ~AlertBox();
    virtual void cleanup();
    
    
    
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //Default menu callback
    virtual void onMenuItemSelected(CCObject* pSender);
    
    //Public function
    void reposition();
    
    //AlertBox function
    void setDisplayText(string text);
    void addButton(int tag, string buttonText, CCObject* pSelectorTarget=NULL, SEL_CallFuncO selector=NULL, CCObject* pSender=NULL);
    void removeButton(int tag);
};

#endif /* defined(__PopulationTest__AlertBox__) */
