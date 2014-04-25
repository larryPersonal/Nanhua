//
//  BuildingInfoMenu.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 19/4/13.
//
//

#include "BuildingInfoMenu.h"
#include "GameHUD.h"
#include "GlobalHelper.h"
#include "SpriteInfoMenu.h"
#include "SelectPopulation.h"
#include "TutorialManager.h"

float portraitScale; //hack
BuildingInfoMenu* BuildingInfoMenu::SP;

BuildingInfoMenu* BuildingInfoMenu::create(Building* building)
{
    BuildingInfoMenu *pRet = new BuildingInfoMenu(building);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void BuildingInfoMenu::cleanup()
{

    PopupMenu::cleanup();
    
}


BuildingInfoMenu::BuildingInfoMenu(Building* building)
{
    if (!building) CCLog("Warning NO BUILDING!!!");
    
    BuildingInfoMenu::SP = this;
    
    this->building = building;
    
    // create the gui handler
    background_rect = new Rect();
    
    // initialize the gui handler
    background_rect->ini(700, 100, 100, 200);
    
    mBuildingVacancy = 0;
    mBuildingCurrPopulation = 0;
    
    mBuildingPrice = 0;
    
    spritePopulationSlot = CCArray::create();
    spritePopulationSlot->retain();
    
    spritePopulation = CCArray::create();
    spritePopulation->retain();
    
    spritePopulationBackground = CCArray::create();
    spritePopulationBackground->retain();
    
    spritePopulationMenu = CCArray::create();
    spritePopulationMenu->retain();
    
    mGameCurrentlyUpgrading = false;
}

BuildingInfoMenu::~BuildingInfoMenu()
{
    BuildingInfoMenu::SP = NULL;
    
    this->unschedule(schedule_selector(BuildingInfoMenu::update));
    
    spritePopulationSlot->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulationSlot);
    
    spritePopulation->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulation);
    
    spritePopulationBackground->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulationBackground);
    
    spritePopulationMenu->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulationMenu);
}

BuildingInfoMenu* BuildingInfoMenu::getThis()
{
    return SP;
}

