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

using namespace cocos2d;

SenarioChooseScene* SenarioChooseScene::SP;


SenarioChooseScene::SenarioChooseScene()
{
    SenarioChooseScene::SP = this;
    
    isSaveFileChooseScreenOpening = false;
    isSaveFileChooseScreenClosing = false;
    
    isGoingToLoadGame = false;
    
    preloadTextures();
}

SenarioChooseScene::~SenarioChooseScene()
{
    SenarioChooseScene::SP = NULL;
}

SenarioChooseScene* SenarioChooseScene::getThis()
{
    return SP;
}

CCScene* SenarioChooseScene::scene()
{
    srand(time(NULL));
    
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SenarioChooseScene *layer = SenarioChooseScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool SenarioChooseScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCRect boxrect = CCRectMake(0,0, screenSize.width * 0.8f,  screenSize.width * 0.2f);
    CCRect exitrect = CCRectMake(0,0, screenSize.width * 0.15f, screenSize.width * 0.15f);
    
    CCRect loadButtonRect = CCRectMake(0, 0, screenSize.width * 0.15f, screenSize.width * 0.15f);
    
    std::string usrname = GameManager::getThis()->username;
    
    backgroundImage = CCSprite::createWithSpriteFrameName("screenlevelselect_v2.png");
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
    
    senarioButtonS3 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario3));
    senarioButtonS3->setContentSize(boxrect.size);
    senarioButtonS3->setVisible(false);
    
    senarioButtonS4 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario4));
    senarioButtonS4->setContentSize(boxrect.size);
    senarioButtonS4->setVisible(false);
    
    senarioButtonS5 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario5));
    senarioButtonS5->setContentSize(boxrect.size);
    senarioButtonS5->setVisible(false);
    
    CCSprite* backButton = CCSprite::createWithSpriteFrameName("back_icon.png");
    senarioButtonS6 = CCMenuItemSprite::create(backButton, backButton, this, menu_selector(SenarioChooseScene::chooseSenario6));
    senarioButtonS6->setContentSize(exitrect.size);
    
    loadGameButton = CCMenuItemImage::create("load_icon.png", "load_icon.png", this, menu_selector(SenarioChooseScene::chooseLoadGame));
    loadGameButton->setContentSize(loadButtonRect.size);
    loadGameButton->setAnchorPoint(ccp(0, 0.5f));
    
    CCLabelTTF* tutorialLabel = CCLabelTTF::create("Tutorial", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    // tutorialLabel->setAnchorPoint(ccp(0.5f, 0.5));
    tutorialLabel->setPosition( ccp(senarioButtonTutorial->boundingBox().size.width * 0.5f, senarioButtonTutorial->boundingBox().size.height * 0.5f));
    senarioButtonTutorial->addChild(tutorialLabel);
    tutorialLabel->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel1 = CCLabelTTF::create("Scenario 1", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel1->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel1->setPosition( ccp(senarioButtonS1->boundingBox().size.width * 0.5f, senarioButtonS1->boundingBox().size.height * 0.5f));
    senarioButtonS1->addChild(senarioLabel1);
    senarioLabel1->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel2 = CCLabelTTF::create("Scenario 2", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel2->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel2->setPosition( ccp(senarioButtonS2->boundingBox().size.width * 0.5f, senarioButtonS2->boundingBox().size.height * 0.5f));
    senarioButtonS2->addChild(senarioLabel2);
    senarioLabel2->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel3 = CCLabelTTF::create("Scenario 3", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel3->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel3->setPosition( ccp(senarioButtonS3->boundingBox().size.width * 0.5f, senarioButtonS3->boundingBox().size.height * 0.5f));
    senarioButtonS3->addChild(senarioLabel3);
    senarioLabel3->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel4 = CCLabelTTF::create("Scenario 4", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel4->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel4->setPosition( ccp(senarioButtonS4->boundingBox().size.width * 0.5f, senarioButtonS4->boundingBox().size.height * 0.5f));
    senarioButtonS4->addChild(senarioLabel4);
    senarioLabel4->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel5 = CCLabelTTF::create("Scenario 5", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel5->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel5->setPosition( ccp(senarioButtonS5->boundingBox().size.width * 0.5f, senarioButtonS5->boundingBox().size.height * 0.5f));
    senarioButtonS5->addChild(senarioLabel5);
    senarioLabel5->setColor(ccc3(255,189,68));
    
    // CCLabelTTF* senarioLabel6 = CCLabelTTF::create("B", "Shojumaru-Regular" ,28, exitrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel6->setAnchorPoint(ccp(0.5f, 0.5));
    // senarioLabel6->setPosition( ccp(senarioButtonS6->boundingBox().size.width * 0.5f, senarioButtonS6->boundingBox().size.height * 0.5f));
    // senarioButtonS6->addChild(senarioLabel6);
    // senarioLabel6->setColor(ccc3(255,189,68));

    
    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
 //       backgroundImage->setScale(1)
        senarioButtonTutorial->setScale(0.4);
        senarioButtonS1->setScale(0.4);
        senarioButtonS2->setScale(0.4);
        senarioButtonS3->setScale(0.4);
        senarioButtonS4->setScale(0.4);
        senarioButtonS5->setScale(0.4);
        senarioButtonS6->setScale(0.4);
        
        loadGameButton->setScale(0.4);
    } else {
   //     background
        senarioButtonTutorial->setScale(0.8);
        senarioButtonS1->setScale(0.8);
        senarioButtonS2->setScale(0.8);
        senarioButtonS3->setScale(0.8);
        senarioButtonS4->setScale(0.8);
        senarioButtonS5->setScale(0.8);
        senarioButtonS6->setScale(0.8);
        
        loadGameButton->setScale(0.8);
    }
    //onOrientateChange();
    
    backgroundImage->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));
    senarioButtonTutorial->setPosition(ccp(screenSize.width * 0.348, screenSize.height * 0.685));
    senarioButtonS1->setPosition(ccp(screenSize.width * 0.348, screenSize.height * 0.475));
    senarioButtonS2->setPosition(ccp(screenSize.width * 0.348, screenSize.height * 0.262));
    senarioButtonS3->setPosition(ccp(screenSize.width * 0.672, screenSize.height * 0.685));
    senarioButtonS4->setPosition(ccp(screenSize.width * 0.672, screenSize.height * 0.475));
    senarioButtonS5->setPosition(ccp(screenSize.width * 0.672, screenSize.height * 0.262));
    senarioButtonS6->setPosition(ccp(screenSize.width * 0.95, screenSize.height * 0.05));
    
    loadGameButton->setPosition(ccp(screenSize.width * 0.015f, screenSize.height * 0.05f));
    
    senarioChooseSceneMenu = CCMenu::create(senarioButtonTutorial, senarioButtonS1, senarioButtonS2, senarioButtonS3, senarioButtonS4, senarioButtonS5, senarioButtonS6, loadGameButton, NULL);
    senarioChooseSceneMenu->setPosition( CCPointZero );
    this->addChild(senarioChooseSceneMenu, 1);
    this->addChild(backgroundImage, 0);
    
    /* loading screen module */
    loadingScreen = CCSprite::create("loading screen.png");
    loadingScreen->setScale(screenSize.width / loadingScreen->boundingBox().size.width);
    loadingScreen->setAnchorPoint(ccp(0.5, 0.5));
    loadingScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(loadingScreen, 10);
    loadingScreen->setVisible(false);
    
    loadingLabel = CCSprite::create("loading.png");
    loadingLabel->setAnchorPoint(ccp(0.5, 0.5));
    loadingLabel->setScale(0.5f);
    loadingLabel->setPosition(ccp(screenSize.width / 2.0f + 20.0f, screenSize.height / 2.0f - 120.0f));
    this->addChild(loadingLabel, 11);
    loadingLabel->setVisible(false);
    
    setupSaveFileChooseScreen();
    
    return true;
}

void SenarioChooseScene::chooseTutorial()
{
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    enableLoadingScreen();
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingTutorial), 0.1f);
}

