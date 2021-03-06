//
//  MainMenuScene.cpp
//  PopulationTest
//
//  Created by Serious Games on 13/3/13.
//
//

#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "SenarioChooseScene.h"
#include "GameScene.h"
#include "GameManager.h"
#include "SoundtrackManager.h"
#include "GlobalHelper.h"
#include "AccountRow.h"
#include "UserProfile.h"
#include "UIButtonControl.h"
#include "TutorialManager.h"

using namespace CocosDenshion;

MainMenuScene* MainMenuScene::SP;

MainMenuScene::MainMenuScene()
{
    MainMenuScene::SP = this;
    
    isScoreScreenJumpIn = false;
    isScoreScreenJumpOut = false;
    
    isWarningScreenJumpIn = false;
    isWarningScreenJumpOut = false;
    
    isClose = false;
    
    boyCumulativeTime = 0;
    boyCheckTime = 0;
    
    girlCumulativeTime = 0;
    girlCheckTime = 0;
    
    boyXOffset = 0;
    boyYOffset = 0;
    
    girlXOffset = 0;
    girlYOffset = 0;
    
    isInBoyAnimation = false;
    isInGirlAnimation = false;
    
    baseTime = 3;
    rangeTime = 6;
    animationRatio = 40;
    
    startGame = false;
    
    teacherManagementIsOpen = false;
    teacherManagementIsClose = false;
    
    accountRowArray = CCArray::create();
    accountRowArray->retain();
    
    mode = login;
    
    teacherManagementNode = NULL;
    
    UIButtonControl::pauseGame();
    TutorialManager::getThis()->lockModule = true;
}

MainMenuScene::~MainMenuScene()
{
    MainMenuScene::SP = NULL;
    
    accountRowArray->removeAllObjects();
    CC_SAFE_RELEASE(accountRowArray);
}

