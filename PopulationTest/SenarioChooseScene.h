//
//  SenarioChooseScene.h
//  PopulationTest
//
//  Created by SGI on 12/12/13.
//
//

#ifndef PopulationTest_SenarioChooseScene_h
#define PopulationTest_SenarioChooseScene_h

#include "cocos2d.h"

using namespace cocos2d;

class SenarioChooseScene : public cocos2d::CCLayer
{
public:
    static SenarioChooseScene* SP;
    
    CCSprite* backgroundImage;
    
    CCMenuItem* senarioButtonTutorial;
    CCMenuItem* senarioButtonS1;
    CCMenuItem* senarioButtonS2;
    CCMenuItem* senarioButtonS3;
    CCMenuItem* senarioButtonS4;
    CCMenuItem* senarioButtonS5;
    CCMenuItem* senarioButtonS6;
    
    CCLabelTTF* tutorialLabel;
    CCLabelTTF* senarioLabel1;
    CCLabelTTF* senarioLabel2;
    CCLabelTTF* senarioLabel3;
    CCLabelTTF* senarioLabel4;
    CCLabelTTF* senarioLabel5;
    
    CCMenuItem* loadGameButton;
    
    CCMenu* senarioChooseSceneMenu;
    
    bool isGoingToLoadGame;
    
public:
    SenarioChooseScene();
    ~SenarioChooseScene();
    
    static SenarioChooseScene* getThis();
    static SenarioChooseScene* create();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    void chooseTutorial();
    void chooseSenario1();
    void chooseSenario2();
    void chooseSenario3();
    void chooseSenario4();
    void chooseSenario5();
    void chooseSenario6();
    
    void onOrientateChange();
    void onOrientationChangedToPortrait();
    void onOrientationChangedToLandscape();
    
    void loadingTutorial();
    void loadingSenario1();
    void loadingSenario2();
    void loadingSenario3();
    void loadingSenario4();
    void loadingSenario5();
    void loadingSenario6();
    
    void chooseLoadGame();
    void loadingLoadGame();
    
public:
    // save file choose screen
    CCSprite* saveFileChooseBackground;
    CCMenuItem* closeButton;
    CCMenu* saveFileMenu;
    CCSprite* blackScreen;
    
    CCLabelTTF* loadScreenTitle;
    
    bool isSaveFileChooseScreenOpening;
    bool isSaveFileChooseScreenClosing;
    
    CCMenuItem* autoLoadButton;
    CCMenuItem* fixedLoadButton;
    CCMenuItem* customLoadButton;
    
    CCMenuItem* autoLoadDisableButton;
    CCMenuItem* fixedLoadDisableButton;
    CCMenuItem* customLoadDisableButton;
    
    CCMenuItem* autoLoadHandleButton;
    CCMenuItem* fixedLoadHandleButton;
    CCMenuItem* customLoadHandleButton;
    
    CCLabelTTF* autoSaveLabel;
    CCLabelTTF* fixedSaveLabel;
    CCLabelTTF* customSaveLabel;
    
    CCLabelTTF* autoSaveStatusLabel;
    CCLabelTTF* fixedSaveStatusLabel;
    CCLabelTTF* customSaveStatusLabel;
    
    CCSpriteBatchNode* mainMenuSceneNode;
    
public:
    // tutorial choose screen
    CCSprite* tutorialChooseBackground;
    CCMenuItem* TutorialChooseCloseButton;
    CCLabelTTF* TutorialChooseLabel;
    CCMenu* tutorialMenu;
    
    CCMenuItem* tutorialChoosePlayButton;
    CCMenuItem* tutorialChooseSkipButton;
    CCSprite* tutorialChooseBlackScreen;
    
    bool isTutorialChooseScreenOpening;
    bool isTutorialChooseScreenClosing;
    
public:
    void setupTutorialChooseScreen();
    void setupSaveFileChooseScreen();
    
    void scheduleTutorialChooseScreenOpening();
    void scheduleSaveFileChooseScreenOpening();
    
    void scheduleTutorialChooseScreenClosing();
    void scheduleSaveFileChooseScreenClosing();
    
    void tutorialChooseScreenOpen(float dt);
    void saveFileChooseScreenOpen(float dt);
    
    void tutorialChooseScreenClose(float dt);
    void saveFileChooseScreenClose(float dt);
    
    void playTutorial();
    void playScenario1();
    void chooseLoadScenario1();
    
    void loadAutoSaveGame();
    void loadFixedSaveGame();
    void loadCustomSaveGame();
    
    void loadDisableWarning();
    void checkLoadStatus();
    
    void clear();
    
    bool isActive();
    
    void loadTextures();
    void releaseTextures();
};


#endif
