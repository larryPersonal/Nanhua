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
#include "SystemMenu.h"
#include "TutorialManager.h"

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
    
    leftPos = 0;
    maxPos = 100.0f;
    
    scroll_in = false;
    scroll_out = false;
    
    scrolled_in = false;
    
    setTutorial = false;
    
    fade_out = false;
    fade_in = false;
    
    labelStatus = Default;
    isToggle = false;
    newPos = CCPointZero;
    targetOpacity = 0;
    fadeSpeed = 0;
    label_fade_in = false;
    label_fade_out = false;
    
    targetMoney = 0;
    isAddingMoney = false;
    moneyLabelDirectionUp = false;
    
    targetReputation = 0;
    isAddingReputation = false;
    
    targetFood = 0;
    isAddingFood = false;
    foodLabelDirectionUp = false;
    targetStorage = 0;
    isAddingStorage = false;
    storageLabelDirectionUp = false;
    
    addMoneyLabelArray = CCArray::create();
    addMoneyLabelArray->retain();
    
    addReputationLabelArray = CCArray::create();
    addReputationLabelArray->retain();
    
    addFoodLabelArray = CCArray::create();
    addFoodLabelArray->retain();
    
    addStorageLabelArray = CCArray::create();
    addStorageLabelArray->retain();
    
    isAlerting = false;
    isAlertingText = false;
    alertFadeIn = false;
    alertTextFadeIn = false;
    alertCumulativeTime = 0;
}

GameHUD::~GameHUD()
{
    delete date;

    if (SystemMenu::SP != NULL)
    {
        CC_SAFE_RELEASE(SystemMenu::SP);
    }
    
    addMoneyLabelArray->removeAllObjects();
    CC_SAFE_RELEASE(addMoneyLabelArray);
    
    addReputationLabelArray->removeAllObjects();
    CC_SAFE_RELEASE(addReputationLabelArray);
    
    addFoodLabelArray->removeAllObjects();
    CC_SAFE_RELEASE(addFoodLabelArray);
    
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
    
    //this->CCLayer::setTouchEnabled(true);
    
    setAllStats();
    createInitialGUI();
    
    return true;
}

void GameHUD::setAllStats()
{
    mGameCurrentFood = 0;
    mGameCurrentStorage = 0;
    
    mGameCurrentPopulation = 0;
    mGameCurrentPopulationRoom = 0;
}

