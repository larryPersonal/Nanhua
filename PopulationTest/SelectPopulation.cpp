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
#include "SpriteInfoMenu.h"
#include "BuildingInfoMenu.h"
#include "GlobalHelper.h"

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
    
    memberRowArray = CCArray::create();
    memberRowArray->retain();
    
    memberRowBackgroundArray = CCArray::create();
    memberRowBackgroundArray->retain();
    
    memberMenuArray = CCArray::create();
    memberMenuArray->retain();
    
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
    
    emptySpaceArray->removeAllObjects();
    CC_SAFE_RELEASE(emptySpaceArray);
    
    memberRowArray->removeAllObjects();
    CC_SAFE_RELEASE(memberRowArray);
    
    memberRowBackgroundArray->removeAllObjects();
    CC_SAFE_RELEASE(memberRowBackgroundArray);
    
    memberMenuArray->removeAllObjects();
    CC_SAFE_RELEASE(memberMenuArray);
}

void SelectPopulation::createMenuItems()
{
    if(!building->inProgress)
    {
       // CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        ccColor3B colorWhite = ccc3(255, 255, 255);
        
        // background
        spriteBackground = CCSprite::create("worker_assign_ui_bg_03.png");
        spriteBackground->setScale(background_rect->width / spriteBackground->boundingBox().size.width * 1.4f);
        spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
        this->addChild(spriteBackground, 3);
        
        // Sprite
        CCSprite* tempSprite = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
        spriteBuilding = CCMenuItemSprite::create(tempSprite, tempSprite, this, menu_selector(SelectPopulation::onMenuItemSelected));
        spriteBuilding->setScale(200.0f / spriteBuilding->boundingBox().size.width);
        spriteBuilding->setAnchorPoint(ccp(0.5, 0.5));
        spriteBuilding->setTag(-3);
        
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
        
        buttonCancel = CCMenuItemImage::create("cancel.png", "cancel.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
        buttonCancel->setScale(buttonClose->boundingBox().size.width / buttonCancel->boundingBox().size.width);
        buttonCancel->setTag(-4);
        buttonCancel->setAnchorPoint(ccp(1, 1));
        
        if (building->isCurrentConstructing || (!building->isCurrentConstructing && memberArray->count() > 0))
        {
            buttonOk->setVisible(false);
            buttonCancel->setVisible(true);
        }
        else
        {
            buttonOk->setVisible(true);
            buttonCancel->setVisible(false);
        }
        
        menuItems->addObject(buttonCancel);
        menuItems->addObject(buttonOk);
        menuItems->addObject(buttonClose);
        menuItems->addObject(spriteBuilding);
        
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
        
        ss.str(std::string());
        if(building->isUnderConstruction())
        {
            if(building->isCurrentConstructing)
            {
                ss << "Builders Working Currently";
            }
            else
            {
                ss << "Builders Available";
            }
        }
        else
        {
            ss << "Workers Available";
        }
        workerLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
        workerLabel->setColor(colorWhite);
        workerLabel->setAnchorPoint(ccp(0.5, 0.5));
        this->addChild(workerLabel, 4);
        
        ss.str(std::string());
        if(building->isUnderConstruction())
        {
            if(building->isCurrentConstructing)
            {
                ss << "Construction in progress";
            }
            else
            {
                ss << "Construction in prepare";
            }
        }
        else
        {
            ss << "Doing Task";
        }
        taskLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
        taskLabel->setColor(colorWhite);
        taskLabel->setAnchorPoint(ccp(0.5, 0.5));
        this->addChild(taskLabel, 4);
        
        int population = 0;
        if(building->isUnderConstruction())
        {
            population = building->builderLimit;
        }
        else
        {
            population = building->number_of_jobs;
        }
        
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
        
        // if the building is in preparing stage, list down all the available worker/builders. if the building is in working stage, list down all the members of the workers and builders.
        CCArray* spritesForSelection;
        if (building->isCurrentConstructing)
        {
            spritesForSelection = building->memberSpriteList;
        }
        else
        {
            spritesForSelection = GameScene::getThis()->spriteHandler->spritesOnMap;
        }
        
        int index = 0;
        int count = 0;
        for(int i = 0; i < spritesForSelection->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesForSelection->objectAtIndex(i);
            
            if(building->isCurrentConstructing || (!building->isCurrentConstructing && (gs->type == M_CITIZEN || gs->type == F_CITIZEN)))
            {
                SpriteRow* sp = SpriteRow::create((GameSprite*) spritesForSelection->objectAtIndex(i), scrollArea, building, index);
                spriteRowArray->addObject((CCObject*) sp);
                index++;
                count++;
            }
        }
        
        scrollArea->setScrollContentSize(CCSizeMake(450, 90.0f * count));
        scrollArea->updateScrollBars();
        
        // set the position of all the elements
        reposition();
        
        this->schedule(schedule_selector(SelectPopulation::update), 0.25f);
        
        CCArray* spritesShown = NULL;
        // if the building is in construction, set the member icons
        if (building->isCurrentConstructing)
        {
            spritesShown = spritesForSelection;
        }
        else
        {
            spritesShown = building->memberSpriteList;
        }
        
        for (int i = 0; i < spritesShown->count(); i++)
        {
            GameSprite* gameSprite = (GameSprite*) spritesShown->objectAtIndex(i);
            
            CCSprite* memberSpriteBackground = CCSprite::create("assign_menu_filled.png");
            memberSpriteBackground->setScale(70.0f / memberSpriteBackground->boundingBox().size.width);
            memberSpriteBackground->setAnchorPoint(ccp(0, 1));
            memberSpriteBackground->setPosition(ccp(30.0f + 70.0f * i, -135.0f));
            this->addChild(memberSpriteBackground, 4);
            
            std::string tempStr = gameSprite->spriteName;
            CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(SelectPopulation::cancelSprite));
            memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
            memberSprite->setAnchorPoint(ccp(0, 1));
            memberSprite->setPosition(ccp(35.0f + 70.0f * i, -137.0f));
            memberSprite->setTag(i);
            
            CCArray* newMenuItems = CCArray::create();
            newMenuItems->addObject(memberSprite);
            CCMenu* newMenu = CCMenu::createWithArray(newMenuItems);
            newMenu->setPosition(CCPointZero);
            this->addChild(newMenu, 4);
            
            memberMenuArray->addObject(newMenu);
            memberRowArray->addObject(memberSprite);
            memberArray->addObject(gameSprite);
            memberRowBackgroundArray->addObject(memberSpriteBackground);
        }
        
    }
}

