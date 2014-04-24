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
#include "TutorialManager.h"

SelectPopulation* SelectPopulation::SP;

SelectPopulation* SelectPopulation::create(Building* building){
    SelectPopulation *pRet = new SelectPopulation(building);
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
    
    population = 0;
    isPerformingTask = false;
    
    isSorted = false;
    happinessIncre = false;
    energyIncre = false;
    isSortedByHappiness = false;
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
    // CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorWhite = ccc3(255, 255, 255);
    
    // background
    spriteBackground = CCSprite::create("SelectPopulationUI.png");
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
    
    buttonClose = CCSprite::create("Closebtn_Sq.png");
    buttonClose->setAnchorPoint(ccp(1, 1));
    this->addChild(buttonClose, 4);
    
    buttonOk = CCMenuItemImage::create("assign_menu_accept.png", "assign_menu_accept.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
    buttonOk->setScale(0.4);
    buttonOk->setTag(-2);
    buttonOk->setAnchorPoint(ccp(1, 1));
    
    buttonCancel = CCMenuItemImage::create("Building Info UI placeholdercircleclose.png", "Building Info UI placeholdercircleclose.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
    //buttonCancel->setScale(buttonClose->boundingBox().size.width / buttonCancel->boundingBox().size.width);
    buttonCancel->setScale(buttonOk->boundingBox().size.width / buttonCancel->boundingBox().size.width);
    buttonCancel->setTag(-4);
    buttonCancel->setAnchorPoint(ccp(1, 1));
    
    buttonOk->setVisible(false);
    
    // building name label
    std::stringstream ss;
    ss << building->buildingName;
    
    labelBuildingName = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelBuildingName->setColor(colorWhite);
    labelBuildingName->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(labelBuildingName, 4);
    
    isCurrentlyUnderConstruction = building->isCurrentConstructing;
    isPerformingTask = building->isCurrentWorking;
    
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
    
    isUnderConstruction = building->isUnderConstruction();
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
    
    population = 0;
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
    scrollArea->createScrollArea(CCSizeMake(400, 240), CCSizeMake(400, 240));
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    scrollArea->hideScroll();
    scrollArea->setAnchorPoint(ccp(0, 1));
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
        
        if(building->isCurrentConstructing || (!building->isCurrentConstructing && gs->villagerClass == V_CITIZEN))
        {
            SpriteRow* sp = SpriteRow::create((GameSprite*) spritesForSelection->objectAtIndex(i), scrollArea, building, index);
            spriteRowArray->addObject((CCObject*) sp);
            index++;
            count++;
        }
    }
    num_of_citizens = count;
    
    int cnt = 0;
    if(count > 0)
    {
        if(count % 4 == 0)
        {
            cnt = count / 4;
        }
        else
        {
            cnt = count / 4 + 1;
        }
    }
    
    scrollArea->setScrollContentSize(CCSizeMake(400, 10.0f + 100.0f * cnt));
    if(10.0f + 100.0f * cnt > 240)
    {
        scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    }
    scrollArea->updateScrollBars();
    
    sortButton = CCMenuItemImage::create("sortbtn.png", "sortpressbtn.png", NULL, this, NULL, menu_selector(SelectPopulation::clickSortButton));
    sortButton->setScale(0.6f);
    sortButton->setAnchorPoint(ccp(0, 1));
    sortButton->setPosition(ccp(75, 515));
    
    sortHappinessButton = CCMenuItemImage::create("sortby_happinessbtn.png", "sortby_happiness_pressbtn.png", NULL, this, NULL, menu_selector(SelectPopulation::clickSortHappinessButton));
    sortHappinessButton->setScale(0.6f);
    sortHappinessButton->setAnchorPoint(ccp(0, 1));
    sortHappinessButton->setPosition(ccp(200, 515));
    
    menuItems->addObject(buttonCancel);
    menuItems->addObject(buttonOk);
    menuItems->addObject(spriteBuilding);
    menuItems->addObject(sortButton);
    menuItems->addObject(sortHappinessButton);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 3);
    
    // set the position of all the elements
    reposition();
    
    this->schedule(schedule_selector(SelectPopulation::update), 0.06f);
    
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
        memberSpriteBackground->setPosition(ccp(45.0f + 70.0f * i, -105.0f));
        this->addChild(memberSpriteBackground, 4);
        
        std::string tempStr = gameSprite->spriteName;
        CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(SelectPopulation::cancelSprite));
        memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
        memberSprite->setAnchorPoint(ccp(0, 1));
        memberSprite->setPosition(ccp(50.0f + 70.0f * i, -107.0f));
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
            if(!TutorialManager::getThis()->lockButtonClose)
            {
                this->closeMenu(true);
            }
        }
            break;
        case -2:
            // button ok -> to construct
            if(!TutorialManager::getThis()->lockButtonOk)
            {
                if(TutorialManager::getThis()->active && (TutorialManager::getThis()->teachBuildHouse || TutorialManager::getThis()->teachFarming) && TutorialManager::getThis()->miniDragon != NULL)
                {
                    TutorialManager::getThis()->miniDragon->clickNext();
                }
                performTask();
            }
            break;
        case -3:
        {
            if(!TutorialManager::getThis()->lockBuildingInfo)
            {
                PopupMenu::backupCurrentPopupMenu();
                BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(building);
                buildingInfoMenu->useAsTopmostPopupMenu();
            }
        }
            break;
        case -4:
        {
            // button cancel -> cancel the construction;
            if(!TutorialManager::getThis()->lockButtonCancel)
            {
                cancelTask();
            }
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
    building->isCurrentWorking = false;
}

