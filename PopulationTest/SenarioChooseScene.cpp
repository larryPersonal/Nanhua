//
//  SenarioChooseScene.cpp
//  PopulationTest
//
//  Created by SGI on 12/12/13.
//
//

#include "SenarioChooseScene.h"
#include "GameScene.h"
#include "GameManager.h"
#include "MainMenuScene.h"
#include "SoundtrackManager.h"
#include "GlobalHelper.h"
#include "UserProfile.h"

using namespace cocos2d;

SenarioChooseScene* SenarioChooseScene::SP;


SenarioChooseScene::SenarioChooseScene()
{
    SenarioChooseScene::SP = this;
    
    isSaveFileChooseScreenOpening = false;
    isSaveFileChooseScreenClosing = false;
    
    isTutorialChooseScreenClosing = false;
    isTutorialChooseScreenOpening = false;
    
    goToPlayTutorial = false;
    goToSkipTutorial = false;
    
    isGoingToLoadGame = false;
}

SenarioChooseScene::~SenarioChooseScene()
{
    SenarioChooseScene::SP = NULL;
}

SenarioChooseScene* SenarioChooseScene::getThis()
{
    return SP;
}

SenarioChooseScene* SenarioChooseScene::create()
{
    SenarioChooseScene* pRet = new SenarioChooseScene();
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

bool SenarioChooseScene::init()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCRect boxrect = CCRectMake(0,0, screenSize.width * 0.8f,  screenSize.width * 0.2f);
    
    CCRect exitrect = CCRectMake(0,0, screenSize.width * 0.2f, screenSize.width * 0.2f);
    CCRect loadButtonRect = CCRectMake(0,0, screenSize.width * 0.2f, screenSize.width * 0.2f);
    
    std::string usrname = UserProfile::getThis()->username;
    
    backgroundImage = CCSprite::createWithSpriteFrameName("screenlevelselect_v2.jpg");
    backgroundImage->setScale(screenSize.width/backgroundImage->boundingBox().size.width);
    
    senarioButtonTutorial = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseTutorial));
    senarioButtonTutorial->setContentSize(boxrect.size);

    stringstream ss;
    ss.str(std::string());
    ss << usrname << "_level_1_open";
    
    senarioButtonS1 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario1));
    senarioButtonS1->setContentSize(boxrect.size);
    bool value = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    senarioButtonS1->setVisible(value);
    
    ss.str(std::string());
    ss << usrname << "_level_2_open";
    
    senarioButtonS2 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario2));
    senarioButtonS2->setContentSize(boxrect.size);
    value = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    senarioButtonS2->setVisible(value);
    
    ss.str(std::string());
    ss << usrname << "_level_3_open";
    
    senarioButtonS3 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario3));
    senarioButtonS3->setContentSize(boxrect.size);
    value = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    senarioButtonS3->setVisible(value);
    
    ss.str(std::string());
    ss << usrname << "_level_4_open";
    
    senarioButtonS4 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario4));
    senarioButtonS4->setContentSize(boxrect.size);
    value = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    senarioButtonS4->setVisible(value);
    
    ss.str(std::string());
    ss << usrname << "_level_5_open";
    
    senarioButtonS5 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario5));
    senarioButtonS5->setContentSize(boxrect.size);
    value = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    senarioButtonS5->setVisible(value);
    
    CCSprite* backButton = CCSprite::createWithSpriteFrameName("back_icon.png");
    senarioButtonS6 = CCMenuItemSprite::create(backButton, backButton, this, menu_selector(SenarioChooseScene::chooseSenario6));
    senarioButtonS6->setContentSize(exitrect.size);
    senarioButtonS6->setAnchorPoint(ccp(0, 0.5f));
    
    loadGameButton = CCMenuItemImage::create("load_icon.png", "load_icon.png", this, menu_selector(SenarioChooseScene::chooseLoadGame));
    // loadGameButton = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseLoadGame));
    loadGameButton->setContentSize(loadButtonRect.size);
    loadGameButton->setAnchorPoint(ccp(1.0f, 0.5f));
    
    tutorialLabel = CCLabelTTF::create("Scenario 1", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    tutorialLabel->setPosition( ccp(senarioButtonTutorial->boundingBox().size.width * 0.5f, senarioButtonTutorial->boundingBox().size.height * 0.5f));
    senarioButtonTutorial->addChild(tutorialLabel);
    tutorialLabel->setColor(ccc3(255,189,68));
    
    senarioLabel1 = CCLabelTTF::create("Scenario 2", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    senarioLabel1->setPosition( ccp(senarioButtonS1->boundingBox().size.width * 0.5f, senarioButtonS1->boundingBox().size.height * 0.5f));
    senarioButtonS1->addChild(senarioLabel1);
    senarioLabel1->setColor(ccc3(255,189,68));
    
    senarioLabel2 = CCLabelTTF::create("Scenario 3", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    senarioLabel2->setPosition( ccp(senarioButtonS2->boundingBox().size.width * 0.5f, senarioButtonS2->boundingBox().size.height * 0.5f));
    senarioButtonS2->addChild(senarioLabel2);
    senarioLabel2->setColor(ccc3(255,189,68));
    
    senarioLabel3 = CCLabelTTF::create("Scenario 4", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    senarioLabel3->setPosition( ccp(senarioButtonS3->boundingBox().size.width * 0.5f, senarioButtonS3->boundingBox().size.height * 0.5f));
    senarioButtonS3->addChild(senarioLabel3);
    senarioLabel3->setColor(ccc3(255,189,68));
    
    senarioLabel4 = CCLabelTTF::create("Scenario 5", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    senarioLabel4->setPosition( ccp(senarioButtonS4->boundingBox().size.width * 0.5f, senarioButtonS4->boundingBox().size.height * 0.5f));
    senarioButtonS4->addChild(senarioLabel4);
    senarioLabel4->setColor(ccc3(255,189,68));
    
    senarioLabel5 = CCLabelTTF::create("Scenario 6", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    senarioLabel5->setPosition( ccp(senarioButtonS5->boundingBox().size.width * 0.5f, senarioButtonS5->boundingBox().size.height * 0.5f));
    senarioButtonS5->addChild(senarioLabel5);
    senarioLabel5->setColor(ccc3(255,189,68));

    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        senarioButtonTutorial->setScale(0.4);
        senarioButtonS1->setScale(0.4);
        senarioButtonS2->setScale(0.4);
        senarioButtonS3->setScale(0.4);
        senarioButtonS4->setScale(0.4);
        senarioButtonS5->setScale(0.4);
        senarioButtonS6->setScale(0.5);
        
        loadGameButton->setScale(0.5);
    } else {
        senarioButtonTutorial->setScale(0.8);
        senarioButtonS1->setScale(0.8);
        senarioButtonS2->setScale(0.8);
        senarioButtonS3->setScale(0.8);
        senarioButtonS4->setScale(0.8);
        senarioButtonS5->setScale(0.8);
        senarioButtonS6->setScale(1.0);
        
        loadGameButton->setScale(1.0);
    }
    
    backgroundImage->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));
    senarioButtonTutorial->setPosition(ccp(screenSize.width * 0.348, screenSize.height * 0.685));
    senarioButtonS1->setPosition(ccp(screenSize.width * 0.348, screenSize.height * 0.475));
    senarioButtonS2->setPosition(ccp(screenSize.width * 0.348, screenSize.height * 0.262));
    senarioButtonS3->setPosition(ccp(screenSize.width * 0.672, screenSize.height * 0.685));
    senarioButtonS4->setPosition(ccp(screenSize.width * 0.672, screenSize.height * 0.475));
    senarioButtonS5->setPosition(ccp(screenSize.width * 0.672, screenSize.height * 0.262));
    
    senarioButtonS6->setPosition(ccp(screenSize.width * 0.015f, screenSize.height * 0.075f));
    loadGameButton->setPosition(ccp(screenSize.width * 1.0f, screenSize.height * 0.075f));
    
    senarioChooseSceneMenu = CCMenu::create(senarioButtonTutorial, senarioButtonS1, senarioButtonS2, senarioButtonS3, senarioButtonS4, senarioButtonS5, senarioButtonS6, loadGameButton, NULL);
    senarioChooseSceneMenu->setPosition( CCPointZero );
    this->addChild(senarioChooseSceneMenu, 1);
    this->addChild(backgroundImage, 0);
    
    setupSaveFileChooseScreen();
    setupTutorialChooseScreen();
    
    return true;
}

void SenarioChooseScene::chooseTutorial()
{
    if(!isVisible())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    scheduleTutorialChooseScreenOpening();
}

void SenarioChooseScene::playTutorial()
{
    if(!isVisible())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    goToPlayTutorial = true;
    goToSkipTutorial = false;
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleTutorialChooseScreenClosing();
    
    // this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingTutorial), 0.1f);
}

void SenarioChooseScene::loadingTutorial()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 0;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

void SenarioChooseScene::playScenario1()
{
    if(!isVisible())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    goToSkipTutorial = true;
    goToPlayTutorial = false;
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    scheduleTutorialChooseScreenClosing();
    
    // this->scheduleOnce(schedule_selector(SenarioChooseScene::chooseLoadScenario1), 0.1f);
}

void SenarioChooseScene::chooseLoadScenario1()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 1;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

void SenarioChooseScene::chooseSenario1()
{
    if(!isActive())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    if(!senarioButtonS1->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario1), 0.1f);
}

void SenarioChooseScene::loadingSenario1()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 2;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

void SenarioChooseScene::chooseSenario2()
{
    if(!isActive())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    if(!senarioButtonS2->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario2), 0.1f);
}

