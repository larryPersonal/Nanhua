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

GameHUD* GameHUD::SP;


GameHUD::GameHUD()
{
    GameHUD::SP = this;
    hint_show_time = 200;
    curr_hint_show_time = 0;
    date = new Date();
    cumulatedTime = 0.0f;
    
    mGameDay = 0;
    mGameMonth = 0;
    mGameWeek = 0;
    mGameYear = 0;
}

GameHUD::~GameHUD()
{
    removeChild(ri);
    delete ri;
    delete date;

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
    currMenuMode = None;
    menuIsOpen = false;
    
    this->CCLayer::setTouchEnabled(true);
    createInitialGUI();
    
    return true;
}

void GameHUD::update(float deltaTime)
{
    //gameTimer->updateTimer(deltaTime);
    
    if (curr_hint_show_time > 0)
        --curr_hint_show_time;
    else
        if (hintInfoBar->isVisible())
        {
            hintInfoBar->setVisible(false);
            TutorialHandler::getThis()->ReportAction("hint", "timeout", "");
        }
    
    // process time line
    cumulatedTime += deltaTime;
    if(cumulatedTime >= 1.0f * GlobalSettings::time_for_one_day)
    {
        date->addDay();
        cumulatedTime = 0.0f;
    }
    
    // update time label
    if(mGameDay != date->day || mGameMonth != date->month || mGameWeek != date->week || mGameYear != date->year)
    {
        mGameDay = date->day;
        mGameWeek = date->week;
        mGameMonth = date->month;
        mGameYear = date->year;
        
        std::stringstream ss;
        ss << "Year: " << (date->year + 1) << " Month: " << (date->month + 1) << " Week: " << (date->week + 1) << " Day: " << (date->day + 1);
        timeLabel->setString(ss.str().c_str());
    }
}