void SelectPopulation::performTask()
{
    if(memberArray->count() <= 0)
    {
        return;
    }
    
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
    gameSprite->path->removeAllObjects();
    gameSprite->GoBuilding(building);
    
    gameSprite->futureAction1 = EATING;
    gameSprite->futureAction2 = RESTING;
    
    building->memberSpriteList->addObject(gameSprite);
}

/*
 * all scheduling functions
 */
void SelectPopulation::scheduleGuardTower()
{
    for (int i = 0; i < memberArray->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(i);
        
        // must change sprite first because the information for assigning will be lost if change sprite at last.
        //gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_SOLDIER));
        
        gameSprite->nextAction = GUARD;
        
        gameSprite->setJob(SOLDIER);
        
        building->isCurrentWorking = true;
        
        prepareJob(gameSprite);
        
        gameSprite->saySpeech(GUARD_EMOTION, 5.0f);
    }
}

void SelectPopulation::scheduleConstruction()
{
    for (int i = 0; i < memberArray->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(i);
        
        // must change sprite first because the information for assigning will be lost if change sprite at last.
        gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_BUILDER));
        
        gameSprite->nextAction = BUILD;
        
        gameSprite->setJob(BUILDER);
        
        building->isCurrentConstructing = true;
        
        prepareJob(gameSprite);
        
        gameSprite->saySpeech(BUILDER_EMOTION, 5.0f);
    }
}

void SelectPopulation::scheduleFarming()
{
    building->farmState = FARM;
    for (int i = 0; i < memberArray->count(); i++)
    {
        GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(i);
        
        // must change sprite first because the information for assigning will be lost if change sprite at last.
        gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_FARMER));
        
        gameSprite->nextAction = FARMING;
        
        gameSprite->setJob(FARMER);
        
        building->isCurrentWorking = true;
        
        prepareJob(gameSprite);
        
        gameSprite->saySpeech(FARMER_EMOTION, 5.0f);
    }
}


