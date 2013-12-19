//
//  SpriteInfoMenu.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 6/5/13.
//
//

#include "SpriteInfoMenu.h"
#include "GameHUD.h"
#include "GlobalHelper.h"
#include "BuildingInfoMenu.h"

SpriteInfoMenu::SpriteInfoMenu(GameSprite* gameSprite)
{
    if (!gameSprite) CCLog("Warning NO SPRITE!!!");
    this->gameSprite = gameSprite;
    scrollArea = NULL;
    mGameSpriteEnergy = 0;
    mGameSpriteEnergyMax = 0;
    mGameSpriteCash = 0;
    mGameSpriteEduLvl = 0;
    mGameSpriteInt = 0;
    mGameSpriteLoy = 0;
    mGameSpriteSoc = 0;
    mGameSpriteHap = 0;
    mGameSpriteExp = 0;
    mGameSpriteExpMax = 0;
}

SpriteInfoMenu::~SpriteInfoMenu()
{
    
    if (scrollArea != NULL)
    {
        delete scrollArea;
        scrollArea = NULL;
    }
}

void SpriteInfoMenu::createMenuItems()
{
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    
    // Create constant menu items
    spriteBackground = CCSprite::create("outerbox.png");
    spriteBackground->setScale(700 / spriteBackground->boundingBox().size.width);
    
    spriteBackgroundInner = CCSprite::create("innerbox-info.png");
    spriteBackgroundInner->setScale(spriteBackground->getScale());
    
    // Set variables which may become dirty
    mGameSpriteEnergy = gameSprite->getPossessions()->energyRating;
    mGameSpriteEnergyMax = gameSprite->getPossessions()->defaultEnergy;
    mGameSpriteCash = gameSprite->getPossessions()->cashOnHand;
    mGameSpriteEduLvl = gameSprite->getPossessions()->educationLevel;
    mGameSpriteInt = gameSprite->getPossessions()->intelligenceRating;
    mGameSpriteLoy = gameSprite->getPossessions()->loyaltyRating;
    mGameSpriteSoc = gameSprite->getPossessions()->socialRating;
    mGameSpriteHap = gameSprite->getPossessions()->happinessRating;
    mGameSpriteExp = gameSprite->getPossessions()->expRating;
    mGameSpriteExpMax = gameSprite->getPossessions()->getExpToLevel();
    
    // Create header
    textName = CCLabelTTF::create(GlobalHelper::stringToUpper(gameSprite->spriteDisplayedName).c_str(), "Droidiga", 32);
    textName->setColor(colorYellow);
    
    // Cash
    spCash = CCSprite::create("money icon.png");
    spCash->setScale(0.75);
    /*
    labelCash = CCLabelTTF::create("CASH:", "Droidiga", 26);
    labelCash->setColor(colorYellow);
    */
    std::stringstream ss;
    ss << mGameSpriteCash << " G";
    textCash = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    textCash->setColor(colorYellow);
    
    // Sprite
    string spriteFrameName = gameSprite->spriteName.c_str();
    spriteFrameName += "_IDL001.png";
    gameSpriteImage = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
    gameSpriteImage->setScale(150.0f / gameSpriteImage->boundingBox().size.width);
    
    // Attribute labels
   
    ss.str(std::string());
    ss << "Skill Level: " << mGameSpriteEduLvl;
    textEduLvl = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 20);
    textEduLvl->setColor(colorGreen);
    
    
    spInt = CCSprite::create("intelligence icon.png");
    spInt->setScale(0.75);
    spLoy = CCSprite::create("loyalty icon.png");
    spLoy->setScale(0.75);

    spSoc = CCSprite::create("social icon.png");
    spSoc->setScale(0.75);

    spHap = CCSprite::create("happiness icon.png");
    spHap->setScale(0.75);
    
    

    // Attribute text
    ss.str(std::string());
    ss << mGameSpriteInt/10;
    textInt = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    textInt->setColor(colorGreen);
    ss.str(std::string());
    ss << mGameSpriteLoy/10;
    textLoy = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    textLoy->setColor(colorGreen);
    ss.str(std::string());
    ss << mGameSpriteSoc/10;
    textSoc = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    textSoc->setColor(colorGreen);
    ss.str(std::string());
    ss << mGameSpriteHap/10;
    textHap = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 26);
    textHap->setColor(colorGreen);
    
    // Exp
    labelExp = CCLabelTTF::create("NEXT LEVEL", "Droidiga", 20);
    labelExp->setColor(colorBlack);
    ss.str(std::string());
    ss << mGameSpriteExp << "/" << mGameSpriteExpMax;
    textExp = CCLabelTTF::create(ss.str().c_str(), "Droidiga", 20);
    textExp->setColor(colorBlack);
    
    barExp = new ProgressBar();
    barExp->createProgressBar(CCRectMake(0, 0, 80, 20),
                              CCRectMake(5, 5, 70, 10),
                              "loadingbar-empty.png",
                              "loadingbar-left.png",
                              "loadingbar-right.png",
                              "loadingbar-full.png");
    barExp->setValue(mGameSpriteExp / (float)mGameSpriteExpMax);
    
    // Energy Bar
    barEnergy = new ProgressBar();
    barEnergy->createProgressBar(CCRectMake(0, 0, 160, 20),
                                 CCRectMake(5, 5, 150, 10),
                                 "loadingbar-empty.png",
                                 "loadingbar-left.png",
                                 "loadingbar-right.png",
                                 "loadingbar-full.png");
    barEnergy->setValue(mGameSpriteEnergy / (float)mGameSpriteEnergyMax);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    buttonClose = CCMenuItemImage::create("closebutton-up.png", "closebutton-down.png", this, menu_selector(SpriteInfoMenu::onMenuItemSelected));
    buttonClose->setScale(0.4);
    buttonClose->setTag(0);

    buttonHome = CCMenuItemImage::create("homebutton.png", "homebutton_pressed.png", this, menu_selector(SpriteInfoMenu::onMenuItemSelected));
    buttonHome->setScale(0.75);
    buttonHome->setTag(1);

    buttonWorkPlace = CCMenuItemImage::create("jobbutton.png", "jobbutton_pressed.png", this, menu_selector(SpriteInfoMenu::onMenuItemSelected));
    buttonWorkPlace->setScale(0.75);
    buttonWorkPlace->setTag(2);
    
    
    menuItems->addObject(buttonClose);
    menuItems->addObject(buttonWorkPlace);
    menuItems->addObject(buttonHome);
    
    CCMenu* menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Scroll Area
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(CCSizeMake(610, 275), CCSizeMake(610, 275));
    scrollArea->enableScrollVertical(10, "bar.png", "bar.png");
    scrollArea->hideScroll();
    scrollArea->addItem(barEnergy, ccp(80, 20));
    scrollArea->addItem(gameSpriteImage, ccp(90, 25));
    scrollArea->addItem(textEduLvl, ccp(70, 175));
    scrollArea->addItem(spInt, ccp(405, 25));
    scrollArea->addItem(spLoy, ccp(405, 75));
    scrollArea->addItem(spSoc, ccp(405, 125));
    scrollArea->addItem(spHap, ccp(405, 175));
    scrollArea->addItem(textInt, ccp(505, 40));
    scrollArea->addItem(textLoy, ccp(505, 90));
    scrollArea->addItem(textSoc, ccp(505, 140));
    scrollArea->addItem(textHap, ccp(505, 190));
    scrollArea->addItem(labelExp, ccp(35, 225));
    scrollArea->addItem(barExp, ccp(170, 225));
    scrollArea->addItem(textExp, ccp(260, 225));
    
    
    scrollArea->updateScrollBars();
    
    // Add children
    this->addChild(spriteBackground);
    this->addChild(spriteBackgroundInner);
    this->addChild(spCash);
    this->addChild(textName);
    this->addChild(textCash);
    this->addChild(menu);
    
    this->addChild(scrollArea);
    
    // Done creation, now position them
    spriteBackground->setAnchorPoint(ccp(0.5, 0.5));
    textName->setAnchorPoint(ccp(0.5, 1));
    spCash->setAnchorPoint(ccp(1, 0));
    textCash->setAnchorPoint(ccp(1, 0));
    scrollArea->setAnchorPoint(ccp(0.5, 0.5));
    buttonClose->setAnchorPoint(ccp(1, 1));
    
    buttonHome->setAnchorPoint(ccp(1, 1));
    
    buttonWorkPlace->setAnchorPoint(ccp(1, 1));
    
    reposition();
    
    this->schedule(schedule_selector(SpriteInfoMenu::update), 0.25f);
}

