//
//  ScrollArea.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 19/4/13.
//
//

#include "ScrollArea.h"


void ScrollArea::cleanup()
{
    CCNode::cleanup();
    
    // Set update bool to false
    isUpdateScheduled = false;
}

void ScrollArea::createScrollArea(CCSize viewSize, CCSize scrollContentSize)
{
    isUpdateScheduled = false;
    hScrollIsEnabled = false;
    vScrollIsEnabled = false;
    
    // Scroll content should not be smaller than view
    if (scrollContentSize.width < viewSize.width)
    {
        scrollContentSize.width = viewSize.width;
    }
    
    if (scrollContentSize.height < viewSize.height)
    {
        scrollContentSize.height = viewSize.height;
    }
    
    // Create scroll content layer
    layer = CCLayer::create();
    layer->setContentSize(scrollContentSize);
    
    // Create scroll view
    scrollView = ScrollViewEx::create(viewSize);
    scrollView->setDirection(kCCScrollViewDirectionNone);
    scrollView->setBounceable(false);
    scrollView->setContainer(layer);
    scrollView->setContentOffset(ccp(0, viewSize.height - scrollContentSize.height));
    
    this->addChild(scrollView);
}

void ScrollArea::enableScrollHorizontal(float thickness, const char* trackFileName, const char* thumbFileName)
{
    // Enable horizontal scrollbar
    hScrollIsEnabled = true;
    hScrollThickness = thickness;
    hScrollLastOffset = 888.8f;
    hScrollDistanceMax = 0.0f;
    
    // Set direction of view
    if (vScrollIsEnabled)
        scrollView->setDirection(kCCScrollViewDirectionBoth);
    else
        scrollView->setDirection(kCCScrollViewDirectionHorizontal);
    
    
    // Create track
    if(trackFileName == NULL || thumbFileName == NULL) return;
    
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(trackFileName);
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    texture->setTexParameters(&params);
    hScrollBarTrack = CCSprite::createWithTexture(texture);
    hScrollBarTrack->setAnchorPoint(ccp(0, 1));
    hScrollBarTrack->setContentSize(CCSizeMake(scrollView->getViewSize().width, thickness));
    hScrollBarTrack->setTextureRect(CCRectMake(0, 0,
                                               hScrollBarTrack->getContentSize().width * (texture->getPixelsHigh() / thickness),
                                               texture->getPixelsHigh()));
    hScrollBarTrack->setScale(thickness / texture->getPixelsHigh());
    
    // Create thumb
    texture = CCTextureCache::sharedTextureCache()->addImage(thumbFileName);
    texture->setTexParameters(&params);
    hScrollBarThumb = CCSprite::createWithTexture(texture);
    hScrollBarThumb->setAnchorPoint(ccp(0, 1));
    hScrollBarThumb->setContentSize(hScrollBarTrack->getContentSize());
    hScrollBarThumbTexSize = texture->getContentSizeInPixels();
    hScrollBarThumb->setScale(hScrollBarTrack->getScale());
    
    float newWidth = hScrollBarTrack->getContentSize().width * (scrollView->getViewSize().width / layer->getContentSize().width);
    hScrollDistanceMax = hScrollBarThumb->getScale() * (hScrollBarTrack->getContentSize().width - newWidth);
    
    this->addChild(hScrollBarTrack);
    this->addChild(hScrollBarThumb);
    
    // Schedule update if not already scheduled
    if (!isUpdateScheduled)
    {
        isUpdateScheduled = true;
        this->schedule(schedule_selector(ScrollArea::update), 1/60.0f);
    }
}

void ScrollArea::hideScroll()
{
   if (vScrollBarTrack)
   {
    vScrollBarTrack->setVisible(false);
       vScrollBarThumb->setVisible(false);
   }
    if (hScrollBarTrack)
    {
    hScrollBarTrack->setVisible(false);
        hScrollBarThumb->setVisible(false);
    }
}


