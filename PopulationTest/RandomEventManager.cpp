//
//  RandomEventManager.cpp
//  PopulationTest
//
//  Created by admin on 8/7/14.
//
//

#include "RandomEventManager.h"
#include "SystemMenu.h"
#include "GameHUD.h"
#include "RandomEventFileReader.h"
#include "RandomEvent.h"
#include "GlobalHelper.h"

RandomEventManager* RandomEventManager::SP;

RandomEventManager::RandomEventManager()
{
    RandomEventManager::SP = this;
    
    active = false;
    
    randomEvents = CCArray::create();
    randomEvents->retain();
}

RandomEventManager::~RandomEventManager()
{
    RandomEventManager::SP = NULL;
    randomEvents->removeAllObjects();
    CC_SAFE_RELEASE(randomEvents);
}

RandomEventManager* RandomEventManager::getThis()
{
    return SP;
}

RandomEventManager* RandomEventManager::create()
{
    RandomEventManager* pRet = new RandomEventManager();
    if(pRet && pRet->init())
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

bool RandomEventManager::init()
{
    readRandomEventsFile();
    createUI();
    return true;
}

void RandomEventManager::readRandomEventsFile()
{
    randomEvents->removeAllObjects();
    randomEvents->release();
    
    RandomEventFileReader* refr = new RandomEventFileReader();
    refr->parseXMLFile("randomEvents.xml");
    
    randomEvents = refr->randomEventsList;
    delete refr;
}

void RandomEventManager::createUI()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    blackScreen = CCSprite::createWithSpriteFrameName("blackscreen.png");
    blackScreen->setAnchorPoint(ccp(0.5f, 0.5f));
    blackScreen->cocos2d::CCNode::setScale(screenSize.width / blackScreen->boundingBox().size.width, screenSize.height / blackScreen->boundingBox().size.height);
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    blackScreen->setVisible(false);
    this->addChild(blackScreen, 1);
    
    background = CCSprite::createWithSpriteFrameName("scrollerbreaknews.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->cocos2d::CCNode::setScale(1.0f, 1.0f);
    background->setPosition(ccp(-screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(background, 2);
}

void RandomEventManager::constructRandomEventContent()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorGreen = ccc3(0, 255.0f, 0);
    ccColor3B colorRed   = ccc3(255.0f, 0, 0);
    
    int total = 0;
    for (int i = 0; i < randomEvents->count(); i++)
    {
        RandomEvent* re = (RandomEvent*) randomEvents->objectAtIndex(i);
        total += re->percentage;
    }
    
    int rollNumber = rand() % total;
    theRandomEvent = NULL;
    for (int i = 0; i < randomEvents->count(); i++)
    {
        RandomEvent* re = (RandomEvent*) randomEvents->objectAtIndex(i);
        if(rollNumber < re->percentage)
        {
            theRandomEvent = re;
            break;
        }
        else
        {
            rollNumber -= re->percentage;
        }
    }
    if(theRandomEvent == NULL)
    {
        theRandomEvent = (RandomEvent*) randomEvents->objectAtIndex(randomEvents->count() - 1);
    }
    
    titleLabel = CCLabelTTF::create(theRandomEvent->event_name.c_str(), "Shojumaru-Regular", 24);
    titleLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    titleLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height * 3.0f / 4.0f));
    titleLabel->setColor(colorBlack);
    background->addChild(titleLabel);
    
    image = CCSprite::createWithSpriteFrameName(theRandomEvent->event_image.c_str());
    image->setAnchorPoint(ccp(0.5f, 0.5f));
    image->setScale(0.6f);
    image->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f + 80.0f));
    background->addChild(image);
    
    descriptionLabel = CCLabelTTF::create(theRandomEvent->event_description.c_str(), "Shojumaru-Regular", 18);
    descriptionLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    descriptionLabel->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f - 40.0f));
    descriptionLabel->setColor(colorBlack);
    background->addChild(descriptionLabel);
    
    vector<string> effects = theRandomEvent->event_effects;
    int index = 0;
    float startY = screenSize.height / 2.0f - 80.0f;
    bool isOdd = false;
    
    for (int i = 0; i < effects.size(); i++)
    {
        string eff = effects.at(i);
        vector<std::string> tokens = GlobalHelper::split(eff, ':');
        string eff_title = tokens[0];
        int eff_value = ::atoi(tokens[1].c_str());
        
        index = i / 2;
        isOdd = ((i % 2) == 1);
        
        stringstream ss;
        ss.str(std::string());
        ss << eff_title << ":";
        CCLabelTTF* tempLabel1 = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16);
        tempLabel1->setAnchorPoint(ccp(0, 0.5f));
        if(!isOdd)
        {
            tempLabel1->setPosition(ccp(screenSize.width / 2.0f - 160.0f, startY - 40.0f * index));
        }
        else
        {
            tempLabel1->setPosition(ccp(screenSize.width / 2.0f + 60.0f, startY - 40.0f * index));
        }
        tempLabel1->setColor(colorBlack);
        background->addChild(tempLabel1);
        
        ss.str(std::string());
        if(eff_value > 0)
        {
            ss << "+";
        }
        ss << eff_value;
        CCLabelTTF* tempLabel2 = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular", 16);
        tempLabel2->setAnchorPoint(ccp(0, 0.5f));
        if(!isOdd)
        {
            tempLabel2->setPosition(ccp(screenSize.width / 2.0f - 100.0f, startY - 40.0f * index));
        }
        else
        {
            tempLabel2->setPosition(ccp(screenSize.width / 2.0f + 120.0f, startY - 40.0f * index));
        }
        
        if(eff_value >= 0)
        {
            tempLabel2->setColor(colorGreen);
        }
        else
        {
            tempLabel2->setColor(colorRed);
        }
        background->addChild(tempLabel2);
    }
    
    okButton = CCMenuItemImage::create("confirm.png", "confirm_press.png", this, menu_selector(RandomEventManager::hideUI));
    okButton->setAnchorPoint(ccp(0.5f, 0.5f));
    okButton->setScale(0.6f);
    okButton->setPosition(ccp(screenSize.width * 1.0f / 4.0f - 10.0f, - screenSize.height * 1.0f / 4.0f + 20.0f));
    
    menu = CCMenu::create(okButton, NULL);
    
    background->addChild(menu);
}

