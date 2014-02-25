//
//  GameHUD.cpp
//  PopulationTest
//
//  Created by Serious Games on 8/3/13.
//
//

#include "GameHUD.h"
#include "GameScene.h"
#include "InGameMenu.h"
#include "BuildMenu.h"
#include "PopulationMenu.h"
#include "TutorialHandler.h"
#include "GlobalSettings.h"
#include "GlobalHelper.h"

GameHUD* GameHUD::SP;


GameHUD::GameHUD()
{
    GameHUD::SP = this;
    date = new Date();
    
    cumulatedTime = 0;
    buildScroll = NULL;
    
    pause = false;
    
    mGameDay = 0;
    mGameMonth = 0;
    mGameWeek = 0;
    mGameYear = 0;
}

GameHUD::~GameHUD()
{
    delete date;
    
    menuItems_build->removeAllObjects();
    menuItems_objective->removeAllObjects();
    menuItems_pause->removeAllObjects();
    
    menuItems_build->release();
    menuItems_objective->release();
    menuItems_pause->release();

    GameHUD::SP = NULL;
}

GameHUD* GameHUD::getThis()
{
    return SP;
}


GameHUD* GameHUD::create()
{
    GameHUD *pRet = new GameHUD();
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

bool GameHUD::init()
{
    currTapMode = Normal;
    menuIsOpen = false;
    
    this->CCLayer::setTouchEnabled(true);
    createInitialGUI();
    
    return true;
}

void GameHUD::update(float deltaTime)
{
    if(pause)
    {
        return;
    }
    
    // update date
    cumulatedTime += deltaTime;
    
    if(cumulatedTime > 1)
    {
        date->addDay();
        cumulatedTime = 0;
    }
    
    // update time group
    // if week has been changed
    if(mGameWeek != date->week)
    {
        switch (date->week) {
            case 0:
                firstWeekLabel->setVisible(true);
                secondWeekLabel->setVisible(false);
                thirdWeekLabel->setVisible(false);
                lastWeekLabel->setVisible(false);
                break;
            case 1:
                firstWeekLabel->setVisible(false);
                secondWeekLabel->setVisible(true);
                thirdWeekLabel->setVisible(false);
                lastWeekLabel->setVisible(false);
                break;
            case 2:
                firstWeekLabel->setVisible(false);
                secondWeekLabel->setVisible(false);
                thirdWeekLabel->setVisible(true);
                lastWeekLabel->setVisible(false);
                break;
            case 3:
                firstWeekLabel->setVisible(false);
                secondWeekLabel->setVisible(false);
                thirdWeekLabel->setVisible(false);
                lastWeekLabel->setVisible(true);
                break;
                
            default:
                break;
        }
        mGameWeek = date->week;
    }
    
    // if month has been changed
    if(mGameMonth != date->month)
    {
        if(date->month < 3)
        {
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_spring-bg.png"));
        }
        else if(date->month < 6)
        {
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_summer-bg.png"));
        }
        else if(date->month < 9)
        {
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_autumn-bg.png"));
        }
        else
        {
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_winter-bg.png"));
        }
        
        std::stringstream ss;
        ss << "Month: " << (date->month + 1);
        timeLabel_2->setString(ss.str().c_str());
        mGameMonth = date->month;
    }
    
    // if year has been changed
    if(mGameYear != date->year)
    {
        std::stringstream ss;
        ss << "Year: " << (date->year + 1);
        timeLabel_1->setString(ss.str().c_str());
        mGameYear = date->year;
    }
}

void GameHUD::createInitialGUI(){
    
    createStatsMenu();
    createObjectiveMenu();
    createTimeMenu();
    createBuildMenu();
 
}

void GameHUD::setTapMode(int mode)
{
    currTapMode = (tapMode)mode;
}

int GameHUD::getTapMode()
{
    return currTapMode;
}

void GameHUD::onOrientationChanged(){
    rotateStatsMenu();
    rotateTimeMenu();
    rotateObjectiveMenu();
    rotateBuildMenu();
    
    if(buildScroll != NULL)
    {
        buildScroll->onOrientationChanged();
    }
}

void GameHUD::closeAllMenuAndResetTapMode()
{    
    GameScene::getThis()->resetBuildMode();
    
    // Continues building path once built
    if (currTapMode == 5)
    {
        setTapMode(3);
        return;
    }
    
    //hideBuildLabel();
    setTapMode(0);
}

void GameHUD::buyBuilding(int cost){
    GameManager::getThis()->currentMoney -= cost;
    updateMoneyLabel();
}

void GameHUD::onSpriteAddedToMap(GameSprite* sprite)
{
    /*
    GameManager::getThis()->totalHappiness += sprite->getPossessions()->happinessRating;
    updatePopTotalLabel();
    updateAvgHapLabel();
    
    if (currMenuMode == PopulationMenu)
    {
        if (class PopulationMenu* menu = dynamic_cast<class PopulationMenu*>(PopupMenu::getBottommostPopupMenu()))  
            menu->addPopulationToList(sprite);
    }
    */
}

void GameHUD::onSpriteRemovedFromMap(GameSprite* sprite)
{
    /*
    GameManager::getThis()->totalHappiness -= sprite->getPossessions()->happinessRating;
    updatePopTotalLabel();
    updateAvgHapLabel();
    
    if (currMenuMode == PopulationMenu)
    {
        if (class PopulationMenu* menu = dynamic_cast<class PopulationMenu*>(PopupMenu::getBottommostPopupMenu()))
            menu->removePopulationFromList(sprite);
    }
    */
}

void GameHUD::updatePopTotalLabel()
{
    /*
    std::stringstream ss;
    ss << "Total Pop: " << GameScene::getThis()->spriteHandler->spritesOnMap->count();
    popTotalLabel->setString(ss.str().c_str());
    */
}

void GameHUD::updateAvgHapLabel()
{
    /*
    std::stringstream ss;
    ss << "Avg Hap: " << GameManager::getThis()->getAverageHappiness();
    avgHapLabel->setString(ss.str().c_str());
    */
}

void GameHUD::updateMoneyLabel()
{
    /*
    std::stringstream ss;
    ss << GameManager::getThis()->currentMoney << " G";
    */
}

void GameHUD::updateBuildCostLabel(int cost, int dist)
{
    /*
    std::stringstream ss;
    ss << "Cost: " << cost * (dist >= 0 ? dist : 1) << "G";
    if (dist >= 0)
        ss << " (" << dist << "x" << cost << "G)";
    
    buildCostLabel->setString(ss.str().c_str());
    */
}


void GameHUD::showHint(std::string hintText)
{
    /*
    hintLabel->setFontSize(default_hint_font_size);
    hintLabel->setString(hintText.c_str());
    if ((hintLabel->getContentSize().width + 10) > hintInfoBar->size.width)
    {

        hintLabel->setFontSize(default_hint_font_size * (hintInfoBar->size.width/(hintLabel->getContentSize().width + 10)));
    }
    
    hintInfoBar->setVisible(true);
    
    curr_hint_show_time = hint_show_time;
    */
}

Date* GameHUD::getDate()
{
    return date;
}

/*************************************************** jerry added ************************************************/
void GameHUD::createStatsMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    bool isHori = GlobalHelper::isHorizontal();
    
    // create the background of the stats menu
    statsMenu = CCSprite::create("main-info-menu-bg_01.png");
    CCSize spriteSize = statsMenu->getContentSize();
    if(isHori)
    {
        statsMenu->setScale(screenSize.width / spriteSize.width * 0.6f);
    }
    else
    {
        statsMenu->setScale(screenSize.height / spriteSize.width * 0.6f);
    }
    statsMenu->setAnchorPoint(ccp(0, 1));
    statsMenu->setPosition(ccp(0, screenSize.height));
    this->addChild(statsMenu, 1);
    
    // create the money indicator
    moneyIcon = CCSprite::create("main-menu-resource-icons_gold.png");
    if(isHori)
    {
        moneyIcon->setScale(screenSize.width / spriteSize.width * 0.6f);
    }
    else
    {
        moneyIcon->setScale(screenSize.height / spriteSize.width * 0.6f);
    }
    moneyIcon->setAnchorPoint(ccp(0, 1));
    moneyIcon->setPosition(ccp(180, screenSize.height - 40));
    this->addChild(moneyIcon, 2);
    
    std::stringstream ss;
    ss << "1250g";
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    moneyLabel->setColor(colorBlack);
    moneyLabel->setAnchorPoint(ccp(0, 1));
    this->addChild(moneyLabel, 2);
    moneyLabel->CCNode::setPosition(240, screenSize.height - 60);
    
    // create the food indicator
    foodIcon = CCSprite::create("main-menu-resource-icons_food.png");
    if(isHori)
    {
        foodIcon->setScale(screenSize.width / spriteSize.width * 0.6f);
    }
    else
    {
        foodIcon->setScale(screenSize.height / spriteSize.width * 0.6f);
    }
    foodIcon->setAnchorPoint(ccp(0, 1));
    foodIcon->setPosition(ccp(315, screenSize.height - 40));
    this->addChild(foodIcon, 2);
    
    ss.str(std::string());
    ss << "50/100";
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    foodLabel->setColor(colorBlack);
    foodLabel->setAnchorPoint(ccp(0, 1));
    this->addChild(foodLabel, 2);
    foodLabel->CCNode::setPosition(375, screenSize.height - 60);
    
    // create the population indicator
    populationIcon = CCSprite::create("main-menu-resource-icons_population.png");
    if(isHori)
    {
        populationIcon->setScale(screenSize.width / spriteSize.width * 0.6f);
    }
    else
    {
        populationIcon->setScale(screenSize.height / spriteSize.width * 0.6f);
    }
    populationIcon->setAnchorPoint(ccp(0, 1));
    populationIcon->setPosition(ccp(460, screenSize.height - 40));
    this->addChild(populationIcon, 2);
    
    ss.str(std::string());
    ss << "10/15";
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    populationLabel->setColor(colorBlack);
    populationLabel->setAnchorPoint(ccp(0, 1));
    this->addChild(populationLabel, 2);
    populationLabel->CCNode::setPosition(520, screenSize.height - 60);
    
    // create the achievements label for the values
    ss.str(std::string());
    ss << "1100/2000";
    achivementsLabel = CCLabelTTF::create(ss.str().c_str(), "Arial", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    achivementsLabel->setColor(colorBlack);
    achivementsLabel->setAnchorPoint(ccp(0, 1));
    this->addChild(achivementsLabel, 2);
    achivementsLabel->CCNode::setPosition(190, screenSize.height - 105);
}

void GameHUD::createTimeMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    bool isHori = GlobalHelper::isHorizontal();
    
    // create the time group background
    string timeBackground = "time_spring-bg.png";
    timeMenu = CCSprite::create(timeBackground.c_str());
    CCSize spriteSize = timeMenu->getContentSize();
    if(isHori)
    {
        timeMenu->setAnchorPoint(ccp(1, 1));
        timeMenu->setScale(screenSize.width / spriteSize.width * 0.25f);
        timeMenu->setPosition(ccp(screenSize.width, screenSize.height - 20.0f));
    }
    else
    {
        timeMenu->setAnchorPoint(ccp(0, 1));
        timeMenu->setScale(screenSize.height / spriteSize.width * 0.25f);
        timeMenu->setPosition(ccp(0, screenSize.height - 140.0f));
    }
    this->addChild(timeMenu, 1);
    
    // create the four weeks labels
    firstWeekLabel = CCSprite::create("time-indicator-week-1.png");
    firstWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        firstWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25f);
        firstWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
    }
    else
    {
        firstWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        firstWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
    }
    this->addChild(firstWeekLabel, 2);
    
    secondWeekLabel = CCSprite::create("time_indicator-week-2.png");
    secondWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        secondWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25);
        secondWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
    }
    else
    {
        secondWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        secondWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
    }
    this->addChild(secondWeekLabel, 2);
    
    thirdWeekLabel = CCSprite::create("time_indicator-week-3.png");
    thirdWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        thirdWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25f);
        thirdWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
    }
    else
    {
        thirdWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        thirdWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
    }
    this->addChild(thirdWeekLabel, 2);
    
    lastWeekLabel = CCSprite::create("time_indicator-week-4.png");
    lastWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        lastWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25f);
        lastWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
    }
    else
    {
        lastWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        lastWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
    }
    this->addChild(lastWeekLabel, 2);
    
    firstWeekLabel->setVisible(true);
    secondWeekLabel->setVisible(false);
    thirdWeekLabel->setVisible(false);
    lastWeekLabel->setVisible(false);
    
    // save the current date
    mGameWeek = date->week;
    mGameMonth = date->month;
    mGameYear = date->year;
    
    // display the date in the time menu
    std::stringstream ss;
    ss << "Year: " << (date->year + 1);
    timeLabel_1 = CCLabelTTF::create(ss.str().c_str(), "Arial", 28, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    timeLabel_1->setColor(colorBlack);
    timeLabel_1->setAnchorPoint(ccp(0.5, 1));
    this->addChild(timeLabel_1, 2);
    if(isHori)
    {
        timeLabel_1->CCNode::setPosition(screenSize.width - 50.0f, screenSize.height - 40.0f);
    }
    else
    {
        timeLabel_1->CCNode::setPosition(timeMenu->boundingBox().size.width - 50.0f, screenSize.height - 160.0f);
    }
    
    ss.str(std::string());
    ss << "Month: " << (date->month + 1);
    timeLabel_2 = CCLabelTTF::create(ss.str().c_str(), "Arial", 28, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    timeLabel_2->setColor(colorBlack);
    timeLabel_2->setAnchorPoint(ccp(0.5, 1));
    this->addChild(timeLabel_2, 2);
    if(isHori)
    {
        timeLabel_2->CCNode::setPosition(screenSize.width - 50.0f, screenSize.height - 80.0f);
    }
    else
    {
        timeLabel_2->CCNode::setPosition(timeMenu->boundingBox().size.width - 50.0f, screenSize.height - 200.0f);
    }
    
    menuItems_pause = CCArray::create();
    menuItems_pause->retain();
    
    pauseButton = CCMenuItemImage::create("pauseIcon.png", "pauseIcon.png", this, menu_selector(GameHUD::pauseGame));
    resumeButton = CCMenuItemImage::create("nextButton.png", "nextButton.png", this, menu_selector(GameHUD::pauseGame));
    if(isHori)
    {
        pauseButton->setScale(screenSize.width / spriteSize.width * 0.05f);
        resumeButton->setScale(screenSize.width / spriteSize.width * 0.05f);

    }
    else
    {
        pauseButton->setScale(screenSize.height / spriteSize.width * 0.05f);
        resumeButton->setScale(screenSize.height / spriteSize.width * 0.05f);
    }
    resumeButton->setVisible(false);
    
    menuItems_pause->addObject(pauseButton);
    menuItems_pause->addObject(resumeButton);
    menu_pause = CCMenu::createWithArray(menuItems_pause);
    menu_pause->setAnchorPoint(ccp(0.5, 0.5));
    
    if(isHori)
    {
        menu_pause->setAnchorPoint(ccp(1, 1));
        menu_pause->setPosition(screenSize.width - 280.0f, screenSize.height - 40.0f);
    }
    else
    {
        menu_pause->setAnchorPoint(ccp(0, 1));
        menu_pause->setPosition(screenSize.width - 150.0f, screenSize.height - 140.0f);
    }
    
    this->addChild(menu_pause, 5);
}

void GameHUD::pauseGame()
{
    if(pause)
    {
        pause = false;
        pauseButton->setVisible(true);
        resumeButton->setVisible(false);
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* sp = (GameSprite*)spritesOnMap->objectAtIndex(i);
            sp->followPath();
        }
    }
    else
    {
        pause = true;
        pauseButton->setVisible(false);
        resumeButton->setVisible(true);
    }
}

