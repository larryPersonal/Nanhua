//
//  PopulationMenu.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 27/5/13.
//
//

#include "PopulationMenu.h"
#include "GameScene.h"
#include "SpriteInfoMenu.h"
#include "BuildingInfoMenu.h"

PopulationMenu::~PopulationMenu()
{
    scrollArea->removeAllChildren();
    scrollArea->release();
}

void PopulationMenu::createMenuItems()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float yPosition = 0.0f;
    
    // Build Menu UI vars
    CCSize PopulationMenuSize = CCSizeMake(622.0f, 280.0f);
    //ccColor3B colorYellow = ccc3(225, 219, 108);
    
    menuItems = CCArray::createWithCapacity(4);
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    CCArray* arrSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
    
    for (int i = 0; i < arrSprites->count(); i++)
    {
        GameSprite* currSprite = (GameSprite*)arrSprites->objectAtIndex(i);
          if (!currSprite) continue;
        
        // Sprite texture
        string spriteFrameName = currSprite->spriteName.c_str();
        spriteFrameName += "_IDL001.png";
        CCSprite* menuImage = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
        
        float newScale = 128 / menuImage->boundingBox().size.width;
        menuImage->setScale(newScale);
        
        CCMenuItemSprite *menuItem = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(PopulationMenu::onMenuItemSelected));
        menuItem->setTag((int)currSprite);      //set Tag as address of the GameSprite object
        menuItem->setAnchorPoint(ccp(0, 1));
        menuItem->setContentSize(menuImage->boundingBox().size);
        
        Building* hloc = currSprite->getPossessions()->homeLocation;
        
        CCSprite* homeImg = CCSprite::create();
        if (hloc != NULL)
        {
            homeImg->initWithTexture(hloc->buildingTexture, hloc->buildingRect);
            homeImg->setScale(newScale *0.75);
            
        }
        
        CCMenuItemSprite *homeItem = CCMenuItemSprite::create(homeImg, NULL, this, menu_selector(PopulationMenu::onBuildingItemSelected));
        homeItem->setAnchorPoint(ccp(0,1));
        homeItem->setContentSize(menuItem->boundingBox().size);
        homeItem->setTag(1001);
 
 //       Building* wloc = currSprite->getPossessions()->jobLocation;
        
        CCSprite* workImg =  CCSprite::create();
       /*
        if (wloc != NULL)
        {
           workImg->initWithTexture(wloc->buildingTexture, wloc->buildingRect);
            workImg->setScale(newScale * 0.75);
        }*/
        
        
        CCMenuItemSprite *workItem = CCMenuItemSprite::create(workImg, NULL, this, menu_selector(PopulationMenu::onBuildingItemSelected));
        workItem->setAnchorPoint(ccp(0,1));
        workItem->setContentSize(menuItem->boundingBox().size);
        workItem->setTag(1002);
       
        std::string spclass = currSprite->spriteClass;
        if (spclass.compare("citizen") == 0)
            spclass = "merchant";
        
        spclass = spclass + "-icon.png";
        
        CCSprite* classImg = CCSprite::create(spclass.c_str());
        classImg->setScale(newScale);
        classImg->setTag(1003);
        
       
        //Tag labels to it
        CCLabelTTF* nameLabel =         CCLabelTTF::create( currSprite->spriteDisplayedName.c_str(), "Shojumaru-Regular", 18);
      //  CCLabelTTF* descriptionLabel =  CCLabelTTF::create( currSprite->spriteClass.c_str(), "Shojumaru-Regular", 26);
        
        menuItem->addChild(nameLabel);
    //    menuItem->addChild(descriptionLabel);
        menuItem->addChild(classImg);
        menuItem->addChild(workItem);
        menuItem->addChild(homeItem);
        
        float offset = menuItem->boundingBox().size.width * 0.25f;
   
        nameLabel->         setPosition(ccp(menuImage->boundingBox().size.width * 0.5f , 0));// - menuItem->boundingBox().size.height));
    //    descriptionLabel->  setPosition(ccp(PopulationMenuSize.width * 0.5f,  0));
        classImg->setPosition(ccp(PopulationMenuSize.width * 0.2f + offset, classImg->boundingBox().size.height * 0.5f));
        
        
        workItem->setPosition(ccp(PopulationMenuSize.width * 0.375f, workItem->boundingBox().size.height));
        homeItem->setPosition(ccp(PopulationMenuSize.width * 0.575f, homeItem->boundingBox().size.height));
        
    
        menuItem->setPosition(CCPointMake(0, yPosition));
        menuItems->addObject(menuItem);

        yPosition -= (menuImage->boundingBox().size.height + 40);
    }
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    

    
    // Scroll Area
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(PopulationMenuSize, CCSizeMake(0.0f, -yPosition));
    scrollArea->addItem(menu, ccp(20.0f, 0.0f));
    scrollArea->enableScrollVertical(10, "bar.png", "bar.png");
    scrollArea->hideScroll();
    scrollArea->reposition();
    scrollArea->updateScrollBars();
    
    // Tabbed View
    tabbedView = new TabbedView();
    
    // Tab Menu Background
    CCSprite* tabbedViewBg = CCSprite::create("outerbox.png");
    tabbedViewBg->setAnchorPoint(CCPointZero);
    tabbedViewBg->setScale(700.0f / tabbedViewBg->getContentSize().width);
    
    
    CCSprite* tabbedViewBgInner = CCSprite::create("innerbox.png");
    tabbedViewBgInner->setAnchorPoint(CCPointZero);
    tabbedViewBgInner->setPosition(ccp(10, 50));
    tabbedViewBgInner->setScale(tabbedViewBg->getScale());
    
    // Tab Layers
    tabLayerPopulation = CCLayer::create();
    tabLayerPopulation->setContentSize(PopulationMenuSize);
    tabLayerPopulation->addChild(scrollArea);
    
    // Tab Items
    tabItemPopulation= CCMenuItemImage::create("nav button.png", "nav button-pressed.png");
    tabItemPopulation->setVisible(false);

    
    // Create tabbed view and add its items
    tabbedView->createTabbedView(ccp(0.0f, tabbedViewBg->getContentSize().height * tabbedViewBg->getScale()),
                                 ccp(42.0f, 85.0f));
    tabbedView->addChild(tabbedViewBg);
    tabbedView->addChild(tabbedViewBgInner);
    tabbedView->addTabWithContent(tabItemPopulation, ccp(218.0f, -20.0f), tabLayerPopulation);
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    
    this->addChild(tabbedView);
    TitleLabel = CCLabelTTF::create("Population Information", "Vinland", 64);
    
    NameLabel = CCLabelTTF::create("NAME", "Shojumaru-Regular", 25);
    ClassLabel = CCLabelTTF::create("CLASS", "Shojumaru-Regular",25 );
    JobLabel = CCLabelTTF::create("JOB", "Shojumaru-Regular", 25);
    HomeLabel =CCLabelTTF::create("HOME", "Shojumaru-Regular", 25);
    CashLabel =CCLabelTTF::create("CASH", "Shojumaru-Regular", 25);

    
    if (screenSize.width > screenSize.height) // landscape
        {
            TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.725f));
            
            float headerYPos = screenSize.height * 0.625f;
            NameLabel->setPosition(ccp(screenSize.width * 0.275f, headerYPos));
            ClassLabel->setPosition(ccp(screenSize.width * 0.375f, headerYPos));
            JobLabel->setPosition(ccp(screenSize.width * 0.475f, headerYPos));
            HomeLabel->setPosition(ccp(screenSize.width * 0.6f, headerYPos));
            CashLabel->setPosition(ccp(screenSize.width * 0.725f, headerYPos));
            
        }
    else
    {
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.675f));
        float headerYPos = screenSize.height * 0.6f;

        NameLabel->setPosition(ccp(screenSize.width * 0.2f, headerYPos));
        ClassLabel->setPosition(ccp(screenSize.width * 0.35f, headerYPos));
        JobLabel->setPosition(ccp(screenSize.width * 0.5f, headerYPos));
        HomeLabel->setPosition(ccp(screenSize.width * 0.65f, headerYPos));
        CashLabel->setPosition(ccp(screenSize.width * 0.8f, headerYPos));

    }
    
    this->addChild(TitleLabel,4);
    this->addChild(NameLabel,4);
    this->addChild(ClassLabel,4);
    this->addChild(JobLabel,4);
    this->addChild(HomeLabel,4);
    this->addChild(CashLabel,4);
    
    
    //    Animate();
}

