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
    int mGameSpriteEduLvl;
    int mGameSpriteInt;
    int mGameSpriteLoy;
    int mGameSpriteSoc;
    int mGameSpriteHap;
    int mGameSpriteExp;
    int mGameSpriteExpMax;
    
    // Graphical items
    GameSprite* gameSprite;
    
    CCSprite* spriteBackground;
    CCSprite* spriteBackgroundInner;
    CCSprite* gameSpriteImage;
    
    CCLabelTTF* labelExp;
    CCLabelTTF* labelCash;
    
    CCSprite* spCash;
    CCSprite* spEduLvl;
    
    CCSprite* spLoy;
    CCSprite* spHap;
    
    
    CCLabelTTF* textName;
    CCLabelTTF* textCash;
    CCLabelTTF* textEduLvl;
    
    CCLabelTTF* textExp;
    CCLabelTTF* textInt;
    CCLabelTTF* textLoy;
    CCLabelTTF* textSoc;
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
    
    ProgressBar* barExp;
    ProgressBar* barEnergy;
    
    CCMenuItemImage* buttonClose;
    CCMenuItemImage* buttonWorkPlace;
    CCMenuItemImage* buttonHome;
    
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
