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
#include "SpriteRow.h"

SelectPopulation* SelectPopulation::create(Building* building){
    SelectPopulation *pRet = new SelectPopulation(building);
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

void SelectPopulation::cleanup(){
    PopupMenu::cleanup();
}

SelectPopulation::SelectPopulation(Building* building){
    if(!building){
        CCLog("Warning No Building!");
    }
    spriteRowArray = CCArray::create();
    spriteRowArray->retain();
    
    this->building = building;
    
    // create gui handler
    background_rect = new Rect();
    
    // initialize the gui handler
    background_rect->ini(700, 100, 100, 200);
}

SelectPopulation::~SelectPopulation()
{
    /*
     for (int i = 0; i < allSprites->count(); ++i)
     {
     spriteRowArray->objectAtIndex(i)->release();
     }*/
    spriteRowArray->removeAllObjects();
    CC_SAFE_RELEASE(spriteRowArray);
}

void SelectPopulation::createMenuItems()
{
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    
    // Create constant menu items
    spriteBackground = CCSprite::create("outerbox.png");
    spriteBackground->setScale(background_rect->width / spriteBackground->boundingBox().size.width);
    
    // Sprite
    spriteBuilding = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    spriteBuilding->setScale(64.0f / spriteBuilding->boundingBox().size.width);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCMenuItemImage::create("closebutton-up.png", "closebutton-down.png", this, menu_selector(SelectPopulation::onMenuItemSelected));
    buttonClose->setScale(0.4);
    buttonClose->setTag(-1);
    
    spInt = CCSprite::create("intelligence icon.png");
    spInt->setScale(30.0f / spInt->boundingBox().size.width);
    spLoy = CCSprite::create("loyalty icon.png");
    spLoy->setScale(30.0f / spLoy->boundingBox().size.width);
    spSoc = CCSprite::create("social icon.png");
    spSoc->setScale(30.0f / spSoc->boundingBox().size.width);
    spHap = CCSprite::create("happiness icon.png");
    spHap->setScale(30.0f / spHap->boundingBox().size.width);
    
    menuItems->addObject(buttonClose);
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    std::stringstream ss;
    
    ss << building->buildingName;
    
    labelBuildingName = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelBuildingName->setColor(colorGreen);
    
    ss.str(std::string());
    ss << "Level: 1";
    
    labelBuildingLevel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelBuildingLevel->setColor(colorGreen);
    
    // stats labels
    ss.str(std::string());
    ss << "0";
    
    labelTextInt = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelTextInt->setColor(colorGreen);
    
    ss.str(std::string());
    ss << "0";
    
    labelTextLoy = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelTextLoy->setColor(colorGreen);
    
    ss.str(std::string());
    ss << "0";
    
    labelTextSoc = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelTextSoc->setColor(colorGreen);
    
    ss.str(std::string());
    ss << "0";
    
    labelTextHap = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelTextHap->setColor(colorGreen);
    
    ss.str(std::string());
    ss << "Villager";
    
    // sprite row header
    labelSRImage = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    labelSRImage->setColor(colorBlack);
    
    // scroll section
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(spriteBackground->boundingBox().size.width, spriteBackground->boundingBox().size.height - spriteBuilding->boundingBox().size.height - labelSRImage->boundingBox().size.height - 18.0f), CCSizeMake(spriteBackground->boundingBox().size.width, spriteBackground->boundingBox().size.height - spriteBuilding->boundingBox().size.height - labelSRImage->boundingBox().size.height - 18.0f));
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    scrollArea->hideScroll();
    
    customSprite = CCSprite::create("blue square.png");
    customSprite->setScale(64.0f / customSprite->boundingBox().size.width);
    customSprite->setAnchorPoint(ccp(0.5, 0.5));
    
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* sprite = (GameSprite*)spritesOnMap->objectAtIndex(i);
        
        SpriteRow* sp = SpriteRow::create(sprite, scrollArea, i);
        spriteRowArray->addObject((CCObject*) sp);
    }
    
    scrollArea->addItem(customSprite, ccp(spriteBackground->boundingBox().size.width / 2.0f, spriteBackground->boundingBox().size.height / 2.0f));
    
    scrollArea->updateScrollBars();
    
    // add children
    this->addChild(spriteBackground);
    this->addChild(spriteBuilding, 3);
    this->addChild(menu, 3);
    
    this->addChild(spInt);
    this->addChild(spLoy);
    this->addChild(spSoc);
    this->addChild(spHap);
    
    this->addChild(labelBuildingName);
    this->addChild(labelBuildingLevel);
    
    this->addChild(labelTextInt);
    this->addChild(labelTextLoy);
    this->addChild(labelTextSoc);
    this->addChild(labelTextHap);
    
    // sprite row header
    this->addChild(labelSRImage);
    
    this->addChild(scrollArea);
    
    // done creation
    spriteBuilding->setAnchorPoint(ccp(0, 1));
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    spInt->setAnchorPoint(ccp(0, 1));
    spLoy->setAnchorPoint(ccp(0, 1));
    spSoc->setAnchorPoint(ccp(0, 1));
    spHap->setAnchorPoint(ccp(0, 1));
    
    labelBuildingName->setAnchorPoint(ccp(0, 1));
    labelBuildingLevel->setAnchorPoint(ccp(0, 1));
    
    labelTextInt->setAnchorPoint(ccp(0, 1));
    labelTextLoy->setAnchorPoint(ccp(0, 1));
    labelTextSoc->setAnchorPoint(ccp(0, 1));
    labelTextHap->setAnchorPoint(ccp(0, 1));
    
    // sprite row header
    labelSRImage->setAnchorPoint(ccp(0, 1));
    
    scrollArea->setAnchorPoint(ccp(0.5, 0.5));
    
    reposition();
}