void BuildingInfoMenu::createMenuItems()
{
    if (building->buildingName.length() == 0)
    {
        this->closeMenu(true);
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorYellow = ccc3(225, 219, 108);
    
    // Create constant menu items
    spriteBackground = CCSprite::create("BuildingInfoUI.png");
    spriteBackground->setScale(background_rect->width / spriteBackground->boundingBox().size.width);
    spriteBackground->setAnchorPoint(ccp(0, 1));
    spriteBackground->setPosition(ccp(screenSize.width / 2.0f - spriteBackground->boundingBox().size.width / 2.0f, screenSize.height / 2.0f + spriteBackground->boundingBox().size.height / 2.0f));
    this->addChild(spriteBackground);
    
    spriteBackgroundInner = CCSprite::create("blackscreen.png");
    spriteBackgroundInner->setScale(spriteBackground->getScale());
    
    // Set variables which may become dirty
    mBuildingLevel = GameManager::getThis()->town_hall_level;
    if(building->isUnderConstruction())
    {
        mBuildingVacancy = building->builderLimit;
    }
    else
    {
        if(building->buildingType == HOUSING)
        {
            mBuildingVacancy = building->populationLimit;
        }
        else
        {
            mBuildingVacancy = building->number_of_jobs;
        }
    }
    mBuildingCurrPopulation = building->memberSpriteList->count();
    
    mBuildingUnderConstruction = building->isUnderConstruction();
    
    mBuildingUnitCurrent = building->build_uint_current;
    mBuildingUnitRequired = building->build_uint_required;
    mBuildingFoodStorageCurrent = building->currentStorage;
    mBuildingFoodStorageMax = building->storageLimit;
    mBuildingWorkUnitCurrent = building->work_unit_current;
    mBuildingWorkUnitRequired = building->work_unit_required;
    
    // Create header
    textName = CCLabelTTF::create(GlobalHelper::stringToUpper(building->buildingName).c_str(), "Shojumaru-Regular", 32, CCSizeMake(building->buildingName.length() * 25.0f, 5.0f), kCCTextAlignmentCenter);
    textName->setColor(colorYellow);
    
    spLoy = CCSprite::create("loyalty icon.png");
    spLoy->setScale(0.75);
    spHap = CCSprite::create("happiness icon.png");
    spHap->setScale(0.75);
    spPrice = CCSprite::create("yuanbao.png");
    spPrice->setScale(0.75);
    spCash = CCSprite::create("yuanbao.png");
    //spCash = CCSprite::create("happiness icon.png");
    spCash->setScale(0.75);
    
    std::stringstream ss;
    ss << "Yearly: " << mBuildingPrice << "Y";
    textPrice = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    textPrice->setColor(colorYellow);
    
    // Sprite
    spriteBuilding = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    spriteBuilding->setAnchorPoint(ccp(0.5, 0.5));
    
    spriteBuilding->setScale(256.0f / spriteBuilding->boundingBox().size.width);
    
    if(building->isUnderConstruction())
    {
        spriteBuilding->setOpacity(120);
    }
    
    // Attribute labels
    ss.str(std::string());
    ss << "Level: " << (mBuildingLevel + 1);
    labelLevel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    labelLevel->setColor(colorYellow);
    
    ss.str(std::string());
    if(building->build_uint_current < building->build_uint_required)
    {
        ss << "Under construction!";
    }
    else
    {
        ss << "Ready for service";
    }
    labelStatus = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelStatus->setColor(colorYellow);
    
    ss.str(std::string());
    ss << building->build_uint_current << "/" << building->build_uint_required;
    unitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    unitLabel->setColor(colorYellow);
    
    unitBar = new ProgressBar();
    unitBar->createProgressBar(
                               CCRectMake(0, 0, 70, 10),
                               CCRectMake(0, 0, 70, 10),
                               "Energy_brown bar.png",
                               "NONE",
                               "NONE",
                               "Energybar.png",
                               true
    );
    unitBar->setValue((float)building->build_uint_current / (float)building->build_uint_required);
    
    // recovery rate
    ss.str(std::string());
    ss << "RR:";
    recoveryRateTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    recoveryRateTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << building->recovery_rate;
    recoveryRateLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    recoveryRateLabel->setColor(colorYellow);
    
    // food consumption rate
    ss.str(std::string());
    ss << "FC:";
    foodConsumptionRateTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodConsumptionRateTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    
    ss << building->food_consumption_rate;
    foodConsumptionRateLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodConsumptionRateLabel->setColor(colorYellow);
    
    // food storage limit
    ss.str(std::string());
    ss << "Food Storage:";
    foodStorageTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodStorageTitleLabel->setColor(colorYellow);
    
    // food storage
    foodStorageBar = new ProgressBar();
    foodStorageBar->createProgressBar(
                               CCRectMake(0, 0, 70, 10),
                               CCRectMake(0, 0, 70, 10),
                               "Energy_brown bar.png",
                               "NONE",
                               "NONE",
                               "Energybar.png",
                               true
                               );
    foodStorageBar->setValue((float)building->currentStorage / (float)building->storageLimit);
    
    ss.str(std::string());
    ss << building->currentStorage << "/" << building->storageLimit;
    foodStorageLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodStorageLabel->setColor(colorYellow);
    
    // work unit done
    ss.str(std::string());
    ss << "Work Unit Done:";
    workCompleteTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workCompleteTitleLabel->setColor(colorYellow);
    
    workCompleteBar = new ProgressBar();
    workCompleteBar->createProgressBar(
                                      CCRectMake(0, 0, 70, 10),
                                      CCRectMake(0, 0, 70, 10),
                                      "Energy_brown bar.png",
                                      "NONE",
                                      "NONE",
                                      "Energybar.png",
                                      true
                                      );
    workCompleteBar->setValue((float)building->work_unit_current / (float)building->work_unit_required);
    
    ss.str(std::string());
    ss << (int) building->work_unit_current << "/" << (int) building->work_unit_required;
    workCompleteLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workCompleteLabel->setColor(colorYellow);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCSprite::create("Closebtn_Sq.png");
    buttonClose->setTag(-1);
    this->addChild(buttonClose);
    
    /*
    menuItems->addObject(buttonClose);
    */
    selectWorkerButton = CCMenuItemImage::create("allocatebtn.png", "allocatebtn-press.png", this, menu_selector(BuildingInfoMenu::selectPop));
    selectWorkerButton->setScale( 150.0f / selectWorkerButton->boundingBox().size.width );
    selectWorkerButton->setAnchorPoint(ccp(0, 1));
    
    if(!building->isUnderConstruction() && building->buildingType != AMENITY && building->buildingType != MILITARY)
    {
        selectWorkerButton->setVisible(false);
    }
    
    menuItems->addObject(selectWorkerButton);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Add children
    this->addChild(spPrice);
    this->addChild(textName);
    this->addChild(textPrice);
    this->addChild(menu, 3);
    
    this->addChild(spriteBuilding);
    this->addChild(labelLevel);
    
    this->addChild(labelStatus);
    this->addChild(unitBar);
    this->addChild(unitLabel);
    
    this->addChild(recoveryRateTitleLabel);
    this->addChild(recoveryRateLabel);
    this->addChild(foodConsumptionRateLabel);
    this->addChild(foodConsumptionRateTitleLabel);
    this->addChild(foodStorageTitleLabel);
    this->addChild(foodStorageBar);
    this->addChild(foodStorageLabel);
    
    this->addChild(workCompleteTitleLabel);
    this->addChild(workCompleteBar);
    this->addChild(workCompleteLabel);
    
    // Done creation, now position them
    
    textName->setAnchorPoint(ccp(0.5, 1));
    spPrice->setAnchorPoint(ccp(1, 0));
    textPrice->setAnchorPoint(ccp(1, 0));
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    spriteBuilding->setAnchorPoint(ccp(0, 1));
    labelLevel->setAnchorPoint(ccp(0.5, 1));
    
    labelStatus->setAnchorPoint(ccp(0, 1));
    unitBar->setAnchorPoint(ccp(0, 1));
    unitLabel->setAnchorPoint(ccp(0, 1));
    
    recoveryRateTitleLabel->setAnchorPoint(ccp(0, 1));
    recoveryRateLabel->setAnchorPoint(ccp(0, 1));
    foodConsumptionRateLabel->setAnchorPoint(ccp(0, 1));
    foodConsumptionRateTitleLabel->setAnchorPoint(ccp(0, 1));
    foodStorageTitleLabel->setAnchorPoint(ccp(0, 1));
    foodStorageBar->setAnchorPoint(ccp(0, 1));
    foodStorageLabel->setAnchorPoint(ccp(0, 1));
    
    workCompleteTitleLabel->setAnchorPoint(ccp(0, 1));
    workCompleteBar->setAnchorPoint(ccp(0, 1));
    workCompleteLabel->setAnchorPoint(ccp(0, 1));
    
    // sprite population label
    ss.str(string());
    ss << "Villages Assigned!";
    spritePopulationLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    spritePopulationLabel->setColor(colorYellow);
    spritePopulationLabel->setAnchorPoint(ccp(0, 1));
    this->addChild(spritePopulationLabel);
    
    // Create population icons
    for (int i = 0; i < mBuildingVacancy; i++)
    {
        CCSprite* vacancySlot = CCSprite::create("assign_menu_unfilled.png");
        vacancySlot->setScale(70.0f / vacancySlot->boundingBox().size.width);
        vacancySlot->setAnchorPoint(ccp(0, 1));
        this->addChild(vacancySlot);
        spritePopulationSlot->addObject(vacancySlot);
    }
    
    for (int i = 0; i < building->memberSpriteList->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) building->memberSpriteList->objectAtIndex(i);
        
        CCSprite* currentMemberBackground = CCSprite::create("assign_menu_filled.png");
        currentMemberBackground->setScale(70.0f / currentMemberBackground->boundingBox().size.width);
        currentMemberBackground->setAnchorPoint(ccp(0, 1));
        this->addChild(currentMemberBackground);
        spritePopulationBackground->addObject(currentMemberBackground);
        
        std::string tempStr = gameSprite->spriteName;
        CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(BuildingInfoMenu::showSprite));
        memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
        memberSprite->setAnchorPoint(ccp(0, 1));
        memberSprite->setTag(i);
        spritePopulation->addObject(memberSprite);
        
        CCArray* newMenuItems = CCArray::create();
        newMenuItems->addObject(memberSprite);
        CCMenu* newMenu = CCMenu::createWithArray(newMenuItems);
        newMenu->setPosition(CCPointZero);
        this->addChild(newMenu, 4);
        spritePopulationMenu->addObject(newMenu);
    }
    
    /*********************** for building level up *********************/
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
    moneyIcon = CCSprite::create("yuanbao_icon.png");
    moneyIcon->setScale(0.4f);
    moneyIcon->setAnchorPoint(ccp(0, 1));
    moneyIcon->setPosition(ccp(-200 + hw, -190 + hh));
    this->addChild(moneyIcon);
    
    ss.str(string());
    int level = GameManager::getThis()->town_hall_level;
    mGameLevel = level;
    ss << GameManager::getThis()->housingLimitation->gold_required.at(level) << "Y";
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 24.0f, 5.0f), kCCTextAlignmentLeft);
    moneyLabel->setAnchorPoint(ccp(0, 1));
    moneyLabel->setPosition(ccp(-90 + hw, -205 + hh));
    moneyLabel->setColor(colorYellow);
    this->addChild(moneyLabel);
    
    foodIcon = CCSprite::create("rice_icon.png");
    foodIcon->setScale(0.4f);
    foodIcon->setAnchorPoint(ccp(0, 1));
    foodIcon->setPosition(ccp(0 + hw, -180 + hh));
    this->addChild(foodIcon);
    
    ss.str(string());
    ss << GameManager::getThis()->housingLimitation->food_required.at(level);
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 24.0f, 5.0f), kCCTextAlignmentLeft);
    foodLabel->setAnchorPoint(ccp(0, 1));
    foodLabel->setPosition(ccp(80 + hw, -205 + hh));
    foodLabel->setColor(colorYellow);
    this->addChild(foodLabel);
    
    upgradeButton = CCMenuItemImage::create("upgrade.png", "upgrade.png", this, menu_selector(BuildingInfoMenu::upgrade));
   // upgradeButton->setScale(f);
    upgradeButton->setAnchorPoint(ccp(0, 1));
    upgradeButton->setPosition(ccp(150 + hw, -105 + hh));
    
    cancelUpgradeButton = CCMenuItemImage::create("cancel.png", "cancel.png", this, menu_selector(BuildingInfoMenu::upgrade));
    cancelUpgradeButton->setScale(0.25f);
  //  cancelUpgradeButton->setScale(cancelUpgradeButton->boundingBox().size.width / cancelUpgradeButton->boundingBox().size.width, cancelUpgradeButton->boundingBox().size.height / cancelUpgradeButton->boundingBox().size.height);
    cancelUpgradeButton->setAnchorPoint(ccp(0, 1));
    cancelUpgradeButton->setPosition(ccp(160 + hw, -165 + hh));
    
    menuItemsUpgrade = CCArray::create();
    menuItemsUpgrade->retain();
    
    menuItemsUpgrade->addObject(upgradeButton);
    menuItemsUpgrade->addObject(cancelUpgradeButton);
    
    upgradeMenu = CCMenu::createWithArray(menuItemsUpgrade);
    upgradeMenu->setPosition(CCPointZero);
    
    this->addChild(upgradeMenu);
    
    upgradeBar = new ProgressBar();
    upgradeBar->createProgressBar(
                                       CCRectMake(0, 0, 80, 20),
                                       CCRectMake(5, 5, 70, 10),
                                       "Energy_brown bar.png",
                                       "NONE",
                                       "NONE",
                                       "Energybar.png",
                                        true
                                       );
    upgradeBar->setValue((float)building->current_upgrade_unit / (float)building->upgrade_unit_max);
    upgradeBar->setAnchorPoint(ccp(0, 1));
    upgradeBar->setPosition(ccp(-230 + hw, -90 + hh));
    this->addChild(upgradeBar);
    
    mGameUpgradeUnit = building->current_upgrade_unit;
    ss.str(string());
    ss << building->current_upgrade_unit << "/" << building->upgrade_unit_max;
    upgradeBarLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 24.0f, 5.0f), kCCTextAlignmentLeft);
    upgradeBarLabel->setAnchorPoint(ccp(0, 1));
    upgradeBarLabel->setPosition(ccp(-120 + hw, -90 + hh));
    upgradeBarLabel->setColor(colorYellow);
    this->addChild(upgradeBarLabel);
    
    ss.str(string());
    ss << "HOUSE:";
    houseLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    houseLimitTitle->setAnchorPoint(ccp(0, 1));
    houseLimitTitle->setPosition(ccp(60 + hw, 100 + hh));
    houseLimitTitle->setColor(colorYellow);
    this->addChild(houseLimitTitle);
    
    CCArray* allHouse = GameScene::getThis()->buildingHandler->housingOnMap;
    mGameHouseNumber = allHouse->count();
    
    ss.str(string());
    ss << allHouse->count() << "/" << GameManager::getThis()->housingLimitation->housing_limits.at(level);
    houseLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    houseLimitLabel->setAnchorPoint(ccp(0, 1));
    houseLimitLabel->setPosition(ccp(240 + hw, 100 + hh));
    houseLimitLabel->setColor(colorYellow);
    this->addChild(houseLimitLabel);
    
    ss.str(string());
    ss << "GRANARY:";
    granaryLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    granaryLimitTitle->setAnchorPoint(ccp(0, 1));
    granaryLimitTitle->setPosition(ccp(60 + hw, 70 + hh));
    granaryLimitTitle->setColor(colorYellow);
    this->addChild(granaryLimitTitle);
    
    CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
    mGameGranaryNumber = allGranary->count();
    
    ss.str(string());
    ss << allGranary->count() << "/" << GameManager::getThis()->housingLimitation->granary_limits.at(level);
    granaryLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    granaryLimitLabel->setAnchorPoint(ccp(0, 1));
    granaryLimitLabel->setPosition(ccp(240 + hw, 70 + hh));
    granaryLimitLabel->setColor(colorYellow);
    this->addChild(granaryLimitLabel);
    
    ss.str(string());
    ss << "FARM:";
    farmLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    farmLimitTitle->setAnchorPoint(ccp(0, 1));
    farmLimitTitle->setPosition(ccp(60 + hw, 40 + hh));
    farmLimitTitle->setColor(colorYellow);
    this->addChild(farmLimitTitle);
    
    CCArray* allFarm = GameScene::getThis()->buildingHandler->amenityOnMap;
    mGameFarmNumber = allFarm->count();
    
    ss.str(string());
    ss << allFarm->count() << "/" << GameManager::getThis()->housingLimitation->farm_limits.at(level);
    farmLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    farmLimitLabel->setAnchorPoint(ccp(0, 1));
    farmLimitLabel->setPosition(ccp(240 + hw, 40 + hh));
    farmLimitLabel->setColor(colorYellow);
    this->addChild(farmLimitLabel);
    
    ss.str(string());
    ss << "GUARD TOWER:";
    guardTowerLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    guardTowerLimitTitle->setAnchorPoint(ccp(0, 1));
    guardTowerLimitTitle->setPosition(ccp(60 + hw, 10 + hh));
    guardTowerLimitTitle->setColor(colorYellow);
    this->addChild(guardTowerLimitTitle);
    
    CCArray* allTower = GameScene::getThis()->buildingHandler->militaryOnMap;
    mGameTowerNumber = allTower->count();
    
    ss.str(string());
    ss << allTower->count() << "/" << GameManager::getThis()->housingLimitation->guard_tower_limits.at(level);
    guardTowerLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    guardTowerLimitLabel->setAnchorPoint(ccp(0, 1));
    guardTowerLimitLabel->setPosition(ccp(240 + hw, 10 + hh));
    guardTowerLimitLabel->setColor(colorYellow);
    this->addChild(guardTowerLimitLabel);
    
    mGameCurrentlyUpgrading = building->isCurrentUpgrading;
    
    if(building->buildingType == SPECIAL)
    {
        // special building means town hall
        foodConsumptionRateLabel->setVisible(false);
        foodConsumptionRateTitleLabel->setVisible(false);
        
        foodStorageTitleLabel->setVisible(false);
        foodStorageBar->setVisible(false);
        foodStorageLabel->setVisible(false);
        
        recoveryRateTitleLabel->setVisible(false);
        recoveryRateLabel->setVisible(false);
        
        labelStatus->setVisible(false);
        unitLabel->setVisible(false);
        unitBar->setVisible(false);
        
        workCompleteTitleLabel->setVisible(false);
        workCompleteBar->setVisible(false);
        workCompleteLabel->setVisible(false);
        
        spPrice->setVisible(false);
        spCash->setVisible(false);
        textPrice->setVisible(false);
        
        spritePopulationLabel->setVisible(false);
        
        if(mGameCurrentlyUpgrading)
        {
            cancelUpgradeButton->setVisible(true);
            upgradeButton->setVisible(false);
        }
        else
        {
            cancelUpgradeButton->setVisible(false);
            upgradeButton->setVisible(true);
        }
    }
    else
    {
        moneyIcon->setVisible(false);
        moneyLabel->setVisible(false);
        
        foodIcon->setVisible(false);
        foodLabel->setVisible(false);
        
        upgradeButton->setVisible(false);
        cancelUpgradeButton->setVisible(false);
        
        upgradeBar->setVisible(false);
        upgradeBarLabel->setVisible(false);
        
        houseLimitTitle->setVisible(false);
        houseLimitLabel->setVisible(false);
        
        granaryLimitTitle->setVisible(false);
        granaryLimitLabel->setVisible(false);
        
        farmLimitTitle->setVisible(false);
        farmLimitLabel->setVisible(false);
        
        guardTowerLimitTitle->setVisible(false);
        guardTowerLimitLabel->setVisible(false);
        
        labelLevel->setVisible(false);
        
        if(building->buildingType == HOUSING)
        {
            foodConsumptionRateLabel->setVisible(false);
            foodConsumptionRateTitleLabel->setVisible(false);
            
            foodStorageTitleLabel->setVisible(false);
            foodStorageBar->setVisible(false);
            foodStorageLabel->setVisible(false);
            
            workCompleteTitleLabel->setVisible(false);
            workCompleteBar->setVisible(false);
            workCompleteLabel->setVisible(false);
        }
        else if(building->buildingType == AMENITY)
        {
            foodConsumptionRateTitleLabel->setVisible(false);
            foodConsumptionRateLabel->setVisible(false);
            
            recoveryRateTitleLabel->setVisible(false);
            recoveryRateLabel->setVisible(false);
        }
        else if(building->buildingType == GRANARY)
        {
            workCompleteTitleLabel->setVisible(false);
            workCompleteBar->setVisible(false);
            workCompleteLabel->setVisible(false);
        }
        else if(building->buildingType == MILITARY)
        {
            foodConsumptionRateTitleLabel->setVisible(false);
            foodConsumptionRateLabel->setVisible(false);
            
            recoveryRateTitleLabel->setVisible(false);
            recoveryRateLabel->setVisible(false);
            
            foodStorageTitleLabel->setVisible(false);
            foodStorageBar->setVisible(false);
            foodStorageLabel->setVisible(false);
            
            workCompleteTitleLabel->setVisible(false);
            workCompleteBar->setVisible(false);
            workCompleteLabel->setVisible(false);
        }
    }
    
    reposition();
    this->schedule(schedule_selector(BuildingInfoMenu::update), 0.06f);
}

