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
    
    objectiveStrs = CCArray::create();
    objectiveStrs->retain();
    
    leftPos = 0;
    maxPos = 100.0f;
    
    scroll_in = false;
    scroll_out = false;
    
    scrolled_in = false;
    
    setTutorial = false;
}

GameHUD::~GameHUD()
{
    delete date;
    
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
                    firstWeekLabel->setOpacity(255);
                    secondWeekLabel->setOpacity(120);
                    thirdWeekLabel->setOpacity(120);
                    lastWeekLabel->setOpacity(120);
                    break;
                case 1:
                    firstWeekLabel->setOpacity(120);
                    secondWeekLabel->setOpacity(255);
                    thirdWeekLabel->setOpacity(120);
                    lastWeekLabel->setOpacity(120);
                    break;
                case 2:
                    firstWeekLabel->setOpacity(120);
                    secondWeekLabel->setOpacity(120);
                    thirdWeekLabel->setOpacity(255);
                    lastWeekLabel->setOpacity(120);
                    break;
                case 3:
                    firstWeekLabel->setOpacity(120);
                    secondWeekLabel->setOpacity(120);
                    thirdWeekLabel->setOpacity(120);
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
    moneyLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
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
    foodLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    foodLabel->setColor(colorWhite);
    foodLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(foodLabel, 2);
    foodLabel->CCNode::setPosition(495, screenSize.height - 28);
    
    // create the population indicator
    populationIcon = CCSprite::create("population_amount.png");
    populationIcon->cocos2d::CCNode::setScale(screenSize.width / spriteSize.width * 0.28f, screenSize.width / spriteSize.width * 0.25f);

    populationIcon->setAnchorPoint(ccp(0, 1));
    populationIcon->setPosition(ccp(580, screenSize.height - 2));
    this->addChild(populationIcon, 2);
    
    mGameCurrentCitizenPopulation = 0;
    mGameCurrentPopulationRoom = 0;
    
    CCArray* allSprites = GameScene::getThis()->spriteHandler->spritesOnMap;
    for(int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
        if(gs->villagerClass != V_BANDIT && gs->villagerClass != V_CLASS_END && gs->getHome() != NULL)
        {
            mGameCurrentCitizenPopulation++;
        }
    }
    
    CCArray* allHousing = GameScene::getThis()->buildingHandler->housingOnMap;
    for(int i = 0; i < allHousing->count(); i++)
    {
        Building* b = (Building*) allHousing->objectAtIndex(i);
        mGameCurrentPopulationRoom += b->populationLimit;
    }
    
    ss.str(std::string());
    ss << mGameCurrentCitizenPopulation << "/" << mGameCurrentPopulationRoom;
    populationLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 14, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    populationLabel->setColor(colorWhite);
    populationLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(populationLabel, 2);
    populationLabel->CCNode::setPosition(700, screenSize.height - 28);
    
    // create the achievements label for the values
    ss.str(std::string());
    ss << GameScene::getThis()->configSettings->default_ini_reputation;
    achivementsLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    achivementsLabel->setColor(colorBlack);
    achivementsLabel->setAnchorPoint(ccp(0.5, 1));
    this->addChild(achivementsLabel, 2);
    achivementsLabel->CCNode::setPosition(180, screenSize.height - 76);
    
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
    firstWeekLabel = CCSprite::create("timeclockgreen.png");
    firstWeekLabel->setAnchorPoint(ccp(0, 1));
    firstWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.13f);
    firstWeekLabel->setPosition(ccp(screenSize.width - 63.0f - leftPos, screenSize.height - 75.0f));

    this->addChild(firstWeekLabel, 3);
    
    secondWeekLabel = CCSprite::create("timeclockbrown.png");
    secondWeekLabel->setAnchorPoint(ccp(0, 1));
    secondWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.13f);
    secondWeekLabel->setPosition(ccp(screenSize.width - 36.0f - leftPos, screenSize.height - 55.0f));

    this->addChild(secondWeekLabel, 3);
    
    thirdWeekLabel = CCSprite::create("timeclockred.png");
    thirdWeekLabel->setAnchorPoint(ccp(0, 1));
    thirdWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.125f);
    thirdWeekLabel->setPosition(ccp(screenSize.width - 35.5f - leftPos, screenSize.height - 35.0f));

    this->addChild(thirdWeekLabel, 3);
    
    lastWeekLabel = CCSprite::create("timeclockyellow.png");
    lastWeekLabel->setAnchorPoint(ccp(0, 1));
    lastWeekLabel->setScale(screenSize.width / timeMenu->getContentSize().width * 0.125f);
    lastWeekLabel->setPosition(ccp(screenSize.width - 49.0f - leftPos, screenSize.height - 15.0f));

    this->addChild(lastWeekLabel, 3);
    
    firstWeekLabel->setOpacity(255);
    secondWeekLabel->setOpacity(120);
    thirdWeekLabel->setOpacity(120);
    lastWeekLabel->setOpacity(120);
    
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
    firstWeekLabel->setPosition(ccp(screenSize.width - 63.0f - leftPos, screenSize.height - 75.0f));
    secondWeekLabel->setPosition(ccp(screenSize.width - 36.0f - leftPos, screenSize.height - 55.0f));
    thirdWeekLabel->setPosition(ccp(screenSize.width - 35.5f - leftPos, screenSize.height - 35.0f));
    lastWeekLabel->setPosition(ccp(screenSize.width - 49.0f - leftPos, screenSize.height - 15.0f));
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
    ccColor3B colorGreen = ccc3(0, 255, 0);
    ccColor3B colorBlack = ccc3(0, 0, 0);
    
    // create the objective group background
    string objectiveBackground = "objective.png";
    objectiveMenu = CCSprite::create(objectiveBackground.c_str());
    CCSize spriteSize = objectiveMenu->getContentSize();
    objectiveMenu->setVisible(false);
    objectiveMenu->setAnchorPoint(ccp(0.3, 0.5));
    objectiveMenu->setScale(screenSize.width / spriteSize.width * 0.35f);
    objectiveMenu->setPosition(ccp(-1000, screenSize.height - 85.0f));
    
    // create the objective button
    objectiveButton = CCSprite::create("objective-menu-button_06.png");
    objectiveButton->setAnchorPoint(ccp(0.3, 0.5));
    objectiveButton->setScale(screenSize.width / spriteSize.width * 0.35f);
    objectiveButton->setPosition(ccp(40, screenSize.height - 160));
    this->addChild(objectiveButton);
    
    // create the objective title and objective strings!
    stringstream ss;
    ss << "Objective";
    objectiveTitle = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentCenter);
    objectiveTitle->setAnchorPoint(ccp(0.5, 0));
    objectiveTitle->setColor(colorBlack);
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
        
        tempLabel->setColor(colorBlack);
        if(objective->complete)
        {
            tempLabel->setColor(colorGreen);
        }
    }
    
    this->addChild(objectiveMenu, 2);
    this->addChild(objectiveTitle, 3);
    
    objectiveTitle->setVisible(false);
    for (int i = 0; i < objectiveStrs->count(); i++)
    {
        CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
        objectiveStr->setVisible(false);
    }
}

