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
    
    background = CCSprite::create("Building Info UI placeholder_dialog.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    background->setVisible(false);
    
    this->addChild(background, 2);
}

void RandomEventManager::showUI()
{
    background->setVisible(true);
}

void RandomEventManager::hideUI()
{
    background->setVisible(false);
}