//
//  BuildingCard.cpp
//  PopulationTest
//
//  Created by admin on 13/2/14.
//
//

#include "BuildingCard.h"
#include "GameHUD.h"
#include "BuildScroll.h"
#include "TutorialManager.h"

BuildingCard::BuildingCard(Building* building, ScrollArea* scrollArea, int index, int type = 0)
{
    this->building = building;
    this->scrollArea = scrollArea;
    this->index = index;
    this->type = type;
    this->active = false;
    init();
}

BuildingCard::~BuildingCard()
{
    menuItemsArray->removeAllObjects();
    CC_SAFE_RELEASE(menuItemsArray);
    infoButtonMenuItemsArray->removeAllObjects();
    CC_SAFE_RELEASE(infoButtonMenuItemsArray);
}

BuildingCard* BuildingCard::create(Building* building, ScrollArea* scrollArea, int index, int type = 0)
{
    BuildingCard *pRet = new BuildingCard(building, scrollArea, index, type);
    if (pRet)
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

void BuildingCard::init()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    stringstream ss;
    
    std::string buildingname;
    if (type == 1)
    {
        buildingname = "Build Path";
    }
    else if (type == 2)
    {
       buildingname = "Destroy Path";
    }
    else if (type == 3)
    {
       buildingname = "Demolish";
    }
    else
    {
       buildingname = building->buildingName;
    }
    
    // display the name of the building
    //std::stringstream ss;
    //ss << building->buildingName;
    buildingNameLabel = CCLabelTTF::create(buildingname.c_str(), "Shojumaru-Regular", 20);
    buildingNameLabel->setColor(colorBlack);
    buildingNameLabel->setAnchorPoint(ccp(0, 1));
    
    // display the building info button
    buildingInfoButton = CCMenuItemImage::create();
    buildingInfoButton->initWithNormalImage("build-menu_info.png", "build-menu_info.png", NULL, this, menu_selector(BuildingCard::pressDownInfo), menu_selector(BuildingCard::showBuildingInfo) );
    buildingInfoButton->setScale(64.0f / buildingInfoButton->boundingBox().size.width);
    buildingInfoButton->setAnchorPoint(ccp(0.5, 0.5));
    
    infoButtonMenuItemsArray = CCArray::create();
    infoButtonMenuItemsArray->retain();
    infoButtonMenuItemsArray->addObject(buildingInfoButton);
    infoButtonMenu = CCMenu::createWithArray(infoButtonMenuItemsArray);
    
    // display the picture of the building
    if (type == 0)
    {
        buildingImage = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
        buildingImage->setScale(192.0f / buildingImage->boundingBox().size.width);
        buildingImage->setAnchorPoint(ccp(0.5, 0.5));
        buildingImage->setContentSize(buildingImage->boundingBox().size);
    }
    else
    {
        //building is likely to be NULL here! use hardcoded stuff. sorry.
        
        if (type == 1)
        {
            buildingImage = CCSprite::createWithSpriteFrameName("path.png");
        }
        else if (type == 2)
        {
            buildingImage = CCSprite::createWithSpriteFrameName("path-destroy.png");
        }
        else if (type == 3)
        {
            buildingImage = CCSprite::createWithSpriteFrameName("demolish.png");
        }
        else
        {
            buildingImage = CCSprite::createWithSpriteFrameName("path.png");
        }
       
        buildingImage->setScale(192.0f / buildingImage->boundingBox().size.width);
        buildingImage->setAnchorPoint(ccp(0.5, 0.5));
        buildingImage->setContentSize(buildingImage->boundingBox().size);
    }
    
    // cost
    costImage = CCSprite::createWithSpriteFrameName("build-menu_goldreq.png");
    costImage->setScale(42.0f / costImage->boundingBox().size.width);
    costImage->setAnchorPoint(ccp(0, 1));
    
    std::string buildingcost;
    if (type == 1)
    {
        buildingcost = "0";
    }
    else if (type == 2)
    {
        buildingcost = "0";
    }
    else if (type == 3)
    {
        buildingcost = "0";
    }
    else
    {
        ss.str(std::string());
        ss << building->buildingCost;
        buildingcost = ss.str();
    }
    costLabel = CCLabelTTF::create(buildingcost.c_str(), "Shojumaru-Regular", 24);
    costLabel->setColor(colorBlack);
    costLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    ss.str(std::string());
    int level = GameManager::getThis()->town_hall_level;
    if(type == 0 && building->buildingType != SOCIAL)
    {
        if(building->buildingType == HOUSING)
        {
            CCArray* allHousing = GameScene::getThis()->buildingHandler->housingOnMap;
            CCArray* allGhostHousing = GameScene::getThis()->buildingHandler->housingGhostOnMap;
            available_number = GameManager::getThis()->housingLimitation->housing_limits.at(level) - allHousing->count() - allGhostHousing->count();
        }
        else if(building->buildingType == AMENITY)
        {
            CCArray* allAmenity = GameScene::getThis()->buildingHandler->amenityOnMap;
            CCArray* allGhostAmenity = GameScene::getThis()->buildingHandler->amenityGhostOnMap;
            available_number = GameManager::getThis()->housingLimitation->farm_limits.at(level) - allAmenity->count() - allGhostAmenity->count();
        }
        else if(building->buildingType == GRANARY)
        {
            CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
            CCArray* allGhostGranary = GameScene::getThis()->buildingHandler->granaryGhostOnMap;
            available_number = GameManager::getThis()->housingLimitation->granary_limits.at(level) - allGranary->count() - allGhostGranary->count();
        }
        else if(building->buildingType == MARKET)
        {
            CCArray* allMarket = GameScene::getThis()->buildingHandler->marketOnMap;
            CCArray* allGhostMarket = GameScene::getThis()->buildingHandler->marketGhostOnMap;
            available_number = GameManager::getThis()->housingLimitation->market_limits.at(level) - allMarket->count() - allGhostMarket->count();
        }
        else if(building->buildingType == MILITARY)
        {
            CCArray* allMimitary = GameScene::getThis()->buildingHandler->militaryOnMap;
            CCArray* allGhostMilitary = GameScene::getThis()->buildingHandler->militaryGhostOnMap;
            available_number = GameManager::getThis()->housingLimitation->guard_tower_limits.at(level) - allMimitary->count() - allGhostMilitary->count();
        }
        
        if (available_number < 0)
        {
            available_number = 0;
        }
        
        ss << available_number;
    }
    else
    {
        available_number = 999999;
        ss << "unlimited";
    }
    
    available_number_label = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24);
    available_number_label->setAnchorPoint(ccp(1, 0));
    available_number_label->setColor(colorBlack);
    available_number_label->setPosition(ccp(340, 130));
    
    // population
    populationImage = CCSprite::createWithSpriteFrameName("population_icon.png");
    populationImage->setScale(42.0f / populationImage->boundingBox().size.width);
    populationImage->setAnchorPoint(ccp(0, 1));
    
    std::string populationLimit;
    if (type != 0)
    {
        populationLimit = "0";
    }
    else
    {
        ss.str(std::string());
        ss << building->builderLimit;
        populationLimit = ss.str();
    }
    populationLabel = CCLabelTTF::create(populationLimit.c_str(), "Shojumaru-Regular", 24);
    populationLabel->setColor(colorBlack);
    populationLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    // building time
    buildingTimeImage = CCSprite::createWithSpriteFrameName("build-menu_time-req.png");
    buildingTimeImage->setScale(42.0f / buildingTimeImage->boundingBox().size.width);
    buildingTimeImage->setAnchorPoint(ccp(0, 1));
    
    std::string buildTime;
    if (type != 0)
    {
        buildTime = "0";
    }
    else
    {
        ss.str(std::string());
        ss << building->build_uint_required;
        buildTime = ss.str();
    }
    buildingTimeLabel = CCLabelTTF::create(buildTime.c_str(), "Shojumaru-Regular", 24);
    buildingTimeLabel->setColor(colorBlack);
    buildingTimeLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    
    cardBG = CCSprite::createWithSpriteFrameName("buildmenu.png");
    if(type == 0)
    {
        cardBG->setTag(building->ID);
    }
    else
    {
        cardBG->setTag(-type); //this should only give -1, -2 and -3.
    }
    cardBG->setAnchorPoint(ccp(0.5, 0.5));
    cardBG->setScaleY(screenSize.height / cardBG->boundingBox().size.height * 0.325f);
    cardBG->setScaleX(screenSize.width / cardBG->boundingBox().size.width * 0.18f);
    
    largeScale = ccp(cardBG->getScaleX(), cardBG->getScaleY());
    smallScale = largeScale * 0.98f;
    
    mask = CCSprite::createWithSpriteFrameName("black.png");
    mask->cocos2d::CCNode::setScale(cardBG->boundingBox().size.width / mask->boundingBox().size.width, cardBG->boundingBox().size.height / mask->boundingBox().size.height * 1.05f);
    mask->setOpacity((GLubyte) 120);
    
    if(TutorialManager::getThis()->teachBuildRoad)
    {
        if(type == 1)
        {
            mask->setOpacity((GLubyte) 0);
        }
    }
    else if(TutorialManager::getThis()->teachBuildHouse)
    {
        if(type != 1 && type != 2 && type != 3 && building->buildingType == HOUSING)
        {
            mask->setOpacity((GLubyte) 0);
        }
    }
    else if(TutorialManager::getThis()->teachBuildGranary)
    {
        if(type != 1 && type != 2 && type != 3 && building->buildingType == GRANARY && building->buildingName.compare("Granary") == 0)
        {
            mask->setOpacity((GLubyte) 0);
        }
    }
    else if(TutorialManager::getThis()->teachBuildFarm)
    {
        if(type != 1 && type != 2 && type != 3 && building->buildingType == AMENITY)
        {
            mask->setOpacity((GLubyte) 0);
        }
    }
    else
    {
        if(type != 0 || available_number > 0)
        {
            mask->setOpacity((GLubyte) 0);
        }
    }
    
    // check whether we have enough money to buy this building
    if (type != 1 && type != 2 && type !=3 && GameHUD::getThis()->targetMoney < building->buildingCost)
    {
        mask->setOpacity((GLubyte) 120);
    }
    
    eventTrigger = CCMenuItemImage::create();
    eventTrigger->initWithNormalImage("black.png", "black.png", NULL, this, menu_selector(BuildingCard::pressBuildingCard), menu_selector(BuildingCard::onMenuItemSelected));
    if(type == 0)
    {
        eventTrigger->setTag(building->ID);
    }
    else
    {
        eventTrigger->setTag(-type);
    }
    eventTrigger->setOpacity((GLubyte) 0);
    
    menuItemsArray = CCArray::create();
    menuItemsArray->retain();
    menuItemsArray->addObject(eventTrigger);
    menu = CCMenu::createWithArray(menuItemsArray);
    menu->setAnchorPoint(ccp(0, 0));
    
    cardBG->addChild(available_number_label);
    
    cardBG->addChild(buildingNameLabel);
    buildingNameLabel->setPosition(ccp(20, cardBG->boundingBox().size.height * 2 - 30));
    
    cardBG->addChild(buildingImage);
    if(type == 0)
    {
        if(building->buildingType == SOCIAL)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width + 40, cardBG->boundingBox().size.height + 70));
        }
        else if(building->buildingType == HOUSING)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width + 40, cardBG->boundingBox().size.height + 70));
        }
        else if(building->buildingType == AMENITY)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width + 40, cardBG->boundingBox().size.height + 85));
        }
        else if(building->buildingType == MARKET)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width + 40, cardBG->boundingBox().size.height + 80));
        }
        else if(building->buildingType == GRANARY)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width - 35, cardBG->boundingBox().size.height + 10));
        }
        else
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width - 20, cardBG->boundingBox().size.height + 0));
        }
    }
    else if(type == 3)
    {
        buildingImage->setPosition(ccp(cardBG->boundingBox().size.width + 40, cardBG->boundingBox().size.height + 60));
    }
    else
    {
        buildingImage->setPosition(ccp(cardBG->boundingBox().size.width + 40, cardBG->boundingBox().size.height + 40));
    }
    
    cardBG->addChild(infoButtonMenu);
    infoButtonMenu->setPosition(ccp(330, 450));
    
    cardBG->addChild(costImage);
    costImage->setScale(costImage->getScale() * 1.5f);
    costImage->setPosition(ccp(40, 120));
    
    cardBG->addChild(costLabel);
    costLabel->setPosition(ccp(70, 45));
    
    cardBG->addChild(populationImage);
    populationImage->setScale(populationImage->getScale() * 1.5f);
    populationImage->setPosition(ccp(160, 120));
    
    cardBG->addChild(populationLabel);
    populationLabel->setPosition(ccp(190, 45));
    
    cardBG->addChild(buildingTimeImage);
    buildingTimeImage->setScale(buildingTimeImage->getScale() * 1.2f);
    buildingTimeImage->setPosition(ccp(280, 120));
    
    cardBG->addChild(buildingTimeLabel);
    buildingTimeLabel->setPosition(ccp(305, 45));
    
    cardBG->addChild(menu);
    eventTrigger->setAnchorPoint(ccp(0, 0));
    eventTrigger->cocos2d::CCNode::setScale(23.0f, 26.0f);
    eventTrigger->setPosition(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    
    cardBG->addChild(mask);
    mask->cocos2d::CCNode::setScale(mask->getScaleX() / cardBG->getScaleX(), mask->getScaleY() / cardBG->getScaleY());
    mask->setAnchorPoint(ccp(0, 0));
    mask->setPosition(ccp(0.0f, 0.0f));
    
    scrollArea->addItem(cardBG, ccp(100.0f + screenSize.width * 0.19f * index, 126), false);
}