void BuildingInfoMenu::onMenuItemSelected(CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case -1:
        {
            this->closeMenu(true);
            GameScene::getThis()->setTouchEnabled(true);
        }
        break;
            
        default:
        {
            SelectPopulation* selectPopulationMenu = SelectPopulation::create(this->building);
            selectPopulationMenu->useAsTopmostPopupMenu();
        }
        break;
    }
}

void BuildingInfoMenu::reposition()
{
    //Refresh first, in case vacancy changed
    refreshAllMenuItemValues();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
    // Anchored top left
    spriteBuilding->CCNode::setPosition(-290.0f + hw, 220.0f + hh);
    labelLevel->CCNode::setPosition(-150.0f + hw, -50.0f + hh);
    
    labelStatus->CCNode::setPosition(60.0f + hw, 100.0f + hh);
    unitBar->CCNode::setPosition(60.0f + hw, 70.0f + hh);
    unitLabel->CCNode::setPosition(160.0f + hw, 70.0f + hh);
    
    recoveryRateTitleLabel->CCNode::setPosition(50.0f + hw, 40.0f + hh);
    recoveryRateLabel->CCNode::setPosition(150.0f + hw, 40.0f + hh);
    
    foodConsumptionRateTitleLabel->CCNode::setPosition(50.0f + hw, 10.0f + hh);
    foodConsumptionRateLabel->CCNode::setPosition(150.0f + hw, 10.0f + hh);
    
    foodStorageTitleLabel->CCNode::setPosition(50.0f + hw, -20.0f + hh);
    foodStorageBar->CCNode::setPosition(50.0f + hw, -50.0f + hh);
    foodStorageLabel->CCNode::setPosition(150.0f + hw, -50.0f + hh);
    
    workCompleteTitleLabel->CCNode::setPosition(50.0f + hw, -80.0f + hh);
    workCompleteBar->CCNode::setPosition(50.0f + hw, -110.0f + hh);
    workCompleteLabel->CCNode::setPosition(150.0f + hw, -110.0f + hh);
    
    // Anchored top
    textName->CCNode::setPosition(hw, halfHeight - 20.0f + hh);
    
    // Anchored top right
    buttonClose->setPosition(ccp(halfWidth - 60.0f + hw, halfHeight - 20.0f + hh));
    
    // Anchored bottom right
    textPrice->CCNode::setPosition(halfWidth -200.0f + hw, -halfHeight + 40.0f + hh);
    spPrice->CCNode::setPosition(halfWidth - 430.0f + hw, -halfHeight + 40.0f + hh);
    
    // Anchored top left
    spritePopulationLabel->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw, -35.0 + hh));
    
    for (int i = 0; i < spritePopulationSlot->count(); i++)
    {
        CCSprite* spSlot = (CCSprite*) spritePopulationSlot->objectAtIndex(i);
        spSlot->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + (spSlot->boundingBox().size.width + 5.0f) * i, -60.0f + hh));
    }
    
    for (int i = 0; i < spritePopulationBackground->count(); i++)
    {
        CCSprite* spb = (CCSprite*) spritePopulationBackground->objectAtIndex(i);
        spb->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + (spb->boundingBox().size.width + 5.0f) * i, -60.0f + hh));
    }
    
    for (int i = 0; i < spritePopulation->count(); i++)
    {
        CCMenuItemImage* spp = (CCMenuItemImage*) spritePopulation->objectAtIndex(i);
        spp->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + 5.0f + (((CCSprite*)spritePopulationSlot->objectAtIndex(0))->boundingBox().size.width + 5.0f) * i, -62.0f + hh));
    }
    
    selectWorkerButton->setPosition(ccp(150.0f + hw, -halfHeight + 132.0f + hh));
}