void SenarioChooseScene::loadingTutorial()
{
    // update the level number in the Game Manager
    GameManager::getThis()->setLevel(0);
    
    // load the actual game scene
    GlobalHelper::clearCache();
    GlobalHelper::clearPreloadedTexture();
    
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void SenarioChooseScene::chooseSenario1()
{
    if(saveFileChooseBackground->isVisible())
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
    enableLoadingScreen();
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario1), 0.1f);
}

void SenarioChooseScene::loadingSenario1()
{
    // update the level number in the Game Manager
    GameManager::getThis()->setLevel(1);
    
    // load the actual game scene
    GlobalHelper::clearCache();
    GlobalHelper::clearPreloadedTexture();
    
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void SenarioChooseScene::chooseSenario2()
{
    if(saveFileChooseBackground->isVisible())
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
    enableLoadingScreen();
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario2), 0.1f);
}

void SenarioChooseScene::loadingSenario2()
{
    GameManager::getThis()->setLevel(2);
    GlobalHelper::clearCache();
    GlobalHelper::clearPreloadedTexture();
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void SenarioChooseScene::chooseSenario3()
{
    if(saveFileChooseBackground->isVisible())
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
}

void SenarioChooseScene::loadingSenario3()
{
    
}

void SenarioChooseScene::chooseSenario4()
{
    if(saveFileChooseBackground->isVisible())
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
}

void SenarioChooseScene::loadingSenario4()
{
    
}

void SenarioChooseScene::chooseSenario5()
{
    if(saveFileChooseBackground->isVisible())
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
}

void SenarioChooseScene::loadingSenario5()
{
    
}

//THIS IS ACTUALLY THE GO BACK BUTTON.
void SenarioChooseScene::chooseSenario6()
{
    if(saveFileChooseBackground->isVisible())
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    enableLoadingScreen();
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingSenario6), 0.1f);
}