void BuildingCard::refreshAllMenuItems()
{
}

void BuildingCard::pressDownInfo()
{
    buildingInfoButton->setScaleX(buildingInfoButton->getScaleX() * 9.0f / 10.0f);
    buildingInfoButton->setScaleY(buildingInfoButton->getScaleY() * 9.0f / 10.0f);
}

void BuildingCard::showBuildingInfo()
{
    buildingInfoButton->setScaleX(buildingInfoButton->getScaleX() * 10.0f / 9.0f);
    buildingInfoButton->setScaleY(buildingInfoButton->getScaleY() * 10.0f / 9.0f);
}

void BuildingCard::pressBuildingCard()
{
    if (!GameHUD::getThis()->isThisTapCounted)
    {
        return;
    }
    cardBG->setScaleX(smallScale.x);
    cardBG->setScaleY(smallScale.y);
}

void BuildingCard::onMenuItemSelected(CCObject* pSender)
{
    cardBG->setScaleX(largeScale.x);
    cardBG->setScaleY(largeScale.y);
    
    if (!GameHUD::getThis()->isThisTapCounted)
    {
        return;
    }
    
    if (GameHUD::getThis()->buildScroll != NULL && (GameHUD::getThis()->buildScroll->scroll_in || GameHUD::getThis()->buildScroll->scroll_out))
    {
        return;
    }
    
    if(TutorialManager::getThis()->active)
    {
        if (TutorialManager::getThis()->lockBuildScroll)
        {
            return;
        }
    }
    
    //cardBG->setScaleX(cardBG->getScaleX() * 1.0f / 0.95f);
    //cardBG->setScaleY(cardBG->getScaleY() * 1.0f / 0.95f);
    
    CCMenuItemImage* pMenuItemImage = (CCMenuItemImage *)(pSender);
    int tag = pMenuItemImage->getTag();
    switch (tag)
    {
        case -1 : //build path
        {
            if(TutorialManager::getThis()->active)
            {
                if(TutorialManager::getThis()->teachBuildHouse || TutorialManager::getThis()->teachBuildGranary || TutorialManager::getThis()->teachBuildFarm)
                {
                    return;
                }
            }
        
            GameHUD::getThis()->setTapMode(3);
            GameHUD::getThis()->isThisTapCounted = false;
            GameHUD::getThis()->buildScroll->scheduleScrollOut();
            GameHUD::getThis()->buildButton->setVisible(true);
            
            if(TutorialManager::getThis()->active)
            {
                if(TutorialManager::getThis()->teachBuildRoad && !TutorialManager::getThis()->miniDragon->notFirst && !TutorialManager::getThis()->miniDragon->connectGranary && !TutorialManager::getThis()->miniDragon->connectFarm)
                {
                    /*
                    //GameHUD::getThis()->buildButton->setTexture(CCTextureCache::sharedTextureCache()->addImage());
                    CCSpriteFrame * l_SpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache() -> spriteFrameByName("main-game-buttons_cancel_build.png");
                    
                    GameHUD::getThis()->buildButton -> setTexture( l_SpriteFrame -> getTexture() );
                    GameHUD::getThis()->buildButton -> setTextureRect( l_SpriteFrame -> getRect( ) );
                    */
                    
                    TutorialManager::getThis()->miniDragon->move(ccp(0, -220));
                    TutorialManager::getThis()->miniDragon->clickNext();
                }
                
                if(TutorialManager::getThis()->miniDragon->connectGranary || TutorialManager::getThis()->miniDragon->connectFarm)
                {
                    TutorialManager::getThis()->miniDragon->move(ccp(0, -220));
                }
            }
        }
            break;
        case -2 : //unbuild path
        {
            if(TutorialManager::getThis()->active)
            {
                if(TutorialManager::getThis()->teachBuildHouse)
                {
                    return;
                }
                
                if(TutorialManager::getThis()->teachBuildRoad)
                {
                    return;
                }
                
                if(TutorialManager::getThis()->teachBuildGranary)
                {
                    return;
                }
                
                if(TutorialManager::getThis()->teachBuildFarm)
                {
                    return;
                }
            }
            
            GameHUD::getThis()->setTapMode(4);
            GameHUD::getThis()->isThisTapCounted = false;
            
            GameHUD::getThis()->buildScroll->scheduleScrollOut();
            GameHUD::getThis()->buildButton->setVisible(true);
            
        }
            break;
        case -3: //destory building
        {
            if(TutorialManager::getThis()->active)
            {
                if(TutorialManager::getThis()->teachBuildHouse)
                {
                    return;
                }
                
                if(TutorialManager::getThis()->teachBuildRoad)
                {
                    return;
                }
                
                if(TutorialManager::getThis()->teachBuildGranary)
                {
                    return;
                }
                
                if(TutorialManager::getThis()->teachBuildFarm)
                {
                    return;
                }
            }
            
            //I'll need to set tap mode to demolish. TODO
            GameHUD::getThis()->setTapMode(2);
            GameHUD::getThis()->isThisTapCounted = false;
            
            GameHUD::getThis()->buildScroll->scheduleScrollOut();
            
            GameHUD::getThis()->buildButton->setVisible(true);
        }
            break;
        default:
        {
            if(TutorialManager::getThis()->active)
            {
                if(TutorialManager::getThis()->teachBuildRoad)
                {
                    return;
                }
            }
            
            if(GameHUD::getThis()->getTapMode() != 0)
            {
                return;
            }
            
            if(TutorialManager::getThis()->active)
            {
                if(TutorialManager::getThis()->teachBuildHouse)
                {
                    TutorialManager::getThis()->miniDragon->move(ccp(0, -220));
                    TutorialManager::getThis()->miniDragon->clickNext();
                }
            }
            
            tryToBuild(tag);
        }
            break;
    }
}