void SelectPopulation::reposition(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
    spriteBackground->setPosition(ccp(hw, hh));
    
    spriteBuilding->setPosition(ccp(240.0f + hw, 40.0f + hh));
    
    // Anchored top right
    buttonClose->setPosition(ccp(halfWidth - 50.0f + hw, halfHeight -25.0f + hh));
    
    buttonOk->setPosition(halfWidth - 125.0f + hw, -halfHeight + 80.0f + hh);
    buttonCancel->setPosition(halfWidth - 70.0f + hw, -halfHeight + 75.0f + hh);
    
    labelBuildingName->CCNode::setPosition(285.0f + hw, -100.0f + hh);
    
    workerLabel->setPosition(ccp(-halfWidth / 2.0f + 40.0f + hw, halfHeight - 40.0f + hh));
    taskLabel->setPosition(ccp(halfWidth / 2.0f + 40.0f + hw, halfHeight - 40.0f + hh));
    
    // for empty space
    for (int i = 0; i < emptySpaceArray->count(); i++)
    {
        ((CCSprite*) emptySpaceArray->objectAtIndex(i))->setPosition(ccp(45.0f + 70.0f * i + hw, -105.0f + hh));
    }
    
    // Scroll area in center
    scrollArea->CCNode::setPosition(-halfWidth + 45.0f + hw, hh / 4.0f + 110.0f);
    scrollArea->reposition();
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
}

void SelectPopulation::refreshAllMenuItemValues()
{
    // refresh all the sprite rows
    for(int i = 0; i < spriteRowArray->count(); i++)
    {
        ((SpriteRow*) spriteRowArray->objectAtIndex(i))->refreshAllMenuItems();
    }
    
    /*
     * if the number of assigned villages meet the requirement of the job provided by that building, show the confirm (ok) button.
     */
    
}

void SelectPopulation::willChangeOrientation(){
    
}

void SelectPopulation::onOrientationChanged(){
    
}

