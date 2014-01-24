//
//  SpriteInfoMenu.h
//  PopulationTest
//
//  Created by Ong Heng Le on 6/5/13.
//
//

#ifndef PopulationTest_SpriteInfoMenu_h
#define PopulationTest_SpriteInfoMenu_h

#include "CCScrollView.h"
using namespace cocos2d::extension;

#include "PopupMenu.h"
#include "ProgressBar.h"
#include "Sprite.h"

using namespace std;

class SpriteInfoMenu:public PopupMenu
{
    
private:
    
    // GameSprite variables
    int mGameSpriteEnergy;
    int mGameSpriteEnergyMax;
    int mGameSpriteCash;
    int mGameSpriteLoy;
    int mGameSpriteHap;
    SpriteAction mGameSpriteCurrAction;
    int mGameSpriteFoodCarriage;
    int mGameSpriteFoodCarriageLimit;
    int mGameSpriteHungry;
    int mGameSpriteHungryMax;
    int mGameSpriteWorkRate;
    float mGameSpriteWorkUnitPerDay;
    
    
    
    // Graphical items
    GameSprite* gameSprite;
    
    CCSprite* spriteBackground;
    CCSprite* spriteBackgroundInner;
    CCSprite* gameSpriteImage;
    
    CCLabelTTF* labelCash;
    
    CCSprite* spCash;
    
    CCSprite* spLoy;
    CCSprite* spHap;
    
    
    CCLabelTTF* textName;
    CCLabelTTF* textCash;
    
    CCLabelTTF* textLoy;
    CCLabelTTF* textHap;
    
    CCLabelTTF* energyLabel;
    CCLabelTTF* energyTitleLabel;
    CCLabelTTF* classLabel;
    CCLabelTTF* classTitleLabel;
    
    CCLabelTTF* genderLabel;
    CCLabelTTF* genderTitleLabel;
    CCLabelTTF* raceLabel;
    CCLabelTTF* raceTitleLabel;
    
    CCLabelTTF* movementSpeedLabel;
    CCLabelTTF* movementSpeedTitleLabel;
    CCLabelTTF* loyaltyLabel;
    CCLabelTTF* happinessLabel;
    
    ProgressBar* barEnergy;
    
    CCMenuItemImage* buttonClose;
    CCMenuItemImage* buttonWorkPlace;
    CCMenuItemImage* buttonHome;
    
    ProgressBar* barHungry;
    CCLabelTTF* hungryTitleLabel;
    CCLabelTTF* hungryLabel;
    
    ProgressBar* barFoodCarriage;
    CCLabelTTF* foodCarriageTitleLabel;
    CCLabelTTF* foodCarriageLabel;
    
    CCLabelTTF* workRateTitleLabel;
    CCLabelTTF* workRateLabel;
    CCLabelTTF* workUnitPerDayTitleLabel;
    CCLabelTTF* workUnitPerDayLabel;
    
    CCLabelTTF* movementRangeTitleLabel;
    CCLabelTTF* movementRangeLabel;
    
    CCLabelTTF* animateSpeedTitleLabel;
    CCLabelTTF* animateSpeedLabel;
    
    CCLabelTTF* currentActionTitleLabel;
    CCLabelTTF* currentActionLabel;
    
    CCLabelTTF* targetLocationTitleLabel;
    CCMenuItemImage* targetLocationImage;
    
protected:
    
    //General create menu
    virtual void createMenuItems();
    
public:
    virtual ~SpriteInfoMenu();
    
    // Constructor with Building object
    SpriteInfoMenu(GameSprite* gameSprite);
    
    virtual void willChangeOrientation();
    virtual void onOrientationChanged();
    
    //General menu callback
    virtual void onMenuItemSelected(CCObject* pSender);
    
    //Public function
    void reposition();
    void refreshAllMenuItemValues();
    
    //Update
    virtual void update(float deltaTime);
};

#endif