void BuildingCard::tryToBuild(int tag)
{
    Building* buildingToBuy = GameScene::getThis()->buildingHandler->getBuilding(tag);
    if(buildingToBuy == NULL)
    {
        return;
    }
    
    BuildingCategory type = buildingToBuy->buildingType;
    int level = GameManager::getThis()->town_hall_level;
    
    if(type == SOCIAL)
    {
        // at current situation, there is no limitation on the number of social decoration buildings.
    }
    else if(type == HOUSING)
    {
        if(GameScene::getThis()->buildingHandler->housingOnMap->count() + GameScene::getThis()->buildingHandler->housingGhostOnMap->count() >= GameManager::getThis()->housingLimitation->housing_limits.at(level))
        {
            // alert player that the number of buildings has reached the limitation.
            return;
        }
        
        if(TutorialManager::getThis()->active)
        {
            if(TutorialManager::getThis()->teachBuildGranary)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildFarm)
            {
                return;
            }
        }
        //buildingToBuy  = GameScene::getThis()->buildingHandler->getRandomBuildingWithName(buildingToBuy->buildingName);
        
        
    }
    else if(type == GRANARY)
    {
        if(GameScene::getThis()->buildingHandler->granaryOnMap->count() + GameScene::getThis()->buildingHandler->granaryGhostOnMap->count() >= GameManager::getThis()->housingLimitation->granary_limits.at(level))
        {
            return;
        }
        
        if(TutorialManager::getThis()->active)
        {
            if(TutorialManager::getThis()->teachBuildGranary)
            {
                TutorialManager::getThis()->miniDragon->move(ccp(0, -220));
                TutorialManager::getThis()->miniDragon->clickNext();
            }
            
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildFarm)
            {
                return;
            }
        }
    }
    else if(type == AMENITY)
    {
        // buildingToBuy  = GameScene::getThis()->buildingHandler->getRandomBuildingWithName(buildingToBuy->buildingName);
        if(GameScene::getThis()->buildingHandler->amenityOnMap->count() + GameScene::getThis()->buildingHandler->amenityGhostOnMap->count() >= GameManager::getThis()->housingLimitation->farm_limits.at(level))
        {
            return;
        }
        
        if(TutorialManager::getThis()->active)
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildGranary)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildFarm)
            {
                TutorialManager::getThis()->miniDragon->move(ccp(0, -220));
                TutorialManager::getThis()->lockScroll = false;
            }
        }
    }
    else if(type == MARKET)
    {
        if(GameScene::getThis()->buildingHandler->marketOnMap->count() + GameScene::getThis()->buildingHandler->marketGhostOnMap->count() >= GameManager::getThis()->housingLimitation->market_limits.at(level))
        {
            return;
        }
        
        if(TutorialManager::getThis()->active)
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildGranary)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildFarm)
            {
                return;
            }
        }
    }
    else if(type == MILITARY)
    {
        if(GameScene::getThis()->buildingHandler->militaryOnMap->count() + GameScene::getThis()->buildingHandler->militaryGhostOnMap->count() >= GameManager::getThis()->housingLimitation->guard_tower_limits.at(level))
        {
            return;
        }
        
        if(TutorialManager::getThis()->active)
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildGranary)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildFarm)
            {
                return;
            }
        }
    }
    else
    {
        if(TutorialManager::getThis()->active)
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildGranary)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildFarm)
            {
                return;
            }
        }
    }
    
    if(GameHUD::getThis()->money > buildingToBuy->buildingCost)
    {
        GameHUD::getThis()->scheduleAddMoney(-buildingToBuy->buildingCost);
        GameHUD::getThis()->setTapMode(1);
        GameScene::getThis()->buildingHandler->selectedBuilding = buildingToBuy;
        
        GameHUD::getThis()->isThisTapCounted = false;
        
        GameHUD::getThis()->buildScroll->scheduleScrollOut();
    }
    else
    {
        
    }
    
    /*
    BuildScroll::getThis()->closeMenu();
    GameHUD::getThis()->buildButton->setVisible(true);
    GameHUD::getThis()->buildScroll = NULL;
    */
}

void BuildingCard::setOpacity(GLubyte opacity)
{
    cardBG->setOpacity(opacity);
    buildingNameLabel->setOpacity(opacity);
    buildingImage->setOpacity(opacity);
    buildingInfoButton->setOpacity(opacity);
    costImage->setOpacity(opacity);
    costLabel->setOpacity(opacity);
    populationImage->setOpacity(opacity);
    populationLabel->setOpacity(opacity);
    buildingTimeImage->setOpacity(opacity);
    buildingTimeLabel->setOpacity(opacity);
}

void BuildingCard::updateMask()
{
    if(type != 1 && type != 2 && type != 3)
    {
        if(GameHUD::getThis()->targetMoney < building->buildingCost)
        {
            mask->setOpacity((GLubyte) 120);
        }
        else
        {
            mask->setOpacity((GLubyte) 0);
        }
    }
}