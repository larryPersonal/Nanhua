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
    
    mBuildingBuilderCurrent = 0;
    mBuildingBuilderLimit = 0;
    mBuildingMemberCurrent = 0;
    mBuildingPopulationLimit = 0;
    mBuildingUnitCurrent = 0;
    mBuildingUnitRequired = 0;
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
    mBuildingUnitCurrent = building->build_uint_current;
    mBuildingUnitRequired = building->build_uint_required;
    mBuildingPopulationLimit = building->populationLimit;
    mBuildingBuilderLimit = building->builderLimit;
    
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    ccColor3B colorWhite = ccc3(255, 255, 255);
    
    // Create constant menu items
    spriteBackground = CCSprite::create("worker_assign_ui_bg_03.png");
    spriteBackground->setScale(background_rect->width / spriteBackground->boundingBox().size.width * 1.4f);
    
    // Sprite
    spriteBuilding = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    spriteBuilding->setScale(256.0f / spriteBuilding->boundingBox().size.width);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCMenuItemImage::create("closebutton-up.png", "closebutton-down.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
    buttonClose->setScale(0.4);
    buttonClose->setTag(-1);
    
    menuItems->addObject(buttonClose);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    std::stringstream ss;
    
    ss << building->buildingName;
    
    labelBuildingName = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelBuildingName->setColor(colorWhite);

    ss.str(std::string());
    ss << building->build_uint_current << "/" << building->build_uint_required;
    
    progressBarLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    progressBarLabel->setColor(colorGreen);
    
    // sprite row header
    ss.str(std::string());
    ss << "Villager";
    
    // scroll section for other villagers
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(spriteBackground->boundingBox().size.width, spriteBackground->boundingBox().size.height - spriteBuilding->boundingBox().size.height - labelSRImage->boundingBox().size.height - labelSRMember->boundingBox().size.height - memberScrollArea->boundingBox().size.height - 190.0f), CCSizeMake(spriteBackground->boundingBox().size.width, spriteBackground->boundingBox().size.height - spriteBuilding->boundingBox().size.height - labelSRImage->boundingBox().size.height - labelSRMember->boundingBox().size.height - memberScrollArea->boundingBox().size.height - 190.0f));
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    scrollArea->hideScroll();
    
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    int index1 = 0;
    int index2 = 0;
    
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* sprite = (GameSprite*)spritesOnMap->objectAtIndex(i);
        
        

        // when the building is house, only refugee will be listed in the waiting home list.
        if(building->buildingType == HOUSING && building->build_uint_current >= building->build_uint_required)
        {
            if(sprite->getHome() == building)
            {
                SpriteRow* sr = SpriteRow::create(sprite, memberScrollArea, building, index1, true);
                memberSpriteRowArray->addObject((CCObject*) sr);
                index1++;
            }
            else if(sprite->spriteClass.compare("refugee") == 0)
            {
                SpriteRow* sr = SpriteRow::create(sprite, scrollArea, building, index2, false);
                spriteRowArray->addObject((CCObject*) sr);
                index2++;
            }
        }
        else if(building->buildingType == AMENITY && building->food_consumption_rate > 0 && building->build_uint_current >= building->build_uint_required)
        // when the building is granary, every villagers (includes refugee) is able to eat food in the building.
        {
            if(sprite->getTargetLocation() == building)
            {
                SpriteRow* sr = SpriteRow::create(sprite, memberScrollArea, building, index1, true);
                memberSpriteRowArray->addObject((CCObject*) sr);
                index1++;
            }
            else if(sprite->getPossessions()->currentHungry <= 20)
            {
                SpriteRow* sr = SpriteRow::create(sprite, scrollArea, building, index2, false);
                spriteRowArray->addObject((CCObject*) sr);
                index2++;
            }
        }
        else
        // when the builidng is those building that provide jobs, only citizens will be listed in the waiting home list
        {
            if(sprite->getJobLocation() == building)
            {
                SpriteRow* sr = SpriteRow::create(sprite, memberScrollArea, building, index1, true);
                memberSpriteRowArray->addObject((CCObject*) sr);
                index1++;
            }
            else if(sprite->spriteClass.compare("citizen") == 0)
            {
                SpriteRow* sr = SpriteRow::create(sprite, scrollArea, building, index2, false);
                spriteRowArray->addObject((CCObject*) sr);
                index2++;
            }
        }
    }
    
    memberScrollArea->updateScrollBars();
    scrollArea->updateScrollBars();
    
    // add children
    this->addChild(spriteBackground, 3);
    this->addChild(spriteBuilding, 6);
    this->addChild(menu, 3);
    
    //this->addChild(spInt);
    this->addChild(spLoy);
    //this->addChild(spSoc);
    this->addChild(spHap);
    
    this->addChild(labelBuildingName, 4);
    this->addChild(labelBuildingLevel);
    
    //this->addChild(labelTextInt);
    this->addChild(labelTextLoy);
    //this->addChild(labelTextSoc);
    this->addChild(labelTextHap);
    
    this->addChild(progressBar);
    this->addChild(progressBarLabel);
    
    // sprite row header
    this->addChild(labelSRImage);
    this->addChild(labelSRMember);
    
    this->addChild(memberScrollArea);
    this->addChild(scrollArea);
    
    this->addChild(memberLabel);
    this->addChild(builderLabel);
    
    // done creation
    spriteBuilding->setAnchorPoint(ccp(0.5, 0.5));
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    //spInt->setAnchorPoint(ccp(0, 1));
    spLoy->setAnchorPoint(ccp(0, 1));
    //spSoc->setAnchorPoint(ccp(0, 1));
    spHap->setAnchorPoint(ccp(0, 1));
    
    labelBuildingName->setAnchorPoint(ccp(0.5, 0.5));
    labelBuildingLevel->setAnchorPoint(ccp(0, 1));
    
    //labelTextInt->setAnchorPoint(ccp(0, 1));
    labelTextLoy->setAnchorPoint(ccp(0, 1));
    //labelTextSoc->setAnchorPoint(ccp(0, 1));
    labelTextHap->setAnchorPoint(ccp(0, 1));
    
    progressBar->setAnchorPoint(ccp(0, 1));
    progressBarLabel->setAnchorPoint(ccp(0, 1));
    
    // sprite row header
    labelSRImage->setAnchorPoint(ccp(0, 1));
    
    scrollArea->setAnchorPoint(ccp(0.5, 0.5));
    
    reposition();
    
    this->schedule(schedule_selector(SelectPopulation::update), 0.25f);
    
    /*
     * Organize the display of all elements
     */
    if(building->isUnderConstruction())
    {
        memberLabel->setVisible(false);
        builderLabel->setVisible(true);
    }
    else
    {
        memberLabel->setVisible(true);
        builderLabel->setVisible(false);
    }
}

