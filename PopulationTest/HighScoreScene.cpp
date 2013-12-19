//
//  HighScoreScene.cpp
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#include "HighScoreScene.h"

HighScoreScene* HighScoreScene::SP;


HighScoreScene::HighScoreScene()
{
    HighScoreScene::SP = this;
}

HighScoreScene::~HighScoreScene()
{
    HighScoreScene::SP = NULL;
}

CCScene* HighScoreScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HighScoreScene *layer = HighScoreScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool HighScoreScene::init()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    buttonBack = CCMenuItemImage::create("Icon-Small-50.png", "Icon-72.png", this, menu_selector(HighScoreScene::onButtonBackPressed));
    buttonBack->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.5));
    
    CCMenu* menu = CCMenu::create(buttonBack, NULL);
    menu->setPosition( CCPointZero );
    this->addChild(menu, 1);
    
    return true;
}

void HighScoreScene::onButtonBackPressed(CCObject* pSender){
    CCDirector::sharedDirector()->popScene();
}