void SpriteInfoMenu::onMenuItemSelected(CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    
    switch (pMenuItem->getTag())
    {
        case 0:
        {
            // buttonClose
            this->closeMenu(false);
        }
            break;
        case 1:
        {
            //home location
            if (gameSprite != NULL)
            {
                if (gameSprite->getPossessions()->hasHouse)
                {
                    this->closeMenu(false);
                
                    BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(gameSprite->getPossessions()->homeLocation);//new BuildingInfoMenu(selectedTile->building);
                //   buildingInfoMenu->autorelease();
                    buildingInfoMenu->useAsTopmostPopupMenu();
                }
            }
        }
            break;
        case 2:
        {
            //job location
            if (gameSprite != NULL)
            {
                if (gameSprite->getPossessions()->hasJob)
                {
                    this->closeMenu(false);
            
                    BuildingInfoMenu* buildingInfoMenu = BuildingInfoMenu::create(gameSprite->getPossessions()->jobLocation);//new BuildingInfoMenu(selectedTile->building);
                    //   buildingInfoMenu->autorelease();
                    buildingInfoMenu->useAsTopmostPopupMenu();
                }
            }
        }
            break;
            
        default:
        {
        }
            break;
    }
}

void SpriteInfoMenu::reposition()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    this->CCNode::setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
    
    float halfWidth = spriteBackground->boundingBox().size.width / 2.0f;
    float halfHeight = spriteBackground->boundingBox().size.height / 2.0f;
    
    // Anchored top
    textName->CCNode::setPosition(0, halfHeight - 40.0f);
    
    // Anchored top right
    buttonClose->setPosition(halfWidth - 20.0f, halfHeight - 20.0f);
    buttonHome->setPosition(-halfWidth + 140.0f,  -halfHeight + 80.0f);
    buttonWorkPlace->setPosition(-halfWidth + 80.0f, -halfHeight + 80.0f);
    
    
    // Anchored bottom right
    textCash->CCNode::setPosition(halfWidth - 55.0f, -halfHeight + 40.0f);
    spCash->CCNode::setPosition(halfWidth - 150.0f, -halfHeight + 40.0f);
    
    // Scroll area in center
    scrollArea->CCNode::setPosition(spriteBackground->getPositionX() - 305,
                                    spriteBackground->getPositionY() - 150);
    scrollArea->reposition();
}

