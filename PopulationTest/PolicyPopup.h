//
//  PolicyPopup.h
//  PopulationTest
//
//  Created by Serious Games on 29/7/13.
//
//

#ifndef __PopulationTest__PolicyPopup__
#define __PopulationTest__PolicyPopup__

#include <iostream>


#include "cocos2d.h"
using namespace cocos2d;

#include <string>
using namespace std;

#include "PopupMenu.h"

class PolicyPopup:public PopupMenu
{
    
private:
    
    CCSprite* background;
    CCSprite* backgroundOverlay;
    CCLabelTTF* displayText;
    CCLabelTTF* costText;
    CCLabelTTF* currValText;
    
    CCLabelTTF* deltaValText;
    CCLabelTTF* deltaDurText;
    
    CCMenu* buttonMenu;
    
    CCMenu* adjustMenu;
    
    int costVal;
    
    int amtVal;
    int deltaAmtVal;
    int deltaDurationVal;
    
    int menuMode;
    
    void updateCost();
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    
    // Constructor with text to display
    PolicyPopup();
    static PolicyPopup* create();
    // virtual bool init();
    
    virtual ~PolicyPopup();
    virtual void cleanup();
       
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //Default menu callback
    virtual void onMenuItemSelected(CCObject* pSender);
    
    //Public function
    void reposition();
    
    //AlertBox function
    void setDisplayText(string text);
    void addButton(int tag, string buttonText, CCObject* pSender=NULL);
    void removeButton(int tag);
    
    
    void setupBirthPolicy();
    void setupInteractPolicy();
    
};



#endif /* defined(__PopulationTest__PolicyPopup__) */
