//
//  MiniDragon.cpp
//  PopulationTest
//
//  Created by AIXUE on 2/4/14.
//
//

#include "MiniDragon.h"
#include "TutorialManager.h"
#include "GlobalHelper.h"
#include "AnimatedString.h"
#include "GameScene.h"
#include "GameHUD.h"
#include "Senario.h"

MiniDragon::MiniDragon()
{
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    dragon = CCSprite::create("dragon_c.png");
    dragon->setScale(screenSize.width / dragon->boundingBox().size.width * 0.2f);
    dragon->setAnchorPoint(ccp(0, 0));
    dragon->setPosition(ccp(0, 0));
    TutorialManager::getThis()->addChild(dragon, 2);
    
    bubble = CCSprite::create("bubble.png");
    bubble->cocos2d::CCNode::setScale(dragon->boundingBox().size.width / bubble->boundingBox().size.width * 2.0f, dragon->boundingBox().size.height / bubble->boundingBox().size.height * 0.4f);
    bubble->setAnchorPoint(ccp(0, 1));
    bubble->setPosition(ccp(dragon->boundingBox().size.width * 2.0f / 5.0f, dragon->boundingBox().size.height * 5.5f / 5.0f));
    TutorialManager::getThis()->addChild(bubble, 1);
    
    startX = dragon->boundingBox().size.width * 2.0f / 5.0f + 65.0f;
    startY = dragon->boundingBox().size.height * 5.5f / 5.0f - 20.0f;
    
    offX = 0;
    offY = 0;
    
    autoJump = false;
    
    delay = 0;
    
    ds = Dragon_Start;
}

MiniDragon::~MiniDragon()
{
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
    
    TutorialManager::getThis()->removeChild(dragon);
    TutorialManager::getThis()->removeChild(bubble);
}

