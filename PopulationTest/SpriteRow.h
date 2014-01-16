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
    CCSprite* villagerImage;
    CCLabelTTF* villagerNameLabel;
    //CCLabelTTF* villagerGenderLabel;
    
    //CCLabelTTF* villagerLoyLabel;
    //CCLabelTTF* villagerHapLabel;
    
    //CCLabelTTF* villagerLevelLabel;
    //CCLabelTTF* movementSpeedLabel;
    
    ProgressBar* villagerEnergyBar;
    CCLabelTTF* villagerEnergyLabel;
    
    CCLabelTTF* actionLabel;
    CCLabelTTF* isDongingJobLabel;
    CCLabelTTF* jobLabel;
    
    CCMenuItemImage* villagerSelectButton;
    CCMenuItemImage* villagerCancelButton;
    
    GameSprite* gameSprite;
    ScrollArea* scrollArea;
    Building* building;
    int index;
    
    CCArray* menuItems;
    CCPointArray* menuItemPositions;
    CCMenu* menu;
    
public:
    SpriteRow(GameSprite*, ScrollArea*, Building*, int);
    ~SpriteRow();
    
    static SpriteRow* create(GameSprite*, ScrollArea*, Building*, int);
    
    void init();
    
    CCSprite* getVillagerImage();
    CCLabelTTF* getVillagerNameLabel();
    CCLabelTTF* getVillagerGenderLabel();
    CCLabelTTF* getVillagerLoyLabel();
    CCLabelTTF* getVillagerHapLabel();
    
    CCMenuItemImage* getVillagerSelectButton();
    
    void construction();
    void doJob();
    void cancelJob();
    
    std::string getJobString(SpriteJob);
    std::string getActionString(SpriteAction);
};

#endif
