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
#include "Building.h"

MiniDragon::MiniDragon()
{
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    dragon = CCSprite::create("dragon_c.png");
    dragon->setScale(screenSize.width / dragon->boundingBox().size.width * 0.2f);
    dragon->setAnchorPoint(ccp(0, 0));
    dragon->setPosition(ccp(0, 0));
    dragon->setFlipX(true);
    TutorialManager::getThis()->addChild(dragon, 2);
    
    bubble = CCSprite::create("bubble.png");
    bubble->cocos2d::CCNode::setScale(dragon->boundingBox().size.width / bubble->boundingBox().size.width * 2.0f, dragon->boundingBox().size.height / bubble->boundingBox().size.height * 1.0f);
    bubble->setAnchorPoint(ccp(0, 1));
    bubble->setPosition(ccp(dragon->boundingBox().size.width * 2.0f / 5.0f, dragon->boundingBox().size.height * 6.5f / 5.0f));
    TutorialManager::getThis()->addChild(bubble, 1);
    
    startX = dragon->boundingBox().size.width * 2.0f / 5.0f + 65.0f;
    startY = dragon->boundingBox().size.height * 5.5f / 5.0f - 20.0f;
    
    offX = 0;
    offY = 0;
    
    delay = 0;
    
    ds = Dragon_Start;
    
    slidesList = CCArray::create();
    slidesList->retain();
    curSlide = 0;
    
    clickToNext = false;
    GameHUD::getThis()->setTutorial = true;
}

MiniDragon::~MiniDragon()
{
    animatedStringList->removeAllObjects();
    
    TutorialManager::getThis()->removeChild(dragon);
    TutorialManager::getThis()->removeChild(bubble);
    CC_SAFE_RELEASE(animatedStringList);
    
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
}

void MiniDragon::playMiniDraggon()
{
    curSlide = 0;
    readTutorialFile();
    constructTutorialSlide();
}

void MiniDragon::readTutorialFile()
{
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
    
    TutorialReader* tr = new TutorialReader();
    tr->parseXMLFile("tutorial.xml");
    
    slidesList = tr->slidesList;
    delete tr;
}

bool MiniDragon::constructTutorialSlide()
{
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        TutorialManager::getThis()->removeChild(as->getLabel());
    }
    animatedStringList->removeAllObjects();
    
    if(curSlide >= slidesList->count())
    {
        return false;
    }
    
    TutorialSlide* ts = (TutorialSlide*) slidesList->objectAtIndex(curSlide);
    
    TutorialManager::getThis()->clickable = false;
    clickToNext = false;
    
    offX = 0;
    offY = 0;
    
    string str = ts->speech;
    
    bool cameraMove = ts->cameraMove;
    bool dragonMove = ts->dragonMove;
    CCPoint target = ts->target;
    CCPoint moveVector = ts->moveVector;
    clickToNext = ts->clickToNext;
    TutorialManager* tm = TutorialManager::getThis();
    
    if(cameraMove)
    {
        moveCamera(target);
    }
    
    if(dragonMove)
    {
        move(moveVector);
    }
    
    if(ts->fade_in)
    {
        fadeInScreen(ts->target_opacity);
    }
    
    if(ts->fade_out)
    {
        fadeOutScreen(ts->target_opacity);
    }
    
    if(ts->deHighlight)
    {
        deHighlightBuilding();
    }
    
    if(ts->highlight)
    {
        highlightBuilding(ts->highlightBuilding);
    }
    
    for(int i = 0; i < ts->commands.size(); i++)
    {
        string command = ts->commands.at(i);
        vector<string> temps = GlobalHelper::split(command, ':');
        string order = temps.at(0);
        int flag = ::atoi(temps.at(1).c_str());
        
        if(order.compare("lockMap") == 0)
        {
            tm->lockMap = (flag == 1);
        }
        else if(order.compare("lockVillager") == 0)
        {
            tm->lockVillager = (flag == 1);
        }
        else if(order.compare("active") == 0)
        {
            tm->active = (flag == 1);
        }
        else if(order.compare("lockBuildButton") == 0)
        {
            tm->lockBuildButton = (flag == 1);
        }
        else if(order.compare("lockObjectiveButton") == 0)
        {
            tm->lockObjectiveButton = (flag == 1);
        }
        else if(order.compare("lockSystemButton") == 0)
        {
            tm->lockSystemButton = (flag == 1);
        }
        else if(order.compare("lockTimeGroup") == 0)
        {
            tm->lockTimeGroup = (flag == 1);
        }
        else if(order.compare("lockScroll") == 0)
        {
            tm->lockScroll = (flag == 1);
        }
        else if(order.compare("lockBuildScroll") == 0)
        {
            tm->lockBuildScroll = (flag == 1);
        }
        else if(order.compare("lockBuildingButton") == 0)
        {
            tm->lockBuildingButton = (flag == 1);
        }
        else if(order.compare("lockPopup") == 0)
        {
            tm->lockPopup = (flag == 1);
        }
        else if(order.compare("lockButtonOk") == 0)
        {
            tm->lockButtonOk = (flag == 1);
        }
        else if(order.compare("lockButtonCancel") == 0)
        {
            tm->lockButtonCancel = (flag == 1);
        }
        else if(order.compare("lockButtonClose") == 0)
        {
            tm->lockButtonClose = (flag == 1);
        }
        else if(order.compare("lockManpowerSelect") == 0)
        {
            tm->lockManpowerSelect = (flag == 1);
        }
        else if(order.compare("teachBuildButton") == 0)
        {
            tm->teachBuildButton = (flag == 1);
        }
        else if(order.compare("teachBuildHouse") == 0)
        {
            tm->teachBuildHouse = (flag == 1);
        }
        else if(order.compare("teachFarming") == 0)
        {
            tm->teachFarming = (flag == 1);
        }
        else if(order.compare("teachBuildRoad") == 0)
        {
            tm->teachBuildRoad = (flag == 1);
        }
    }
    
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
    return true;
}