void MiniDragon::display()
{
    offX = 0;
    offY = 0;
    
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        TutorialManager::getThis()->removeChild(as->getLabel());
    }
    animatedStringList->removeAllObjects();
    ds++;
    
    string str = "";
    
    bool goDisplay = true;
    bool fDisplay = false;
    switch (ds)
    {
        case D1T1:
            str = "Try to build a new house for the last refugee. Tap on the build menu button.";
            autoJump = true;
            break;
        case D1T2:
        {
            if(GameScene::getThis()->systemConfig->skipTutorial)
            {
                TutorialManager::getThis()->scheduleFadeIn(0, 1);
                TutorialManager::getThis()->unlockAll();
                TutorialManager::getThis()->active = false;
                move(ccp(0, -500));
                return;
            }
            else
            {
                str = "Try to build a new house for the last refugee. Tap on the build menu button.";
                fDisplay = true;
                TutorialManager::getThis()->scheduleFadeOut(160.0f, 1.0f);
                autoJump = false;
                GameHUD::getThis()->buildButton->setZOrder(30);
                GameHUD::getThis()->setTutorial = true;
                TutorialManager::getThis()->lockMap = false;
                TutorialManager::getThis()->lockBuildButton = false;
                TutorialManager::getThis()->teachBuildButton = true;
            }
        }
            break;
        case D1T3:
            str = "Tap a house from the build screen.";
            break;
        case D1T4:
            str = "Tap on any empty plot of land! Red preview means cannot build there.";
            TutorialManager::getThis()->scheduleFadeIn(0, 1.0f);
            GameHUD::getThis()->buildButton->setZOrder(1);
            TutorialManager::getThis()->lockBuildButton = true;
            TutorialManager::getThis()->teachBuildButton = false;
            TutorialManager::getThis()->lockScroll = false;
            move(ccp(0, -220));
            break;
        case D1T5:
            str = "Tap again to confirm building location. Tap on another area to change the building location.";
            break;
        case D2T1:
            str = "In order to build, or work, you need to allocate manpower for the activity.";
            autoJump = true;
            break;
        case D2T2:
            str = "Tap on the designated building to open up the allocation screen.";
            autoJump = false;
            TutorialManager::getThis()->lockPopup = false;
            break;
        case D2T3:
            str = "Tap on the villagers to allocate them and tap again to deallocate them.";
            TutorialManager::getThis()->lockScroll = true;
            TutorialManager::getThis()->scheduleFadeOut(160, 1);
            TutorialManager::getThis()->lockManpowerSelect = false;
            break;
        case D2T4:
            str = "Tap on the allocate button to confirm the allocation and commerce the activity.";
            TutorialManager::getThis()->lockManpowerSelect = true;
            TutorialManager::getThis()->lockButtonOk = false;
            offY = 100;
            break;
        case D2T5:
            str = "Tap on the cancel button to halt the activeity and return you 75% of the building cost.";
            TutorialManager::getThis()->lockButtonOk = true;
            TutorialManager::getThis()->lockButtonCancel = false;
            offY = 100;
            break;
        case D2T6:
            str = "Now, tap on the allocate button to start the building process.";
            TutorialManager::getThis()->lockButtonCancel = true;
            TutorialManager::getThis()->lockButtonOk = false;
            offY = 100;
            break;
        case D2T7:
            str = "Wait for building to be completed, the remaining refugee will find the new home.";
            TutorialManager::getThis()->lockButtonOk = true;
            TutorialManager::getThis()->scheduleFadeIn(0, 1);
            TutorialManager::getThis()->lockScroll = false;
            TutorialManager::getThis()->teachBuildHouse = false;
            move(ccp(0, 100));
            autoJump = true;
            break;
        case D2T8:
            move(ccp(0, -500));
            TutorialManager::getThis()->teachFarming = true;
            autoJump = false;
            break;
        case D3T1:
            str = "Now you need to produce food to sustain your population.";
            move(ccp(0, 500));
            TutorialManager::getThis()->lockScroll = true;
            TutorialManager::getThis()->scheduleFadeOut(160, 1);
            autoJump = true;
            GameHUD::getThis()->foodIcon->setZOrder(30);
            GameHUD::getThis()->foodLabel->setZOrder(31);
            break;
        case D3T2:
            str = "Now you need to produce food to sustain your population.";
            autoJump = false;
            break;
        case D3T3:
            str = "Tap on the farm to see the stats of the farm.";
            TutorialManager::getThis()->lockScroll = false;
            TutorialManager::getThis()->scheduleFadeIn(0, 1);
            GameHUD::getThis()->foodIcon->setZOrder(2);
            GameHUD::getThis()->foodLabel->setZOrder(2);
            TutorialManager::getThis()->lockPopup = false;
            break;
        case D3T4:
            str = "Tap on Allocate to open up the allocate screen.";
            TutorialManager::getThis()->lockScroll = true;
            break;
        case D3T5:
            str = "Allocate manpower for the farming process.";
            offY = 100;
            TutorialManager::getThis()->lockManpowerSelect = false;
            TutorialManager::getThis()->lockButtonOk = false;
            break;
        case D3T6:
            str = "After farming is completed, food will be transported to the granary.";
            move(ccp(0, 100));
            TutorialManager::getThis()->lockManpowerSelect = true;
            TutorialManager::getThis()->lockButtonOk = true;
            TutorialManager::getThis()->lockScroll = false;
            autoJump = true;
            break;
        case D3T7:
            move(ccp(0, -500));
            autoJump = false;
            break;
        case D3T8:
        {
            str = "If the villages keep happy, they will have chance to drop reputatoin tokens, collect them by tapping on them.";
            move(ccp(0, 500));
            CCArray* sprites = GameScene::getThis()->spriteHandler->spritesOnMap;
            for(int i = 0; i < sprites->count(); i++)
            {
                GameSprite* gs = (GameSprite*) sprites->objectAtIndex(i);
                gs->dropToken(true);
            }
            autoJump = true;
        }
            break;
        case D4T1:
            str = "Be careful, reputation tokens will disappear after a period of time, collect them as soon as possible.";
            autoJump = true;
            delay = 15;
            break;
        case D4T2:
            str = "Well done, now you know how to build our village already, keep it.";
            delay = 10;
            break;
        case D4T3:
            move(ccp(0, -500));
            autoJump = false;
            cumulativeTime = 0;
            TutorialManager::getThis()->schedule(schedule_selector( MiniDragon::scheduleSenario ), 1.0f / 120.0f);
            break;
        default:
            break;
    }
    
    if(goDisplay)
    {
        ccColor3B color = ccc3(0, 0, 0);
        
        vector<std::string> tokens = GlobalHelper::split(str, ' ');
        
        float flashTimeGap = 0.05f;
        int flashGapCount = 0;
        
        for (int i = 0; i < tokens.size(); i++)
        {
            std::string tokenStr = tokens.at(i);
            CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), "Shojumaru-Regular", 12);
            tempLabel->retain();
            
            if (startX + offX + tempLabel->boundingBox().size.width > 400.0f)
            {
                offY = offY + 15.0f;
                offX = 0;
            }
            
            CC_SAFE_RELEASE(tempLabel);
            
            for (int j = 0; j < tokenStr.length(); j++)
            {
                string tempStr = tokenStr.substr(j, 1);
                AnimatedString* as = AnimatedString::create(tempStr, flashTimeGap * (j + flashGapCount), "Shojumaru-Regular", 12, 80.0f);
                as->getLabel()->setColor(color);
                as->getLabel()->setAnchorPoint(ccp(0, 1));
                
                as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
                offX += as->label->boundingBox().size.width;
                
                TutorialManager::getThis()->addChild(as->getLabel(), 20);
                animatedStringList->addObject(as);
            }
            
            flashGapCount += tokenStr.size();
            offX += 10;
        }
        
        TutorialManager::getThis()->schedule( schedule_selector( MiniDragon::update ), 1.0f / 120.0f );
        //text->setString(ss.str().c_str());
        
        if(fDisplay)
        {
            finishDisplay();
        }
    }
}

