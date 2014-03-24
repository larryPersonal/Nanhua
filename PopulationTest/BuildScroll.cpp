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
#include "GameHUD.h"

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
    
    leftPos = 0;
    maxPos = 0.98f;
    
    buildingCards = CCArray::create();
    buildingCards->retain();
    
    scroll_in = false;
    scroll_out = false;
}

BuildScroll::~BuildScroll()
{
    BuildScroll::SP = NULL;
    
    buildingCards->removeAllObjects();
    CC_SAFE_RELEASE(buildingCards);
}

void BuildScroll::createMenuItems()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // create the build group background
    string buildBackground = "scroller.png";
    buildMenu = CCSprite::create(buildBackground.c_str());
    CCSize spriteSize = buildMenu->getContentSize();
    buildMenu->setAnchorPoint(ccp(0, 0));
    buildMenu->setScale(screenSize.width / spriteSize.width * 0.98f);
    buildMenu->setPosition(ccp(screenSize.width * (1.0f - leftPos), 0));
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
    BuildingCard* bc = BuildingCard::create(NULL, scrollArea, numberOfBuildingCards, 1);
    numberOfBuildingCards++;
    buildingCards->addObject(bc);

    bc = BuildingCard::create(NULL, scrollArea, numberOfBuildingCards, 2);
    numberOfBuildingCards++;
    buildingCards->addObject(bc);
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == HOUSING)
        {
            bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
            buildingCards->addObject(bc);
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == AMENITY)
        {
            bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
            buildingCards->addObject(bc);
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == GRANARY)
        {
            bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
            buildingCards->addObject(bc);
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == MILITARY)
        {
            bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
            numberOfBuildingCards++;
            buildingCards->addObject(bc);
        }
    }
    
    scrollArea->setScrollContentSize(CCSizeMake(250.0f * numberOfBuildingCards, buildMenu->boundingBox().size.height * 0.7f));
    scrollArea->setPosition(ccp(screenSize.width * 0.105f + screenSize.width * (maxPos - leftPos), buildMenu->boundingBox().size.height * 0.14f));
    scrollArea->updateScrollBars();
    this->addChild(scrollArea, 1);
    
    scroll_in = true;
    this->schedule(schedule_selector( BuildScroll::scrollIn ), 1/120.0f);
}

void BuildScroll::onMenuItemSelected(CCObject* pSender){
    
}

void BuildScroll::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float temp = (leftPos / maxPos) * 255;
    
    if(temp < 0)
    {
        temp = 0;
    }
    else if(temp > 255)
    {
        temp = 255;
    }
    
    GLubyte opacity = temp;
    
    buildMenu->setOpacity(opacity);
    buildMenu->setPosition(ccp(screenSize.width * (1.0f - leftPos), 0));
    
    scrollArea->setPosition(ccp(screenSize.width * 0.105f + screenSize.width * (maxPos - leftPos), buildMenu->boundingBox().size.height * 0.14f));
    
    for(int i = 0; i < buildingCards->count(); i++)
    {
        BuildingCard* bc = (BuildingCard*) buildingCards->objectAtIndex(i);
        bc->setOpacity(opacity);
    }
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

void BuildScroll::scrollIn(float dt)
{
    if(scroll_in)
    {
        float speed = 0.05f;
        
        leftPos = leftPos + speed;
        
        if(leftPos >= maxPos)
        {
            leftPos = maxPos;
            this->unschedule(schedule_selector( BuildScroll::scrollIn ));
            scroll_in = false;
        }
        reposition();
    }
}

void BuildScroll::scrollOut(float dt)
{
    if(scroll_out)
    {
        float speed = 0.05f;
        
        leftPos = leftPos - speed;
        if(leftPos <= 0)
        {
            leftPos = 0;
            this->unschedule(schedule_selector( BuildScroll::scrollOut ));
            scroll_out = false;
            
            GameHUD::getThis()->buildScroll->closeMenu();
            GameHUD::getThis()->buildScroll = NULL;
            GameHUD::getThis()->buildButton->setVisible(true);
        }
        else
        {
            reposition();
        }
    }
}

void BuildScroll::scheduleScrollOut()
{
    if(!scroll_out && !scroll_in)
    {
        scroll_out = true;
        this->schedule(schedule_selector( BuildScroll::scrollOut ), 1/120.0f);
    }
}

