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
#include "GlobalHelper.h"
#include "MainMenuScene.h"
#include "Objective.h"

GameHUD* GameHUD::SP;


GameHUD::GameHUD()
{
    GameHUD::SP = this;
    date = new Date();
    
    cumulatedTime = 0;
    buildScroll = NULL;
    
    pause = false;
    getMoney = true;
    
    mGameDay = 0;
    mGameMonth = 0;
    mGameWeek = 0;
    mGameYear = 0;
    
    mGameReputation = 0;
    mGameReputationMax = 0;
    reputation = 0;
    reputationMax = 0;
    
    mAverageHappiness = 0;
    average_happiness = 0;
    
    is_token_drop_cooldown = false;
    token_drop_cooldown_time = 0;
    
    growthPopulation = 0;
    mGameTapMode = Normal;
    money = GameScene::getThis()->settingsLevel->default_start_money;
    
    originalHappiness = 0;
    stickHappiness = false;
    
    isThisTapCounted = false;
    
    startWar = false;
    
    objectiveStrs = CCArray::create();
    objectiveStrs->retain();
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
    
    objectiveStrs->removeAllObjects();
    CC_SAFE_RELEASE(objectiveStrs);

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
    
    //this->CCLayer::setTouchEnabled(true);
    
    setAllStats();
    createInitialGUI();
    
    return true;
}

void GameHUD::setAllStats()
{
    mGameCurrentFood = 0;
    mGameCurrentStorage = 0;
    
    mGameCurrentCitizenPopulation = 0;
    mGameCurrentPopulationRoom = 0;
}