void GameHUD::update(float deltaTime)
{
    
    if(!pause)
    {
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
            scheduleAddMoney(100 * GameScene::getThis()->buildingHandler->housingOnMap->count());
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
                    firstWeekLabel->setOpacity(255);
                    secondWeekLabel->setOpacity(0);
                    thirdWeekLabel->setOpacity(0);
                    lastWeekLabel->setOpacity(0);
                    break;
                case 1:
                    firstWeekLabel->setOpacity(0);
                    secondWeekLabel->setOpacity(255);
                    thirdWeekLabel->setOpacity(0);
                    lastWeekLabel->setOpacity(0);
                    break;
                case 2:
                    firstWeekLabel->setOpacity(0);
                    secondWeekLabel->setOpacity(0);
                    thirdWeekLabel->setOpacity(255);
                    lastWeekLabel->setOpacity(0);
                    break;
                case 3:
                    firstWeekLabel->setOpacity(0);
                    secondWeekLabel->setOpacity(0);
                    thirdWeekLabel->setOpacity(0);
                    lastWeekLabel->setOpacity(255);
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
                timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock0.png"));
                
            }
            else if(date->month < 6)
            {
                //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_summer-bg.png"));
                timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock0.png"));
                
            }
            else if(date->month < 9)
            {
                //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_autumn-bg.png"));
                timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock0.png"));
                
            }
            else
            {
                //timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("time_winter-bg.png"));
                timeMenu->setTexture(CCTextureCache::sharedTextureCache()->addImage("timeclock0.png"));
                
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
    
    // reputation change
    if(mGameReputation != reputation || mGameReputationMax != reputationMax)
    {
        mGameReputation = reputation;
        mGameReputationMax = reputationMax;
        
        std::stringstream ss;
        ss << mGameReputation;
        
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
        if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_CLASS_END)
        {
            temp++;
        }
    }
    
    if(mGameCurrentPopulation != temp)
    {
        mGameCurrentPopulation = temp;
        std::stringstream ss;
        ss << mGameCurrentPopulation << "/" << mGameCurrentPopulationRoom;
        populationLabel->setString(ss.str().c_str());
        if(mGameCurrentPopulation > mGameCurrentPopulationRoom)
        {
            populationLabel->setColor(ccc3(255, 0, 0));
        }
        else
        {
            populationLabel->setColor(ccc3(255, 255, 255));
        }
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
        ss << mGameCurrentPopulation << "/" << mGameCurrentPopulationRoom;
        populationLabel->setString(ss.str().c_str());
        if(mGameCurrentPopulation > mGameCurrentPopulationRoom)
        {
            populationLabel->setColor(ccc3(255, 0, 0));
        }
        else
        {
            populationLabel->setColor(ccc3(255, 255, 255));
        }
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
        UpdateBuildButton();

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
    
    if(isAddingFood)
    {
        if(targetFood != temp)
        {
            scheduleAddFood(temp - targetFood);
        }
    }
    else
    {
        if(mGameCurrentFood != temp)
        {
            scheduleAddFood(temp - mGameCurrentFood);
        }
    }
    
    if(isAddingStorage)
    {
        if(targetStorage != temp_storage)
        {
            scheduleAddStorage(temp_storage - targetStorage);
        }
    }
    else
    {
        if(mGameCurrentStorage != temp_storage)
        {
            scheduleAddStorage(temp_storage - mGameCurrentStorage);
        }
    }
    
    /*
    if (mGameCurrentFood != temp || mGameCurrentStorage != temp_storage)
    {
        mGameCurrentFood = temp;
        mGameCurrentStorage = temp_storage;
        
        stringstream ss;
        ss << mGameCurrentFood << "/" << mGameCurrentStorage;
        foodLabel->setDimensions(CCSizeMake(ss.str().length() * 20.0f, 5.0f));
        foodLabel->setString(ss.str().c_str());
    }
    */
    
    updateSoldierHelper(deltaTime);
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
    createSoldierHelper();
    createAlertSystem();
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
    
    // create the background of the stats menu
    statsMenu = CCSprite::create("chargirl_represent.png");
    CCSize spriteSize = statsMenu->getContentSize();
    statsMenu->setScale(screenSize.width / spriteSize.width * 0.25f);

    statsMenu->setAnchorPoint(ccp(0, 1));
    statsMenu->setPosition(ccp(0, screenSize.height));
    this->addChild(statsMenu, 1);
    
    // create the money indicator
    moneyIcon = CCSprite::create("yuanbao_amount.png");
    moneyIcon->cocos2d::CCNode::setScale(screenSize.width / spriteSize.width * 0.28f, screenSize.width / spriteSize.width * 0.25f);

    moneyIcon->setAnchorPoint(ccp(0, 1));
    moneyIcon->setPosition(ccp(120, screenSize.height - 8));
    this->addChild(moneyIcon, 2);
    
    std::stringstream ss;
    ss << money << "G";
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14);
    moneyLabel->setColor(colorWhite);
    moneyLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(moneyLabel, 2);
    moneyLabel->CCNode::setPosition(285, screenSize.height - 26);
    
    // create the food indicator
    foodIcon = CCSprite::create("rice_amount.png");
    foodIcon->cocos2d::CCNode::setScale(screenSize.width / spriteSize.width * 0.32f, screenSize.width / spriteSize.width * 0.25f);

    foodIcon->setAnchorPoint(ccp(0, 1));
    foodIcon->setPosition(ccp(350, screenSize.height));
    this->addChild(foodIcon, 2);
    
    mGameCurrentFood = 0;
    mGameCurrentStorage = 0;
    CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
    for (int i = 0; i < allGranary->count(); i++)
    {
        Building* b = (Building*) allGranary->objectAtIndex(i);
        mGameCurrentFood += b->currentStorage;
        mGameCurrentStorage += b->storageLimit;
    }
    ss.str(std::string());
    ss << mGameCurrentFood << "/" << mGameCurrentStorage;
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14);
    foodLabel->setColor(colorWhite);
    foodLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(foodLabel, 2);
    foodLabel->CCNode::setPosition(495, screenSize.height - 28);
    
    // create the population indicator
    populationIcon = CCSprite::create("population_amount1.png");
    populationIcon->cocos2d::CCNode::setScale(screenSize.width / spriteSize.width * 0.24f, screenSize.width / spriteSize.width * 0.25f);

    populationIcon->setAnchorPoint(ccp(0, 1));
    populationIcon->setPosition(ccp(565, screenSize.height - 2));
    this->addChild(populationIcon, 2);
    
    mGameCurrentPopulation = 0;
    mGameCurrentPopulationRoom = 0;
    
    CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
    for(int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
        if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_CLASS_END)
        {
            mGameCurrentPopulation++;
        }
    }
    
    CCArray* allHousing = GameScene::getThis()->buildingHandler->housingOnMap;
    for(int i = 0; i < allHousing->count(); i++)
    {
        Building* b = (Building*) allHousing->objectAtIndex(i);
        mGameCurrentPopulationRoom += b->populationLimit;
    }
    
    ss.str(std::string());
    ss << mGameCurrentPopulation << "/" << mGameCurrentPopulationRoom;
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14);
    populationLabel->setColor(colorWhite);
    populationLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(populationLabel, 2);
    populationLabel->CCNode::setPosition(700, screenSize.height - 28);
    
    // create the achievements label for the values
    ss.str(std::string());
    ss << GameScene::getThis()->configSettings->default_ini_reputation;
    achivementsLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18);
    achivementsLabel->setColor(colorBlack);
    achivementsLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(achivementsLabel, 2);
    achivementsLabel->CCNode::setPosition(180, screenSize.height - 76);
    
    // showing the average happiness attribute
    average_happiness = 0;
    mAverageHappiness = average_happiness;
    
    ss.str(std::string());
    ss << mAverageHappiness;
    
    average_happiness_label = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24);
    average_happiness_label->setColor(colorBlack);
    average_happiness_label->setAnchorPoint(ccp(0, 0));
    this->addChild(average_happiness_label, 2);
    average_happiness_label->CCNode::setPosition(620, 0);
    
    infoBackground = CCSprite::create("infor display ui.png");
    infoBackground->setScale(screenSize.width / infoBackground->boundingBox().size.width * 0.25f);
    infoBackground->setAnchorPoint(ccp(0.5, 1));
    infoBackground->setPosition(ccp(220, screenSize.height));
    this->addChild(infoBackground, 3);
    
    float opacity = 0;
    infoBackground->setOpacity(opacity);
    
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
    
    // create the time group background
    string timeBG = "timeclock0.png";
    timeMenu = CCSprite::create(timeBG.c_str());
    CCSize spriteSize = timeMenu->getContentSize();
    timeMenu->setAnchorPoint(ccp(1, 1));
    timeMenu->setScale(screenSize.width / spriteSize.width * 0.125f);
    timeMenu->setPosition(ccp(screenSize.width - leftPos, screenSize.height - 2.0f));
    
    this->addChild(timeMenu, 2);
    
    timeBackground = CCSprite::create("timeclock1.png");
    spriteSize = timeBackground->getContentSize();
    timeBackground->setAnchorPoint(ccp(1, 1));
    timeBackground->setScale(screenSize.width / spriteSize.width * 0.18f);
    timeBackground->setPosition(ccp(screenSize.width + maxPos - leftPos, screenSize.height));
    
    this->addChild(timeBackground, 1);
    
    // create the four weeks labels
    firstWeekLabel = CCSprite::create("timeclock_week1.png");
    firstWeekLabel->setAnchorPoint(ccp(1, 1));
    firstWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.13f);
    firstWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));

    this->addChild(firstWeekLabel, 3);
    
    secondWeekLabel = CCSprite::create("timeclock_week2.png");
    secondWeekLabel->setAnchorPoint(ccp(1, 1));
    secondWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.13f);
    secondWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));

    this->addChild(secondWeekLabel, 3);
    
    thirdWeekLabel = CCSprite::create("timeclock_week3.png");
    thirdWeekLabel->setAnchorPoint(ccp(1, 1));
    thirdWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.125f);
    thirdWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));

    this->addChild(thirdWeekLabel, 3);
    
    lastWeekLabel = CCSprite::create("timeclock_week4.png");
    lastWeekLabel->setAnchorPoint(ccp(1, 1));
    lastWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.125f);
    lastWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));

    this->addChild(lastWeekLabel, 3);
    
    firstWeekLabel->setOpacity(255);
    secondWeekLabel->setOpacity(0);
    thirdWeekLabel->setOpacity(0);
    lastWeekLabel->setOpacity(0);
    
    // save the current date
    mGameWeek = date->week;
    mGameMonth = date->month;
    mGameYear = date->year;
    
    // display the date in the time menu
    std::stringstream ss;
    ss << "Year: " << (date->year + 1);
    timeLabel_1 = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    timeLabel_1->setColor(colorBlack);
    timeLabel_1->setAnchorPoint(ccp(0.5, 1));
    this->addChild(timeLabel_1, 3);
    timeLabel_1->CCNode::setPosition(screenSize.width - 30.0f + maxPos - leftPos, screenSize.height - 25.0f);

    ss.str(std::string());
    ss << "Month: " << (date->month + 1);
    timeLabel_2 = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    timeLabel_2->setColor(colorBlack);
    timeLabel_2->setAnchorPoint(ccp(0.5, 1));
    this->addChild(timeLabel_2, 3);
    timeLabel_2->CCNode::setPosition(screenSize.width - 28.0f + maxPos - leftPos, screenSize.height - 55.0f);
    
    timeBackground->setVisible(false);
    timeLabel_1->setVisible(false);
    timeLabel_2->setVisible(false);
}