void ScrollArea::enableScrollVertical(float thickness, const char* trackFileName, const char* thumbFileName)
{
    // Enable vertical scrollbar
    vScrollIsEnabled = true;
    vScrollThickness = thickness;
    vScrollLastOffset = 888.8f;
    vScrollDistanceMax = 0.0f;
    
    // Set direction of view 
    if (hScrollIsEnabled)
        scrollView->setDirection(kCCScrollViewDirectionBoth);
    else
        scrollView->setDirection(kCCScrollViewDirectionVertical);
    
    // Create track
    if(trackFileName == NULL || thumbFileName == NULL) return;
    
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(trackFileName);
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    texture->setTexParameters(&params);
    vScrollBarTrack = CCSprite::createWithTexture(texture);
    vScrollBarTrack->setAnchorPoint(CCPointZero);
    vScrollBarTrack->setContentSize(CCSizeMake(thickness, scrollView->getViewSize().height));
    vScrollBarTrack->setTextureRect(CCRectMake(0, 0,
                                    texture->getPixelsWide(),
                                    vScrollBarTrack->getContentSize().height * (texture->getPixelsWide() / thickness)));
    vScrollBarTrack->setScale(thickness / texture->getPixelsWide());
    
    // Create thumb
    texture = CCTextureCache::sharedTextureCache()->addImage(thumbFileName);
    texture->setTexParameters(&params);
    vScrollBarThumb = CCSprite::createWithTexture(texture);
    vScrollBarThumb->setAnchorPoint(CCPointZero);
    vScrollBarThumb->setContentSize(vScrollBarTrack->getContentSize());
    vScrollBarThumbTexSize = texture->getContentSizeInPixels();
    vScrollBarThumb->setScale(vScrollBarTrack->getScale());
    
    float newHeight = vScrollBarTrack->getContentSize().height * (scrollView->getViewSize().height / layer->getContentSize().height);
    vScrollDistanceMax = vScrollBarThumb->getScale() * (vScrollBarTrack->getContentSize().height - newHeight);
    
    this->addChild(vScrollBarTrack);
    this->addChild(vScrollBarThumb);
    
    // Schedule update if not already scheduled
    if (!isUpdateScheduled)
    {
        isUpdateScheduled = true;
        this->schedule(schedule_selector(ScrollArea::update), 1/60.0f);
    }
}

void ScrollArea::addItem(CCNode* node, CCPoint position)
{
    // Add items with (0, 0) being top left,
    // and going towards bottom right with increasing values
    layer->addChild(node);
    node->setAnchorPoint(ccp(0, 1));
    position.y = layer->getContentSize().height - position.y;
    node->setPosition(position);
}

void ScrollArea::removeI(CCNode* node)
{
    layer->removeChild(node);
}

void ScrollArea::setViewSize(CCSize viewSize)
{
    scrollView->setViewSize(viewSize);
    resizeScrollBars();
    reposition();
}

void ScrollArea::setScrollContentSize(CCSize scrollContentSize, bool resetContentOffset)
{
    // Scroll content should not be smaller than view
    if (scrollContentSize.width < scrollView->getViewSize().width)
        scrollContentSize.width = scrollView->getViewSize().width;
    if (scrollContentSize.height < scrollView->getViewSize().height)
        scrollContentSize.height = scrollView->getViewSize().height;
    
    if (scrollContentSize.height != layer->getContentSize().height)
    {
        int diff = scrollContentSize.height - layer->getContentSize().height;
        for (int i = 0; i < layer->getChildrenCount(); i++)
        {
            CCNode* node = (CCNode*)layer->getChildren()->objectAtIndex(i);
            node->setPositionY(node->getPositionY() + diff);
        }
        
        // Set content offset for height
        CCPoint offset = scrollView->getContentOffset();
        offset.y -= diff;
        scrollView->setContentOffset(offset);
        
        if (resetContentOffset)
            scrollView->setContentOffset(ccp(0, scrollView->getViewSize().height - scrollContentSize.height));
        else
        {
            if (scrollView->getContentOffset().x < scrollView->getViewSize().width - scrollContentSize.width)
                scrollView->setContentOffset(ccp(scrollView->getViewSize().width - scrollContentSize.width, scrollView->getContentOffset().y));
            
            if (scrollView->getContentOffset().y < scrollView->getViewSize().height - scrollContentSize.height)
                scrollView->setContentOffset(ccp(scrollView->getContentOffset().x, scrollView->getViewSize().height - scrollContentSize.height));
        }
    }
    
    layer->setContentSize(scrollContentSize);
    
    if (hScrollIsEnabled)
    {
        float newWidth = hScrollBarTrack->getContentSize().width * (scrollView->getViewSize().width / layer->getContentSize().width);
        hScrollDistanceMax = hScrollBarThumb->getScale() * (hScrollBarTrack->getContentSize().width - newWidth);
    }
    
    if (vScrollIsEnabled)
    {
        float newHeight = vScrollBarTrack->getContentSize().height * (scrollView->getViewSize().height / layer->getContentSize().height);
        vScrollDistanceMax = vScrollBarThumb->getScale() * (vScrollBarTrack->getContentSize().height - newHeight);
    }
}

