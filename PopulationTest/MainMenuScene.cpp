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
    
    
    backgroundImage = CCSprite::create("splash0.png");
    backgroundImage->setScale(screenSize.width/backgroundImage->boundingBox().size.width);
    
    backgroundDeco = CCSprite::create("splash1.png");
    
    backgroundDeco2 = CCSprite::create("student.png");

    buttonStart = CCMenuItemImage::create("start.png", "press_start.png", this, menu_selector(MainMenuScene::onButtonStartPressed));
    buttonOptions = CCMenuItemImage::create("options.png", "press_options.png", this, menu_selector(MainMenuScene::onButtonOptionsPressed));
   // buttonCredits = CCMenuItemImage::create("quit.png", "press_quit.png", this, menu_selector(MainMenuScene::onButtonCreditsPressed));
    
    
    CCLabelTTF* startLabel = CCLabelTTF::create("", "Shojumaru-Regular" ,128, buttonStart->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    startLabel->setPosition( ccp(buttonStart->boundingBox().size.width * 0.5f, buttonStart->boundingBox().size.height * 0.5f));
    buttonStart->addChild(startLabel);
    startLabel->setColor(ccc3(255,189,68));
    
    CCLabelTTF* optionsLabel = CCLabelTTF::create("", "Shojumaru-Regular" ,128, buttonOptions->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    optionsLabel->setPosition( ccp(buttonOptions->boundingBox().size.width * 0.5f, buttonOptions->boundingBox().size.height * 0.5f));
    buttonOptions->addChild(optionsLabel);
    optionsLabel->setColor(ccc3(255,189,68));
    /*
    CCLabelTTF* creditsLabel = CCLabelTTF::create("", "Shojumaru-Regular" ,128, buttonCredits->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    creditsLabel->setPosition( ccp(buttonCredits->boundingBox().size.width * 0.5f, buttonCredits->boundingBox().size.height * 0.5f));
    buttonCredits->addChild(creditsLabel);
    creditsLabel->setColor(ccc3(255,189,68));
    */
    
    
    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        backgroundImage->setScale(0.5);
        backgroundDeco->setScale(0.5);
        backgroundDeco2->setScale(0.5);
        buttonStart->setScale(0.5);
        buttonOptions->setScale(0.5);
    //    buttonCredits->setScale(0.5);
    }
    onOrientationChanged();

 
    //Need to call this once to init the positions and scales
    
    //Create menu, it's an autorelease object
    CCMenu* menu = CCMenu::create(buttonStart, buttonOptions, NULL);
    menu->setPosition( CCPointZero );
    this->addChild(menu, 1);
    this->addChild(backgroundImage, 0);
    this->addChild(backgroundDeco, 0);
    this->addChild(backgroundDeco2, 0);
    

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
    
    onNewGame(NULL);
}

void MainMenuScene::onLoadGame(cocos2d::CCObject *pSender)
{
    PopupMenu::closeAllPopupMenu();
    
    //GameManager::getThis()->setTutorialMode(true);
    
    //GameManager::getThis()->setLoadedGame(true);
    //GameManager::getThis()->newGameData();.
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

}

void MainMenuScene::onNewGame(cocos2d::CCObject *pSender)
{
    /*
    PopupMenu::closeAllPopupMenu();
    AlertBox* alert = AlertBox::create();
    alert->useAsExclusivePopupMenu();
    alert->setDisplayText("Play the tutorial?");
    
    alert->addButton(0, "Yes", this, callfuncO_selector(MainMenuScene::onAcceptTutorial), pSender);
    alert->addButton(0, "No", this, callfuncO_selector(MainMenuScene::onRejectTutorial), pSender);
    */
    
    //onRejectTutorial(pSender);
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();

    CCDirector::sharedDirector()->replaceScene(SenarioChooseScene::scene());
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
    
}
void MainMenuScene::onButtonOptionsPressed(CCObject* pSender){
    //CCDirector::sharedDirector()->pushScene(OptionsScene::scene());
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
    
    if (screenSize.width < screenSize.height) //i.e portrait
    {
    
        backgroundDeco->setPosition(ccp(screenSize.width*0.5, screenSize.height * 0.1));
        backgroundDeco2->setPosition(ccp(screenSize.width*0.5, screenSize.height * 0.1));

        buttonStart->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.375));
        buttonOptions->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.225));
      //  buttonCredits->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.075));
    }
    else
    {
        backgroundDeco->setPosition(ccp(screenSize.width*0.5, screenSize.height * 0.5));
        backgroundDeco2->setPosition(ccp(screenSize.width*0.5, screenSize.height * 0.5));

        buttonStart->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.65));
        buttonOptions->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.45));
       // buttonCredits->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.275));
        
    }
}

void MainMenuScene::onOrientationChangedToPortrait(){
}

void MainMenuScene::onOrientationChangedToLandscape(){
}