void SenarioChooseScene::loadingSenario2()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 3;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

void SenarioChooseScene::chooseSenario3()
{
    if(!isActive())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    if(!senarioButtonS3->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario3), 0.1f);
}

void SenarioChooseScene::loadingSenario3()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 4;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

void SenarioChooseScene::chooseSenario4()
{
    if(!isActive())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    if(!senarioButtonS4->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario4), 0.1f);
}

void SenarioChooseScene::loadingSenario4()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 5;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

void SenarioChooseScene::chooseSenario5()
{
    if(!isActive())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    if(!senarioButtonS5->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario5), 0.1f);
}

void SenarioChooseScene::loadingSenario5()
{
    // update the level number in the Game Manager
    UserProfile::getThis()->gameLevel = 6;
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::startGame), 0.1f);
}

//THIS IS ACTUALLY THE GO BACK BUTTON.
void SenarioChooseScene::chooseSenario6()
{
    if(!isActive())
    {
        return;
    }
    
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario6), 0.1f);
}

void SenarioChooseScene::loadingSenario6()
{
    clear();
    releaseTextures();
    MainMenuScene::getThis()->init();
    GlobalHelper::clearCache();
    GameManager::getThis()->enableMainMenuScene();
}

void SenarioChooseScene::onOrientateChange(){
}