MainMenuScene* MainMenuScene::create()
{
    MainMenuScene* pRet = new MainMenuScene();
    if(pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

MainMenuScene* MainMenuScene::getThis()
{
    return SP;
}

bool MainMenuScene::init()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    CCRect boxrect = CCRectMake(0,0, screenSize.width * 0.8f,  screenSize.width * 0.1f);
    
    if (UserProfile::getThis()->isLoggedIn)
    {
        backgroundImage = CCSprite::createWithSpriteFrameName("newsplashpage1.jpg");
    }
    else
    {
        backgroundImage = CCSprite::createWithSpriteFrameName("loginPage.jpg");
    }
    
    backgroundImage->setScale(screenSize.width/backgroundImage->boundingBox().size.width);
    backgroundImage->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    buttonStart = CCMenuItemImage::create("", "", this, menu_selector(MainMenuScene::onButtonStartPressed));
    buttonOptions = CCMenuItemImage::create("", "", this, menu_selector(MainMenuScene::onButtonOptionsPressed));
    buttonExit = CCMenuItemImage::create("", "", this, menu_selector(MainMenuScene::onButtonExitPressed));
    
    buttonStart->setContentSize(boxrect.size);
    buttonOptions->setContentSize(boxrect.size);
    buttonExit->setContentSize(boxrect.size);
    
    buttonStart->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonStart->setPosition(ccp(0, -120));
    
    buttonOptions->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonOptions->setPosition(0, -220);
    
    buttonExit->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonExit->setPosition(ccp(0, -320));
    
    // buttonCredits = CCMenuItemImage::create("quit.png", "press_quit.png", this, menu_selector(MainMenuScene::onButtonCreditsPressed));
    
    highScoreScreen = CCSprite::createWithSpriteFrameName("trophyscreen.png");
    highScoreScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    highScoreScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    //highScoreScreen->setScaleX(0.9f);
    //highScoreScreen->setScaleY(0.8f);
    highScoreScreen->setScale(0.9f);
    this->addChild(highScoreScreen, 9);
    
    blackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    blackScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    this->addChild(blackScreen, 8);
    
    blackScreen->setOpacity((GLubyte) 0);
    
    CCSprite* cancelSprite = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    cancelButton = CCMenuItemSprite::create(cancelSprite, cancelSprite, this, menu_selector(MainMenuScene::closeScoreScreen));
    cancelButton->setAnchorPoint(ccp(0, 1));
    cancelButton->setPosition(ccp(screenSize.width / 2.0f - 180.0f, screenSize.height / 2.0f + 8.0f));
    cancelButton->setScale(1.2f);
    
    cancelButtonMenu = CCMenu::create(cancelButton, NULL);
    highScoreScreen->addChild(cancelButtonMenu);
    
    usernameLabel = CCLabelTTF::create("user", "GillSansMT", 48);
    usernameLabel->setAnchorPoint(ccp(0.5, 0.5));
    usernameLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 376.0f));
    highScoreScreen->addChild(usernameLabel);
    
    tutorialScoreLabel = CCLabelTTF::create("Tutorial high score is: 0", "GillSansMT", 32);
    tutorialScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    tutorialScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 200.0f));
    highScoreScreen->addChild(tutorialScoreLabel);
    
    level1ScoreLabel = CCLabelTTF::create("Level 1 high score is: 0", "GillSansMT", 32);
    level1ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    level1ScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 150.0f));
    highScoreScreen->addChild(level1ScoreLabel);
    
    level2ScoreLabel = CCLabelTTF::create("Level 2 high score is: 0", "GillSansMT", 32);
    level2ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    level2ScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 100.0f));
    highScoreScreen->addChild(level2ScoreLabel);
    
    level3ScoreLabel = CCLabelTTF::create("Level 3 high score is: 0", "GillSansMT", 32);
    level3ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    level3ScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 50.0f));
    highScoreScreen->addChild(level3ScoreLabel);
    
    level4ScoreLabel = CCLabelTTF::create("Level 4 high score is: 0", "GillSansMT", 32);
    level4ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    level4ScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 0.0f));
    highScoreScreen->addChild(level4ScoreLabel);
    
    level5ScoreLabel = CCLabelTTF::create("Level 5 high score is: 0", "GillSansMT", 32);
    level5ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    level5ScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 50.0f));
    highScoreScreen->addChild(level5ScoreLabel);
    
    level6ScoreLabel = CCLabelTTF::create("Level 6 high score is: 0", "GillSansMT", 32);
    level6ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    level6ScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 100.0f));
    highScoreScreen->addChild(level6ScoreLabel);
    
    totalScoreLabel = CCLabelTTF::create("Total score is: 0", "GillSansMT", 32);
    totalScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    totalScoreLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 150.0f));
    highScoreScreen->addChild(totalScoreLabel);
    
    highScoreScreen->setVisible(false);
    
    /* login module */
    
    loginModuleTitle = CCLabelTTF::create("LOGIN:", "GillSansMT", 38);
    loginModuleTitle->setAnchorPoint(ccp(0, 1));
    loginModuleTitle->setPosition(ccp(screenSize.width / 2.0f + 100.0f, screenSize.height * 3.0f / 4.0f - 110.0f));
    this->addChild(loginModuleTitle, 9);
    
    usernameBox = CCEditBox::create(ccp(300, 40), CCScale9Sprite::createWithSpriteFrameName("inputBar.png"));
    usernameBox->setAnchorPoint(ccp(0, 1));
    usernameBox->setPosition(ccp(screenSize.width / 2.0f + 100.0f, screenSize.height * 3.0f / 4.0f - 200.0f));
    usernameBox->setFontColor(ccBLACK);
    usernameBox->setPlaceHolder(" Name:");
    usernameBox->setMaxLength(20);
    usernameBox->setReturnType(kKeyboardReturnTypeDone);
    this->addChild(usernameBox, 9);
    
    passwordBox = CCEditBox::create(ccp(300, 40), CCScale9Sprite::createWithSpriteFrameName("inputBar1.png"));
    passwordBox->setAnchorPoint(ccp(0, 1));
    passwordBox->setPosition(ccp(screenSize.width / 2.0f + 100.0f, screenSize.height * 3.0f / 4.0f - 280.0f));
    passwordBox->setFontColor(ccBLACK);
    passwordBox->setPlaceHolder(" Password:");
    passwordBox->setMaxLength(30);
    passwordBox->setReturnType(kKeyboardReturnTypeDone);
    passwordBox->setInputFlag(kEditBoxInputFlagPassword);
    this->addChild(passwordBox, 10);
    
    usernameBoxLabel = CCLabelTTF::create("Username:", "GillSansMT", 24);
    usernameBoxLabel->setAnchorPoint(ccp(0, 1));
    usernameBoxLabel->setPosition(ccp(screenSize.width / 2.0f + 100.0f, screenSize.height * 3.0f / 4.0f - 170.0f));
    this->addChild(usernameBoxLabel, 9);
    
    passwordBoxLabel = CCLabelTTF::create("Password:", "GillSansMT", 24);
    passwordBoxLabel->setAnchorPoint(ccp(0, 1));
    passwordBoxLabel->setPosition(ccp(screenSize.width / 2.0f + 100.0f, screenSize.height * 3.0f / 4.0f - 250.0f));
    this->addChild(passwordBoxLabel, 9);
    
    // the login button for the login window
    loginButton = CCMenuItemImage::create("login.png", "login_press.png", this, menu_selector(MainMenuScene::clickLoginButton));
    loginButton->setScale(0.3f);
    loginButton->setAnchorPoint(ccp(0, 1));
    loginButton->setPosition(ccp(100.0f, screenSize.height * 1.0f / 4.0f - 340.0f));
    
    // create button
    createButton = CCMenuItemImage::create("create.png", "create_press.png", this, menu_selector(MainMenuScene::clickCreateButton));
    createButton->setScale(0.3f);
    createButton->setAnchorPoint(ccp(0, 1));
    createButton->setPosition(ccp(250.0f, screenSize.height * 1.0f / 4.0f - 340.0f));
    
    loginMenu = CCMenu::create(loginButton, createButton, NULL);
    this->addChild(loginMenu, 9);
    
    // warning background
    warningBackground = CCSprite::createWithSpriteFrameName("popup_dialogbox.png");
    warningBackground->setAnchorPoint(ccp(0.5f, 0.5f));
    warningBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    warningBackground->setScale(1.0f);
    this->addChild(warningBackground, 15);
    
    // warning cancelButton
    warningCancelButton = CCMenuItemImage::create("Closebtn_Sq.png", "Closebtn_Sq.png", this, menu_selector(MainMenuScene::closeWarningScreen));
    warningCancelButton->setAnchorPoint(ccp(0, 1));
    warningCancelButton->setPosition(ccp(screenSize.width / 2.0f - 180.0f, 0 + 18.0f));
    warningCancelButton->setScale(1.2f);
    
    // warning confirmButton
    warningConfirmButton = CCMenuItemImage::create("confirm.png", "confirm_press.png", this, menu_selector(MainMenuScene::closeResetPasswordConfirmScreen));
    warningConfirmButton->setAnchorPoint(ccp(0, 1));
    warningConfirmButton->setPosition(ccp(screenSize.width / 2.0f - 320.0f, -240.0f));
    warningConfirmButton->setScale(0.5f);
    
    warningConfirmButton->setVisible(false);
    
    warningCancelMenu = CCMenu::create(warningCancelButton, warningConfirmButton, NULL);
    warningBackground->addChild(warningCancelMenu);
    
    // warning head
    warningHead = CCLabelTTF::create("WARNING", "GillSansMT", 32);
    warningHead->setAnchorPoint(ccp(0.5f, 0.5f));
    warningHead->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    warningBackground->addChild(warningHead);
    
    // warning description
    warningDescription = CCLabelTTF::create("WARNING DESCRIPTION", "GillSansMT", 24);
    warningDescription->setAnchorPoint(ccp(0.5f, 0.5f));
    warningDescription->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 180.0f));
    warningBackground->addChild(warningDescription);
    
    // create account button
    createAccountButton = CCMenuItemImage::create("create.png", "create_press.png", this, menu_selector(MainMenuScene::clickCreateAccountButton));
    createAccountButton->setScale(0.3f);
    createAccountButton->setAnchorPoint(ccp(0, 1));
    createAccountButton->setPosition(ccp(100.0f, screenSize.height * 1.0f / 4.0f - 340.0f));
    
    // back button
    backButton = CCMenuItemImage::create("back.png", "back_press.png", this, menu_selector(MainMenuScene::clickBackButton));
    backButton->setScale(0.3f);
    backButton->setAnchorPoint(ccp(0, 1));
    backButton->setPosition(ccp(250.0f, screenSize.height * 1.0f / 4.0f - 340.0f));
    
    signupMenu = CCMenu::create(createAccountButton, backButton, NULL);
    this->addChild(signupMenu, 9);
    
    // login background -> the window
    loginBackground = CCSprite::createWithSpriteFrameName("trophyscreen.png");
    loginBackground->setAnchorPoint(ccp(0.5f, 0.5f));
    loginBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    loginBackground->setScale(0.9f);
    this->addChild(loginBackground, 10);
    
    // the cancel button for the login window
    CCSprite* loginCancelButtonSprite = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    loginCancelButton = CCMenuItemSprite::create(loginCancelButtonSprite, loginCancelButtonSprite, this, menu_selector(MainMenuScene::closeLoginScreen));
    loginCancelButton->setAnchorPoint(ccp(0, 1));
    loginCancelButton->setPosition(ccp(screenSize.width / 2.0f - 180.0f, screenSize.height /2.0f + 8.0f));
    loginCancelButton->setScale(1.2f);
    
    /*
    buttonSprite = CCSprite::createWithSpriteFrameName("button.png");
    buttonPressedSprite = CCSprite::createWithSpriteFrameName("button.png");
    // the play anolymous button for the login window
    playAnolymouslyButton = cocos2d::CCMenuItemSprite::create(buttonSprite, buttonPressedSprite, this, menu_selector(MainMenuScene::clickPlayAnolymouslyButton));
    playAnolymouslyButton->setAnchorPoint(ccp(0.5, 0.5));
    playAnolymouslyButton->setPosition(ccp(0, 0 - 160.0f));
    
    CCLabelTTF* playAnolymouslyButtonLabel = CCLabelTTF::create("Guest Account", "GillSansMT", 32);
    playAnolymouslyButtonLabel->setAnchorPoint(ccp(0.5, 0.5));
    playAnolymouslyButtonLabel->setPosition(ccp(playAnolymouslyButton->boundingBox().size.width / 2.0f, playAnolymouslyButton->boundingBox().size.height / 2.0f + 3.0f));
    playAnolymouslyButton->addChild(playAnolymouslyButtonLabel);
    */
    
    // add all buttons into the button menu
    // loginCancelButtonMenu = CCMenu::create(loginCancelButton, playAnolymouslyButton, NULL);
    loginCancelButtonMenu = CCMenu::create(loginCancelButton, NULL);
    loginBackground->addChild(loginCancelButtonMenu);
    
    loginBackground->setVisible(false);
    
    loginScreenTitleLabel = CCLabelTTF::create("Login", "GillSansMT", 48);
    loginScreenTitleLabel->setAnchorPoint(ccp(0.5, 0.5));
    loginScreenTitleLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 376.0f));
    loginBackground->addChild(loginScreenTitleLabel);
    
    loginScreenInstructionLabel = CCLabelTTF::create("You haven't login yet! Please login or play anonymously!", "GillSansMT", 32);
    loginScreenInstructionLabel->setAnchorPoint(ccp(0.5, 0.5));
    loginScreenInstructionLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 240.0f));
    loginBackground->addChild(loginScreenInstructionLabel);
    
    loginScreenWarningLabel = CCLabelTTF::create("", "GillSansMT", 32);
    loginScreenWarningLabel->setAnchorPoint(ccp(0.5, 0.5));
    loginScreenWarningLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 200.0f));
    loginBackground->addChild(loginScreenWarningLabel);
    
    /* boy and girl animation */
    boyTexture = CCTextureCache::sharedTextureCache()->addImage("igamechar_boy.png");
    girlTexture = CCTextureCache::sharedTextureCache()->addImage("ingame_chargirl.png");
    
    boyFrameNo = 0;
    girlFrameNo = 0;
    
    boyMaxFrameNo = 14;
    girlMaxFrameNo = 16;
    
    boy_delay_animFrame = 0.1f;
    boy_delay_current = 0.1f;
    
    girl_delay_animFrame = 0.1f;
    girl_delay_current = 0.1f;
    
    frameWidth = 245;
    frameHeight = 225;
    
    boyRect = CCRectMake(0, 0, frameWidth, frameHeight);
    girlRect = CCRectMake(0, 0, frameWidth, frameHeight);
    
    boyHead = CCSprite::createWithTexture(boyTexture, boyRect);
    boyHead->setAnchorPoint(ccp(0.5, 0.5));
    boyHead->setPosition(ccp(screenSize.width / 2.0f - 170.0f, screenSize.height / 2.0f + 50.0f));
    boyHead->setScale(0.8f);
    loginBackground->addChild(boyHead);
    
    girlHead = CCSprite::createWithTexture(girlTexture, girlRect);
    girlHead->setAnchorPoint(ccp(0.5, 0.5));
    girlHead->setPosition(ccp(screenSize.width / 2.0f + 200.0f, screenSize.height / 2.0f + 50.0f));
    girlHead->setScale(0.8f);
    loginBackground->addChild(girlHead);
    
    
    // 2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        // backgroundImage->setScale(0.5);
        buttonStart->setScale(0.5);
        buttonOptions->setScale(0.5);
        buttonExit->setScale(0.5);
        // buttonCredits->setScale(0.5);
    }
    
    //Create menu, it's an autorelease object
    mainMenuSceneMenu = CCMenu::create(buttonStart, buttonOptions, buttonExit, NULL);
    this->addChild(mainMenuSceneMenu, 1);
    this->addChild(backgroundImage, 0);
    
    if(UserProfile::getThis()->isLoggedIn)
    {
        mode = mainPage;
    }
    else
    {
        mode = login;
        // mode = mainPage;
    }
    
    configMode();
    
    this->setTouchEnabled(true);
    
    GlobalHelper::clearCache();
    
    return true;
}