void GameHUD::createObjectiveMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    bool isHori = GlobalHelper::isHorizontal();
    
    // create the objective group background
    string objectiveBackground = "objective-menu-bg_07.png";
    objectiveMenu = CCSprite::create(objectiveBackground.c_str());
    CCSize spriteSize = objectiveMenu->getContentSize();
    objectiveMenu->setVisible(false);
    objectiveMenu->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        objectiveMenu->setScale(screenSize.width / spriteSize.width * 0.55f);
        objectiveMenu->setPosition(ccp(40, screenSize.height - 185.0f));
    }
    else
    {
        objectiveMenu->setScale(screenSize.height / spriteSize.width * 0.55f);
        objectiveMenu->setPosition(ccp(40, screenSize.height - 305.0f));
    }
    this->addChild(objectiveMenu, 1);
    
    // create the objective button
    // Menu items
    menuItems_objective = CCArray::create();
    menuItems_objective->retain();
    
    objectiveButton = CCMenuItemImage::create("objective-menu-button_06.png", "objective-menu-button_06.png", this, menu_selector(GameHUD::clickObjectiveButton));
    objectiveButton->setAnchorPoint(ccp(0.3, 0.5));
    if(isHori)
    {
        objectiveButton->setScale(screenSize.width / spriteSize.width * 0.55f);
    }
    else
    {
        objectiveButton->setScale(screenSize.height / spriteSize.width * 0.55f);
    }
    objectiveButton->setTag(-1);
    
    menuItems_objective->addObject(objectiveButton);
    menu_objective = CCMenu::createWithArray(menuItems_objective);
    menu_objective->setAnchorPoint(ccp(0.5, 0.5));
    if(isHori)
    {
        menu_objective->setPosition(ccp(40, screenSize.height - 185));
    }
    else
    {
        menu_objective->setPosition(ccp(40, screenSize.height - 305));
    }
    
    this->addChild(menu_objective, 2);
}