void SenarioChooseScene::onOrientationChangedToPortrait(){
}

void SenarioChooseScene::onOrientationChangedToLandscape(){
}

void SenarioChooseScene::loadAutoSaveGame()
{
    if(isActive())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << "loadingGameType";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 0);
    
    string username = UserProfile::getThis()->username;
    ss.str(std::string());
    ss << "autosave_" << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    UserProfile::getThis()->gameLevel = levelNumber;
    
    isGoingToLoadGame = true;
    
    scheduleSaveFileChooseScreenClosing();
}

void SenarioChooseScene::loadFixedSaveGame()
{
    if(!isActive())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << "loadingGameType";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 1);
    
    string username = UserProfile::getThis()->username;
    ss.str(std::string());
    ss << "fixedsave_" << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    UserProfile::getThis()->gameLevel = levelNumber;
    
    isGoingToLoadGame = true;
    
    scheduleSaveFileChooseScreenClosing();
}

void SenarioChooseScene::loadCustomSaveGame()
{
    if(!isActive())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << "loadingGameType";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 2);
    
    string username = UserProfile::getThis()->username;
    ss.str(std::string());
    ss << "customsave_" << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    UserProfile::getThis()->gameLevel = levelNumber;
    
    isGoingToLoadGame = true;
    
    scheduleSaveFileChooseScreenClosing();
}

void SenarioChooseScene::chooseLoadGame()
{
    if(!isActive())
    {
        return;
    }
    
    if(tutorialChooseBackground->isVisible())
    {
        return;
    }
    
    scheduleSaveFileChooseScreenOpening();
}