void SelectPopulation::update(float deltaTime){
    refreshAllMenuItemValues();
    
    if(isUnderConstruction != building->isUnderConstruction())
    {
        isUnderConstruction = building->isUnderConstruction();
        population = building->number_of_jobs;
        
        memberArray->removeAllObjects();
        memberMenuArray->removeAllObjects();
        memberRowArray->removeAllObjects();
        memberRowBackgroundArray->removeAllObjects();
        emptySpaceArray->removeAllObjects();
        
        for (int i = 0; i < population; i++)
        {
            CCSprite* eSpace = CCSprite::create("assign_menu_unfilled.png");
            eSpace->setScale(70.0f / eSpace->boundingBox().size.width);
            eSpace->setAnchorPoint(ccp(0, 1));
            this->addChild(eSpace, 4);
            emptySpaceArray->addObject(eSpace);
        }
        
        stringstream ss;
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
        workerLabel->setString(ss.str().c_str());
        
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
        taskLabel->setString(ss.str().c_str());
    }
    
    int pp = 0;
    if(building->isUnderConstruction())
    {
        pp = building->builderLimit;
    }
    else
    {
        pp = building->number_of_jobs;
    }
    
    if(population != pp)
    {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        float hw = screenSize.width / 2.0f;
        float hh = screenSize.height / 2.0f;
        
        population = pp;
        for(int i = 0; i < emptySpaceArray->count(); i++)
        {
            CCSprite* tempSp = (CCSprite*) emptySpaceArray->objectAtIndex(i);
            this->removeChild(tempSp);
        }
        emptySpaceArray->removeAllObjects();
        
        for (int i = 0; i < population; i++)
        {
            CCSprite* eSpace = CCSprite::create("assign_menu_unfilled.png");
            eSpace->setScale(70.0f / eSpace->boundingBox().size.width);
            eSpace->setAnchorPoint(ccp(0, 1));
            eSpace->setPosition(ccp(45.0f + 70.0f * i + hw, -105.0f + hh));
            this->addChild(eSpace, 4);
            emptySpaceArray->addObject(eSpace);
        }
    }
    
    if(isCurrentlyUnderConstruction != building->isCurrentConstructing)
    {
        stringstream ss;
        if(building->isCurrentConstructing)
        {
            ss << "Builders Working Currently";
        }
        else
        {
            ss << "Builders Available";
        }
        workerLabel->setString(ss.str().c_str());
        
        ss.str(std::string());
        if(building->isCurrentConstructing)
        {
            ss << "Construction in progress";
        }
        else
        {
            ss << "Construction in prepare";
        }
        taskLabel->setString(ss.str().c_str());
        
        this->closeMenu(true);
    }
    
    
    if(memberArray->count() == population && !buttonOk->isVisible() && memberArray->count() > 0 && !building->isCurrentConstructing && !building->isCurrentWorking)
    {
        buttonOk->setVisible(true);
        if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachBuildHouse)
        {
            TutorialManager::getThis()->miniDragon->clickNext();
        }
    }
    else if(memberArray->count() < population && buttonOk->isVisible())
    {
        buttonOk->setVisible(false);
    }
    
    // if the building is in preparing stage, list down all the available worker/builders. if the building is in working stage, list down all the members of the workers and builders.
    CCArray* spritesForSelection = getSpriteList();
    
    int count = 0;
    int index = 0;
    
    for(int i = 0; i < spritesForSelection->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesForSelection->objectAtIndex(i);
        
        if(building->isCurrentConstructing || (!building->isCurrentConstructing && gs->villagerClass == V_CITIZEN))
        {
            count++;
        }
    }
    
    if(num_of_citizens != count)
    {
        num_of_citizens = count;
        for(int i = 0; i < spriteRowArray->count(); i++)
        {
            SpriteRow* sr = (SpriteRow*)spriteRowArray->objectAtIndex(i);
            sr->unlinkChildren();
        }
        spriteRowArray->removeAllObjects();
        
        count = 0;
        index = 0;
        for(int i = 0; i < spritesForSelection->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesForSelection->objectAtIndex(i);
            
            if(building->isCurrentConstructing || (!building->isCurrentConstructing && gs->villagerClass == V_CITIZEN))
            {
                SpriteRow* sp = SpriteRow::create((GameSprite*) spritesForSelection->objectAtIndex(i), scrollArea, building, index);
                spriteRowArray->addObject((CCObject*) sp);
                index++;
                count++;
                
                bool flag = false;
                for(int j = 0; j < memberArray->count(); j++)
                {
                    GameSprite* gameS = (GameSprite*) memberArray->objectAtIndex(j);
                    if(gameS == gs)
                    {
                        flag = true;
                        break;
                    }
                }
                
                if(flag)
                {
                    sp->getMask()->setVisible(true);
                }
            }
        }
        
        int cnt = 0;
        if(count > 0)
        {
            if(count % 4 == 0)
            {
                cnt = count / 4;
            }
            else
            {
                cnt = count / 4 + 1;
            }
        }
        
        scrollArea->setScrollContentSize(CCSizeMake(400, 10.0f + 100.0f * cnt));
        if(10.0f + 100.0f * cnt > 240)
        {
            scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
        }
        scrollArea->updateScrollBars();
    }
    
    spritesForSelection->removeAllObjects();
    CC_SAFE_RELEASE(spritesForSelection);
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
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockManpowerSelect)
    {
        return;
    }
    
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
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
    if(memberArray->count() < limit)
    {
        spriteRow->getMask()->setVisible(true);
        
        CCSprite* memberSpriteBackground = CCSprite::create("assign_menu_filled.png");
        memberSpriteBackground->setScale(70.0f / memberSpriteBackground->boundingBox().size.width);
        memberSpriteBackground->setAnchorPoint(ccp(0, 1));
        memberSpriteBackground->setPosition(ccp(45.0f + 70.0f * memberRowArray->count() + hw, -105.0f + hh));
        this->addChild(memberSpriteBackground, 4);
        
        std::string tempStr = gameSprite->spriteName;
        CCMenuItemImage* memberSprite = CCMenuItemImage::create(tempStr.append("_port.png").c_str(), tempStr.c_str(), this,  menu_selector(SelectPopulation::cancelSprite));
        memberSprite->setScale( 60.0f / memberSprite->boundingBox().size.width );
        memberSprite->setAnchorPoint(ccp(0, 1));
        memberSprite->setPosition(ccp(50.0f + 70.0f * memberRowArray->count() + hw, -107.0f + hh));
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
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockManpowerSelect)
    {
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float hw = screenSize.width / 2.0f;
    float hh = screenSize.height / 2.0f;
    
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
                tempSprite->setPosition(ccp(50.0f + 70.0f * j + hw, -107.0f + hh));
                tempSprite->setTag(j);
            }
            
            for(int j = 0; j < memberRowBackgroundArray->count(); j++)
            {
                CCSprite* tempSprite = (CCSprite*) memberRowBackgroundArray->objectAtIndex(j);
                tempSprite->setPosition(ccp(45.0f + 70.0f * j + hw, -105.0f + hh));
            }
        }
    }
}

