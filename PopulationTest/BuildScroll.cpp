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
#include "TutorialManager.h"
#include "FloatingArraw.h"

BuildScroll* BuildScroll::SP;

BuildScroll* BuildScroll::create(){
    BuildScroll *pRet = new BuildScroll();
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
    string buildBackground = "scrollermiddle_b.png";
    buildMenu = CCSprite::create(buildBackground.c_str());
    CCSize spriteSize = buildMenu->getContentSize();
    buildMenu->setAnchorPoint(ccp(0, 0));
    buildMenu->setScale(screenSize.width / spriteSize.width * 0.98f);
    buildMenu->setPosition(ccp(screenSize.width * (1.01f - leftPos), 25));
    this->addChild(buildMenu, 1);
    
    scrollerBar = CCSprite::create("scroller_side.png");
    scrollerBar->setAnchorPoint(ccp(0, 0));
    scrollerBar->setScale(screenSize.width / spriteSize.width * 0.98f);
    scrollerBar->setPosition(ccp(screenSize.width * (1.0f - leftPos), 0));
    this->addChild(scrollerBar, 2);
    
    // scroll section for other villagers
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(buildMenu->boundingBox().size.width * 0.96f, buildMenu->boundingBox().size.height * 0.92f), CCSizeMake(GameScene::getThis()->buildingHandler->allBuildings->count() , buildMenu->boundingBox().size.height * 0.92f));
    scrollArea->enableScrollHorizontal(0, "bar.png", "bar.png");
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
            if (!nameExists(tempBuilding->buildingName))
            {
            
            
                bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
                numberOfBuildingCards++;
                buildingCards->addObject(bc);
            }
        }
    }
    
    for(int i = 0; i < allBuildings->count(); i++)
    {
        Building* tempBuilding = (Building*) allBuildings->objectAtIndex(i);
        if(tempBuilding->buildingType == AMENITY)
        {
           // if (!nameExists(tempBuilding->buildingName))
           // {
                
                
                bc = BuildingCard::create(tempBuilding, scrollArea, numberOfBuildingCards, 0);
                numberOfBuildingCards++;
                buildingCards->addObject(bc);
          //  }
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
        if(tempBuilding->buildingType == MARKET)
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
    
    scrollArea->setScrollContentSize(CCSizeMake(10.0f + screenSize.width * 0.19f * numberOfBuildingCards, buildMenu->boundingBox().size.height * 0.92f));
    scrollArea->setPosition(ccp(screenSize.width * 0.06f + screenSize.width * (maxPos - leftPos), buildMenu->boundingBox().size.height * 0.14f));
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
    buildMenu->setPosition(ccp(screenSize.width * (1.01f - leftPos), 25));
    
    scrollerBar->setOpacity(opacity);
    scrollerBar->setPosition(ccp(screenSize.width * (1.0f - leftPos), 0));
    
    scrollArea->setPosition(ccp(screenSize.width * 0.06f + screenSize.width * (maxPos - leftPos), buildMenu->boundingBox().size.height * 0.14f));
    
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
    /*
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
    */
}

void BuildScroll::update(float deltaTime)
{
    CCLog("lalalalalal");
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
            
            if(TutorialManager::getThis()->active && TutorialManager::getThis()->miniDragon != NULL && FloatingArraw::getThis() != NULL && FloatingArraw::getThis()->arrow != NULL)
            {
                this->scrollArea->removeI(FloatingArraw::getThis()->arrow);
            }
            
            GameHUD::getThis()->buildScroll->closeMenu(true);
            GameHUD::getThis()->buildScroll = NULL;
            GameHUD::getThis()->buildButton->setVisible(true);
            
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            GameHUD::getThis()->buildButton->setPosition(ccp(screenSize.width, 0));
            GameHUD::getThis()->isThisTapCounted = true;
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

//This check is optional because some buildings have default names and won't be added otherwise.
//To use this supply the name of the building you are about to add to the array
bool BuildScroll::nameExists(const std::string& checkName)
{
    if (buildingCards == NULL) return false;
    if (buildingCards->count() == 0) return false;
    for (int i = 0; i < buildingCards->count(); ++i)
    {
        if (buildingCards->objectAtIndex(i) == NULL) continue;
        if (((BuildingCard*)buildingCards->objectAtIndex(i))->building == NULL) continue;
        if (checkName.compare(((BuildingCard*)buildingCards->objectAtIndex(i))->building->buildingName));
        return true;
    }
    return false;
}