void SenarioChooseScene::loadingLoadGame()
{
    // load the actual game scene
    GlobalHelper::clearCache();
    
    clear();
    MainMenuScene::getThis()->releaseMainMenuTextures();
    MainMenuScene::getThis()->releaseTeacherManagementTextures();
    releaseTextures();
    
    // load the actual game scene
    GlobalHelper::clearCache();
    
    GameScene::getThis()->scheduleOnce(schedule_selector(GameScene::loadGame), 0.1f);
}

void SenarioChooseScene::setupSaveFileChooseScreen()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    saveFileChooseBackground = CCSprite::createWithSpriteFrameName("popup_dialogbox.png");
    saveFileChooseBackground->setAnchorPoint(ccp(0.5f, 0.5f));
    saveFileChooseBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    loadScreenTitle = CCLabelTTF::create("Load Saved Game", "Shojumaru-Regular", 36);
    loadScreenTitle->setAnchorPoint(ccp(0.5f, 0.5f));
    loadScreenTitle->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    saveFileChooseBackground->addChild(loadScreenTitle);
    
    CCSprite* button = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    CCSprite* buttonPressed = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    closeButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::scheduleSaveFileChooseScreenClosing));
    closeButton->setAnchorPoint(ccp(0.5f, 0.5f));
    closeButton->setPosition(ccp(screenSize.width / 2.0f - 160.0f, -5.0f));
    
    button = CCSprite::createWithSpriteFrameName("load.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_press.png");
    autoLoadButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadAutoSaveGame));
    autoLoadButton->setAnchorPoint(ccp(0.5f, 0.5f));
    autoLoadButton->setScale(0.6f);
    autoLoadButton->setPosition(ccp(-350.0f, -85.0f));
    
    button = CCSprite::createWithSpriteFrameName("load_disable.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_press.png");
    autoLoadDisableButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadDisableWarning));
    autoLoadDisableButton->setAnchorPoint(ccp(0.5f, 0.5f));
    autoLoadDisableButton->setScale(0.6f);
    autoLoadDisableButton->setPosition(ccp(-350.0f, -85.0f));
    
    button = CCSprite::createWithSpriteFrameName("load_button.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_button_press.png");
    autoLoadHandleButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadAutoSaveGame));
    autoLoadHandleButton->setAnchorPoint(ccp(0.0f, 0.5f));
    autoLoadHandleButton->setScale(0.6f);
    autoLoadHandleButton->setPosition(ccp(200.0f, -85.0f));
    
    button = CCSprite::createWithSpriteFrameName("load.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_press.png");
    fixedLoadButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadFixedSaveGame));
    fixedLoadButton->setAnchorPoint(ccp(0.5f, 0.5f));
    fixedLoadButton->setScale(0.6f);
    fixedLoadButton->setPosition(ccp(-350.0f, -165.0f));
    
    button = CCSprite::createWithSpriteFrameName("load_disable.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_press.png");
    fixedLoadDisableButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadDisableWarning));
    fixedLoadDisableButton->setAnchorPoint(ccp(0.5f, 0.5f));
    fixedLoadDisableButton->setScale(0.6f);
    fixedLoadDisableButton->setPosition(ccp(-350.0f, -165.0f));
    
    button = CCSprite::createWithSpriteFrameName("load_button.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_button_press.png");
    fixedLoadHandleButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadFixedSaveGame));
    fixedLoadHandleButton->setAnchorPoint(ccp(0.0f, 0.5f));
    fixedLoadHandleButton->setScale(0.6f);
    fixedLoadHandleButton->setPosition(ccp(200.0f, -165.0f));
    
    button = CCSprite::createWithSpriteFrameName("load.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_press.png");
    customLoadButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadCustomSaveGame));
    customLoadButton->setAnchorPoint(ccp(0.5f, 0.5f));
    customLoadButton->setScale(0.6f);
    customLoadButton->setPosition(ccp(-350.0f, -245.0f));
    
    button = CCSprite::createWithSpriteFrameName("load_disable.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_press.png");
    customLoadDisableButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadDisableWarning));
    customLoadDisableButton->setAnchorPoint(ccp(0.5f, 0.5f));
    customLoadDisableButton->setScale(0.6f);
    customLoadDisableButton->setPosition(ccp(-350.0f, -245.0f));
    
    button = CCSprite::createWithSpriteFrameName("load_button.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("load_button_press.png");
    customLoadHandleButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::loadCustomSaveGame));
    customLoadHandleButton->setAnchorPoint(ccp(0.0f, 0.5f));
    customLoadHandleButton->setScale(0.6f);
    customLoadHandleButton->setPosition(ccp(200.0f, -245.0f));
    
    saveFileMenu = CCMenu::create(closeButton, autoLoadButton, autoLoadDisableButton, autoLoadHandleButton, fixedLoadButton, fixedLoadDisableButton, fixedLoadHandleButton, customLoadButton, customLoadDisableButton, customLoadHandleButton, NULL);
    saveFileChooseBackground->addChild(saveFileMenu);
    
    autoSaveLabel = CCLabelTTF::create("Autosave:", "GillSansMT", 24);
    autoSaveLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    autoSaveLabel->setPosition(ccp(screenSize.width / 2.0f - 300.0f, screenSize.height / 2.0f - 70.0f));
    
    saveFileChooseBackground->addChild(autoSaveLabel);
    
    autoSaveStatusLabel = CCLabelTTF::create("Autosave", "GillSansMT", 24);
    autoSaveStatusLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    autoSaveStatusLabel->setPosition(ccp(screenSize.width / 2.0f - 150.0f, screenSize.height / 2.0f - 70.0f));
    
    saveFileChooseBackground->addChild(autoSaveStatusLabel);
    
    fixedSaveLabel = CCLabelTTF::create("Fixedsave:", "GillSansMT", 24);
    fixedSaveLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    fixedSaveLabel->setPosition(ccp(screenSize.width / 2.0f - 300.0f, screenSize.height / 2.0f - 150.0f));
    
    saveFileChooseBackground->addChild(fixedSaveLabel);
    
    fixedSaveStatusLabel = CCLabelTTF::create("Fixedsave", "GillSansMT", 24);
    fixedSaveStatusLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    fixedSaveStatusLabel->setPosition(ccp(screenSize.width / 2.0f - 150.0f, screenSize.height / 2.0f - 150.0f));
    
    saveFileChooseBackground->addChild(fixedSaveStatusLabel);
    
    customSaveLabel = CCLabelTTF::create("Customsave:", "GillSansMT", 24);
    customSaveLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    customSaveLabel->setPosition(ccp(screenSize.width / 2.0f - 300.0f, screenSize.height / 2.0f - 230.0f));
    
    saveFileChooseBackground->addChild(customSaveLabel);
    
    customSaveStatusLabel = CCLabelTTF::create("Customsave", "GillSansMT", 24);
    customSaveStatusLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    customSaveStatusLabel->setPosition(ccp(screenSize.width / 2.0f - 150.0f, screenSize.height / 2.0f - 230.0f));
    
    saveFileChooseBackground->addChild(customSaveStatusLabel);
    
    this->addChild(saveFileChooseBackground, 11);
    
    blackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    blackScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(blackScreen, 10);
    
    saveFileChooseBackground->setScale(0.001f);
    saveFileChooseBackground->setOpacity((GLubyte) 0.0f);
    
    saveFileChooseBackground->setVisible(false);
    
    blackScreen->setOpacity((GLubyte) 0.0f);
    
    checkLoadStatus();
}