void SelectPopulation::cancelSprite(CCObject *pSender)
{
    if(TutorialManager::getThis()->active && TutorialManager::getThis()->lockSpriteInfo)
    {
        return;
    }
    
    if(building->isCurrentConstructing)
    {
        return;
    }
    
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = pMenuItem->getTag();
    
    GameSprite* gameSprite = (GameSprite*) memberArray->objectAtIndex(tag);
    
    PopupMenu::backupCurrentPopupMenu();
    SpriteInfoMenu* spriteInfoMenu = SpriteInfoMenu::create(gameSprite);
    spriteInfoMenu->useAsTopmostPopupMenu();
    
}

void SelectPopulation::adjustZIndex(bool tutorial)
{
    if(tutorial)
    {
        scrollArea->setZOrder(100);
    }
    else
    {
        scrollArea->setZOrder(4);
    }
}

void SelectPopulation::clickSortButton()
{
    isSorted = true;
    isSortedByHappiness = false;
    energyIncre = !energyIncre;
    
    // if the building is in preparing stage, list down all the available worker/builders. if the building is in working stage, list down all the members of the workers and builders.
    CCArray* spritesForSelection = getSpriteList();
    
    int count = 0;
    int index = 0;
    
    for(int i = 0; i < spriteRowArray->count(); i++)
    {
        SpriteRow* sr = (SpriteRow*)spriteRowArray->objectAtIndex(i);
        sr->unlinkChildren();
    }
    spriteRowArray->removeAllObjects();
    
    count = 0;
    index = 0;
    for(int i = 0; i < spritesForSelection->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesForSelection->objectAtIndex(i);
        
        if(building->isCurrentConstructing || (!building->isCurrentConstructing && gs->villagerClass == V_CITIZEN))
        {
            SpriteRow* sp = SpriteRow::create((GameSprite*) spritesForSelection->objectAtIndex(i), scrollArea, building, index);
            spriteRowArray->addObject((CCObject*) sp);
            index++;
            count++;
            
            bool flag = false;
            for(int j = 0; j < memberArray->count(); j++)
            {
                GameSprite* gameS = (GameSprite*) memberArray->objectAtIndex(j);
                if(gameS == gs)
                {
                    flag = true;
                    break;
                }
            }
            
            if(flag)
            {
                sp->getMask()->setVisible(true);
            }
        }
    }
    
    spritesForSelection->removeAllObjects();
    CC_SAFE_RELEASE(spritesForSelection);
}

void SelectPopulation::clickSortHappinessButton()
{
    isSorted = true;
    isSortedByHappiness = true;
    happinessIncre = !happinessIncre;
    
    // if the building is in preparing stage, list down all the available worker/builders. if the building is in working stage, list down all the members of the workers and builders.
    CCArray* spritesForSelection = getSpriteList();
    
    int count = 0;
    int index = 0;
    
    for(int i = 0; i < spriteRowArray->count(); i++)
    {
        SpriteRow* sr = (SpriteRow*)spriteRowArray->objectAtIndex(i);
        sr->unlinkChildren();
    }
    spriteRowArray->removeAllObjects();
    
    count = 0;
    index = 0;
    for(int i = 0; i < spritesForSelection->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesForSelection->objectAtIndex(i);
        
        if(building->isCurrentConstructing || (!building->isCurrentConstructing && gs->villagerClass == V_CITIZEN))
        {
            SpriteRow* sp = SpriteRow::create((GameSprite*) spritesForSelection->objectAtIndex(i), scrollArea, building, index);
            spriteRowArray->addObject((CCObject*) sp);
            index++;
            count++;
            
            bool flag = false;
            for(int j = 0; j < memberArray->count(); j++)
            {
                GameSprite* gameS = (GameSprite*) memberArray->objectAtIndex(j);
                if(gameS == gs)
                {
                    flag = true;
                    break;
                }
            }
            
            if(flag)
            {
                sp->getMask()->setVisible(true);
            }
        }
    }
    
    spritesForSelection->removeAllObjects();
    CC_SAFE_RELEASE(spritesForSelection);
}