void SelectPopulation::refreshUI()
{
    
}

void SelectPopulation::onMenuItemSelected(CCObject* pSender){
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case -1:
            // buttonClose
        {
            this->closeMenu(true);
            GameScene::getThis()->setTouchEnabled(true);
        }
            break;
        case -2:
            // button ok -> to construct
            performTask();
            break;
        case -3:
        {
            this->closeMenu(true);
            BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(building);
            buildingInfoMenu->useAsBasePopupMenu();
        }
            break;
        case -4:
        {
            // button cancel -> cancel the construction;
            cancelTask();
            break;
        }
        default:
        {
            break;
        }
    }
}

void SelectPopulation::cancelTask()
{
    CCArray* memberSprites = building->memberSpriteList;
    
    
    for (int i = 0; i < memberSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) memberSprites->objectAtIndex(i);
        gs->currAction = IDLE;
        gs->nextAction = IDLE;
        gs->setIsDoingJob(false);
        gs->spriteClass = "citizen";
        gs->setJob(NONE);
        gs->setJobLocation(NULL);
        gs->setTargetLocation(NULL);
        gs->changeToCitizen();
    }
    building->memberSpriteList->removeAllObjects();
    
    if(building->isUnderConstruction())
    {
        CCPoint tilePos = building->getWorldPosition();
        tilePos = GameScene::getThis()->mapHandler->tilePosFromLocation(tilePos);
        GameScene::getThis()->mapHandler->UnBuild(tilePos);
        this->closeMenu(true);
        GameScene::getThis()->setTouchEnabled(true);
    }
    building->isCurrentConstructing = false;
}

void SelectPopulation::performTask()
{
    if(building->isUnderConstruction())
    {
        // if the building is under construction -> construct the building.
        building->isCurrentConstructing = true;
        scheduleConstruction();
    }
    else
    {
        // if the building is a guard tower -> assign a sprite to become sodier!!!
        if (building->buildingType == MILITARY)
        {
            scheduleGuardTower();
        }
        else if(building->buildingType == AMENITY)
        {
            scheduleFarming();
        }
    }
    this->closeMenu(true);
    GameScene::getThis()->setTouchEnabled(true);
}

void SelectPopulation::prepareJob(GameSprite* gameSprite)
{
    gameSprite->currAction = WALKING;
    gameSprite->setJobLocation(building);
    gameSprite->setTargetLocation(building);
    gameSprite->GoBuilding(building);
    
    gameSprite->futureAction1 = EATING;
    gameSprite->futureAction2 = RESTING;
    
    building->memberSpriteList->addObject(gameSprite);
}

void SelectPopulation::scheduleGuardTower()
{
    for (int i = 0; i < memberArray->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(i);
        
        gameSprite->ChangeSpriteTo(GlobalHelper::getSpriteType(gameSprite, M_SOLDIER, F_SOLDIER));
        
        prepareJob(gameSprite);
        
        gameSprite->nextAction = GUARD;
        
        gameSprite->setJob(SOLDIER);
    }
}