void SpriteInfoMenu::refreshAllMenuItemValues()
{
 
    std::stringstream ss;
    
    if (mGameSpriteEnergy != gameSprite->getPossessions()->energyRating)
    {
        mGameSpriteEnergy = gameSprite->getPossessions()->energyRating;
        barEnergy->setValue(mGameSpriteEnergy / (float)mGameSpriteEnergyMax);
    }
    
    if (mGameSpriteEnergyMax != gameSprite->getPossessions()->defaultEnergy)
    {
        mGameSpriteEnergyMax = gameSprite->getPossessions()->defaultEnergy;
        barEnergy->setValue(mGameSpriteEnergy / (float)mGameSpriteEnergyMax);
    }
    
    if (mGameSpriteCash != gameSprite->getPossessions()->cashOnHand)
    {
        mGameSpriteCash = gameSprite->getPossessions()->cashOnHand;
        ss.str(std::string());
        ss << mGameSpriteCash << " G";
        textCash->setString(ss.str().c_str());
    }
    
    if (mGameSpriteEduLvl != gameSprite->getPossessions()->educationLevel)
    {
        mGameSpriteEduLvl = gameSprite->getPossessions()->educationLevel;
        ss.str(std::string());
        ss << "Skill Level: " << mGameSpriteEduLvl;
        textEduLvl->setString(ss.str().c_str());
    }
    if (mGameSpriteInt != gameSprite->getPossessions()->intelligenceRating)
    {
        mGameSpriteInt = gameSprite->getPossessions()->intelligenceRating;
        ss.str(std::string());
        ss << mGameSpriteInt/10;
        textInt->setString(ss.str().c_str());
    }
    if (mGameSpriteLoy != gameSprite->getPossessions()->loyaltyRating)
    {
        mGameSpriteLoy = gameSprite->getPossessions()->loyaltyRating;
        ss.str(std::string());
        ss << mGameSpriteLoy/10;
        textLoy->setString(ss.str().c_str());
    }
    if (mGameSpriteSoc != gameSprite->getPossessions()->socialRating)
    {
        mGameSpriteSoc = gameSprite->getPossessions()->socialRating;
        ss.str(std::string());
        ss << mGameSpriteSoc/10;
        textSoc->setString(ss.str().c_str());
    }
    if (mGameSpriteHap != gameSprite->getPossessions()->happinessRating)
    {
        mGameSpriteHap = gameSprite->getPossessions()->happinessRating;
        ss.str(std::string());
        ss << mGameSpriteHap/10;
        textHap->setString(ss.str().c_str());
    }
    
    if (mGameSpriteExp != gameSprite->getPossessions()->expRating)
    {
        mGameSpriteExp = gameSprite->getPossessions()->expRating;
        barExp->setValue(mGameSpriteExp / (float)mGameSpriteExpMax);
        ss.str(std::string());
        ss << mGameSpriteExp << "/" << mGameSpriteExpMax;
        textExp->setString(ss.str().c_str());
    }
    
    if (mGameSpriteExpMax != gameSprite->getPossessions()->getExpToLevel())
    {
        mGameSpriteExpMax = gameSprite->getPossessions()->getExpToLevel();
        barExp->setValue(mGameSpriteExp / (float)mGameSpriteExpMax);
        ss.str(std::string());
        ss << mGameSpriteExp << "/" << mGameSpriteExpMax;
        textExp->setString(ss.str().c_str());
    }
}

void SpriteInfoMenu::willChangeOrientation()
{
}

void SpriteInfoMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    reposition();
}

void SpriteInfoMenu::update(float deltaTime)
{
    if (gameSprite->isLeavingNextUpdate)
    {
        this->closeMenu(false);
        return;
    }
    refreshAllMenuItemValues();
}