void MainMenuScene::clearMainMenuScene()
{
    buttonStart->removeFromParentAndCleanup(true);
    buttonOptions->removeFromParentAndCleanup(true);
    buttonExit->removeFromParentAndCleanup(true);
    mainMenuSceneMenu->removeAllChildren();
    this->removeChild(mainMenuSceneMenu, true);
    
    this->removeChild(backgroundImage, true);
    this->removeChild(blackScreen, true);
    
    cancelButtonMenu->removeChild(cancelButton, true);
    cancelButtonMenu->removeAllChildren();
    highScoreScreen->removeChild(cancelButtonMenu, true);
    
    highScoreScreen->removeChild(usernameLabel, true);
    highScoreScreen->removeChild(tutorialScoreLabel, true);
    highScoreScreen->removeChild(level1ScoreLabel, true);
    highScoreScreen->removeChild(level2ScoreLabel, true);
    highScoreScreen->removeChild(level3ScoreLabel, true);
    highScoreScreen->removeChild(level4ScoreLabel, true);
    highScoreScreen->removeChild(level5ScoreLabel, true);
    highScoreScreen->removeChild(level6ScoreLabel, true);
    highScoreScreen->removeChild(totalScoreLabel, true);
    
    this->removeChild(highScoreScreen, true);
    this->removeChild(loginModuleTitle, true);
    this->removeChild(usernameBox, true);
    this->removeChild(passwordBox, true);
    this->removeChild(usernameBoxLabel, true);
    this->removeChild(passwordBoxLabel, true);
    
    loginMenu->removeChild(loginButton, true);
    loginMenu->removeChild(createButton, true);
    this->removeChild(loginMenu, true);
    
    warningCancelMenu->removeChild(warningCancelButton, true);
    warningCancelMenu->removeChild(warningConfirmButton, true);
    warningBackground->removeChild(warningCancelMenu, true);
    warningBackground->removeChild(warningHead, true);
    warningBackground->removeChild(warningDescription, true);
    this->removeChild(warningBackground, true);
    
    signupMenu->removeChild(createAccountButton, true);
    signupMenu->removeChild(backButton, true);
    this->removeChild(signupMenu, true);
    
    loginCancelButtonMenu->removeChild(loginCancelButton, true);
    loginBackground->removeChild(loginCancelButtonMenu, true);
    loginBackground->removeChild(loginScreenTitleLabel, true);
    loginBackground->removeChild(loginScreenInstructionLabel, true);
    loginBackground->removeChild(loginScreenWarningLabel, true);
    this->removeChild(loginBackground, true);
    
    this->removeChild(boyHead, true);
    this->removeChild(girlHead, true);
    
    this->removeAllChildrenWithCleanup(true);
}

void MainMenuScene::onButtonStartPressed(CCObject* pSender){
    if(!isActive())
    {
        return;
    }
    
    if (loginBackground->isVisible() || highScoreScreen->isVisible())
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    onNewGame(NULL);
}

void MainMenuScene::onNewGame(cocos2d::CCObject *pSender)
{
    this->scheduleOnce(schedule_selector(MainMenuScene::loadSenarioChooseScene), 0.1f);
}

