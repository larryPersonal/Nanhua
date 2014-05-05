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
    STUCK_FOOD,
    FIND_HOME,
    HOMELESS,
    STEAL_MONEY,
    STEAL_FOOD,
    UNHAPPY,
    FIND_BANDIT,
    BATTLE,
    GUARD_EMOTION,
    BUILDER_EMOTION,
    FARMER_EMOTION,
    TRANSPORT_EMOTION
};


class SpeechBubble:public cocos2d::CCNode
{
    
    CCSprite* background;
    CCNode* contentNode;
    CCSprite* emotionSprite;
    
    bool isHideScheduled;
    
    float orig_sizeX;
    float node_sizeX;
    
    bool hasAnimation;
    
public:
    // this part is for the frame animation
    int frameHeight;
    int frameWidth;
    
    CCTexture2D* bubbleTexture;
    CCRect bubbleRect;
    int x_frameno;
    int x_maxframeno;
    int x_offset;
    int y_offset;
    
    float delay_animFrame;
    float delay_curr;
    
    int numberOfElementInOneRow;
    int startElementId;
    
public:

    SpeechBubble();
    void createSpeechBubble();
    
    void addContent(CCNode* node, CCPoint offset);
    void addContent(std::string texture, CCPoint offset, int maxFrameNo, int numberOfFrames, int startID, float frameW = 64, float frameH = 64);
    void clearContent();
    
    void show(float time=5.0f);
    void hide();
    
    void rescale();
    
    void update(float);
};

#endif /* defined(__PopulationTest__SpeechBubble__) */