void ScrollArea::setScrollContentOffset(CCPoint scrollContentOffset)
{
    scrollView->setContentOffset(scrollContentOffset);
}

CCSize ScrollArea::getViewSize()
{
    return scrollView->getViewSize();
}

CCSize ScrollArea::getScrollContentSize()
{
    return layer->getContentSize();
}

CCPoint ScrollArea::getScrollContentOffset()
{
    return scrollView->getContentOffset();
}

void ScrollArea::reposition()
{
    if (hScrollIsEnabled)
    {
        // Reposition horizontal scrollbar
        hScrollBarTrack->setPosition(ccp(0.0f, hScrollThickness));
        hScrollBarThumb->setPositionY(hScrollThickness);
    }
    
    if (vScrollIsEnabled)
    {
        // Reposition vertical scrollbar
        vScrollBarTrack->CCNode::setPosition(scrollView->getViewSize().width - vScrollThickness, 0.0f);
        vScrollBarThumb->setPositionX(scrollView->getViewSize().width - vScrollThickness);
    }
}

void ScrollArea::resizeScrollBars()
{
    if (hScrollIsEnabled)
    {
        // Resize horizontal scrollbar
        hScrollBarTrack->setContentSize(CCSizeMake(scrollView->getViewSize().width, hScrollThickness));
        hScrollBarTrack->setTextureRect(CCRectMake(0, 0,
                                                   hScrollBarTrack->getContentSize().width * (hScrollBarTrack->getTexture()->getPixelsHigh() / hScrollThickness),
                                                   hScrollBarTrack->getTexture()->getPixelsHigh()));
        
        float newWidth = hScrollBarTrack->getContentSize().width * (scrollView->getViewSize().width / layer->getContentSize().width);
        hScrollDistanceMax = hScrollBarThumb->getScale() * (hScrollBarTrack->getContentSize().width - newWidth);
    }
    
    if (vScrollIsEnabled)
    {
        // Resize vertical scrollbar
        vScrollBarTrack->setContentSize(CCSizeMake(vScrollThickness, scrollView->getViewSize().height));
        vScrollBarTrack->setTextureRect(CCRectMake(0, 0,
                                                   vScrollBarTrack->getTexture()->getPixelsWide(),
                                                   vScrollBarTrack->getContentSize().height * (vScrollBarTrack->getTexture()->getPixelsWide() / vScrollThickness)));
        
        float newHeight = vScrollBarTrack->getContentSize().height * (scrollView->getViewSize().height / layer->getContentSize().height);
        vScrollDistanceMax = vScrollBarThumb->getScale() * (vScrollBarTrack->getContentSize().height - newHeight);
    }
}