void SenarioChooseScene::setupTutorialChooseScreen()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tutorialChooseBackground = CCSprite::createWithSpriteFrameName("popup_dialogbox.png");
    tutorialChooseBackground->setAnchorPoint(ccp(0.5f, 0.5f));
    tutorialChooseBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    TutorialChooseLabel = CCLabelTTF::create("Do you want to play Tutorial?", "Shojumaru-Regular", 36);
    TutorialChooseLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    TutorialChooseLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 100));
    tutorialChooseBackground->addChild(TutorialChooseLabel);
    
    CCSprite* button = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    CCSprite* buttonPressed = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    TutorialChooseCloseButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::scheduleTutorialChooseScreenClosing));
    TutorialChooseCloseButton->setAnchorPoint(ccp(0.5f, 0.5f));
    TutorialChooseCloseButton->setPosition(ccp(screenSize.width / 2.0f - 160.0f, -5.0f));
    
    button = CCSprite::createWithSpriteFrameName("play.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("play_press.png");
    tutorialChoosePlayButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::playTutorial));
    tutorialChoosePlayButton->setAnchorPoint(ccp(0.5f, 0.5f));
    tutorialChoosePlayButton->setScale(0.4f);
    tutorialChoosePlayButton->setPosition(ccp(-230.0f, -225.0f));
    
    button = CCSprite::createWithSpriteFrameName("skip.png");
    buttonPressed = CCSprite::createWithSpriteFrameName("skip_press.png");
    tutorialChooseSkipButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector(SenarioChooseScene::playScenario1));
    tutorialChooseSkipButton->setAnchorPoint(ccp(0.5f, 0.5f));
    tutorialChooseSkipButton->setScale(0.4f);
    tutorialChooseSkipButton->setPosition(ccp(230.0f, -225.0f));
    
    tutorialMenu = CCMenu::create(TutorialChooseCloseButton, tutorialChoosePlayButton, tutorialChooseSkipButton, NULL);
    tutorialChooseBackground->addChild(tutorialMenu);
    
    this->addChild(tutorialChooseBackground, 11);
    
    tutorialChooseBlackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    tutorialChooseBlackScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    tutorialChooseBlackScreen->cocos2d::CCNode::setScale(screenSize.width / tutorialChooseBlackScreen->boundingBox().size.width, screenSize.height / tutorialChooseBlackScreen->boundingBox().size.height);
    tutorialChooseBlackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(tutorialChooseBlackScreen, 10);
    
    tutorialChooseBackground->setScale(0.001f);
    tutorialChooseBackground->setOpacity((GLubyte) 0.0f);
    
    tutorialChooseBackground->setVisible(false);
    
    tutorialChooseBlackScreen->setOpacity((GLubyte) 0.0f);
}