void GameHUD::update(float deltaTime)
{
    if(pause)
    {
        return;
    }
    
    // update date
    cumulatedTime += deltaTime;
    
    if(cumulatedTime > GameScene::getThis()->configSettings->secondToDayRatio)
    {
        date->addDay();
        cumulatedTime = 0;
        
        if(date->week == 0 && date->day == 0)
        {
            CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
            for(int i = 0; i < spritesOnMap->count(); i++)
            {
                GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
                gs->futureAction1 = EATING;
                gs->futureAction2 = RESTING;
            }
        }
    }
    
    // if it's the first day of the month, update money;
    if (date->day == 0 && date->week == 0 && getMoney)
    {
        money += 100 * GameScene::getThis()->buildingHandler->housingOnMap->count();
        getMoney = false;
    }
    
    // if it's the last day of the month, change get money to true;
    if (date->day == 6 && date->week == 3)
    {
        getMoney = true;
    }
    
    // if money has change, update in the UI
    if (mGameMoney != money)
    {
        mGameMoney = money;
        stringstream ss;
        ss << mGameMoney << "g";
        moneyLabel->setString(ss.str().c_str());
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
            //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_spring-bg.png"));
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock.png"));
            
        }
        else if(date->month < 6)
        {
            //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_summer-bg.png"));
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock.png"));
            
        }
        else if(date->month < 9)
        {
           //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_autumn-bg.png"));
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock.png"));
            
        }
        else
        {
            //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_winter-bg.png"));
            timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock.png"));
            
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
    
    // reputation change
    if(mGameReputation != reputation || mGameReputationMax != reputationMax)
    {
        mGameReputation = reputation;
        mGameReputationMax = reputationMax;
        
        std::stringstream ss;
        ss << mGameReputation << "/" << mGameReputationMax;
        
        achivementsLabel->setString(ss.str().c_str());
    }
    
    float totalHappiness = 0;
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        totalHappiness += gs->getPossessions()->happinessRating;
    }
    average_happiness = totalHappiness / (float) spritesOnMap->count();
    
    if(mAverageHappiness != average_happiness)
    {
        mAverageHappiness = average_happiness;
        std::stringstream ss;
        ss << mAverageHappiness;
        average_happiness_label->setString(ss.str().c_str());
    }
    
    // room and population change;
    int temp = 0;
    spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    for (int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        if(gs->getHome() != NULL)
        {
            temp++;
        }
    }
    
    if(mGameCurrentCitizenPopulation != temp)
    {
        mGameCurrentCitizenPopulation = temp;
        std::stringstream ss;
        ss << mGameCurrentCitizenPopulation << "/" << mGameCurrentPopulationRoom;
        populationLabel->setString(ss.str().c_str());
    }
    
    temp = 0;
    CCArray* housingOnMap = GameScene::getThis()->buildingHandler->housingOnMap;
    for (int i = 0; i < housingOnMap->count(); i++)
    {
        Building* building = (Building*) housingOnMap->objectAtIndex(i);
        temp += building->populationLimit;
    }
    
    if(mGameCurrentPopulationRoom != temp)
    {
        mGameCurrentPopulationRoom = temp;
        std::stringstream ss;
        ss << mGameCurrentCitizenPopulation << "/" << mGameCurrentPopulationRoom;
        populationLabel->setString(ss.str().c_str());
    }
    
    // update the tapMode
    if (mGameTapMode != currTapMode)
    {
        mGameTapMode = currTapMode;
        std::stringstream ss;
        switch (mGameTapMode)
        {
            case 0:
                ss << "Normal";
                break;
            case 1:
                ss << "Build";
                break;
            case 2:
                ss << "Deconstruct";
                break;
            case 3:
                ss << "BuildPathPreview";
                break;
            case 4:
                ss << "UnBuildPath";
                break;
            case 5:
                ss << "BuildPathLine";
                break;
            default:
                break;

        }
        tapModeLabel->setString(ss.str().c_str());
    }
    
    // for food and storage change!
    temp = 0;
    int temp_storage = 0;
    CCArray* buildingsOnMap = GameScene::getThis()->buildingHandler->allBuildingsOnMap;
    for (int i = 0; i < buildingsOnMap->count(); i++)
    {
        Building* building = (Building*) buildingsOnMap->objectAtIndex(i);
        if(building->buildingType == GRANARY)
        {
            temp += building->currentStorage;
            temp_storage += building->storageLimit;
        }
    }
    
    if (mGameCurrentFood != temp || mGameCurrentStorage != temp_storage)
    {
        mGameCurrentFood = temp;
        mGameCurrentStorage = temp_storage;
        
        stringstream ss;
        ss << mGameCurrentFood << "/" << mGameCurrentStorage;
        foodLabel->setDimensions(CCSizeMake(ss.str().length() * 20.0f, 5.0f));
        foodLabel->setString(ss.str().c_str());
    }
}

