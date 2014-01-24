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

class SpriteRow : CCObject
{
private:
    int mSpriteRowEnergyCurrent;
    int mSpriteRowEnergyRequired;
    int mSpriteRowHungryCurrent;
    int mSpriteRowHungryRequired;
    SpriteAction mSpriteRowAction;
    bool mSpriteRowIsDoingJob;
    std::string mSpriteRowSpriteName;
    
    CCMenuItemImage* villagerImage;
    CCLabelTTF* villagerNameLabel;
    CCLabelTTF* villagerGenderLabel;
    
    /*
    CCLabelTTF* villagerLoyLabel;
    CCLabelTTF* villagerHapLabel;
    
    CCLabelTTF* villagerLevelLabel;
    CCLabelTTF* movementSpeedLabel;
    */
    
    ProgressBar* villagerEnergyBar;
    CCLabelTTF* villagerEnergyLabel;
    
    ProgressBar* villagerHungryBar;
    CCLabelTTF* villagerHungryLabel;
    
    CCLabelTTF* actionLabel;
    CCLabelTTF* isDongingJobLabel;
    
    /*
    CCLabelTTF* jobLabel;
    */
    
    CCMenuItemImage* villagerSelectButton;
    CCMenuItemImage* villagerCancelButton;
    CCMenuItemImage* villagerResignButton;
    
    GameSprite* gameSprite;
    ScrollArea* scrollArea;
    Building* building;
    int index;
    
    CCArray* menuItems;
    CCPointArray* menuItemPositions;
    CCMenu* menu;
    
    bool isMember;
    
public:
    SpriteRow(GameSprite*, ScrollArea*, Building*, int, bool);
    ~SpriteRow();
    
    static SpriteRow* create(GameSprite*, ScrollArea*, Building*, int, bool);
    
    void init();
    
    CCSprite* getVillagerImage();
    CCLabelTTF* getVillagerNameLabel();
    CCLabelTTF* getVillagerGenderLabel();
    CCLabelTTF* getVillagerLoyLabel();
    CCLabelTTF* getVillagerHapLabel();
    
    CCMenuItemImage* getVillagerSelectButton();
    
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
};

#endif
