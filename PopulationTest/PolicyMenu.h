//
//  PolicyMenu.h
//  PopulationTest
//
//  Created by Serious Games on 27/5/13.
//
//

#ifndef __PopulationTest__PolicyMenu__
#define __PopulationTest__PolicyMenu__

#include "PopupMenu.h"
#include "TabbedView.h"
#include "ScrollArea.h"

class PolicyMenu:public PopupMenu
{
    
private:
    int toSpawn = 10;
    
    
    TabbedView* tabbedView;
    
    CCMenuItemImage* tabItemPolicies;
    
    CCLayer* tabLayerPolicies;
    
    ScrollArea* scrollArea;
    
    int numberToInstantlyImport;
    
    
    CCLabelTTF* taxRateNum;
    CCLabelTTF* taxRateChangeNum;
    CCLabelTTF* taxRateCost;
 
    CCLabelTTF* aPopNum;
    CCLabelTTF* aPopChangeNum;
    CCLabelTTF* aPopCost;
    
    
    CCLabelTTF* a2PopNum;
    
    //CCLabelTTF* iiNum;
    
    int deltaTax, deltaPop;
    int currTax, currPop;
    
    int deltaBirthRate, deltaBirthMonthDur;
    int currBR, currBMD;
    
    int deltaCulturalEx, deltaCulturalMonthDur;
    int currCE, currCMD;
    
    //I'll need to update these numbers
    CCLabelTTF* localBirthNum;
    CCLabelTTF* localBirthTimeLeft;
    
    CCLabelTTF* localBirthRateChangeNum;
    CCLabelTTF* localBirthPolDurationNum;
    
    CCLabelTTF* birthPolCost;
    
    CCLabelTTF* culturalExNum;
    CCLabelTTF* culturalExTimeLeft;
    
    CCLabelTTF* culturalExChangeNum;
    CCLabelTTF* culturalExPolDurationNum;
    
    CCLabelTTF* culturalPolCost;
    
    CCLabelTTF* TitleLabel;
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~PolicyMenu();
 
    //General menu callback.
    virtual void onMenuItemSelected(CCObject* pSender);
    
    virtual void onOrientationChanged() ;
    
    
};

#endif
