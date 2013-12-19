//
//  InfoBar.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 15/4/13.
//
//

#include "InfoBar.h"
#include "GameHUD.h"

InfoBar::InfoBar()
{
    // Default values
    position = CCPointZero;
    size = CCSizeZero;
    initialSize = CCSizeZero;
    
    // Default anchored offsets, similar to CCS attributes,
    // negative value for no anchor
    left = -1;
    top = -1;
    right = -1;
    bottom = -1;
}

InfoBar::~InfoBar()
{
    // Delete and remove child items
    removeAllItems();
}

InfoBar* InfoBar::create()
{
    InfoBar *pRet = new InfoBar();
    if (pRet)
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

void InfoBar::createInfoBar(float width, float height, const char* bgFileName)
{
    // Set size
    size.width = width;
    size.height = height;
    initialSize = size;
    
    // Create the repeatable texture in cache
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(bgFileName);
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    texture->setTexParameters(&params);
    
    // Create background sprite and store attributes
    background = CCSprite::createWithTexture(texture);
    background->setAnchorPoint(CCPointZero);
    spriteSize = background->getContentSize();
    backgroundTexSize = texture->getContentSizeInPixels();
    
    // Initialize
    reposition();
    
    // Add background to this layer
    this->addChild(background, 1);
}

void InfoBar::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // Construct x and width based on left / right anchors
    if (left >= 0.0f && right >= 0.0f)
    {
        position.x = left;
        size.width = screenSize.width - (left + right);
    }
    else if (left >= 0.0f)
    {
        position.x = left;
        size.width = initialSize.width;
    }
    else if (right >= 0.0f)
    {
        size.width = initialSize.width;
        position.x = screenSize.width - (size.width + right);
    }
    
    // Construct y and height based on top / bottom anchors
    if (top >= 0.0f && bottom >= 0.0f)
    {
        position.y = bottom;
        size.height = screenSize.height - (top + bottom);
    }
    else if (top >= 0.0f)
    {
        size.height = initialSize.height;
        position.y = screenSize.height - (size.height + bottom);
    }
    else if (bottom >= 0.0f)
    {
        position.y = bottom;
        size.height = initialSize.height;
    }
    
    // Set position and size
    background->setPosition(position);
    background->setContentSize(size);
    
    // Construct width and height for scaling
    float aspectRatio = size.width / size.height;
    float texAspectRatio = backgroundTexSize.width / backgroundTexSize.height;
    int width = 0, height = 0;
    if (aspectRatio >= texAspectRatio)
    {
        width = size.width * (backgroundTexSize.height / size.height);
        height = backgroundTexSize.height;
    }
    else
    {
        width = backgroundTexSize.width;
        height = size.height * (backgroundTexSize.width / size.width);
    }
    
    // Set texture rect and scale it to desired size
    background->setTextureRect(CCRectMake(0, 0, width, height));
    background->setScale(aspectRatio >= texAspectRatio ? size.width / width : size.height / height);
    
    // Reposition all child items too
    repositionAllItems();
}

void InfoBar::onOrientationChanged()
{
    reposition();
}




void InfoBar::addItem(string key, CCNode* item)
{
    // Add item to dictionary and set its parent to this layer
    itemDictionary[key] = InfoBarItem(item);
    this->addChild(item, itemDictionary.size()+1);
    repositionItem(key);
}

void InfoBar::removeItem(string key)
{
    // Remove from dictionary and this layer
    this->removeChild(itemDictionary.at(key).item);
    itemDictionary.erase(key);
}

void InfoBar::anchorItem(string key, float offsetX, InfoBarAnchor anchor)
{
    itemDictionary.at(key).offsetX = offsetX;
    itemDictionary.at(key).anchor = anchor;
    switch (anchor)
    {
        case ANCHOR_LEFT:
            itemDictionary.at(key).item->setAnchorPoint(ccp(0.0f, 0.5f));
            break;
        case ANCHOR_CENTER:
            itemDictionary.at(key).item->setAnchorPoint(ccp(0.5f, 0.5f));
            break;
        case ANCHOR_RIGHT:
            itemDictionary.at(key).item->setAnchorPoint(ccp(1.0f, 0.5f));
            break;
    }
    repositionItem(key);
}

void InfoBar::repositionItem(string key)
{
    InfoBarItem infoBarItem = itemDictionary.at(key);
    
    switch (infoBarItem.anchor)
    {
        case ANCHOR_LEFT:
            infoBarItem.item->setPosition(position.x + infoBarItem.offsetX, position.y + (size.height / 2.0f));
            break;
        case ANCHOR_CENTER:
            infoBarItem.item->setPosition((position.x + (size.width / 2.0f)) + infoBarItem.offsetX, position.y + (size.height / 2));
            break;
        case ANCHOR_RIGHT:
            infoBarItem.item->setPosition((position.x + size.width + 5) - infoBarItem.offsetX, position.y + (size.height / 2.0f));
            break;
    }
}

CCNode* InfoBar::getItem(string key)
{
    return itemDictionary.at(key).item;
}



void InfoBar::repositionAllItems()
{
    // Iterate all in itemDictionary and call repositionItem
    for (map<string, InfoBarItem>::iterator it=itemDictionary.begin();
         it!=itemDictionary.end(); ++it)
    {
        repositionItem(it->first);
    }
}

void InfoBar::removeAllItems()
{
    // Iterate and removeItem at the same time
    map<string, InfoBarItem>::iterator it = itemDictionary.begin();
    while (it != itemDictionary.end()) {
        map<string, InfoBarItem>::iterator toErase = it;
        ++it;
        removeItem(toErase->first);
    }
}