void GameHUD::timeGroupReposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    timeMenu->setPosition(ccp(screenSize.width - leftPos, screenSize.height - 2.0f));
    timeBackground->setPosition(ccp(screenSize.width + maxPos - leftPos, screenSize.height));
    firstWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));
    secondWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));
    thirdWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));
    lastWeekLabel->setPosition(ccp(screenSize.width + 1.5f - leftPos, screenSize.height - 0.5f));
    timeLabel_1->CCNode::setPosition(screenSize.width - 30.0f + maxPos - leftPos, screenSize.height - 25.0f);
    timeLabel_2->CCNode::setPosition(screenSize.width - 28.0f + maxPos - leftPos, screenSize.height - 55.0f);
    
    int temp = leftPos / maxPos * 255;
    if(temp < 0)
    {
        temp = 0;
    }
    else if(temp > 255)
    {
        temp = 255;
    }
    
    GLubyte opacity = temp;
    
    timeBackground->setOpacity(opacity);
    timeLabel_1->setOpacity(opacity);
    timeLabel_2->setOpacity(opacity);
}

void GameHUD::scrollIn(float dt)
{
    if(scroll_in)
    {
        float speed = 5;
        
        leftPos += speed;
        
        if(leftPos >= maxPos)
        {
            leftPos = maxPos;
            this->unschedule(schedule_selector( GameHUD::scrollIn ));
            scroll_in = false;
            scrolled_in = true;
        }
        
        timeGroupReposition();
    }
}

void GameHUD::scrollOut(float dt)
{
    if(scroll_out)
    {
        float speed = 5;
        
        leftPos -= speed;
        
        if(leftPos <= 0)
        {
            leftPos = 0;
            this->unschedule(schedule_selector( GameHUD::scrollOut ));
            scroll_out = false;
            timeBackground->setVisible(false);
            timeLabel_1->setVisible(false);
            timeLabel_2->setVisible(false);
            scrolled_in = false;
        }
        
        timeGroupReposition();
    }
}

void GameHUD::scheduleScrollIn()
{
    if(!scroll_in && !scroll_out && leftPos <= 0)
    {
        scroll_in = true;
        timeBackground->setVisible(true);
        timeBackground->setOpacity(0);
        timeLabel_1->setVisible(true);
        timeLabel_1->setOpacity(0);
        timeLabel_2->setVisible(true);
        timeLabel_2->setOpacity(0);
        this->schedule(schedule_selector( GameHUD::scrollIn ), 1.0f / 120.0f);
    }
}

void GameHUD::scheduleScrollOut()
{
    if(!scroll_in && !scroll_out)
    {
        scroll_out = true;
        this->schedule(schedule_selector( GameHUD::scrollOut ), 1.0f / 120.0f);
    }
}

void GameHUD::createObjectiveMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    
    // create the objective group background
    string objectiveBackground = "objective.png";
    objectiveMenu = CCSprite::create(objectiveBackground.c_str());
    CCSize spriteSize = objectiveMenu->getContentSize();
    objectiveMenu->setVisible(false);
    objectiveMenu->setAnchorPoint(ccp(0.0, 1.0));
    objectiveMenu->setScale(screenSize.width / spriteSize.width * 0.35f);
    objectiveMenu->setPosition(ccp(screenSize.width * 0.015f - 1000, screenSize.height - 115.0f));
    
    // create the objective button
    objectiveButton = CCSprite::create("objective-menu-button_06.png");
    objectiveButton->setAnchorPoint(ccp(0, 1));
    objectiveButton->setScale(screenSize.width / spriteSize.width * 0.35f);
    objectiveButton->setPosition(ccp(15, screenSize.height - 113));
    this->addChild(objectiveButton);
    
    // create the objective title and objective strings!
    stringstream ss;
    ss << "Objective";
    objectiveTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 24);
    objectiveTitle->setAnchorPoint(ccp(0.5, 0));
    objectiveTitle->setColor(colorBlack);
    objectiveTitle->setPosition(ccp(screenSize.width * 0.28f + objectiveMenu->boundingBox().size.width / 2.0f, screenSize.height - 480));
    
    ss.str(std::string());
    ss << "Description";
    objectiveDescription = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20);
    objectiveDescription->setAnchorPoint(ccp(0, 1));
    objectiveDescription->setColor(colorBlack);
    objectiveDescription->setPosition(ccp(screenSize.width * 0.14f, screenSize.height - 540));
    
    ss.str(std::string());
    ss << "Progress";
    objectiveProgress = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20);
    objectiveProgress->setAnchorPoint(ccp(0, 1));
    objectiveProgress->setColor(colorBlack);
    objectiveProgress->setPosition(ccp(screenSize.width * 0.14f, screenSize.height - 600));
    
    objectiveMenu->addChild(objectiveTitle);
    objectiveMenu->addChild(objectiveDescription);
    objectiveMenu->addChild(objectiveProgress);
    
    this->addChild(objectiveMenu, 6);
}