void GameHUD::clickObjectiveButton()
{
    if(objectiveMenu->isVisible())
    {
        objectiveMenu->setVisible(false);
    }
    else
    {
        objectiveMenu->setVisible(true);
    }
}

void GameHUD::createBuildMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // create the build button
    // Menu items
    menuItems_build = CCArray::create();
    menuItems_build->retain();
    
    buildButton = CCMenuItemImage::create("main-game-buttons_build.png", "main-game-buttons_build.png", this, menu_selector(GameHUD::clickBuildButton));
    buildButton->setScale(0.3f);
    buildButton->setAnchorPoint(ccp(1, 1));
    buildButton->setTag(-1);
    
    menuItems_build->addObject(buildButton);
    menu_build = CCMenu::createWithArray(menuItems_build);
    menu_build->setAnchorPoint(ccp(1, 1));
    menu_build->setPosition(ccp(screenSize.width, screenSize.height - 120));
    
    this->addChild(menu_build, 2);
}

void GameHUD::clickBuildButton()
{
    if(BuildScroll::getThis() == NULL)
    {
        buildScroll = BuildScroll::create();
        buildScroll->useAsTopmostPopupMenu();
    }
    else
    {
        BuildScroll::getThis()->closeMenu(true);
        buildScroll = NULL;
    }
}