void ScrollArea::updateScrollBars()
{
    /*
    // Update horizontal scrollbar if enabled and has moved
    if (hScrollIsEnabled &&
        scrollView->getContentOffset().x != hScrollLastOffset)
    {
        float scrollOffset = -scrollView->getContentOffset().x;
        float scrollOffsetMax = layer->getContentSize().width - scrollView->getViewSize().width;
        float viewSize = scrollView->getViewSize().width;
        
        if (scrollOffset < scrollOffsetMax)
        {
            if (scrollOffset < 0.0f)
            {
                // Reached start of scrollbar
                //viewSize += scrollOffset;
                if (viewSize < 0.0f)
                    viewSize = 0.0f;
                scrollOffset = 0.0f;
            }
            float newWidth = hScrollBarTrack->getContentSize().width * (viewSize / layer->getContentSize().width);
            //hScrollBarThumb->setTextureRect(CCRectMake(0, 0, newWidth, hScrollBarThumbTexSize.height));
            
            //hScrollBarThumb->setPositionX((scrollOffsetMax == 0.0f ? 0.0f : (scrollOffset / scrollOffsetMax)) * hScrollDistanceMax);
        }
        else
        {
            // Reached end of scrollbar
            //viewSize += scrollOffsetMax - scrollOffset;
            if (viewSize < 0.0f)
                viewSize = 0.0f;
            
            float newWidth = hScrollBarTrack->getContentSize().width * (viewSize / layer->getContentSize().width);
            //hScrollBarThumb->setTextureRect(CCRectMake(0, 0, newWidth, hScrollBarThumbTexSize.height));
            
            //hScrollBarThumb->setPositionX(hScrollBarThumb->getScale() * (hScrollBarTrack->getContentSize().width - newWidth));
        }
    }
    */
    
    /*
    // Update vertical scrollbar if enabled and has moved
    if (vScrollIsEnabled &&
        scrollView->getContentOffset().y != vScrollLastOffset)
    {
        float scrollOffset = -scrollView->getContentOffset().y;
        float scrollOffsetMax = layer->getContentSize().height - scrollView->getViewSize().height;
        float viewSize = scrollView->getViewSize().height;
        
        if (scrollOffset < scrollOffsetMax)
        {
            if (scrollOffset < 0.0f)
            {
                // Reached start of scrollbar
                viewSize += scrollOffset;
                if (viewSize < 0.0f)
                    viewSize = 0.0f;
                scrollOffset = 0.0f;
            }
            float newHeight = vScrollBarTrack->getContentSize().height * (viewSize / layer->getContentSize().height);
            vScrollBarThumb->setTextureRect(CCRectMake(0, 0, vScrollBarThumbTexSize.width, newHeight));
            
            vScrollBarThumb->setPositionY((scrollOffsetMax == 0.0f ? 0.0f : (scrollOffset / scrollOffsetMax)) * vScrollDistanceMax);
        }
        else
        {
            // Reached end of scrollbar
            viewSize += scrollOffsetMax - scrollOffset;
            if (viewSize < 0.0f)
                viewSize = 0.0f;
            
            float newHeight = vScrollBarTrack->getContentSize().height * (viewSize / layer->getContentSize().height);
            vScrollBarThumb->setTextureRect(CCRectMake(0, 0, vScrollBarThumbTexSize.width, newHeight));
            
            vScrollBarThumb->setPositionY(vScrollBarThumb->getScale() * (vScrollBarTrack->getContentSize().height - newHeight));
        }
    }
    */
}

void ScrollArea::update(float delta)
{
    CCNode::update(delta);
    
    updateScrollBars();
}

void ScrollArea::onEnter()
{
    CCNode::onEnter();
    
    // Schedule update if not already scheduled
    if ((hScrollIsEnabled || vScrollIsEnabled) &&
        !isUpdateScheduled)
    {
        isUpdateScheduled = true;
        this->schedule(schedule_selector(ScrollArea::update), 1/60.0f);
    }
}

void ScrollArea::onExit()
{
    CCNode::onExit();
    
    // Schedule update if not already scheduled
    if ((hScrollIsEnabled || vScrollIsEnabled) &&
        isUpdateScheduled)
    {
        isUpdateScheduled = false;
        this->unschedule(schedule_selector(ScrollArea::update));
    }
}

CCLayer* ScrollArea::getLayer()
{
    return layer;
}

void ScrollArea::stopScroll()
{
    scrollView->canScroll = false;
}

void ScrollArea::resumeScroll()
{
    scrollView->canScroll = true;
}
