//
//  PolicyPopup.cpp
//  PopulationTest
//
//  Created by Serious Games on 29/7/13.
//
//

#include "PolicyPopup.h"
#include "GameDefaults.h"
#include "Gamemanager.h"
#include "GlobalHelper.h"
#include "GameScene.h"
#include "GameHUD.h"

PolicyPopup::PolicyPopup()
{
    background = NULL;
    displayText = NULL;
    
    deltaAmtVal = 0;
    deltaDurationVal = 0;
    costVal = 0;
}

PolicyPopup::~PolicyPopup()
{
    
}

/*
 bool PolicyPopup::init()
 {
 if ( !CCLayer::init() )
 {
 return false;
 }
 return true;
 
 }*/


void PolicyPopup::cleanup()
{
    
    // this->removeAllChildren();
    // this->removeAllChildrenWithCleanup(true);
    this->removeChild(background);
    this->removeChild(displayText);
    this->removeChild(buttonMenu);
    this->removeChild(adjustMenu);
    // displayText->release();
    
    PopupMenu::cleanup();
    
}

PolicyPopup* PolicyPopup::create()
{
    PolicyPopup *pRet = new PolicyPopup();
    if (pRet && pRet->init())
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
void PolicyPopup::createMenuItems()
{
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(0);
    menuItemPositions->retain();
    
    
    background = CCSprite::create("outerbox.png");
    background->setScale(500.0f / background->getContentSize().height);
    
    backgroundOverlay = CCSprite::create("innerbox-policy.png");
    backgroundOverlay->setScale(background->getScale());
    
    displayText = CCLabelTTF::create("", "Droidiga", 30, CCSizeMake(background->boundingBox().size.width - 100.0f, 100.0f),
                                     kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    displayText->setAnchorPoint(ccp(0.5f, 1.0f));
    
    buttonMenu = CCMenu::createWithArray(menuItems);
    buttonMenu->setAnchorPoint(ccp(0.5f, 0.0f));
    
    adjustMenu = CCMenu::createWithArray(menuItems);
    adjustMenu->setAnchorPoint(ccp(0.5f, 0.0f));
   
    deltaValText =CCLabelTTF::create(("Rate +:\n" +GlobalHelper::Convert(deltaAmtVal)).c_str(), "Droidiga", 26, CCSizeMake(background->boundingBox().size.width - 100.0f, 100.0f),
                                     kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    deltaValText->setAnchorPoint(ccp(0.5f, 1.0f));
        
    deltaDurText = CCLabelTTF::create(("Duration:\n" + GlobalHelper::Convert(deltaDurationVal)).c_str(), "Droidiga", 26, CCSizeMake(background->boundingBox().size.width - 100.0f, 100.0f),
                                     kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    deltaDurText->setAnchorPoint(ccp(0.5f, 1.0f));
    
    costText = CCLabelTTF::create(("Cost to implement: " + GlobalHelper::Convert(costVal) +" G").c_str(), "Droidiga", 26, CCSizeMake(background->boundingBox().size.width - 100.0f, 100.0f),
                                   kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    costText->setAnchorPoint(ccp(0.5f, 1.0f));
    
    currValText = CCLabelTTF::create("", "Droidiga", 26, CCSizeMake(background->boundingBox().size.width - 100.0f, 100.0f),
                                     kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    currValText->setAnchorPoint(ccp(0.5f, 1.0f));
    
    
    this->addChild(background);
    this->addChild(backgroundOverlay);
    this->addChild(displayText);
    this->addChild(buttonMenu);
    this->addChild(adjustMenu);
    this->addChild(deltaDurText);
    this->addChild(deltaValText);
    this->addChild(costText);
    this->addChild(currValText);
    
    CCPoint labelPosition = displayText->getPosition();
    float yPosition = labelPosition.y;
    //Up arrow for increment
    CCSprite* upCImg =     CCSprite::create("arrow-up2.png");
    CCSprite* upCSelImg =   CCSprite::create("arrow-up.png");
    upCImg->               setScale(128 / (upCImg->boundingBox().size.height * 2));
     upCSelImg->             setScale(upCImg->getScale());
     CCMenuItemSprite* ceUpItem =    CCMenuItemSprite::create(upCImg, upCSelImg, this, menu_selector(PolicyPopup::onMenuItemSelected) );
     ceUpItem->  setAnchorPoint(ccp(0, 1));
     ceUpItem->  setContentSize(upCImg->boundingBox().size);
     ceUpItem->setPosition(ccp (labelPosition.x -(upCImg->boundingBox().size.height * 4), yPosition + upCImg->boundingBox().size.height * 0.25f));
     ceUpItem->setTag(2);
     
     //Down arrow for decrement
     CCSprite* downCImg =     CCSprite::create("arrow-down2.png");
     CCSprite* downCSelImg =   CCSprite::create("arrow-down.png");
     downCImg->               setScale(128 / (downCImg->boundingBox().size.height * 2));
     downCSelImg->             setScale(downCImg->getScale());
     CCMenuItemSprite* ceDownItem =    CCMenuItemSprite::create(downCImg, downCSelImg, this, menu_selector(PolicyPopup::onMenuItemSelected) );
     ceDownItem->  setAnchorPoint(ccp(0, 1));
     ceDownItem->  setContentSize(downCImg->boundingBox().size);
     ceDownItem->setPosition(ccp (labelPosition.x -(downCImg->boundingBox().size.height * 4), yPosition - downCImg->boundingBox().size.height * 0.75f));
     ceDownItem->setTag(3);
     
     //Up arrow for increment
     CCSprite* upDCImg =     CCSprite::create("arrow-up2.png");
     CCSprite* upDCSelImg =   CCSprite::create("arrow-up.png");
     upDCImg->               setScale(128 / (upDCImg->boundingBox().size.height * 2));
     upDCSelImg->             setScale(upDCImg->getScale());
     CCMenuItemSprite* ceDUpItem =    CCMenuItemSprite::create(upDCImg, upDCSelImg, this, menu_selector(PolicyPopup::onMenuItemSelected) );
     ceDUpItem->  setAnchorPoint(ccp(0, 1));
     ceDUpItem->  setContentSize(upDCImg->boundingBox().size);
     ceDUpItem->setPosition(ccp (0, yPosition + upDCImg->boundingBox().size.height * 0.25f));
     ceDUpItem->setTag(4);
     
     //Down arrow for decrement
     CCSprite* downDCImg =     CCSprite::create("arrow-down2.png");
     CCSprite* downDCSelImg =   CCSprite::create("arrow-down.png");
     downDCImg->               setScale(128 / (downDCImg->boundingBox().size.height * 2));
     downDCSelImg->             setScale(downDCImg->getScale());
     CCMenuItemSprite* ceDDownItem =    CCMenuItemSprite::create(downDCImg, downDCSelImg, this, menu_selector(PolicyPopup::onMenuItemSelected) );
     ceDDownItem->  setAnchorPoint(ccp(0, 1));
     ceDDownItem->  setContentSize(downDCImg->boundingBox().size);
     ceDDownItem->setPosition(ccp (0, yPosition - downDCImg->boundingBox().size.height * 0.75f));
     ceDDownItem->setTag(5);
    

    adjustMenu->addChild(ceUpItem);
    adjustMenu->addChild(ceDownItem);
    adjustMenu->addChild(ceDUpItem);
    adjustMenu->addChild(ceDDownItem);
    
    
    addButton(0, "Cancel");
    addButton(1, "Proceed");

    displayText->setColor(ccc3(81, 77, 2));
    
    deltaDurText->setColor(ccc3(111, 107, 50));
    deltaValText->setColor(ccc3(111, 107, 50));
    costText->setColor(ccc3(111, 107, 50));
    currValText->setColor(ccc3(111, 107, 50));
    
    
    reposition();
}

void PolicyPopup::willChangeOrientation()
{
}

void PolicyPopup::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    reposition();
}

void PolicyPopup::onMenuItemSelected(CCObject* pSender)
{
    CCMenuItem* item = (CCMenuItem*)pSender;
    
    if (item->getTag() == 0)
    {
        this->closeMenu(false);
    }
    else
    {
        switch (item->getTag())
        {
            case 2:
                if (menuMode == 0)
                {
                
                    if (deltaAmtVal + GameScene::getThis()->policyHandler->monthly_localBirthRate < max_birth_rate)
                        ++deltaAmtVal;
                }
                else
                {
                    if (deltaAmtVal + GameScene::getThis()->policyHandler->getCulturalExchangeRatio() < max_cultural_percent)
                        ++deltaAmtVal;
                    
                    
                }
                deltaValText->setString(("Rate +:\n" +GlobalHelper::Convert(deltaAmtVal)).c_str());
                currValText->setString(("Current Rate: " + GlobalHelper::Convert(amtVal) +" -> " +GlobalHelper::Convert(amtVal + deltaAmtVal)).c_str());
                updateCost();
                break;
            case 3:
                if (menuMode == 0)
                {
                    if (deltaAmtVal + GameScene::getThis()->policyHandler->monthly_localBirthRate > min_birth_rate)
                        --deltaAmtVal;
                }
                else
                {
                    if (deltaAmtVal + GameScene::getThis()->policyHandler->getCulturalExchangeRatio() > min_cultural_percent)
                        --deltaAmtVal;
                    
                }
                deltaValText->setString(("Rate +:\n" +GlobalHelper::Convert(deltaAmtVal)).c_str());
                currValText->setString(("Current Rate: " + GlobalHelper::Convert(amtVal) +" -> " +GlobalHelper::Convert(amtVal + deltaAmtVal)).c_str());
                updateCost();
                break;
            case 4:
                if (menuMode == 0)
                {
                    if (deltaDurationVal < max_birth_policy_duration_months)
                        ++deltaDurationVal;
                    
                }
                else
                {
                    if (deltaDurationVal < max_cultural_policy_duration_months)
                        ++deltaDurationVal;
                    
                }
                deltaDurText->setString(("Duration:\n" + GlobalHelper::Convert(deltaDurationVal)).c_str());
                currValText->setString(("Current Rate: " + GlobalHelper::Convert(amtVal) +"% -> " +GlobalHelper::Convert(amtVal + deltaAmtVal) +"%").c_str());
                updateCost();
                break;
            case 5:
                if (menuMode == 0)
                {
                    if (deltaDurationVal > min_birth_policy_duration_months && deltaDurationVal > 0)
                        --deltaDurationVal;
                }
                else
                {
                    if (deltaDurationVal > min_cultural_policy_duration_months && deltaDurationVal > 0)
                        --deltaDurationVal;
                    
                }
                deltaDurText->setString(("Duration:\n" + GlobalHelper::Convert(deltaDurationVal)).c_str());
                currValText->setString(("Current Rate: " + GlobalHelper::Convert(amtVal) +"% -> " +GlobalHelper::Convert(amtVal + deltaAmtVal) +"%").c_str());
                updateCost();
                break;
            default:
                if (deltaAmtVal != 0 && deltaDurationVal != 0)
                {
                    if (GameManager::getThis()->currentMoney >= costVal && costVal > 0)
                    {
                        GameManager::getThis()->currentMoney -= costVal;
                        GameHUD::getThis()->updateMoneyLabel();
                        
                        if (menuMode == 0)
                            GameScene::getThis()->policyHandler->setBirthRatePolicy(deltaAmtVal, deltaDurationVal);
                        else
                            GameScene::getThis()->policyHandler->setCulturalExchangePolicy(deltaAmtVal, deltaDurationVal);
                        
                        deltaAmtVal = 0;
                        deltaDurationVal = 0;
                        
                        this->closeMenu();
                        
                        
                    }
                    
                    
                }
                
                break;

        }
        
        
    }

    
}

void PolicyPopup::updateCost()
{
    if (menuMode == 0)
        costVal = abs(deltaAmtVal) * deltaDurationVal * cost_per_1_birth_rate_per_month;
    else
        costVal = abs(deltaAmtVal) * deltaDurationVal * cost_per_1_cultural_rate_per_month;
    
    costText->setString(("Cost to implement: " + GlobalHelper::Convert(costVal) +" G").c_str());
    //update the cost display
}

void PolicyPopup::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    background->CCNode::setPosition(screenSize.width / 2.0f, screenSize.height / 2.0f);
    backgroundOverlay->CCNode::setPosition(screenSize.width / 2.0f, screenSize.height / 2.0f);
    displayText->CCNode::setPosition(background->getPositionX(), background->boundingBox().getMaxY() - 15);
    buttonMenu->CCNode::setPosition(background->getPositionX(), background->boundingBox().getMinY() + 100);
   
    adjustMenu->CCNode::setPosition(background->getPositionX(), background->boundingBox().getMinY() + background->boundingBox().size.height * 0.5f);
   
        deltaDurText->CCNode::setPosition(adjustMenu->CCNode::getPositionX() + backgroundOverlay->boundingBox().size.width * 0.25f, adjustMenu->CCNode::getPositionY());
        deltaValText->CCNode::setPosition(adjustMenu->CCNode::getPositionX() - backgroundOverlay->boundingBox().size.width * 0.15f, adjustMenu->CCNode::getPositionY());

    costText->CCNode::setPosition(adjustMenu->CCNode::getPositionX(), background->boundingBox().getMinY() + background->boundingBox().size.height * 0.7f);
    currValText->CCNode::setPosition(adjustMenu->CCNode::getPositionX(), background->boundingBox().getMinY() + background->boundingBox().size.height * 0.825f);
    
    
}

void PolicyPopup::setDisplayText(string text)
{
    displayText->setString(text.c_str());
}

void PolicyPopup::addButton(int tag, string buttonText, CCObject* pSender)
{
    CCMenuItemImage* button = CCMenuItemImage::create("nav button.png", "nav button-pressed.png", this, menu_selector(PolicyPopup::onMenuItemSelected));
    
    button->setTag(tag);
    button->setScale(75.0f / button->getContentSize().height);
    /*
    if (selector != NULL)
    {
        callback = CCCallFuncO::create(pSelectorTarget, selector, pSender);
        callback->retain();
        button->setUserData(callback);
    }*/
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create(buttonText.c_str(), "Droidiga", 26.0f);
    buttonLabel->setScale(1.0f / button->getScale());
    buttonLabel->CCNode::setPosition(button->getContentSize().width / 2.0f, button->getContentSize().height / 2.0f);
    button->addChild(buttonLabel);
    
    buttonMenu->addChild(button);
    buttonMenu->alignItemsHorizontallyWithPadding(10.0f);
}

void PolicyPopup::removeButton(int tag)
{
    buttonMenu->removeChildByTag(tag);
}

void PolicyPopup::setupBirthPolicy()
{
    setDisplayText("Implement Birth Policy");
    amtVal =GameScene::getThis()->policyHandler->monthly_localBirthRate;
    std::string defaultrate = GlobalHelper::Convert(amtVal);
    currValText->setString(("Current Rate: " + defaultrate +" -> " + defaultrate).c_str());
    
    
    menuMode = 0;
}

void PolicyPopup::setupInteractPolicy()
{
    setDisplayText("Implement Social Interaction Policy");
    amtVal = GameScene::getThis()->policyHandler->getCulturalExchangeRatio();
    std::string defaultrate = GlobalHelper::Convert(amtVal);
    currValText->setString(("Current Rate: " + defaultrate +"% -> " + defaultrate +"%").c_str());

    
    menuMode = 1;
}