void GameHUD::createInitialGUI(){
    
    reputation = GameScene::getThis()->configSettings->default_ini_reputation;
    reputationMax = GameScene::getThis()->settingsLevel->default_max_reputation;
    
    mGameReputation = reputation;
    mGameReputationMax = reputationMax;
    
    createStatsMenu();
    createObjectiveMenu();
    createTimeMenu();
    createBuildMenu();
    createSystemMenu();
 
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
    ccColor3B colorWhite = ccc3(255, 255, 255);
    bool isHori = GlobalHelper::isHorizontal();
    
    // create the background of the stats menu
    statsMenu = CCSprite::create("chargirl_represent.png");
    CCSize spriteSize = statsMenu->getContentSize();
    if(isHori)
    {
        statsMenu->setScale(screenSize.width / spriteSize.width * 0.25f);
    }
    else
    {
        statsMenu->setScale(screenSize.height / spriteSize.width * 0.25f);
    }
    statsMenu->setAnchorPoint(ccp(0, 1));
    statsMenu->setPosition(ccp(0, screenSize.height));
    this->addChild(statsMenu, 1);
    
    // create the money indicator
    moneyIcon = CCSprite::create("yuanbao_amount.png");
    if(isHori)
    {
        moneyIcon->setScale(screenSize.width / spriteSize.width * 0.25f);
    }
    else
    {
        moneyIcon->setScale(screenSize.height / spriteSize.width * 0.25f);
    }
    moneyIcon->setAnchorPoint(ccp(0, 1));
    moneyIcon->setPosition(ccp(110, screenSize.height - 8));
    this->addChild(moneyIcon, 2);
    
    std::stringstream ss;
    ss << money << "G";
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    moneyLabel->setColor(colorWhite);
    moneyLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(moneyLabel, 2);
    moneyLabel->CCNode::setPosition(255, screenSize.height - 25);
    
    // create the food indicator
    foodIcon = CCSprite::create("rice_amount.png");
    if(isHori)
    {
        foodIcon->setScale(screenSize.width / spriteSize.width * 0.25f);
    }
    else
    {
        foodIcon->setScale(screenSize.height / spriteSize.width * 0.25f);
    }
    foodIcon->setAnchorPoint(ccp(0, 1));
    foodIcon->setPosition(ccp(340, screenSize.height));
    this->addChild(foodIcon, 2);
    
    ss.str(std::string());
    ss << mGameCurrentFood << "/" << mGameCurrentStorage;
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    foodLabel->setColor(colorWhite);
    foodLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(foodLabel, 2);
    foodLabel->CCNode::setPosition(450, screenSize.height - 25);
    
    // create the population indicator
    populationIcon = CCSprite::create("population_amount.png");
    if(isHori)
    {
        populationIcon->setScale(screenSize.width / spriteSize.width * 0.25f);
    }
    else
    {
        populationIcon->setScale(screenSize.height / spriteSize.width * 0.25f);
    }
    populationIcon->setAnchorPoint(ccp(0, 1));
    populationIcon->setPosition(ccp(540, screenSize.height - 2));
    this->addChild(populationIcon, 2);
    
    ss.str(std::string());
    ss << mGameCurrentCitizenPopulation << "/" << mGameCurrentPopulationRoom;
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    populationLabel->setColor(colorWhite);
    populationLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(populationLabel, 2);
    populationLabel->CCNode::setPosition(650, screenSize.height - 25);
    
    // create the achievements label for the values
    ss.str(std::string());
    ss << GameScene::getThis()->configSettings->default_ini_reputation << "/" << GameScene::getThis()->settingsLevel->default_max_reputation;
    achivementsLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    achivementsLabel->setColor(colorBlack);
    achivementsLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(achivementsLabel, 2);
    achivementsLabel->CCNode::setPosition(185, screenSize.height - 75);
    
    // showing the average happiness attribute
    average_happiness = 0;
    mAverageHappiness = average_happiness;
    
    ss.str(std::string());
    ss << mAverageHappiness;
    
    average_happiness_label = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    average_happiness_label->setColor(colorBlack);
    average_happiness_label->setAnchorPoint(ccp(0, 0));
    this->addChild(average_happiness_label, 2);
    average_happiness_label->CCNode::setPosition(620, 0);
    
    // tap mode label
    ss.str(std::string());
    switch (currTapMode)
    {
        case 0:
            ss << "Normal";
            break;
        case 1:
            ss << "Build";
            break;
        case 2:
            ss << "Deconstruct";
            break;
        case 3:
            ss << "BuildPathPreview";
            break;
        case 4:
            ss << "UnBuildPath";
            break;
        case 5:
            ss << "BuildPathLine";
            break;
        default:
            break;
    }
    tapModeLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    tapModeLabel->setColor(colorBlack);
    tapModeLabel->setAnchorPoint(ccp(0, 0));
    this->addChild(tapModeLabel, 2);
    tapModeLabel->CCNode::setPosition(720, 0);
}