void SenarioChooseScene::scheduleSaveFileChooseScreenOpening()
{
    if(isSaveFileChooseScreenOpening || isSaveFileChooseScreenClosing)
    {
        return;
    }
    
    checkLoadStatus();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    saveFileChooseBackground->setVisible(true);
    
    isSaveFileChooseScreenOpening = true;
    isSaveFileChooseScreenClosing = false;
    this->schedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenOpen), 1.0f/120.0f);
}

void SenarioChooseScene::scheduleTutorialChooseScreenOpening()
{
    if(isTutorialChooseScreenOpening || isTutorialChooseScreenClosing)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    tutorialChooseBackground->setVisible(true);
    
    isTutorialChooseScreenOpening = true;
    isTutorialChooseScreenClosing = false;
    this->schedule(schedule_selector(SenarioChooseScene::tutorialChooseScreenOpen), 1.0f/120.0f);
}

void SenarioChooseScene::scheduleSaveFileChooseScreenClosing()
{
    if(!isActive())
    {
        return;
    }
    
    if(isSaveFileChooseScreenOpening || isSaveFileChooseScreenClosing)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    isSaveFileChooseScreenClosing = true;
    isSaveFileChooseScreenOpening = false;
    this->schedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenClose), 1.0f/120.0f);
}

void SenarioChooseScene::scheduleTutorialChooseScreenClosing()
{
    if(!isActive())
    {
        return;
    }
    
    if(isTutorialChooseScreenOpening || isTutorialChooseScreenClosing)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    isTutorialChooseScreenClosing = true;
    isTutorialChooseScreenOpening = false;
    this->schedule(schedule_selector(SenarioChooseScene::tutorialChooseScreenClose), 1.0f/120.0f);
}

void SenarioChooseScene::saveFileChooseScreenOpen(float dt)
{
    float scale = saveFileChooseBackground->getScale();
    scale += 0.05f;
    
    if(scale >= 1.0f)
    {
        scale = 1.0f;
        isSaveFileChooseScreenOpening = false;
        isSaveFileChooseScreenClosing = false;
        this->unschedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenOpen));
    }
    
    saveFileChooseBackground->setScale(scale);
    
    float opacity = 255.0f * (scale / 1.0f);
    
    saveFileChooseBackground->setOpacity((GLubyte) opacity);
    blackScreen->setOpacity((GLubyte) opacity);
}

