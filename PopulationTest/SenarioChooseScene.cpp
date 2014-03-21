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
    
    CCRect boxrect = CCRectMake(0,0, screenSize.width * 0.8f,  screenSize.width * 0.2f);
    CCRect exitrect = CCRectMake(0,0, screenSize.width * 0.15f, screenSize.width * 0.15f);
    
    
    backgroundImage = CCSprite::create("Level Select Screen.png");
    backgroundImage->setScale(screenSize.width/backgroundImage->boundingBox().size.width);
    
    senarioButtonTutorial = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseTutorial));
    senarioButtonTutorial->setContentSize(boxrect.size);

    senarioButtonS1 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario1));
    senarioButtonS1->setContentSize(boxrect.size);
    
    senarioButtonS2 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario2));
    senarioButtonS2->setContentSize(boxrect.size);
    
    senarioButtonS3 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario3));
    senarioButtonS3->setContentSize(boxrect.size);
    
    senarioButtonS4 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario4));
    senarioButtonS4->setContentSize(boxrect.size);
    
    senarioButtonS5 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario5));
    senarioButtonS5->setContentSize(boxrect.size);
    
    senarioButtonS6 = CCMenuItemImage::create("", "", this, menu_selector(SenarioChooseScene::chooseSenario6));
    senarioButtonS6->setContentSize(exitrect.size);
    
    CCLabelTTF* tutorialLabel = CCLabelTTF::create("Tutorial", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    // tutorialLabel->setAnchorPoint(ccp(0.5f, 0.5));
    tutorialLabel->setPosition( ccp(senarioButtonTutorial->boundingBox().size.width * 0.5f, senarioButtonTutorial->boundingBox().size.height * 0.5f));
    senarioButtonTutorial->addChild(tutorialLabel);
    tutorialLabel->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel1 = CCLabelTTF::create("Senario 1", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel1->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel1->setPosition( ccp(senarioButtonS1->boundingBox().size.width * 0.5f, senarioButtonS1->boundingBox().size.height * 0.5f));
    senarioButtonS1->addChild(senarioLabel1);
    senarioLabel1->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel2 = CCLabelTTF::create("Senario 2", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel2->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel2->setPosition( ccp(senarioButtonS2->boundingBox().size.width * 0.5f, senarioButtonS2->boundingBox().size.height * 0.5f));
    senarioButtonS2->addChild(senarioLabel2);
    senarioLabel2->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel3 = CCLabelTTF::create("Senario 3", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel3->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel3->setPosition( ccp(senarioButtonS3->boundingBox().size.width * 0.5f, senarioButtonS3->boundingBox().size.height * 0.5f));
    senarioButtonS3->addChild(senarioLabel3);
    senarioLabel3->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel4 = CCLabelTTF::create("Senario 4", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel4->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel4->setPosition( ccp(senarioButtonS4->boundingBox().size.width * 0.5f, senarioButtonS4->boundingBox().size.height * 0.5f));
    senarioButtonS4->addChild(senarioLabel4);
    senarioLabel4->setColor(ccc3(255,189,68));
    
    CCLabelTTF* senarioLabel5 = CCLabelTTF::create("Senario 5", "Shojumaru-Regular" ,96, boxrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel5->setAnchorPoint(ccp(0.5f, 0.5));
    senarioLabel5->setPosition( ccp(senarioButtonS5->boundingBox().size.width * 0.5f, senarioButtonS5->boundingBox().size.height * 0.5f));
    senarioButtonS5->addChild(senarioLabel5);
    senarioLabel5->setColor(ccc3(255,189,68));
    
   // CCLabelTTF* senarioLabel6 = CCLabelTTF::create("B", "Shojumaru-Regular" ,28, exitrect.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // senarioLabel6->setAnchorPoint(ccp(0.5f, 0.5));
  //  senarioLabel6->setPosition( ccp(senarioButtonS6->boundingBox().size.width * 0.5f, senarioButtonS6->boundingBox().size.height * 0.5f));
   // senarioButtonS6->addChild(senarioLabel6);
   // senarioLabel6->setColor(ccc3(255,189,68));

    
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
    
    backgroundImage->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.5));
    senarioButtonTutorial->setPosition(ccp(screenSize.width * 0.30, screenSize.height * 0.625));
    senarioButtonS1->setPosition(ccp(screenSize.width * 0.30, screenSize.height * 0.4));
    senarioButtonS2->setPosition(ccp(screenSize.width * 0.30, screenSize.height * 0.175));
    senarioButtonS3->setPosition(ccp(screenSize.width * 0.70, screenSize.height * 0.625));
    senarioButtonS4->setPosition(ccp(screenSize.width * 0.70, screenSize.height * 0.4));
    senarioButtonS5->setPosition(ccp(screenSize.width * 0.70, screenSize.height * 0.175));
    senarioButtonS6->setPosition(ccp(screenSize.width * 0.95, screenSize.height * 0.05));
    
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
    GameManager::getThis()->setLevel(1);
    
    CCDirector::sharedDirector()->pushScene(GameScene::scene());
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

//THIS IS ACTUALLY THE GO BACK BUTTON.
void SenarioChooseScene::chooseSenario6()
{
    CCLog ("Back pressed");
}

void SenarioChooseScene::onOrientateChange(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    senarioButtonTutorial->setPosition(ccp(screenSize.width * 0.25, screenSize.height * 0.6));
    senarioButtonS1->setPosition(ccp(screenSize.width * 0.25, screenSize.height * 0.4));
    senarioButtonS2->setPosition(ccp(screenSize.width * 0.25, screenSize.height * 0.2));
    senarioButtonS3->setPosition(ccp(screenSize.width * 0.75, screenSize.height * 0.6));
    senarioButtonS4->setPosition(ccp(screenSize.width * 0.75, screenSize.height * 0.4));
    senarioButtonS5->setPosition(ccp(screenSize.width * 0.75, screenSize.height * 0.2));
    senarioButtonS6->setPosition(ccp(screenSize.width * 0.85, screenSize.height * 0.1));
    
    
}

void SenarioChooseScene::onOrientationChangedToPortrait(){
}

void SenarioChooseScene::onOrientationChangedToLandscape(){
}