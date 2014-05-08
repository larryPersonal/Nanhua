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
#include "BuildingInfoMenu.h"
#include "SpriteInfoMenu.h"
#include "SelectPopulation.h"

// static pointer to the current open popup menu
static vector<PreviousMenu*> pOpenPopupMenu;

PopupMenu::PopupMenu()
{
    menuItems = NULL;
    menuItemPositions = NULL;
    canPressButtons = false;
}

PopupMenu::~PopupMenu()
{

}

void PopupMenu::cleanup()
{
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

void PopupMenu::backupCurrentPopupMenu()
{
    if(BuildingInfoMenu::getThis() != NULL)
    {
        PreviousMenu* pm = new PreviousMenu(BuInfoMenu, BuildingInfoMenu::getThis()->building);
        pOpenPopupMenu.push_back(pm);
        BuildingInfoMenu::getThis()->closeMenu(false);
    }
    
    if(SpriteInfoMenu::getThis() != NULL)
    {
        PreviousMenu* pm = new PreviousMenu(SpInfoMenu, SpriteInfoMenu::getThis()->gameSprite);
        pOpenPopupMenu.push_back(pm);
        SpriteInfoMenu::getThis()->closeMenu(false);
    }
    
    if(SelectPopulation::getThis() != NULL)
    {
        PreviousMenu* pm = new PreviousMenu(SePopulationMenu, SelectPopulation::getThis()->building);
        pOpenPopupMenu.push_back(pm);
        SelectPopulation::getThis()->closeMenu(false);
    }
}

void PopupMenu::useAsExclusivePopupMenu()
{
    closeAllPopupMenu();
    useAsTopmostPopupMenu();
}

void PopupMenu::useAsTopmostPopupMenu()
{
    if (GameHUD::getThis() != NULL)
    {
        GameHUD::getThis()->addChild(this, 5);
    }
    else
    {
        MainMenuScene::getThis()->addChild(this, 5);
    }
    this->createMenuItems();
}

int PopupMenu::openPopupCount()
{
    return pOpenPopupMenu.size();
}


void PopupMenu::closeAllPopupMenu()
{
    if(BuildingInfoMenu::getThis() != NULL)
    {
        BuildingInfoMenu::getThis()->closeMenu(false);
    }
    
    if(SpriteInfoMenu::getThis() != NULL)
    {
        SpriteInfoMenu::getThis()->closeMenu(false);
    }
    
    if(SelectPopulation::getThis() != NULL)
    {
        SelectPopulation::getThis()->closeMenu(false);
    }
    
    pOpenPopupMenu.clear();
}


void PopupMenu::closeMenu(bool openPreviousMenu)
{
    if(openPreviousMenu)
    {
        if(pOpenPopupMenu.size() > 0)
        {
            PreviousMenu* pm = pOpenPopupMenu.back();
            pOpenPopupMenu.pop_back();
            
            if(pm->pmt == BuInfoMenu)
            {
                CCLog("test1");
                BuildingInfoMenu* bim = BuildingInfoMenu::create(pm->b);
                //bim->autorelease();
                bim->useAsTopmostPopupMenu();
            }
            else if(pm->pmt == SpInfoMenu)
            {
                CCLog("test2");
                SpriteInfoMenu* sim = new SpriteInfoMenu(pm->gs);
                //sim->autorelease();
                sim->useAsTopmostPopupMenu();
            }
            else if(pm->pmt == SePopulationMenu)
            {
                CCLog("test3");
                SelectPopulation* sp = SelectPopulation::create(pm->b);
                //sp->autorelease();
                sp->useAsTopmostPopupMenu();
            }
            else
            {
                CCLog("test4");
                closeMenu(openPreviousMenu);
                return;
            }
        }
        else
        {
            CCLog("test5");
        }
    }
    else
    {
        CCLog("test6");
    }
    this->cleanup();
}

/*
void PopupMenu::scrollY(float scrollBy)
{
    this->setPositionY( this->getPositionY() + scrollBy);
}*/

void PopupMenu::willChangeOrientation()
{
    /*
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
    */
}

void PopupMenu::onOrientationChanged()
{
    /*
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    */
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


