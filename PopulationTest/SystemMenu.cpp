//
//  SystemMenu.cpp
//  PopulationTest
//
//  Created by Serious Games on 27/5/13.
//
//

#include "SystemMenu.h"
SystemMenu::~SystemMenu()
{
    
    scrollArea->removeAllChildren();
    scrollArea->release();
    
    
}

void SystemMenu::createMenuItems()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float yPosition = 0.0f;
    
    /*policy menu*/
    menuItems = CCArray::createWithCapacity(4);
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    //Demolish & Cancel
    CCSprite* demoNormalImage =     CCSprite::create("demolish.png");
    CCSprite* demoSelectedImage =   CCSprite::create("demolish.png");
    demoNormalImage->               setScale(128 / demoNormalImage->boundingBox().size.width);
    demoSelectedImage->             setScale(demoNormalImage->getScale());
    CCMenuItemSprite* demoItem =    CCMenuItemSprite::create(demoNormalImage, demoSelectedImage, this, menu_selector(SystemMenu::onMenuItemSelected) );
    
    demoItem->  setTag(-1);
    demoItem->  setAnchorPoint(ccp(0, 1));
    demoItem->  setContentSize(demoNormalImage->boundingBox().size);
    
    CCLabelTTF* demoLabel =     CCLabelTTF::create("Demolish", "Shojumaru-Regular", 30);
    
    CCPoint labelPosition = ccp(demoItem->boundingBox().size.width*2.5,
                                demoItem->boundingBox().size.height*0.5);
    
    demoItem->  setPosition(CCPointMake(0, yPosition));
    
    demoLabel->     setPosition(labelPosition);
    
    demoItem->addChild(demoLabel);
    
    menuItems->addObject(demoItem);
    
    CCMenu* menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Build Menu UI vars
    CCSize PolicyMenuSize = CCSizeMake(622.0f, 370.0f);
  //  ccColor3B colorYellow = ccc3(225, 219, 108);
    
    // Scroll Area
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(PolicyMenuSize, CCSizeMake(0.0f, -(yPosition - 200)));
    scrollArea->addItem(menu, ccp(20.0f, 0.0f));
    scrollArea->enableScrollVertical(10, "bar.png", "bar.png");
    scrollArea->reposition();
    scrollArea->updateScrollBars();
    
    
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
    tabLayerSystem = CCLayer::create();
    tabLayerSystem->setContentSize(PolicyMenuSize);
    tabLayerSystem->addChild(scrollArea);
    
    // Tab Items
    tabItemSystem= CCMenuItemImage::create("nav button.png", "nav button-pressed.png");
   // tabItemSystem->setAnchorPoint(CCPointZero);
    tabItemSystem->setVisible(false);
//    tabItemSystem->setScale(tabbedViewBg->getScale());
 //   CCLabelTTF* label = CCLabelTTF::create("System", "Shojumaru-Regular", 32);
 //   label->setColor(colorYellow);
  //  label->setScale(1.0f / tabItemSystem->getScale());
   // label->CCNode::setPosition(tabItemSystem->getContentSize().width / 2.0f, tabItemSystem->getContentSize().height / 2.0f);
   // tabItemSystem->addChild(label);
   
    // Create tabbed view and add its items
    tabbedView->createTabbedView(ccp(0.0f, tabbedViewBg->getContentSize().height * tabbedViewBg->getScale()),
                                 ccp(42.0f, 55.0f));
    tabbedView->addChild(tabbedViewBg);
    tabbedView->addChild(tabbedViewBgInner);
    tabbedView->addTabWithContent(tabItemSystem, ccp(218.0f, -20.0f), tabLayerSystem);
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    
    this->addChild(tabbedView);
    
    TitleLabel = CCLabelTTF::create("System", "Vinland", 64);
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
    this->addChild(TitleLabel,4);
    
    //    Animate();
}

void SystemMenu::onMenuItemSelected(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    
    
    switch (tag)
    {
    }
}

void SystemMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
}

