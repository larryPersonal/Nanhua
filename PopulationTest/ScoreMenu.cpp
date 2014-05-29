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
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    
    blackScreen = CCSprite::create("blackscreen.png");
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    blackScreen->setOpacity((GLubyte) 0);
    layer->addChild(blackScreen, 9);
    
    background = CCSprite::create("achievementscreen.png");
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
    
    renToken = CCSprite::create("ren_icon.png");
    renToken->setAnchorPoint(ccp(0, 0.5));
    renToken->setScale(1.0f);
    background->addChild(renToken);
    renToken->setPosition(ccp(screenSize.width - 150.0f, screenSize.height / 2.0f * 3.0f - 300.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->targetReputation;
    
    renTokenLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 44);
    renTokenLabel->setAnchorPoint(ccp(0.5, 0.5));
    renTokenLabel->setColor(colorBlack);
    background->addChild(renTokenLabel);
    renTokenLabel->setPosition(ccp(screenSize.width + 100.0f, screenSize.height / 2.0f * 3.0f - 300.0f));
    
    money = CCSprite::create("money_label.png");
    money->setAnchorPoint(ccp(0, 0.5));
    money->setScale(0.5f);
    background->addChild(money);
    money->setPosition(ccp(screenSize.width / 2.0f + 140.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->targetMoney;
    
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 32);
    moneyLabel->setAnchorPoint(ccp(0, 0.5));
    moneyLabel->setColor(colorBlack);
    background->addChild(moneyLabel);
    moneyLabel->setPosition(ccp(screenSize.width / 2.0f + 250.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    food = CCSprite::create("food_label.png");
    food->setAnchorPoint(ccp(0, 0.5));
    food->setScale(0.5f);
    background->addChild(food);
    food->setPosition(ccp(screenSize.width * 2.0f / 2.0f - 90.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->targetFood << "/" << GameHUD::getThis()->targetStorage;
    
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 32);
    foodLabel->setAnchorPoint(ccp(0, 0.5));
    foodLabel->setColor(colorBlack);
    background->addChild(foodLabel);
    foodLabel->setPosition(ccp(screenSize.width * 2.0f / 2.0f + 0.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    population = CCSprite::create("population_icon.png");
    population->setAnchorPoint(ccp(0, 0.5));
    population->setScale(0.5f);
    background->addChild(population);
    population->setPosition(ccp(screenSize.width * 3.0f / 2.0f - 350.0f, screenSize.height / 2.0f * 3.0f - 410.0f));
    
    ss.str(std::string());
    ss << GameHUD::getThis()->populationLabel->getString();
    
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 32);
    populationLabel->setAnchorPoint(ccp(0, 0.5));
    populationLabel->setColor(colorBlack);
    background->addChild(populationLabel);
    populationLabel->setPosition(ccp(screenSize.width * 3.0f / 2.0f - 250.0f, screenSize.height / 2.0f * 3.0f - 400.0f));
    
    ss.str(std::string());
    ss << "Congratulations for your good result!";
    congratsLabel = CCLabelTTF::create(ss.str().c_str(), "GillSansMT", 40);
    congratsLabel->setAnchorPoint(ccp(0.5, 1));
    congratsLabel->setColor(colorBlack);
    background->addChild(congratsLabel);
    congratsLabel->setPosition(ccp(screenSize.width, screenSize.height / 2.0f - 10.0f));
    
    return true;
}

void ScoreMenu::releaseAll()
{
    GameHUD::getThis()->removeChild(blackScreen);
    GameHUD::getThis()->removeChild(background);
    CC_SAFE_RELEASE(this);
}

void ScoreMenu::scheduleShowScoreMenu()
{
    if(!show && !hide)
    {
        show = true;
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


