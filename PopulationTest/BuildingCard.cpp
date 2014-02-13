//
//  BuildingCard.cpp
//  PopulationTest
//
//  Created by admin on 13/2/14.
//
//

#include "BuildingCard.h"

BuildingCard::BuildingCard(Building* building, ScrollArea* scrollArea)
{
    this->building = building;
    this->scrollArea = scrollArea;
    init();
}

BuildingCard::~BuildingCard()
{
    
}

BuildingCard* BuildingCard::create(Building* building, ScrollArea* scrollArea)
{
    BuildingCard *pRet = new BuildingCard(building, scrollArea);
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
    CCLog("test******************");
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    std::stringstream ss;
    
    // display the name of the building
    ss << building->buildingName;
    CCLog(building->buildingName.c_str());
    buildingNameLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    buildingNameLabel->setAnchorPoint(ccp(0, 1));
    
    // display the building info button
    buildingInfoButton = CCMenuItemImage::create( "build-menu_info.png", "build-menu_info.png", this, menu_selector(BuildingCard::showBuildingInfo) );
    buildingInfoButton->setScale(24.0f / buildingInfoButton->boundingBox().size.width);
    buildingInfoButton->setAnchorPoint(ccp(0, 1));
    
    // display the picture of the building
    buildingImage = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    buildingImage->setScale(48.0f / buildingImage->boundingBox().size.width);
    buildingImage->setAnchorPoint(ccp(0, 1));
    
    scrollArea->addItem(buildingNameLabel, ccp(-100.0f, 20.0f));
    scrollArea->addItem(buildingInfoButton, ccp(50.0f, -50.0f));
    scrollArea->addItem(buildingImage, ccp(30.0f, 0.0f));
}

void BuildingCard::refreshAllMenuItems()
{
}

void BuildingCard::showBuildingInfo()
{
    
}