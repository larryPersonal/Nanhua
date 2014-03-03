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

using namespace cocos2d;

SenarioChooseScene* SenarioChooseScene::SP;


SenarioChooseScene::SenarioChooseScene()
{
    SenarioChooseScene::SP = this;
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
    
    backgroundImage = CCSprite::create("bkg.png");
    backgroundImage->setScale(screenSize.width/backgroundImage->boundingBox().size.width);
    
    senarioButtonTutorial = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseTutorial));
    senarioButtonS1 = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseSenario1));
    senarioButtonS2 = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseSenario2));
    senarioButtonS3 = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseSenario3));
    senarioButtonS4 = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseSenario4));
    senarioButtonS5 = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseSenario5));
    senarioButtonS6 = CCMenuItemImage::create("mainmenu-button.png", "mainmenu-button-pressed.png", this, menu_selector(SenarioChooseScene::chooseSenario6));
    
    CCLabelTTF* tutorialLabel = CCLabelTTF::create("Tutorial", "MBBlockType" ,96, senarioButtonTutorial->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // tutorialLabel->setAnchorPoint(ccp(0.5f, 0.5));
    tutorialLabel->setPosition( ccp(senarioButtonTutorial->boundingBox().size.width * 0.5f, senarioButtonTutorial->boundingBox().size.height * 0.5f));
    senarioButtonTutorial->addChild(tutorialLabel);
    tutorialLabel->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel1 = CCLabelTTF::create("Senario 1", "MBBlockType" ,96, senarioButtonS1->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel1->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel1->setPosition( ccp(senarioButtonS1->boundingBox().size.width * 0.5f, senarioButtonS1->boundingBox().size.height * 0.5f));
    senarioButtonS1->addChild(senarioLabel1);
    senarioLabel1->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel2 = CCLabelTTF::create("Senario 2", "MBBlockType" ,96, senarioButtonS2->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel2->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel2->setPosition( ccp(senarioButtonS2->boundingBox().size.width * 0.5f, senarioButtonS2->boundingBox().size.height * 0.5f));
    senarioButtonS2->addChild(senarioLabel2);
    senarioLabel2->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel3 = CCLabelTTF::create("Senario 3", "MBBlockType" ,96, senarioButtonS3->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel3->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel3->setPosition( ccp(senarioButtonS3->boundingBox().size.width * 0.5f, senarioButtonS3->boundingBox().size.height * 0.5f));
    senarioButtonS3->addChild(senarioLabel3);
    senarioLabel3->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel4 = CCLabelTTF::create("Senario 4", "MBBlockType" ,96, senarioButtonS4->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel4->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel4->setPosition( ccp(senarioButtonS4->boundingBox().size.width * 0.5f, senarioButtonS4->boundingBox().size.height * 0.5f));
    senarioButtonS4->addChild(senarioLabel4);
    senarioLabel4->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel5 = CCLabelTTF::create("Senario 5", "MBBlockType" ,96, senarioButtonS5->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel5->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel5->setPosition( ccp(senarioButtonS5->boundingBox().size.width * 0.5f, senarioButtonS5->boundingBox().size.height * 0.5f));
    senarioButtonS5->addChild(senarioLabel5);
    senarioLabel5->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel6 = CCLabelTTF::create("Senario 6", "MBBlockType" ,96, senarioButtonS6->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel6->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel6->setPosition( ccp(senarioButtonS6->boundingBox().size.width * 0.5f, senarioButtonS6->boundingBox().size.height * 0.5f));
    senarioButtonS6->addChild(senarioLabel6);
    senarioLabel6->setColor(ccc3(255,189,68));

    
    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        backgroundImage->setScale(0.5);
        senarioButtonTutorial->setScale(0.4);
        senarioButtonS1->setScale(0.4);
        senarioButtonS2->setScale(0.4);
        senarioButtonS3->setScale(0.4);
        senarioButtonS4->setScale(0.4);
        senarioButtonS5->setScale(0.4);
        senarioButtonS6->setScale(0.4);
    } else {
        senarioButtonTutorial->setScale(0.8);
        senarioButtonS1->setScale(0.8);
        senarioButtonS2->setScale(0.8);
        senarioButtonS3->setScale(0.8);
        senarioButtonS4->setScale(0.8);
        senarioButtonS5->setScale(0.8);
        senarioButtonS6->setScale(0.8);
    }
    //onOrientateChange();
    
    backgroundImage->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.25));
    senarioButtonTutorial->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.86));
    senarioButtonS1->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.74));
    senarioButtonS2->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.62));
    senarioButtonS3->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.50));
    senarioButtonS4->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.38));
    senarioButtonS5->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.26));
    senarioButtonS6->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.14));
    
    CCMenu* menu = CCMenu::create(senarioButtonTutorial, senarioButtonS1, senarioButtonS2, senarioButtonS3, senarioButtonS4, senarioButtonS5, senarioButtonS6, NULL);
    menu->setPosition( CCPointZero );
    this->addChild(menu, 1);
    this->addChild(backgroundImage, 0);
    
    return true;
}

void SenarioChooseScene::chooseTutorial()
{
    // need to parse the specific senario file (xml file) and construct the senario manager to play the senario
    
    // update the level number in the Game Manager
    GameManager::getThis()->setLevel(0);
    
    // load the actual game scene
    CCDirector::sharedDirector()->pushScene(GameScene::scene());
}

void SenarioChooseScene::chooseSenario1()
{
    
}

void SenarioChooseScene::chooseSenario2()
{
    
}

void SenarioChooseScene::chooseSenario3()
{
    
}

void SenarioChooseScene::chooseSenario4()
{
    
}

void SenarioChooseScene::chooseSenario5()
{
    
}

void SenarioChooseScene::chooseSenario6()
{
    
}

void SenarioChooseScene::onOrientateChange(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    backgroundImage->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.25));
    senarioButtonTutorial->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.86));
    senarioButtonS1->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.74));
    senarioButtonS2->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.62));
    senarioButtonS3->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.50));
    senarioButtonS4->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.38));
    senarioButtonS5->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.26));
    senarioButtonS6->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.14));
    
    
}

void SenarioChooseScene::onOrientationChangedToPortrait(){
}

void SenarioChooseScene::onOrientationChangedToLandscape(){
}