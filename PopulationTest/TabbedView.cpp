//
//  TabbedView.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 26/4/13.
//
//

#include "TabbedView.h"

void TabbedView::createTabbedView(CCPoint tabOrigin, CCPoint contentOrigin)
{
    currentIndex = 0;
    
    menu = CCMenu::create();
    menu->setPosition(tabOrigin);
    // Priority must be higher than its layers' menus
    menu->setTouchPriority(kCCMenuHandlerPriority -1);
    
    layers = LayerMultiplex::create();
    layers->setPosition(contentOrigin);
    
    
    this->addChild(menu);
}

void TabbedView::addTabWithContent(CCMenuItem* tab, CCPoint tabOffset, CCLayer* contentLayer, bool manualcall)
{
    if (!manualcall)
        tab->setTarget(this, menu_selector(TabbedView::onTabSelected));
    
    tab->setTag(menu->getChildrenCount());
    tab->setPosition(tabOffset);
    menu->addChild(tab);
    contentLayer->setTouchPriority(kCCMenuHandlerPriority);
    layers->addLayer(contentLayer);

    if (tab->getTag() == 0)
    {
        this->addChild(layers);
        onTabSelected(tab);
    }
}

void TabbedView::setOrigins(CCPoint tabOrigin, CCPoint contentOrigin)
{
    menu->setPosition(tabOrigin);
    layers->setPosition(contentOrigin);
}

void TabbedView::onTabSelected(CCObject* pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem*)(menu->getChildByTag(currentIndex));
    pMenuItem->unselected();
    
    pMenuItem = (CCMenuItem*)(pSender);
    pMenuItem->selected();
    int i = (int)pMenuItem->getTag();
    
    layers->switchTo(i);
    currentIndex = i;
}

/*for manual calls*/
void TabbedView::onTabSelected(cocos2d::CCMenuItem *tab)
{
    CCMenuItem* pMenuItem = (CCMenuItem*)(menu->getChildByTag(currentIndex));
    pMenuItem->unselected();
    pMenuItem = tab;
    pMenuItem->selected();
    int i = (int)pMenuItem->getTag();
    
    layers->switchTo(i);
    currentIndex = i;    
}