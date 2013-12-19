//
//  InfoMenu.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 27/5/13.
//
//

#include "InfoMenu.h"
#include "GameScene.h"
#include "GameManager.h"
#include "BuildingInfoMenu.h"
#include "GlobalHelper.h"

InfoMenu::~InfoMenu()
{
    scrollArea->removeAllChildren();
    scrollArea->release();
    
    
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

void InfoMenu::createMenuItems()
{
    
    //    Animate();
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float yPosition = 0.0f;
    
    CCSize buildMenuSize = CCSizeMake(622.0f, 320.0f);
    
    
    menuItems = CCArray::createWithCapacity(GameScene::getThis()->buildingHandler->allBuildings->count() + 1); //+1 for the exit button;
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    CCArray* arrBuildings = GameScene::getThis()->buildingHandler->allBuildingsOnMap;
    
    
    
    
    
    /*The rest*/
    float yHousing = 0.0f;
    float yAmenity = 0.0f;
    float yCommerce = 0.0f;
    float yEducation = 0.0f;
    float yMilitary = 0.0f;
    float ySocial = 0.0f;
    
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
        if (!currBuilding) continue;
        if (currBuilding->buildingType == BUILDINGCATEGORYMAX)continue;
        
        //Copy the texture from TMX
        CCSprite* menuImage = CCSprite::createWithTexture(currBuilding->buildingTexture);
        menuImage->setTextureRect(currBuilding->buildingRect);
        
        float newScale = 128 / menuImage->boundingBox().size.width;
        menuImage->setScale(newScale);
        
        CCMenuItemSprite *menuItem = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(InfoMenu::onMenuItemSelected));
       menuItem->setTag((int)currBuilding);      //set Tag as address of the Building object
        menuItem->setAnchorPoint(ccp(0, 1));
        menuItem->setContentSize(menuImage->boundingBox().size);
        
        //Tag labels to it
        CCLabelTTF* nameLabel =         CCLabelTTF::create( currBuilding->buildingName.c_str(), "Droidiga", 18);
        
        menuItem->addChild(nameLabel);
        nameLabel->  setPosition(ccp(menuImage->getPositionX() + menuImage->boundingBox().size.width * 0.5f, menuImage->getPositionY() - menuImage->boundingBox().size.height * 0.15f));
        
        
        CCLabelTTF* typeLabel =     CCLabelTTF::create("", "Droidiga", 30);
        menuItem->addChild(typeLabel);
        typeLabel->  setPosition(ccp(buildMenuSize.width * 0.35f + menuImage->boundingBox().size.width * 0.5f, menuImage->getPositionY()));
        
        CCLabelTTF* occupancyLabel = CCLabelTTF::create("", "Droidiga", 30);
        menuItem->addChild(occupancyLabel);
        occupancyLabel->  setPosition(ccp(buildMenuSize.width * 0.75f + menuImage->boundingBox().size.width * 0.5f, menuImage->getPositionY()));
        
        if (currBuilding->populationLimit == 0)
        {
            occupancyLabel->setString("N/A");
        }
        else
        {
            occupancyLabel->setString( (GlobalHelper::Convert(currBuilding->getPopulationCount()) +"/" + GlobalHelper::Convert( currBuilding->populationLimit)).c_str());
            if (currBuilding->isOverpopulated())
                occupancyLabel->setColor(ccRED);
            
            
        }
        
        
        
        switch (currBuilding->buildingType)
        {
            case HOUSING:
                typeLabel->setString( "Housing");
                
                menuItem->setPosition(CCPointMake(0, yHousing));
                
                menuItemArray[0]->addObject(menuItem);
                yHousing -=(menuImage->boundingBox().size.height + 40);
                
                break;
            case AMENITY:
                typeLabel->setString("Amenity");
 

                menuItem->setPosition(CCPointMake(0, yAmenity));
                
                menuItemArray[1]->addObject(menuItem);
                yAmenity -=(menuImage->boundingBox().size.height + 40);
                
                
                break;
            case COMMERCE:
                typeLabel->setString("Commerce");
 
                menuItem->setPosition(CCPointMake(0, yCommerce));
                
                menuItemArray[2]->addObject(menuItem);
                yCommerce -=(menuImage->boundingBox().size.height + 40);
                
                
                break;
            case MILITARY:
                typeLabel->setString( "Military");
 

                menuItem->setPosition(CCPointMake(0, yMilitary));
                
                menuItemArray[5]->addObject(menuItem);
                yMilitary -=(menuImage->boundingBox().size.height + 40);
                
                
                break;
            case EDUCATION:
                typeLabel->setString( "Education");
 
                menuItem->setPosition(CCPointMake(0, yEducation));
                
                menuItemArray[3]->addObject(menuItem);
                yEducation -=(menuImage->boundingBox().size.height + 40);
                
                
                break;
            case SOCIAL:
                typeLabel->setString("Social");
                occupancyLabel->setString( "0 / 0" );

                menuItem->setPosition(CCPointMake(0, ySocial));
                
                menuItemArray[4]->addObject(menuItem);
                ySocial -=(menuImage->boundingBox().size.height + 40);
                
                
                break;
            default:
                occupancyLabel->setString( "0 / 0" );
                typeLabel->setString("SPECIAL");
                
                break;
        }
    }
    
    
    
    for (int i = 0; i < menuItemArray.size(); ++i)
    {
        CCMenu* menu = CCMenu::createWithArray(menuItemArray[i]);
        menu->setPosition(CCPointZero);
        menu->retain();
        menuArray.push_back(menu);
    }
    
    
    
    // Build Menu UI vars
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
    
    // Tabbed View
    tabbedView = new TabbedView();
    
    // Tab Menu Background
    CCSprite* tabbedViewBg = CCSprite::create("outerbox.png");
    tabbedViewBg->setAnchorPoint(CCPointZero);
    tabbedViewBg->setScale(700.0f / tabbedViewBg->getContentSize().width);
    
    CCSprite* tabbedViewBgInner = CCSprite::create("innerbox-building.png");
    tabbedViewBgInner->setAnchorPoint(CCPointZero);
    tabbedViewBgInner->setPosition(ccp(10, 10));
    tabbedViewBgInner->setScale(tabbedViewBg->getScale());
    
    
    for (int i = 0; i < scrollAreaArray.size(); ++i)
    {
        CCLayer* tabLayerBuildings = CCLayer::create();
        tabLayerBuildings->setContentSize(buildMenuSize);
        tabLayerBuildings->addChild(scrollAreaArray[i]);
        tabLayerArray.push_back(tabLayerBuildings);
    }
    
    /*Populate tab items*/
    for (int i = 0; i < tabLayerArray.size(); ++i )
    {
        CCMenuItemImage* tabItem = CCMenuItemImage::create("nav button.png", "nav button-pressed.png");
        tabItem->setAnchorPoint(CCPointZero);
        tabItem->setScale(tabbedViewBg->getScale() *1.5f);
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
                buildCat = "City Limits";
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
    
    /*END populate tab items*/
    
    // Create tabbed view and add its items
    tabbedView->createTabbedView(ccp(0.0f, tabbedViewBg->getContentSize().height * tabbedViewBg->getScale()),
                                 ccp(42.0f, 48.0f));
    tabbedView->addChild(tabbedViewBg);
    tabbedView->addChild(tabbedViewBgInner);
    float x = -7.0f;
    min_tabx = x;
    max_tabx = x + (150 * 3.5);
    
    for (int i = 0; i < tabItemArray.size(); ++i)
    {
        tabbedView->addTabWithContent(tabItemArray[i],ccp(x, 0.0f), tabLayerArray[i], true);
        x+= 150;
        
    }

    
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    this->setTouchPriority(kCCMenuHandlerPriority);
    this->addChild(tabbedView);
    
    TitleLabel = CCLabelTTF::create("Building Information", "Vinland", 64);
    NameLabel = CCLabelTTF::create("Name", "Droidiga", 30);
    
    TypeLabel = CCLabelTTF::create("Type", "Droidiga", 30);
    OccLabel = CCLabelTTF::create("Occupancy", "Droidiga", 30);
    
    
    if (screenSize.width > screenSize.height) // landscape
    {
        float headerYPos = screenSize.height * 0.625f;
        NameLabel->setPosition(ccp(screenSize.width * 0.275f, headerYPos));
        
   
        TypeLabel ->setPosition(ccp(screenSize.width * 0.5f, headerYPos));
        OccLabel ->setPosition(ccp(screenSize.width * 0.7f,headerYPos));

        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.725f));
    }
    else
    {
         float headerYPos = screenSize.height * 0.6f;
        NameLabel->setPosition(ccp(screenSize.width * 0.2f, headerYPos));
        
        TypeLabel ->setPosition(ccp(screenSize.width * 0.5f, headerYPos));

        OccLabel ->setPosition(ccp(screenSize.width * 0.75f, headerYPos));


        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.675f));
    }

    this->addChild(TitleLabel,4);
    this->addChild(NameLabel,4);
    this->addChild(TypeLabel,4);
    this->addChild(OccLabel,4);

    //    Animate();

    
    
    
    
}

