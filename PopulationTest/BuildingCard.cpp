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

BuildingCard::BuildingCard(Building* building, ScrollArea* scrollArea, int index)
{
    this->building = building;
    this->scrollArea = scrollArea;
    this->index = index;
    init();
}

BuildingCard::~BuildingCard()
{
    menuItemsArray->removeAllObjects();
    menuItemsArray->release();
}

BuildingCard* BuildingCard::create(Building* building, ScrollArea* scrollArea, int index)
{
    BuildingCard *pRet = new BuildingCard(building, scrollArea, index);
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
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    std::stringstream ss;
    
    // display the name of the building
    ss << building->buildingName;
    buildingNameLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    buildingNameLabel->setColor(colorBlack);
    buildingNameLabel->setAnchorPoint(ccp(0.5, 1));
    
    // display the building info button
    buildingInfoButton = CCMenuItemImage::create( "build-menu_info.png", "build-menu_info.png", this, menu_selector(BuildingCard::showBuildingInfo) );
    buildingInfoButton->setScale(32.0f / buildingInfoButton->boundingBox().size.width);
    buildingInfoButton->setAnchorPoint(ccp(0, 1));
    
    // display the picture of the building
    CCSprite* menuImage = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    menuImage->setScale(128.0f / menuImage->boundingBox().size.width);
    buildingImage = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(BuildingCard::onMenuItemSelected));
    buildingImage->setTag(building->ID);
    buildingImage->setAnchorPoint(ccp(0, 1));
    buildingImage->setContentSize(menuImage->boundingBox().size);
    
    menuItemsArray = CCArray::create();
    menuItemsArray->retain();
    menuItemsArray->addObject(buildingImage);
    menu = CCMenu::createWithArray(menuItemsArray);
    
    // cost
    costImage = CCSprite::create("build-menu_goldreq.png");
    costImage->setScale(28.0f / costImage->boundingBox().size.width);
    costImage->setAnchorPoint(ccp(0, 1));
    
    ss.str(std::string());
    ss << building->buildingCost;
    costLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    costLabel->setColor(colorBlack);
    costLabel->setAnchorPoint(ccp(0, 1));
    
    // population
    populationImage = CCSprite::create("build-menu_worker-req.png");
    populationImage->setScale(28.0f / populationImage->boundingBox().size.width);
    populationImage->setAnchorPoint(ccp(0, 1));
    
    ss.str(std::string());
    ss << building->populationLimit;
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    populationLabel->setColor(colorBlack);
    populationLabel->setAnchorPoint(ccp(0, 1));
    
    // building time
    buildingTimeImage = CCSprite::create("build-menu_time-req.png");
    buildingTimeImage->setScale(28.0f / buildingTimeImage->boundingBox().size.width);
    buildingTimeImage->setAnchorPoint(ccp(0, 1));
    
    ss.str(std::string());
    ss << building->build_uint_required;
    buildingTimeLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    buildingTimeLabel->setColor(colorBlack);
    buildingTimeLabel->setAnchorPoint(ccp(0, 1));
    
    scrollArea->addItem(buildingNameLabel, ccp(60.0f + 200.0f * index, 15.0f));
    scrollArea->addItem(buildingInfoButton, ccp(180.0f + 200.0f * index, 10.0f));
    scrollArea->addItem(menu, ccp(60.0f + 200.0f * index, 40.0f));
    scrollArea->addItem(costImage, ccp(20.0f + 200.0f * index, 190.0f));
    scrollArea->addItem(costLabel, ccp(50.0f + 200.0f * index, 195.0f));
    scrollArea->addItem(populationImage, ccp(100.0f + 200.0f * index, 190.0f));
    scrollArea->addItem(populationLabel, ccp(130.0f + 200.0f * index, 195.0f));
    scrollArea->addItem(buildingTimeImage, ccp(150.0f + 200.0f * index, 190.0f));
    scrollArea->addItem(buildingTimeLabel, ccp(180.0f + 200.0f * index, 195.0f));
}

void BuildingCard::refreshAllMenuItems()
{
}

void BuildingCard::showBuildingInfo()
{
    
}

void BuildingCard::onMenuItemSelected(CCObject* pSender)
{
    CCMenuItemImage* pMenuItemImage = (CCMenuItemImage *)(pSender);
    int tag =pMenuItemImage->getTag();
    switch (tag)
    {
        case -3 : //build path
        {
            GameHUD::getThis()->setTapMode(3);
            GameScene::getThis()->isThisTapCounted = true;
            BuildScroll::getThis()->closeMenu();
        }
            break;
        case -4 : //unbuild path
        {
            GameHUD::getThis()->setTapMode(4);
            BuildScroll::getThis()->closeMenu();
        }
            break;
        default:
        {
            stringstream ss;
            Building* buildingToBuy = GameScene::getThis()->buildingHandler->getBuilding(tag);
            
            if(buildingToBuy == NULL)
            {
                return;
            }
            GameHUD::getThis()->setTapMode(1);
            GameScene::getThis()->buildingHandler->selectedBuilding = buildingToBuy;
            GameScene::getThis()->isThisTapCounted = true;
            BuildScroll::getThis()->closeMenu();
        }
            break;
    }
}