//
//  SpriteInfoMenu.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 6/5/13.
//
//

#include "SpriteInfoMenu.h"
#include "GameHUD.h"
#include "GlobalHelper.h"
#include "BuildingInfoMenu.h"

SpriteInfoMenu* SpriteInfoMenu::SP;

SpriteInfoMenu* SpriteInfoMenu::create(GameSprite* gs)
{
    SpriteInfoMenu *pRet = new SpriteInfoMenu(gs);
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

SpriteInfoMenu::SpriteInfoMenu(GameSprite* gameSprite)
{
    if (!gameSprite) CCLog("Warning NO SPRITE!!!");
    this->gameSprite = gameSprite;
    mGameSpriteEnergy = 0;
    mGameSpriteEnergyMax = 0;
    mGameSpriteCash = 0;
    mGameSpriteLoy = 0;
    mGameSpriteHap = 0;
    mGameSpriteCurrAction = IDLE;
    mGameSpriteFoodCarriage = 0;
    mGameSpriteFoodCarriageLimit = 0;
    mGameSpriteHungry = 0;
    mGameSpriteHungryMax = 0;
    mGameSpriteWorkRate = 0;
    mGameSpriteWorkUnitPerDay = 0.0f;
    
    SpriteInfoMenu::SP = this;
}

SpriteInfoMenu::~SpriteInfoMenu()
{
    this->unschedule(schedule_selector(SpriteInfoMenu::update));
    SpriteInfoMenu::SP = NULL;
}

SpriteInfoMenu* SpriteInfoMenu::getThis()
{
    return SP;
}

void SpriteInfoMenu::createMenuItems()
{
    ccColor3B colorYellow = ccc3(225, 219, 108);
    
    // Create constant menu items
    spriteBackground = CCSprite::create("BuildingInfoUI.png");
    spriteBackground->setScale(700 / spriteBackground->boundingBox().size.width);
    
    // Set variables which may become dirty
    mGameSpriteEnergy = gameSprite->getPossessions()->energyRating;
    mGameSpriteEnergyMax = gameSprite->getPossessions()->default_energy_limit;
    mGameSpriteLoy = gameSprite->getPossessions()->loyaltyRating;
    mGameSpriteHap = gameSprite->getPossessions()->happinessRating;
    mGameSpriteCurrAction = gameSprite->currAction;
    mGameSpriteFoodCarriage = gameSprite->getFoodCarried();
    mGameSpriteFoodCarriageLimit = gameSprite->getPossessions()->default_food_carriage_limit;
    mGameSpriteHungry = gameSprite->getPossessions()->currentHungry;
    mGameSpriteHungryMax = gameSprite->getPossessions()->default_hungry_limit;
    mGameSpriteWorkRate = gameSprite->getPossessions()->default_work_rate;
    mGameSpriteWorkUnitPerDay = gameSprite->getPossessions()->default_work_unit_per_day;
    
    // Create header
    textName = CCLabelTTF::create(GlobalHelper::stringToUpper(gameSprite->spriteDisplayedName).c_str(), "Shojumaru-Regular", 32, CCSizeMake(gameSprite->spriteDisplayedName.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    textName->setColor(colorYellow);
    
    // Cash
    spCash = CCSprite::createWithSpriteFrameName("yuanbao.png");
    spCash->setScale(0.75);
    
  //  std::string tempStr = "CASH";
   // labelCash = CCLabelTTF::create(tempStr.c_str(), "Shojumaru-Regular", 20, CCSizeMake(tempStr.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    //labelCash->setColor(colorYellow);
    
    std::stringstream ss;
    ss << mGameSpriteCash << "Y";
    textCash = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    textCash->setColor(colorYellow);
    
    // Sprite
    string spriteFrameName = gameSprite->spriteName.c_str();
    spriteFrameName += "_IDL001.png";
    gameSpriteImage = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
    gameSpriteImage->setScale(128.0f / gameSpriteImage->boundingBox().size.width);
    
    // Attribute labels
    spLoy = CCSprite::create("loyalty icon.png");
    spLoy->setScale(0.75);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->loyaltyRating << "/" << gameSprite->getPossessions()->default_loyalty_limit;
    loyaltyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    loyaltyLabel->setColor(colorYellow);

    spHap = CCSprite::create("happiness icon.png");
    spHap->setScale(0.75);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->happinessRating << "/" << gameSprite->getPossessions()->default_hapiness_limit;
    happinessLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    happinessLabel->setColor(colorYellow);
    
    // energty stats
    ss.str(std::string());
    ss << gameSprite->getPossessions()->energyRating << "/" << gameSprite->getPossessions()->default_energy_limit;
    energyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    energyLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << "Energy:";
    energyTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 28, CCSizeMake(ss.str().length() * 30.0f, 5.0f), kCCTextAlignmentLeft);
    energyTitleLabel->setColor(colorYellow);
    
    
    // Energy Bar
    barEnergy = new ProgressBar();
    barEnergy->createProgressBar(CCRectMake(0, 0, 126, 16),
                                 CCRectMake(3, 3, 120, 10),
                                 "Energy_brown bar.png",
                                 "NONE",
                                 "NONE",
                                 "Energybarblue.png",
                                 true);
    barEnergy->setValue(mGameSpriteEnergy / (float)mGameSpriteEnergyMax);
    
    ss.str(std::string());
    ss << "Class:";
    classTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 28, CCSizeMake(ss.str().length() * 30.0f, 5.0f), kCCTextAlignmentLeft);
    classTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->spriteClass;
    classLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 28, CCSizeMake(ss.str().length() * 30.0f, 5.0f), kCCTextAlignmentLeft);
    classLabel->setColor(colorYellow);
    
    // stats
    ss.str(std::string());
    ss << "Gender:";
    genderTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    genderTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->gender;
    genderLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    genderLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << "Race:";
    raceTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    raceTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->race;
    raceLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    raceLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << "MS:";
    movementSpeedTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    movementSpeedTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->default_movement_speed;
    movementSpeedLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    movementSpeedLabel->setColor(colorYellow);
    
    // hungry
    ss.str(std::string());
    ss << "Hungry:";
    hungryTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    hungryTitleLabel->setColor(colorYellow);
    
    barHungry = new ProgressBar();
    barHungry->createProgressBar(CCRectMake(0, 0, 76, 16),
                                 CCRectMake(3, 3, 70, 10),
                                 "Energy_brown bar.png",
                                 "NONE",
                                 "NONE",
                                 "Energybarblue.png",
                                 true);
    barHungry->setValue((float)gameSprite->getPossessions()->currentHungry / (float)gameSprite->getPossessions()->default_hungry_limit);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->currentHungry << "/" << gameSprite->getPossessions()->default_hungry_limit;
    hungryLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    hungryLabel->setColor(colorYellow);
    
    // food carriage
    ss.str(std::string());
    ss << "Food Carriage:";
    foodCarriageTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodCarriageTitleLabel->setColor(colorYellow);
    
    barFoodCarriage = new ProgressBar();
    barFoodCarriage->createProgressBar(CCRectMake(0, 0, 76, 16),
                                 CCRectMake(3, 3, 70, 10),
                                 "Energy_brown bar.png",
                                 "NONE",
                                 "NONE",
                                 "Energybarblue.png",
                                true);
    barFoodCarriage->setValue((float)gameSprite->getFoodCarried() / (float)gameSprite->getPossessions()->default_food_carriage_limit);
    
    ss.str(std::string());
    ss << gameSprite->getFoodCarried() << "/" << gameSprite->getPossessions()->default_food_carriage_limit;
    foodCarriageLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodCarriageLabel->setColor(colorYellow);
    
    // Work Rate
    ss.str(std::string());
    ss << "Work Rate:";
    workRateTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workRateTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->default_work_rate;
    workRateLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workRateLabel->setColor(colorYellow);
    
    // Work Unit Per Day
    ss.str(std::string());
    ss << "WUPD:";
    workUnitPerDayTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workUnitPerDayTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->default_work_unit_per_day;
    workUnitPerDayLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    workUnitPerDayLabel->setColor(colorYellow);
    
    // Movement Range
    ss.str(std::string());
    ss << "MR:";
    movementRangeTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    movementRangeTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->default_movement_range;
    movementRangeLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    movementRangeLabel->setColor(colorYellow);
    
    // Animate Speed
    ss.str(std::string());
    ss << "AS:";
    animateSpeedTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    animateSpeedTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << gameSprite->getPossessions()->default_animate_speed;
    animateSpeedLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    animateSpeedLabel->setColor(colorYellow);
    
    // Current Action
    ss.str(std::string());
    ss << "Current Action:";
    currentActionTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    currentActionTitleLabel->setColor(colorYellow);
    
    ss.str(std::string());
    ss << GlobalHelper::getActionString(gameSprite->currAction);
    currentActionLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    currentActionLabel->setColor(colorYellow);
    
    // Target Location
    ss.str(std::string());
    ss << "Target:";
    targetLocationTitleLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    targetLocationTitleLabel->setColor(colorYellow);
    
    // TODO:
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCSprite::create("Closebtn_Sq.png");
    this->addChild(buttonClose, 2);

    buttonHome = CCMenuItemImage::create("homebutton.png", "homebutton_pressed.png", this, menu_selector(SpriteInfoMenu::onMenuItemSelected));
    buttonHome->setScale(0.75);
    buttonHome->setTag(1);
    buttonHome->setVisible(false);

    buttonWorkPlace = CCMenuItemImage::create("jobbutton.png", "jobbutton_pressed.png", this, menu_selector(SpriteInfoMenu::onMenuItemSelected));
    buttonWorkPlace->setScale(0.75);
    buttonWorkPlace->setTag(2);
    buttonWorkPlace->setVisible(false);
    
    menuItems->addObject(buttonWorkPlace);
    menuItems->addObject(buttonHome);
    
    CCMenu* menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Add children
    this->addChild(spriteBackground);
    this->addChild(spCash);
    this->addChild(textName);
    this->addChild(textCash);
    this->addChild(menu);
    
    this->addChild(barEnergy);
    this->addChild(energyLabel);
    this->addChild(energyTitleLabel);
    
    this->addChild(classLabel);
    this->addChild(classTitleLabel);
    
    this->addChild(gameSpriteImage);
    
    this->addChild(genderLabel);
    this->addChild(genderTitleLabel);
    this->addChild(raceLabel);
    this->addChild(raceTitleLabel);
    this->addChild(movementSpeedLabel);
    this->addChild(movementSpeedTitleLabel);
    
    this->addChild(spLoy);
    this->addChild(loyaltyLabel);
    this->addChild(spHap);
    this->addChild(happinessLabel);
    
    this->addChild(hungryTitleLabel);
    this->addChild(barHungry);
    this->addChild(hungryLabel);
    
    this->addChild(foodCarriageTitleLabel);
    this->addChild(barFoodCarriage);
    this->addChild(foodCarriageLabel);
    
    this->addChild(workRateTitleLabel);
    this->addChild(workRateLabel);
    this->addChild(workUnitPerDayTitleLabel);
    this->addChild(workUnitPerDayLabel);
    
    this->addChild(movementRangeTitleLabel);
    this->addChild(movementRangeLabel);
    
    this->addChild(animateSpeedTitleLabel);
    this->addChild(animateSpeedLabel);
    
    this->addChild(currentActionTitleLabel);
    this->addChild(currentActionLabel);
    
    this->addChild(targetLocationTitleLabel);
    
    // Done creation, now position them
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    textName->setAnchorPoint(ccp(0.5, 1));
    spCash->setAnchorPoint(ccp(1, 0));
   // labelCash->setAnchorPoint(ccp(1, 0));
    textCash->setAnchorPoint(ccp(1, 0));
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    buttonHome->setAnchorPoint(ccp(1, 1));
    
    buttonWorkPlace->setAnchorPoint(ccp(1, 1));
    
    barEnergy->setAnchorPoint(ccp(0, 1));
    energyLabel->setAnchorPoint(ccp(0, 1));
    energyTitleLabel->setAnchorPoint(ccp(0, 1));
    
    classLabel->setAnchorPoint(ccp(0, 1));
    classTitleLabel->setAnchorPoint(ccp(0, 1));
    
    gameSpriteImage->setAnchorPoint(ccp(0, 1));
    
    genderLabel->setAnchorPoint(ccp(0, 1));
    genderTitleLabel->setAnchorPoint(ccp(0, 1));
    raceLabel->setAnchorPoint(ccp(0, 1));
    raceTitleLabel->setAnchorPoint(ccp(0, 1));
    movementSpeedLabel->setAnchorPoint(ccp(0, 1));
    movementSpeedTitleLabel->setAnchorPoint(ccp(0, 1));
    
    spLoy->setAnchorPoint(ccp(0, 1));
    loyaltyLabel->setAnchorPoint(ccp(0, 1));
    spHap->setAnchorPoint(ccp(0, 1));
    happinessLabel->setAnchorPoint(ccp(0, 1));
    
    hungryTitleLabel->setAnchorPoint(ccp(0, 1));
    barHungry->setAnchorPoint(ccp(0, 1));
    hungryLabel->setAnchorPoint(ccp(0, 1));
    
    foodCarriageTitleLabel->setAnchorPoint(ccp(0, 1));
    barFoodCarriage->setAnchorPoint(ccp(0, 1));
    foodCarriageLabel->setAnchorPoint(ccp(0, 1));
    
    workRateTitleLabel->setAnchorPoint(ccp(0, 1));
    workRateLabel->setAnchorPoint(ccp(0, 1));
    workUnitPerDayTitleLabel->setAnchorPoint(ccp(0, 1));
    workUnitPerDayLabel->setAnchorPoint(ccp(0, 1));
    
    movementRangeTitleLabel->setAnchorPoint(ccp(0, 1));
    movementRangeLabel->setAnchorPoint(ccp(0, 1));
    
    animateSpeedTitleLabel->setAnchorPoint(ccp(0, 1));
    animateSpeedLabel->setAnchorPoint(ccp(0, 1));
    
    currentActionTitleLabel->setAnchorPoint(ccp(0, 1));
    currentActionLabel->setAnchorPoint(ccp(0, 1));
    
    targetLocationTitleLabel->setAnchorPoint(ccp(0, 1));
    
    reposition();
    
    this->schedule(schedule_selector(SpriteInfoMenu::update), 0.25f);
}