void RandomEventManager::removeRandomEventContent()
{
    if(background != NULL)
    {
        background->removeAllChildrenWithCleanup(true);
    }
}

void RandomEventManager::showUI()
{
    CCLog("test2");
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    active = true;
    
    constructRandomEventContent();
    blackScreen->setVisible(true);
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
}

void RandomEventManager::hideUI()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    active = false;
    
    applyRandomEventEffects();
    removeRandomEventContent();
    blackScreen->setVisible(false);
    GlobalHelper::clearCache();
    
    background->setPosition(ccp(-screenSize.width / 2.0f, screenSize.height / 2.0f));
}

void RandomEventManager::applyRandomEventEffects()
{
    vector<string> effects = theRandomEvent->event_effects;
    for (int i = 0; i < effects.size(); i++)
    {
        string effect = effects.at(i);
        vector<string> tokens = GlobalHelper::split(effect, ':');
        if(tokens[0].compare("gold") == 0)
        {
            int valueChange = ::atoi(tokens[1].c_str());
            GameHUD::getThis()->targetMoney < valueChange ? GameHUD::getThis()->scheduleAddMoney(-GameHUD::getThis()->targetMoney) : GameHUD::getThis()->scheduleAddMoney(valueChange);
        }
        else if(tokens[0].compare("food") == 0)
        {
            int valueChange = ::atoi(tokens[1].c_str());
            CCArray* allGranary = GameScene::getThis()->buildingHandler->granaryOnMap;
            CCArray* allMarket = GameScene::getThis()->buildingHandler->marketOnMap;
            
            if(valueChange < 0)
            {
                for (int i = 0; i < allGranary->count(); i++)
                {
                    Building* granary = (Building*) allGranary->objectAtIndex(i);
                    if(valueChange + granary->currentStorage < 0)
                    {
                        valueChange = valueChange + granary->currentStorage;
                        granary->currentStorage = 0;
                    }
                    else
                    {
                        granary->currentStorage = valueChange + granary->currentStorage;
                        valueChange = 0;
                        break;
                    }
                }
                
                if(valueChange != 0)
                {
                    for (int i = 0; i < allMarket->count(); i++)
                    {
                        Building* market = (Building*) allMarket->objectAtIndex(i);
                        if(valueChange + market->currentStorage < 0)
                        {
                            valueChange = valueChange + market->currentStorage;
                            market->currentStorage = 0;
                        }
                        else
                        {
                            market->currentStorage = valueChange + market->currentStorage;
                            valueChange = 0;
                            break;
                        }
                    }
                }
            }
            else if(valueChange > 0)
            {
                for (int i = 0; i < allGranary->count(); i++)
                {
                    Building* granary = (Building*) allGranary->objectAtIndex(i);
                    if(valueChange + granary->currentStorage > granary->storageLimit)
                    {
                        valueChange = valueChange - (granary->storageLimit - granary->currentStorage);
                        granary->currentStorage = granary->storageLimit;
                    }
                    else
                    {
                        granary->currentStorage = granary->currentStorage + valueChange;
                        valueChange = 0;
                        break;
                    }
                }
                
                if(valueChange != 0)
                {
                    for (int i = 0; i < allMarket->count(); i++)
                    {
                        Building* market = (Building*) allMarket->objectAtIndex(i);
                        if(valueChange + market->currentStorage > market->storageLimit)
                        {
                            valueChange = valueChange - (market->storageLimit - market->currentStorage);
                            market->currentStorage = market->storageLimit;
                        }
                        else
                        {
                            market->currentStorage = market->currentStorage + valueChange;
                            valueChange = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void RandomEventManager::clickSystemButton()
{
    SystemMenu* sm = SystemMenu::create(this);
    sm->retain();
    GameHUD::getThis()->pause = true;
    sm->scheduleShowSystemMenu();
}