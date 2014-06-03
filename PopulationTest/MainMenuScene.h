//
//  MainMenuScene.h
//  PopulationTest
//
//  Created by Serious Games on 13/3/13.
//
//

#ifndef __PopulationTest__MainMenuScene__
#define __PopulationTest__MainMenuScene__

#include "cocos2d.h"

using namespace cocos2d;


class MainMenuScene : public cocos2d::CCLayer
{
    static MainMenuScene* SP;
    
    CCSprite* backgroundImage;
    CCMenuItemImage* buttonStart;
    CCMenuItemImage* buttonOptions;
    CCMenuItemImage* buttonCredits;
    
    // loading screen module
    CCSprite* loadingScreen;
    CCSprite* loadingLabel;
    
    CCSprite* highScoreScreen;
    CCLabelTTF* level1ScoreLabel;
    CCLabelTTF* level2ScoreLabel;
    CCMenuItemImage* cancelButton;
    
    
public:
    
    CCSize sz; 
    
    MainMenuScene();
    ~MainMenuScene();
    static MainMenuScene* getThis();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    void onButtonStartPressed(CCObject* pSender);
    void onButtonOptionsPressed(CCObject* pSender);
    void onButtonCreditsPressed(CCObject* pSender);
    void onOrientationChanged();
    void onOrientationChangedToPortrait();
    void onOrientationChangedToLandscape();
    
    void onLoadGame(CCObject* pSender);
    void onNewGame(CCObject* pSender);
    
    void onAcceptTutorial(CCObject* pSender);
    void onRejectTutorial(CCObject* pSender);
    
    void enableLoadingScreen();
    void loadSenarioChooseScene();
    
    void closeScoreScreen();
    void openScoreScreen();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainMenuScene);
};

#endif /* defined(__PopulationTest__MainMenuScene__) */