void GameHUD::fadeIn(float dt)
{
    if(fade_in)
    {
        float speed = 20;
        float opacity = ((float)objectiveMenu->getOpacity()) + speed;
        
        if(opacity >= 255)
        {
            opacity = 255;
            this->unschedule(schedule_selector( GameHUD::fadeIn ));
            fade_in = false;
        }
        objectiveMenu->setOpacity((GLubyte) opacity);
        objectiveTitle->setOpacity((GLubyte) opacity);
        objectiveDescription->setOpacity((GLubyte) opacity);
        objectiveProgress->setOpacity((GLubyte) opacity);
    }
}

void GameHUD::fadeOut(float dt)
{
    if(fade_out)
    {
        float speed = 20;
        float opacity = ((float) objectiveMenu->getOpacity()) - speed;
        
        if(opacity <= 0)
        {
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            opacity = 0;
            this->unschedule(schedule_selector( GameHUD::fadeOut ));
            fade_out = false;
            objectiveMenu->setVisible(false);
            objectiveMenu->setPosition(ccp(screenSize.width * 0.015f - 1000, screenSize.height - 115.0f));
            objectiveTitle->setVisible(false);
        }
        objectiveMenu->setOpacity((GLubyte) opacity);
        objectiveTitle->setOpacity((GLubyte) opacity);
        objectiveDescription->setOpacity((GLubyte) opacity);
        objectiveProgress->setOpacity((GLubyte) opacity);
    }
}

void GameHUD::clickObjectiveButton()
{
    if(!fade_in && !fade_out)
    {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        if(objectiveMenu->isVisible())
        {
            objectiveMenu->setOpacity((GLubyte) 255);
            objectiveTitle->setOpacity((GLubyte) 255);
            objectiveDescription->setOpacity((GLubyte) 255);
            objectiveProgress->setOpacity((GLubyte) 255);
            fade_out = true;
            this->schedule(schedule_selector( GameHUD::fadeOut ), 1.0f / 240.0f);
        }
        else
        {
            objectiveMenu->setOpacity((GLubyte) 0);
            objectiveMenu->setVisible(true);
            objectiveTitle->setOpacity((GLubyte) 0);
            objectiveTitle->setVisible(true);
            objectiveDescription->setOpacity((GLubyte) 0);
            objectiveProgress->setOpacity((GLubyte) 0);
            objectiveMenu->setPosition(ccp(screenSize.width * 0.015f, screenSize.height - 115.0f));
            fade_in = true;
            this->schedule(schedule_selector( GameHUD::fadeIn ), 1.0f / 240.0f);
        }
    }
}



void GameHUD::createBuildMenu()
{
    // set common variables
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // create the build button
    buildButton = CCSprite::create("main-game-buttons_build.png");
    buildButton->setScale(0.3f);

    buildButton->setAnchorPoint(ccp(1, 0));
    buildButton->setPosition(ccp(screenSize.width, -screenSize.height * 0.025f));
    
    this->addChild(buildButton, 2);
    
    mask = CCSprite::create("black.png");
    mask->setScale(screenSize.width / mask->boundingBox().size.width);
    mask->setAnchorPoint(ccp(0.5, 0.5));
    mask->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    mask->setOpacity((GLubyte) 0);
    this->addChild(mask, 10);
}




void GameHUD::clickBuildButton()
{
    if(BuildScroll::getThis() == NULL)
    {
        if(TutorialManager::getThis()->active && (TutorialManager::getThis()->teachBuildButton || TutorialManager::getThis()->teachBuildRoad))
        {
            TutorialManager::getThis()->lockBuildScroll = true;
            TutorialManager::getThis()->miniDragon->move(ccp(0, 220));
            if(TutorialManager::getThis()->teachBuildButton)
            {
                TutorialManager::getThis()->teachBuildButton = false;
                TutorialManager::getThis()->teachBuildHouse = true;
            }
            TutorialManager::getThis()->miniDragon->clickNext();
        }
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        if(currTapMode == Build && GameScene::getThis()->buildingHandler->selectedBuilding != NULL)
        {
            GameHUD::getThis()->scheduleAddMoney(GameScene::getThis()->buildingHandler->selectedBuilding->buildingCost);
        }
        buildButton->setVisible(false);
        currTapMode = Normal;
        GameScene::getThis()->buildingHandler->selectedBuilding = NULL;
        GameScene::getThis()->mapHandler->UnBuildPreview();
        GameScene::getThis()->mapHandler->UnPathPreview();
        buildScroll = BuildScroll::create();
        buildScroll->useAsTopmostPopupMenu();
        buildButton->setPosition(ccp(screenSize.width + buildButton->boundingBox().size.width, 0));
        
        buildScroll->cocos2d::CCNode::setZOrder(30);
    }
    else
    {
        BuildScroll::getThis()->closeMenu(true);
        buildScroll = NULL;
    }
}

