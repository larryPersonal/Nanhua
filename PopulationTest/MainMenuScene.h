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
#include "CCEditBox.h"
#include "ScrollArea.h"

using namespace cocos2d;
using namespace cocos2d::extension;

enum Mode {
    login, signup, chooseCharacter, mainPage, teacher
};

class MainMenuScene : public cocos2d::CCLayer
{
    static MainMenuScene* SP;
    
    CCSprite* backgroundImage;
    CCMenuItemImage* buttonStart;
    CCMenuItemImage* buttonOptions;
    CCMenuItemImage* buttonExit;
    
    CCLabelTTF* usernameLabel;
    
    CCSprite* blackScreen;
    
    CCSprite* highScoreScreen;
    CCLabelTTF* tutorialScoreLabel;
    CCLabelTTF* level1ScoreLabel;
    CCLabelTTF* level2ScoreLabel;
    CCLabelTTF* level3ScoreLabel;
    CCLabelTTF* level4ScoreLabel;
    CCLabelTTF* level5ScoreLabel;
    CCLabelTTF* level6ScoreLabel;
    CCLabelTTF* totalScoreLabel;
    CCMenuItem* cancelButton;
    
    CCMenu* mainMenuSceneMenu;
    CCMenu* cancelButtonMenu;
    CCMenu* loginCancelButtonMenu;
    
    CCMenu* loginMenu;
    CCMenu* signupMenu;
    CCMenu* warningCancelMenu;
    
    bool isScoreScreenJumpIn;
    bool isScoreScreenJumpOut;
    
    bool isWarningScreenJumpIn;
    bool isWarningScreenJumpOut;
    
    /* login module */
    bool isClose;
    bool startGame;
    
    CCLabelTTF* loginModuleTitle;
    
    CCEditBox* usernameBox;
    CCEditBox* passwordBox;
    
    CCLabelTTF* usernameBoxLabel;
    CCLabelTTF* passwordBoxLabel;
    
    CCMenuItem* loginButton;
    CCMenuItem* createButton;
    
    /* warning window */
    CCSprite* warningBackground;
    CCLabelTTF* warningHead;
    CCLabelTTF* warningDescription;
    CCMenuItem* warningCancelButton;
    CCMenuItem* warningConfirmButton;
    
    CCSprite* loginBackground;
    CCMenuItem* loginCancelButton;
    
    CCMenuItem* playAnolymouslyButton;
    CCMenuItem* createAccountButton;
    
    CCMenuItem* backButton;
    
    CCLabelTTF* loginScreenTitleLabel;
    CCLabelTTF* loginScreenInstructionLabel;
    CCLabelTTF* loginScreenWarningLabel;
    
    Mode mode;
    
    /* boy, girl animation */
    
    CCSprite* boyHead;
    CCSprite* girlHead;
    
    CCTexture2D* boyTexture;
    CCTexture2D* girlTexture;
    
    CCRect boyRect;
    CCRect girlRect;
    
    int frameWidth;
    int frameHeight;
    
    int boyFrameNo;
    int girlFrameNo;
    
    int boyMaxFrameNo;
    int girlMaxFrameNo;
    
    int boyXOffset;
    int boyYOffset;
    int girlXOffset;
    int girlYOffset;
    
    float boy_delay_animFrame;
    float boy_delay_current;
    float girl_delay_animFrame;
    float girl_delay_current;
    
    bool isInBoyAnimation;
    bool isInGirlAnimation;
    
    float boyCumulativeTime;
    float girlCumulativeTime;
    
    float boyCheckTime;
    float girlCheckTime;
    
    int baseTime;
    int rangeTime;
    int animationRatio;
    
public:
    
    CCSize sz; 
    
    MainMenuScene();
    ~MainMenuScene();
    static MainMenuScene* getThis();
    static MainMenuScene* create();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    void onButtonStartPressed(CCObject* pSender);
    void onButtonOptionsPressed(CCObject* pSender);
    void onButtonExitPressed(CCObject* pSender);
    
    void onNewGame(CCObject* pSender);
    
    void loadSenarioChooseScene();
    
    void closeScoreScreen();
    
    void closeWarningScreen();
    void openWarningScreen();
    
    void onOrientationChanged();
    
    void scheduleScoreScreenJumpIn();
    void scheduleScoreScreenJumpOut();
    
    void scoreScreenJumpIn(float deltaTime);
    void scoreScreenJumpOut(float deltaTime);
    
    void scheduleWarningScreenJumpIn();
    void scheduleWarningScreenJumpOut();
    
    void warningScreenJumpIn(float deltaTime);
    void warningScreenJumpOut(float deltaTime);
    
    void scheduleLoginScreenJumpIn();
    void scheduleLoginScreenJumpOut();
    
    void loginScreenJumpIn(float deltaTime);
    void loginScreenJumpOut(float deltaTime);
    
    void closeLoginScreen();
    
    void clickLoginButton();
    
    void clickCreateAccountButton();
    
    void clickBackButton();
    
    void clickCreateButton();
    
    void clickPlayAnolymouslyButton();
    
    void configMode();
    
    void update(float deltaTime);
    
    void enableModule();
    void disableModule();
    
    virtual void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
    
    void clearMainMenuScene();
    
public:
    CCSprite* teacherManagementBackground;
    
    CCMenuItem* teacherManagementScreenLogoutButton;
    
    CCMenu* teacherManagementScreenMenu;
    
    CCLabelTTF* teacherManagementScreenTitile;
    
    ScrollArea* scrollArea;
    
    CCArray* accountRowArray;
    
    bool teacherManagementIsOpen;
    bool teacherManagementIsClose;
    
    CCSpriteBatchNode* teacherManagementNode;
    CCSpriteBatchNode* mainMenuSceneNode;
    
public:
    bool checkIsTeacherLogin(std::string username, std::string password);
    void showTeacherScreen();
    void setupTeacherManagementScreen();
    void removeTeacherManagementScreen();
    
    void scheduleTeacherManagementScreenOpen();
    void scheduleTeacherManagementScreenClose();
    
    void teacherManagementScreenOpen(float dt);
    void teacherManagementScreenClose(float dt);
    
    void clearTeacherManagementScreen();
    
    void deleteAccountRow(int index);
    
    void openResetPasswordConfirmScreen(std::string username);
    void closeResetPasswordConfirmScreen();
    
    bool isActive();
    
    void loadTeacherManagementTextures();
    void releaseTeacherManagementTextures();
    
    void loadMainMenuTextures();
    void releaseMainMenuTextures();
};

#endif /* defined(__PopulationTest__MainMenuScene__) */