void PopulationMenu::onMenuItemSelected(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = pMenuItem->getTag();
    
    SpriteInfoMenu* spriteInfoMenu = new SpriteInfoMenu((GameSprite*)tag);
    spriteInfoMenu->useAsTopmostPopupMenu();
}

void PopulationMenu::onBuildingItemSelected(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = pMenuItem->getTag();
    
    BuildingInfoMenu* bInfoMenu = new BuildingInfoMenu((Building*)tag);
    bInfoMenu->useAsTopmostPopupMenu();
    
}

void PopulationMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    if (screenSize.width > screenSize.height) // landscape
    {
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.725f));
        
        float headerYPos = screenSize.height * 0.625f;
        NameLabel->setPosition(ccp(screenSize.width * 0.275f, headerYPos));
        ClassLabel->setPosition(ccp(screenSize.width * 0.375f, headerYPos));
        JobLabel->setPosition(ccp(screenSize.width * 0.475f, headerYPos));
        HomeLabel->setPosition(ccp(screenSize.width * 0.6f, headerYPos));
        CashLabel->setPosition(ccp(screenSize.width * 0.725f, headerYPos));
        
        
    }
    else
    {
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.675f));
        float headerYPos = screenSize.height * 0.6f;
        
        NameLabel->setPosition(ccp(screenSize.width * 0.2f, headerYPos));
        ClassLabel->setPosition(ccp(screenSize.width * 0.35f, headerYPos));
        JobLabel->setPosition(ccp(screenSize.width * 0.5f, headerYPos));
        HomeLabel->setPosition(ccp(screenSize.width * 0.65f, headerYPos));
        CashLabel->setPosition(ccp(screenSize.width * 0.8f, headerYPos));
        
    }
    
}

