//
//  SystemMenu.h
//  PopulationTest
//
//  Created by Serious Games on 27/5/13.
//
//

#ifndef __PopulationTest__SystemMenu__
#define __PopulationTest__SystemMenu__

#include <iostream>
#include "PopupMenu.h"
#include "TabbedView.h"
#include "ScrollArea.h"

class SystemMenu:public PopupMenu
{
    
private:
    
    TabbedView* tabbedView;
    CCMenuItemImage* tabItemSystem;
    CCLayer* tabLayerSystem;
    ScrollArea* scrollArea;
    
    CCLabelTTF* TitleLabel;
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~SystemMenu();
    
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    virtual void onOrientationChanged();
    
};

#endif /* defined(__PopulationTest__SystemMenu__) */