/*
 PS: reputation max is associated to the population growth, need to settle how to raise the growth of population and the reputation max with kris again.
 
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
*/

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
    
    pauseButton = CCSprite::create("pauseIcon.png");
    resumeButton = CCSprite::create("nextButton.png");
    systemButton = CCSprite::create("optionmenubutton.png");
    stickHappinessButton = CCSprite::create("happyFace.png");
    resumeHappinessButton = CCSprite::create("normalFace.png");
    warButton = CCSprite::create("banditicon.png");
    peaceButton = CCSprite::create("peaceicon.png");
    
    pauseButton->setScale(screenSize.width / pauseButton->boundingBox().size.width * 0.05f);
    pauseButton->setAnchorPoint(ccp(1, 1));
    pauseButton->setPosition(ccp(screenSize.width - 50.0f, screenSize.height - 125.0f));
    
    resumeButton->setScale(screenSize.width / resumeButton->boundingBox().size.width * 0.05f);
    resumeButton->setAnchorPoint(ccp(1, 1));
    resumeButton->setPosition(ccp(screenSize.width + 450.0f, screenSize.height - 125.0f));
    
    systemButton->setScale(screenSize.width / systemButton->boundingBox().size.width * 0.05f);
    systemButton->setAnchorPoint(ccp(1, 1));
    systemButton->setPosition(ccp(screenSize.width * 0.075f, screenSize.height - 205.0f));
    
    stickHappinessButton->setScale(screenSize.width / stickHappinessButton->boundingBox().size.width * 0.05f);
    stickHappinessButton->setAnchorPoint(ccp(1, 1));
    stickHappinessButton->setPosition(ccp(screenSize.width - 100.0f, screenSize.height - 125.0f));
    
    resumeHappinessButton->setScale(screenSize.width / resumeHappinessButton->boundingBox().size.width * 0.05f);
    resumeHappinessButton->setAnchorPoint(ccp(1, 1));
    resumeHappinessButton->setPosition(ccp(screenSize.width + 400.0f, screenSize.height - 125.0f));
    
    warButton->setScale(screenSize.width / warButton->boundingBox().size.width * 0.05f);
    warButton->setAnchorPoint(ccp(0, 0));
    warButton->setPosition(ccp(0, 0));
    
    peaceButton->setScale(screenSize.width / peaceButton->boundingBox().size.width * 0.05f);
    peaceButton->setAnchorPoint(ccp(0, 0));
    peaceButton->setPosition(ccp(0, -500));
    
    resumeButton->setVisible(false);
    resumeHappinessButton->setVisible(false);
    peaceButton->setVisible(false);
    
    this->addChild(stickHappinessButton, 5);
    this->addChild(resumeHappinessButton, 5);
    this->addChild(systemButton, 5);
    this->addChild(pauseButton, 5);
    this->addChild(resumeButton, 5);
    this->addChild(warButton, 5);
    this->addChild(peaceButton, 5);
}

void GameHUD::banditsAttack()
{
    if(!isAlerting && !isAlertingText)
    {
        /*
        if(startWar)
        {
            startWar = false;
             peaceButton->setVisible(false);
             peaceButton->setPosition(ccp(peaceButton->getPositionX(), peaceButton->getPositionY() - 500));
             warButton->setVisible(true);
             warButton->setPosition(ccp(warButton->getPositionX(), warButton->getPositionY() + 500));
             GameScene::getThis()->banditsAttackHandler->stopWar();
        }
        else
        {
            startWar = true;
        */
            redMask->setOpacity((GLubyte) 0);
            redMask->setVisible(true);
            alertText->setScale(0);
            alertText->setVisible(true);
            isAlerting = true;
            isAlertingText = true;
            alertFadeIn = true;
            alertTextFadeIn = true;
            alertCumulativeTime = 0;
            this->schedule(schedule_selector(GameHUD::alertBanditsAttackFade), 1.0f / 120.0f);
            this->schedule(schedule_selector(GameHUD::alertBanditsAttackText), 1.0f / 120.0f);
            /*
             peaceButton->setVisible(true);
             peaceButton->setPosition(ccp(peaceButton->getPositionX(), peaceButton->getPositionY() + 500));
             warButton->setVisible(false);
             warButton->setPosition(ccp(warButton->getPositionX(), warButton->getPositionY() - 500));
             GameScene::getThis()->banditsAttackHandler->startWar();
             */
        //}
    }
}

void GameHUD::alertBanditsAttackFade(float dt)
{
    if(alertFadeIn)
    {
        float temp = (float) redMask->getOpacity();
        temp += 5.0f;
        if(temp >= 120.0f)
        {
            alertFadeIn = false;
            temp = 120.0f;
        }
        redMask->setOpacity((GLubyte) temp);
    }
    else
    {
        float temp = (float) redMask->getOpacity();
        temp -= 5.0f;
        if(temp <= 0)
        {
            this->unschedule(schedule_selector(GameHUD::alertBanditsAttackFade));
            temp = 0;
            redMask->setVisible(false);
            isAlerting = false;
        }
        redMask->setOpacity((GLubyte) temp);
    }
}

void GameHUD::alertBanditsAttackText(float dt)
{
    if(alertTextFadeIn)
    {
        float scale = (float) alertText->getScale();
        scale += 0.1f;
        if(scale >= 1.0f)
        {
            alertTextFadeIn = false;
            scale = 1.0f;
        }
        alertText->setScale(scale);
    }
    else if(alertCumulativeTime < 1)
    {
        alertCumulativeTime += dt;
    }
    else
    {
        float scale = (float) alertText->getScale();
        scale -= 0.1f;
        if(scale <= 0)
        {
            this->unschedule(schedule_selector(GameHUD::alertBanditsAttackText));
            scale = 0;
            alertText->setVisible(false);
            isAlertingText = false;
        }
        alertText->setScale(scale);
    }
}

void GameHUD::pauseGame()
{
    if(pause)
    {
        pause = false;
        pauseButton->setVisible(true);
        pauseButton->setPosition(ccp(pauseButton->getPositionX() - 500, pauseButton->getPositionY()));
        resumeButton->setVisible(false);
        resumeButton->setPosition(ccp(resumeButton->getPositionX() + 500, resumeButton->getPositionY()));
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
        pauseButton->setPosition(ccp(pauseButton->getPositionX() + 500, pauseButton->getPositionY()));
        resumeButton->setVisible(true);
        resumeButton->setPosition(ccp(resumeButton->getPositionX() - 500, resumeButton->getPositionY()));
    }
}

