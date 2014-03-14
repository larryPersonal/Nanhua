//
//  BuildingInfoMenu.h
//  PopulationTest
//
//  Created by Ong Heng Le on 19/4/13.
//
//

#ifndef PopulationTest_BuildingInfoMenu_h
#define PopulationTest_BuildingInfoMenu_h

#include "CCScrollView.h"
using namespace cocos2d::extension;

#include "PopupMenu.h"
#include "ProgressBar.h"
#include "ScrollArea.h"
#include "Building.h"

using namespace std;

class BuildingInfoMenu:public PopupMenu
{
private:
    // Building variables
    int mBuildingLevel;
    int mBuildingPrice;
    int mBuildingVacancy;
    int mBuildingOverload;
    int mBuildingCurrPopulation;
    
    int mBuildingUnitCurrent;
    int mBuildingUnitRequired;
    int mBuildingFoodStorageCurrent;
    int mBuildingFoodStorageMax;
    int mBuildingWorkUnitCurrent;
    int mBuildingWorkUnitRequired;
    
    // Background Rect
    Rect* background_rect;
    
    // Graphical items
    Building* building;
    
    CCSprite* spriteBackground;
    CCSprite* spriteBackgroundInner;
    
    CCSprite* spriteBuilding;
    
    vector<CCSprite*> spritePopulationSlot;
    vector<CCSprite*> spritePopulationOverloadSlot;
    vector<CCMenuItemImage*> spritePopulation;
    
    CCSprite* spPrice;
    CCSprite* spLoy;
    CCSprite* spHap;
    CCSprite* spCash;
    
    CCLabelTTF* labelLevel;

    CCLabelTTF* labelLoy;
    CCLabelTTF* labelHap;

    CCLabelTTF* textName;
    CCLabelTTF* textPrice;

    ProgressBar* unitBar;
    
    CCLabelTTF* textLoy;
    CCLabelTTF* textHap;
    
    CCLabelTTF* textCashMod;
    
    CCLabelTTF* labelStatus;
    CCLabelTTF* unitLabel;
    
    CCLabelTTF* recoveryRateTitleLabel;
    CCLabelTTF* recoveryRateLabel;
    
    CCLabelTTF* foodConsumptionRateTitleLabel;
    CCLabelTTF* foodConsumptionRateLabel;
    
    ProgressBar* foodStorageBar;
    CCLabelTTF* foodStorageLabel;
    CCLabelTTF* foodStorageTitleLabel;
    
    ProgressBar* workCompleteBar;
    CCLabelTTF* workCompleteLabel;
    CCLabelTTF* workCompleteTitleLabel;
    
    CCMenuItemImage* buttonClose;
    
    CCMenu* menu;
    
    /* for building level up */
    int mGameLevel;
    int mGameUpgradeUnit;
    int mGameHouseNumber;
    int mGameFarmNumber;
    int mGameGranaryNumber;
    int mGameTowerNumber;
    
    CCSprite* moneyIcon;
    CCSprite* foodIcon;
    
    CCLabelTTF* moneyLabel;
    CCLabelTTF* foodLabel;
    
    CCMenuItemImage* upgradeButton;
    CCMenuItemImage* cancelUpgradeButton;
    
    ProgressBar* upgradeBar;
    CCLabelTTF* upgradeBarLabel;
    
    CCArray* menuItemsUpgrade;
    CCMenu* upgradeMenu;
    
    CCLabelTTF* houseLimitTitle;
    CCLabelTTF* houseLimitLabel;
    
    CCLabelTTF* granaryLimitTitle;
    CCLabelTTF* granaryLimitLabel;
    
    CCLabelTTF* farmLimitTitle;
    CCLabelTTF* farmLimitLabel;
    
    CCLabelTTF* guardTowerLimitTitle;
    CCLabelTTF* guardTowerLimitLabel;
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    void upgrade();
    
    static BuildingInfoMenu* create(Building* building);
    
    virtual void cleanup();
    
    virtual ~BuildingInfoMenu();
    // Constructor with Building object
    BuildingInfoMenu(Building* building);
    
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //General menu callback
    virtual void onMenuItemSelected(CCObject* pSender);
    
    //Public function
    void reposition();
    void refreshAllMenuItemValues();
    
    //Update
    virtual void update(float deltaTime);
    
    Building* getBuilding();
};

#endif