void InfoMenu::onMenuItemSelected(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = pMenuItem->getTag();
    
    BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create((Building*)tag);
    buildingInfoMenu->useAsTopmostPopupMenu();
}

void InfoMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    if (screenSize.width > screenSize.height) // landscape
    {
        float headerYPos = screenSize.height * 0.625f;
        NameLabel->setPosition(ccp(screenSize.width * 0.275f, headerYPos));
        
        
        TypeLabel ->setPosition(ccp(screenSize.width * 0.5f, headerYPos));
        OccLabel ->setPosition(ccp(screenSize.width * 0.7f,headerYPos));
        
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.725f));
    }
    else
    {
        float headerYPos = screenSize.height * 0.6f;
        NameLabel->setPosition(ccp(screenSize.width * 0.2f, headerYPos));
        
        TypeLabel ->setPosition(ccp(screenSize.width * 0.5f, headerYPos));
        
        OccLabel ->setPosition(ccp(screenSize.width * 0.75f, headerYPos));
        
        
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.675f));
    }

    
}

void InfoMenu::updateItemLevelLabel(Building* building)
{
    std::stringstream ss;
    ss << "Level: " << building->currentLevel;
    ((CCLabelTTF*)menu->getChildByTag((int)building)->getChildByTag(123))->setString(ss.str().c_str());
}

