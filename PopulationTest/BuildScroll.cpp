//
//  BuildScroll.cpp
//  PopulationTest
//
//  Created by admin on 13/2/14.
//
//

#include "BuildScroll.h"
#include "GameScene.h"
#include "BuildingCard.h"
#include "GlobalHelper.h"

BuildScroll* BuildScroll::SP;

BuildScroll* BuildScroll::create(){
    BuildScroll *pRet = new BuildScroll();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        //pRet->setTouchEnabled(true);
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

BuildScroll* BuildScroll::getThis(){
    return SP;
}

void BuildScroll::cleanup(){
    PopupMenu::cleanup();
}

BuildScroll::BuildScroll(){
    BuildScroll::SP = this;
    
    numberOfBuildingCards = 0;
    
    // create gui handler
    background_rect = new Rect();
    
    // initialize the gui handler
    background_rect->ini(700, 100, 100, 200);
    
    //this->setTouchEnabled(true);
}

BuildScroll::~BuildScroll()
{
    BuildScroll::SP = NULL;
}

void BuildScroll::createMenuItems()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    bool isHori = GlobalHelper::isHorizontal();
    
    // create the build group background
    string buildBackground = "build-menu-scroll.png";
    buildMenu = CCSprite::create(buildBackground.c_str());
    CCSize spriteSize = buildMenu->getContentSize();
    buildMenu->setAnchorPoint(ccp(1, 0));
    if(isHori)
    {
        buildMenu->setScale(screenSize.width / spriteSize.width * 0.98f);
    }
    else
    {
        buildMenu->cocos2d::CCNode::setScale(screenSize.width / spriteSize.width * 0.98f, screenSize.height / spriteSize.width * 0.98f);
    }
    buildMenu->setPosition(ccp(screenSize.width, 0));
    this->addChild(buildMenu, 1);
    
    // scroll section for other villagers
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(buildMenu->boundingBox().size.width * 0.875f + screenSize.width * 0.02f, buildMenu->boundingBox().size.height * 0.7f), CCSizeMake(200.0f * GameScene::getThis()->buildingHandler->allBuildings->count(), buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->enableScrollHorizontal(0, "bar.png", "bar.png");
    //scrollArea->hideScroll();
    scrollArea->setAnchorPoint(ccp(0, 0));
    
    // create all building references for all building categories.
    CCArray* allBuildings = GameScene::getThis()->buildingHandler->allBuildings;
    // list down all the buildings
    numberOfBuildingCards = 0;
    //Path
    /*
    CCSprite* pathImg =   CCSprite::create("path.png");
    CCSprite* pathSelectedImg = CCSprite::create("path.png");
    pathImg->             setScale(128 / pathImg->boundingBox().size.width);
    pathSelectedImg->           setScale(pathImg->getScale());
    CCMenuItemSprite* buildPathItem =  CCMenuItemSprite::create(pathImg, pathSelectedImg, this, menu_selector(BuildMenu::onMenuItemSelected) );
    buildPathItem->setTag(-3);
    
    
    
    buildPathItem->setAnchorPoint(ccp(0, 1));
    buildPathItem->setContentSize(pathImg->boundingBox().size);
    
    CCLabelTTF* pathLabel =     CCLabelTTF::create("Build Path", "Shojumaru-Regular", 30);
    CCPoint pathlabelPosition = ccp(buildPathItem->boundingBox().size.width*2.5,
                                    buildPathItem->boundingBox().size.height*0.5);
    pathLabel->setPosition(pathlabelPosition);
    buildPathItem->addChild(pathLabel);
    
    buildPathItem->  setPosition(CCPointMake(0, 0));
    */
    BuildingCard* bc1 = BuildingCard::create(NULL, scrollArea, numberOfBuildingCards, 1);
    numberOfBuildingCards++;

    BuildingCard* bc2 = BuildingCard::create(NULL, scrollArea, numberOfBuildingCards, 2);
    numberOfBuildingCards++;
    
    
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == HOUSING)
        {
            BuildingCard* bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == AMENITY)
        {
            BuildingCard* bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == GRANARY)
        {
            BuildingCard* bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == MILITARY)
        {
            BuildingCard* bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    scrollArea->setScrollContentSize(CCSizeMake(200.0f * numberOfBuildingCards + screenSize.width * 0.02f, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setPosition(ccp(screenSize.width * 0.125f, buildMenu->boundingBox().size.height * 0.14f));
    scrollArea->updateScrollBars();
    this->addChild(scrollArea, 1);
}

void BuildScroll::onMenuItemSelected(CCObject* pSender){
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case -1:
            // buttonClose
            this->closeMenu(true);
            break;
            
        default:
            break;
    }
}

void BuildScroll::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
}

void BuildScroll::refreshAllMenuItemValues()
{
}

void BuildScroll::willChangeOrientation()
{
    
}

void BuildScroll::onOrientationChanged()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    bool isHori = GlobalHelper::isHorizontal();
    CCSize spriteSize = buildMenu->getContentSize();
    
    if(isHori)
    {
        buildMenu->setScale(screenSize.width / spriteSize.width * 0.98f);
    }
    else
    {
        buildMenu->cocos2d::CCNode::setScale(screenSize.width / spriteSize.width * 0.98f, screenSize.height / spriteSize.width * 0.98f);
    }
    buildMenu->setPosition(ccp(screenSize.width, 0));
    
    scrollArea->setScrollContentSize(CCSizeMake(200.0f * numberOfBuildingCards + screenSize.width * 0.02f, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setViewSize(CCSizeMake(buildMenu->boundingBox().size.width * 0.875f + screenSize.width * 0.02f, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setPosition(ccp(screenSize.width * 0.125f, buildMenu->boundingBox().size.height * 0.14f));
    scrollArea->updateScrollBars();
}

void BuildScroll::update(float deltaTime)
{
    refreshAllMenuItemValues();
}


