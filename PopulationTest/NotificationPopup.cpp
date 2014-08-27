//
//  NotificationPopup.cpp
//  PopulationTest
//
//  Created by admin on 22/8/14.
//
//

#include "NotificationPopup.h"
#include "GameHUD.h"

NotificationPopup* NotificationPopup::SP;

NotificationPopup::NotificationPopup()
{
    NotificationPopup::SP = this;
    
    background = NULL;
    active = false;
    
    isJumpIn = false;
    isJumpOut = false;
    
    congratsTitleLabel_scenario3 = NULL;
    congratsContentLabel_scenario3 = NULL;
    
    targetScale = 0;
}

NotificationPopup::~NotificationPopup()
{
    NotificationPopup::SP = NULL;
}

NotificationPopup* NotificationPopup::getThis()
{
    return SP;
}

NotificationPopup* NotificationPopup::create()
{
    NotificationPopup* pRet = new NotificationPopup();
    if(pRet)
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

void NotificationPopup::showUI()
{
    createUI();
    
    active = true;
    
    scheduleJumpIn();
}

void NotificationPopup::hideUI()
{
    active = false;
    
    scheduleJumpOut();
}

void NotificationPopup::scheduleJumpIn()
{
    blackScreen->setOpacity((GLubyte) 0);
    background->setScale(0);
    
    this->schedule(schedule_selector(NotificationPopup::jumpIn), 1.0f/120.0f);
}

void NotificationPopup::jumpIn(float dt)
{
    float scale = background->getScale();
    
    scale = scale * 1.1f + 0.01f;
    
    if(scale >= targetScale)
    {
        scale = targetScale;
        isJumpIn = false;
        isJumpOut = false;
        this->unschedule(schedule_selector(NotificationPopup::jumpIn));
    }
    
    float opacity = scale / targetScale * 160.0f;
    blackScreen->setOpacity((GLubyte) opacity);
    
    background->setScale(scale);
}

void NotificationPopup::scheduleJumpOut()
{
    blackScreen->setOpacity((GLubyte) 160);
    background->setScale(targetScale);
    
    this->schedule(schedule_selector(NotificationPopup::jumpOut), 1.0f / 120.0f);
}

void NotificationPopup::jumpOut(float dt)
{
    float scale = background->getScale();
    
    scale = scale / 1.1f - 0.01f;
    if(scale <= 0)
    {
        scale = 0;
        isJumpIn = false;
        isJumpOut = false;
        this->unschedule(schedule_selector(NotificationPopup::jumpOut));
        
        blackScreen->setOpacity((GLubyte) 0);
        background->setScale(0);
        
        // remove Notification elements here!
        clearUI();
        
        if(proceedToNextLevel)
        {
            proceedToNextLevel = false;
            GameHUD::getThis()->scheduleOnce(schedule_selector(GameHUD::clickScoreButton), 5.0f);
        }
    }
    else
    {
        float opacity = scale / targetScale * 160.0f;
        blackScreen->setOpacity((GLubyte) opacity);
        
        background->setScale(scale);
    }
}

void NotificationPopup::createUI()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    blackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    blackScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(blackScreen, 1);
    
    background = CCSprite::createWithSpriteFrameName("popup_dialogbox.png");
    
    targetScale = screenSize.width / background->boundingBox().size.width * 0.75f;
    
    background->setScale(targetScale);
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(background, 2);
    
    /*
    cancelButton = CCSprite::createWithSpriteFrameName("Closebtn_Sq.png");
    cancelButton->setScale(1.4f);
    cancelButton->setAnchorPoint(ccp(0.5f, 0.5f));
    cancelButton->setPosition(ccp(screenSize.width - 150.0f, screenSize.height / 2.0f));
    background->addChild(cancelButton);
    */
    // cancelButton->setPosition(ccp(screenSize.width / 2.0f + 200.0f, screenSize.height / 2.0f));
    // this->addChild(cancelButton, 3);
}

