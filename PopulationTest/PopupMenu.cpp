//
//  PopupMenu.cpp
//  PopulationTest
//
//  Created by Serious Games on 14/3/13.
//
//

#include "PopupMenu.h"
#include "GameHUD.h"
#include "MainMenuScene.h"

// static pointer to the current open popup menu
static vector<PopupMenu*> pOpenPopupMenu;


PopupMenu::PopupMenu()
{
    menuItems = NULL;
    menuItemPositions = NULL;
    menuIsOpen = false;
    canPressButtons = false;
    setTouchEnabled(true);
}

PopupMenu::~PopupMenu()
{
//    if (menuItems != NULL) CCLog("menuitems not null!");
 //   if (menuItemPositions != NULL) CCLog("positions not null!");

}


void PopupMenu::cleanup()
{

    // Another check here, in case this gets destroyed without proper call
    if (pOpenPopupMenu.back() == this)
        pOpenPopupMenu.pop_back();

    if (this->getChildrenCount() > 0)
    {
      
        this->removeAllChildren();
    }
    CCLayer::cleanup();
    CCLog("Menu Items Uncreated");
    
    if (menuItems != NULL)
    {
        menuItems->removeAllObjects();
        menuItems->release();
        
        menuItems = NULL;
    }
    if (menuItemPositions != NULL)
    {
        menuItemPositions->release();
        menuItemPositions = NULL;
    }
    this->removeFromParentAndCleanup(false);    //cleanup should be called by closeMenu()
}

void PopupMenu::useAsBasePopupMenu()
{
    if (pOpenPopupMenu.size() > 0)
    {
        CCLog("Popup already open, release!");
        this->closeMenu(false);
        return;
    }
    useAsTopmostPopupMenu();
}

void PopupMenu::useAsExclusivePopupMenu()
{
    closeAllPopupMenu(false);
    useAsTopmostPopupMenu();
}

void PopupMenu::useAsTopmostPopupMenu()
{
    pOpenPopupMenu.push_back(this);
    if (GameHUD::getThis() !=NULL)
        GameHUD::getThis()->addChild(this, 1);
    else
        MainMenuScene::getThis()->addChild(this, 1);
    this->createMenuItems();
}

PopupMenu* PopupMenu::getTopmostPopupMenu()
{
    return pOpenPopupMenu.back();
}

PopupMenu* PopupMenu::getBottommostPopupMenu()
{
    return pOpenPopupMenu.front();
}


int PopupMenu::openPopupCount()
{
    return pOpenPopupMenu.size();
}


void PopupMenu::closeAllPopupMenu(bool toFullyClose)
{
    while (pOpenPopupMenu.size() > 0){
        pOpenPopupMenu.back()->closeMenu(toFullyClose);
    }
}


void PopupMenu::closeMenu(bool toFullyClose)
{
    if (toFullyClose)
    {
        if (GameHUD::getThis() != NULL)
        GameHUD::getThis()->menuIsOpen = false;
    }
    
    
    
    if (pOpenPopupMenu.back() == this)
        pOpenPopupMenu.pop_back();
    
        this->cleanup();
}

/*
void PopupMenu::scrollY(float scrollBy)
{
    this->setPositionY( this->getPositionY() + scrollBy);
}*/

void PopupMenu::willChangeOrientation()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    for (int x = 0; x < menuItems->count(); x++) {
        CCMenuItemSprite* thatItemImage = (CCMenuItemSprite*)menuItems->objectAtIndex(x);
        CCPoint oldPosition = thatItemImage->getPosition();
        CCPoint newPosition;
        newPosition = ccp(((float)oldPosition.x / (float)screenSize.width) * 100, ((float)oldPosition.y / (float)screenSize.height) * 100);
        
        if (menuItemPositions->count() < menuItems->count()) {
            menuItemPositions->addControlPoint(newPosition);
        }
        else{
            menuItemPositions->getControlPointAtIndex(x).setPoint(newPosition.x, newPosition.y);
        }
    }
}

void PopupMenu::onOrientationChanged()
{
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
}

void PopupMenu::registerWithTouchDispatcher()
{
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool PopupMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void PopupMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    return;
}


