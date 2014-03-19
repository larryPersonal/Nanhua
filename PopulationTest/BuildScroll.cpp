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
    string buildBackground = "scroller.png";
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
    scrollArea->createScrollArea(CCSizeMake(buildMenu->boundingBox().size.width * 0.85f, buildMenu->boundingBox().size.height * 0.7f), CCSizeMake(GameScene::getThis()->buildingHandler->allBuildings->count() , buildMenu->boundingBox().size.height * 0.5f));
    scrollArea->enableScrollHorizontal(0, "bar.png", "bar.png");
    //scrollArea->hideScroll();
    scrollArea->setAnchorPoint(ccp(0, 0));
    
    // create all building references for all building categories.
    CCArray* allBuildings = GameScene::getThis()->buildingHandler->allBuildings;
    // list down all the buildings
    numberOfBuildingCards = 0;
    BuildingCard::create(NULL, scrollArea, numberOfBuildingCards, 1);
    numberOfBuildingCards++;

    BuildingCard::create(NULL, scrollArea, numberOfBuildingCards, 2);
    numberOfBuildingCards++;
    
    
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == HOUSING)
        {
            BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == AMENITY)
        {
            BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == GRANARY)
        {
            BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == MILITARY)
        {
            BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
        }
    }
    /*
    CCMenuItemImage* leftBtn = CCMenuItemImage::create("leftgreen_arrowbtn.png", "leftgreen_arrowbtn.png", this, menu_selector(BuildScroll::onMenuItemSelected));
    leftBtn->setTag(-3);      //set Tag as address of the GameSprite object
    leftBtn->setPosition(ccp(screenSize.width * 0.07f, buildMenu->boundingBox().size.height * 0.5f));
    
    CCMenuItemImage* rightBtn = CCMenuItemImage::create("leftgreen_arrowbtn.png", "leftgreen_arrowbtn.png", this, menu_selector(BuildScroll::onMenuItemSelected));
    rightBtn->setTag(-4);      //set Tag as address of the GameSprite object
    rightBtn->setPosition(ccp(screenSize.width * 0.97f, buildMenu->boundingBox().size.height * 0.5f));
    */
    scrollArea->setScrollContentSize(CCSizeMake(250.0f * numberOfBuildingCards, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setPosition(ccp(screenSize.width * 0.105f, buildMenu->boundingBox().size.height * 0.14f));
    scrollArea->updateScrollBars();
    this->addChild(scrollArea, 1);
   // this->addChild(leftBtn, 2);
   // this->addChild(rightBtn, 2);

}

void BuildScroll::onMenuItemSelected(CCObject* pSender){
    
   // CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
     //Menu items handle their own click events!
    //Otherwise, only these items will be recognized!
    
    //-3 to scroll left
    //-4 to scroll right
    //-5 to close the scroll and do nothing else
    /*
    switch (pMenuItem->getTag())
    {
        case -3:
            //buttonleft
            CCLog("LEFT CLICKED");
            break;

        case -4:
            //buttonright
            break;

        //I shouldn't need to do this anymore.
        case -5:
            // buttonClose
            this->closeMenu(true);
            break;
        default:
            //its handled elsewhere
            break;
    }
    */
    
}

void BuildScroll::reposition()
{
    //CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
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
    
    scrollArea->setScrollContentSize(CCSizeMake(200.0f * numberOfBuildingCards, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setViewSize(CCSizeMake(buildMenu->boundingBox().size.width * 0.875f, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setPosition(0, buildMenu->boundingBox().size.height);
    scrollArea->updateScrollBars();
}

void BuildScroll::update(float deltaTime)
{
    refreshAllMenuItemValues();
}