void SpriteInfoMenu::onMenuItemSelected(CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case 0:
        {
            // buttonClose
            //this->closeMenu(false);
        }
            break;
        case 1:
        {
            //home location
            if (gameSprite != NULL)
            {
                if (gameSprite->getPossessions()->homeLocation != NULL)
                {
                    GameScene::getThis()->tapped = true;
                    PopupMenu::backupCurrentPopupMenu();
                    BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(gameSprite->getPossessions()->homeLocation);//new BuildingInfoMenu(selectedTile->building);
                    buildingInfoMenu->useAsTopmostPopupMenu();
                }
            }
        }
            break;
        case 2:
        {
            //job location
            if (gameSprite != NULL)
            {
                if (gameSprite->getPossessions()->jobLocation != NULL)
                {
                    GameScene::getThis()->tapped = true;
                    PopupMenu::backupCurrentPopupMenu();
                    BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(gameSprite->getPossessions()->jobLocation);//new BuildingInfoMenu(selectedTile->building);
                    buildingInfoMenu->useAsTopmostPopupMenu();
                }
            }
        }
            break;
            
        default:
        {
        }
            break;
    }
}

void SpriteInfoMenu::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
    spriteBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    // Anchored top left
    barEnergy->CCNode::setPosition(-140.0f + hw, 140.0f + hh);
    energyLabel->CCNode::setPosition(-140.0f + hw, 160.0f + hh);
    energyTitleLabel->CCNode::setPosition(-280.0f + hw, 156.0f + hh);
    
    classTitleLabel->CCNode::setPosition(20.0f + hw, 156.0f + hh);
    classLabel->CCNode::setPosition(160.0f + hw, 156.0f + hh);
    
    gameSpriteImage->CCNode::setPosition(-300.0f + hw, 125.0f + hh);
    
    foodCarriageTitleLabel->CCNode::setPosition(40.0f + hw, 90.0f + hh);
    barFoodCarriage->CCNode::setPosition(40.0f + hw, 60.0f + hh);
    foodCarriageLabel->CCNode::setPosition(180.0f + hw, 60.0f + hh);
    
    genderTitleLabel->CCNode::setPosition(-160.0f + hw, 100.0f + hh);
    genderLabel->CCNode::setPosition(-60.0f + hw, 100.0f + hh);
    raceTitleLabel->CCNode::setPosition(-160.0f + hw, 70.0f + hh);
    raceLabel->CCNode::setPosition(-60.0f + hw, 70.0f + hh);
    movementSpeedTitleLabel->CCNode::setPosition(-160.0f + hw, 40.0f + hh);
    movementSpeedLabel->CCNode::setPosition(-60.0f + hw, 40.0f + hh);
    
    spLoy->CCNode::setPosition(-280.0f + hw, 20.0f + hh);
    loyaltyLabel->CCNode::setPosition(-220.0f + hw, 5.0f + hh);
    spHap->CCNode::setPosition(-140.0f + hw, 20.0f + hh);
    happinessLabel->CCNode::setPosition(-80.0f + hw, 5.0f + hh);
    
    movementRangeTitleLabel->CCNode::setPosition(-280.0f + hw, -35.0f + hh);
    movementRangeLabel->CCNode::setPosition(-200.0f + hw, -35.0f + hh);
    animateSpeedTitleLabel->CCNode::setPosition(-120.0f + hw, -35.0f + hh);
    animateSpeedLabel->CCNode::setPosition(-40.0f + hw, -35.0f + hh);
    
    currentActionTitleLabel->CCNode::setPosition(-280.0f + hw, -65.0f + hh);
    currentActionLabel->CCNode::setPosition(-100.0f + hw, -65.0f + hh);
    
    targetLocationTitleLabel->CCNode::setPosition(-280.0f + hw, -100.0f + hh);
    
    hungryTitleLabel->CCNode::setPosition(40.0f + hw, 30.0f + hh);
    barHungry->CCNode::setPosition(40.0f + hw, 0.0f + hh);
    hungryLabel->CCNode::setPosition(180.0f + hw, 0.0f + hh);
    
    workRateTitleLabel->CCNode::setPosition(40.0f + hw, -30.0f + hh);
    workRateLabel->CCNode::setPosition(180.0f + hw, -30.0f + hh);
    
    workUnitPerDayTitleLabel->CCNode::setPosition(40.0f + hw, -60.0f + hh);
    workUnitPerDayLabel->CCNode::setPosition(180.0f + hw, -60.0f + hh);
    
    
    // Anchored top
    textName->CCNode::setPosition(0 + hw, halfHeight - 20.0f + hh);
    
    // Anchored top right
    buttonClose->setPosition(ccp(halfWidth - 60.0f + hw, halfHeight - 20.0f + hh));
    buttonHome->setPosition(-halfWidth + 140.0f + hw,  -halfHeight + 80.0f + hh);
    buttonWorkPlace->setPosition(-halfWidth + 80.0f + hw, -halfHeight + 80.0f + hh);
    
    
    // Anchored bottom right
