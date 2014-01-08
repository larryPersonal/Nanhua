//
//  BuildMenu.cpp
//  PopulationTest
//
//  Created by Serious Games on 14/3/13.
//
//

#include "BuildMenu.h"
#include "GameScene.h"
#include "GameHUD.H"
#include "InGameMenu.h"
#include "GameDefaults.h"
//#include "GameUnlocks.h"

BuildMenu::~BuildMenu()
{
    pathMenuItems->removeAllObjects();
    pathMenuItems->release();
    
    
    scrollPathArea->removeAllChildren();
    scrollPathArea->release();
    
    for (int i = 0; i < tabItemArray.size();  ++i)
    {
        tabItemArray[i]->release();
    }
    tabItemArray.clear();
    
    for (int i = 0; i < scrollAreaArray.size();  ++i)
    {
        if (scrollAreaArray[i]->getChildrenCount() > 0)
            scrollAreaArray[i]->removeAllChildren();
        scrollAreaArray[i]->release();
    }
    scrollAreaArray.clear();
    
    for (int i = 0; i < menuItemArray.size();  ++i)
    {
        if (menuItemArray[i]->count() > 0)
            menuItemArray[i]->removeAllObjects();
        menuItemArray[i]->release();
    }
    menuItemArray.clear();
    
    for (int i = 0; i < menuArray.size(); ++i)
    {
        menuArray[i]->release();

    }
    menuArray.clear();
}