void NotificationPopup::clearUI()
{
    // background->removeChild(cancelButton, true);
    this->removeChild(background, true);
    this->removeChild(blackScreen, true);
    
    this->unschedule(schedule_selector(NotificationPopup::jumpIn));
    this->unschedule(schedule_selector(NotificationPopup::jumpOut));
    
    background = NULL;
    blackScreen = NULL;
    
    if(congratsTitleLabel_scenario3 != NULL)
    {
        this->removeChild(congratsTitleLabel_scenario3, true);
        congratsTitleLabel_scenario3 = NULL;
    }
    
    if(congratsContentLabel_scenario3 != NULL)
    {
        this->removeChild(congratsContentLabel_scenario3, true);
        congratsContentLabel_scenario3 = NULL;
    }
}

void NotificationPopup::clickNotificationButton()
{
    if(!isJumpIn && !isJumpOut)
    {
        if(active)
        {
            isJumpOut = true;
            isJumpIn = false;
            hideUI();
        }
        else
        {
            isJumpIn = true;
            isJumpOut = false;
            showUI();
        }
    }
}

void NotificationPopup::showScenario3Congratulations()
{
    if(!isJumpIn && !isJumpOut)
    {
        if(!active)
        {
            isJumpIn = true;
            isJumpOut = false;
            proceedToNextLevel = true;
            
            showUI();
            
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            
            congratsTitleLabel_scenario3 = CCLabelTTF::create("Congratulations!", "GillSansMT", 48);
            congratsTitleLabel_scenario3->setAnchorPoint(ccp(0.5f, 1.0f));
            congratsTitleLabel_scenario3->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 100.0f));
            background->addChild(congratsTitleLabel_scenario3, 3);
            
            congratsContentLabel_scenario3 = CCLabelTTF::create("You have built 2 hospitals!", "GillSansMT", 48);
            congratsContentLabel_scenario3->setAnchorPoint(ccp(0.5f, 1.0f));
            congratsContentLabel_scenario3->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 200.0f));
            background->addChild(congratsContentLabel_scenario3, 3);
        }
    }
}

void NotificationPopup::showScenario4Congratulations()
{
    if(!isJumpIn && !isJumpOut)
    {
        if(!active)
        {
            isJumpIn = true;
            isJumpOut = false;
            proceedToNextLevel = true;
            
            showUI();
            
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            
            congratsTitleLabel_scenario3 = CCLabelTTF::create("Congratulations!", "GillSansMT", 48);
            congratsTitleLabel_scenario3->setAnchorPoint(ccp(0.5f, 1.0f));
            congratsTitleLabel_scenario3->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 100.0f));
            background->addChild(congratsTitleLabel_scenario3, 3);
            
            congratsContentLabel_scenario3 = CCLabelTTF::create("Your village has raised 10 population!", "GillSansMT", 48);
            congratsContentLabel_scenario3->setAnchorPoint(ccp(0.5f, 1.0f));
            congratsContentLabel_scenario3->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 200.0f));
            background->addChild(congratsContentLabel_scenario3, 3);
        }
    }
}

void NotificationPopup::showScenario5Congratulations()
{
    if(!isJumpIn && !isJumpOut)
    {
        if(!active)
        {
            isJumpIn = true;
            isJumpOut = false;
            proceedToNextLevel = true;
            
            showUI();
            
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            
            congratsTitleLabel_scenario3 = CCLabelTTF::create("Congratulations!", "GillSansMT", 48);
            congratsTitleLabel_scenario3->setAnchorPoint(ccp(0.5f, 1.0f));
            congratsTitleLabel_scenario3->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 100.0f));
            background->addChild(congratsTitleLabel_scenario3, 3);
            
            congratsContentLabel_scenario3 = CCLabelTTF::create("Your have harvested 1500 food for storage!", "GillSansMT", 48);
            congratsContentLabel_scenario3->setAnchorPoint(ccp(0.5f, 1.0f));
            congratsContentLabel_scenario3->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 200.0f));
            background->addChild(congratsContentLabel_scenario3, 3);
        }
    }
}