void MainMenuScene::configMode()
{
    GlobalHelper::clearCache();
    if(mode == login)
    {
        loginModuleTitle->setVisible(true);
        
        loginMenu->setVisible(true);
        signupMenu->setVisible(false);
        
        usernameBox->setVisible(true);
        passwordBox->setVisible(true);
        
        usernameBoxLabel->setVisible(true);
        passwordBoxLabel->setVisible(true);
        
        buttonStart->setVisible(false);
        buttonOptions->setVisible(false);
        
        boyHead->setVisible(false);
        girlHead->setVisible(false);
        
        warningBackground->setVisible(false);
        
        loginModuleTitle->setString("LOGIN:");
        
        blackScreen->setVisible(false);
    }
    else if(mode == signup)
    {
        loginModuleTitle->setVisible(true);
        
        loginMenu->setVisible(false);
        signupMenu->setVisible(true);
        
        usernameBox->setVisible(true);
        passwordBox->setVisible(true);
        
        usernameBoxLabel->setVisible(true);
        passwordBoxLabel->setVisible(true);
        
        buttonStart->setVisible(false);
        buttonOptions->setVisible(false);
        
        boyHead->setVisible(false);
        girlHead->setVisible(false);
        
        warningBackground->setVisible(false);
        
        loginModuleTitle->setString("SIGNUP:");
        
        blackScreen->setVisible(false);
    }
    else if(mode == chooseCharacter)
    {
        loginModuleTitle->setVisible(true);
        
        // playAnolymouslyButton->setVisible(false);
        
        loginMenu->setVisible(false);
        signupMenu->setVisible(true);
        
        usernameBox->setVisible(true);
        passwordBox->setVisible(true);
        
        usernameBoxLabel->setVisible(true);
        passwordBoxLabel->setVisible(true);
        
        boyHead->setVisible(true);
        girlHead->setVisible(true);
        
        warningBackground->setVisible(false);
        
        loginScreenTitleLabel->setString("Character");
        string str = "Welcome, " + CCUserDefault::sharedUserDefault()->getStringForKey("tempUser") + "!";
        loginScreenInstructionLabel->setString(str.c_str());
        
        loginScreenWarningLabel->setString("Choose your character:");
        
        blackScreen->setVisible(true);
    }
    else if(mode == mainPage)
    {
        loginModuleTitle->setVisible(false);
        
        loginMenu->setVisible(false);
        signupMenu->setVisible(false);
        
        usernameBox->setVisible(false);
        passwordBox->setVisible(false);
        
        usernameBoxLabel->setVisible(false);
        passwordBoxLabel->setVisible(false);
        
        buttonStart->setVisible(true);
        buttonOptions->setVisible(true);
        
        boyHead->setVisible(false);
        girlHead->setVisible(false);
        
        warningBackground->setVisible(false);
        
        blackScreen->setVisible(true);
    }
    else if(mode == teacher)
    {
        loginModuleTitle->setVisible(false);
        
        loginMenu->setVisible(false);
        signupMenu->setVisible(false);
        
        usernameBox->setVisible(false);
        passwordBox->setVisible(false);
        
        usernameBoxLabel->setVisible(false);
        passwordBoxLabel->setVisible(false);
        
        buttonStart->setVisible(false);
        buttonOptions->setVisible(false);
        
        boyHead->setVisible(false);
        girlHead->setVisible(false);
        
        warningBackground->setVisible(false);
        
        blackScreen->setVisible(true);
    }
}

void MainMenuScene::clickLoginButton()
{
    if(!isActive())
    {
        return;
    }
    
    if(warningBackground->isVisible())
    {
        return;
    }
    
    // when click the login button will check the username and password
    string username = usernameBox->getText();
    string password = passwordBox->getText();
    
    if(checkIsTeacherLogin(username, password))
    {
        // go to teacher management page
        // loadTeacherManagementTextures();
        setupTeacherManagementScreen();
        mode = teacher;
        configMode();
        showTeacherScreen();
    }
    else
    {
        int userNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("userNumber");
        bool valid = false;
        stringstream ss;
        
        for (int i = 0; i < userNumber; i++)
        {
            ss.str(std::string());
            ss << "user_" << i;
            string tempName = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
            if (username.compare(tempName) == 0)
            {
                valid = true;
                break;
            }
        }
        
        if (!valid)
        {
            warningDescription->setString("Either username or password is invalid!");
            openWarningScreen();
            return;
        }
        
        string key = username + "_password";
        string expectedPassword = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str());
        
        SoundtrackManager::PlaySFX("Button_press.wav");
        
        if(password.length() <= 0 || expectedPassword.length() <= 0)
        {
            warningDescription->setString("Either username or password is invalid!");
            openWarningScreen();
        }
        else
        {
            if(password.compare(expectedPassword) == 0)
            {
                // login successful
                UserProfile::getThis()->username = username;
                UserProfile::getThis()->password = password;
                
                string key = username + "_gender";
                bool isBoy = CCUserDefault::sharedUserDefault()->getBoolForKey(key.c_str());
                UserProfile::getThis()->gender = isBoy;
                UserProfile::getThis()->isLoggedIn = true;
                
                CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
                
                this->removeChild(backgroundImage);
                backgroundImage = CCSprite::createWithSpriteFrameName("newsplashpage1.jpg");
                backgroundImage->setScale(screenSize.width / backgroundImage->boundingBox().size.width);
                backgroundImage->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
                this->addChild(backgroundImage, 0);
                
                mode = mainPage;
                configMode();
            }
            else
            {
                warningDescription->setString("Either username or password is invalid!");
                openWarningScreen();
            }
        }
    }
}

