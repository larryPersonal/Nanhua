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

BuildScroll* BuildScroll::SP;

BuildScroll* BuildScroll::create(){
    BuildScroll *pRet = new BuildScroll();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        pRet->setTouchEnabled(true);
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
    
    //buildingCards->CCArray::create();
    //buildingCards->retain();
    
    // create gui handler
    background_rect = new Rect();
    
    // initialize the gui handler
    background_rect->ini(700, 100, 100, 200);
}

BuildScroll::~BuildScroll()
{
    //buildingCards->removeAllObjects();
    //buildingCards->release();
    BuildScroll::SP = NULL;
}

void BuildScroll::createMenuItems()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorBlue = ccc3(0, 0, 255);
    ccColor3B colorWhite = ccc3(255, 255, 255);
    
    // create the build group background
    string buildBackground = "build-menu-scroll.png";
    buildMenu = CCSprite::create(buildBackground.c_str());
    CCSize spriteSize = buildMenu->getContentSize();
    buildMenu->setScale(screenSize.width / spriteSize.width * 0.98f);
    buildMenu->setAnchorPoint(ccp(1, 0));
    buildMenu->setPosition(ccp(screenSize.width, 0));
    this->addChild(buildMenu, 1);
    
    // scroll section for other villagers
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(buildMenu->boundingBox().size.width * 0.9f, buildMenu->boundingBox().size.height), CCSizeMake(buildMenu->boundingBox().size.width * 2.0f, buildMenu->boundingBox().size.height));
    scrollArea->enableScrollHorizontal(0, "bar.png", "bar.png");
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    //scrollArea->hideScroll();
    scrollArea->setAnchorPoint(ccp(0, 1));
    
    
    
    // create all building references for all building categories.
    CCArray* allBuildings = GameScene::getThis()->buildingHandler->allBuildings;
    // list down all the buildings
    for(int i = 0; i < allBuildings->count(); i++)
    {
        BuildingCard* bc = BuildingCard::create((Building*) allBuildings->objectAtIndex(i), scrollArea);
        //buildingCards->addObject((CCObject*) bc);
    }
    scrollArea->setPosition(ccp(0, screenSize.height / 2.0f));
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

void BuildScroll::reposition(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
}

void BuildScroll::refreshAllMenuItemValues()
{
}

void BuildScroll::willChangeOrientation(){
    
}

void BuildScroll::onOrientationChanged(){
    
}

void BuildScroll::update(float deltaTime){
    refreshAllMenuItemValues();
}