void SenarioChooseScene::tutorialChooseScreenOpen(float dt)
{
    float scale = tutorialChooseBackground->getScale();
    scale += 0.05f;
    
    if(scale >= 1.0f)
    {
        scale = 1.0f;
        isTutorialChooseScreenOpening = false;
        isTutorialChooseScreenClosing = false;
        this->unschedule(schedule_selector(SenarioChooseScene::tutorialChooseScreenOpen));
    }
    
    tutorialChooseBackground->setScale(scale);
    
    float opacity = 255.0f * (scale / 1.0f);
    
    tutorialChooseBackground->setOpacity((GLubyte) opacity);
    tutorialChooseBlackScreen->setOpacity((GLubyte) opacity);
}

void SenarioChooseScene::saveFileChooseScreenClose(float dt)
{
    float scale = saveFileChooseBackground->getScale();
    scale -= 0.05f;
    
    if(scale <= 0.0f)
    {
        scale = 0.0f;
        isSaveFileChooseScreenClosing = false;
        isSaveFileChooseScreenOpening = false;
        saveFileChooseBackground->setVisible(false);
        this->unschedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenClose));
        if(isGoingToLoadGame)
        {
            isGoingToLoadGame = false;
            this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingLoadGame), 0.1f);
        }
    }
    else
    {
        saveFileChooseBackground->setScale(scale);
        
        float opacity = 255.0f * (scale / 1.0f);
        
        saveFileChooseBackground->setOpacity((GLubyte) opacity);
        blackScreen->setOpacity((GLubyte) opacity);
    }
}

void SenarioChooseScene::tutorialChooseScreenClose(float dt)
{
    float scale = tutorialChooseBackground->getScale();
    scale -= 0.05f;
    
    if(scale <= 0.0f)
    {
        scale = 0.0f;
        isTutorialChooseScreenClosing = false;
        isTutorialChooseScreenOpening = false;
        tutorialChooseBackground->setVisible(false);
        this->unschedule(schedule_selector(SenarioChooseScene::tutorialChooseScreenClose));
        // play 1 or play tutorial
        if(goToPlayTutorial)
        {
            this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingTutorial), 0.1f);
        }
        else if(goToSkipTutorial)
        {
            this->scheduleOnce(schedule_selector(SenarioChooseScene::chooseLoadScenario1), 0.1f);
        }
        
        goToPlayTutorial = false;
        goToSkipTutorial = false;
    }
    else
    {
        tutorialChooseBackground->setScale(scale);
        float opacity = 255.0f * (scale / 1.0f);
        
        tutorialChooseBackground->setOpacity((GLubyte) opacity);
        tutorialChooseBlackScreen->setOpacity((GLubyte) opacity);
    }
}

void SenarioChooseScene::loadDisableWarning()
{
    // by now, nothing will happen. Reserve for possible warning screen.
}

void SenarioChooseScene::clear()
{
    // clear load module
    saveFileMenu->removeChild(closeButton, true);
    
    saveFileMenu->removeChild(autoLoadButton, true);
    saveFileMenu->removeChild(autoLoadDisableButton, true);
    saveFileMenu->removeChild(autoLoadHandleButton, true);
    
    saveFileMenu->removeChild(fixedLoadButton, true);
    saveFileMenu->removeChild(fixedLoadDisableButton, true);
    saveFileMenu->removeChild(fixedLoadHandleButton, true);
    
    saveFileMenu->removeChild(customLoadButton, true);
    saveFileMenu->removeChild(customLoadDisableButton, true);
    saveFileMenu->removeChild(customLoadHandleButton, true);
    
    saveFileChooseBackground->removeChild(loadScreenTitle, true);
    saveFileChooseBackground->removeChild(saveFileMenu, true);
    
    saveFileChooseBackground->removeChild(autoSaveLabel, true);
    saveFileChooseBackground->removeChild(autoSaveStatusLabel, true);
    
    saveFileChooseBackground->removeChild(fixedSaveLabel, true);
    saveFileChooseBackground->removeChild(fixedSaveStatusLabel, true);
    
    saveFileChooseBackground->removeChild(customSaveLabel, true);
    saveFileChooseBackground->removeChild(customSaveStatusLabel, true);
    
    this->removeChild(saveFileChooseBackground, true);
    this->removeChild(blackScreen, true);
    
    // clear level select scene
    senarioButtonTutorial->removeChild(tutorialLabel, true);
    senarioButtonS1->removeChild(senarioLabel1, true);
    senarioButtonS2->removeChild(senarioLabel2, true);
    senarioButtonS3->removeChild(senarioLabel3, true);
    senarioButtonS4->removeChild(senarioLabel4, true);
    senarioButtonS5->removeChild(senarioLabel5, true);
    
    senarioChooseSceneMenu->removeChild(senarioButtonTutorial, true);
    senarioChooseSceneMenu->removeChild(senarioButtonS1, true);
    senarioChooseSceneMenu->removeChild(senarioButtonS2, true);
    senarioChooseSceneMenu->removeChild(senarioButtonS3, true);
    senarioChooseSceneMenu->removeChild(senarioButtonS4, true);
    senarioChooseSceneMenu->removeChild(senarioButtonS5, true);
    senarioChooseSceneMenu->removeChild(senarioButtonS6, true);
    senarioChooseSceneMenu->removeChild(loadGameButton, true);
    
    this->removeChild(senarioChooseSceneMenu, true);
    this->removeChild(backgroundImage, true);
}