CCArray* SelectPopulation::getSpriteList()
{
    if(isSorted && isSortedByHappiness)
    {
        return sortByHappiness();
    }
    else
    {
        return sortByEnergy();
    }
}

CCArray* SelectPopulation::sortByHappiness()
{
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
    
    CCArray* unsortedArray = CCArray::create();
    unsortedArray->retain();
    
    for(int i = 0; i < spritesForSelection->count(); i++)
    {
        unsortedArray->addObject(spritesForSelection->objectAtIndex(i));
    }
    
    if(isSorted)
    {
        CCArray* sortedArray = CCArray::create();
        sortedArray->retain();
        
        while (unsortedArray->count() > 0)
        {
            GameSprite* gs = getSpriteWithHappiness(unsortedArray, happinessIncre);
            sortedArray->addObject(gs);
            unsortedArray->removeObject(gs);
        }
        
        unsortedArray->removeAllObjects();
        CC_SAFE_RELEASE(unsortedArray);
        return sortedArray;
    }
    else
    {
        return unsortedArray;
    }
}

CCArray* SelectPopulation::sortByEnergy()
{
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
    
    CCArray* unsortedArray = CCArray::create();
    unsortedArray->retain();
    
    for(int i = 0; i < spritesForSelection->count(); i++)
    {
        unsortedArray->addObject(spritesForSelection->objectAtIndex(i));
    }
    
    if(isSorted)
    {
        CCArray* sortedArray = CCArray::create();
        sortedArray->retain();
        
        while (unsortedArray->count() > 0)
        {
            GameSprite* gs = getSpriteWithEnergy(unsortedArray, energyIncre);
            sortedArray->addObject(gs);
            unsortedArray->removeObject(gs);
        }
        
        unsortedArray->removeAllObjects();
        CC_SAFE_RELEASE(unsortedArray);
        return sortedArray;
    }
    else
    {
        return unsortedArray;
    }
}

GameSprite* SelectPopulation::getSpriteWithEnergy(CCArray* spriteList, bool fromMin)
{
    if(spriteList->count() <= 0)
    {
        return NULL;
    }
    GameSprite* temp;
    if(fromMin)
    {
        temp = (GameSprite*)spriteList->objectAtIndex(0);
        for(int i = 0; i < spriteList->count(); i++)
        {
            GameSprite* gs = (GameSprite*)spriteList->objectAtIndex(i);
            if(gs->getPossessions()->energyRating < temp->getPossessions()->energyRating)
            {
                temp = gs;
            }
        }
    }
    else
    {
        temp = (GameSprite*)spriteList->objectAtIndex(spriteList->count() - 1);
        for(int i = spriteList->count() - 1; i >= 0; i--)
        {
            GameSprite* gs = (GameSprite*)spriteList->objectAtIndex(i);
            if(gs->getPossessions()->energyRating > temp->getPossessions()->energyRating)
            {
                temp = gs;
            }
        }
    }
    return temp;
}

GameSprite* SelectPopulation::getSpriteWithHappiness(CCArray* spriteList, bool fromMin)
{
    if(spriteList->count() <= 0)
    {
        return NULL;
    }
    GameSprite* temp = (GameSprite*)spriteList->objectAtIndex(0);
    
    for(int i = 0; i < spriteList->count(); i++)
    {
        GameSprite* gs = (GameSprite*)spriteList->objectAtIndex(i);
        if(fromMin)
        {
            if(gs->getPossessions()->happinessRating < temp->getPossessions()->happinessRating)
            {
                temp = gs;
            }
        }
        else
        {
            if(gs->getPossessions()->happinessRating > temp->getPossessions()->happinessRating)
            {
                temp = gs;
            }
        }
    }
    return temp;
}
