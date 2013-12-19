//
//  OptionsScene.cpp
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#include "OptionsScene.h"

OptionsScene* OptionsScene::SP;


OptionsScene::OptionsScene()
{
    OptionsScene::SP = this;
}

OptionsScene::~OptionsScene()
{
    OptionsScene::SP = NULL;
}

CCScene* OptionsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    OptionsScene *layer = OptionsScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool OptionsScene::init()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    buttonBack = CCMenuItemImage::create("Icon-Small-50.png", "Icon-72.png", this, menu_selector(OptionsScene::onButtonBackPressed));
    buttonBack->setPosition(ccp(screenSize.width*0.5, screenSize.height*0.5));
    
    CCMenu* menu = CCMenu::create(buttonBack, NULL);
    menu->setPosition( CCPointZero );
    this->addChild(menu, 1);
    
    return true;
}

void OptionsScene::onButtonBackPressed(CCObject* pSender){
    CCDirector::sharedDirector()->popScene();
}