void GameHUD::stickGameHappiness()
{
    if(stickHappiness)
    {
        stickHappiness = false;
        stickHappinessButton->setVisible(true);
        stickHappinessButton->setPosition(ccp(stickHappinessButton->getPositionX() - 500, stickHappinessButton->getPositionY()));
        resumeHappinessButton->setVisible(false);
        resumeHappinessButton->setPosition(ccp(resumeHappinessButton->getPositionX() + 500, resumeHappinessButton->getPositionY()));
    }
    else
    {
        stickHappiness = true;
        stickHappinessButton->setVisible(false);
        stickHappinessButton->setPosition(ccp(stickHappinessButton->getPositionX() + 500, stickHappinessButton->getPositionY()));
        resumeHappinessButton->setVisible(true);
        resumeHappinessButton->setPosition(ccp(resumeHappinessButton->getPositionX() - 500, resumeHappinessButton->getPositionY()));
    }
}

void GameHUD::clickSystemButton()
{
   SystemMenu* sm = SystemMenu::create(this);
    sm->retain();
    pause = true;
}

void GameHUD::createAlertSystem()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    redMask = CCSprite::create("red.png");
    redMask->cocos2d::CCNode::setScale(screenSize.width / redMask->boundingBox().size.width, screenSize.height / redMask->boundingBox().size.height);
    redMask->setAnchorPoint(ccp(0.5f, 0.5f));
    redMask->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    redMask->setOpacity((GLubyte) 60);
    
    this->addChild(redMask, 100);
    
    alertText = CCLabelTTF::create("Bandits Attack!", "Shojumaru-Regular", 48);
    alertText->setAnchorPoint(ccp(0.5f, 0.5f));
    alertText->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    
    this->addChild(alertText, 101);
    
    redMask->setVisible(false);
    alertText->setVisible(false);
}

void GameHUD::UpdateBuildButton()
{
    if (buildButton == NULL) return;
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
    CCTexture2D* tex;
    if (getTapMode() == 0)
    {
        tex = CCTextureCache::sharedTextureCache()->addImage("main-game-buttons_build.png");
    }
    else
    {
        tex = CCTextureCache::sharedTextureCache()->addImage("main_game_buttons_cancel_build.png");
        
    }
    buildButton->setTexture(tex);
//    CC_SAFE_RELEASE(tex);
}

void GameHUD::createSoldierHelper()
{
    CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
    GameSprite* firstSoldier = NULL;
    for(int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* temp = (GameSprite*) allSprites->objectAtIndex(i);
        if(temp->villagerClass == V_SOLDIER)
        {
            firstSoldier = temp;
            break;
        }
    }
    
    stringstream ss;
    
    if(firstSoldier == NULL)
    {
        ss << "NULL";
    }
    else
    {
        ss << firstSoldier->spriteName;
    }
    
    soldierName = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14);
    soldierName->setAnchorPoint(ccp(0, 0));
    soldierName->setPosition(ccp(50, 0));
    this->addChild(soldierName);
    
    ss.str(string());
    if(firstSoldier == NULL || firstSoldier->enermy == NULL)
    {
        ss << "NULL";
    }
    else
    {
        ss << firstSoldier->enermy->spriteName;
    }
    
    enermyName = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14);
    enermyName->setAnchorPoint(ccp(0, 0));
    enermyName->setPosition(ccp(250, 0));
    this->addChild(enermyName);
    
    ss.str(string());
    if(firstSoldier == NULL)
    {
        ss << "NULL";
    }
    else
    {
        ss << "StopAction: " << (firstSoldier->stopAction ? "true" : "false");
    }
    
    stopActionLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14);
    stopActionLabel->setAnchorPoint(ccp(0, 0));
    stopActionLabel->setPosition(ccp(450, 0));
    this->addChild(stopActionLabel);
}

void GameHUD::updateSoldierHelper(float dt)
{
    CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
    GameSprite* firstSoldier = NULL;
    for(int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* temp = (GameSprite*) allSprites->objectAtIndex(i);
        if(temp->villagerClass == V_SOLDIER)
        {
            firstSoldier = temp;
            break;
        }
    }
    
    stringstream ss;
    
    if(firstSoldier == NULL)
    {
        ss << "NULL";
    }
    else
    {
        ss << firstSoldier->spriteName;
    }
    
    soldierName->setString(ss.str().c_str());
    
    ss.str(string());
    if(firstSoldier == NULL || firstSoldier->enermy == NULL)
    {
        ss << "NULL";
    }
    else
    {
        ss << firstSoldier->enermy->spriteName;
    }
    
    enermyName->setString(ss.str().c_str());
    
    ss.str(string());
    if(firstSoldier == NULL)
    {
        ss << "NULL";
    }
    else
    {
        ss << "StopAction: " << (firstSoldier->stopAction ? "true" : "false");
    }
    
    stopActionLabel->setString(ss.str().c_str());
}

void GameHUD::labelBackgroundFade(float dt)
{
    int tempOpacity = (int) infoBackground->getOpacity();
    if(label_fade_in)
    {
        tempOpacity += fadeSpeed;
        if(tempOpacity >= targetOpacity)
        {
            tempOpacity = targetOpacity;
            newPos = CCPointZero;
            targetOpacity = 0;
            fadeSpeed = 0;
            label_fade_out = false;
            label_fade_in = false;
            this->unschedule(schedule_selector(GameHUD::labelBackgroundFade));
        }
    }
    else if(label_fade_out)
    {
        tempOpacity -= fadeSpeed;
        if(tempOpacity <= targetOpacity)
        {
            tempOpacity = targetOpacity;
            if(isToggle)
            {
                targetOpacity = 255 * 0.9f;
                label_fade_out = false;
                label_fade_in = true;
                infoBackground->setPosition(newPos);
            }
            else
            {
                newPos = CCPointZero;
                targetOpacity = 0;
                fadeSpeed = 0;
                label_fade_out = false;
                label_fade_in = false;
                this->unschedule(schedule_selector(GameHUD::labelBackgroundFade));
            }
        }
    }
    else
    {
        this->unschedule(schedule_selector(GameHUD::labelBackgroundFade));
        return;
    }
    infoBackground->setOpacity((GLubyte) tempOpacity);
}

