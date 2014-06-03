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
#include "HighScoreScene.h"
#include "OptionsScene.h"
#include "GameManager.h"
#include "AlertBox.h"
#include "SoundtrackManager.h"

using namespace CocosDenshion;

MainMenuScene* MainMenuScene::SP;

MainMenuScene::MainMenuScene()
{
    MainMenuScene::SP = this;
}

MainMenuScene::~MainMenuScene()
{
    MainMenuScene::SP = NULL;
}

MainMenuScene* MainMenuScene::getThis()
{
    return SP;
}

CCScene* MainMenuScene::scene()
{
    srand(time(NULL));
    
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MainMenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    CCRect boxrect = CCRectMake(0,0, screenSize.width * 0.8f,  screenSize.width * 0.1f);
    
    backgroundImage = CCSprite::create("newsplashpage2.png");
    backgroundImage->setScale(screenSize.width/backgroundImage->boundingBox().size.width);
    
    buttonStart = CCMenuItemImage::create("", "", this, menu_selector(MainMenuScene::onButtonStartPressed));
    buttonOptions = CCMenuItemImage::create("", "", this, menu_selector(MainMenuScene::onButtonOptionsPressed));
    
    buttonStart->setContentSize(boxrect.size);
    buttonOptions->setContentSize(boxrect.size);
    
    buttonStart->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonStart->setPosition(ccp(0, -120));
    
    buttonOptions->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonOptions->setPosition(0, -220);
    
    // buttonCredits = CCMenuItemImage::create("quit.png", "press_quit.png", this, menu_selector(MainMenuScene::onButtonCreditsPressed));
    
    /* loading screen module */
    loadingScreen = CCSprite::create("loading screen.png");
    loadingScreen->setScale(screenSize.width / loadingScreen->boundingBox().size.width);
    loadingScreen->setAnchorPoint(ccp(0.5, 0.5));
    loadingScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(loadingScreen, 10);
    loadingScreen->setVisible(false);
    
    loadingLabel = CCSprite::create("loading.png");
    loadingLabel->setAnchorPoint(ccp(0.5, 0.5));
    loadingLabel->setPosition(ccp(screenSize.width / 2.0f + 20.0f, screenSize.height / 2.0f - 120.0f));
    loadingLabel->setScale(0.5f);
    this->addChild(loadingLabel, 11);
    loadingLabel->setVisible(false);
    
    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        backgroundImage->setScale(0.5);
        buttonStart->setScale(0.5);
        buttonOptions->setScale(0.5);
    //    buttonCredits->setScale(0.5);
    }
    onOrientationChanged();

 
    //Need to call this once to init the positions and scales
    
    //Create menu, it's an autorelease object
    CCMenu* menu = CCMenu::create(buttonStart, buttonOptions, NULL);
    this->addChild(menu, 1);
    this->addChild(backgroundImage, 0);
    

    SoundtrackManager::PlayBGM("Ishikari Lore.mp3");
    
    return true;
}

void MainMenuScene::onButtonStartPressed(CCObject* pSender){
    /*
    if (GameManager::getThis()->checkGameDataExists())
    {
    
        AlertBox* alert = AlertBox::create();
        alert->useAsExclusivePopupMenu();
        alert->setDisplayText("Load a previously saved game?");
    
        alert->addButton(0, "Yes", this, callfuncO_selector(MainMenuScene::onLoadGame), pSender);
        alert->addButton(0, "No", this, callfuncO_selector(MainMenuScene::onNewGame), pSender);
    }
    else
    {
        onNewGame(NULL);
    }
    */
    SoundtrackManager::PlaySFX("Button_press.wav");
    
    onNewGame(NULL);
}

void MainMenuScene::onLoadGame(cocos2d::CCObject *pSender)
{
    PopupMenu::closeAllPopupMenu();
    
    //GameManager::getThis()->setTutorialMode(true);
    
    //GameManager::getThis()->setLoadedGame(true);
    //GameManager::getThis()->newGameData();.
    
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
    CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

}

void MainMenuScene::onNewGame(cocos2d::CCObject *pSender)
{
    enableLoadingScreen();
    this->scheduleOnce(schedule_selector(MainMenuScene::loadSenarioChooseScene), 0.1f);
}

void MainMenuScene::loadSenarioChooseScene()
{
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
    CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
    
    CCDirector::sharedDirector()->purgeCachedData();
    CCDirector::sharedDirector()->replaceScene(SenarioChooseScene::scene());
}

void MainMenuScene::enableLoadingScreen()
{
    this->removeChild(backgroundImage);
    this->removeChild(buttonStart);
    this->removeChild(buttonOptions);
    
    loadingScreen->setVisible(true);
    loadingLabel->setVisible(true);
}

void MainMenuScene::onRejectTutorial(cocos2d::CCObject *pSender)
{
//    GameManager::getThis()->setTutorialMode(false);
//    GameManager::getThis()->setLoadedGame(true);
    
 //   CCDirector::sharedDirector()->pushScene(GameScene::scene());
    //PopupMenu::closeAllPopupMenu();
    
    //GameManager::getThis()->setTutorialMode(false);
    //GameManager::getThis()->setLoadedGame(false);
    
    //GameManager::getThis()->newGameData();
    
    //CCDirector::sharedDirector()->pushScene(GameScene::scene());

    
}

void MainMenuScene::onAcceptTutorial(cocos2d::CCObject *pSender)
{
    PopupMenu::closeAllPopupMenu();
    
    //GameManager::getThis()->setTutorialMode(true);
    //GameManager::getThis()->setLoadedGame(false);
    
    //GameManager::getThis()->newGameData();
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
    
    //CCDirector::sharedDirector()->pushScene(GameScene::scene());
    
}

void MainMenuScene::onButtonOptionsPressed(CCObject* pSender){
    //CCDirector::sharedDirector()->pushScene(OptionsScene::scene());
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void MainMenuScene::onButtonCreditsPressed(CCObject* pSender){
   // CCDirector::sharedDirector()->pushScene(HighScoreScene::scene());
}

/*
void MainMenuScene::onButtonQuitPressed(CCObject* pSender){
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
*/

void MainMenuScene::onOrientationChanged(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    backgroundImage->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.5));
}

void MainMenuScene::onOrientationChangedToPortrait(){
}

void MainMenuScene::onOrientationChangedToLandscape(){
}