void GameHUD::rotateStatsMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    statsMenu->setPosition(ccp(0, screenSize.height));
    moneyIcon->setPosition(ccp(180, screenSize.height - 40));
    moneyLabel->CCNode::setPosition(240, screenSize.height - 60);
    foodIcon->setPosition(ccp(315, screenSize.height - 40));
    foodLabel->CCNode::setPosition(375, screenSize.height - 60);
    populationIcon->setPosition(ccp(460, screenSize.height - 40));
    populationLabel->CCNode::setPosition(520, screenSize.height - 60);
    achivementsLabel->CCNode::setPosition(190, screenSize.height - 105);
}

void GameHUD::rotateTimeMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    bool isHori = GlobalHelper::isHorizontal();
    
    if(isHori)
    {
        timeMenu->setAnchorPoint(ccp(1, 1));
        timeMenu->setPosition(ccp(screenSize.width, screenSize.height - 20.0f));
        firstWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
        secondWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
        thirdWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
        lastWeekLabel->setPosition(ccp(screenSize.width - 256.0f, screenSize.height - 20.0f));
        timeLabel_1->CCNode::setPosition(screenSize.width - 50.0f, screenSize.height - 40.0f);
        timeLabel_2->CCNode::setPosition(screenSize.width - 50.0f, screenSize.height - 80.0f);
    }
    else
    {
        timeMenu->setAnchorPoint(ccp(0, 1));
        timeMenu->setPosition(ccp(0, screenSize.height - 140.0f));
        firstWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
        secondWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
        thirdWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
        lastWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 256.0f, screenSize.height - 140.0f));
        timeLabel_1->CCNode::setPosition(timeMenu->boundingBox().size.width - 50.0f, screenSize.height - 160.0f);
        timeLabel_2->CCNode::setPosition(timeMenu->boundingBox().size.width - 50.0f, screenSize.height - 200.0f);
    }
}

void GameHUD::rotateObjectiveMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    bool isHori = GlobalHelper::isHorizontal();
    
    if(isHori)
    {
        objectiveMenu->setPosition(ccp(40, screenSize.height - 185.0f));
        menu_objective->setPosition(ccp(40, screenSize.height - 185));
    }
    else
    {
        objectiveMenu->setPosition(ccp(40, screenSize.height - 305.0f));
        menu_objective->setPosition(ccp(40, screenSize.height - 305));
    }
}

void GameHUD::rotateBuildMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

    menu_build->setPosition(ccp(screenSize.width, screenSize.height - 120));
}