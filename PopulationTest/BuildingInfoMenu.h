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
    static BuildingInfoMenu* SP;
    
public:
    // Building variables
    int mBuildingLevel;
    int mBuildingPrice;
    
    int mBuildingVacancy;
    int mBuildingCurrPopulation;
    bool mBuildingUnderConstruction;
    
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
    
    /*
     * section for population and population slot
     */
    CCLabelTTF* spritePopulationLabel;
    CCArray* spritePopulationSlot;
    CCArray* spritePopulation;
    CCArray* spritePopulationBackground;
    CCArray* spritePopulationMenu;
    
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
    
    CCSprite* buttonClose;
    
    CCMenu* menu;
    
    /* for building level up */
    int mGameLevel;
    int mGameUpgradeUnit;
    int mGameHouseNumber;
    int mGameFarmNumber;
    int mGameGranaryNumber;
    int mGameTowerNumber;
    bool mGameCurrentlyUpgrading;
    
    CCSprite* moneyIcon;
    CCSprite* foodIcon;
    
    CCLabelTTF* moneyLabel;
    CCLabelTTF* foodLabel;
    
    CCSprite* upgradeButton;
    CCSprite* cancelUpgradeButton;
    
    ProgressBar* upgradeBar;
    CCLabelTTF* upgradeBarLabel;
    
    /*
    CCArray* menuItemsUpgrade;
    CCMenu* upgradeMenu;
    */
    
    CCLabelTTF* houseLimitTitle;
    CCLabelTTF* houseLimitLabel;
    
    CCLabelTTF* granaryLimitTitle;
    CCLabelTTF* granaryLimitLabel;
    
    CCLabelTTF* farmLimitTitle;
    CCLabelTTF* farmLimitLabel;
    
    CCLabelTTF* guardTowerLimitTitle;
    CCLabelTTF* guardTowerLimitLabel;
    
    CCSprite* selectWorkerButton;
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    static BuildingInfoMenu* getThis();
    
    void upgrade();
    
    static BuildingInfoMenu* create(Building* building);
    
    virtual void cleanup();
    
    void showSprite(CCObject *pSender);
    
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
    
    void selectPop();
};

#endif
