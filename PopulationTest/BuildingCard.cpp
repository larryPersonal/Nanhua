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
  //  ccColor3B colorYellow = ccc3(225, 219, 108);
 //   ccColor3B colorGreen = ccc3(81, 77, 2);
    stringstream ss;
    
    std::string buildingname;
    if (type == 1)
        buildingname = "Build Path";
    else if (type == 2)
       buildingname = "Destroy Path";
    else if (type == 3)
       buildingname = "Demolish";
    else
       buildingname = building->buildingName;
    
    // display the name of the building
    //std::stringstream ss;
    //ss << building->buildingName;
    buildingNameLabel = CCLabelTTF::create(buildingname.c_str(), "Shojumaru-Regular", 20);
    buildingNameLabel->setColor(colorBlack);
    buildingNameLabel->setAnchorPoint(ccp(0, 1));
    
    // display the building info button
    buildingInfoButton = CCMenuItemImage::create( "build-menu_info.png", "build-menu_info.png", NULL, this, menu_selector(BuildingCard::pressDownInfo), menu_selector(BuildingCard::showBuildingInfo) );
    buildingInfoButton->setScale(32.0f / buildingInfoButton->boundingBox().size.width);
    buildingInfoButton->setAnchorPoint(ccp(0.5, 0.5));
    
    infoButtonMenuItemsArray = CCArray::create();
    infoButtonMenuItemsArray->retain();
    infoButtonMenuItemsArray->addObject(buildingInfoButton);
    infoButtonMenu = CCMenu::createWithArray(infoButtonMenuItemsArray);
    
    // display the picture of the building
    if (type == 0)
    {
        CCSprite* menuImage = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
        menuImage->setScale(192.0f / menuImage->boundingBox().size.width);
        buildingImage = menuImage;
        buildingImage->setAnchorPoint(ccp(0.5, 0.5));
        buildingImage->setContentSize(menuImage->boundingBox().size);
    }
    else
    {
        //building is likely to be NULL here! use hardcoded stuff. sorry.
        
     //   CCSprite* menuImage = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
        CCSprite* menuImage;
        if (type == 1)
            menuImage = CCSprite::create("path.png");
        else if (type == 2)
            menuImage = CCSprite::create("path-destroy.png");
        else
            menuImage = CCSprite::create("path.png");
       
        menuImage->setScale(192.0f / menuImage->boundingBox().size.width);
        buildingImage = menuImage;
        buildingImage->setAnchorPoint(ccp(0.5, 0.5));
        buildingImage->setContentSize(menuImage->boundingBox().size);
    }
    
    // cost
    costImage = CCSprite::create("build-menu_goldreq.png");
    costImage->setScale(42.0f / costImage->boundingBox().size.width);
    costImage->setAnchorPoint(ccp(0, 1));
    
    std::string buildingcost;
    if (type == 1)
        buildingcost = "0";
    else if (type == 2)
        buildingcost = "0";
    else if (type == 3)
        buildingcost = "0";
    else
    {
        ss.str(std::string());
        ss << building->buildingCost;
        buildingcost = ss.str();
    }
    costLabel = CCLabelTTF::create(buildingcost.c_str(), "Shojumaru-Regular", 12, CCSizeMake(buildingcost.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    costLabel->setColor(colorBlack);
    costLabel->setAnchorPoint(ccp(0, 1));
    
    // population
    populationImage = CCSprite::create("build-menu_worker-req.png");
    populationImage->setScale(42.0f / populationImage->boundingBox().size.width);
    populationImage->setAnchorPoint(ccp(0, 1));
    
    std::string populationLimit;
    if (type != 0) populationLimit = "0";
    else
    {
        ss.str(std::string());
        ss << building->populationLimit;
        populationLimit = ss.str();
    }
    populationLabel = CCLabelTTF::create(populationLimit.c_str(), "Shojumaru-Regular", 12, CCSizeMake(populationLimit.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    populationLabel->setColor(colorBlack);
    populationLabel->setAnchorPoint(ccp(0, 1));
    
    // building time
    buildingTimeImage = CCSprite::create("build-menu_time-req.png");
    buildingTimeImage->setScale(42.0f / buildingTimeImage->boundingBox().size.width);
    buildingTimeImage->setAnchorPoint(ccp(0, 1));
    
    std::string buildTime;
    if (type != 0) buildTime = "0";
    else
    {
        ss.str(std::string());
        ss << building->build_uint_required;
        buildTime = ss.str();
    }
    buildingTimeLabel = CCLabelTTF::create(buildTime.c_str(), "Shojumaru-Regular", 12, CCSizeMake(buildTime.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    buildingTimeLabel->setColor(colorBlack);
    buildingTimeLabel->setAnchorPoint(ccp(0, 1));
    
    
    cardBG = CCMenuItemImage::create("house_btn.png", NULL, NULL, this, menu_selector(BuildingCard::pressBuildingCard), menu_selector(BuildingCard::onMenuItemSelected));
    if(type == 0)
    {
        cardBG->setTag(building->ID);
    }
    else
    {
        cardBG->setTag(-type); //this should only give -1, -2 and -3.
    }
    cardBG->setAnchorPoint(ccp(0.5, 0.5));
    cardBG->setScaleY(18.0f / buildingInfoButton->boundingBox().size.width);
    cardBG->setScaleX(22.0f / buildingInfoButton->boundingBox().size.width);
    
    menuItemsArray = CCArray::create();
    menuItemsArray->retain();
    menuItemsArray->addObject(cardBG);
    menu = CCMenu::createWithArray(menuItemsArray);
    
    cardDetailBG = CCSprite::create("housedetailred.png");
    cardDetailBG->setScale(1.01f);
    cardDetailBG->setAnchorPoint(ccp(0.5, 0.5));
    
    mask = CCSprite::create("black.png");
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
    else
    {
        mask->setOpacity((GLubyte) 0);
    }
    
    
    cardBG->addChild(buildingNameLabel);
    buildingNameLabel->setPosition(ccp(20, cardBG->boundingBox().size.height * 2 - 50));
    
    cardBG->addChild(buildingImage);
    if(type == 0)
    {
        if(building->buildingType == HOUSING)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width, cardBG->boundingBox().size.height + 40));
        }
        else if(building->buildingType == AMENITY)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width, cardBG->boundingBox().size.height + 60));
        }
        else if(building->buildingType == GRANARY)
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width - 75, cardBG->boundingBox().size.height - 20));
        }
        else
        {
            buildingImage->setPosition(ccp(cardBG->boundingBox().size.width, cardBG->boundingBox().size.height + 30));
        }
    }
    else
    {
        buildingImage->setPosition(ccp(cardBG->boundingBox().size.width, cardBG->boundingBox().size.height + 30));
    }
    
    cardBG->addChild(cardDetailBG);
    cardDetailBG->setPosition(ccp(cardBG->boundingBox().size.width - 54, 12));
    
    cardBG->addChild(infoButtonMenu);
    buildingInfoButton->setScaleX(buildingInfoButton->getScaleX() * 1.2f);
    buildingInfoButton->setScaleY(buildingInfoButton->getScaleY() * 22.0f / 18.0f * 1.2f);
    infoButtonMenu->setPosition(ccp(265, 265));
    
    cardBG->addChild(costImage);
    costImage->setPosition(ccp(12, 30));
    
    cardBG->addChild(costLabel);
    costLabel->setScaleX(costLabel->getScaleX() * 1.4f);
    costLabel->setScaleY(costLabel->getScaleY() * 22.0f / 18.0f * 1.4f);
    costLabel->setPosition(ccp(55, 20));
    
    cardBG->addChild(populationImage);
    populationImage->setPosition(ccp(110, 30));
    
    cardBG->addChild(populationLabel);
    populationLabel->setScaleX(populationLabel->getScaleX() * 1.4f);
    populationLabel->setScaleY(populationLabel->getScaleY() * 22.0f / 18.0f * 1.4f);
    populationLabel->setPosition(ccp(60, 20));
    
    cardBG->addChild(buildingTimeImage);
    buildingTimeImage->setPosition(ccp( 170, 30));
    
    cardBG->addChild(buildingTimeLabel);
    buildingTimeLabel->setScaleX(buildingTimeLabel->getScaleX() * 1.4f);
    buildingTimeLabel->setScaleY(buildingTimeLabel->getScaleY() * 22.0f / 18.0f * 1.4f);
    populationLabel->setPosition(ccp(200, 20));
    
    scrollArea->addItem(menu, ccp(100.0f + 200.0f * index, 82.0f));
    scrollArea->addItem(mask, ccp(200.0f * index, 0.0f));
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
    //cardBG->setScaleX(cardBG->getScaleX() * 0.95f / 1.0f);
    //cardBG->setScaleY(cardBG->getScaleY() * 0.95f / 1.0f);
}