bool MainMenuScene::checkIsTeacherLogin(string username, string password)
{
    stringstream ss;
    ss << "UserManagement_Account";
    std::string stored_username = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
    
    ss.str(std::string());
    ss << "UserManagement_Password";
    std::string stored_password = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
    
    if(username.compare(stored_username) == 0 && password.compare(stored_password) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MainMenuScene::clickPlayAnolymouslyButton()
{
    if(!isActive())
    {
        return;
    }
    
    startGame = true;
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleLoginScreenJumpOut();
}

void MainMenuScene::clickCreateAccountButton()
{
    if(!isActive())
    {
        return;
    }
    
    if (warningBackground->isVisible())
    {
        return;
    }
    
    string username = usernameBox->getText();
    string password = passwordBox->getText();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    if(username.length() < 4)
    {
        warningDescription->setString("Username should be at least 4 characters!");
        openWarningScreen();
    }
    else if(password.length() < 4)
    {
        warningDescription->setString("Password should be at least 4 characters!");
        openWarningScreen();
    }
    else
    {
        int userNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("userNumber");
        stringstream ss;
        
        bool exist = false;
        for (int i = 0; i < userNumber; i++)
        {
            ss.str(std::string());
            ss << i;
            string key = "user_" + ss.str();
            
            string user = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str());
            if (username.compare(user) == 0)
            {
                exist = true;
                break;
            }
        }
        
        if(exist)
        {
            warningDescription->setString("The username has already been taken. Try again!");
            openWarningScreen();
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("tempUser", username);
            CCUserDefault::sharedUserDefault()->setStringForKey("tempPassword", password);
            
            //isClose = false;
            mode = chooseCharacter;
            
            //scheduleLoginScreenJumpOut();
            scheduleLoginScreenJumpIn();
        }
    }
}

void MainMenuScene::clickBackButton()
{
    if(!isActive())
    {
        return;
    }
    
    if (warningBackground->isVisible())
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    mode = login;
    configMode();
}

void MainMenuScene::clickCreateButton()
{
    if(!isActive())
    {
        return;
    }
    
    if(warningBackground->isVisible())
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    mode = signup;
    configMode();
}

void MainMenuScene::scheduleLoginScreenJumpIn()
{
    if (isScoreScreenJumpIn || isScoreScreenJumpOut)
    {
        return;
    }
    
    disableModule();
    configMode();
    
    isScoreScreenJumpIn = true;
    loginBackground->setVisible(true);
    blackScreen->setOpacity((GLubyte) 0);
    loginBackground->setScale(0);
    this->schedule(schedule_selector(MainMenuScene::loginScreenJumpIn), 1.0f/120.0f);
}

void MainMenuScene::scheduleLoginScreenJumpOut()
{
    if (isScoreScreenJumpIn || isScoreScreenJumpOut)
    {
        return;
    }
    
    isScoreScreenJumpOut = true;
    blackScreen->setOpacity((GLubyte) 255);
    loginBackground->setScale(0.9f);
    this->schedule(schedule_selector(MainMenuScene::loginScreenJumpOut), 1.0f/120.0f);
}

void MainMenuScene::loginScreenJumpIn(float dt)
{
    float scale = loginBackground->getScale();
    scale += 0.05f;
    
    if(scale >= 0.9f)
    {
        scale = 0.9f;
        isScoreScreenJumpIn = false;
        isScoreScreenJumpOut = false;
        this->unschedule(schedule_selector(MainMenuScene::loginScreenJumpIn));
    }
    
    loginBackground->setScale(scale);
    blackScreen->setOpacity((GLubyte) ( 255.0f * scale / 0.9f ));

}

void MainMenuScene::loginScreenJumpOut(float dt)
{
    float scale = loginBackground->getScale();
    scale -= 0.05f;
    
    if(scale <= 0)
    {
        scale = 0;
        isScoreScreenJumpIn = false;
        isScoreScreenJumpOut = false;
        
        usernameBox->setText("");
        passwordBox->setText("");
        loginScreenWarningLabel->setString("");
        
        this->unschedule(schedule_selector(MainMenuScene::loginScreenJumpOut));
        
        enableModule();
        
        if (isClose)
        {
            loginBackground->setVisible(false);
        }
        else if(startGame)
        {
            startGame = false;
            this->scheduleOnce(schedule_selector(MainMenuScene::loadSenarioChooseScene), 0.1f);
        }
        else
        {
            // I think after change the structure, this branch should not be triggered any more
            isScoreScreenJumpIn = true;
            configMode();
            this->schedule(schedule_selector(MainMenuScene::loginScreenJumpIn), 1.0f/120.0f);
        }
    }
    
    loginBackground->setScale(scale);
    blackScreen->setOpacity((GLubyte) (255.0f * scale / 0.9f ));
}

void MainMenuScene::closeLoginScreen()
{
    SoundtrackManager::PlaySFX("Button_press.wav");
    isClose = true;
    scheduleLoginScreenJumpOut();
}

void MainMenuScene::loadSenarioChooseScene()
{
    this->setTouchEnabled(false);
    GlobalHelper::clearCache();
    clearMainMenuScene();
    // releaseMainMenuTextures();
    SenarioChooseScene::getThis()->loadTextures();
    SenarioChooseScene::getThis()->init();
    GameManager::getThis()->enableSenarioChooseScene();
}

void MainMenuScene::onButtonOptionsPressed(CCObject* pSender){
    if(!isActive())
    {
        return;
    }
    
    if (highScoreScreen->isVisible() || loginBackground->isVisible())
    {
        return;
    }
    
    if (isScoreScreenJumpIn || isScoreScreenJumpOut)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    scheduleScoreScreenJumpIn();
    
    string username = UserProfile::getThis()->username;
    
    stringstream ss;
    ss << username << "_level_0_score";
    float value0 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Tutorial high score is: " << value0;
    tutorialScoreLabel->setString(ss.str().c_str());
    
    ss.str(std::string());
    ss << username << "_level_1_score";
    float value1 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Level 1 high score is: " << value1;
    level1ScoreLabel->setString(ss.str().c_str());
    
    ss.str(std::string());
    ss << username << "_level_2_score";
    float value2 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Level 2 high score is: " << value2;
    level2ScoreLabel->setString(ss.str().c_str());
    
    ss.str(std::string());
    ss << username << "_level_3_score";
    float value3 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Level 3 high score is: " << value3;
    level3ScoreLabel->setString(ss.str().c_str());
    
    ss.str(std::string());
    ss << username << "_level_4_score";
    float value4 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Level 4 high score is: " << value4;
    level4ScoreLabel->setString(ss.str().c_str());
    
    ss.str(std::string());
    ss << username << "_level_5_score";
    float value5 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Level 5 high score is: " << value5;
    level5ScoreLabel->setString(ss.str().c_str());
    
    ss.str(std::string());
    ss << username << "_level_6_score";
    float value6 = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
    ss.str(std::string());
    ss << "Level 6 high score is: " << value6;
    level6ScoreLabel->setString(ss.str().c_str());
    
    float value = value1 + value0 + value2 + value3 + value4 + value5 + value6;
    
    ss.str(std::string());
    ss << "Total high score is: " << value;
    totalScoreLabel->setString(ss.str().c_str());
}

void MainMenuScene::showTeacherScreen()
{
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleTeacherManagementScreenOpen();
}

void MainMenuScene::onButtonExitPressed(CCObject* pSender)
{
    if(!isActive())
    {
        return;
    }
    
    if (highScoreScreen->isVisible() || loginBackground->isVisible() || mode == teacher)
    {
        return;
    }
    
    if (isScoreScreenJumpIn || isScoreScreenJumpOut)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    UserProfile::getThis()->username = "";
    UserProfile::getThis()->password = "";
    UserProfile::getThis()->gender = false;
    UserProfile::getThis()->isLoggedIn = false;
    
    usernameBox->setText("");
    passwordBox->setText("");
    
    mode = login;
    configMode();
    
    this->removeChild(backgroundImage);
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    backgroundImage = CCSprite::createWithSpriteFrameName("loginPage.jpg");
    backgroundImage->setScale(screenSize.width / backgroundImage->boundingBox().size.width);
    backgroundImage->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(backgroundImage, 0);
}

void MainMenuScene::openWarningScreen()
{
    if(!isActive())
    {
        return;
    }
    
    if (isWarningScreenJumpIn || isWarningScreenJumpOut)
    {
        return;
    }
    
    disableModule();
    
    scheduleWarningScreenJumpIn();
}

void MainMenuScene::openResetPasswordConfirmScreen(std::string username)
{
    if(!isActive())
    {
        return;
    }
    
    if (isWarningScreenJumpIn || isWarningScreenJumpOut)
    {
        return;
    }
    
    warningConfirmButton->setVisible(true);
    warningHead->setString("Reset Password");
    
    stringstream ss;
    ss << username << "_password";
    string password = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
    
    ss.str(std::string());
    ss << "Current password is : " << password << ". Do you want to reset it to : test?";
    
    warningDescription->setString(ss.str().c_str());
    
    scheduleWarningScreenJumpIn();
}

void MainMenuScene::closeResetPasswordConfirmScreen()
{
    if(!isActive())
    {
        return;
    }
    
    if (isWarningScreenJumpIn || isWarningScreenJumpOut)
    {
        return;
    }
    
    warningConfirmButton->setVisible(false);
    warningHead->setString("WARNING");
    
    string username = CCUserDefault::sharedUserDefault()->getStringForKey("currentWarningUsername");
    
    stringstream ss;
    ss << username << "_password";
    
    CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), "test");
    
    scheduleWarningScreenJumpOut();
}

