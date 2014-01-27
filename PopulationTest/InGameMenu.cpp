//
//  InGameMenu.cpp
//  PopulationTest
//
//  Created by Serious Games on 15/3/13.
//
//

#include "InGameMenu.h"
#include "GameHUD.H"
#include "BuildMenu.h"
#include "ResearchMenu.h"

#include "SystemMenu.h"
#include "PopulationMenu.h"
#include "InfoMenu.h"
#include "AlertBox.h"
#include "PolicyMenu.h"

#include "TutorialHandler.h"

#include "SoundtrackManager.h"

InGameMenu::~InGameMenu()
{
    
}

void InGameMenu::cleanup()
{
    //removeChild(researchIndicator);
   // delete researchIndicator;

    PopupMenu::cleanup();
}

InGameMenu* InGameMenu::create()
{
    InGameMenu *pRet = new InGameMenu();
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

void InGameMenu::createMenuItems()
{
 //   CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    /*
    CCSprite* buildImage =          CCSprite::create("game-menu-mockup-buttons-build.png");
    CCSprite* researchImage =       CCSprite::create("game-menu-mockup-buttons-research.png");
    CCSprite* populationImage =     CCSprite::create("game-menu-mockup-buttons-pop.png");
    CCSprite* policyImage =         CCSprite::create("game-menu-mockup-buttons-policy.png");
    CCSprite* infoImage =           CCSprite::create("game-menu-mockup-buttons-info.png");
    CCSprite* systemImage =         CCSprite::create("game-menu-mockup-buttons-system.png");
    CCSprite* exitImage =           CCSprite::create("game-menu-mockup-buttons-exit.png");
*/
    
    CCSprite* buildImage =          CCSprite::create("button-side.png");
    /*
    CCSprite* researchImage =       CCSprite::create("button-side.png");
    CCSprite* populationImage =     CCSprite::create("button-side.png");
    CCSprite* policyImage =         CCSprite::create("button-side.png");
    CCSprite* infoImage =           CCSprite::create("button-side.png");
    CCSprite* systemImage =         CCSprite::create("button-side.png");*/
    CCSprite* exitImage =           CCSprite::create("button-side.png");

    
    menuItemsBuild      = CCMenuItemSprite::create(buildImage, buildImage, this, menu_selector(InGameMenu::onMenuItemSelected) );
    /*
    menuItemsResearch   = CCMenuItemSprite::create(researchImage, researchImage, this, menu_selector(InGameMenu::onMenuItemSelected) );
    menuItemsPopulation = CCMenuItemSprite::create(populationImage, populationImage, this, menu_selector(InGameMenu::onMenuItemSelected) );
    menuItemsPolicy     = CCMenuItemSprite::create(policyImage, policyImage, this, menu_selector(InGameMenu::onMenuItemSelected) );
    menuItemsInfo       = CCMenuItemSprite::create(infoImage, infoImage, this, menu_selector(InGameMenu::onMenuItemSelected) );
    menuItemsSystem     = CCMenuItemSprite::create(systemImage, systemImage, this, menu_selector(InGameMenu::onMenuItemSelected) );*/
    menuItemsExit       = CCMenuItemSprite::create(exitImage, exitImage, this, menu_selector(InGameMenu::onMenuItemSelected) );
    
    
    /*menu item texts*/
    CCLabelTTF* buildLabel = CCLabelTTF::create("BUILD", "Verdana" ,24, menuItemsBuild->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    buildLabel->setPosition( ccp(menuItemsBuild->getPositionX() + menuItemsBuild->boundingBox().size.width * 0.5f, menuItemsBuild->getPositionY() + menuItemsBuild->boundingBox().size.height * 0.55f));
    menuItemsBuild->addChild(buildLabel);
/*
    CCLabelTTF* researchLabel = CCLabelTTF::create("RESEARCH", "Verdana" ,24, menuItemsResearch->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    researchLabel->setPosition( ccp(menuItemsResearch->getPositionX() + menuItemsResearch->boundingBox().size.width * 0.5f, menuItemsResearch->getPositionY() + menuItemsResearch->boundingBox().size.height * 0.55f));
    menuItemsResearch->addChild(researchLabel);

    CCLabelTTF* populationLabel = CCLabelTTF::create("POPULATION", "Verdana" ,24, menuItemsPopulation->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    populationLabel->setPosition( ccp(menuItemsPopulation->getPositionX() + menuItemsPopulation->boundingBox().size.width * 0.5f, menuItemsPopulation->getPositionY() + menuItemsPopulation->boundingBox().size.height * 0.55f));
    menuItemsPopulation->addChild(populationLabel);

    CCLabelTTF* policyLabel = CCLabelTTF::create("POLICY", "Verdana" ,24, menuItemsPolicy->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    policyLabel->setPosition( ccp(menuItemsPolicy->getPositionX() + menuItemsPolicy->boundingBox().size.width * 0.5f, menuItemsPolicy->getPositionY() + menuItemsPolicy->boundingBox().size.height * 0.55f));
    menuItemsPolicy->addChild(policyLabel);

    CCLabelTTF* infoLabel = CCLabelTTF::create("INFO", "Verdana" ,24, menuItemsInfo->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    infoLabel->setPosition( ccp(menuItemsInfo->getPositionX() + menuItemsInfo->boundingBox().size.width * 0.5f, menuItemsInfo->getPositionY() + menuItemsInfo->boundingBox().size.height * 0.55f));
    menuItemsInfo->addChild(infoLabel);

    CCLabelTTF* systemLabel = CCLabelTTF::create("SYSTEM", "Verdana" ,24, menuItemsSystem->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    systemLabel->setPosition( ccp(menuItemsInfo->getPositionX() + menuItemsSystem->boundingBox().size.width * 0.5f, menuItemsInfo->getPositionY() + menuItemsSystem->boundingBox().size.height * 0.55f));
    menuItemsSystem->addChild(systemLabel);*/

    CCLabelTTF* exitLabel = CCLabelTTF::create("EXIT", "Verdana" ,24, menuItemsExit->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // menuLabel->setAnchorPoint(ccp(0.5f, 0.5));
    exitLabel->setPosition( ccp(menuItemsExit->getPositionX() + menuItemsExit->boundingBox().size.width * 0.5f, menuItemsExit->getPositionY() + menuItemsExit->boundingBox().size.height * 0.55f));
    menuItemsExit->addChild(exitLabel);
    
    
    
    //2 is retina, 1 is normal
    if (CC_CONTENT_SCALE_FACTOR() == 1) {
        menuItemsBuild->        setScale(0.67);
        /*
        menuItemsResearch->     setScale(0.67);
        menuItemsPopulation->   setScale(0.67);
        menuItemsPolicy->       setScale(0.67);
        menuItemsInfo->         setScale(0.67);
        menuItemsSystem->       setScale(0.67);*/
        menuItemsExit->         setScale(0.67);
    }
    
    menuItems->addObject(menuItemsBuild);
   //menuItems->addObject(menuItemsResearch);
    //menuItems->addObject(menuItemsPolicy);
    
    
    GameManager* m = GameManager::getThis();
    /*
    if (!m->isMenuItemUnlocked("menu_policy"))
    {
        menuItemsPolicy->setEnabled(false);
        menuItemsPolicy->setColor(ccGRAY);
        
    }
    
    if (!m->isMenuItemUnlocked("menu_research"))
    {
        menuItemsResearch->setEnabled(false);
        menuItemsResearch->setColor(ccGRAY);
        
    }*/
    
    
  //  menuItems->addObject(menuItemsPopulation);
   // menuItems->addObject(menuItemsInfo);
   // menuItems->addObject(menuItemsSystem);
    menuItems->addObject(menuItemsExit);
    
    for (int x = 0; x < menuItems->count(); x++) {
        CCMenuItemSprite* thatItem = (CCMenuItemSprite*)menuItems->objectAtIndex(x);
        thatItem->setPosition(ccp(GameHUD::getThis()->getMenuButton()->getPositionX(), GameHUD::getThis()->getMenuButton()->getPositionY() + menuItemsBuild->boundingBox().size.height *0.8f * (7 -x)));
        thatItem->setTag(x);
    }
    
    CCMenu* menu = CCMenu::createWithArray(menuItems);
    menu->setPosition( CCPointZero );
    
    /*
    researchIndicator = new ResearchIndicator();
    researchIndicator->createResearchIndicator();
    researchIndicator->setPosition(screenSize.width - 350.0f, screenSize.height - 200.0f);
    */
    //this->addChild(researchIndicator);
    this->addChild(menu, 2);
}

void InGameMenu::onMenuItemSelected(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    
    switch (tag)
    {
        case 0: // Build
        {
            CCLog("Build");
            PopupMenu* buildMenu = new BuildMenu();
            buildMenu->useAsExclusivePopupMenu();
            GameHUD::getThis()->miscLabel->setString("Back");
            
            GameHUD::getThis()->setMenuMode(2);
        }
        break;
           /*
        case 1: //Research
        {
            CCLog("Research");
            PopupMenu* researchMenu = new ResearchMenu();
            researchMenu->useAsExclusivePopupMenu();
            GameHUD::getThis()->miscLabel->setString("Back");
            
            GameHUD::getThis()->setMenuMode(3);
        }
        break;
            
        case 2: //Policy
        {
            
            CCLog("Policy");
            PopupMenu* policyMenu = new PolicyMenu();
            policyMenu->useAsExclusivePopupMenu();
            
            GameHUD::getThis()->miscLabel->setString("Back");
            GameHUD::getThis()->setMenuMode(5);
           
        }
        break;
      
        case 3: //Population
        {
            CCLog("Population");
            PopupMenu* populationMenu = new PopulationMenu();
            populationMenu->useAsExclusivePopupMenu();
            GameHUD::getThis()->miscLabel->setString("Back");
            
            GameHUD::getThis()->setMenuMode(4);
            
        }
        break;
            
        case 4: //Info
        {
            CCLog("Info");
            PopupMenu* infoMenu = new InfoMenu();
            infoMenu->useAsExclusivePopupMenu();
            GameHUD::getThis()->miscLabel->setString("Back");
            
            GameHUD::getThis()->setMenuMode(6);
        }
        break;
                    
        case 5: //System
        {
            CCLog("System");
            PopupMenu* systemMenu = new SystemMenu();
            systemMenu->useAsExclusivePopupMenu();
            GameHUD::getThis()->miscLabel->setString("Back");
            GameHUD::getThis()->setMenuMode(7);
        }
        break;*/
            //was 6
        case 1: //Exit
        {
            AlertBox* alert = AlertBox::create();
            alert->useAsTopmostPopupMenu();
            alert->setDisplayText("Are you sure you want to exit the game?");
            
            alert->addButton(0, "Yes", this, callfuncO_selector(InGameMenu::exitGame), pSender);
            alert->addButton(-1, "No");
        }
        break;
                        
        default:
        {
        }
        break;
    }
}

void InGameMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
   // CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    //No need to reposition this
    /*
    for (int x = 0; x < menuItems->count(); x++) {
        CCMenuItemSprite* thatItemImage = (CCMenuItemSprite*)menuItems->objectAtIndex(x);
        CCPoint oldPosition = menuItemPositions->getControlPointAtIndex(x);
        CCPoint newPosition;
        newPosition = ccp((float)oldPosition.x / 100.0f * screenSize.width, (float)oldPosition.y / 100.0f * screenSize.height);
        thatItemImage->setPosition(newPosition);
    }*/
    
   // researchIndicator->setPosition(screenSize.width - 350.0f, screenSize.height - 200.0f);
}
/*
void InGameMenu::scrollY(float scrollBy)
{
}*/

void InGameMenu::exitGame(CCObject* pSender)
{
    GameManager::getThis()->firstPlay = false;
    GameManager::getThis()->saveGameData();
    closeAllPopupMenu(true);
    
    SoundtrackManager::PlayBGM("Tikopia.mp3");
    
    CCDirector::sharedDirector()->popScene();
}