void SelectPopulation::onMenuItemSelected(CCObject* pSender){
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case -1:
            // buttonClose
            clear();
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
    
    spriteBuilding->setPosition(ccp(15.0f - halfWidth, halfHeight - 10.0f));
    
    // Anchored top right
    buttonClose->setPosition(halfWidth - 20.0f, halfHeight - 20.0f);
    
    spInt->setPosition(ccp(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + 30.0f, halfHeight - 12.0f));
    spLoy->setPosition(ccp(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + 30.0f, halfHeight - spInt->boundingBox().size.height - 15.0f));
    spSoc->setPosition(ccp(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width +  100.0f, halfHeight - 12.0f));
    spHap->setPosition(ccp(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width +  100.0f, halfHeight - spSoc->boundingBox().size.height - 15.0f));
    
    labelBuildingName->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + 25.0f, halfHeight - 12.0f);
    labelBuildingLevel->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + 25.0f, halfHeight - labelBuildingName->boundingBox().size.height - 15.0f);
    
    labelTextInt->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width + 60.0f, halfHeight - 14.0f);
    labelTextLoy->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width + 60.0f, halfHeight - spInt->boundingBox().size.height - 17.0f);
    labelTextSoc->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width + spSoc->boundingBox().size.width + 130.0f, halfHeight - 14.0f);
    labelTextHap->CCNode::setPosition(-halfWidth + spriteBuilding->boundingBox().size.width + labelBuildingName->boundingBox().size.width + spInt->boundingBox().size.width + spSoc->boundingBox().size.width + 130.0f, halfHeight - spInt->boundingBox().size.height - 17.0f);
    
    // sprite row header
    labelSRImage->CCNode::setPosition(-halfWidth + 10.0f, halfHeight - spriteBuilding->boundingBox().size.height - 15.0f);
    
    // Scroll area in center
    scrollArea->CCNode::setPosition(-halfWidth,
                                    -halfHeight);
    
    scrollArea->reposition();
}

void SelectPopulation::clear(){
    /*
    for(int i = 0; i < spriteRowArray->count(); i++){
        ((SpriteRow*)spriteRowArray->objectAtIndex(i))->clear();
    }
    spriteRowArray->removeAllObjects();
    CC_SAFE_RELEASE(spriteRowArray);
    
    delete background_rect;
    delete buttonClose;
    delete spriteBackground;
    delete spriteBuilding;
    delete customSprite;
    
    delete spInt;
    delete spLoy;
    delete spSoc;
    delete spHap;
    
    delete labelBuildingName;
    delete labelBuildingLevel;
    
    delete labelTextInt;
    delete labelTextLoy;
    delete labelTextSoc;
    delete labelTextHap;
    
    delete labelSRImage;
    //delete labelSRName;
    */
}

//void SelectPopulation::refreshAllMenuItemValues(){
    
//}

void SelectPopulation::willChangeOrientation(){
    
}

void SelectPopulation::onOrientationChanged(){
    
}

void SelectPopulation::update(float deltaTime){
    
}