/*
void PopulationMenu::updateItemCashLabel(GameSprite* sprite)
{
    std::stringstream ss;
    ss << sprite->getPossessions()->cashOnHand << " G";
    ((CCLabelTTF*)menu->getChildByTag((int)sprite)->getChildByTag(1000))->setString(ss.str().c_str());
}*/

void PopulationMenu::updateItemJobLabel(GameSprite* sprite)
{
    
}

void PopulationMenu::updateItemClassLabel(GameSprite* sprite)
{
    
}

void PopulationMenu::updateItemHomeLabel(GameSprite *sprite)
{
    
}


void PopulationMenu::addPopulationToList(GameSprite* sprite)
{
    CCSize PopulationMenuSize = CCSizeMake(622.0f, 370.0f);
    
    // Sprite texture
    string spriteFrameName = sprite->spriteName.c_str();
    spriteFrameName += "_IDL001.png";
    CCSprite* menuImage = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
    
    float newScale = 128 / menuImage->boundingBox().size.width;
    menuImage->setScale(newScale);
    
    CCMenuItemSprite *menuItem = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(PopulationMenu::onMenuItemSelected));
    menuItem->setTag((int)sprite);      //set Tag as address of the GameSprite object
    menuItem->setAnchorPoint(ccp(0, 1));
    menuItem->setContentSize(menuImage->boundingBox().size);
    
    //Tag labels to it
    CCLabelTTF* nameLabel =         CCLabelTTF::create( sprite->spriteDisplayedName.c_str(), "Shojumaru-Regular", 18);

    Building* hloc = sprite->getPossessions()->homeLocation;

    CCSprite* homeImg = CCSprite::create();
    if (hloc != NULL)
    {
        homeImg->initWithTexture(hloc->buildingTexture, hloc->buildingRect);
        homeImg->setScale(newScale *0.75);
        
    }
    
    CCMenuItemSprite *homeItem = CCMenuItemSprite::create(homeImg, NULL, this, menu_selector(PopulationMenu::onBuildingItemSelected));
    homeItem->setAnchorPoint(ccp(0,1));
    homeItem->setContentSize(menuItem->boundingBox().size);
    homeItem->setTag(1001);
    
    //Building* wloc = sprite->getPossessions()->jobLocation;
    
    CCSprite* workImg =  CCSprite::create();
    /*
    if (wloc != NULL)
    {
        workImg->initWithTexture(wloc->buildingTexture, wloc->buildingRect);
        workImg->setScale(newScale * 0.75);
    }*/
    
    
    CCMenuItemSprite *workItem = CCMenuItemSprite::create(workImg, NULL, this, menu_selector(PopulationMenu::onBuildingItemSelected));
    workItem->setAnchorPoint(ccp(0,1));
    workItem->setContentSize(menuItem->boundingBox().size);
    workItem->setTag(1002);
    
    
    
    std::string spclass = sprite->spriteClass;
    if (spclass.compare("citizen") == 0)
        spclass = "merchant";
    
    spclass = spclass + "-icon.png";
    
    CCSprite* classImg = CCSprite::create(spclass.c_str());
    classImg->setScale(newScale);

    
    
    menuItem->addChild(nameLabel);
    menuItem->addChild(classImg);
    menuItem->addChild(workItem);
    menuItem->addChild(homeItem);
    
    float offset = menuItem->boundingBox().size.width * 0.25f;
    
    nameLabel->         setPosition(ccp(menuImage->boundingBox().size.width * 0.5f , 0));// - menuItem->boundingBox().size.height));
    classImg->setPosition(ccp(PopulationMenuSize.width * 0.2f + offset, classImg->boundingBox().size.height * 0.5f));
    workItem->setPosition(ccp(PopulationMenuSize.width * 0.375f, workItem->boundingBox().size.height));
    homeItem->setPosition(ccp(PopulationMenuSize.width * 0.575f, homeItem->boundingBox().size.height));
    
    
    menuItem->setPosition(CCPointMake(0, -(menuImage->boundingBox().size.height + 40) * menuItems->count()));
    
    
    menuItems->addObject(menuItem);
    
    menu->addChild(menuItem);
    
    /*TODO: new layout. Sprite, Cash, Home, Work in this order*/
    
    scrollArea->setScrollContentSize(CCSizeMake(0, (menuImage->boundingBox().size.height + 40) * menuItems->count()));
}

void PopulationMenu::removePopulationFromList(GameSprite* sprite)
{
    CCMenuItem* menuItem = (CCMenuItem*)menu->getChildByTag((int)sprite);
    
    menuItems->removeObject(menuItem);
    menu->removeChildByTag((int)sprite);
    
    // Reposition items
    float yPosition = 0.0f;
    for (int i = 0; i < menuItems->count(); i++)
    {
        CCMenuItem* menuItem = (CCMenuItem*)menuItems->objectAtIndex(i);
        
        menuItem->setPosition(CCPointMake(0, yPosition));
        yPosition -= (menuItem->boundingBox().size.height + 40);
    }
    
    scrollArea->setScrollContentSize(CCSizeMake(0.0f, -yPosition));
}









