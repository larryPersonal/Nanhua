//
//  AlertBox.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 20/5/13.
//
//

#include "AlertBox.h"

AlertBox::AlertBox()
{
    background = NULL;
    displayText = NULL;
    callback = NULL;
    
    close_called = false;
}

AlertBox::~AlertBox()
{
}

/*
bool AlertBox::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    return true;

}*/


void AlertBox::cleanup()
{
        
    // this->removeAllChildren();
   // this->removeAllChildrenWithCleanup(true);
    this->removeChild(background);
    this->removeChild(displayText);
    this->removeChild(buttonMenu);
   // displayText->release();

 ///   if (callback != NULL) callback->release();
  //  callback = NULL;

    
    PopupMenu::cleanup();
    
    
}

AlertBox* AlertBox::create()
{
    AlertBox *pRet = new AlertBox();
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
void AlertBox::createMenuItems()
{
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(0);
    menuItemPositions->retain();
    
    background = CCSprite::create("outerbox.png");
    background->setScale(500.0f / background->getContentSize().height);
    
    backgroundOverlay = CCSprite::create("board-alert.png");
    backgroundOverlay->setScale(background->getScale());
    
    
    displayText = CCLabelTTF::create("", "Droidiga", 26, CCSizeMake(background->boundingBox().size.width - 100.0f, 300.0f),
                                     kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    displayText->setAnchorPoint(ccp(0.5f, 1.0f));
    
    buttonMenu = CCMenu::createWithArray(menuItems);
    buttonMenu->setAnchorPoint(ccp(0.5f, 0.0f));
    
    this->addChild(background);
    this->addChild(backgroundOverlay);
    this->addChild(displayText);
    this->addChild(buttonMenu);
    
    reposition();
}

void AlertBox::willChangeOrientation()
{
}

void AlertBox::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    reposition();
}

void AlertBox::onMenuItemSelected(CCObject* pSender)
{
    CCMenuItem* item = (CCMenuItem*)pSender;
    
    if (item->getUserData() == NULL)
        this->closeMenu(true);
    else
    {
        CCAction* cb = (CCAction*)item->getUserData();
        item->runAction(cb);
        CCCallFuncO* call = (CCCallFuncO*)callback;
        CC_SAFE_RELEASE(call);        // donttrytoreleaseadynamiccastedobjectyoudolt->release();
        
        callback = NULL;
       //delete cb;
        //cleanup();
     //   this->closeMenu(true);
    }
    
}

void AlertBox::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    background->CCNode::setPosition(screenSize.width / 2.0f, screenSize.height / 2.0f);
    backgroundOverlay->CCNode::setPosition(screenSize.width / 2.0f, screenSize.height / 2.0f);
    displayText->CCNode::setPosition(background->getPositionX(), background->boundingBox().getMaxY() - 50);
    displayText->setColor(ccc3(81, 77, 2));
    buttonMenu->CCNode::setPosition(background->getPositionX(), background->boundingBox().getMinY() + 100);
}

void AlertBox::setDisplayText(string text)
{
    displayText->setString(text.c_str());
}

void AlertBox::addButton(int tag, string buttonText, CCObject* pSelectorTarget, SEL_CallFuncO selector, CCObject* pSender)
{
    CCMenuItemImage* button = CCMenuItemImage::create("nav button.png", "nav button-pressed.png", this, menu_selector(AlertBox::onMenuItemSelected));
    
    button->setTag(tag);
    button->setScale(75.0f / button->getContentSize().height);
    
    if (selector != NULL)
    {
        callback = CCCallFuncO::create(pSelectorTarget, selector, pSender);
        callback->retain();
        button->setUserData(callback);
        
        
        
    }
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create(buttonText.c_str(), "Droidiga", 30.0f);
    buttonLabel->setColor(ccc3(111, 107, 50));
    buttonLabel->setScale(1.0f / button->getScale());
    buttonLabel->CCNode::setPosition(button->getContentSize().width / 2.0f, button->getContentSize().height / 2.0f);
    button->addChild(buttonLabel);
    
    buttonMenu->addChild(button);
    buttonMenu->alignItemsHorizontallyWithPadding(10.0f);
}

void AlertBox::removeButton(int tag)
{
    buttonMenu->removeChildByTag(tag);
}
