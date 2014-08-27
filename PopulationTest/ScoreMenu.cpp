//
//  ScoreMenu.cpp
//  PopulationTest
//
//  Created by admin on 28/5/14.
//
//

#include "ScoreMenu.h"
#include "GameHUD.h"
#include "GameManager.h"
#include "Senario.h"
#include "GlobalHelper.h"
#include "SanGuoXiaoXueTang.h"
#include "UIButtonControl.h"

ScoreMenu* ScoreMenu::SP;

ScoreMenu* ScoreMenu::create(CCLayer* layer)
{
    ScoreMenu* pRet = new ScoreMenu(layer);
    if(pRet && pRet->init(layer))
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

ScoreMenu* ScoreMenu::getThis()
{
    return SP;
}

ScoreMenu::ScoreMenu(CCLayer* layer)
{
    ScoreMenu::SP = this;
    
    show = false;
    hide = false;
}

ScoreMenu::~ScoreMenu()
{
    ScoreMenu::SP = NULL;
}

bool ScoreMenu::init(CCLayer* layer)
{
    preloadTextures();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    
    blackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    blackScreen->setOpacity((GLubyte) 0);
    layer->addChild(blackScreen, 9);
    
    background = CCSprite::createWithSpriteFrameName("achievementscreen.png");
    background->setAnchorPoint(ccp(0.5, 0.5));
    background->setScale(0);
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    layer->addChild(background, 10);
    
    stringstream ss;
    
    int level = GameManager::getThis()->getLevel();
    if(level == 0)
    {
        ss << "Tutorial";
    }
    else
    {
        ss << "Scenario: " << level;
    }
    
    scenarioTitle = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 52);
    scenarioTitle->setAnchorPoint(ccp(0.5f, 1.0f));
    scenarioTitle->setColor(colorBlack);
    background->addChild(scenarioTitle);
    scenarioTitle->setPosition(ccp(screenSize.width, screenSize.height / 2.0f * 3.0f - 150.0f));
    
    ss.str(std::string());
    ss << "Player: " << GameManager::getThis()->username.c_str();
    userLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 44);
    userLabel->setAnchorPoint(ccp(0.5f, 1.0f));
    userLabel->setColor(colorBlack);
    background->addChild(userLabel);
    userLabel->setPosition(ccp(screenSize.width, screenSize.height / 2.0f * 3.0f - 250.0f));
    
    renToken = CCSprite::createWithSpriteFrameName("ren_icon.png");
    renToken->setAnchorPoint(ccp(0, 0.5));
    renToken->setScale(1.0f);
    background->addChild(renToken);
    renToken->setPosition(ccp(screenSize.width - 150.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->targetReputation;
    
    float total = 0;
    for (int i = 0; i < GameScene::getThis()->spriteHandler->spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) GameScene::getThis()->spriteHandler->spritesOnMap->objectAtIndex(i);
        if(gs->villagerClass == V_SOLDIER)
        {
            total += 100.0f;
        }
    }
    
    if(GameManager::getThis()->getLevel() == 2)
    {
        ss << " + " << total;
    }
    
    renTokenLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 44);
    renTokenLabel->setAnchorPoint(ccp(0.5, 0.5));
    renTokenLabel->setColor(colorBlack);
    background->addChild(renTokenLabel);
    renTokenLabel->setPosition(ccp(screenSize.width + 100.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    money = CCSprite::createWithSpriteFrameName("money_label.png");
    money->setAnchorPoint(ccp(0, 0.5));
    money->setScale(0.5f);
    background->addChild(money);
    money->setPosition(ccp(screenSize.width / 2.0f + 140.0f, screenSize.height / 2.0f * 3.0f - 500.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->targetMoney;
    
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 32);
    moneyLabel->setAnchorPoint(ccp(0, 0.5));
    moneyLabel->setColor(colorBlack);
    background->addChild(moneyLabel);
    moneyLabel->setPosition(ccp(screenSize.width / 2.0f + 250.0f, screenSize.height / 2.0f * 3.0f - 500.0f));
    
    food = CCSprite::createWithSpriteFrameName("food_label.png");
    food->setAnchorPoint(ccp(0, 0.5));
    food->setScale(0.5f);
    background->addChild(food);
    food->setPosition(ccp(screenSize.width * 2.0f / 2.0f - 90.0f, screenSize.height / 2.0f * 3.0f - 500.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->targetFood << "/" << GameHUD::getThis()->targetStorage;
    
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 32);
    foodLabel->setAnchorPoint(ccp(0, 0.5));
    foodLabel->setColor(colorBlack);
    background->addChild(foodLabel);
    foodLabel->setPosition(ccp(screenSize.width * 2.0f / 2.0f + 0.0f, screenSize.height / 2.0f * 3.0f - 500.0f));
    
    population = CCSprite::createWithSpriteFrameName("population_icon.png");
    population->setAnchorPoint(ccp(0, 0.5));
    population->setScale(0.5f);
    background->addChild(population);
    population->setPosition(ccp(screenSize.width * 3.0f / 2.0f - 350.0f, screenSize.height / 2.0f * 3.0f - 510.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->populationLabel->getString();
    
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 32);
    populationLabel->setAnchorPoint(ccp(0, 0.5));
    populationLabel->setColor(colorBlack);
    background->addChild(populationLabel);
    populationLabel->setPosition(ccp(screenSize.width * 3.0f / 2.0f - 250.0f, screenSize.height / 2.0f * 3.0f - 500.0f));
    
    ss.str(std::string());
    ss << "Congratulations for your good result!";
    congratsLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 40);
    congratsLabel->setAnchorPoint(ccp(0.5, 1));
    congratsLabel->setColor(colorBlack);
    background->addChild(congratsLabel);
    congratsLabel->setPosition(ccp(screenSize.width, screenSize.height / 2.0f - 10.0f));
    
    CCSprite* button = CCSprite::createWithSpriteFrameName("accept.png");
    CCSprite* buttonPressed = CCSprite::createWithSpriteFrameName("accept_press.png");
    scoreMenuButton = cocos2d::CCMenuItemSprite::create(button, buttonPressed, this, menu_selector( ScoreMenu::clickScoreMenuButton ));
    scoreMenuButton->setScale(1.5f);
    scoreMenuButton->setAnchorPoint(ccp(0.5f, 0.5f));
    scoreMenuButton->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 250.0f));
    
    noticeLabel = CCLabelTTF::create("Re-launch the game to play the next level!", "GillSansMT", 32);
    noticeLabel->setColor(colorBlack);
    noticeLabel->setAnchorPoint(ccp(0.5, 1));
    noticeLabel->setPosition(ccp(screenSize.width, screenSize.height / 2.0f + 200.0f));
    background->addChild(noticeLabel);
    noticeLabel->setVisible(false);
    
    CCArray* menuItems = CCArray::create();
    menuItems->retain();
    
    menuItems->addObject(scoreMenuButton);
    
    menu = CCMenu::createWithArray(menuItems);
    
    background->addChild(menu);
    
    menuItems->removeAllObjects();
    CC_SAFE_RELEASE(menuItems);
    
    return true;
}

void ScoreMenu::releaseAll()
{
    GameHUD::getThis()->unschedule(schedule_selector(ScoreMenu::showScoreMenu));
    GameHUD::getThis()->unschedule(schedule_selector(ScoreMenu::hideScoreMenu));
    
    GameHUD::getThis()->removeChild(blackScreen, true);
    GameHUD::getThis()->removeChild(background, true);
    
    clearTextures();
    
    CC_SAFE_RELEASE(this);
}

void ScoreMenu::scheduleShowScoreMenu()
{
    if(!show && !hide)
    {
        show = true;
        UIButtonControl::pauseGame();
        GameHUD::getThis()->schedule(schedule_selector(ScoreMenu::showScoreMenu), 1.0f/120.0f);
    }
}

void ScoreMenu::scheduleHideScoreMenu()
{
    if(!show && !hide)
    {
        hide = true;
        GameHUD::getThis()->schedule(schedule_selector(ScoreMenu::hideScoreMenu), 1.0f/120.0f);
    }
}

void ScoreMenu::showScoreMenu(float dt)
{
    float scale = ScoreMenu::getThis()->background->getScale();
    scale += 0.1f * 0.5f;
    if(scale >= 0.5f)
    {
        scale = 0.5f;
        ScoreMenu::getThis()->show = false;
        GameHUD::getThis()->unschedule(schedule_selector(ScoreMenu::showScoreMenu));
    }
    
    float opacity = 255.0f * (scale / 0.5f);
    
    ScoreMenu::getThis()->blackScreen->setOpacity((GLubyte) opacity);
    ScoreMenu::getThis()->background->setScale(scale);
}

void ScoreMenu::hideScoreMenu(float dt)
{
    float scale = ScoreMenu::getThis()->background->getScale();
    scale -= 0.1f * 0.5f;
    if(scale <= 0)
    {
        scale = 0;
        ScoreMenu::getThis()->hide = false;
        GameHUD::getThis()->unschedule(schedule_selector(ScoreMenu::hideScoreMenu));
        ScoreMenu::getThis()->removeScoreMenu();
        UIButtonControl::resumeGame();
    }
    else
    {
        float opacity = 255.0f * (scale / 0.5f);
        
        ScoreMenu::getThis()->blackScreen->setOpacity((GLubyte) opacity);
        ScoreMenu::getThis()->background->setScale(scale);
    }
}

void ScoreMenu::removeScoreMenu()
{
    releaseAll();
}

void ScoreMenu::clickScoreMenuButton()
{
    string username = GameManager::getThis()->username;
    stringstream ss;
    
    GlobalHelper::resumeAllVillagers();
    GameHUD::getThis()->clearAllLabels();
    if(GameManager::getThis()->getLevel() == 0)
    {
        int reputation = GameHUD::getThis()->targetReputation;
        ss.str(std::string());
        ss << username << "_level_0_score";
        float prevReputation = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        if(prevReputation < reputation)
        {
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), (float) reputation);
        }
        
        ss.str(std::string());
        ss << username << "_level_1_open";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
        
        GameManager::getThis()->setLevel(1);
        
        scheduleHideScoreMenu();
        
        GameScene::getThis()->reSetupLevel();
    }
    else if(GameManager::getThis()->getLevel() == 1)
    {
        int reputation = GameHUD::getThis()->targetReputation;
        ss.str(std::string());
        ss << username << "_level_1_score";
        float prevReputation = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        if(prevReputation < reputation)
        {
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), (float) reputation);
        }
        
        ss.str(std::string());
        ss << username << "_level_2_open";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
        
        GameManager::getThis()->setLevel(2);
        
        scheduleHideScoreMenu();
        
        GameScene::getThis()->reSetupLevel();
    }
    else if(GameManager::getThis()->getLevel() == 2)
    {
        int reputation = GameHUD::getThis()->targetReputation;
        ss.str(std::string());
        ss << username << "_level_2_score";
        CCArray* sprites = GameScene::getThis()->spriteHandler->spritesOnMap;
        float total = 0;
        for (int i = 0; i < sprites->count(); i++)
        {
            GameSprite* gs = (GameSprite*) sprites->objectAtIndex(i);
            if(gs->villagerClass == V_SOLDIER)
            {
                total += 100;
            }
        }
        
        if(total > 1000)
        {
            total = 1000;
        }
        
        reputation += total;
        
        float prevReputation = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        if(prevReputation < reputation)
        {
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), (float) reputation);
        }
        
        ss.str(std::string());
        ss << username << "_level_3_open";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
        
        GameManager::getThis()->setLevel(3);
        
        scheduleHideScoreMenu();
        
        GameScene::getThis()->reSetupLevel();
    }
    else if(GameManager::getThis()->getLevel() == 3)
    {
        int reputation = GameHUD::getThis()->targetReputation;
        ss.str(std::string());
        ss << username << "_level_3_score";
        
        float prevReputation = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        if(prevReputation < reputation)
        {
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), (float) reputation);
        }
        
        ss.str(std::string());
        ss << username << "_level_4_open";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
        
        GameManager::getThis()->setLevel(4);
        
        scheduleHideScoreMenu();
        
        GameScene::getThis()->reSetupLevel();
    }
    else if(GameManager::getThis()->getLevel() == 4)
    {
        int reputation = GameHUD::getThis()->targetReputation;
        ss.str(std::string());
        ss << username << "_level_4_score";
        
        float prevReputation = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        if(prevReputation < reputation)
        {
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), (float) reputation);
        }
        
        ss.str(std::string());
        ss << username << "_level_5_open";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
        
        GameManager::getThis()->setLevel(5);
        
        scheduleHideScoreMenu();
        
        GameScene::getThis()->reSetupLevel();
    }
    else if(GameManager::getThis()->getLevel() == 5)
    {
        int reputation = GameHUD::getThis()->targetReputation;
        ss.str(std::string());
        ss << username << "_level_5_score";
        
        float prevReputation = CCUserDefault::sharedUserDefault()->getFloatForKey(ss.str().c_str());
        if(prevReputation < reputation)
        {
            CCUserDefault::sharedUserDefault()->setFloatForKey(ss.str().c_str(), (float) reputation);
        }
        
        ss.str(std::string());
        ss << username << "_level_6_open";
        CCUserDefault::sharedUserDefault()->setBoolForKey(ss.str().c_str(), true);
        
        GameManager::getThis()->setLevel(6);
        
        scheduleHideScoreMenu();
        
        GameScene::getThis()->reSetupLevel();

    }
}

void ScoreMenu::preloadTextures()
{
    scoreMenuNode = CCSpriteBatchNode::create("ScoreMenu.png");
    GameHUD::getThis()->addChild(scoreMenuNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ScoreMenu.plist");
}

void ScoreMenu::clearTextures()
{
    GameHUD::getThis()->removeChild(scoreMenuNode, true);
    scoreMenuNode = NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ScoreMenu.plist");
}

