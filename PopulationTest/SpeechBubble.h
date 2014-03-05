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
    UNHAPPY=5
};


class SpeechBubble:public cocos2d::CCNode
{
    
    CCSprite* background;
    CCNode* contentNode;
    
    bool isHideScheduled;
    
    float orig_sizeX;
    float node_sizeX;
public:

    SpeechBubble();
    void createSpeechBubble();
    
    void addContent(CCNode* node, CCPoint offset);
    void addContent(SpeechMood s, CCPoint offset);
    void clearContent();
    
    void show(float time=5.0f);
    void hide();
    
    void rescale();
};

#endif /* defined(__PopulationTest__SpeechBubble__) */