void SelectPopulation::scheduleConstruction()
{
    for (int i = 0; i < memberArray->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(i);
        
        gameSprite->ChangeSpriteTo(GlobalHelper::getSpriteType(gameSprite, M_BUILDER, F_BUILDER));
        
        prepareJob(gameSprite);
        
        gameSprite->nextAction = BUILD;
        
        
        
        gameSprite->setJob(BUILDER);
    }

}

void SelectPopulation::scheduleFarming()
{
    for (int i = 0; i < memberArray->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(i);
        
        prepareJob(gameSprite);
        
        gameSprite->nextAction = FARMING;
        
        gameSprite->ChangeSpriteTo(GlobalHelper::getSpriteType(gameSprite, M_FARMER, F_FARMER));
        
        gameSprite->setJob(FARMER);
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
    buttonCancel->setPosition(halfWidth - 80.0f, -halfHeight + 75.0f);
    
    labelBuildingName->CCNode::setPosition(285.0f, -100.0f);
    
    workerLabel->setPosition(ccp(-halfWidth / 2.0f + 40.0f, halfHeight - 40.0f));
    taskLabel->setPosition(ccp(halfWidth / 2.0f + 40.0f, halfHeight - 40.0f));
    
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

void SelectPopulation::selectSprite(GameSprite* gameSprite, SpriteRow* spriteRow)
{
    if(building->isCurrentConstructing)
    {
        return;
    }
    
    int limit = 0;
    if(building->isUnderConstruction())
    {
        limit = building->builderLimit;
    }
    else
    {
        limit = building->number_of_jobs;
    }
    
    if(memberArray->count() < limit)
    {
        spriteRow->getMask()->setVisible(true);
        
        CCSprite* memberSpriteBackground = CCSprite::create("assign_menu_filled.png");
        memberSpriteBackground->setScale(70.0f / memberSpriteBackground->boundingBox().size.width);
        memberSpriteBackground->setAnchorPoint(ccp(0, 1));
        memberSpriteBackground->setPosition(ccp(30.0f + 70.0f * memberRowArray->count(), -135.0f));
        this->addChild(memberSpriteBackground, 4);
        
        std::string tempStr = gameSprite->spriteName;
        CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(SelectPopulation::cancelSprite));
        memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
        memberSprite->setAnchorPoint(ccp(0, 1));
        memberSprite->setPosition(ccp(35.0f + 70.0f * memberRowArray->count(), -137.0f));
        memberSprite->setTag(memberRowArray->count());
        
        CCArray* newMenuItems = CCArray::create();
        newMenuItems->addObject(memberSprite);
        CCMenu* newMenu = CCMenu::createWithArray(newMenuItems);
        newMenu->setPosition(CCPointZero);
        this->addChild(newMenu, 4);
        
        memberMenuArray->addObject(newMenu);
        memberRowArray->addObject(memberSprite);
        memberArray->addObject(gameSprite);
        memberRowBackgroundArray->addObject(memberSpriteBackground);
    }
}

void SelectPopulation::unselectSprite(GameSprite* gameSprite, SpriteRow* spriteRow)
{
    for(int i = 0; i < memberArray->count(); i++)
    {
        if(gameSprite == (GameSprite*) memberArray->objectAtIndex(i))
        {
            spriteRow->getMask()->setVisible(false);
            
            memberArray->removeObject(gameSprite);
            
            CCMenuItemImage* temp = (CCMenuItemImage*) memberRowArray->objectAtIndex(i);
            memberRowArray->removeObject(temp);
            
            CCSprite* tempSprite = (CCSprite*) memberRowBackgroundArray->objectAtIndex(i);
            this->removeChild(tempSprite);
            memberRowBackgroundArray->removeObject(tempSprite);
            
            CCMenu* tempMenu = (CCMenu*) memberMenuArray->objectAtIndex(i);
            this->removeChild(tempMenu);
            memberMenuArray->removeObject(tempMenu);
            
            i--;
            
            for(int j = 0; j < memberRowArray->count(); j++)
            {
                CCMenuItemImage* tempSprite = (CCMenuItemImage*) memberRowArray->objectAtIndex(j);
                tempSprite->setPosition(ccp(35.0f + 70.0f * j, -137.0f));
                tempSprite->setTag(j);
            }
            
            for(int j = 0; j < memberRowBackgroundArray->count(); j++)
            {
                CCSprite* tempSprite = (CCSprite*) memberRowBackgroundArray->objectAtIndex(j);
                tempSprite->setPosition(ccp(30.0f + 70.0f * j, -135.0f));
            }
        }
    }
}

void SelectPopulation::cancelSprite(CCObject *pSender)
{
    if(building->isCurrentConstructing)
    {
        return;
    }
    
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = pMenuItem->getTag();
    
    GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(tag);
    
    this->closeMenu(true);
    
    SpriteInfoMenu* spriteInfoMenu = new SpriteInfoMenu(gameSprite);
    spriteInfoMenu->autorelease();
    spriteInfoMenu->useAsBasePopupMenu();
    
}