void SenarioChooseScene::loadingSenario6()
{
    GlobalHelper::clearCache();
    GlobalHelper::clearPreloadedTexture();
    CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

void SenarioChooseScene::enableLoadingScreen()
{
    this->removeChild(backgroundImage);
    this->removeChild(senarioChooseSceneMenu);
    
    loadingScreen->setVisible(true);
    loadingLabel->setVisible(true);
}

void SenarioChooseScene::onOrientateChange(){
}

void SenarioChooseScene::onOrientationChangedToPortrait(){
}

void SenarioChooseScene::onOrientationChangedToLandscape(){
}

void SenarioChooseScene::preloadTextures()
{
    CCSpriteBatchNode* mainMenuSceneNode = CCSpriteBatchNode::create("LevelSelectSceneSpriteSheet.png");
    this->addChild(mainMenuSceneNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("LevelSelectSceneSpriteSheet.plist");
}

void SenarioChooseScene::loadAutoSaveGame()
{
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << "loadingGameType";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 0);
    
    string username = GameManager::getThis()->username;
    ss.str(std::string());
    ss << "autosave_" << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameManager::getThis()->setLevel(levelNumber);
    
    isGoingToLoadGame = true;
    
    enableLoadingScreen();
    // this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingLoadGame), 0.1f);
    
    scheduleSaveFileChooseScreenClosing();
}

void SenarioChooseScene::loadFixedSaveGame()
{
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << "loadingGameType";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 1);
    
    string username = GameManager::getThis()->username;
    ss.str(std::string());
    ss << "fixedsave_" << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameManager::getThis()->setLevel(levelNumber);
    
    isGoingToLoadGame = true;
    
    enableLoadingScreen();
    // this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingLoadGame), 0.1f);
    
    scheduleSaveFileChooseScreenClosing();
}

void SenarioChooseScene::loadCustomSaveGame()
{
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << "loadingGameType";
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), 2);
    
    string username = GameManager::getThis()->username;
    ss.str(std::string());
    ss << "customsave_" << username << "_levelNumber";
    int levelNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str());
    GameManager::getThis()->setLevel(levelNumber);
    
    isGoingToLoadGame = true;
    
    enableLoadingScreen();
    // this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingLoadGame), 0.1f);
    
    scheduleSaveFileChooseScreenClosing();
}

void SenarioChooseScene::chooseLoadGame()
{
    
    
    scheduleSaveFileChooseScreenOpening();
    /*
    stringstream ss;
    ss << "isLoadingGame";
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    enableLoadingScreen();
    this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingLoadGame), 0.1f);
    */
}