void SenarioChooseScene::checkLoadStatus()
{
    string username = UserProfile::getThis()->username;
    
    stringstream ss;
    
    ss.str(std::string());
    ss << "autosave_" << username << "_hasSavedGame";
    bool autosaveStatus = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    
    ss.str(std::string());
    ss << "fixedsave_" << username << "_hasSavedGame";
    bool fixedsaveStatus = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    
    ss.str(std::string());
    ss << "customsave_" << username << "_hasSavedGame";
    bool customsaveStatus = CCUserDefault::sharedUserDefault()->getBoolForKey(ss.str().c_str());
    
    if(autosaveStatus)
    {
        autoLoadButton->setVisible(true);
        autoLoadDisableButton->setVisible(false);
        autoLoadHandleButton->setVisible(true);
        
        ss.str(std::string());
        ss << "autosave_" << username << "_levelNumber";
        int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << "Level: " << levelNumber;
        autoSaveStatusLabel->setString(ss.str().c_str());
    }
    else
    {
        autoLoadButton->setVisible(false);
        autoLoadDisableButton->setVisible(true);
        autoLoadHandleButton->setVisible(false);
        
        autoSaveStatusLabel->setString("No record found!");
    }
    
    if(fixedsaveStatus)
    {
        fixedLoadButton->setVisible(true);
        fixedLoadDisableButton->setVisible(false);
        fixedLoadHandleButton->setVisible(true);
        
        ss.str(std::string());
        ss << "fixedsave_" << username << "_levelNumber";
        int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << "Level: " << levelNumber;
        fixedSaveStatusLabel->setString(ss.str().c_str());
    }
    else
    {
        fixedLoadButton->setVisible(false);
        fixedLoadDisableButton->setVisible(true);
        fixedLoadHandleButton->setVisible(false);
        
        fixedSaveStatusLabel->setString("No record found!");
    }
    
    if(customsaveStatus)
    {
        customLoadButton->setVisible(true);
        customLoadDisableButton->setVisible(false);
        customLoadHandleButton->setVisible(true);
        
        ss.str(std::string());
        ss << "customsave_" << username << "_levelNumber";
        int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << "Level: " << levelNumber;
        customSaveStatusLabel->setString(ss.str().c_str());
    }
    else
    {
        customLoadButton->setVisible(false);
        customLoadDisableButton->setVisible(true);
        customLoadHandleButton->setVisible(false);
        
        customSaveStatusLabel->setString("No record found!");
    }
}

bool SenarioChooseScene::isActive()
{
    return SenarioChooseScene::getThis()->isVisible();
}

void SenarioChooseScene::loadTextures()
{
    // level select scene
    mainMenuSceneNode = CCSpriteBatchNode::create("LevelSelectSceneSpriteSheet.png");
    this->addChild(mainMenuSceneNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("LevelSelectSceneSpriteSheet.plist");
}

void SenarioChooseScene::releaseTextures()
{
    this->removeChild(mainMenuSceneNode, true);
    mainMenuSceneNode = NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("LevelSelectSceneSpriteSheet.plist");
}