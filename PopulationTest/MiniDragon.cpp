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
#include "FloatingArraw.h"

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
    
    bubble = CCSprite::create("speechdialog.png");
    bubble->cocos2d::CCNode::setScale(dragon->boundingBox().size.width / bubble->boundingBox().size.width * 2.0f, dragon->boundingBox().size.height / bubble->boundingBox().size.height * 1.0f);
    bubble->setAnchorPoint(ccp(0, 1));
    bubble->setPosition(ccp(dragon->boundingBox().size.width * 1.0f / 5.0f, dragon->boundingBox().size.height * 8.0f / 5.0f));
    TutorialManager::getThis()->addChild(bubble, 1);
    
    startX = dragon->boundingBox().size.width * 1.0f / 5.0f + 65.0f;
    startY = dragon->boundingBox().size.height * 7.5f / 5.0f - 30.0f;
    
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
        if(TutorialManager::getThis()->hide)
        {
            as->getLabel()->autorelease();
            GameHUD::getThis()->removeChild(as->getLabel());
        }
        else
        {
            TutorialManager::getThis()->removeChild(as->getLabel());
        }
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
    
    TutorialManager::getThis()->hide = ts->hide;
    TutorialManager::getThis()->show = ts->show;
    
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
        deHighlightBuilding(ts->deHighlightBuilding);
    }
    
    if(ts->highlight)
    {
        highlightBuilding(ts->highlightBuilding);
    }
    
    if(TutorialManager::getThis()->show)
    {
        GameHUD::getThis()->removeChild(dragon);
        GameHUD::getThis()->removeChild(bubble);
        TutorialManager::getThis()->addChild(dragon, 2);
        TutorialManager::getThis()->addChild(bubble, 1);
        dragon->autorelease();
        bubble->autorelease();
    }
    
    if(ts->triggerDropToken)
    {
        CCArray* sprites = GameScene::getThis()->spriteHandler->spritesOnMap;
        for(int i = 0; i < sprites->count(); i++)
        {
            GameSprite* gs = (GameSprite*) sprites->objectAtIndex(i);
            gs->dropToken(true);
        }

    }
    
    if(ts->unlockAll)
    {
        cumulativeTime = 0;
        TutorialManager::getThis()->unlockAll();
        TutorialManager::getThis()->scheduleOnce(schedule_selector( MiniDragon::scheduleSenario ), 360);
    }
    
    if(ts->hideBubble)
    {
        bubble->setVisible(false);
        dragon->setVisible(false);
        TutorialManager::getThis()->setupNarrator();
    }
    
    if(ts->showArrow)
    {
        FloatingArraw::getThis()->showArrow(ccp(ts->arrowX, ts->arrowY), ts->arrowAngle, ts->arrowScale, ts->arrowLayer);
    }
    
    if(ts->hideArrow)
    {
        FloatingArraw::getThis()->hideArrow();
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
        else if(order.compare("pause") == 0)
        {
            GameHUD::getThis()->pause = (flag == 1);
        }
        else if(order.compare("spriteInfo") == 0)
        {
            TutorialManager::getThis()->lockSpriteInfo = (flag == 1);
        }
        else if(order.compare("buildingInfo") == 0)
        {
            TutorialManager::getThis()->lockBuildingInfo = (flag == 1);
        }
        else if(order.compare("goldLabel") == 0)
        {
            TutorialManager::getThis()->lockGoldLabel = (flag == 1);
        }
        else if(order.compare("foodLabel") == 0)
        {
            TutorialManager::getThis()->lockFoodLabel = (flag == 1);
        }
        else if(order.compare("populationLabel") == 0)
        {
            TutorialManager::getThis()->lockPopulationLabel = (flag == 1);
        }
    }
    
    if(ts->addVillager > 0)
    {
        TutorialManager::getThis()->addVillagers(ts->addVillager);
    }
    
    ccColor3B color = ccc3(0, 0, 0);
    
    vector<std::string> tokens = GlobalHelper::split(str, ' ');
    
    float flashTimeGap = 0.05f;
    int flashGapCount = 0;
    
    float offsetY = ts->offY;
    
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
            
            as->getLabel()->setPosition(ccp(startX + offX, startY - offY + offsetY));
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
                if(TutorialManager::getThis()->hide)
                {
                    TutorialManager::getThis()->miniDragon->hideDragonGroup();
                }
            }
        }
        else
        {
            TutorialManager::getThis()->unschedule( schedule_selector( MiniDragon::update ) );
            TutorialManager::getThis()->clickable = true;
            if(TutorialManager::getThis()->hide)
            {
                TutorialManager::getThis()->miniDragon->hideDragonGroup();
            }
        }
    }
}

void MiniDragon::hideDragonGroup()
{
    dragon->retain();
    bubble->retain();
    TutorialManager::getThis()->removeChild(dragon);
    TutorialManager::getThis()->removeChild(bubble);
    GameHUD::getThis()->addChild(dragon, 32);
    GameHUD::getThis()->addChild(bubble, 31);
    
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        CCLabelTTF* temp = ((AnimatedString*) animatedStringList->objectAtIndex(i))->getLabel();
        temp->retain();
        TutorialManager::getThis()->removeChild(temp);
        GameHUD::getThis()->addChild(temp, 33);
    }
}

void MiniDragon::showAllSpeech()
{
    CCArray* list = TutorialManager::getThis()->miniDragon->animatedStringList;
    for(int i = 0; i < list->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) list->objectAtIndex(i);
        as->getLabel()->setOpacity((GLubyte) 255);
    }
}

void MiniDragon::move(CCPoint vec)
{
    if(vec.y > 0)
    {
        dragon->setPosition(ccp(dragon->getPositionX() + vec.x, 220));
        bubble->setPosition(ccp(bubble->getPositionX() + vec.x, dragon->boundingBox().size.height * 8.0f / 5.0f + 220));
    }
    else
    {
        dragon->setPosition(ccp(dragon->getPositionX() + vec.x, 0));
        bubble->setPosition(ccp(bubble->getPositionX() + vec.x, dragon->boundingBox().size.height * 8.0f / 5.0f));
    }
    //dragon->setPosition(ccp(dragon->getPositionX() + vec.x, dragon->getPositionY() + vec.y));
    //bubble->setPosition(ccp(bubble->getPositionX() + vec.x, bubble->getPositionY() + vec.y));
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
        else if(b.compare("foodLabel") == 0)
        {
            GameHUD::getThis()->foodIcon->setZOrder(30);
            GameHUD::getThis()->foodLabel->setZOrder(31);
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
        
        TutorialManager::getThis()->highlightedBuilding->buildingRep->setZOrder(0);
    }
}

void MiniDragon::deHighlightBuilding(string b)
{
    if(b.compare("buildButton") == 0)
    {
        GameHUD::getThis()->buildButton->setZOrder(2);
        return;
    }
    else if(b.compare("foodLabel") == 0)
    {
        GameHUD::getThis()->foodIcon->setZOrder(2);
        GameHUD::getThis()->foodLabel->setZOrder(2);
        return;
    }
    
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
}