void MainMenuScene::closeScoreScreen()
{
    if(!isActive())
    {
        return;
    }
    
    if (isScoreScreenJumpIn || isScoreScreenJumpOut)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleScoreScreenJumpOut();
}

void MainMenuScene::closeWarningScreen()
{
    if(!isActive())
    {
        return;
    }
    
    if (isWarningScreenJumpIn || isWarningScreenJumpOut)
    {
        return;
    }
    
    if(mode == teacher)
    {
        warningHead->setString("WARNING");
        warningDescription->setString("");
        warningConfirmButton->setVisible(false);
    }
    else
    {
        enableModule();
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleWarningScreenJumpOut();
}

void MainMenuScene::scheduleScoreScreenJumpIn()
{
    isScoreScreenJumpIn = true;
    highScoreScreen->setVisible(true);
    blackScreen->setOpacity((GLubyte) 0);
    highScoreScreen->setScale(0);
    this->schedule(schedule_selector(MainMenuScene::scoreScreenJumpIn), 1.0f/120.0f);
}

void MainMenuScene::scheduleScoreScreenJumpOut()
{
    isScoreScreenJumpOut = true;
    blackScreen->setOpacity((GLubyte) 255);
    highScoreScreen->setScale(0.9f);
    this->schedule(schedule_selector(MainMenuScene::scoreScreenJumpOut), 1.0f/120.0f);
}

void MainMenuScene::scheduleWarningScreenJumpIn()
{
    isWarningScreenJumpIn = true;
    warningBackground->setVisible(true);
    blackScreen->setOpacity((GLubyte) 0);
    warningBackground->setScale(0);
    this->schedule(schedule_selector(MainMenuScene::warningScreenJumpIn), 1.0f/120.0f);
}

void MainMenuScene::scheduleWarningScreenJumpOut()
{
    isWarningScreenJumpOut = true;
    blackScreen->setOpacity((GLubyte) 255);
    warningBackground->setScale(1.0f);
    this->schedule(schedule_selector(MainMenuScene::warningScreenJumpOut), 1.0f/120.0f);
}

void MainMenuScene::scoreScreenJumpIn(float dt)
{
    float scale = highScoreScreen->getScale();
    scale += 0.05f;
    
    if(scale >= 0.9f)
    {
        scale = 0.9f;
        isScoreScreenJumpIn = false;
        isScoreScreenJumpOut = false;
        this->unschedule(schedule_selector(MainMenuScene::scoreScreenJumpIn));
    }
    
    highScoreScreen->setScale(scale);
    blackScreen->setOpacity((GLubyte) ( 255.0f * scale / 0.9f ));
}

void MainMenuScene::scoreScreenJumpOut(float dt)
{
    float scale = highScoreScreen->getScale();
    scale -= 0.05f;
    
    if(scale <= 0)
    {
        scale = 0;
        isScoreScreenJumpIn = false;
        isScoreScreenJumpOut = false;
        this->unschedule(schedule_selector(MainMenuScene::scoreScreenJumpOut));
        highScoreScreen->setVisible(false);
        
        if(mode == teacher)
        {
            mode = login;
            configMode();
        }
    }
    
    highScoreScreen->setScale(scale);
    blackScreen->setOpacity((GLubyte) (255.0f * scale / 0.9f ));
}

void MainMenuScene::warningScreenJumpIn(float dt)
{
    float scale = warningBackground->getScale();
    scale += 0.05f;
    
    if(scale >= 1.0f)
    {
        scale = 1.0f;
        isWarningScreenJumpIn = false;
        isWarningScreenJumpOut = false;
        this->unschedule(schedule_selector(MainMenuScene::warningScreenJumpIn));
    }
    
    warningBackground->setScale(scale);
    blackScreen->setOpacity((GLubyte) ( 255.0f * scale / 1.0f ));
}

void MainMenuScene::warningScreenJumpOut(float dt)
{
    float scale = warningBackground->getScale();
    scale -= 0.05f;
    
    if(scale <= 0)
    {
        scale = 0;
        isWarningScreenJumpIn = false;
        isWarningScreenJumpOut = false;
        this->unschedule(schedule_selector(MainMenuScene::warningScreenJumpOut));
        warningBackground->setVisible(false);
    }
    
    warningBackground->setScale(scale);
    blackScreen->setOpacity((GLubyte) (255.0f * scale / 1.0f ));
}

void MainMenuScene::ccTouchesEnded(CCSet *touches, CCEvent *pEvent){
    if(!isActive())
    {
        return;
    }
    
    CCTouch* touch = (CCTouch*)*touches->begin();
    CCPoint touchLoc = touch->getLocation();
    
    if((boyHead->boundingBox().containsPoint(touchLoc) && boyHead->isVisible()) || (girlHead->boundingBox().containsPoint(touchLoc) && girlHead->isVisible()))
    {
        int userNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("userNumber");
        CCUserDefault::sharedUserDefault()->setIntegerForKey("userNumber", userNumber + 1);
        
        string tempUser = CCUserDefault::sharedUserDefault()->getStringForKey("tempUser");
        string tempPassword = CCUserDefault::sharedUserDefault()->getStringForKey("tempPassword");
        
        stringstream ss;
        ss << "user_" << userNumber;
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), tempUser);
        
        ss.str(std::string());
        ss << tempUser << "_password";
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), tempPassword);
        
        ss.str(std::string());
        ss << tempUser << "_gender";
        
        UserProfile::getThis()->username = tempUser;
        UserProfile::getThis()->password = tempPassword;
        UserProfile::getThis()->isLoggedIn = true;
        
        startGame = true;
        scheduleLoginScreenJumpOut();
        
        mode = login;
        configMode();
        
        /*
        if(boyHead->boundingBox().containsPoint(touchLoc))
        {
            UserProfile::getThis()->gender = true;
            CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
            
            AccountRow* accRow = AccountRow::create(scrollArea, userNumber);
            accRow->getUserAccount()->setAccountName(tempUser);
            accRow->refresh();
            accountRowArray->addObject((CCObject*)accRow);
            
            scrollArea->setScrollContentSize(CCSizeMake(teacherManagementBackground->boundingBox().size.width * 2.0f, 110.0f * userNumber));
            scrollArea->updateScrollBars();
        }
        else
        {
            UserProfile::getThis()->gender = false;
            CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
            
            AccountRow* accRow = AccountRow::create(scrollArea, userNumber);
            accRow->getUserAccount()->setAccountName(tempUser);
            accRow->refresh();
            accountRowArray->addObject((CCObject*)accRow);
            
            scrollArea->setScrollContentSize(CCSizeMake(teacherManagementBackground->boundingBox().size.width * 2.0f, 110.0f * userNumber));
            scrollArea->updateScrollBars();
        }
        */
    }
    
    if(mode == login || mode == signup)
    {
        if(warningBackground->isVisible() && !warningBackground->boundingBox().containsPoint(touchLoc))
        {
            closeWarningScreen();
        }
    }
    
    if(mode == chooseCharacter)
    {
        if(loginBackground->isVisible() && !loginBackground->boundingBox().containsPoint(touchLoc))
        {
            closeLoginScreen();
        }
    }
}

