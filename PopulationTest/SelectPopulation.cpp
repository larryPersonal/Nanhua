//
//  SelectPopulation.cpp
//  PopulationTest
//
//  Created by admin on 10/1/14.
//
//

#include "SelectPopulation.h"
#include "GameScene.h"
#include "Sprite.h"

SelectPopulation* SelectPopulation::SP;

SelectPopulation* SelectPopulation::create(Building* building){
    SelectPopulation *pRet = new SelectPopulation(building);
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

SelectPopulation* SelectPopulation::getThis(){
    return SP;
}

void SelectPopulation::cleanup(){
    PopupMenu::cleanup();
}

SelectPopulation::SelectPopulation(Building* building){
    if(!building){
        CCLog("Warning No Building!");
    }
    SelectPopulation::SP = this;
    
    spriteRowArray = CCArray::create();
    spriteRowArray->retain();
    
    memberArray = CCArray::create();
    memberArray->retain();
    
    emptySpaceArray = CCArray::create();
    emptySpaceArray->retain();
    
    this->building = building;
    
    // create gui handler
    background_rect = new Rect();
    
    // initialize the gui handler
    background_rect->ini(700, 100, 100, 200);
}

SelectPopulation::~SelectPopulation()
{
    SelectPopulation::SP = NULL;
    /*
     for (int i = 0; i < allSprites->count(); ++i)
     {
     spriteRowArray->objectAtIndex(i)->release();
     }*/
    spriteRowArray->removeAllObjects();
    CC_SAFE_RELEASE(spriteRowArray);
    
    memberArray->removeAllObjects();
    CC_SAFE_RELEASE(memberArray);
    
    emptySpaceArray = CCArray::create();
    CC_SAFE_RELEASE(emptySpaceArray);
}

void SelectPopulation::createMenuItems()
{
    
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    ccColor3B colorWhite = ccc3(255, 255, 255);
    
    // background
    spriteBackground = CCSprite::create("worker_assign_ui_bg_03.png");
    spriteBackground->setScale(background_rect->width / spriteBackground->boundingBox().size.width * 1.4f);
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(spriteBackground, 3);
    
    // Sprite
    spriteBuilding = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    spriteBuilding->setScale(200.0f / spriteBuilding->boundingBox().size.width);
    spriteBuilding->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(spriteBuilding, 4);
    
    // Menu items: including ok button and cancel button
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCMenuItemImage::create("assign_menu_cancel.png", "assign_menu_cancel.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
    buttonClose->setScale(0.4);
    buttonClose->setTag(-1);
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    buttonOk = CCMenuItemImage::create("assign_menu_accept.png", "assign_menu_accept.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
    buttonOk->setScale(0.4);
    buttonOk->setTag(-2);
    buttonOk->setAnchorPoint(ccp(1, 1));
    
    menuItems->addObject(buttonOk);
    menuItems->addObject(buttonClose);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 3);
    
    // building name label
    std::stringstream ss;
    ss << building->buildingName;
    
    labelBuildingName = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelBuildingName->setColor(colorWhite);
    labelBuildingName->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(labelBuildingName, 4);

    // empty spaces
    int population = building->populationLimit;
    
    for (int i = 0; i < population; i++)
    {
        CCSprite* eSpace = CCSprite::create("assign_menu_unfilled.png");
        eSpace->setScale(70.0f / eSpace->boundingBox().size.width);
        eSpace->setAnchorPoint(ccp(0, 1));
        this->addChild(eSpace, 4);
        emptySpaceArray->addObject(eSpace);
    }
    
    // scroll section for other villagers
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(450, 360), CCSizeMake(450, 360));
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    scrollArea->hideScroll();
    scrollArea->setAnchorPoint(ccp(0, 0.5));
    this->addChild(scrollArea, 4);
    
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        SpriteRow* sp = SpriteRow::create((GameSprite*) spritesOnMap->objectAtIndex(i), scrollArea, building, i);
        spriteRowArray->addObject((CCObject*) sp);
    }
    
    scrollArea->updateScrollBars();
    
    // set the position of all the elements
    reposition();
    
    this->schedule(schedule_selector(SelectPopulation::update), 0.25f);
}

void SelectPopulation::onMenuItemSelected(CCObject* pSender){
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case -1:
            // buttonClose
            this->closeMenu(true);
            GameScene::getThis()->setTouchEnabled(true);
            break;
            
        default:
            break;
    }
}


void SelectPopulation::reposition(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    
    spriteBuilding->setPosition(ccp(240.0f, 40.0f));
    
    // Anchored top right
    buttonClose->setPosition(halfWidth - 25.0f, -halfHeight + 75.0f);
    buttonOk->setPosition(halfWidth - 80.0f, -halfHeight + 75.0f);
    
    labelBuildingName->CCNode::setPosition(285.0f, -100.0f);
    
    // for empty space
    for (int i = 0; i < emptySpaceArray->count(); i++)
    {
        ((CCSprite*) emptySpaceArray->objectAtIndex(i))->setPosition(ccp(30.0f + 70.0f * i, -135.0f));
    }
    
    // Scroll area in center
    scrollArea->CCNode::setPosition(-halfWidth + 20.0f, -halfHeight / 2.0f - 95.0f);
    scrollArea->reposition();
}

void SelectPopulation::refreshAllMenuItemValues()
{
    // refresh all the sprite rows
    for(int i = 0; i < spriteRowArray->count(); i++)
    {
        ((SpriteRow*) spriteRowArray->objectAtIndex(i))->refreshAllMenuItems();
    }
}

void SelectPopulation::willChangeOrientation(){
    
}

void SelectPopulation::onOrientationChanged(){
    
}

void SelectPopulation::update(float deltaTime){
    refreshAllMenuItemValues();
}

void SelectPopulation::addMemberRow(GameSprite* gameSprite, SpriteRow* spriteRow)
{
    spriteRowArray->removeObject((CCObject*)spriteRow);
    
    for(int i = 0; i < spriteRowArray->count(); i++)
    {
        ((SpriteRow*)(spriteRowArray->objectAtIndex(i)))->rearrange(i);
    }
    
}

void SelectPopulation::addVillagerRow(GameSprite * gameSprite, SpriteRow * spriteRow)
{
    SpriteRow* sr = SpriteRow::create(gameSprite, scrollArea, building, spriteRowArray->count());
    spriteRowArray->addObject((CCObject*) sr);
}

