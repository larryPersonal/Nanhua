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
#include "Building.h"
#include "ScrollArea.h"
#include "ProgressBar.h"
using namespace cocos2d;

class SpriteRow : CCLayer
{
private:
    
    int mSpriteRowEnergyCurrent;
    int mSpriteRowEnergyRequired;
    std::string mSpriteRowSpriteName;
    
    CCSprite* spriteRowBackground;
    CCSprite* spriteRowMask;
    
    CCMenuItemImage* villagerImage;
    CCMenuItemImage* buttonCollider;
    
    ProgressBar* villagerEnergyBar;
    CCLabelTTF* villagerEnergyLabel;
    
    GameSprite* gameSprite;
    ScrollArea* scrollArea;
    Building* building;
    int index;
    
    CCPointArray* menuItemPositions;
    CCMenu* menu;

public:
    CCArray* mi;
    
public:
    SpriteRow(GameSprite*, ScrollArea*, Building*, int);
    ~SpriteRow();
    
    static SpriteRow* create(GameSprite*, ScrollArea*, Building*, int);
    
    bool init();
    
    CCSprite* getVillagerImage();
    
    void construction();
    void recover();
    void doJob();
    void cancelJob();
    void assignHome();
    void resignHome();
    
    void eatFood();
    void farming();
    
    void quitJob();
    void quitHome();

    void rearrange(int);
    void resetPosition(CCNode*, CCPoint);
    
    GameSprite* getSpriteType(SpriteType, SpriteType);
    bool hasVacancy();
    
    void unlinkChildren();
    
    std::string getJobString(SpriteJob);
    std::string getActionString(SpriteAction);
    
    void showSprite();
    
    void refreshAllMenuItems();
    
    void clickSprite();
    
    CCSprite* getMask();
    int getIndex();
};

#endif
