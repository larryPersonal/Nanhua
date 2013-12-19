//
//  ResearchMenu.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 30/4/13.
//
//

#include "ResearchMenu.h"
#include "GameScene.h"
#include "GameHUD.H"
#include "InGameMenu.h"
#include "AlertBox.h"
#include "GlobalHelper.h"

ResearchMenu::~ResearchMenu()
{
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

void ResearchMenu::cleanup()
{
    timeLabelsToUpdate->removeAllObjects();
    timeLabelsToUpdate->release();
    
    PopupMenu::cleanup();
}

void ResearchMenu::createMenuItems()
{
    isUpdateScheduled = false;
    timeLabelsToUpdate = CCArray::create();
    timeLabelsToUpdate->retain();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    menuItems = CCArray::createWithCapacity(GameScene::getThis()->buildingHandler->allBuildings->count() + 1); //+1 for the exit button;
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    CCArray* arrBuildings = GameManager::getThis()->researchableBuildings;
    float yPosition = 0.0f;
    
    map<int, float>::iterator it = GameManager::getThis()->buildingIdResearchEndTime.begin();
    
    /*The rest*/
    float yHousing = 0.0f;
    float yAmenity = 0.0f;
    float yCommerce = 0.0f;
    float yEducation = 0.0f;
    float yMilitary = 0.0f;
    float ySocial = 0.0f;
    
    for (int i = 0; i < 7; ++i)
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
        if (it != GameManager::getThis()->buildingIdResearchEndTime.end() &&
            currBuilding->ID == it->first) continue; //researching buildings are removed
        //Copy the texture from TMX
        CCSprite* menuImage = CCSprite::createWithTexture(currBuilding->buildingTexture);
        menuImage->setTextureRect(currBuilding->buildingRect);
        
        float newScale = 128 / menuImage->boundingBox().size.width;
        menuImage->setScale(newScale);
        
        CCMenuItemSprite *menuItem = CCMenuItemSprite::create(menuImage, NULL, this, menu_selector(ResearchMenu::onMenuItemSelected));
        menuItem->setTag(currBuilding->ID);
        menuItem->setAnchorPoint(ccp(0, 1));
        menuItem->setContentSize(menuImage->boundingBox().size);
        
        //Tag labels to it
        CCLabelTTF* nameLabel =         CCLabelTTF::create( currBuilding->buildingName.c_str(), "Arial", 40);
     //   CCSize descSize;
      //  descSize.width = menuItem->boundingBox().size.width * 2.5;
      //  descSize.height = menuItem->boundingBox().size.height*0.5;
      
     //   CCLabelTTF* descriptionLabel =  CCLabelTTF::create(currBuilding->buildingDesc.c_str(), "Arial", 30, descSize, kCCTextAlignmentCenter);  std::stringstream ss;
     //   ss << currBuilding->researchCost << " G";
        CCLabelTTF* costLabel;//   CCLabelTTF::create(ss.str().c_str(), "Arial", 30);

        if (GameScene::getThis()->researchHandler->getResearchingBuildingCount() == 1)
            costLabel =    CCLabelTTF::create("Researchers Occupied", "Arial", 30);
        
        else
            costLabel =    CCLabelTTF::create((GlobalHelper::Convert(currBuilding->researchCost) + "G").c_str(), "Arial", 30);
        
        /*
        if (it != GameManager::getThis()->buildingIdResearchEndTime.end() &&
            currBuilding->ID == it->first)
        {
            // addResearchingLabel(costLabel, GameScene::getThis()->researchHandler->getResearchingBuildingTimeLeft(it->first));
           // ++it;
        }*/
        
        CCPoint newPosition = ccp(menuItem->getPositionX() + menuItem->boundingBox().size.width*2.5,
                                  menuItem->getPositionY() + menuItem->boundingBox().size.height*0.5);
        nameLabel->         setPosition(ccp(newPosition.x, newPosition.y + nameLabel->boundingBox().size.height * 0.5f));
       // descriptionLabel->  setPosition(newPosition);
        costLabel->         setPosition(ccp(newPosition.x, newPosition.y - nameLabel->boundingBox().size.height * 0.5f));
        costLabel->setTag((int)"costLabel");
        
        menuItem->addChild(nameLabel);
       // menuItem->addChild(descriptionLabel);
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
    
    /*add the City Limits Increase to this place under menuItemArray[6]*/
    AreaUnlock* au = GameManager::getThis()->getNextAreaUnlock();
    if (au != NULL)
    {
        if (au->hasMetCriteria(GameManager::getThis()->getCurrentMoney()))
        {
            
            
            CCSprite* cityLimits =   CCSprite::create("Icon.png");
            CCSprite* cityLimitsS = CCSprite::create("Icon.png");
            cityLimits->             setScale(128 / cityLimits->boundingBox().size.width);
            cityLimitsS->           setScale(cityLimitsS->getScale());
            CCMenuItemSprite* cityLimitItem =  CCMenuItemSprite::create(cityLimits, cityLimitsS, this, menu_selector(ResearchMenu::onMenuItemSelected) );
            cityLimitItem->setTag(-3);
            CCLabelTTF* cityLimitLabel =     CCLabelTTF::create("Expand Town", "Arial", 30);
            CCPoint labelPos = ccp(cityLimits->boundingBox().size.width*2.5,
                                    cityLimits->boundingBox().size.height*0.5);
            cityLimitLabel->setPosition(labelPos);
            cityLimitItem->addChild(cityLimitLabel);
    
            cityLimitItem->setAnchorPoint(ccp(0, 1));
            cityLimitItem->setContentSize(cityLimits->boundingBox().size);
    
            cityLimitItem->setPosition(CCPointMake(0, 0));
            menuItemArray[6]->addObject(cityLimitItem);
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
    CCSize buildMenuSize = CCSizeMake(622.0f, 370.0f);
   /// ccColor3B colorYellow = ccc3(225, 219, 108);
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
    
    CCSprite* tabbedViewBgInner = CCSprite::create("innerbox-policy.png");
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
        tabItem->setScale(tabbedViewBg->getScale() *1.5);
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
                                 ccp(42.0f, 55.0f));
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
    TitleLabel = CCLabelTTF::create("Research", "Vinland", 64);
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
    this->addChild(TitleLabel,4);
    //    Animate();
}

void ResearchMenu::onMenuItemSelected(CCObject *pSender)
{
    if (this != PopupMenu::getTopmostPopupMenu())
        return;
    
    
    if (GameScene::getThis()->researchHandler->getResearchingBuildingCount() == 1)
    {
        GameHUD::getThis()->showHint("Your researchers are researching something else.");
        return;
    }
    
    
    CCMenuItem* item = (CCMenuItem*)pSender;
    
    if (GameScene::getThis()->researchHandler->isAlreadyResearching(item->getTag()))
        return;
    
    
   
    if (item->getTag() == -3)
    {
        AlertBox* alert = AlertBox::create();
        alert->useAsTopmostPopupMenu();
        string text = "Are you sure you want to increase your town limits?";
        alert->setDisplayText(text);
        
        alert->addButton(item->getTag(), "Yes", this, callfuncO_selector(ResearchMenu::onResearchButtonPressed), item);
        alert->addButton(-1, "No");
    }
    else
    {
        Building* b = GameScene::getThis()->buildingHandler->getBuilding(item->getTag());
        
        
        AlertBox* alert = AlertBox::create();
        alert->useAsTopmostPopupMenu();
        string text = "Are you sure you want to research ";
        text += b->buildingName.c_str();
        text += "?";
        alert->setDisplayText(text);
        
        alert->addButton(item->getTag(), "Yes", this, callfuncO_selector(ResearchMenu::onResearchButtonPressed), item);
        alert->addButton(-1, "No");
        
    }
    
    
    
}

void ResearchMenu::onResearchButtonPressed(CCObject* pSender)
{
    CCMenuItem* rItem = (CCMenuItem*)pSender;
    if (rItem->getTag() == -3)
    {
        /*Note that it is IMPOSSIBLE to get here if there's nothing in the City Limits menu.*/
        AreaUnlock* au = GameManager::getThis()->getNextAreaUnlock();
        if (GameManager::getThis()->currentMoney >=au->researchcost)
        {
            GameManager::getThis()->currentMoney -= au->researchcost;
                    
            
            //WHOOPS, map unlock is not a building. Nevermind, unlock immediately.
            GameManager::getThis()->triggerNextAreaUnlock();
            
            //researchBuilding(rItem, time);   // TODO: get research time here
            PopupMenu::closeAllPopupMenu();
        }
        
        return;
    }
    
    /*if rItem isn't -3, then it continues.*/
    Building* b = GameScene::getThis()->buildingHandler->getBuilding(rItem->getTag());
    if (GameManager::getThis()->currentMoney >= b->researchCost)
    {
        GameManager::getThis()->currentMoney -= b->researchCost;
        float time = b->researchTime;
        
    
        researchBuilding(rItem, time);   // TODO: get research time here
        GameHUD::getThis()->ri->createResearchIndicator();
        PopupMenu::closeAllPopupMenu();
    }
}


void ResearchMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
}

void ResearchMenu::update(float deltaTime)
{
    if (isUpdateScheduled &&
        timeLabelsToUpdate->count() <= 0)
    {
        isUpdateScheduled = false;
        this->unschedule(schedule_selector(ResearchMenu::update));
        return;
    }
    
    for (int i = timeLabelsToUpdate->count() - 1; i >= 0; i--)
    {
        CCLabelTTF* label = (CCLabelTTF*)timeLabelsToUpdate->objectAtIndex(i);
        int buildingID = label->getParent()->getTag();
        float timeLeft = GameScene::getThis()->researchHandler->getResearchingBuildingTimeLeft(buildingID);
        
        if (timeLeft > 0.0f)
        {
            std::stringstream ss;
            ss << "Researching: " << (int)timeLeft + 1 << " seconds left";
            label->setString(ss.str().c_str());
        }
        else
        {
            timeLabelsToUpdate->removeObjectAtIndex(i);
            label->getParent()->removeFromParentAndCleanup(true);
            
            for (int i = 0; i < menuItemArray.size(); ++i)
            {
                if (menuItemArray[i]->containsObject(label->getParent()))
                {
                    menuItemArray[i]->removeObject(label->getParent());
                }
                else
                    continue;
            }
            repositionMenuItems();
        }
    }
}

void ResearchMenu::repositionMenuItems()
{
    float yPosition = 0.0f;
    float yHousing = 0.0f;
    float yAmenity = 0.0f;
    float yCommerce = 0.0f;
    float yEducation = 0.0f;
    float yMilitary = 0.0f;
    float ySocial = 0.0f;
    
    for (int i = 0; i < menuItemArray.size(); ++i)
    {
        for (int j = 0; j < menuItemArray[i]->count(); ++j)
        {
            CCMenuItem* menuItem = (CCMenuItem*)menuItemArray[i]->objectAtIndex(j);
            
            if (menuItem->getTag() == -3)
            {
                menuItem->setPosition(CCPointMake(0, yPosition));
                continue;
            }
            
            
            Building* currBuilding = GameScene::getThis()->buildingHandler->getBuilding(menuItem->getTag());
            
            
            
            switch (currBuilding->buildingType)
            {
                case HOUSING:
                    menuItem->setPosition(CCPointMake(0, yHousing));
                    
                    yHousing -=(menuItem->boundingBox().size.height + 40);
                    
                    break;
                case AMENITY:
                    menuItem->setPosition(CCPointMake(0, yAmenity));
                    
                    yAmenity -=(menuItem->boundingBox().size.height + 40);
                    
                    
                    break;
                case COMMERCE:
                    menuItem->setPosition(CCPointMake(0, yCommerce));
                    
                    yCommerce -=(menuItem->boundingBox().size.height + 40);
                    
                    
                    break;
                case MILITARY:
                    menuItem->setPosition(CCPointMake(0, yMilitary));
                    
                    yMilitary -=(menuItem->boundingBox().size.height + 40);
                    
                    
                    break;
                case EDUCATION:
                    menuItem->setPosition(CCPointMake(0, yEducation));
                    
                    yEducation -=(menuItem->boundingBox().size.height + 40);
                    
                    
                    break;
                case SOCIAL:
                    menuItem->setPosition(CCPointMake(0, ySocial));
                    
                    ySocial -=(menuItem->boundingBox().size.height + 40);
                    
                    
                    break;
                default:
                    break;
            }
            
        }

        
    }

    /*
    for (int i = 0; i < menuItems->count(); i++)
    {
        CCMenuItem* menuItem = (CCMenuItem*)menuItems->objectAtIndex(i);
        
        menuItem->setPosition(CCPointMake(0, yPosition));
        yPosition -= (menuItem->boundingBox().size.height + 40);
    }*/
    
   // scrollArea->setScrollContentSize(CCSizeMake(0.0f, -yPosition));
}

void ResearchMenu::researchBuilding(CCMenuItem* sender, float time)
{
    if (GameScene::getThis()->researchHandler->isAlreadyResearching(sender->getTag()))
        return;
    
    GameScene::getThis()->researchHandler->addResearchingBuilding(sender->getTag(), time);
    CCLabelTTF* label = (CCLabelTTF*)sender->getChildByTag((int)"costLabel");
    
    addResearchingLabel(label, time);
}

void ResearchMenu::addResearchingLabel(CCLabelTTF* label, float time)
{
    std::stringstream ss;
    ss << "Research completing in " << (int)time + 1 << " seconds";
    label->setString(ss.str().c_str());
    
    if (!timeLabelsToUpdate->containsObject(label))
        timeLabelsToUpdate->addObject(label);
    
    if (!isUpdateScheduled)
    {
        isUpdateScheduled = true;
        this->schedule(schedule_selector(ResearchMenu::update), 0.25f);
    }
}

bool ResearchMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    touchBegin = tabbedView->menu->convertToNodeSpace(pTouch->getLocation());
    return true;
}

void ResearchMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
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

/*Due to the limitations of CCMenu (which always swallows touches, no exceptions), I need to implement my own touch logic and disable the way to click the menu.*/
void ResearchMenu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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