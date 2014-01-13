//
//  SpriteRow.h
//  PopulationTest
//
//  Created by admin on 13/1/14.
//
//

#ifndef PopulationTest_SpriteRow_h
#define PopulationTest_SpriteRow_h

#include "cocos2d.h"
#include "Sprite.h"
#include "ScrollArea.h"
using namespace cocos2d;

class SpriteRow : CCObject
{
private:
    CCSprite* villagerImage;
    CCLabelTTF* villagerNameLabel;
    CCLabelTTF* villagerGenderLabel;
    
    CCLabelTTF* villagerIntLabel;
    CCLabelTTF* villagerLoyLabel;
    CCLabelTTF* villagerSocLabel;
    CCLabelTTF* villagerHapLabel;
    
    CCMenuItemImage* villagerSelectButton;
    
    GameSprite* gameSprite;
    ScrollArea* scrollArea;
    
    int index;
    
public:
    SpriteRow(GameSprite*, ScrollArea*, int);
    ~SpriteRow();
    
    static SpriteRow* create(GameSprite*, ScrollArea*, int);
    
    void init();
    void clear();
    
    CCSprite* getVillagerImage();
    CCLabelTTF* getVillagerNameLabel();
    CCLabelTTF* getVillagerGenderLabel();
    CCLabelTTF* getVillagerIntLabel();
    CCLabelTTF* getVillagerLoyLabel();
    CCLabelTTF* getVillagerSocLabel();
    CCLabelTTF* getVillagerHapLabel();
    
    CCMenuItemImage* getVillagerSelectButton();
};

#endif