void GameHUD::fadeIn(float dt)
{
    if(fade_in)
    {
        float speed = 5;
        float opacity = ((float)objectiveMenu->getOpacity()) + speed;
        
        if(opacity >= 255)
        {
            opacity = 255;
            this->unschedule(schedule_selector( GameHUD::fadeIn ));
            fade_in = false;
        }
        objectiveMenu->setOpacity((GLubyte) opacity);
        objectiveTitle->setOpacity((GLubyte) opacity);
        for(int i = 0; i < objectiveStrs->count(); i++)
        {
            CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
            objectiveStr->setOpacity((GLubyte) opacity);
        }
    }
}

void GameHUD::fadeOut(float dt)
{
    if(fade_out)
    {
        float speed = 5;
        float opacity = ((float) objectiveMenu->getOpacity()) - speed;
        
        if(opacity <= 0)
        {
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
            opacity = 0;
            this->unschedule(schedule_selector( GameHUD::fadeOut ));
            fade_out = false;
            objectiveMenu->setVisible(false);
            objectiveTitle->setVisible(false);
            objectiveMenu->setPosition(ccp(-1000, screenSize.height - 120.0f));
            for(int i = 0; i < objectiveStrs->count(); i++)
            {
                CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
                objectiveStr->setVisible(false);
            }
        }
        objectiveMenu->setOpacity((GLubyte) opacity);
        objectiveTitle->setOpacity((GLubyte) opacity);
        for(int i = 0; i < objectiveStrs->count(); i++)
        {
            CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
            objectiveStr->setOpacity((GLubyte) opacity);
        }
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
            for (int i = 0; i < objectiveStrs->count(); i++)
            {
                CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
                objectiveStr->setOpacity((GLubyte) 255);
            }
            fade_out = true;
            this->schedule(schedule_selector( GameHUD::fadeOut ), 1.0f / 240.0f);
        }
        else
        {
            objectiveMenu->setOpacity((GLubyte) 0);
            objectiveMenu->setVisible(true);
            objectiveTitle->setOpacity((GLubyte) 0);
            objectiveTitle->setVisible(true);
            objectiveMenu->setPosition(ccp(0, screenSize.height - 120.0f));
            for (int i = 0; i < objectiveStrs->count(); i++)
            {
                CCLabelTTF* objectiveStr = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
                objectiveStr->setOpacity((GLubyte) 0);
                objectiveStr->setVisible(true);
            }
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
    buildButton->setScale(0.5f);

    buildButton->setAnchorPoint(ccp(1, 0));
    buildButton->setPosition(ccp(screenSize.width, 0));
    
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
        if(TutorialManager::getThis()->active && TutorialManager::getThis()->teachBuildButton)
        {
            TutorialManager::getThis()->lockBuildScroll = true;
            TutorialManager::getThis()->miniDragon->display();
            TutorialManager::getThis()->miniDragon->move(ccp(0, 220));
            TutorialManager::getThis()->teachBuildButton = false;
            TutorialManager::getThis()->teachBuildHouse = true;
        }
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        if(currTapMode == Build && GameScene::getThis()->buildingHandler->selectedBuilding != NULL)
        {
            money += GameScene::getThis()->buildingHandler->selectedBuilding->buildingCost;
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
    
    pauseButton = CCSprite::create("pauseIcon.png");
    resumeButton = CCSprite::create("nextButton.png");
    systemButton = CCSprite::create("system.png");
    stickHappinessButton = CCSprite::create("happyFace.png");
    resumeHappinessButton = CCSprite::create("normalFace.png");
    warButton = CCSprite::create("banditicon.png");
    peaceButton = CCSprite::create("peaceicon.png");
    
    pauseButton->setScale(screenSize.width / pauseButton->boundingBox().size.width * 0.05f);
    pauseButton->setAnchorPoint(ccp(1, 1));
    pauseButton->setPosition(ccp(screenSize.width - 50.0f, screenSize.height - 125.0f));
    
    resumeButton->setScale(screenSize.width / resumeButton->boundingBox().size.width * 0.05f);
    resumeButton->setAnchorPoint(ccp(1, 1));
    resumeButton->setPosition(ccp(screenSize.width - 50.0f, screenSize.height - 125.0f));
    
    systemButton->setScale(screenSize.width / systemButton->boundingBox().size.width * 0.05f);
    systemButton->setAnchorPoint(ccp(1, 1));
    systemButton->setPosition(ccp(screenSize.width * 0.075f, screenSize.height - 205.0f));
    
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
    SystemMenu* sm = SystemMenu::create(this);
    sm->retain();
    pause = true;
}