void MainMenuScene::update(float dt)
{
    if(isActive())
    {
        if(mode == chooseCharacter)
        {
            if(!isInBoyAnimation)
            {
                if(boyCheckTime <= 0)
                {
                    boyCheckTime = rand() % rangeTime + baseTime;
                    boyCumulativeTime = 0;
                }
                else
                {
                    boyCumulativeTime += dt;
                    if(boyCumulativeTime >= boyCheckTime)
                    {
                        boyCumulativeTime = 0;
                        boyCheckTime = 0;
                        
                        int randomNumber = rand() % 100 + 1;
                        if (randomNumber <= animationRatio)
                        {
                            isInBoyAnimation = true;
                            boyFrameNo = -1;
                        }
                        else
                        {
                            isInBoyAnimation = false;
                        }
                    }
                }
            }
            else
            {
                boy_delay_current -= dt;
                if (boy_delay_current <= 0)
                {
                    boyFrameNo++;
                    
                    if(boyFrameNo >= boyMaxFrameNo)
                    {
                        boyFrameNo = 0;
                        isInBoyAnimation = false;
                    }
                    
                    boyXOffset = boyFrameNo % 4;
                    boyYOffset = boyFrameNo / 4;
                    
                    boyRect.setRect(boyXOffset * frameWidth, boyYOffset * frameHeight, frameWidth, frameHeight);
                    boyHead->setTextureRect(boyRect);
                    boy_delay_current = boy_delay_animFrame;
                }
            }
            
            if(!isInGirlAnimation)
            {
                if(girlCheckTime <= 0)
                {
                    girlCheckTime = rand() % rangeTime + baseTime;
                    girlCumulativeTime = 0;
                }
                else
                {
                    girlCumulativeTime += dt;
                    if(girlCumulativeTime >= girlCheckTime)
                    {
                        girlCumulativeTime = 0;
                        girlCheckTime = 0;
                        
                        int randomNumber = rand() % 100 + 1;
                        if (randomNumber <= animationRatio)
                        {
                            isInGirlAnimation = true;
                            girlFrameNo = -1;
                        }
                        else
                        {
                            isInGirlAnimation = false;
                        }
                    }
                }
            }
            else
            {
                girl_delay_current -= dt;
                if (girl_delay_current <= 0)
                {
                    girlFrameNo++;
                    
                    if(girlFrameNo >= girlMaxFrameNo)
                    {
                        girlFrameNo = 0;
                        isInGirlAnimation = false;
                    }
                    
                    girlXOffset = girlFrameNo % 4;
                    girlYOffset = girlFrameNo / 4;
                    
                    girlRect.setRect(girlXOffset * frameWidth, girlYOffset * frameHeight, frameWidth, frameHeight);
                    girlHead->setTextureRect(girlRect);
                    girl_delay_current = girl_delay_animFrame;
                }
            }
        }
    }
}

void MainMenuScene::enableModule()
{
    usernameBox->setEnabled(true);
    passwordBox->setEnabled(true);
    loginButton->setEnabled(true);
    createButton->setEnabled(true);
    createAccountButton->setEnabled(true);
    backButton->setEnabled(true);
}

void MainMenuScene::disableModule()
{
    usernameBox->setEnabled(false);
    passwordBox->setEnabled(false);
    loginButton->setEnabled(false);
    createButton->setEnabled(false);
    createAccountButton->setEnabled(false);
    backButton->setEnabled(false);
}

void MainMenuScene::onOrientationChanged()
{
}

void MainMenuScene::setupTeacherManagementScreen()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    teacherManagementBackground = CCSprite::createWithSpriteFrameName("adminscreen.png");
    teacherManagementBackground->setAnchorPoint(ccp(0.5f, 0.5f));
    teacherManagementBackground->setScale(0.5f);
    teacherManagementBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    //CCSprite* button = CCSprite::createWithSpriteFrameName("logout.png");
    //CCSprite* buttonPressed = CCSprite::createWithSpriteFrameName("logout_press.png");
    //teacherManagementScreenLogoutButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(MainMenuScene::clearTeacherManagementScreen));
    teacherManagementScreenLogoutButton = CCMenuItemImage::create("logout.png", "logout_press.png", this, menu_selector(MainMenuScene::clearTeacherManagementScreen));
    teacherManagementScreenLogoutButton->setAnchorPoint(ccp(0.5f, 0.5f));
    teacherManagementScreenLogoutButton->setPosition(ccp(screenSize.width + 150.0f, -200.0f));
    teacherManagementScreenLogoutButton->setScale(0.7f);
    
    teacherManagementScreenMenu = CCMenu::create(teacherManagementScreenLogoutButton, NULL);
    
    teacherManagementBackground->addChild(teacherManagementScreenMenu, 2);
    
    teacherManagementScreenTitile = CCLabelTTF::create("Profile", "Shojumaru-Regular", 58);
    teacherManagementScreenTitile->setAnchorPoint(ccp(0.5f, 0.5f));
    teacherManagementScreenTitile->setPosition(ccp(screenSize.width, screenSize.height * 1.85f));
    
    teacherManagementBackground->addChild(teacherManagementScreenTitile);
    
    // scroll section for other villagers
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(teacherManagementBackground->boundingBox().size.width * 2.0f, teacherManagementBackground->boundingBox().size.height * 1.6f), CCSizeMake(teacherManagementBackground->boundingBox().size.width * 2.0f, teacherManagementBackground->boundingBox().size.height * 1.6f));
    scrollArea->enableScrollVertical(0, "bar.png", "bar.png");
    scrollArea->setAnchorPoint(ccp(0.5f, 0.5f));
    
    scrollArea->setPosition(ccp(0, screenSize.height * 0.15f));
    
    // CCUserDefault::sharedUserDefault()->setIntegerForKey("userNumber", 0);
    
    int userNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("userNumber");
    
    for (int i = 0; i < userNumber; i++)
    {
        AccountRow* accRow = AccountRow::create(scrollArea, i);
        
        stringstream ss;
        ss.str(std::string());
        ss << "user_" << i;
        std::string username = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        accRow->getUserAccount()->setAccountName(username);
        
        ss.str(std::string());
        ss << username << "_level_1_open";
        bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        accRow->getUserAccount()->setScenario1Status(flag);
        
        ss.str(std::string());
        ss << username << "_level_2_open";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        accRow->getUserAccount()->setScenario2Status(flag);
        
        ss.str(std::string());
        ss << username << "_level_3_open";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        accRow->getUserAccount()->setScenario3Status(flag);
        
        ss.str(std::string());
        ss << username << "_level_4_open";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        accRow->getUserAccount()->setScenario4Status(flag);
        
        ss.str(std::string());
        ss << username << "_level_5_open";
        flag = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
        accRow->getUserAccount()->setScenario5Status(flag);
        
        accRow->refresh();
        
        accountRowArray->addObject((CCObject*) accRow);
    }
    
    scrollArea->setScrollContentSize(CCSizeMake(teacherManagementBackground->boundingBox().size.width * 2.0f, 110.0f * userNumber));
    scrollArea->updateScrollBars();

    teacherManagementBackground->addChild(scrollArea, 1);
    
    this->addChild(teacherManagementBackground, 10);
    
    teacherManagementBackground->setScale(0.0f);
    teacherManagementBackground->setOpacity((GLubyte) 0.0f);
    teacherManagementBackground->setVisible(false);
}