void BuildingInfoMenu::refreshAllMenuItemValues()
{
    bool isPositionDirty = false;
    
    std::stringstream ss;
    
    /*
    if (mBuildingPrice != building->buildingCost * GameScene::getThis()->policyHandler->percentTax * 0.01)
    {
        mBuildingPrice = building->buildingCost * GameScene::getThis()->policyHandler->percentTax * 0.01;
        ss.str(std::string());
        ss << "Yearly: " << mBuildingPrice << " G";
        textPrice->setString(ss.str().c_str());
    }
    */
    
    if (mBuildingLevel != GameManager::getThis()->town_hall_level)
    {
        mBuildingLevel = GameManager::getThis()->town_hall_level;
        ss.str(std::string());
        ss << "Level: " << (mBuildingLevel + 1);
        labelLevel->setString(ss.str().c_str());
    }
    
    // check whether the building has been completely build or not
    if(mBuildingUnderConstruction != building->isUnderConstruction())
    {
        mBuildingUnderConstruction = building->isUnderConstruction();
        if(building->buildingType != AMENITY && building->buildingType != MILITARY)
        {
            selectWorkerButton->setVisible(false);
        }
        
        if(building->buildingType == HOUSING)
        {
            mBuildingVacancy = building->populationLimit;
        }
        else
        {
            mBuildingVacancy = building->number_of_jobs;
        }
        
        for(int i = 0; i < spritePopulationSlot->count(); i++)
        {
            this->removeChild((CCNode*) spritePopulationSlot->objectAtIndex(i));
        }
        spritePopulationSlot->removeAllObjects();
        CC_SAFE_RELEASE(spritePopulationSlot);
        
        for(int i = 0; i < spritePopulationBackground->count(); i++)
        {
            this->removeChild((CCNode*) spritePopulationBackground->objectAtIndex(i));
        }
        spritePopulationBackground->removeAllObjects();
        CC_SAFE_RELEASE(spritePopulationBackground);
        
        for(int i = 0; i < spritePopulationMenu->count(); i++)
        {
            this->removeChild((CCNode*) spritePopulationMenu->objectAtIndex(i));
        }
        spritePopulationMenu->removeAllObjects();
        CC_SAFE_RELEASE(spritePopulationMenu);
        
        /*
        for(int i = 0; i < spritePopulation->count(); i++)
        {
            
            this->removeChild((CCNode*) spritePopulation->objectAtIndex(i));
        }
        */
        spritePopulation->removeAllObjects();
        CC_SAFE_RELEASE(spritePopulation);
        
        spritePopulationSlot = CCArray::create();
        spritePopulationSlot->retain();
        
        spritePopulationBackground = CCArray::create();
        spritePopulationBackground->retain();
        
        spritePopulationMenu = CCArray::create();
        spritePopulationMenu->retain();
        
        spritePopulation = CCArray::create();
        spritePopulation->retain();
        
        // Create population icons
        for (int i = 0; i < mBuildingVacancy; i++)
        {
            CCSprite* vacancySlot = CCSprite::create("assign_menu_unfilled.png");
            vacancySlot->setScale(70.0f / vacancySlot->boundingBox().size.width);
            vacancySlot->setAnchorPoint(ccp(0, 1));
            this->addChild(vacancySlot);
            spritePopulationSlot->addObject(vacancySlot);
        }
        
        /*
        for (int i = 0; i < building->memberSpriteList->count(); i++)
        {
            GameSprite* gameSprite = (GameSprite*) building->memberSpriteList->objectAtIndex(i);
            
            CCSprite* currentMemberBackground = CCSprite::create("assign_menu_filled.png");
            currentMemberBackground->setScale(70.0f / currentMemberBackground->boundingBox().size.width);
            currentMemberBackground->setAnchorPoint(ccp(0, 1));
            this->addChild(currentMemberBackground);
            spritePopulationBackground->addObject(currentMemberBackground);
            
            std::string tempStr = gameSprite->spriteName;
            CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(BuildingInfoMenu::showSprite));
            memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
            memberSprite->setAnchorPoint(ccp(0, 1));
            memberSprite->setTag(i);
            spritePopulation->addObject(memberSprite);
            
            CCArray* newMenuItems = CCArray::create();
            newMenuItems->addObject(memberSprite);
            CCMenu* newMenu = CCMenu::createWithArray(newMenuItems);
            newMenu->setPosition(CCPointZero);
            this->addChild(newMenu, 4);
            spritePopulationMenu->addObject(newMenu);
        }
        
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
        
        float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    //    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
        
        for (int i = 0; i < spritePopulationSlot->count(); i++)
        {
            CCSprite* spSlot = (CCSprite*) spritePopulationSlot->objectAtIndex(i);
            spSlot->setPosition(ccp(-halfWidth * 3.0f / 4.0f + (spSlot->boundingBox().size.width + 5.0f) * i, -60.0f));
        }
        
        for (int i = 0; i < spritePopulationBackground->count(); i++)
        {
            CCSprite* spb = (CCSprite*) spritePopulationBackground->objectAtIndex(i);
            spb->setPosition(ccp(-halfWidth * 3.0f / 4.0f + (spb->boundingBox().size.width + 5.0f) * i, -60.0f));
        }
        
        for (int i = 0; i < spritePopulation->count(); i++)
        {
            CCMenuItemImage* spp = (CCMenuItemImage*) spritePopulation->objectAtIndex(i);
            spp->setPosition(ccp(-halfWidth * 3.0f / 4.0f + 5.0f + (((CCSprite*)spritePopulationSlot->objectAtIndex(0))->boundingBox().size.width + 5.0f) * i, -62.0f));
        }
        */
        /*
        spritePopulationBackground->removeAllObjects();
        spritePopulation->removeAllObjects();
        */
        
        spriteBuilding->setOpacity(255);
        stringstream ss;
        ss << "Ready for services!";
        labelStatus->setString(ss.str().c_str());
    }
    
    // check whether there is population change
    if(mBuildingUnderConstruction)
    {
        if(mBuildingCurrPopulation != building->builderLimit)
        {
            mBuildingCurrPopulation = building->builderLimit;
            for(int i = 0; i < spritePopulationBackground->count(); i++)
            {
                this->removeChild((CCNode*) spritePopulationBackground->objectAtIndex(i));
            }
            spritePopulationBackground->removeAllObjects();
            
            for(int i = 0; i < spritePopulationMenu->count(); i++)
            {
                this->removeChild((CCNode*) spritePopulationMenu->objectAtIndex(i));
            }
            spritePopulationMenu->removeAllObjects();
            
            for(int i = 0; i < spritePopulation->count(); i++)
            {
                this->removeChild((CCNode*) spritePopulation->objectAtIndex(i));
            }
            spritePopulation->removeAllObjects();
            
            for (int i = 0; i < building->memberSpriteList->count(); i++)
            {
                GameSprite* gameSprite = (GameSprite*) building->memberSpriteList->objectAtIndex(i);
                
                CCSprite* currentMemberBackground = CCSprite::create("assign_menu_filled.png");
                currentMemberBackground->setScale(70.0f / currentMemberBackground->boundingBox().size.width);
                currentMemberBackground->setAnchorPoint(ccp(0, 1));
                this->addChild(currentMemberBackground);
                spritePopulationBackground->addObject(currentMemberBackground);
                
                std::string tempStr = gameSprite->spriteName;
                CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(BuildingInfoMenu::showSprite));
                memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
                memberSprite->setAnchorPoint(ccp(0, 1));
                memberSprite->setTag(i);
                spritePopulation->addObject(memberSprite);
                
                CCArray* newMenuItems = CCArray::create();
                newMenuItems->addObject(memberSprite);
                CCMenu* newMenu = CCMenu::createWithArray(newMenuItems);
                newMenu->setPosition(CCPointZero);
                this->addChild(newMenu, 4);
                spritePopulationMenu->addObject(newMenu);
            }
        }
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
      //  float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
        float hw = screenSize.width / 2.0f;
        float hh = screenSize.height / 2.0f;
        
        for (int i = 0; i < spritePopulationSlot->count(); i++)
        {
            CCSprite* spSlot = (CCSprite*) spritePopulationSlot->objectAtIndex(i);
            spSlot->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + (spSlot->boundingBox().size.width + 5.0f) * i, -60.0f + hh));
        }
        
        for (int i = 0; i < spritePopulationBackground->count(); i++)
        {
            CCSprite* spb = (CCSprite*) spritePopulationBackground->objectAtIndex(i);
            spb->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + (spb->boundingBox().size.width + 5.0f) * i, -60.0f + hh));
        }
        
        for (int i = 0; i < spritePopulation->count(); i++)
        {
            CCMenuItemImage* spp = (CCMenuItemImage*) spritePopulation->objectAtIndex(i);
            spp->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + 5.0f + (((CCSprite*)spritePopulationSlot->objectAtIndex(0))->boundingBox().size.width + 5.0f) * i, -62.0f + hh));
        }
    }
    else
    {
        if(mBuildingCurrPopulation != building->memberSpriteList->count())
        {
            mBuildingCurrPopulation = building->memberSpriteList->count();
            spritePopulationBackground->removeAllObjects();
            spritePopulationMenu->removeAllObjects();
            spritePopulation->removeAllObjects();
            
            for (int i = 0; i < mBuildingCurrPopulation; i++)
            {
                GameSprite* gameSprite = (GameSprite*) building->memberSpriteList->objectAtIndex(i);
                
                CCSprite* currentMemberBackground = CCSprite::create("assign_menu_filled.png");
                currentMemberBackground->setScale(70.0f / currentMemberBackground->boundingBox().size.width);
                currentMemberBackground->setAnchorPoint(ccp(0, 1));
                this->addChild(currentMemberBackground);
                spritePopulationBackground->addObject(currentMemberBackground);
                
                std::string tempStr = gameSprite->spriteName;
                CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(BuildingInfoMenu::showSprite));
                memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
                memberSprite->setAnchorPoint(ccp(0, 1));
                memberSprite->setTag(i);
                spritePopulation->addObject(memberSprite);
                
                CCArray* newMenuItems = CCArray::create();
                newMenuItems->addObject(memberSprite);
                CCMenu* newMenu = CCMenu::createWithArray(newMenuItems);
                newMenu->setPosition(CCPointZero);
                this->addChild(newMenu, 4);
                spritePopulationMenu->addObject(newMenu);
            }
        }
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
        //float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
        float hw = screenSize.width / 2.0f;
        float hh = screenSize.height / 2.0f;
        
        for (int i = 0; i < spritePopulationSlot->count(); i++)
        {
            CCSprite* spSlot = (CCSprite*) spritePopulationSlot->objectAtIndex(i);
            spSlot->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + (spSlot->boundingBox().size.width + 5.0f) * i, -60.0f + hh));
        }
        
        for (int i = 0; i < spritePopulationBackground->count(); i++)
        {
            CCSprite* spb = (CCSprite*) spritePopulationBackground->objectAtIndex(i);
            spb->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + (spb->boundingBox().size.width + 5.0f) * i, -60.0f + hh));
        }
        
        for (int i = 0; i < spritePopulation->count(); i++)
        {
            CCMenuItemImage* spp = (CCMenuItemImage*) spritePopulation->objectAtIndex(i);
            spp->setPosition(ccp(-halfWidth * 3.0f / 4.0f + hw + 5.0f + (((CCSprite*)spritePopulationSlot->objectAtIndex(0))->boundingBox().size.width + 5.0f) * i, -62.0f + hh));
        }
    }
    
    // during construction update the construction status of the building
    if(mBuildingUnitCurrent != building->build_uint_current || mBuildingUnitRequired != building->build_uint_required)
    {
        mBuildingUnitCurrent = building->build_uint_current;
        mBuildingUnitRequired = building->build_uint_required;
        ss.str(std::string());
        ss << building->build_uint_current << "/" << building->build_uint_required;
        unitLabel->setString(ss.str().c_str());
        unitBar->setValue((float) building->build_uint_current / (float) building->build_uint_required);
    }
    
    // updating the food storage
    if(mBuildingFoodStorageCurrent != building->currentStorage || mBuildingFoodStorageMax != building->storageLimit)
    {
        mBuildingFoodStorageCurrent = building->currentStorage;
        mBuildingFoodStorageMax = building->storageLimit;
        ss.str(std::string());
        ss << building->currentStorage << "/" << building->storageLimit;
        foodStorageLabel->setString(ss.str().c_str());
        foodStorageBar->setValue((float) building->currentStorage / (float) building->storageLimit);
    }
    
    // updating the work unit done in the building
    if(mBuildingWorkUnitCurrent != building->work_unit_current || mBuildingWorkUnitRequired != building->work_unit_required)
    {
        mBuildingWorkUnitCurrent = building->work_unit_current;
        mBuildingWorkUnitRequired = building->work_unit_required;
        ss.str(std::string());
        ss << (int) building->work_unit_current << "/" << (int) building->work_unit_required;
        workCompleteLabel->setString(ss.str().c_str());
        workCompleteBar->setValue((float) building->work_unit_current / (float) building->work_unit_required);
    }
    
    if (GameManager::getThis()->town_hall_level != mGameLevel)
    {
        mGameLevel = GameManager::getThis()->town_hall_level;
        
        ss.str(std::string());
        ss << "LEVEL: " << (mGameLevel + 1);
        labelLevel->setString(ss.str().c_str());
        
        upgradeBar->setValue((float) building->current_upgrade_unit / (float) building->upgrade_unit_max);
        ss.str(string());
        ss << building->current_upgrade_unit << "/" << building->upgrade_unit_max;
        upgradeBarLabel->setString(ss.str().c_str());
        
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->housingOnMap->count() << "/" << GameManager::getThis()->housingLimitation->housing_limits.at(mGameLevel);
        houseLimitLabel->setString(ss.str().c_str());
        
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->granaryOnMap->count() << "/" << GameManager::getThis()->housingLimitation->granary_limits.at(mGameLevel);
        granaryLimitLabel->setString(ss.str().c_str());
        
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->amenityOnMap->count() << "/" << GameManager::getThis()->housingLimitation->farm_limits.at(mGameLevel);
        farmLimitLabel->setString(ss.str().c_str());
        
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->militaryOnMap->count() << "/" << GameManager::getThis()->housingLimitation->guard_tower_limits.at(mGameLevel);
        guardTowerLimitLabel->setString(ss.str().c_str());
        
        upgradeButton->setVisible(true);
        cancelUpgradeButton->setVisible(false);
    }
    
    if (building->current_upgrade_unit != mGameUpgradeUnit)
    {
        mGameUpgradeUnit = building->current_upgrade_unit;
        ss.str(string());
        ss << building->current_upgrade_unit << "/" << building->upgrade_unit_max;
        upgradeBarLabel->setString(ss.str().c_str());
        
        upgradeBar->setValue((float) building->current_upgrade_unit / (float) building->upgrade_unit_max);
    }
    
    if (GameScene::getThis()->buildingHandler->housingOnMap->count() != mGameHouseNumber)
    {
        mGameHouseNumber = GameScene::getThis()->buildingHandler->housingOnMap->count();
        ss.str(std::string());
        ss << GameScene::getThis()->buildingHandler->housingOnMap->count() << "/" << GameManager::getThis()->housingLimitation->housing_limits.at(mGameLevel);
        houseLimitLabel->setOpacity(255);
    }
    
    if (GameScene::getThis()->buildingHandler->granaryOnMap->count() != mGameGranaryNumber)
    {
        mGameGranaryNumber = GameScene::getThis()->buildingHandler->granaryOnMap->count();
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->granaryOnMap->count() << "/" << GameManager::getThis()->housingLimitation->granary_limits.at(mGameLevel);
        granaryLimitLabel->setString(ss.str().c_str());
    }
    
    if (GameScene::getThis()->buildingHandler->amenityOnMap->count() != mGameFarmNumber)
    {
        mGameFarmNumber = GameScene::getThis()->buildingHandler->amenityOnMap->count();
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->amenityOnMap->count() << "/" << GameManager::getThis()->housingLimitation->farm_limits.at(mGameLevel);
        farmLimitLabel->setString(ss.str().c_str());
    }
    
    if (GameScene::getThis()->buildingHandler->militaryOnMap->count() != mGameTowerNumber)
    {
        mGameTowerNumber = GameScene::getThis()->buildingHandler->militaryOnMap->count();
        ss.str(string());
        ss << GameScene::getThis()->buildingHandler->militaryOnMap->count() << "/" << GameManager::getThis()->housingLimitation->guard_tower_limits.at(mGameLevel);
        guardTowerLimitLabel->setString(ss.str().c_str());
    }
    
    if (isPositionDirty)
        reposition();
}