/*
BuildMenu* BuildMenu::create()
{
    BuildMenu *pRet = new BuildMenu();
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
}*/
void BuildMenu::createMenuItems()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    this->setTouchPriority(-129);
    /*build menu*/
    menuItems = CCArray::createWithCapacity(GameScene::getThis()->buildingHandler->allBuildings->count() +1 ); //+1 for the demolish button
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    /*The rest*/
    CCArray* arrBuildings = GameManager::getThis()->unlockedBuildings;
    float yPosition = 0.0f;
    float yHousing = 0.0f;
    float yAmenity = 0.0f;
    float yCommerce = 0.0f;
    float yEducation = 0.0f;
    float ySocial = 0.0f;
    float yMilitary = 0.0f;
    
    for (int i = 0; i < 6; ++i)
    {
        
        CCArray* mItem = CCArray::create();
        mItem->retain();
        menuItemArray.push_back(mItem);
    }
    
    std::string buildCat = "";
    
    for (int i = 0; i < arrBuildings->count(); i++)
    {
        Building* currBuilding = (Building*)arrBuildings->objectAtIndex(i);

         //Copy the texture from TMX
        CCSprite* menuImage = CCSprite::createWithTexture(currBuilding->buildingTexture);
        menuImage->setTextureRect(currBuilding->buildingRect);
        
        float newScale = 128 / menuImage->boundingBox().size.width;
        menuImage->setScale(newScale);
        
        CCMenuItemSprite *menuItem = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(BuildMenu::onMenuItemSelected));
        menuItem->setTag(currBuilding->ID);
        menuItem->setAnchorPoint(ccp(0, 1));
        menuItem->setContentSize(menuImage->boundingBox().size);
        
        //Tag labels to it
        CCLabelTTF* nameLabel =         CCLabelTTF::create( currBuilding->buildingName.c_str(), "Droidiga", 40);
        
        CCSize descSize;
        descSize.width = menuItem->boundingBox().size.width * 2.5;
        descSize.height = menuItem->boundingBox().size.height*0.5;
        
        CCLabelTTF* descriptionLabel =  CCLabelTTF::create(currBuilding->buildingDesc.c_str(), "Droidiga", 30, descSize, kCCTextAlignmentCenter);
        std::stringstream ss;
        ss << currBuilding->buildingCost << " G";
        CCLabelTTF* costLabel =         CCLabelTTF::create(ss.str().c_str(), "Droidiga", 30);
        
        CCPoint newPosition = ccp(menuItem->getPositionX() + menuItem->boundingBox().size.width*2.5,
                                  menuItem->getPositionY() + menuItem->boundingBox().size.height*0.5);
        nameLabel->         setPosition(ccp(newPosition.x, newPosition.y + nameLabel->boundingBox().size.height * 0.5f));
        descriptionLabel->  setPosition(newPosition);
        costLabel->         setPosition(ccp(newPosition.x, newPosition.y - nameLabel->boundingBox().size.height * 0.5f));
        
        menuItem->addChild(nameLabel);
        menuItem->addChild(descriptionLabel);
        menuItem->addChild(costLabel);
        
        
        
        switch (currBuilding->buildingType)
        {
            case HOUSING:
                menuItem->setPosition(CCPointMake(0, yHousing));
                
                menuItemArray[0]->addObject(menuItem);
                yHousing -=(menuImage->boundingBox().size.height + 40);

                break;
            case AMENITY:
                menuItem->setPosition(CCPointMake(0, yAmenity));
                
                menuItemArray[1]->addObject(menuItem);
                yAmenity -=(menuImage->boundingBox().size.height + 40);
                

                break;
            case COMMERCE:
                menuItem->setPosition(CCPointMake(0, yCommerce));
                
                menuItemArray[2]->addObject(menuItem);
                yCommerce -=(menuImage->boundingBox().size.height + 40);
                

                break;
            case MILITARY:
                menuItem->setPosition(CCPointMake(0, yMilitary));
                
                menuItemArray[5]->addObject(menuItem);
                yMilitary -=(menuImage->boundingBox().size.height + 40);
                

                break;
            case EDUCATION:
                menuItem->setPosition(CCPointMake(0, yEducation));
                
                menuItemArray[3]->addObject(menuItem);
                yEducation -=(menuImage->boundingBox().size.height + 40);
                

                break;
            case SOCIAL:
                menuItem->setPosition(CCPointMake(0, ySocial));
                
                menuItemArray[4]->addObject(menuItem);
                ySocial -=(menuImage->boundingBox().size.height + 40);
                

                break;
            default:
                break;
        }
        
    }
    
        
    //CCMenu* menu = CCMenu::createWithArray(menuItems);
    //menu->setPosition(CCPointZero);
    
    for (int i = 0; i < menuItemArray.size(); ++i)
    {
        CCMenu* menu = CCMenu::createWithArray(menuItemArray[i]);
        menu->setPosition(CCPointZero);
        menu->retain();
        menuArray.push_back(menu);
    }
    
    // Build Menu UI vars
    CCSize buildMenuSize = CCSizeMake(622.0f, 370.0f);
      ccColor3B colorGrey = ccc3(150 , 150, 150);
    
    for (int i = 0; i < menuItemArray.size(); ++i)
    {
        ScrollArea* scrollArea = new ScrollArea();
        switch (i)
        {
            case 0:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yHousing- 200)));
                break;
            case 1:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yAmenity - 200)));
                break;
            case 2:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yCommerce - 200)));
                break;
            case 3:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yEducation - 200)));
                break;
            case 4:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(ySocial - 200)));
                break;
            case 5:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yMilitary - 200)));
                break;
            default:
                scrollArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yPosition - 200)));
                break;

        }
        
        
        scrollArea->addItem(menuArray[i], ccp(20.0f, 0.0f));
        scrollArea->enableScrollVertical(10, "bar.png", "bar.png");
        scrollArea->hideScroll();
        scrollArea->reposition();
        scrollArea->updateScrollBars();
        scrollAreaArray.push_back(scrollArea);
    }
    
    /*pathMenu*/
    pathMenuItems = CCArray::createWithCapacity(2);
    pathMenuItems->retain();
    
    CCSprite* pathImg =   CCSprite::create("path.png");
    CCSprite* pathSelectedImg = CCSprite::create("path.png");
    pathImg->             setScale(128 / pathImg->boundingBox().size.width);
    pathSelectedImg->           setScale(pathImg->getScale());
    CCMenuItemSprite* buildPathItem =  CCMenuItemSprite::create(pathImg, pathSelectedImg, this, menu_selector(BuildMenu::onMenuItemSelected) );
    buildPathItem->setTag(-3);
    
    
    
    buildPathItem->setAnchorPoint(ccp(0, 1));
    buildPathItem->setContentSize(pathImg->boundingBox().size);

    CCLabelTTF* pathLabel =     CCLabelTTF::create("Build Path", "Droidiga", 30);
    CCPoint pathlabelPosition = ccp(buildPathItem->boundingBox().size.width*2.5,
                                    buildPathItem->boundingBox().size.height*0.5);
    pathLabel->setPosition(pathlabelPosition);
    buildPathItem->addChild(pathLabel);

    buildPathItem->  setPosition(CCPointMake(0, 0));
    
    //Demolish & Cancel
    CCSprite* demoNormalImage =     CCSprite::create("demolish.png");
    CCSprite* demoSelectedImage =   CCSprite::create("demolish.png");
    demoNormalImage->               setScale(128 / demoNormalImage->boundingBox().size.width);
    demoSelectedImage->             setScale(demoNormalImage->getScale());
    CCMenuItemSprite* demoItem =    CCMenuItemSprite::create(demoNormalImage, demoSelectedImage, this, menu_selector(BuildMenu::onMenuItemSelected) );
    
    
    demoItem->  setTag(-1);
    demoItem->  setAnchorPoint(ccp(0, 1));
    demoItem->  setContentSize(demoNormalImage->boundingBox().size);
    
    CCLabelTTF* demoLabel =     CCLabelTTF::create("Demolish", "Droidiga", 30);
    
    CCPoint labelPosition = ccp(demoItem->boundingBox().size.width*2.5,
                                demoItem->boundingBox().size.height*0.5);
    
    demoItem->  setPosition(CCPointMake(0, -(pathImg->boundingBox().size.height + 40) * 2));
    
    demoLabel->     setPosition(labelPosition);
    demoItem->addChild(demoLabel);
    
    
    CCSprite* demo2NormalImage =     CCSprite::create("path-destroy.png");
    CCSprite* demo2SelectedImage =   CCSprite::create("path-destroy.png");
    demo2NormalImage->               setScale(128 / demoNormalImage->boundingBox().size.width);
    demo2SelectedImage->             setScale(demoNormalImage->getScale());
    CCMenuItemSprite* demoPathItem =    CCMenuItemSprite::create(demo2NormalImage, demo2SelectedImage, this, menu_selector(BuildMenu::onMenuItemSelected) );
    demoPathItem->  setTag(-4);
    CCLabelTTF* demoPathLabel =     CCLabelTTF::create("Destroy Path", "Droidiga", 30);
    CCPoint destroyPathPosition = ccp(demoPathItem->boundingBox().size.width*2.5,
                                      demoPathItem->boundingBox().size.height*0.5);
    demoPathLabel->setPosition(destroyPathPosition);
    demoPathItem->addChild(demoPathLabel);
    
    demoPathItem->setAnchorPoint(ccp(0, 1));
    demoPathItem->setContentSize(demo2NormalImage->boundingBox().size);
    
    demoPathItem->  setPosition(CCPointMake(0, -(pathImg->boundingBox().size.height + 40)));
    
    pathMenuItems->addObject(buildPathItem);
    pathMenuItems->addObject(demoPathItem);
    pathMenuItems->addObject(demoItem);

    
    CCMenu* pathMenu = CCMenu::createWithArray(pathMenuItems);
    pathMenu->setPosition(CCPointZero);
    
    scrollPathArea = new ScrollArea();
    scrollPathArea->createScrollArea(buildMenuSize, CCSizeMake(0.0f, -(yPosition - (buildPathItem->boundingBox().size.height * 3) - 200)));
    scrollPathArea->addItem(pathMenu, ccp(20.0f, 0.0f));
    scrollPathArea->enableScrollVertical(10, "bar.png", "bar.png");
    scrollPathArea->hideScroll();
    scrollPathArea->reposition();
    scrollPathArea->updateScrollBars();
    
    
    // Tabbed View
    tabbedView = new TabbedView();
    
    // Tab Menu Background
    CCSprite* tabbedViewBg = CCSprite::create("outerbox.png");
    tabbedViewBg->setAnchorPoint(CCPointZero);
    tabbedViewBg->setScale(700.0f / tabbedViewBg->getContentSize().width);
    
    
    CCSprite* tabbedViewBgInner = CCSprite::create("innerbox-policy.png");
    tabbedViewBgInner->setAnchorPoint(CCPointZero);
    tabbedViewBgInner->setPosition(ccp(10, 10));
    tabbedViewBgInner->setScale(tabbedViewBg->getScale());
    // Tab Layers
    /*
    CCLayer* tabLayerBuildings = CCLayer::create();
    tabLayerBuildings->setContentSize(buildMenuSize);
    tabLayerBuildings->addChild(scrollArea);
    */
    for (int i = 0; i < scrollAreaArray.size(); ++i)
    {
        CCLayer* tabLayerBuildings = CCLayer::create();
        tabLayerBuildings->setContentSize(buildMenuSize);
        tabLayerBuildings->addChild(scrollAreaArray[i]);
        tabLayerArray.push_back(tabLayerBuildings);
    }
    
    
    tabLayerPath = CCLayer::create();
    tabLayerPath->setContentSize(buildMenuSize);
    tabLayerPath->addChild(scrollPathArea);
    
  
    /*Populate tab items*/    
    for (int i = 0; i < tabLayerArray.size(); ++i )
    {
        CCMenuItemImage* tabItem = CCMenuItemImage::create("nav button.png", "nav button-pressed.png");
        tabItem->setAnchorPoint(CCPointZero);
        tabItem->setScale(tabbedViewBg->getScale() * 1.5);
        CCLog(buildCat.c_str());
        std::string buildCat ="";
        switch (i)
        {
            case 0:
                buildCat = "Housing";
                break;
            case 1:
                buildCat = "Amenity";
                break;
            case 2:
                buildCat = "Commerce";
                break;
            case 3:
                buildCat = "Education";

                break;
            case 4:
                buildCat = "Social";

                break;
            case 5:
                buildCat = "Military";

                break;
            default:
                break;
        }
        CCLabelTTF* label = CCLabelTTF::create(buildCat.c_str(), "Droidiga", 20);
        
        /*screw layering rules, I'm gonna handle it on my own*/
        tabItem->setEnabled(false);

        if (menuItemArray[i]->count() == 0)
        {
            label->setColor(colorGrey);
            
        }
        else
        {
            label->setColor(ccc3(111, 107, 50));
        }
        label->setScale(1.0f / tabItem->getScale());
        label->CCNode::setPosition(tabItem->getContentSize().width / 2.0f, tabItem->getContentSize().height / 2.0f);
        tabItem->addChild(label);
        tabItemArray.push_back(tabItem);

    }
 
    tabItemPath = CCMenuItemImage::create ("nav button.png", "nav button-pressed.png");
    tabItemPath->setAnchorPoint(CCPointZero);
    
    /*handling manually*/
    tabItemPath->setEnabled(false);
    
    tabItemPath->setScale(tabbedViewBg->getScale() * 1.5);
    CCLabelTTF* label = CCLabelTTF::create("General", "Droidiga", 20);
    label->setColor(ccc3(111, 107, 50));
    label->setScale(1.0f / tabItemPath->getScale());
    label->CCNode::setPosition(tabItemPath->getContentSize().width / 2.0f, tabItemPath->getContentSize().height / 2.0f);
    tabItemPath->addChild(label);
    
    /*END populate tab items*/
    
    // Create tabbed view and add its items
    tabbedView->createTabbedView(ccp(0.0f, tabbedViewBg->getContentSize().height * tabbedViewBg->getScale()),
                                 ccp(42.0f, 55.0f));
    tabbedView->addChild(tabbedViewBg);
    tabbedView->addChild(tabbedViewBgInner);
    float x = -7.0f;
    min_tabx = x;
    max_tabx = x + (150 * 3.5);
    
    tabbedView->addTabWithContent(tabItemPath, ccp(x, 0.0f), tabLayerPath, true);
    for (int i = 0; i < tabItemArray.size(); ++i)
    {
        x+= 150;
        tabbedView->addTabWithContent(tabItemArray[i],ccp(x, 0.0f), tabLayerArray[i], true);
        
    }
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    this->setTouchPriority(kCCMenuHandlerPriority);
    this->addChild(tabbedView);
    
    TitleLabel = CCLabelTTF::create("Build", "Vinland", 64);
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
  
    this->addChild(TitleLabel,4);
    
    
    //    Animate();
}