void GameHUD::createInitialGUI(){
    
    mGameDay = date->day;
    mGameWeek = date->week;
    mGameMonth = date->month;
    mGameYear = date->year;
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ri = new ResearchIndicator();
    //ri->createResearchIndicator();
    ri->setPosition(screenSize.width - 350.0f, screenSize.height - 150.0f);
    
    ccColor3B colorWhite = ccc3(255, 255, 255);
    
    std::stringstream ss;
    ss << "Year: " << (date->year + 1) << " Month: " << (date->month + 1) << " Week: " << (date->week + 1) << " Day: " << (date->day + 1);
    timeLabel = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    timeLabel->setColor(colorWhite);
    timeLabel->setAnchorPoint(ccp(0, 1));
    
    this->addChild(timeLabel, 3);
    timeLabel->CCNode::setPosition(0.0f, 1000.0f);
    
    createInfoBars();
    //   menuButton = CCMenuItemImage::create("game-menu-mockup-buttons-menu.png", "game-menu-mockup-buttons-menu.png", this, menu_selector(GameHUD::onMenuButtonPressed) );
    
//    miscButton = CCMenuItemImage::create("game-menu-mockup-buttons-menu.png", "game-menu-mockup-buttons-menu.png", this, menu_selector(GameHUD::onMiscButtonPressed));
    
    menuButton = CCMenuItemImage::create("button-up.png", "button-down.png", this, menu_selector(GameHUD::onMenuButtonPressed) );
    
    miscButton = CCMenuItemImage::create("button-up.png", "button-down.png", this, menu_selector(GameHUD::onMiscButtonPressed));
    
    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        menuButton->setScale(0.67);
        miscButton->setScale(0.67);
    }
    //The main menu will always be the first child.

    
    
    CCLabelTTF* menuLabel = CCLabelTTF::create("Menu", "Vinland" ,64, menuButton->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
    CCSize spriteSize = menuButton->boundingBox().size;
    
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
   // menuLabel->setPosition( ccp(menuButton->getPositionX() + menuButton->boundingBox().size.width * 0.25f, menuButton->getPositionY() + menuButton->boundingBox().size.height * 0.25f));
    menuButton->setPosition( ccp(spriteSize.width*0.5, spriteSize.height*0.5) );
    menuButton->addChild(menuLabel);
    menuLabel->setPosition(ccp(menuButton->boundingBox().size.width * 0.75f, menuButton->boundingBox().size.height * 0.75f));
    
    miscLabel = CCLabelTTF::create("Menu", "Vinland" ,64, miscButton->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    spriteSize = miscButton->boundingBox().size;
    miscButton->setPosition( ccp(screenSize.width - spriteSize.width*0.5, spriteSize.height*0.5) );
    
    miscButton->addChild(miscLabel);
    miscLabel->setPosition(ccp(miscButton->boundingBox().size.width * 0.75, miscButton->boundingBox().size.height * 0.75));
    
  
    
    //Need to call this once to init the positions and scales
    onOrientationChanged();
   
    // create menu, it's an autorelease object
    pMenu = CCMenu::create(menuButton, miscButton, NULL);
    pMenu->setPosition( CCPointZero );
    pMenu->setTag((int)"pMenu");
    pMenu->setTouchPriority(kCCMenuHandlerPriority - 999);
    this->addChild(pMenu, 2);
    
    
    
      this->addChild(ri, 3);
 
}

void GameHUD::onMenuButtonPressed()
{
    closeAllMenuAndResetTapMode();
    
    if (!menuIsOpen)
    {
        miscLabel->setString("Cancel");
        closeAllMenuAndResetTapMode();
        
        InGameMenu* inGameMenu = InGameMenu::create();
        inGameMenu->useAsExclusivePopupMenu();
    
        menuIsOpen = true;
        currMenuMode = OpenMenu;
    }
    //This part instantly destroys all buttons, pls edit to animate out if possible
    else
    {
        PopupMenu::closeAllPopupMenu();
        menuIsOpen = false;
        currMenuMode = None;
        miscLabel->setString("Menu");
    }
    CCLog("menuIsOpen = %i", menuIsOpen);
}

void GameHUD::onMiscButtonPressed(){
    switch (currMenuMode) {
        case None:{
            onMenuButtonPressed();
            break;
        }
            
        case OpenMenu:{
            onMenuButtonPressed();
            break;
        }
            
        case BuildingsMenu:{
            if (currTapMode != 0)
            {
                closeAllMenuAndResetTapMode();
                return;
            }
            backToInGameMenu();
            miscLabel->setString("Cancel");
            break;
        }
            
        case ResearchMenu:{
            backToInGameMenu();
            miscLabel->setString("Cancel");
            break;
        }
            
        case PopulationMenu:{
            backToInGameMenu();
            miscLabel->setString("Cancel");

            break;
        }
            
        case PolicyMenu:{
            backToInGameMenu();
            miscLabel->setString("Cancel");

            break;
        }
            
        case InfoMenu:{
            backToInGameMenu();
            miscLabel->setString("Cancel");

            break;
        }
            
        case SystemMenu:{
            backToInGameMenu();
            miscLabel->setString("Cancel");

            break;
        }
            
        default:{
            break;
        }
    }
}

void GameHUD::setTapMode(int mode)
{
    currTapMode = (tapMode)mode;
}

int GameHUD::getTapMode()
{
    return currTapMode;
}

void GameHUD::setMenuMode(int newMode)
{
    currMenuMode = (MenuMode)newMode;
}

int GameHUD::getMenuMode()
{
    return currMenuMode;
}

void GameHUD::onOrientationChanged(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    CCSize spriteSize = miscButton->boundingBox().size;
    
    
    miscButton->setPosition( ccp(screenSize.width - spriteSize.width*0.5, spriteSize.height*0.5) );
    
    if (ri != NULL)
    ri->setPosition(screenSize.width - 350.0f, screenSize.height - 150.0f);

   
    //researchIndicator->setPosition(0.0f, 0.0f);
 
}

void GameHUD::createInfoBars()
{
    
    
    //Create bottom info bar
    bottomInfoBar = InfoBar::create();
    bottomInfoBar->bottom = 6.0f;
    bottomInfoBar->left = 0.0f;
    bottomInfoBar->right = 0.0f;
    
    bottomInfoBar->createInfoBar(0.0f, 64.0f, "bar.png");
    bottomInfoBar->setTag((int)"bottomInfoBar");
    this->addChild(bottomInfoBar, 2);
    
    //create the Hint Info Bar
    hintInfoBar = InfoBar::create();
    hintInfoBar->bottom = 70.0f;
    hintInfoBar->left = 120.0f;
    hintInfoBar->right = 120.0f;
    
    hintInfoBar->createInfoBar(0.0f, 64.0f, "hint box.png");
    hintInfoBar->setTag((int)"bottomInfoBar");
    this->addChild(hintInfoBar, 1);
    
    //Add background sides for hint bar
    CCSprite* sprite = CCSprite::create("hint left.png");
    hintInfoBar->addItem("bgSideLeft", sprite);
    hintInfoBar->anchorItem("bgSideLeft", 0.0f);
    
    sprite = CCSprite::create("hint right.png");
    hintInfoBar->addItem("bgSideRight", sprite);
    hintInfoBar->anchorItem("bgSideRight", 0.0f, ANCHOR_RIGHT);
    
    hintInfoBar->setVisible(false);
    
    default_hint_font_size = 30;
    hintLabel = CCLabelTTF::create("", "Droidiga", default_hint_font_size);
    hintInfoBar->addItem("hintLabel", hintLabel);
    hintInfoBar->anchorItem("hintLabel", 0.0f, ANCHOR_CENTER);
    
    /** Item creation **/
    
    //Create top info bar items
    
    //Add Timer to scene
    
    //Create bottom info bar items
    popTotalLabel = CCLabelTTF::create("Total Pop:", "Droidiga", 20);
    bottomInfoBar->addItem("popTotalLabel", popTotalLabel);
    bottomInfoBar->anchorItem("popTotalLabel", 160.0f);
    avgHapLabel = CCLabelTTF::create("Avg Hap:", "Droidiga", 20);
    bottomInfoBar->addItem("avgHapLabel", avgHapLabel);
    bottomInfoBar->anchorItem("avgHapLabel", 160.0f, ANCHOR_RIGHT);
    
    buildLabel = CCLabelTTF::create("Building", "Doridiga", 20);
    bottomInfoBar->addItem("buildLabel", buildLabel);
    bottomInfoBar->anchorItem("buildLabel", 160.0f);
    buildCostLabel = CCLabelTTF::create("Cost:", "Droidiga", 20);
    bottomInfoBar->addItem("buildCostLabel", buildCostLabel);
    bottomInfoBar->anchorItem("buildCostLabel", 160.0f, ANCHOR_RIGHT);
    hideBuildLabel();

    
    
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
    
    hideBuildLabel();
    setTapMode(0);
}

void GameHUD::backToInGameMenu()
{
    PopupMenu* inGameMenu = InGameMenu::create();
    inGameMenu->useAsExclusivePopupMenu();
    
    menuIsOpen = true;
    currMenuMode = OpenMenu;
}

void GameHUD::showBuildLabel(const char* buildingName)
{
    if (!buildLabel->isVisible())
    {
        buildLabel->setString(buildingName);
        buildLabel->setVisible(true);
        buildCostLabel->setVisible(true);
        popTotalLabel->setVisible(false);
        avgHapLabel->setVisible(false);
    }
}

void GameHUD::hideBuildLabel()
{
    if (buildLabel->isVisible())
    {
        popTotalLabel->setVisible(true);
        avgHapLabel->setVisible(true);
        buildLabel->setVisible(false);
        buildCostLabel->setVisible(false);
    }
}

void GameHUD::buyBuilding(int cost){
    GameManager::getThis()->currentMoney -= cost;
    updateMoneyLabel();
}

void GameHUD::onSpriteAddedToMap(GameSprite* sprite)
{
    GameManager::getThis()->totalHappiness += sprite->getPossessions()->happinessRating;
    updatePopTotalLabel();
    updateAvgHapLabel();
    
    if (currMenuMode == PopulationMenu)
    {
        if (class PopulationMenu* menu = dynamic_cast<class PopulationMenu*>(PopupMenu::getBottommostPopupMenu()))  
            menu->addPopulationToList(sprite);
    }
}

void GameHUD::onSpriteRemovedFromMap(GameSprite* sprite)
{
    GameManager::getThis()->totalHappiness -= sprite->getPossessions()->happinessRating;
    updatePopTotalLabel();
    updateAvgHapLabel();
    
    if (currMenuMode == PopulationMenu)
    {
        if (class PopulationMenu* menu = dynamic_cast<class PopulationMenu*>(PopupMenu::getBottommostPopupMenu()))
            menu->removePopulationFromList(sprite);
    }
}

void GameHUD::updatePopTotalLabel()
{
    std::stringstream ss;
    ss << "Total Pop: " << GameScene::getThis()->spriteHandler->spritesOnMap->count();
    popTotalLabel->setString(ss.str().c_str());
}

void GameHUD::updateAvgHapLabel()
{
    std::stringstream ss;
    ss << "Avg Hap: " << GameManager::getThis()->getAverageHappiness();
    avgHapLabel->setString(ss.str().c_str());
}

void GameHUD::updateMoneyLabel()
{
    std::stringstream ss;
    ss << GameManager::getThis()->currentMoney << " G";
}

void GameHUD::updateBuildCostLabel(int cost, int dist)
{
    std::stringstream ss;
    ss << "Cost: " << cost * (dist >= 0 ? dist : 1) << "G";
    if (dist >= 0)
        ss << " (" << dist << "x" << cost << "G)";
    
    buildCostLabel->setString(ss.str().c_str());
}


void GameHUD::showHint(std::string hintText)
{
    hintLabel->setFontSize(default_hint_font_size);
    hintLabel->setString(hintText.c_str());
    if ((hintLabel->getContentSize().width + 10) > hintInfoBar->size.width)
    {

        hintLabel->setFontSize(default_hint_font_size * (hintInfoBar->size.width/(hintLabel->getContentSize().width + 10)));
    }
    
    hintInfoBar->setVisible(true);
    
    curr_hint_show_time = hint_show_time;
}

CCMenuItemImage* GameHUD::getMenuButton()
{
    return menuButton;
}

Date* GameHUD::getDate()
{
    return date;
}