bool InfoMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    touchBegin = tabbedView->menu->convertToNodeSpace(pTouch->getLocation());
    return true;
}

void InfoMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    /*allowing top menu to be dragged here.*/
    float tomove_x = pTouch->getLocationInView().x - pTouch->getPreviousLocationInView().x;
    //if (tomove_x == 0) return;
    
    float leftPos = tabItemArray[0]->getPositionX() + tomove_x;
    float rightPos = tabItemArray[tabItemArray.size() - 1]->getPositionX() + tomove_x;
    
    if (leftPos > min_tabx) return;
    if (rightPos < max_tabx) return;
    
    
    for (int i = 0; i < tabItemArray.size(); ++i)
    {
        tabItemArray[i]->setPosition(tabItemArray[i]->getPositionX() + tomove_x, tabItemArray[i]->getPositionY());
    }
    
}
void InfoMenu::cleanup()
{
    
    PopupMenu::cleanup();
}
/*Due to the limitations of CCMenu (which always swallows touches, no exceptions), I need to implement my own touch logic and disable the way to click the menu.*/
void InfoMenu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touch = tabbedView->menu->convertToNodeSpace(pTouch->getLocation());
    
    // float touchDist = ccpDistance(touch, touchBegin);
    // if (touchDist != 0) return;
    
    
    bool test = false;
    
    for (int i = 0; i < tabItemArray.size(); ++i)
    {
        test = tabItemArray[i]->boundingBox().containsPoint(touch);
        if (test && menuItemArray[i]->count() >0)
        {
            tabbedView->onTabSelected(tabItemArray[i]);
            return; //only process 1 tab view per run, if it is active
        }
        
    }
}