void BuildMenu::onMenuItemSelected(cocos2d::CCObject *pSender)
{
    //    if (!canPressButtons) {
    //        return;
    //    }
    
    
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    
    //    Animate();
    switch (tag)
    {
        case -1 : //Deconstruct
        {
            GameHUD::getThis()->setTapMode(2);
            this->closeMenu();
        }
            break;
            
        case -2 : //Return to previous menu.
        {
            PopupMenu* prev = new InGameMenu();
            prev->useAsExclusivePopupMenu();
        }
            break;
        case -3 : //build path
        {
            
            if (GameManager::getThis()->currentMoney >= path_cost_per_tile)
            {
                GameHUD::getThis()->setTapMode(3);
                GameHUD::getThis()->showBuildLabel("Path");
                GameScene::getThis()->isThisTapCounted = true;
                this->closeMenu();
            }
            else
            {
                //Audio feedback?
                //Visual feedback? //No money
            }
        }
            break;
        case -4 : //unbuild path
        {
            GameHUD::getThis()->setTapMode(4);
            this->closeMenu();
        }
            break;
        default:
        {
            Building* buildingToBuy = GameScene::getThis()->buildingHandler->getBuilding(tag);
            //If enough money
         //   if (GameManager::getThis()->currentMoney >= buildingToBuy->buildingCost) {
                GameHUD::getThis()->setTapMode(1);
                GameHUD::getThis()->showBuildLabel(buildingToBuy->buildingName.c_str() );
                GameHUD::getThis()->updateBuildCostLabel(buildingToBuy->buildingCost);
                GameScene::getThis()->buildingHandler->selectedBuilding = buildingToBuy;
                GameScene::getThis()->isThisTapCounted = true;
                this->closeMenu();
          //  }
          //  else{
                //Audio feedback?
                
                
                //Visual feedback?
          //  }
        }
            break;
    }
}

void BuildMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
}

bool BuildMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    touchBegin = tabbedView->menu->convertToNodeSpace(pTouch->getLocation());
    return true;
}

void BuildMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    /*allowing top menu to be dragged here.*/
    float tomove_x = pTouch->getLocationInView().x - pTouch->getPreviousLocationInView().x;
    if (tomove_x == 0) return;
    
    float leftPos = tabItemPath->getPositionX() + tomove_x;
    float rightPos = tabItemArray[tabItemArray.size() - 1]->getPositionX() + tomove_x;
    
    if (leftPos > min_tabx) return;
    if (rightPos < max_tabx) return;
    
    
    for (int i = 0; i < tabItemArray.size(); ++i)
    {
        tabItemArray[i]->setPosition(tabItemArray[i]->getPositionX() + tomove_x, tabItemArray[i]->getPositionY());
    }
    tabItemPath->setPosition(tabItemPath->getPositionX() +tomove_x, tabItemPath->getPositionY());

}

/*Due to the limitations of CCMenu (which always swallows touches, no exceptions), I need to implement my own touch logic and disable the way to click the menu.*/
void BuildMenu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touch = tabbedView->menu->convertToNodeSpace(pTouch->getLocation());
    
   // float touchDist = ccpDistance(touch, touchBegin);
   // if (touchDist != 0) return;
    
    bool test = tabItemPath->boundingBox().containsPoint(touch);
    if (test)
    {
        tabbedView->onTabSelected(tabItemPath);
        return; //only process 1 tab view per run, if it is active
    }
    
    for (int i = 0; i < tabItemArray.size(); ++i)
    {
        test = tabItemArray[i]->boundingBox().containsPoint(touch);
        if (test && menuItemArray[i]->count() > 0)
        {
            tabbedView->onTabSelected(tabItemArray[i]);
            return; //only process 1 tab view per run, if it is active
        }
        
    }
}


