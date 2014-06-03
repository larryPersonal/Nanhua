//
//  ScoreMenu.h
//  PopulationTest
//
//  Created by admin on 28/5/14.
//
//

#ifndef PopulationTest_ScoreMenu_h
#define PopulationTest_ScoreMenu_h

#include "cocos2d.h"

using namespace cocos2d;

class ScoreMenu : public CCObject
{
    
public:
    static ScoreMenu* SP;
    
    CCSprite* blackScreen;
    CCSprite* background;
    
    CCLabelTTF* scenarioTitle;
    
    CCSprite* renToken;
    CCLabelTTF* renTokenLabel;
    
    CCSprite* money;
    CCLabelTTF* moneyLabel;
    CCSprite* food;
    CCLabelTTF* foodLabel;
    CCSprite* population;
    CCLabelTTF* populationLabel;
    
    CCLabelTTF* congratsLabel;
    
    CCMenuItemImage* scoreMenuButton;
    CCMenu* menu;
    
    bool show;
    bool hide;
    
    float scaleLimit;
    
    CCLabelTTF* noticeLabel;;
    
public:
    static ScoreMenu* create(CCLayer*);
    static ScoreMenu* getThis();
    
    ScoreMenu(CCLayer*);
    ~ScoreMenu();
    
    bool init(CCLayer*);
    
    
    void scheduleShowScoreMenu();
    void scheduleHideScoreMenu();
    void showScoreMenu(float);
    void hideScoreMenu(float);
    void removeScoreMenu();
    
    void releaseAll();
    
    void clickScoreMenuButton();
};

#endif