void MainMenuScene::scheduleTeacherManagementScreenOpen()
{
    if(teacherManagementIsOpen || teacherManagementIsClose)
    {
        return;
    }
    
    teacherManagementIsOpen = true;
    
    teacherManagementBackground->setVisible(true);
    teacherManagementBackground->setScale(0.0f);
    teacherManagementBackground->setOpacity((GLubyte) 0.0f);
    
    this->schedule(schedule_selector(MainMenuScene::teacherManagementScreenOpen), 1.0f/120.0f);
}

void MainMenuScene::scheduleTeacherManagementScreenClose()
{
    if(teacherManagementIsOpen || teacherManagementIsClose)
    {
        return;
    }
    
    mode = login;
    
    teacherManagementIsClose = true;
    passwordBox->setText("");
    usernameBox->setText("");
    
    this->schedule(schedule_selector(MainMenuScene::teacherManagementScreenClose), 1.0f/120.0f);
}

void MainMenuScene::teacherManagementScreenOpen(float dt)
{
    float scale = teacherManagementBackground->getScale();
    
    scale += 0.025f;
    
    if (scale >= 0.5f)
    {
        scale = 0.5f;
        teacherManagementIsOpen = false;
        teacherManagementIsClose = false;
        this->unschedule(schedule_selector(MainMenuScene::teacherManagementScreenOpen));
    }
    
    teacherManagementBackground->setScale(scale);
    
    float opacity = 255.0f * (scale / 0.5f);
    
    teacherManagementBackground->setOpacity((GLubyte) opacity);
    blackScreen->setOpacity((GLubyte) opacity);
}

void MainMenuScene::teacherManagementScreenClose(float dt)
{
    float scale = teacherManagementBackground->getScale();
    
    scale -= 0.025f;
    
    if (scale <= 0.0f)
    {
        scale = 0.0f;
        teacherManagementIsOpen = false;
        teacherManagementIsClose = false;
        teacherManagementBackground->setVisible(false);
        configMode();
        teacherManagementBackground->setScale(scale);
        float opacity = 255.0f * (scale / 0.5f);
        teacherManagementBackground->setOpacity((GLubyte) opacity);
        blackScreen->setOpacity((GLubyte) opacity);
        this->unschedule(schedule_selector(MainMenuScene::teacherManagementScreenClose));
        removeTeacherManagementScreen();
        // releaseTeacherManagementTextures();
    }
    else
    {
        teacherManagementBackground->setScale(scale);
    
        float opacity = 255.0f * (scale / 0.5f);
        
        teacherManagementBackground->setOpacity((GLubyte) opacity);
        blackScreen->setOpacity((GLubyte) opacity);
    }
}

void MainMenuScene::removeTeacherManagementScreen()
{
    for(int i = 0; i < accountRowArray->count(); i++)
    {
        AccountRow* ar = (AccountRow*) accountRowArray->objectAtIndex(i);
        ar->clear();
    }
    
    accountRowArray->removeAllObjects();
    
    teacherManagementScreenMenu->removeChild(teacherManagementScreenLogoutButton, true);
    teacherManagementBackground->removeChild(teacherManagementScreenMenu, true);
    teacherManagementBackground->removeChild(teacherManagementScreenTitile, true);
    teacherManagementBackground->removeChild(scrollArea, true);
    
    this->removeChild(teacherManagementBackground, true);
    
    GlobalHelper::clearCache();
}

void MainMenuScene::clearTeacherManagementScreen()
{
    if(!isActive())
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleTeacherManagementScreenClose();
}

void MainMenuScene::deleteAccountRow(int index)
{
    if(!isActive())
    {
        return;
    }
    
    for (int i = index; i < accountRowArray->count(); i++)
    {
        if (i < accountRowArray->count() - 1)
        {
            AccountRow* preA = (AccountRow*) accountRowArray->objectAtIndex(i);
            AccountRow* postA = (AccountRow*) accountRowArray->objectAtIndex(i + 1);
            
            preA->getUserAccount()->setAccountName(postA->getUserAccount()->getAccountName());
            preA->getUserAccount()->setScenario1Status(postA->getUserAccount()->getScenario1Status());
            preA->getUserAccount()->setScenario2Status(postA->getUserAccount()->getScenario2Status());
            preA->getUserAccount()->setScenario3Status(postA->getUserAccount()->getScenario3Status());
            preA->getUserAccount()->setScenario4Status(postA->getUserAccount()->getScenario4Status());
            preA->getUserAccount()->setScenario5Status(postA->getUserAccount()->getScenario5Status());
            preA->refresh();
        }
        else
        {
            AccountRow* accRow = (AccountRow*) accountRowArray->objectAtIndex(i);
            accRow->clear();
            accountRowArray->removeObject((CCObject*)accRow);
        }
    }
    
    int userNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("userNumber");
    scrollArea->setScrollContentSize(CCSizeMake(teacherManagementBackground->boundingBox().size.width * 2.0f, 110.0f * userNumber));
    scrollArea->updateScrollBars();
}

bool MainMenuScene::isActive()
{
    return MainMenuScene::getThis()->isVisible();
}

void MainMenuScene::loadTeacherManagementTextures()
{
    // teacher management node
    teacherManagementNode = CCSpriteBatchNode::create("teacherManagement.png");
    this->addChild(teacherManagementNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("teacherManagement.plist");
}

void MainMenuScene::releaseTeacherManagementTextures()
{
    this->removeChild(teacherManagementNode, true);
    teacherManagementNode = NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("teacherManagement.plist");
}

void MainMenuScene::loadMainMenuTextures()
{
    mainMenuSceneNode = CCSpriteBatchNode::create("MainGamePageTexture.png");
    this->addChild(mainMenuSceneNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("MainGamePageTexture.plist");
}

void MainMenuScene::releaseMainMenuTextures()
{
    this->removeChild(mainMenuSceneNode, true);
    mainMenuSceneNode = NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("MainGamePageTexture.plist");
}