void MiniDragon::moveCamera(CCPoint target)
{
    TutorialManager::getThis()->target = target;
    TutorialManager::getThis()->schedule(schedule_selector(TutorialManager::moveCamera), 1.0f / 120.0f);
}

void MiniDragon::display()
{
    
}

/*
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
    ds = (Dragon_State)((int)ds + 1);
    
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
                scheduleSenario();
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
            TutorialManager::getThis()->unlockAll();
            TutorialManager::getThis()->scheduleOnce(schedule_selector( MiniDragon::scheduleSenario ), 360);
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
 */

void MiniDragon::finishDisplay()
{
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        as->getLabel()->setOpacity((GLubyte) 255);
    }
}

void MiniDragon::scheduleSenario()
{
    TutorialManager::getThis()->active = false;
    TutorialManager::getThis()->unlockAll();
    GameHUD::getThis()->pause = true;
    std::string filename = "tutorial.xml";
    Senario::getThis()->playSenario(filename.c_str());
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
                TutorialManager::getThis()->clickable = true;
            }
        }
        else
        {
            TutorialManager::getThis()->unschedule( schedule_selector( MiniDragon::update ) );
            TutorialManager::getThis()->clickable = true;
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

void MiniDragon::fadeInScreen(float targetOpacity)
{
    TutorialManager::getThis()->scheduleFadeIn(targetOpacity, 10);
}

void MiniDragon::fadeOutScreen(float targetOpacity)
{
    TutorialManager::getThis()->scheduleFadeOut(targetOpacity, 10);
}

void MiniDragon::highlightBuilding(string b)
{
    CCArray* buildingsOnMap;
    if(b.compare("special") == 0)
    {
        buildingsOnMap = GameScene::getThis()->buildingHandler->specialOnMap;
    }
    else if(b.compare("housing") == 0)
    {
        buildingsOnMap = GameScene::getThis()->buildingHandler->housingOnMap;
    }
    else if(b.compare("granary") == 0)
    {
        buildingsOnMap = GameScene::getThis()->buildingHandler->granaryOnMap;
    }
    else if(b.compare("amenity") == 0)
    {
        buildingsOnMap = GameScene::getThis()->buildingHandler->amenityOnMap;
    }
    else if(b.compare("military") == 0)
    {
        buildingsOnMap = GameScene::getThis()->buildingHandler->militaryOnMap;
    }
    else
    {
        if(b.compare("buildButton") == 0)
        {
            GameHUD::getThis()->buildButton->setZOrder(30);
        }
        return;
    }
    
    if (buildingsOnMap != NULL && buildingsOnMap > 0)
    {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        TutorialManager::getThis()->highlightedBuilding = (Building*) buildingsOnMap->objectAtIndex(0);
        TutorialManager::getThis()->highlightedBuildingPos = TutorialManager::getThis()->highlightedBuilding->buildingRep->getPosition();
        TutorialManager::getThis()->highlightedBuildingZOrder = TutorialManager::getThis()->highlightedBuilding->buildingRep->getZOrder();
        
        TutorialManager::getThis()->highlightedBuilding->EndAnim();
        TutorialManager::getThis()->highlightedBuilding->buildingRep->retain();
        GameScene::getThis()->mapHandler->getMap()->removeChild(TutorialManager::getThis()->highlightedBuilding->buildingRep);
        
        TutorialManager::getThis()->highlightedBuilding->buildingRep->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
        TutorialManager::getThis()->addChild(TutorialManager::getThis()->highlightedBuilding->buildingRep);
        
        TutorialManager::getThis()->highlightedBuilding->buildingRep->setZOrder(100);
    }
}

void MiniDragon::deHighlightBuilding()
{
    TutorialManager::getThis()->highlightedBuilding->buildingRep->setPosition(TutorialManager::getThis()->highlightedBuildingPos);
    TutorialManager::getThis()->highlightedBuilding->buildingRep->setZOrder((int) TutorialManager::getThis()->highlightedBuildingZOrder);
    TutorialManager::getThis()->removeChild(TutorialManager::getThis()->highlightedBuilding->buildingRep);
    GameScene::getThis()->mapHandler->getMap()->addChild(TutorialManager::getThis()->highlightedBuilding->buildingRep);
    
    TutorialManager::getThis()->highlightedBuilding->buildingRep->autorelease();
    TutorialManager::getThis()->highlightedBuilding->BeginAnim();
    
    TutorialManager::getThis()->highlightedBuilding = NULL;
    TutorialManager::getThis()->highlightedBuildingPos = CCPointZero;
    TutorialManager::getThis()->highlightedBuildingZOrder = 0;
}

void MiniDragon::clickNext()
{
    curSlide++;
    if(constructTutorialSlide())
    {
        
    }
    else
    {
        // no more tutorial slide already
    }
}

