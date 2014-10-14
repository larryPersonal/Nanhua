//
//  AccountRow.cpp
//  PopulationTest
//
//  Created by admin on 7/7/14.
//
//

#include "AccountRow.h"
#include "MainMenuScene.h"
#include "SoundtrackManager.h"
#include "GlobalHelper.h"

AccountRow* AccountRow::create(ScrollArea* sa, int ind)
{
    AccountRow* pRet = new AccountRow(sa, ind);
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

AccountRow::AccountRow(ScrollArea* sa, int ind)
{
    userAccount = new UserAccount();
    scrollArea = sa;
    index = ind;
    constructAccountRow();
}

AccountRow::~AccountRow()
{
    
}

UserAccount* AccountRow::getUserAccount()
{
    return userAccount;
}

void AccountRow::constructAccountRow()
{
    float yOffset = 0.0f;
    float yGap = 110.0f;
    
    accountNameLabel = CCLabelTTF::create(userAccount->getAccountName().c_str(), "Shojumaru-Regular", 54);
    accountNameLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    
    scrollArea->addItem(accountNameLabel, ccp(350.0f, yOffset + 28.0f + yGap * index));
    
    resetPasswordButton = CCMenuItemImage::create("reset.png", "reset_press.png", this, menu_selector(AccountRow::clickResetPasswordButton));
    resetPasswordButton->setScale(1.5f);
    resetPasswordButton->setAnchorPoint(ccp(0.5f, 0.5f));
    
    resetPasswordButton->setPosition(ccp(0.0f, 0.0f));
    
    deleteAccountButton = CCMenuItemImage::create("delete.png", "delete_press.png", this, menu_selector(AccountRow::clickDeleteAccountButton));
    deleteAccountButton->setScale(1.5f);
    deleteAccountButton->setAnchorPoint(ccp(0.5f, 0.5f));
    
    deleteAccountButton->setPosition(ccp(150.0f, 0.0f));
    
    activateScenario1Button = CCMenuItemImage::create("scenario1_disable.png", "scenario1_press.png", this, menu_selector(AccountRow::clickActivateScenario1Button));
    activateScenario1Button->setScale(0.64f);
    activateScenario1Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    activateScenario1Button->setPosition(ccp(300.0f, 0.0f));
    
    deActivateScenario1Button = CCMenuItemImage::create("scenario1.png", "scenario1_press.png", this, menu_selector(AccountRow::clickDeactivateScenario1Button));
    deActivateScenario1Button->setScale(0.64f);
    deActivateScenario1Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    deActivateScenario1Button->setPosition(ccp(300.0f, 0.0f));
    
    activateScenario2Button = CCMenuItemImage::create("scenario2_disable.png", "scenario2_press.png", this, menu_selector(AccountRow::clickActivateScenario2Button));
    activateScenario2Button->setScale(0.64f);
    activateScenario2Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    activateScenario2Button->setPosition(ccp(450.0f, 0.0f));
    
    deActivateScenario2Button = CCMenuItemImage::create("scenario2.png", "scenario2_press.png", this, menu_selector(AccountRow::clickDeactivateScenario2Button));
    deActivateScenario2Button->setScale(0.64f);
    deActivateScenario2Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    deActivateScenario2Button->setPosition(ccp(450.0f, 0.0f));
    
    activateScenario3Button = CCMenuItemImage::create("scenario3_disable.png", "scenario3_press.png", this, menu_selector(AccountRow::clickActivateScenario3Button));
    activateScenario3Button->setScale(0.64f);
    activateScenario3Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    activateScenario3Button->setPosition(ccp(600.0f, 0.0f));
    
    deActivateScenario3Button = CCMenuItemImage::create("scenario3.png", "scenario3_press.png", this, menu_selector(AccountRow::clickDeactivateScenario3Button));
    deActivateScenario3Button->setScale(0.64f);
    deActivateScenario3Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    deActivateScenario3Button->setPosition(ccp(600.0f, 0.0f));
    
    activateScenario4Button = CCMenuItemImage::create("scenario4_disable.png", "scenario4_press.png", this, menu_selector(AccountRow::clickActivateScenario4Button));
    activateScenario4Button->setScale(0.64f);
    activateScenario4Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    activateScenario4Button->setPosition(ccp(750.0f, 0.0f));
    
    deActivateScenario4Button = CCMenuItemImage::create("scenario4.png", "scenario4_press.png", this, menu_selector(AccountRow::clickDeactivateScenario4Button));
    deActivateScenario4Button->setScale(0.64f);
    deActivateScenario4Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    deActivateScenario4Button->setPosition(ccp(750.0f, 0.0f));
    
    activateScenario5Button = CCMenuItemImage::create("scenario5_disable.png", "scenario5_press.png", this, menu_selector(AccountRow::clickActivateScenario5Button));
    activateScenario5Button->setScale(0.64f);
    activateScenario5Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    activateScenario5Button->setPosition(ccp(900.0f, 0.0f));
    
    deActivateScenario5Button = CCMenuItemImage::create("scenario5.png", "scenario5_press.png", this, menu_selector(AccountRow::clickDeactivateScenario5Button));
    deActivateScenario5Button->setScale(0.64f);
    deActivateScenario5Button->setAnchorPoint(ccp(0.5f, 0.5f));
    
    deActivateScenario5Button->setPosition(ccp(900.0f, 0.0f));
    
    refresh();
    
    accountRowMenu = CCMenu::create(resetPasswordButton, deleteAccountButton, activateScenario1Button, deActivateScenario1Button, activateScenario2Button, deActivateScenario2Button, activateScenario3Button, deActivateScenario3Button, activateScenario4Button, deActivateScenario4Button, activateScenario5Button, deActivateScenario5Button, NULL);
    
    scrollArea->addItem(accountRowMenu, ccp(900.0f, yOffset + 64.0f + yGap * index));
}

void AccountRow::refresh()
{
    accountNameLabel->setString(userAccount->getAccountName().c_str());
    
    if(userAccount->getScenario1Status())
    {
        activateScenario1Button->setVisible(false);
        deActivateScenario1Button->setVisible(true);
    }
    else
    {
        activateScenario1Button->setVisible(true);
        deActivateScenario1Button->setVisible(false);
    }
    
    if(userAccount->getScenario2Status())
    {
        activateScenario2Button->setVisible(false);
        deActivateScenario2Button->setVisible(true);
    }
    else
    {
        activateScenario2Button->setVisible(true);
        deActivateScenario2Button->setVisible(false);
    }
    
    if(userAccount->getScenario3Status())
    {
        activateScenario3Button->setVisible(false);
        deActivateScenario3Button->setVisible(true);
    }
    else
    {
        activateScenario3Button->setVisible(true);
        deActivateScenario3Button->setVisible(false);
    }
    
    if(userAccount->getScenario4Status())
    {
        activateScenario4Button->setVisible(false);
        deActivateScenario4Button->setVisible(true);
    }
    else
    {
        activateScenario4Button->setVisible(true);
        deActivateScenario4Button->setVisible(false);
    }
    
    if(userAccount->getScenario5Status())
    {
        activateScenario5Button->setVisible(false);
        deActivateScenario5Button->setVisible(true);
    }
    else
    {
        activateScenario5Button->setVisible(true);
        deActivateScenario5Button->setVisible(false);
    }
}

void AccountRow::clear()
{
    ((CCMenuItemSprite*) resetPasswordButton)->clear();
    ((CCMenuItemSprite*) deleteAccountButton)->clear();
    ((CCMenuItemSprite*) activateScenario1Button)->clear();
    ((CCMenuItemSprite*) deActivateScenario1Button)->clear();
    ((CCMenuItemSprite*) activateScenario2Button)->clear();
    ((CCMenuItemSprite*) deActivateScenario2Button)->clear();
    ((CCMenuItemSprite*) activateScenario3Button)->clear();
    ((CCMenuItemSprite*) deActivateScenario3Button)->clear();
    ((CCMenuItemSprite*) activateScenario4Button)->clear();
    ((CCMenuItemSprite*) deActivateScenario4Button)->clear();
    ((CCMenuItemSprite*) activateScenario5Button)->clear();
    ((CCMenuItemSprite*) deActivateScenario5Button)->clear();
    
    resetPasswordButton->removeFromParentAndCleanup(true);
    deleteAccountButton->removeFromParentAndCleanup(true);
    activateScenario1Button->removeFromParentAndCleanup(true);
    deActivateScenario1Button->removeFromParentAndCleanup(true);
    activateScenario2Button->removeFromParentAndCleanup(true);
    deActivateScenario2Button->removeFromParentAndCleanup(true);
    activateScenario3Button->removeFromParentAndCleanup(true);
    deActivateScenario3Button->removeFromParentAndCleanup(true);
    activateScenario4Button->removeFromParentAndCleanup(true);
    deActivateScenario4Button->removeFromParentAndCleanup(true);
    activateScenario5Button->removeFromParentAndCleanup(true);
    deActivateScenario5Button->removeFromParentAndCleanup(true);
    
    scrollArea->removeChild(accountNameLabel, true);
    scrollArea->removeChild(accountRowMenu, true);
    
    delete userAccount;
    scrollArea = NULL;
    
}

void AccountRow::clickResetPasswordButton()
{
    SoundtrackManager::PlaySFX("Button_press.wav");
    CCUserDefault::sharedUserDefault()->setStringForKey("currentWarningUsername", userAccount->getAccountName().c_str());
    MainMenuScene::getThis()->openResetPasswordConfirmScreen(userAccount->getAccountName());
}

void AccountRow::clickDeleteAccountButton()
{
    std::stringstream ss;
    
    int userNumber = CCUserDefault::sharedUserDefault()->getIntegerForKey("userNumber");
    for (int i = index; i < userNumber - 1; i++)
    {
        ss.str(std::string());
        ss << "user_" << (i + 1);
        std::string tempName = CCUserDefault::sharedUserDefault()->getStringForKey(ss.str().c_str());
        
        ss.str(std::string());
        ss << "user_" << i;
        CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), tempName);
    }
    
    if(userNumber > 0)
    {
        userNumber--;
        CCUserDefault::sharedUserDefault()->setIntegerForKey("userNumber", userNumber);
    }
    
    GlobalHelper::cleanSaveData(userAccount->getAccountName());
    
    MainMenuScene::getThis()->deleteAccountRow(index);
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickActivateScenario1Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    userAccount->setScenario1Status(true);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickDeactivateScenario1Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_3_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_4_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_5_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    userAccount->setScenario1Status(false);
    userAccount->setScenario2Status(false);
    userAccount->setScenario3Status(false);
    userAccount->setScenario4Status(false);
    userAccount->setScenario5Status(false);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickActivateScenario2Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    userAccount->setScenario1Status(true);
    userAccount->setScenario2Status(true);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickDeactivateScenario2Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_3_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_4_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    userAccount->setScenario2Status(false);
    userAccount->setScenario3Status(false);
    userAccount->setScenario4Status(false);
    userAccount->setScenario5Status(false);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickActivateScenario3Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_3_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    userAccount->setScenario1Status(true);
    userAccount->setScenario2Status(true);
    userAccount->setScenario3Status(true);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickDeactivateScenario3Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_3_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    userAccount->setScenario3Status(false);
    userAccount->setScenario4Status(false);
    userAccount->setScenario5Status(false);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickActivateScenario4Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_3_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_4_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    userAccount->setScenario1Status(true);
    userAccount->setScenario2Status(true);
    userAccount->setScenario3Status(true);
    userAccount->setScenario4Status(true);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickDeactivateScenario4Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    userAccount->setScenario4Status(false);
    userAccount->setScenario5Status(false);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickActivateScenario5Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_2_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_3_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_4_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_5_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
    
    userAccount->setScenario1Status(true);
    userAccount->setScenario2Status(true);
    userAccount->setScenario3Status(true);
    userAccount->setScenario4Status(true);
    userAccount->setScenario5Status(true);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}

void AccountRow::clickDeactivateScenario5Button()
{
    std::stringstream ss;
    ss.str(std::string());
    ss << userAccount->getAccountName() << "_level_1_open";
    
    CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), false);
    
    userAccount->setScenario5Status(false);
    
    refresh();
    
    SoundtrackManager::PlaySFX("Button_press.wav");
}