//    labelCash->CCNode::setPosition(halfWidth -380.0f, -halfHeight + 40.0f);
    spCash->CCNode::setPosition(halfWidth - 430.0f + hw, -halfHeight + 40.0f + hh);
    textCash->CCNode::setPosition(halfWidth - 330.0f + hw, -halfHeight + 40.0f + hh);
}

void SpriteInfoMenu::refreshAllMenuItemValues()
{
 
    std::stringstream ss;
    
    if (mGameSpriteEnergy != gameSprite->getPossessions()->energyRating)
    {
        mGameSpriteEnergy = gameSprite->getPossessions()->energyRating;
        barEnergy->setValue(mGameSpriteEnergy / (float)mGameSpriteEnergyMax);
    }
    
    if (mGameSpriteEnergyMax != gameSprite->getPossessions()->default_energy_limit)
    {
        mGameSpriteEnergyMax = gameSprite->getPossessions()->default_energy_limit;
        barEnergy->setValue(mGameSpriteEnergy / (float)mGameSpriteEnergyMax);
    }
    
    if (mGameSpriteHap != gameSprite->getPossessions()->happinessRating)
    {
        mGameSpriteHap = gameSprite->getPossessions()->happinessRating;
        ss.str(std::string());
        ss << mGameSpriteHap;
        happinessLabel->setString(ss.str().c_str());
    }
    
    /*
    if (mGameSpriteLoy != gameSprite->getPossessions()->loyaltyRating)
    {
        mGameSpriteLoy = gameSprite->getPossessions()->loyaltyRating;
        ss.str(std::string());
        ss << mGameSpriteLoy;
        textLoy->setString(ss.str().c_str());
    }
    */
    
    if (mGameSpriteCurrAction != gameSprite->currAction)
    {
        mGameSpriteCurrAction = gameSprite->currAction;
        ss.str(std::string());
        ss << GlobalHelper::getActionString(gameSprite->currAction);
        currentActionLabel->setString(ss.str().c_str());
    }
    
    if (mGameSpriteFoodCarriage != gameSprite->getFoodCarried() || mGameSpriteFoodCarriageLimit != gameSprite->getPossessions()->default_food_carriage_limit)
    {
        mGameSpriteFoodCarriage = gameSprite->getFoodCarried();
        mGameSpriteFoodCarriageLimit = gameSprite->getPossessions()->default_food_carriage_limit;
        ss.str(std::string());
        ss << mGameSpriteFoodCarriage << "/" << mGameSpriteFoodCarriageLimit;
        foodCarriageLabel->setString(ss.str().c_str());
        barFoodCarriage->setValue((float) mGameSpriteFoodCarriage / (float) mGameSpriteFoodCarriageLimit);
    }
    
    if (mGameSpriteHungry != gameSprite->getPossessions()->currentHungry || mGameSpriteHungryMax != gameSprite->getPossessions()->default_hungry_limit);
    {
        mGameSpriteHungry = gameSprite->getPossessions()->currentHungry;
        mGameSpriteHungryMax = gameSprite->getPossessions()->default_hungry_limit;
        ss.str(std::string());
        ss << mGameSpriteHungry << "/" << mGameSpriteHungryMax;
        hungryLabel->setString(ss.str().c_str());
        barHungry->setValue((float) mGameSpriteHungry / (float) mGameSpriteHungryMax);
    }
    
    if (mGameSpriteWorkRate != gameSprite->getPossessions()->default_work_rate)
    {
        mGameSpriteWorkRate = gameSprite->getPossessions()->default_work_rate;
        ss.str(std::string());
        ss << mGameSpriteWorkRate;
        workRateLabel->setString(ss.str().c_str());
    }
    
    if (mGameSpriteWorkUnitPerDay != gameSprite->getPossessions()->default_work_unit_per_day)
    {
        mGameSpriteWorkUnitPerDay = gameSprite->getPossessions()->default_work_unit_per_day;
        ss.str(std::string());
        ss << mGameSpriteWorkUnitPerDay;
        workUnitPerDayLabel->setString(ss.str().c_str());
    }
}

void SpriteInfoMenu::willChangeOrientation()
{
}

void SpriteInfoMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    reposition();
}

void SpriteInfoMenu::update(float deltaTime)
{
    if (gameSprite->isLeavingNextUpdate)
    {
        this->closeMenu(false);
        return;
    }
    refreshAllMenuItemValues();
}