void SenarioChooseScene::loadingLoadGame()
{
    // update the level number in the Game Manager, if loading game fails, it will auto direct the game to tutorial
    // GameManager::getThis()->setLevel(0);
    
    // load the actual game scene
    GlobalHelper::clearCache();
    GlobalHelper::clearPreloadedTexture();
    
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void SenarioChooseScene::setupSaveFileChooseScreen()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    saveFileChooseBackground = CCSprite::create("popup_dialogbox.png");
    saveFileChooseBackground->setAnchorPoint(ccp(0.5f, 0.5f));
    saveFileChooseBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    loadScreenTitle = CCLabelTTF::create("Load Saved Game", "Shojumaru-Regular", 36);
    loadScreenTitle->setAnchorPoint(ccp(0.5f, 0.5f));
    loadScreenTitle->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    saveFileChooseBackground->addChild(loadScreenTitle);
    
    closeButton = CCMenuItemImage::create("Closebtn_Sq.png", "Closebtn_Sq.png", this, menu_selector(SenarioChooseScene::scheduleSaveFileChooseScreenClosing));
    closeButton->setAnchorPoint(ccp(0.5f, 0.5f));
    closeButton->setPosition(ccp(screenSize.width / 2.0f - 160.0f, -5.0f));
    
    autoLoadButton = CCMenuItemImage::create("load.png", "load_press.png", this, menu_selector(SenarioChooseScene::loadAutoSaveGame));
    autoLoadButton->setAnchorPoint(ccp(0.5f, 0.5f));
    autoLoadButton->setScale(0.6f);
    autoLoadButton->setPosition(ccp(-350.0f, -80.0f));
    
    autoLoadDisableButton = CCMenuItemImage::create("load_disable.png", "load_press.png", this, menu_selector(SenarioChooseScene::loadDisableWarning));
    autoLoadDisableButton->setAnchorPoint(ccp(0.5f, 0.5f));
    autoLoadDisableButton->setScale(0.6f);
    autoLoadDisableButton->setPosition(ccp(-350.0f, -80.0f));
    
    autoLoadHandleButton = CCMenuItemImage::create("load_button.png", "load_button_press.png", this, menu_selector(SenarioChooseScene::loadAutoSaveGame));
    autoLoadHandleButton->setAnchorPoint(ccp(0.0f, 0.5f));
    autoLoadHandleButton->setScale(0.6f);
    autoLoadHandleButton->setPosition(ccp(200.0f, -80.0f));
    
    fixedLoadButton = CCMenuItemImage::create("load.png", "load_press.png", this, menu_selector(SenarioChooseScene::loadFixedSaveGame));
    fixedLoadButton->setAnchorPoint(ccp(0.5f, 0.5f));
    fixedLoadButton->setScale(0.6f);
    fixedLoadButton->setPosition(ccp(-350.0f, -160.0f));
    
    fixedLoadDisableButton = CCMenuItemImage::create("load_disable.png", "load_press.png", this, menu_selector(SenarioChooseScene::loadDisableWarning));
    fixedLoadDisableButton->setAnchorPoint(ccp(0.5f, 0.5f));
    fixedLoadDisableButton->setScale(0.6f);
    fixedLoadDisableButton->setPosition(ccp(-350.0f, -160.0f));
    
    fixedLoadHandleButton = CCMenuItemImage::create("load_button.png", "load_button_press.png", this, menu_selector(SenarioChooseScene::loadFixedSaveGame));
    fixedLoadHandleButton->setAnchorPoint(ccp(0.0f, 0.5f));
    fixedLoadHandleButton->setScale(0.6f);
    fixedLoadHandleButton->setPosition(ccp(200.0f, -160.0f));
    
    customLoadButton = CCMenuItemImage::create("load.png", "load_press.png", this, menu_selector(SenarioChooseScene::loadCustomSaveGame));
    customLoadButton->setAnchorPoint(ccp(0.5f, 0.5f));
    customLoadButton->setScale(0.6f);
    customLoadButton->setPosition(ccp(-350.0f, -240.0f));
    
    customLoadDisableButton = CCMenuItemImage::create("load_disable.png", "load_press.png", this, menu_selector(SenarioChooseScene::loadDisableWarning));
    customLoadDisableButton->setAnchorPoint(ccp(0.5f, 0.5f));
    customLoadDisableButton->setScale(0.6f);
    customLoadDisableButton->setPosition(ccp(-350.0f, -240.0f));
    
    customLoadHandleButton = CCMenuItemImage::create("load_button.png", "load_button_press.png", this, menu_selector(SenarioChooseScene::loadCustomSaveGame));
    customLoadHandleButton->setAnchorPoint(ccp(0.0f, 0.5f));
    customLoadHandleButton->setScale(0.6f);
    customLoadHandleButton->setPosition(ccp(200.0f, -240.0f));
    
    saveFileMenu = CCMenu::create(closeButton, autoLoadButton, autoLoadDisableButton, autoLoadHandleButton, fixedLoadButton, fixedLoadDisableButton, fixedLoadHandleButton, customLoadButton, customLoadDisableButton, customLoadHandleButton, NULL);
    saveFileChooseBackground->addChild(saveFileMenu);
    
    autoSaveLabel = CCLabelTTF::create("Autosave:", "GillSansMT", 24);
    autoSaveLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    autoSaveLabel->setPosition(ccp(screenSize.width / 2.0f - 300.0f, screenSize.height / 2.0f - 65.0f));
    
    saveFileChooseBackground->addChild(autoSaveLabel);
    
    autoSaveStatusLabel = CCLabelTTF::create("Autosave", "GillSansMT", 24);
    autoSaveStatusLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    autoSaveStatusLabel->setPosition(ccp(screenSize.width / 2.0f - 150.0f, screenSize.height / 2.0f - 65.0f));
    
    saveFileChooseBackground->addChild(autoSaveStatusLabel);
    
    fixedSaveLabel = CCLabelTTF::create("Fixedsave:", "GillSansMT", 24);
    fixedSaveLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    fixedSaveLabel->setPosition(ccp(screenSize.width / 2.0f - 300.0f, screenSize.height / 2.0f - 145.0f));
    
    saveFileChooseBackground->addChild(fixedSaveLabel);
    
    fixedSaveStatusLabel = CCLabelTTF::create("Fixedsave", "GillSansMT", 24);
    fixedSaveStatusLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    fixedSaveStatusLabel->setPosition(ccp(screenSize.width / 2.0f - 150.0f, screenSize.height / 2.0f - 145.0f));
    
    saveFileChooseBackground->addChild(fixedSaveStatusLabel);
    
    customSaveLabel = CCLabelTTF::create("Customsave:", "GillSansMT", 24);
    customSaveLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    customSaveLabel->setPosition(ccp(screenSize.width / 2.0f - 300.0f, screenSize.height / 2.0f - 225.0f));
    
    saveFileChooseBackground->addChild(customSaveLabel);
    
    customSaveStatusLabel = CCLabelTTF::create("Customsave", "GillSansMT", 24);
    customSaveStatusLabel->setAnchorPoint(ccp(0.0f, 1.0f));
    customSaveStatusLabel->setPosition(ccp(screenSize.width / 2.0f - 150.0f, screenSize.height / 2.0f - 225.0f));
    
    saveFileChooseBackground->addChild(customSaveStatusLabel);
    
    this->addChild(saveFileChooseBackground, 11);
    
    blackScreen = CCSprite::create("blackscreen.png");
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
    this->schedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenOpen), 1.0f/120.0f);
}

