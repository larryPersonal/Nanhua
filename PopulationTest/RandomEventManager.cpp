//
//  RandomEventManager.cpp
//  PopulationTest
//
//  Created by admin on 8/7/14.
//
//

#include "RandomEventManager.h"

RandomEventManager* RandomEventManager::SP;

RandomEventManager::RandomEventManager()
{
    RandomEventManager::SP = this;
}

RandomEventManager::~RandomEventManager()
{
    RandomEventManager::SP = NULL;
}

RandomEventManager* RandomEventManager::getThis()
{
    return SP;
}

RandomEventManager* RandomEventManager::create()
{
    RandomEventManager* pRet = new RandomEventManager();
    if(pRet && pRet->init())
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

bool RandomEventManager::init()
{
    createUI();
    return true;
}

void RandomEventManager::createUI()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    blackScreen = CCSprite::create("blackscreen.png");
    blackScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    blackScreen->setVisible(false);
    this->addChild(blackScreen, 1);
    
    background = CCSprite::create("Building Info UI placeholder_dialog.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->setScale(1.5f);
    background->setPosition(ccp(-screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(background, 2);
}

void RandomEventManager::showUI()
{
    /*
    CCLog("test2");
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    blackScreen->setVisible(true);
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    */
}

void RandomEventManager::hideUI()
{
    /*
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    blackScreen->setVisible(false);
    background->setPosition(ccp(-screenSize.width / 2.0f, screenSize.height / 2.0f));
    */
}