void MiniDragon::finishDisplay()
{
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        as->getLabel()->setOpacity((GLubyte) 255);
    }
}

void MiniDragon::scheduleSenario(float dt)
{
    if(cumulativeTime < 300)
    {
        cumulativeTime += dt;
    }
    else
    {
        TutorialManager::getThis()->unschedule(schedule_selector( MiniDragon::scheduleSenario ));
        TutorialManager::getThis()->active = false;
        TutorialManager::getThis()->unlockAll();
        GameHUD::getThis()->pause = true;
        std::string filename = "tutorial.xml";
        Senario::getThis()->playSenario(filename.c_str());
    }
}

void MiniDragon::update(float dt)
{
    bool end = true;
    CCArray* list = TutorialManager::getThis()->miniDragon->animatedStringList;
    for(int i = 0; i < list->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) list->objectAtIndex(i);
        as->update(dt);
        if(((float)as->getLabel()->getOpacity()) < 255)
        {
            end = false;
        }
    }
    
    if(end)
    {
        if(delay > 0)
        {
            delay -= dt;
            if(delay < 0)
            {
                delay = 0;
                TutorialManager::getThis()->unschedule( schedule_selector( MiniDragon::update ) );
                if(TutorialManager::getThis()->miniDragon->autoJump)
                {
                    TutorialManager::getThis()->miniDragon->display();
                }
            }
        }
        else
        {
            TutorialManager::getThis()->unschedule( schedule_selector( MiniDragon::update ) );
            if(TutorialManager::getThis()->miniDragon->autoJump)
            {
                TutorialManager::getThis()->miniDragon->display();
            }
        }
    }
}

void MiniDragon::move(CCPoint vec)
{
    dragon->setPosition(ccp(dragon->getPositionX() + vec.x, dragon->getPositionY() + vec.y));
    bubble->setPosition(ccp(bubble->getPositionX() + vec.x, bubble->getPositionY() + vec.y));
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        as->getLabel()->setPosition(ccp(as->getLabel()->getPositionX() + vec.x, as->getLabel()->getPositionY() + vec.y));
    }
}

