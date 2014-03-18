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

float portraitScale; //hack

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
    spritePopulationSlot->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulationSlot);
    
    spritePopulation->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulation);
    
    spritePopulationBackground->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulationBackground);
    
    spritePopulationMenu->removeAllObjects();
    CC_SAFE_RELEASE(spritePopulationMenu);
}

void BuildingInfoMenu::createMenuItems()
{
    if (building->buildingName.length() == 0)
    {
        this->closeMenu();
        return;
    }
    
    
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    
    // Create constant menu items
    spriteBackground = CCSprite::create("outerbox.png");
    spriteBackground->setScale(background_rect->width / spriteBackground->boundingBox().size.width);
    
    spriteBackgroundInner = CCSprite::create("innerbox-info.png");
    spriteBackgroundInner->setScale(spriteBackground->getScale());
    
    // Set variables which may become dirty
    mBuildingLevel = building->currentLevel;
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
            mBuildingVacancy = building->populationLimit;
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
    spPrice = CCSprite::create("money icon.png");
    spPrice->setScale(0.75);
    spCash = CCSprite::create("money icon.png");
    //spCash = CCSprite::create("happiness icon.png");
    spCash->setScale(0.75);
    
    std::stringstream ss;
    ss << "Yearly: " << mBuildingPrice << " G";
    textPrice = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 26, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    textPrice->setColor(colorYellow);
    
    // Sprite
    spriteBuilding = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    spriteBuilding->setScale(128.0f / spriteBuilding->boundingBox().size.width);
    spriteBuilding->setAnchorPoint(ccp(0.5, 0.5));
    
    if(building->isUnderConstruction())
    {
        spriteBuilding->setOpacity(120);
    }
    
    // Attribute labels
    ss.str(std::string());
    ss << "Level: " << mBuildingLevel;
    labelLevel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    labelLevel->setColor(colorGreen);
    
    ss.str(std::string());
    if(building->build_uint_current < building->build_uint_required)
    {
        ss << "Under construction!";
    }
    else
    {
        ss << "Ready for service";
    }
    labelStatus = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelStatus->setColor(colorBlack);
    
    ss.str(std::string());
    ss << building->build_uint_current << "/" << building->build_uint_required;
    unitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    unitLabel->setColor(colorBlack);
    
    unitBar = new ProgressBar();
    unitBar->createProgressBar(
                               CCRectMake(0, 0, 80, 20),
                               CCRectMake(5, 5, 70, 10),
                               "loadingbar-empty.png",
                               "loadingbar-left.png",
                               "loadingbar-right.png",
                               "loadingbar-full.png"
    );
    unitBar->setValue((float)building->build_uint_current / (float)building->build_uint_required);
    
    /*
    // loyalty
    ss.str(std::string());
    ss << "0";
    labelLoy = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelLoy->setColor(colorBlack);
    
    // hapiness
    ss.str(std::string());
    ss << "0";
    labelHap = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelHap->setColor(colorBlack);
    */
    
    // recovery rate
    ss.str(std::string());
    ss << "RR:";
    recoveryRateTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    recoveryRateTitleLabel->setColor(colorBlack);
    
    ss.str(std::string());
    ss << building->recovery_rate;
    recoveryRateLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    recoveryRateLabel->setColor(colorBlack);
    
    // food consumption rate
    ss.str(std::string());
    ss << "FC:";
    foodConsumptionRateTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodConsumptionRateTitleLabel->setColor(colorBlack);
    
    ss.str(std::string());
    
    ss << building->food_consumption_rate;
    foodConsumptionRateLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodConsumptionRateLabel->setColor(colorBlack);
    
    // food storage limit
    ss.str(std::string());
    ss << "Food Storage:";
    foodStorageTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodStorageTitleLabel->setColor(colorBlack);
    
    // food storage
    foodStorageBar = new ProgressBar();
    foodStorageBar->createProgressBar(
                               CCRectMake(0, 0, 80, 20),
                               CCRectMake(5, 5, 70, 10),
                               "loadingbar-empty.png",
                               "loadingbar-left.png",
                               "loadingbar-right.png",
                               "loadingbar-full.png"
                               );
    foodStorageBar->setValue((float)building->currentStorage / (float)building->storageLimit);
    
    ss.str(std::string());
    ss << building->currentStorage << "/" << building->storageLimit;
    foodStorageLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodStorageLabel->setColor(colorBlack);
    
    // work unit done
    ss.str(std::string());
    ss << "Work Unit Done:";
    workCompleteTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workCompleteTitleLabel->setColor(colorBlack);
    
    workCompleteBar = new ProgressBar();
    workCompleteBar->createProgressBar(
                                      CCRectMake(0, 0, 80, 20),
                                      CCRectMake(5, 5, 70, 10),
                                      "loadingbar-empty.png",
                                      "loadingbar-left.png",
                                      "loadingbar-right.png",
                                      "loadingbar-full.png"
                                      );
    workCompleteBar->setValue((float)building->work_unit_current / (float)building->work_unit_required);
    
    ss.str(std::string());
    ss << (int) building->work_unit_current << "/" << (int) building->work_unit_required;
    workCompleteLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workCompleteLabel->setColor(colorBlack);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCMenuItemImage::create("closebutton-up.png", "closebutton-down.png", this, menu_selector(BuildingInfoMenu::onMenuItemSelected));
    buttonClose->setScale(0.4);
    buttonClose->setTag(-1);
    
    menuItems->addObject(buttonClose);
    
    selectWorkerButton = CCMenuItemImage::create("schedule.png", "schedule.png", this, menu_selector(BuildingInfoMenu::selectPop));
    selectWorkerButton->setScale( 60.0f / selectWorkerButton->boundingBox().size.width );
    selectWorkerButton->setAnchorPoint(ccp(0, 1));
    
    if(!building->isUnderConstruction() && building->buildingType != AMENITY && building->buildingType != MILITARY)
    {
        selectWorkerButton->setVisible(false);
    }
    
    menuItems->addObject(selectWorkerButton);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Add children
    this->addChild(spriteBackground);
    this->addChild(spriteBackgroundInner);
    this->addChild(spPrice);
    this->addChild(textName);
    this->addChild(textPrice);
    this->addChild(menu, 3);
    
    this->addChild(spriteBuilding);
    this->addChild(labelLevel);
    
    this->addChild(labelStatus);
    this->addChild(unitBar);
    this->addChild(unitLabel);
    //this->addChild(spLoy);
    //this->addChild(spHap);
    //this->addChild(labelLoy);
    //this->addChild(labelHap);
    
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
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    textName->setAnchorPoint(ccp(0.5, 1));
    spPrice->setAnchorPoint(ccp(1, 0));
    textPrice->setAnchorPoint(ccp(1, 0));
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    spriteBuilding->setAnchorPoint(ccp(0, 1));
    labelLevel->setAnchorPoint(ccp(0.5, 1));
    
    labelStatus->setAnchorPoint(ccp(0, 1));
    unitBar->setAnchorPoint(ccp(0, 1));
    unitLabel->setAnchorPoint(ccp(0, 1));
    //spLoy->setAnchorPoint(ccp(0, 1));
    //spHap->setAnchorPoint(ccp(0, 1));
    //labelLoy->setAnchorPoint(ccp(0, 1));
    //labelHap->setAnchorPoint(ccp(0, 1));
    
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
    spritePopulationLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    spritePopulationLabel->setColor(colorGreen);
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
    moneyIcon = CCSprite::create("yuanbao_icon.png");
    moneyIcon->setScale(0.5f);
    moneyIcon->setAnchorPoint(ccp(0, 1));
    moneyIcon->setPosition(ccp(-200, -190));
    this->addChild(moneyIcon);
    
    ss.str(string());
    int level = GameManager::getThis()->town_hall_level;
    mGameLevel = level;
    ss << GameManager::getThis()->housingLimitation->gold_required.at(level) << "G";
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24, CCSizeMake(ss.str().length() * 24.0f, 5.0f), kCCTextAlignmentLeft);
    moneyLabel->setAnchorPoint(ccp(0, 1));
    moneyLabel->setPosition(ccp(-90, -205));
    moneyLabel->setColor(colorGreen);
    this->addChild(moneyLabel);
    
    foodIcon = CCSprite::create("rice_icon.png");
    foodIcon->setScale(0.5f);
    foodIcon->setAnchorPoint(ccp(0, 1));
    foodIcon->setPosition(ccp(30, -180));
    this->addChild(foodIcon);
    
    ss.str(string());
    ss << GameManager::getThis()->housingLimitation->food_required.at(level);
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24, CCSizeMake(ss.str().length() * 24.0f, 5.0f), kCCTextAlignmentLeft);
    foodLabel->setAnchorPoint(ccp(0, 1));
    foodLabel->setPosition(ccp(110, -205));
    foodLabel->setColor(colorGreen);
    this->addChild(foodLabel);
    
    upgradeButton = CCMenuItemImage::create("upgrade.png", "upgrade.png", this, menu_selector(BuildingInfoMenu::upgrade));
    upgradeButton->setScale(0.5f);
    upgradeButton->setAnchorPoint(ccp(0, 1));
    upgradeButton->setPosition(ccp(250, -180));
    
    cancelUpgradeButton = CCMenuItemImage::create("cancel.png", "cancel.png", this, menu_selector(BuildingInfoMenu::upgrade));
    cancelUpgradeButton->setScale(upgradeButton->boundingBox().size.width / cancelUpgradeButton->boundingBox().size.width, upgradeButton->boundingBox().size.height / cancelUpgradeButton->boundingBox().size.height);
    cancelUpgradeButton->setAnchorPoint(ccp(0, 1));
    cancelUpgradeButton->setPosition(ccp(250, -180));
    
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
                                       "loadingbar-empty.png",
                                       "loadingbar-left.png",
                                       "loadingbar-right.png",
                                       "loadingbar-full.png"
                                       );
    upgradeBar->setValue((float)building->current_upgrade_unit / (float)building->upgrade_unit_max);
    upgradeBar->setAnchorPoint(ccp(0, 1));
    upgradeBar->setPosition(ccp(-230, -90));
    this->addChild(upgradeBar);
    
    mGameUpgradeUnit = building->current_upgrade_unit;
    ss.str(string());
    ss << building->current_upgrade_unit << "/" << building->upgrade_unit_max;
    upgradeBarLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 24.0f, 5.0f), kCCTextAlignmentLeft);
    upgradeBarLabel->setAnchorPoint(ccp(0, 1));
    upgradeBarLabel->setPosition(ccp(-120, -90));
    upgradeBarLabel->setColor(colorGreen);
    this->addChild(upgradeBarLabel);
    
    ss.str(string());
    ss << "HOUSE:";
    houseLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    houseLimitTitle->setAnchorPoint(ccp(0, 1));
    houseLimitTitle->setPosition(ccp(60, 100));
    houseLimitTitle->setColor(colorGreen);
    this->addChild(houseLimitTitle);
    
    CCArray* allHouse = GameScene::getThis()->buildingHandler->housingOnMap;
    mGameHouseNumber = allHouse->count();
    
    ss.str(string());
    ss << allHouse->count() << "/" << GameManager::getThis()->housingLimitation->housing_limits.at(level);
    houseLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    houseLimitLabel->setAnchorPoint(ccp(0, 1));
    houseLimitLabel->setPosition(ccp(240, 100));
    houseLimitLabel->setColor(colorGreen);
    this->addChild(houseLimitLabel);
    
    ss.str(string());
    ss << "GRANARY:";
    granaryLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    granaryLimitTitle->setAnchorPoint(ccp(0, 1));
    granaryLimitTitle->setPosition(ccp(60, 70));
    granaryLimitTitle->setColor(colorGreen);
    this->addChild(granaryLimitTitle);
    
    CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
    mGameGranaryNumber = allGranary->count();
    
    ss.str(string());
    ss << allGranary->count() << "/" << GameManager::getThis()->housingLimitation->granary_limits.at(level);
    granaryLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    granaryLimitLabel->setAnchorPoint(ccp(0, 1));
    granaryLimitLabel->setPosition(ccp(240, 70));
    granaryLimitLabel->setColor(colorGreen);
    this->addChild(granaryLimitLabel);
    
    ss.str(string());
    ss << "FARM:";
    farmLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    farmLimitTitle->setAnchorPoint(ccp(0, 1));
    farmLimitTitle->setPosition(ccp(60, 40));
    farmLimitTitle->setColor(colorGreen);
    this->addChild(farmLimitTitle);
    
    CCArray* allFarm = GameScene::getThis()->buildingHandler->amenityOnMap;
    mGameFarmNumber = allFarm->count();
    
    ss.str(string());
    ss << allFarm->count() << "/" << GameManager::getThis()->housingLimitation->farm_limits.at(level);
    farmLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    farmLimitLabel->setAnchorPoint(ccp(0, 1));
    farmLimitLabel->setPosition(ccp(240, 40));
    farmLimitLabel->setColor(colorGreen);
    this->addChild(farmLimitLabel);
    
    ss.str(string());
    ss << "GUARD TOWER:";
    guardTowerLimitTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    guardTowerLimitTitle->setAnchorPoint(ccp(0, 1));
    guardTowerLimitTitle->setPosition(ccp(60, 10));
    guardTowerLimitTitle->setColor(colorGreen);
    this->addChild(guardTowerLimitTitle);
    
    CCArray* allTower = GameScene::getThis()->buildingHandler->militaryOnMap;
    mGameTowerNumber = allTower->count();
    
    ss.str(string());
    ss << allTower->count() << "/" << GameManager::getThis()->housingLimitation->guard_tower_limits.at(level);
    guardTowerLimitLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    guardTowerLimitLabel->setAnchorPoint(ccp(0, 1));
    guardTowerLimitLabel->setPosition(ccp(240, 10));
    guardTowerLimitLabel->setColor(colorGreen);
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
            // buttonClose
            this->closeMenu(true);
            GameScene::getThis()->setTouchEnabled(true);
        }
        break;
            
        default:
        {
            /*
            SpriteInfoMenu* spriteInfoMenu = new SpriteInfoMenu((GameSprite*)pMenuItem->getTag());
            spriteInfoMenu->useAsTopmostPopupMenu();
            */
            
            //SelectPopulation* selectPopulationMenu = new SelectPopulation(this->building);
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
    
    this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    
    // Anchored top left
    spriteBuilding->CCNode::setPosition(-195.0f, 100.0f);
    labelLevel->CCNode::setPosition(-130.0f, -50.0f);
    
    labelStatus->CCNode::setPosition(60.0f, 100.0f);
    unitBar->CCNode::setPosition(60.0f, 70.0f);
    unitLabel->CCNode::setPosition(160.0f, 70.0f);
    //spLoy->CCNode::setPosition(60.0f, 50.0f);
    //spHap->CCNode::setPosition(60.0f, 0.0f);
    //labelLoy->CCNode::setPosition(160.0f, 50.0f - spLoy->boundingBox().size.height / 2.0f + labelLoy->boundingBox().size.height / 2.0f);
    //labelHap->CCNode::setPosition(160.0f, 0.0f - spHap->boundingBox().size.height / 2.0f + labelHap->boundingBox().size.height / 2.0f);
    
    recoveryRateTitleLabel->CCNode::setPosition(60.0f, 40.0f);
    recoveryRateLabel->CCNode::setPosition(160.0f, 40.0f);
    
    foodConsumptionRateTitleLabel->CCNode::setPosition(60.0f, 10.0f);
    foodConsumptionRateLabel->CCNode::setPosition(160.0f, 10.0f);
    
    foodStorageTitleLabel->CCNode::setPosition(60.0f, -20.0f);
    foodStorageBar->CCNode::setPosition(60.0f, -50.0f);
    foodStorageLabel->CCNode::setPosition(160.0f, -50.0f);
    
    workCompleteTitleLabel->CCNode::setPosition(60.0f, -80.0f);
    workCompleteBar->CCNode::setPosition(60.0f, -110.0f);
    workCompleteLabel->CCNode::setPosition(160.0f, -110.0f);
    
    // Anchored top
    textName->CCNode::setPosition(0, halfHeight - 40.0f);
    
    // Anchored top right
    buttonClose->setPosition(halfWidth - 20.0f, halfHeight - 20.0f);
    
    // Anchored bottom right
    textPrice->CCNode::setPosition(halfWidth, -halfHeight + 40.0f);
    spPrice->CCNode::setPosition(halfWidth - 250.0f, -halfHeight + 40.0f);
    
    // Anchored top left
    spritePopulationLabel->setPosition(ccp(-halfWidth * 3.0f / 4.0f, -35.0));
    
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
    
    selectWorkerButton->setPosition(ccp(-halfWidth + 25.0f, -halfHeight + 85.0f));
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
    
    if (mBuildingLevel != building->currentLevel)
    {
        mBuildingLevel = building->currentLevel;
        ss.str(std::string());
        ss << "Level: " << mBuildingLevel;
        labelLevel->setString(ss.str().c_str());
    }
    
    /*
    if (mBuildingVacancy != building->populationLimit)
    {
        int diff = building->populationLimit - mBuildingVacancy;
        mBuildingVacancy = building->populationLimit;
        
        if (diff > 0)
        {
            for (int i = 0; i < diff; i++)
            {
                spritePopulationSlot.push_back(CCSprite::create("vacancy slot.png"));
                spritePopulationSlot.back()->CCNode::setScale(55.0f / spritePopulationSlot.back()->boundingBox().size.width);
                spritePopulationSlot.back()->setAnchorPoint(CCPointZero);
                this->addChild(spritePopulationSlot.back());
            }
            isPositionDirty = true;
        }
        else
        {
            for (int i = 0; i > diff; i--)
            {
                this->removeChild(spritePopulationSlot.back());
                spritePopulationSlot.pop_back();
            }
        }
    }
    */
    
    // check whether the building has been completely build or not
    if(mBuildingUnderConstruction != building->isUnderConstruction())
    {
        mBuildingUnderConstruction = building->isUnderConstruction();
        if(building->buildingType != AMENITY && building->buildingType != MILITARY)
        {
            selectWorkerButton->setVisible(false);
        }
        
        mBuildingVacancy = building->populationLimit;
        
        for(int i = 0; i < spritePopulationSlot->count(); i++)
        {
            this->removeChild((CCNode*) spritePopulationSlot->objectAtIndex(i));
        }
        spritePopulationSlot->removeAllObjects();
        
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
        
        this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
        
        float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
   //     float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
        
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
        
        this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
        
        float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
 //       float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
        
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
        ss << "LEVEL: " << mGameLevel;
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
    
    this->closeMenu(true);
    
    SpriteInfoMenu* spriteInfoMenu = new SpriteInfoMenu(gameSprite);
    spriteInfoMenu->autorelease();
    spriteInfoMenu->useAsBasePopupMenu();
}

void BuildingInfoMenu::selectPop()
{
    this->closeMenu();
    SelectPopulation* selectPopulationMenu = SelectPopulation::create(building);
    selectPopulationMenu->useAsBasePopupMenu();
}