void BuildingCard::onMenuItemSelected(CCObject* pSender)
{
    if (!GameHUD::getThis()->isThisTapCounted)
    {
        return;
    }
    
    //cardBG->setScaleX(cardBG->getScaleX() * 1.0f / 0.95f);
    //cardBG->setScaleY(cardBG->getScaleY() * 1.0f / 0.95f);
    
    CCMenuItemImage* pMenuItemImage = (CCMenuItemImage *)(pSender);
    int tag = pMenuItemImage->getTag();
    switch (tag)
    {
        case -1 : //build path
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
        
            GameHUD::getThis()->setTapMode(3);
            GameScene::getThis()->isThisTapCounted = true;
            GameHUD::getThis()->buildScroll->scheduleScrollOut();
            GameHUD::getThis()->buildButton->setVisible(true);
            
            
        }
            break;
        case -2 : //unbuild path
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildRoad)
            {
                return;
            }
            
            GameHUD::getThis()->setTapMode(4);
            GameScene::getThis()->isThisTapCounted = true;
            
            GameHUD::getThis()->buildScroll->scheduleScrollOut();
            GameHUD::getThis()->buildButton->setVisible(true);
            
        }
            break;
        case -3: //destory building
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            
            if(TutorialManager::getThis()->teachBuildRoad)
            {
                return;
            }
            //I'll need to set tap mode to demolish. TODO
            GameScene::getThis()->isThisTapCounted = true;
            
            GameHUD::getThis()->buildScroll->scheduleScrollOut();
            
            GameHUD::getThis()->buildButton->setVisible(true);
        }
            break;
        default:
        {
            if(TutorialManager::getThis()->teachBuildRoad)
            {
                return;
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
    
    if(type == HOUSING)
    {
        if(GameScene::getThis()->buildingHandler->housingOnMap->count() + GameScene::getThis()->buildingHandler->housingGhostOnMap->count() >= GameManager::getThis()->housingLimitation->housing_limits.at(level))
        {
            // alert player that the number of buildings has reached the limitation.
            return;
        }
    }
    else if(type == GRANARY)
    {
        if(TutorialManager::getThis()->teachBuildHouse)
        {
            return;
        }
        if(GameScene::getThis()->buildingHandler->granaryOnMap->count() + GameScene::getThis()->buildingHandler->granaryGhostOnMap->count() >= GameManager::getThis()->housingLimitation->granary_limits.at(level))
        {
            return;
        }
    }
    else if(type == AMENITY)
    {
        if(TutorialManager::getThis()->teachBuildHouse)
        {
            return;
        }
        if(GameScene::getThis()->buildingHandler->amenityOnMap->count() + GameScene::getThis()->buildingHandler->amenityGhostOnMap->count() >= GameManager::getThis()->housingLimitation->farm_limits.at(level))
        {
            return;
        }
    }
    else if(type == MILITARY)
    {
        if(TutorialManager::getThis()->teachBuildHouse)
        {
            return;
        }
        if(GameScene::getThis()->buildingHandler->militaryOnMap->count() + GameScene::getThis()->buildingHandler->militaryGhostOnMap->count() >= GameManager::getThis()->housingLimitation->guard_tower_limits.at(level))
        {
            return;
        }
    }
    else
    {
        if(TutorialManager::getThis()->teachBuildHouse)
        {
            return;
        }
    }
    
    if(GameHUD::getThis()->money > buildingToBuy->buildingCost)
    {
        if(TutorialManager::getThis()->teachBuildHouse)
        {
            TutorialManager::getThis()->miniDragon->display();
        }
        GameHUD::getThis()->money -= buildingToBuy->buildingCost;
        GameHUD::getThis()->setTapMode(1);
        
        
        
        GameScene::getThis()->buildingHandler->selectedBuilding = buildingToBuy;
    }
    GameScene::getThis()->isThisTapCounted = true;
    
    GameHUD::getThis()->buildScroll->scheduleScrollOut();
    /*
    BuildScroll::getThis()->closeMenu();
    GameHUD::getThis()->buildButton->setVisible(true);
    GameHUD::getThis()->buildScroll = NULL;
    */
}

void BuildingCard::setOpacity(GLubyte opacity)
{
    cardBG->setOpacity(opacity);
    cardDetailBG->setOpacity(opacity);
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