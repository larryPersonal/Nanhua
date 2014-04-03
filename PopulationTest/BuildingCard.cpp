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
    menuItemsArray->release();
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
  //  CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
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
    buildingNameLabel = CCLabelTTF::create(buildingname.c_str(), "Shojumaru-Regular", 16, CCSizeMake(buildingname.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    buildingNameLabel->setColor(colorBlack);
    buildingNameLabel->setAnchorPoint(ccp(0.5, 1));
    
    // display the building info button
    buildingInfoButton = CCMenuItemImage::create( "build-menu_info.png", "build-menu_info.png", this, menu_selector(BuildingCard::showBuildingInfo) );
    buildingInfoButton->setScale(32.0f / buildingInfoButton->boundingBox().size.width);
    buildingInfoButton->setAnchorPoint(ccp(0, 1));
    
    // display the picture of the building
    if (type == 0)
    {
        CCSprite* menuImage = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
        menuImage->setScale(128.0f / menuImage->boundingBox().size.width);
        buildingImage = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(BuildingCard::onMenuItemSelected));
        buildingImage->setTag(building->ID);
        buildingImage->setAnchorPoint(ccp(0, 1));
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
       
        menuImage->setScale(128.0f / menuImage->boundingBox().size.width);
        buildingImage = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(BuildingCard::onMenuItemSelected));
        buildingImage->setTag(-type); //this should only give -1, -2 and -3.
        buildingImage->setAnchorPoint(ccp(0, 1));
        buildingImage->setContentSize(menuImage->boundingBox().size);
    }
    
    
    menuItemsArray = CCArray::create();
    menuItemsArray->retain();
    menuItemsArray->addObject(buildingImage);
    menu = CCMenu::createWithArray(menuItemsArray);
    
    // cost
    costImage = CCSprite::create("build-menu_goldreq.png");
    costImage->setScale(28.0f / costImage->boundingBox().size.width);
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
    populationImage->setScale(28.0f / populationImage->boundingBox().size.width);
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
    buildingTimeImage->setScale(28.0f / buildingTimeImage->boundingBox().size.width);
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
    
    
    cardBG = CCSprite::create("house_btn.png");
    cardBG->setScaleY(18.0f / buildingInfoButton->boundingBox().size.width);
    cardBG->setScaleX(22.0f / buildingInfoButton->boundingBox().size.width);
    
    cardDetailBG = CCSprite::create("housedetail.png");
    cardDetailBG->setScaleY(20.0f / buildingInfoButton->boundingBox().size.width);
    cardDetailBG->setScaleX(22.0f / buildingInfoButton->boundingBox().size.width);
    
    mask = CCSprite::create("black.png");
    mask->cocos2d::CCNode::setScale(cardBG->boundingBox().size.width / mask->boundingBox().size.width, cardBG->boundingBox().size.height / mask->boundingBox().size.height * 1.05f);
    mask->setOpacity((GLubyte) 120);
    if(TutorialManager::getThis()->teachBuildHouse)
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
    
    
    scrollArea->addItem(cardBG, ccp(200.0f * index, 0.0f));
    scrollArea->addItem(cardDetailBG, ccp(3.f + 200.0f * index, 145.0f));

    scrollArea->addItem(buildingNameLabel, ccp(20.0f + 200.0f * index, 5.0f));
    scrollArea->addItem(buildingInfoButton, ccp(160.0f + 200.0f * index, 0.0f));
    scrollArea->addItem(menu, ccp(40.0f + 200.0f * index, 10.0f));
    scrollArea->addItem(costImage, ccp(10.0f + 200.0f * index, 145.0f));
    scrollArea->addItem(costLabel, ccp(40.0f + 200.0f * index, 150.0f));
    scrollArea->addItem(populationImage, ccp(80.0f + 200.0f * index, 145.0f));
    scrollArea->addItem(populationLabel, ccp(110.0f + 200.0f * index, 150.0f));
    scrollArea->addItem(buildingTimeImage, ccp(130.0f + 200.0f * index, 145.0f));
    scrollArea->addItem(buildingTimeLabel, ccp(160.0f + 200.0f * index, 150.0f));
    scrollArea->addItem(mask, ccp(200.0f * index, 0.0f));
}

void BuildingCard::refreshAllMenuItems()
{
}

void BuildingCard::showBuildingInfo()
{
    
}

void BuildingCard::onMenuItemSelected(CCObject* pSender)
{
    if (!GameHUD::getThis()->isThisTapCounted)
    {
        return;
    }
    
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
            BuildScroll::getThis()->closeMenu(false);
            GameHUD::getThis()->buildButton->setVisible(true);
            GameHUD::getThis()->buildScroll = NULL;
        }
            break;
        case -2 : //unbuild path
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            GameHUD::getThis()->setTapMode(4);
            BuildScroll::getThis()->closeMenu(false);
            GameHUD::getThis()->buildButton->setVisible(true);
            GameHUD::getThis()->buildScroll = NULL;
        }
            break;
        case -3: //destory building
        {
            if(TutorialManager::getThis()->teachBuildHouse)
            {
                return;
            }
            //I'll need to set tap mode to demolish.
            BuildScroll::getThis()->closeMenu(false);
            GameHUD::getThis()->buildButton->setVisible(true);
            GameHUD::getThis()->buildScroll = NULL;
        }
            break;
        default:
        {
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