void GameHUD::clickMoneyLabel()
{
    if(label_fade_in || label_fade_out)
    {
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    if(labelStatus == InMoneyLabel)
    {
        isToggle = false;
        newPos = CCPointZero;
        targetOpacity = 0;
        fadeSpeed = 20;
        label_fade_out = true;
        labelStatus = Default;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
    else if(labelStatus == Default)
    {
        isToggle = false;
        infoBackground->setPosition(ccp(220, screenSize.height - 55));
        targetOpacity = 255 * 0.9f;
        fadeSpeed = 20;
        label_fade_in = true;
        labelStatus = InMoneyLabel;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
    else
    {
        isToggle = true;
        newPos = ccp(220, screenSize.height - 55);
        targetOpacity = 0;
        fadeSpeed = 20;
        label_fade_out = true;
        labelStatus = InMoneyLabel;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
}

void GameHUD::clickFoodLabel()
{
    if(label_fade_in || label_fade_out)
    {
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    if(labelStatus == InFoodLabel)
    {
        isToggle = false;
        newPos = CCPointZero;
        targetOpacity = 0;
        fadeSpeed = 20;
        label_fade_out = true;
        labelStatus = Default;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
    else if(labelStatus == Default)
    {
        isToggle = false;
        infoBackground->setPosition(ccp(440, screenSize.height - 55));
        targetOpacity = 255 * 0.9f;
        fadeSpeed = 20;
        label_fade_in = true;
        labelStatus = InFoodLabel;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
    else
    {
        isToggle = true;
        newPos = ccp(440, screenSize.height - 55);
        targetOpacity = 0;
        fadeSpeed = 20;
        label_fade_out = true;
        labelStatus = InFoodLabel;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
}

void GameHUD::clickPopulationLabel()
{
    if(label_fade_in || label_fade_out)
    {
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    if(labelStatus == InPopulationLabel)
    {
        isToggle = false;
        newPos = CCPointZero;
        targetOpacity = 0;
        fadeSpeed = 20;
        label_fade_out = true;
        labelStatus = Default;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
    else if(labelStatus == Default)
    {
        isToggle = false;
        infoBackground->setPosition(ccp(660, screenSize.height - 55));
        targetOpacity = 255 * 0.9f;
        fadeSpeed = 20;
        label_fade_in = true;
        labelStatus = InPopulationLabel;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
    else
    {
        isToggle = true;
        newPos = ccp(660, screenSize.height - 55);
        targetOpacity = 0;
        fadeSpeed = 20;
        label_fade_out = true;
        labelStatus = InPopulationLabel;
        this->schedule(schedule_selector(GameHUD::labelBackgroundFade), 1.0f / 120.0f);
    }
}

void GameHUD::scheduleAddMoney(int mon)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorGreen = ccc3(0, 255, 0);
    ccColor3B colorRed = ccc3(255, 0, 0);
    if(!isAddingMoney)
    {
        targetMoney = money + mon;
    }
    else
    {
        targetMoney += mon;
    }
    
    stringstream ss;
    
    if(mon >= 0)
    {
        ss << "+";
        moneyLabelDirectionUp = true;
    }
    else
    {
        moneyLabelDirectionUp = false;
    }
    
    ss << mon << "G";
    
    CCLabelTTF* addMoneyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20);
    addMoneyLabel->setPosition(ccp(280, screenSize.height - 40));
    addMoneyLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    if(mon < 0)
    {
        addMoneyLabel->setColor(colorRed);
    }
    else
    {
        addMoneyLabel->setColor(colorGreen);
    }
    
    addMoneyLabel->setScale(0.01f);
    this->addChild(addMoneyLabel, 10);
    addMoneyLabelArray->addObject(addMoneyLabel);
    
    if(!isAddingMoney)
    {
        isAddingMoney = true;
        this->schedule(schedule_selector(GameHUD::addMoney), 1.0f / 120.0f);
    }
}

void GameHUD::addMoney(float dt)
{
    bool stop = true;
    
    if(money < targetMoney)
    {
        money++;
        stop = false;
    }
    else if(money > targetMoney)
    {
        money--;
        stop = false;
    }
    
    for(int i = 0; i < addMoneyLabelArray->count(); i++)
    {
        CCLabelTTF* temp = (CCLabelTTF*) addMoneyLabelArray->objectAtIndex(i);
        
        float scale = temp->getScale();
        scale += 0.2f;
        if(scale > 1)
        {
            scale = 1;
        }
        temp->setScale(scale);
        
        if(moneyLabelDirectionUp)
        {
            temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() + 0.2f));
        }
        else
        {
            temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() - 0.2f));
        }
        
        float opacity = (float) temp->getOpacity();
        opacity -= 1.5f;
        if(opacity <= 0)
        {
            addMoneyLabelArray->removeObject(temp);
            i--;
        }
        else
        {
            temp->setOpacity((GLubyte) opacity);
        }
        stop = false;
    }
    
    if(stop)
    {
        isAddingMoney = false;
        this->unschedule(schedule_selector(GameHUD::addMoney));
    }
}

void GameHUD::scheduleAddReputation(int re)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorGreen = ccc3(0, 255, 0);
    
    targetReputation += re;
    
    stringstream ss;
    ss << "+" << re;
    
    CCLabelTTF* addReputationLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20);
    addReputationLabel->setPosition(ccp(175, screenSize.height - 90));
    addReputationLabel->setAnchorPoint(ccp(0.5, 0.5));
    addReputationLabel->setColor(colorGreen);
    addReputationLabel->setScale(0.01);
    
    this->addChild(addReputationLabel, 10);
    addReputationLabelArray->addObject(addReputationLabel);
    
    if(!isAddingReputation)
    {
        isAddingReputation = true;
        this->schedule(schedule_selector(GameHUD::addReputation));
    }
}

void GameHUD::addReputation(float dt)
{
    bool stop = true;
    
    if(reputation < targetReputation)
    {
        reputation++;
        stop = false;
    }
    
    for(int i = 0; i < addReputationLabelArray->count(); i++)
    {
        CCLabelTTF* temp = (CCLabelTTF*) addReputationLabelArray->objectAtIndex(i);
        
        float scale = temp->getScale();
        scale += 0.2f;
        if(scale > 1)
        {
            scale = 1;
        }
        temp->setScale(scale);
        temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() + 0.2f));
        
        float opacity = (float) temp->getOpacity();
        opacity -= 1.5f;
        if(opacity <= 0)
        {
            addReputationLabelArray->removeObject(temp);
            i--;
        }
        else
        {
            temp->setOpacity((GLubyte) opacity);
        }
        stop = false;
    }
    
    if(stop)
    {
        isAddingReputation = false;
        this->unschedule(schedule_selector(GameHUD::addMoney));
    }

}

void GameHUD::scheduleAddFood(int fo)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorGreen = ccc3(0, 255, 0);
    ccColor3B colorRed = ccc3(255, 0, 0);
    if(!isAddingFood)
    {
        targetFood = mGameCurrentFood + fo;
    }
    else
    {
        targetFood += fo;
    }
    
    stringstream ss;
    
    if(fo >= 0)
    {
        ss << "+";
        foodLabelDirectionUp = true;
    }
    else
    {
        foodLabelDirectionUp = false;
    }
    
    ss << fo;
    
    CCLabelTTF* addFoodLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20);
    addFoodLabel->setPosition(ccp(500, screenSize.height - 40));
    addFoodLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    if(fo < 0)
    {
        addFoodLabel->setColor(colorRed);
    }
    else
    {
        addFoodLabel->setColor(colorGreen);
    }
    
    addFoodLabel->setScale(0.01f);
    this->addChild(addFoodLabel, 10);
    addFoodLabelArray->addObject(addFoodLabel);
    
    if(!isAddingFood)
    {
        isAddingFood = true;
        this->schedule(schedule_selector(GameHUD::addFood), 1.0f / 120.0f);
    }
}