void SelectPopulation::onMenuItemSelected(CCObject* pSender){
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


void SelectPopulation::reposition(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    
    spriteBuilding->setPosition(ccp(240, 70));
    
    // Anchored top right
    buttonClose->setPosition(halfWidth - 20.0f, halfHeight - 20.0f);
    
    //spInt->setPosition(ccp(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + 30.0f, halfHeight - 12.0f));
    spLoy->setPosition(ccp(-halfWidth + 480.0f, halfHeight - 12.0f));
    //spSoc->setPosition(ccp(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width +  100.0f, halfHeight - 12.0f));
    spHap->setPosition(ccp(-halfWidth + 480.0f, halfHeight - spLoy->boundingBox().size.height - 15.0f));
    
    
    labelBuildingName->CCNode::setPosition(240, -50);
    
    
    
    
    labelBuildingLevel->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + 25.0f, halfHeight - labelBuildingName->boundingBox().size.height - 15.0f);
    
    //labelTextInt->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width + 60.0f, halfHeight - 14.0f);
    labelTextLoy->CCNode::setPosition(-halfWidth + 560.0f, halfHeight - 14.0f);
    //labelTextSoc->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width + spSoc->boundingBox().size.width + 130.0f, halfHeight - 14.0f);
    labelTextHap->CCNode::setPosition(-halfWidth + 560.0f, halfHeight - spLoy->boundingBox().size.height - 17.0f);
    
    progressBar->CCNode::setPosition(-halfWidth + 250.0f, halfHeight - spriteBuilding->boundingBox().size.height / 4.0f + progressBar->boundingBox().size.height / 2.0f - 12.0f);
    progressBarLabel->CCNode::setPosition(-halfWidth + 360.0f, halfHeight - 14.0f);
    
    // member sprite row header
    labelSRMember->CCNode::setPosition(-halfWidth + 10.0f, halfHeight - spriteBuilding->boundingBox().size.height - 15.0f);
    memberLabel->CCNode::setPosition(-halfWidth + 140.0f, halfHeight - spriteBuilding->boundingBox().size.height - 15.0f);
    builderLabel->CCNode::setPosition(-halfWidth + 140.0f, halfHeight - spriteBuilding->boundingBox().size.height - 15.0f);
    
    // sprite row header
    labelSRImage->CCNode::setPosition(-halfWidth + 10.0f, halfHeight - spriteBuilding->boundingBox().size.height - 200.0f);
    
    
    
    // Scroll area in center
    scrollArea->CCNode::setPosition(-halfWidth,
                                    -halfHeight + 11.0f);
    memberScrollArea->CCNode::setPosition(-halfWidth, 10.0f);
    
    memberScrollArea->reposition();
    scrollArea->reposition();
}

