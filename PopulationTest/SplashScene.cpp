//
//  SplashScene.cpp
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#include "SplashScene.h"
#include "MainMenuScene.h"
#include "GameManager.h"

SplashScene* SplashScene::SP;


SplashScene::SplashScene()
{
    SplashScene::SP = this;
}

SplashScene::~SplashScene()
{
    SplashScene::SP = NULL;
}

CCScene* SplashScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SplashScene *layer = SplashScene::create();
    
    //People, please initialize properly! C++ does not do this for you, and NULL != 0. - Larry
    layer->transitionState = 0;
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool SplashScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    splashImage = CCSprite::create("HelloWorld.png");
    CCSize spriteSize = splashImage->getContentSize();
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    splashImage->setScaleX(screenSize.width / spriteSize.width);
    splashImage->setScaleY(screenSize.height / spriteSize.height);
    CCSize newSize = splashImage->boundingBox().size;
    splashImage->setPosition(ccp(newSize.width/2, newSize.height/2));
    this->addChild(splashImage, -1);
    
    this->initWithColor(ccc4(0, 0, 0, 255), screenSize.width, screenSize.height);
    this->schedule(schedule_selector(SplashScene::update), 1/60);
    
    new GameManager();
    
    return true;
}


void SplashScene::update(float dt){
    float alpha = this->getOpacity();
    switch (transitionState) {
        case 0:
            alpha -= 5;
            if (alpha <= 0) {
                transitionState++;
            }
            break;
        case 1:
            alpha += 5;
            if (alpha >= 255) {
                CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
            }
            break;
            
        default:
            break;
    }
    
    this->setOpacity(alpha);
}