void GameHUD::scheduleAddStorage(int st)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorPurple = ccc3(255, 0, 255);
    ccColor3B colorBlue = ccc3(0, 0, 255);
    if(!isAddingStorage)
    {
        targetStorage = mGameCurrentStorage + st;
    }
    else
    {
        targetStorage += st;
    }
    
    stringstream ss;
    
    if(st >= 0)
    {
        ss << "+";
        storageLabelDirectionUp = true;
    }
    else
    {
        storageLabelDirectionUp = false;
    }
    
    ss << st;
    
    CCLabelTTF* addStorageLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 20);
    addStorageLabel->setPosition(ccp(500, screenSize.height - 40));
    addStorageLabel->setAnchorPoint(ccp(0.5, 0.5));
    
    if(st < 0)
    {
        addStorageLabel->setColor(colorPurple);
    }
    else
    {
        addStorageLabel->setColor(colorBlue);
    }
    
    addStorageLabel->setScale(0.01f);
    this->addChild(addStorageLabel, 10);
    addStorageLabelArray->addObject(addStorageLabel);
    
    if(!isAddingStorage)
    {
        isAddingStorage = true;
        this->schedule(schedule_selector(GameHUD::addStorage), 1.0f / 120.0f);
    }
}

void GameHUD::addFood(float dt)
{
    bool stop = true;
    
    if(mGameCurrentFood < targetFood)
    {
        mGameCurrentFood++;
        stop = false;
    }
    else if(mGameCurrentFood > targetFood)
    {
        mGameCurrentFood--;
        stop = false;
    }
    
    for(int i = 0; i < addFoodLabelArray->count(); i++)
    {
        CCLabelTTF* temp = (CCLabelTTF*) addFoodLabelArray->objectAtIndex(i);
        
        float scale = temp->getScale();
        scale += 0.2f;
        if(scale > 1)
        {
            scale = 1;
        }
        temp->setScale(scale);
        
        if(foodLabelDirectionUp)
        {
            temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() + 0.2f));
        }
        else
        {
            temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() - 0.2f));
        }
        
        float opacity = (float) temp->getOpacity();
        opacity -= 1.5f;
        if(opacity <= 0)
        {
            addFoodLabelArray->removeObject(temp);
            i--;
        }
        else
        {
            temp->setOpacity((GLubyte) opacity);
        }
        stop = false;
    }
    
    stringstream ss;
    ss << mGameCurrentFood << "/" << mGameCurrentStorage;
    foodLabel->setString(ss.str().c_str());
    
    if(stop)
    {
        isAddingFood = false;
        this->unschedule(schedule_selector(GameHUD::addFood));
    }
}

void GameHUD::addStorage(float dt)
{
    bool stop = true;
    
    if(mGameCurrentStorage < targetStorage)
    {
        mGameCurrentStorage++;
        stop = false;
    }
    else if(mGameCurrentStorage > targetStorage)
    {
        mGameCurrentStorage--;
        stop = false;
    }
    
    for(int i = 0; i < addStorageLabelArray->count(); i++)
    {
        CCLabelTTF* temp = (CCLabelTTF*) addStorageLabelArray->objectAtIndex(i);
        
        float scale = temp->getScale();
        scale += 0.2f;
        if(scale > 1)
        {
            scale = 1;
        }
        temp->setScale(scale);
        
        if(storageLabelDirectionUp)
        {
            temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() + 0.2f));
        }
        else
        {
            temp->setPosition(ccp(temp->getPositionX(), temp->getPositionY() - 0.2f));
        }
        
        float opacity = (float) temp->getOpacity();
        opacity -= 1.5f;
        if(opacity <= 0)
        {
            addStorageLabelArray->removeObject(temp);
            i--;
        }
        else
        {
            temp->setOpacity((GLubyte) opacity);
        }
        stop = false;
    }
    
    stringstream ss;
    ss << mGameCurrentFood << "/" << mGameCurrentStorage;
    foodLabel->setString(ss.str().c_str());
    
    if(stop)
    {
        isAddingStorage = false;
        this->unschedule(schedule_selector(GameHUD::addStorage));
    }
}