void BuildingInfoMenu::willChangeOrientation()
{
}

void BuildingInfoMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    reposition();
}

void BuildingInfoMenu::update(float deltaTime)
{
    refreshAllMenuItemValues();
}

Building* BuildingInfoMenu::getBuilding()
{
    return building;
}

void BuildingInfoMenu::upgrade()
{
    if(building->isCurrentUpgrading)
    {
        upgradeButton->setVisible(true);
        cancelUpgradeButton->setVisible(false);
        building->isCurrentUpgrading = false;
        building->current_upgrade_unit = 0;
    }
    else
    {
        int level = GameManager::getThis()->town_hall_level;
        int food = 0;
        CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
        for (int i = 0; i < allGranary->count(); i++)
        {
            Building* b = (Building*) allGranary->objectAtIndex(i);
            food += b->currentStorage;
        }

        if(GameHUD::getThis()->money < GameManager::getThis()->housingLimitation->gold_required.at(level) || food < GameManager::getThis()->housingLimitation->food_required.at(level))
        {
            return;
        }
        
        upgradeButton->setVisible(false);
        cancelUpgradeButton->setVisible(true);
        building->isCurrentUpgrading = true;
        GameHUD::getThis()->money -= GameManager::getThis()->housingLimitation->gold_required.at(level);
        for (int i = 0; i < allGranary->count(); i++)
        {
            Building* b = (Building*) allGranary->objectAtIndex(i);
            if(food <= 0)
            {
                break;
            }
            food -= b->currentStorage;
            b->currentStorage = 0;
        }

    }
}

void BuildingInfoMenu::showSprite(CCObject *pSender)
{
    if(building->isCurrentConstructing)
    {
        return;
    }
    
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = pMenuItem->getTag();
    
    GameSprite* gameSprite = (GameSprite*) building->memberSpriteList->objectAtIndex(tag);
    
    if (SpriteInfoMenu::getThis() == NULL)
    {
        GameScene::getThis()->tapped = true;
        PopupMenu::backupCurrentPopupMenu();
        SpriteInfoMenu* spriteInfoMenu = SpriteInfoMenu::create(gameSprite);
        spriteInfoMenu->useAsTopmostPopupMenu();
    }
}

void BuildingInfoMenu::selectPop()
{
    PopupMenu::backupCurrentPopupMenu();
    SelectPopulation* selectPopulationMenu = SelectPopulation::create(building);
    selectPopulationMenu->useAsTopmostPopupMenu();
    
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachFarming)
    {
        selectPopulationMenu->setZOrder(35);
        TutorialManager::getThis()->miniDragon->clickNext();
    }
}
