//
//  ProgressBar.h
//  PopulationTest
//
//  Created by Ong Heng Le on 17/4/13.
//
//

#ifndef PopulationTest_ProgressBar_h
#define PopulationTest_ProgressBar_h

#include "cocos2d.h"
using namespace cocos2d;

/* SUMMARY_
 *
 * ProgressBar is an UI element which shows a horizontal bar 
 * with a background and side images, and fills up to its value.
 * This acts as a parent node, which contains its own child nodes.
 *
 *
 *
 * Creation_
 *
 * Create ProgressBar object with the specified arguments.
 * bgBodyFileName and barFileName must be of POT dimensions,
 * and are textures repeated along the x-axis.
 *
 *
 *
 * Setting_Values_
 *
 * Use setValue(float) to set the fill of the progress bar.
 * Values are clamped between 0.0f and 1.0f.
 * getValue() returns the current value.
 *
 *
 *
 * Repositioning_
 *
 * Call CCNode::setPosition() of the progressBar,
 * and call reposition() afterwards.
 *
 */
class ProgressBar:public cocos2d::CCNode
{
    
private:
    
    // Background sprites
    CCSprite* bgBody;
    CCSprite* bgLeft;
    CCSprite* bgRight;
    
    // Bar fill and stored attributes
    CCSprite* barFill;
    CCRect barFillOffsetRect;
    CCSize barFillTexSize;
    float barFillScale;
    
    // Value of this progress bar
    float currentValue;
    
public:
    
    // Constructor / Destructor
    ~ProgressBar();
    
    static ProgressBar* create();
    
    
    // ProgressBar functions
    void createProgressBar(CCRect bgBodyRect, CCRect barOffsetRect,
                           const char* bgBodyFileName,
                           const char* bgLeftFileName,
                           const char* bgRightFileName,
                           const char* barFileName);
    void reposition();
    
    // Value functions
    float getValue();
    void setValue(float value);
};

#endif