void GameHUD::createTimeMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    bool isHori = GlobalHelper::isHorizontal();
    
    // create the time group background
    string timeBackground = "timeclock.png";
    timeMenu = CCSprite::create(timeBackground.c_str());
    CCSize spriteSize = timeMenu->getContentSize();
    if(isHori)
    {
        timeMenu->setAnchorPoint(ccp(1, 1));
        timeMenu->setScale(screenSize.width / spriteSize.width * 0.275f);
        timeMenu->setPosition(ccp(screenSize.width, screenSize.height - 20.0f));
    }
    else
    {
        timeMenu->setAnchorPoint(ccp(0, 1));
        timeMenu->setScale(screenSize.height / spriteSize.width * 0.275f);
        timeMenu->setPosition(ccp(0, screenSize.height - 140.0f));
    }
    this->addChild(timeMenu, 1);
    
    // create the four weeks labels
    firstWeekLabel = CCSprite::create("time-indicator-week-1.png");
    firstWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        firstWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25f);
        firstWeekLabel->setPosition(ccp(screenSize.width - 295.0f, screenSize.height - 20.0f));
    }
    else
    {
        firstWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        firstWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 295.0f, screenSize.height - 140.0f));
    }
    this->addChild(firstWeekLabel, 2);
    
    secondWeekLabel = CCSprite::create("time_indicator-week-2.png");
    secondWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        secondWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25);
        secondWeekLabel->setPosition(ccp(screenSize.width - 295.0f, screenSize.height - 20.0f));
    }
    else
    {
        secondWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        secondWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 295.0f, screenSize.height - 140.0f));
    }
    this->addChild(secondWeekLabel, 2);
    
    thirdWeekLabel = CCSprite::create("time_indicator-week-3.png");
    thirdWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        thirdWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25f);
        thirdWeekLabel->setPosition(ccp(screenSize.width - 295.0f, screenSize.height - 20.0f));
    }
    else
    {
        thirdWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        thirdWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 295.0f, screenSize.height - 140.0f));
    }
    this->addChild(thirdWeekLabel, 2);
    
    lastWeekLabel = CCSprite::create("time_indicator-week-4.png");
    lastWeekLabel->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        lastWeekLabel->setScale(screenSize.width / spriteSize.width * 0.25f);
        lastWeekLabel->setPosition(ccp(screenSize.width - 295.0f, screenSize.height - 20.0f));
    }
    else
    {
        lastWeekLabel->setScale(screenSize.height / spriteSize.width * 0.25f);
        lastWeekLabel->setPosition(ccp(timeMenu->boundingBox().size.width - 295.0f, screenSize.height - 140.0f));
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
    timeLabel_1 = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
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
    timeLabel_2 = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
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
}

