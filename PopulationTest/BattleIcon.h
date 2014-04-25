//
//  BattleIcon.h
//  PopulationTest
//
//  Created by admin on 25/4/14.
//
//

#ifndef PopulationTest_BattleIcon_h
#define PopulationTest_BattleIcon_h

#include "cocos2d.h"
#include "Sprite.h"
using namespace cocos2d;

class BattleIcon : public cocos2d::CCNode
{
public:
    CCSprite* battleSprite;
    GameSprite* parent;
    
    // this part is for the frame animation
    int frameHeight;
    int frameWidth;
    
    CCTexture2D* battleIconTexture;
    CCRect battleIconRect;
    int x_frameno;
    int x_maxframeno;
    int x_offset;
    int y_offset;
    
    float delay_animFrame;
    float delay_curr;
    
public:
    static BattleIcon* create(GameSprite* parentGS);
    BattleIcon(GameSprite* parentGS);
    ~BattleIcon();
    
    void update(float deltaTime);
    void show();
    void hide();
    
    void adjustPosition();
};

#endif