void SenarioChooseScene::scheduleSaveFileChooseScreenClosing()
{
    if(isSaveFileChooseScreenOpening || isSaveFileChooseScreenClosing)
    {
        return;
    }
    
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    isSaveFileChooseScreenClosing = true;
    this->schedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenClose), 1.0f/120.0f);
}

void SenarioChooseScene::saveFileChooseScreenOpen(float dt)
{
    float scale = saveFileChooseBackground->getScale();
    scale += 0.05f;
    
    if(scale >= 1.0f)
    {
        scale = 1.0f;
        isSaveFileChooseScreenOpening = false;
        this->unschedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenOpen));
    }
    
    saveFileChooseBackground->setScale(scale);
    
    float opacity = 255.0f * (scale / 1.0f);
    
    saveFileChooseBackground->setOpacity((GLubyte) opacity);
    blackScreen->setOpacity((GLubyte) opacity);
}

void SenarioChooseScene::saveFileChooseScreenClose(float dt)
{
    float scale = saveFileChooseBackground->getScale();
    scale -= 0.05f;
    
    if(scale <= 0.0f)
    {
        scale = 0.0f;
        isSaveFileChooseScreenClosing = false;
        saveFileChooseBackground->setVisible(false);
        this->unschedule(schedule_selector(SenarioChooseScene::saveFileChooseScreenClose));
        if(isGoingToLoadGame)
        {
            isGoingToLoadGame = false;
            this->scheduleOnce(schedule_selector(SenarioChooseScene::loadingLoadGame), 0.1f);
        }
    }
    
    saveFileChooseBackground->setScale(scale);
    
    float opacity = 255.0f * (scale / 1.0f);
    
    saveFileChooseBackground->setOpacity((GLubyte) opacity);
    blackScreen->setOpacity((GLubyte) opacity);
}

void SenarioChooseScene::loadDisableWarning()
{
    // by now, nothing will happen. Reserve for possible warning screen.
}

void SenarioChooseScene::checkLoadStatus()
{
    string username = GameManager::getThis()->username;
    
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