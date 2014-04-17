//
//  SpeechBubble.h
//  PopulationTest
//
//  Created by Ong Heng Le on 7/5/13.
//
//

#ifndef __PopulationTest__SpeechBubble__
#define __PopulationTest__SpeechBubble__

#include "cocos2d.h"
using namespace cocos2d;

enum SpeechMood
{
    IDLING=0,
    HAPPY,
    HUNGRY,
    STUCK,
    TIRED,
    GUARD_EMOTION,
    BUILDER_EMOTION,
    FARMER_EMOTION,
    TRANSPORT_EMOTION,
    BATTLE,
    STUCK_FOOD,
    FIND_HOME,
    HOMELESS,
    STEAL_MONEY,
    STEAL_FOOD,
    UNHAPPY=14
};


class SpeechBubble:public cocos2d::CCNode
{
    
    CCSprite* background;
    CCNode* contentNode;
    
    bool isHideScheduled;
    
    float orig_sizeX;
    float node_sizeX;
    
public:
    // this part is for the frame animation
    int frameHeight;
    int frameWidth;
    
    CCTexture2D* bubbleTexture;
    CCRect bubbleRect;
    int x_frameno;
    int x_maxframeno;
    int y_offset;
    
    float delay_animFrame;
    float delay_curr;
    
public:

    SpeechBubble();
    void createSpeechBubble();
    
    void addContent(CCNode* node, CCPoint offset);
    void addContent(SpeechMood s, CCPoint offset);
    void clearContent();
    
    void show(float time=5.0f);
    void hide();
    
    void rescale();
    
    void displayTransportBubble(float);
};

#endif /* defined(__PopulationTest__SpeechBubble__) */
