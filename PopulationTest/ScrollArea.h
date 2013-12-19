//
//  ScrollArea.h
//  PopulationTest
//
//  Created by Ong Heng Le on 19/4/13.
//
//

#ifndef PopulationTest_ScrollArea_h
#define PopulationTest_ScrollArea_h

#include "cocos2d.h"
#include "CCScrollView.h"

using namespace cocos2d;
using namespace cocos2d::extension;

/* SUMMARY_
 *
 * ScrollArea is a scroll view which may contain horizontal and vertical scrollbars.
 * Following the standards of common scroll views, the vertical scrollbar is 
 * placed to the right and the horizontal scrollbar is placed to the bottom.
 * The view starts at the top-left of the scroll content.
 *
 *
 *
 * Creation_
 *
 * Create an instance of ScrollArea, and call createScrollArea()
 * with the scroll content size and the view size. 
 * You may then call enableScrollHorizontal() / enableScrollVertical() 
 * to enable scrolling and its scrollbars.
 *
 *
 *
 * Adding_Items_
 *
 * Call addItem() with the node and position to be added onto the
 * scroll content layer. (0, 0) is the top-left, and increasing
 * values move towards the bottom-right.
 * You may want to call setScrollContentSize() to fit the items.
 *
 *
 * 
 * Repositioning_
 *
 * Call CCNode::setPosition() of the ScrollArea,
 * and call reposition() afterwards.
 *
 *
 *
 * Update_
 *
 * update() is scheduled when either enableScrollHorizontal() / 
 * enableScrollVertical() is called, which in turn calls updateScrollBars().
 * The scrollbars' position and size is recalculated only when required.
 *
 */
class ScrollArea:public cocos2d::CCNode
{
    
private:
    
    // Extended CCScrollView to override registerWithTouchDispatcher()
    class ScrollViewEx:public cocos2d::extension::CCScrollView
    {
    public:
        static ScrollViewEx* create(CCSize size)
        {
            ScrollViewEx* pRet = new ScrollViewEx();
            if (pRet && pRet->initWithViewSize(size, NULL))
            {
                pRet->autorelease();
            }
            else
            {
                CC_SAFE_DELETE(pRet);
            }
            return pRet;
        }
        
        virtual void registerWithTouchDispatcher()
        {
            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
        }
    };
    
    // ScrollArea variables
    ScrollViewEx* scrollView;   // The scroll view
    CCLayer* layer;             // The scroll content layer
    bool isUpdateScheduled;

    // Horizontal scroll bar
    bool hScrollIsEnabled;
    CCSprite* hScrollBarTrack;  // Track is the scrollbar's background
    CCSprite* hScrollBarThumb;  // Thumb is the scrollbar's scroller
    CCSize hScrollBarThumbTexSize;
    float hScrollThickness;
    float hScrollLastOffset;
    float hScrollDistanceMax;
    
    // Vertical scroll bar
    bool vScrollIsEnabled;
    CCSprite* vScrollBarTrack;
    CCSprite* vScrollBarThumb;
    CCSize vScrollBarThumbTexSize;
    float vScrollThickness;
    float vScrollLastOffset;
    float vScrollDistanceMax;
    
public:
    
    // Cleanup
    virtual void cleanup();
    
    // Initialization
    void createScrollArea(CCSize viewSize, CCSize scrollContentSize);
    void enableScrollHorizontal(float thickness, const char* trackFileName, const char* thumbFileName);
    void enableScrollVertical(float thickness, const char* trackFileName, const char* thumbFileName);
    void hideScroll();
    
    // ScrollArea functions
    void addItem(CCNode* node, CCPoint position);
    void setViewSize(CCSize viewSize);
    void setScrollContentSize(CCSize scrollContentSize, bool resetContentOffset = false);
    void setScrollContentOffset(CCPoint scrollContentOffset);
    
    CCSize getViewSize();
    CCSize getScrollContentSize();
    CCPoint getScrollContentOffset();
    
    void reposition();
    void resizeScrollBars();
    
    // Update
    void updateScrollBars();
    virtual void update(float delta);
    virtual void onEnter();
    virtual void onExit();
};

#endif
