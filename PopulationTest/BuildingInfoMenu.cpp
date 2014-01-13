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
    
    mBuildingVacancy = mBuildingOverload = 0;
    mBuildingExp = mBuildingExpMax = mBuildingPrice = 0;
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
    mBuildingExp = building->currentExp;
    mBuildingExpMax = building->getExpToLevel();
    mBuildingLevel = building->currentLevel;
    mBuildingPrice = building->buildingCost * GameScene::getThis()->policyHandler->percentTax * 0.01;
   // if (building->buildingType == HOUSING)
        mBuildingVacancy = building->populationLimit;
   // else
     //   mBuildingVacancy = building->getJobsAvailable()->count();
 //   mBuildingOverload = building->populationOverload;
    mBuildingCurrPopulation = building->getPopulationCount();
    
    // Create header
    textName = CCLabelTTF::create(GlobalHelper::stringToUpper(building->buildingName).c_str(), "Droidiga", 32);
    textName->setColor(colorYellow);
    
    spInt = CCSprite::create("intelligence icon.png");
    spInt->setScale(0.75);
    spLoy = CCSprite::create("loyalty icon.png");
    spLoy->setScale(0.75);
    spSoc = CCSprite::create("social icon.png");
    spSoc->setScale(0.75);
    spHap = CCSprite::create("happiness icon.png");
    spHap->setScale(0.75);
    spPrice = CCSprite::create("money icon.png");
    spPrice->setScale(0.75);
    spCash = CCSprite::create("money icon.png");
    //spCash = CCSprite::create("happiness icon.png");
    spCash->setScale(0.75);
    
    /*
    textInt = CCLabelTTF::create(GlobalHelper::Convert(building->int_mod).c_str(), "Droidiga", 26);
    textInt->setColor(colorGreen);
    textLoy = CCLabelTTF::create(GlobalHelper::Convert(building->loyalty_mod).c_str(), "Droidiga", 26);
    textLoy->setColor(colorGreen);
    textSoc = CCLabelTTF::create(GlobalHelper::Convert(building->social_mod).c_str(), "Droidiga", 26);
    textSoc->setColor(colorGreen);
    textHap = CCLabelTTF::create(GlobalHelper::Convert(building->happiness_mod).c_str(), "Droidiga", 26);
    textHap->setColor(colorGreen);
    
    textCashMod = CCLabelTTF::create(GlobalHelper::Convert(building->cash_mod).c_str(), "Droidiga", 26);
    textCashMod->setColor(colorGreen);
    */
    std::stringstream ss;
    ss << "Yearly: " << mBuildingPrice << " G";
    textPrice = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    textPrice->setColor(colorYellow);
    
    // Sprite
    spriteBuilding = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    spriteBuilding->setScale(128.0f / spriteBuilding->boundingBox().size.width);
    spriteBuilding->setAnchorPoint(ccp(0.5, 0.5));
    
    //spriteGy = CCSprite::create("blue square.png", CCRectMake(0, 0, 610, 600));
    //spriteGy->setScale(610 / spriteGy->boundingBox().size.width);
    
    // Attribute labels
    ss.str(std::string());
    ss << "Level: " << mBuildingLevel;
    labelLevel = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    labelLevel->setColor(colorGreen);
    // Exp
    labelExp = CCLabelTTF::create("NEXT LEVEL", "Droidiga", 20);
    labelExp->setColor(colorBlack);
    ss.str(std::string());
    ss << mBuildingExp << "/" << mBuildingExpMax;
    textExp = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 20);
    textExp->setColor(colorBlack);
    
    barExp = new ProgressBar();
    barExp->createProgressBar(CCRectMake(0, 0, 80, 20),
                                CCRectMake(5, 5, 70, 10),
                              "loadingbar-empty.png",
                              "loadingbar-left.png",
                              "loadingbar-right.png",
                              "loadingbar-full.png");
    barExp->setValue(mBuildingExp / (float)mBuildingExpMax);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCMenuItemImage::create("closebutton-up.png", "closebutton-down.png", this, menu_selector(BuildingInfoMenu::onMenuItemSelected));
    buttonClose->setScale(0.4);
    buttonClose->setTag(-1);
    
    menuItems->addObject(buttonClose);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Scroll Area
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(610, 300), CCSizeMake(610, 300));
    scrollArea->enableScrollVertical(10, "bar.png", "bar.png");
    scrollArea->hideScroll();
    
    scrollArea->addItem(spriteBuilding, ccp(105, 45));
    scrollArea->addItem(spInt, ccp(405, 45));
    scrollArea->addItem(spLoy, ccp(405, 95));
    scrollArea->addItem(spSoc, ccp(405, 145));
    scrollArea->addItem(spHap, ccp(405, 195));
    //scrollArea->addItem(textInt, ccp(505, 60));
    //scrollArea->addItem(textLoy, ccp(505, 110));
    //scrollArea->addItem(textSoc, ccp(505, 160));
    //scrollArea->addItem(textHap, ccp(505, 210));

    if (building->buildingType == COMMERCE)
    {
        scrollArea->addItem(spCash, ccp(405, 245));
        scrollArea->addItem(textCashMod , ccp(505, 260));
    }
    scrollArea->addItem(labelLevel, ccp(50, 195));
    scrollArea->addItem(labelExp, ccp(35, 245));
    scrollArea->addItem(barExp, ccp(170, 245));
    scrollArea->addItem(textExp, ccp(260, 245));
    scrollArea->updateScrollBars();
    
    // Add children
    this->addChild(spriteBackground);
    this->addChild(spriteBackgroundInner);
    this->addChild(spPrice);
    this->addChild(textName);
    this->addChild(textPrice);
    this->addChild(menu, 3);
    this->addChild(scrollArea);
    //this->addChild(spriteGy);
    
    // Done creation, now position them
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    textName->setAnchorPoint(ccp(0.5, 1));
    spPrice->setAnchorPoint(ccp(1, 0));
    textPrice->setAnchorPoint(ccp(1, 0));
    scrollArea->setAnchorPoint(ccp(0.5, 0.5));
    buttonClose->setAnchorPoint(ccp(1, 1));
    //spriteGy->setAnchorPoint(ccp(0.5, 0.5));
    
    // Create population icons
    for (int i = 0; i < mBuildingVacancy; i++)
    {
        spritePopulationSlot.push_back(CCSprite::create("vacancy slot.png"));
        spritePopulationSlot.back()->CCNode::setScale(55.0f / spritePopulationSlot[i]->boundingBox().size.width);
        spritePopulationSlot.back()->setAnchorPoint(CCPointZero);
        this->addChild(spritePopulationSlot.back());
    }
    for (int i = 0; i < mBuildingOverload; i++)
    {
        spritePopulationOverloadSlot.push_back(CCSprite::create("vacancy slot overload.png"));
        spritePopulationOverloadSlot.back()->CCNode::setScale(55.0f / spritePopulationOverloadSlot[i]->boundingBox().size.width);
        spritePopulationOverloadSlot.back()->setAnchorPoint(CCPointZero);
        this->addChild(spritePopulationOverloadSlot.back());
    }
    
    
    for (int i = 0; i < mBuildingCurrPopulation; i++)
    {
        std::string spriteName = building->getPopulationAt(i)->spriteName + "_port.png";
        
        CCMenuItemImage* menuItem = CCMenuItemImage::create(spriteName.c_str(), NULL,
                                                            this, menu_selector(BuildingInfoMenu::onMenuItemSelected));
        menuItem->setTag((int)building->getPopulationAt(i));  // Store GameSprite* address in tag
        spritePopulation.push_back(menuItem);
        
        portraitScale = 40.0f / spritePopulation[i]->boundingBox().size.width;
        
        spritePopulation.back()->CCNode::setScale(portraitScale);
        spritePopulation.back()->setAnchorPoint(ccp(-0.20, -0.25));
        
        menuItems->addObject(menuItem);
        menu->addChild(menuItem);
    }
    
    reposition();
    this->schedule(schedule_selector(BuildingInfoMenu::update), 0.25f);
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
        }
        break;
            
        default:
        {
            SpriteInfoMenu* spriteInfoMenu = new SpriteInfoMenu((GameSprite*)pMenuItem->getTag());
            spriteInfoMenu->useAsTopmostPopupMenu();
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
    
    // Anchored top
    textName->CCNode::setPosition(0, halfHeight - 40.0f);
    
    // Anchored top right
    buttonClose->setPosition(halfWidth - 20.0f, halfHeight - 20.0f);
    
    // Anchored bottom right
    textPrice->CCNode::setPosition(halfWidth - 55.0f, -halfHeight + 40.0f);
    spPrice->CCNode::setPosition(halfWidth - 250.0f, -halfHeight + 40.0f);
    
    // Anchored bottom left
    for (int i = 0; i < mBuildingVacancy; i++)
        spritePopulationSlot[i]->CCNode::setPosition(-halfWidth + 28.0f + (60.0f * i), -halfHeight + 28.0f);
    for (int i = 0; i < mBuildingOverload; i++)
        spritePopulationOverloadSlot[i]->CCNode::setPosition(-halfWidth + 28.0f + (60.0f * (i+mBuildingVacancy)), -halfHeight + 28.0f);
    for (int i = 0; i < mBuildingCurrPopulation; i++)
        spritePopulation[i]->CCNode::setPosition(-halfWidth + 28.0f + (60.0f * i), -halfHeight + 28.0f);
    
    // Scroll area in center
    scrollArea->CCNode::setPosition(spriteBackground->getPositionX() - 305,
                                    spriteBackground->getPositionY() - 150);
    scrollArea->reposition();
    
    //spriteGy->CCNode::setPosition(spriteBackground->getPositionX(), spriteBackground->getPositionY());
}