void SelectPopulation::refreshAllMenuItemValues()
{
    // refresh all the sprite rows
    for(int i = 0; i < spriteRowArray->count(); i++)
    {
        ((SpriteRow*) spriteRowArray->objectAtIndex(i))->refreshAllMenuItems();
    }
    
    for(int i = 0; i < memberSpriteRowArray->count(); i++)
    {
        ((SpriteRow*) memberSpriteRowArray->objectAtIndex(i))->refreshAllMenuItems();
    }
    
    std::stringstream ss;
    // refresh other elements
    if(mBuildingUnitCurrent != building->build_uint_current || mBuildingUnitRequired != building->build_uint_required)
    {
        mBuildingUnitCurrent = building->build_uint_current;
        mBuildingUnitRequired = building->build_uint_required;
        ss.str(std::string());
        ss << building->build_uint_current << "/" << building->build_uint_required;
        progressBar->setValue((float) building->build_uint_current / (float) building->build_uint_required);
        progressBarLabel->setString(ss.str().c_str());
    }
    
    if(mBuildingMemberCurrent != memberSpriteRowArray->count() || mBuildingPopulationLimit != building->populationLimit)
    {
        mBuildingMemberCurrent = memberSpriteRowArray->count();
        mBuildingPopulationLimit = building->populationLimit;
        ss.str(std::string());
        ss << memberSpriteRowArray->count() << "/" << building->populationLimit;
        memberLabel->setString(ss.str().c_str());
    }
    
    if(mBuildingBuilderCurrent != memberSpriteRowArray->count() || mBuildingBuilderLimit != building->builderLimit)
    {
        mBuildingBuilderCurrent = memberSpriteRowArray->count();
        mBuildingBuilderLimit = building->builderLimit;
        ss.str(std::string());
        ss << memberSpriteRowArray->count() << "/" << building->builderLimit;
        builderLabel->setString(ss.str().c_str());
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
    
    SpriteRow* sr = SpriteRow::create(gameSprite, memberScrollArea, building, memberSpriteRowArray->count(), true);
    memberSpriteRowArray->addObject((CCObject*)sr);
    
}

void SelectPopulation::addVillagerRow(GameSprite * gameSprite, SpriteRow * spriteRow)
{
    memberSpriteRowArray->removeObject((CCObject*)spriteRow);
    
    for(int i = 0; i < memberSpriteRowArray->count(); i++)
    {
        ((SpriteRow*)(memberSpriteRowArray->objectAtIndex(i)))->rearrange(i);
    }
    
    SpriteRow* sr = SpriteRow::create(gameSprite, scrollArea, building, spriteRowArray->count(), false);
    spriteRowArray->addObject((CCObject*) sr);
}