void GameHUD::createObjectiveMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    bool isHori = GlobalHelper::isHorizontal();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorWhite = ccc3(255, 255, 255);
    ccColor3B colorGreen = ccc3(0, 255, 0);
    ccColor3B colorRed = ccc3(255, 0, 0);
    
    // create the objective group background
    string objectiveBackground = "objective.png";
    objectiveMenu = CCSprite::create(objectiveBackground.c_str());
    CCSize spriteSize = objectiveMenu->getContentSize();
    objectiveMenu->setVisible(false);
    objectiveMenu->setAnchorPoint(ccp(0, 1));
    if(isHori)
    {
        objectiveMenu->setScale(screenSize.width / spriteSize.width * 0.55f);
        objectiveMenu->setPosition(ccp(0, screenSize.height - 120.0f));
    }
    else
    {
        objectiveMenu->setScale(screenSize.height / spriteSize.width * 0.55f);
        objectiveMenu->setPosition(ccp(0, screenSize.height - 240.0f));
    }
    
    
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
    
    // create the objective title and objective strings!
    stringstream ss;
    ss << "Objective";
    objectiveTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    objectiveTitle->setAnchorPoint(ccp(0.5, 0));
    objectiveTitle->setPosition(ccp((objectiveMenu->boundingBox().size.width - 60) / 2.0f + 60, screenSize.height - 240));
    
    CCArray* allObjectives = GameScene::getThis()->objectiveHandler->objectives;
    for (int i = 0; i < allObjectives->count(); i++)
    {
        Objective* objective = (Objective*) allObjectives->objectAtIndex(i);
        std::string objectiveStr = objective->getObjectiveString();
        CCLabelTTF* tempLabel = CCLabelTTF::create(objectiveStr.c_str(), "Shojumaru-Regular", 16, CCSizeMake(objectiveStr.length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
        tempLabel->setAnchorPoint(ccp(0, 0));
        tempLabel->setPosition(ccp(75.0f, screenSize.height - 280 - 20.0f * i));
        objectiveStrs->addObject(tempLabel);
        this->addChild(tempLabel, 3);
        if(objective->complete)
        {
            tempLabel->setColor(colorGreen);
        }
    }
    
    this->addChild(menu_objective, 1);
    this->addChild(objectiveMenu, 2);
    this->addChild(objectiveTitle, 3);
    
    objectiveTitle->setVisible(false);
    for (int i = 0; i < objectiveStrs->count(); i++)
    {
        CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
        objectiveStr->setVisible(false);
    }
}

void GameHUD::clickObjectiveButton()
{
    if(objectiveMenu->isVisible())
    {
        objectiveMenu->setVisible(false);
        objectiveTitle->setVisible(false);
        for (int i = 0; i < objectiveStrs->count(); i++)
        {
            CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
            objectiveStr->setVisible(false);
        }
    }
    else
    {
        objectiveMenu->setVisible(true);
        objectiveTitle->setVisible(true);
        for (int i = 0; i < objectiveStrs->count(); i++)
        {
            CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
            objectiveStr->setVisible(true);
        }
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
    buildButton->setAnchorPoint(ccp(1, 0));
    buildButton->setTag(-1);
    buildButton->setPosition(ccp(screenSize.width, 0));
    
    menuItems_build->addObject(buildButton);
    menu_build = CCMenu::createWithArray(menuItems_build);
    menu_build->setAnchorPoint(ccp(1, 1));
    menu_build->setPosition(CCPointZero);
    
    this->addChild(menu_build, 2);
}

void GameHUD::clickBuildButton()
{
    if(BuildScroll::getThis() == NULL)
    {
        if(currTapMode == Build && GameScene::getThis()->buildingHandler->selectedBuilding != NULL)
        {
            money += GameScene::getThis()->buildingHandler->selectedBuilding->buildingCost;
        }
        buildButton->setVisible(false);
        currTapMode = Normal;
        GameScene::getThis()->buildingHandler->selectedBuilding = NULL;
        GameScene::getThis()->mapHandler->UnBuildPreview();
        GameScene::getThis()->mapHandler->UnPathPreview();
        GameScene::getThis()->isOpenBuildScroll = true;
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

void GameHUD::addReputation(int incre)
{
    reputation += incre;
    if(reputation >= reputationMax)
    {
        reputation = reputationMax;
    }
    
    int projectedPopulationGrowth = GameScene::getThis()->settingsLevel->projected_population_growth;
    float baseValue = powf((float) projectedPopulationGrowth / (float) 3, (float) 1 / (float) reputationMax);
    
    int projectedPopulation = (int) (3 * pow(baseValue, reputationMax) - 3 * pow(baseValue, (reputationMax - reputation)));
    
    if(growthPopulation < projectedPopulation)
    {
        growthPopulation++;
        addPopulation();
    }
}

void GameHUD::addPopulation(){
    CCPoint target = CCPointMake(29,33);
    
    GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE);
    
    /*
    int isMale = rand() % 2;
    
    if(isMale)
    {
        GameScene::getThis()->spriteHandler->addSpriteToMap(target, M_REFUGEE);
    }
    else
    {
        GameScene::getThis()->spriteHandler->addSpriteToMap(target, F_REFUGEE);
    }
    */
}

void GameHUD::createSystemMenu()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    menuItems_pause = CCArray::create();
    menuItems_pause->retain();
    
    pauseButton = CCMenuItemImage::create("pauseIcon.png", "pauseIcon.png", this, menu_selector(GameHUD::pauseGame));
    resumeButton = CCMenuItemImage::create("nextButton.png", "nextButton.png", this, menu_selector(GameHUD::pauseGame));
    systemButton = CCMenuItemImage::create("system.png", "system.png", this, menu_selector(GameHUD::clickSystemButton));
    stickHappinessButton = CCMenuItemImage::create("happyFace.png", "happyFace.png", this, menu_selector(GameHUD::stickGameHappiness));
    resumeHappinessButton = CCMenuItemImage::create("normalFace.png", "normalFace.png", this, menu_selector(GameHUD::stickGameHappiness));
    warButton = CCMenuItemImage::create("banditicon.png", "banditicon.png", this, menu_selector(GameHUD::banditsAttack));
    peaceButton = CCMenuItemImage::create("peaceicon.png", "peaceicon.png", this, menu_selector(GameHUD::banditsAttack));
    
    pauseButton->setScale(screenSize.width / pauseButton->boundingBox().size.width * 0.05f);
    pauseButton->setAnchorPoint(ccp(1, 1));
    pauseButton->setPosition(ccp(screenSize.width - 50.0f, screenSize.height - 125.0f));
    
    resumeButton->setScale(screenSize.width / resumeButton->boundingBox().size.width * 0.05f);
    resumeButton->setAnchorPoint(ccp(1, 1));
    resumeButton->setPosition(ccp(screenSize.width - 50.0f, screenSize.height - 125.0f));
    
    systemButton->setScale(screenSize.width / systemButton->boundingBox().size.width * 0.05f);
    systemButton->setAnchorPoint(ccp(1, 1));
    systemButton->setPosition(ccp(screenSize.width, screenSize.height - 125.0f));
    
    stickHappinessButton->setScale(screenSize.width / stickHappinessButton->boundingBox().size.width * 0.05f);
    stickHappinessButton->setAnchorPoint(ccp(1, 1));
    stickHappinessButton->setPosition(ccp(screenSize.width - 100.0f, screenSize.height - 125.0f));
    
    resumeHappinessButton->setScale(screenSize.width / resumeHappinessButton->boundingBox().size.width * 0.05f);
    resumeHappinessButton->setAnchorPoint(ccp(1, 1));
    resumeHappinessButton->setPosition(ccp(screenSize.width - 100.0f, screenSize.height - 125.0f));
    
    warButton->setScale(screenSize.width / warButton->boundingBox().size.width * 0.05f);
    warButton->setAnchorPoint(ccp(0, 0));
    warButton->setPosition(ccp(0, 0));
    
    peaceButton->setScale(screenSize.width / peaceButton->boundingBox().size.width * 0.05f);
    peaceButton->setAnchorPoint(ccp(0, 0));
    peaceButton->setPosition(ccp(0, 0));
    
    resumeButton->setVisible(false);
    resumeHappinessButton->setVisible(false);
    peaceButton->setVisible(false);
    
    menuItems_pause->addObject(stickHappinessButton);
    menuItems_pause->addObject(resumeHappinessButton);
    menuItems_pause->addObject(systemButton);
    menuItems_pause->addObject(pauseButton);
    menuItems_pause->addObject(resumeButton);
    menuItems_pause->addObject(warButton);
    menuItems_pause->addObject(peaceButton);
    
    menu_pause = CCMenu::createWithArray(menuItems_pause);
    menu_pause->setAnchorPoint(ccp(0.5, 0.5));
    menu_pause->setPosition(CCPointZero);
    
    this->addChild(menu_pause, 5);
}

void GameHUD::banditsAttack()
{
    if(startWar)
    {
        startWar = false;
        peaceButton->setVisible(false);
        warButton->setVisible(true);
        GameScene::getThis()->banditsAttackHandler->stopWar();
    }
    else
    {
        startWar = true;
        peaceButton->setVisible(true);
        warButton->setVisible(false);
        GameScene::getThis()->banditsAttackHandler->startWar();
    }
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

void GameHUD::stickGameHappiness()
{
    if(stickHappiness)
    {
        stickHappiness = false;
        stickHappinessButton->setVisible(true);
        resumeHappinessButton->setVisible(false);
    }
    else
    {
        stickHappiness = true;
        stickHappinessButton->setVisible(false);
        resumeHappinessButton->setVisible(true);
    }
}

void GameHUD::clickSystemButton()
{
    GameScene::getThis()->mapHandler->UnBuildEndGame();
    CCDirector::sharedDirector()->pushScene(MainMenuScene::scene());
}