void BuildingInfoMenu::refreshAllMenuItemValues()
{
    bool isPositionDirty = false;
    
    std::stringstream ss;
    
    if (mBuildingExp != building->currentExp)
    {
        mBuildingExp = building->currentExp;
        barExp->setValue(mBuildingExp / (float)mBuildingExpMax);
        ss.str(std::string());
        ss << mBuildingExp << "/" << mBuildingExpMax;
        textExp->setString(ss.str().c_str());
    }
    
    if (mBuildingExpMax != building->getExpToLevel())
    {
        mBuildingExpMax = building->getExpToLevel();
        barExp->setValue(mBuildingExp / (float)mBuildingExpMax);
        ss.str(std::string());
        ss << mBuildingExp << "/" << mBuildingExpMax;
        textExp->setString(ss.str().c_str());
    }
    
    if (mBuildingPrice != building->buildingCost * GameScene::getThis()->policyHandler->percentTax * 0.01)
    {
        mBuildingPrice = building->buildingCost * GameScene::getThis()->policyHandler->percentTax * 0.01;
        ss.str(std::string());
        ss << "Yearly: " << mBuildingPrice << " G";
        textPrice->setString(ss.str().c_str());
    }
    
    if (mBuildingLevel != building->currentLevel)
    {
        mBuildingLevel = building->currentLevel;
        ss.str(std::string());
        ss << "Level: " << mBuildingLevel;
        labelLevel->setString(ss.str().c_str());
    }
    
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
    /*
    if (mBuildingOverload != building->populationOverload)
    {
        int diff = building->populationOverload - mBuildingOverload;
        mBuildingOverload = building->populationOverload;
        
        if (diff > 0)
        {
            for (int i = 0; i < diff; i++)
            {
                spritePopulationOverloadSlot.push_back(CCSprite::create("vacancy slot.png"));
                spritePopulationOverloadSlot.back()->CCNode::setScale(55.0f / spritePopulationOverloadSlot.back()->boundingBox().size.width);
                spritePopulationOverloadSlot.back()->setAnchorPoint(CCPointZero);
                this->addChild(spritePopulationOverloadSlot.back());
            }
            isPositionDirty = true;
        }
        else
        {
            for (int i = 0; i > diff; i--)
            {
                this->removeChild(spritePopulationOverloadSlot.back());
                spritePopulationOverloadSlot.pop_back();
            }
        }
    }*/
    
    if (mBuildingCurrPopulation != building->getPopulationCount())
    {
        int diff = building->getPopulationCount() - mBuildingCurrPopulation;
        mBuildingCurrPopulation = building->getPopulationCount();
        
        if (diff > 0)
        {
            for (int i = 0; i < diff; i++)
            {
                std::string spriteName = building->getPopulationAt(i)->spriteName + "_port.png";
                
                CCMenuItemImage* menuItem = CCMenuItemImage::create(spriteName.c_str(), NULL,
                                                                    this, menu_selector(BuildingInfoMenu::onMenuItemSelected));
                menuItem->setTag((int)building->getPopulationAt(i));  // Store GameSprite* address in tag
                spritePopulation.push_back(menuItem);
                spritePopulation.back()->CCNode::setScale(portraitScale);
                spritePopulation.back()->setAnchorPoint(ccp(-0.20, -0.25));
                
                menuItems->addObject(menuItem);
                menu->addChild(menuItem);
            }
            isPositionDirty = true;
        }
        else
        {
            for (int i = 0; i > diff; i--)
            {
                this->removeChild(spritePopulation.back());
                spritePopulation.pop_back();
            }
        }
    }
    
    /*
    textInt->setString(GlobalHelper::Convert(building->int_mod).c_str());
    textLoy->setString(GlobalHelper::Convert(building->loyalty_mod).c_str());
    textSoc->setString(GlobalHelper::Convert(building->social_mod).c_str());
    textHap->setString(GlobalHelper::Convert(building->happiness_mod).c_str());
    */
    
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

