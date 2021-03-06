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
#include "BuildingCard.h"
#include "Senario.h"
#include "NanhuaGameStaticAPI.h"

MiniDragon::MiniDragon()
{
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    dragon = CCSprite::createWithSpriteFrameName("dragon_c.png");
    
    
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
    
    startX = dragon->boundingBox().size.width * 1.0f / 10.0f + 65.0f;
    startY = dragon->boundingBox().size.height * 7.5f / 5.0f - 10.0f;
    
    offX = 0;
    offY = 0;
    
    delay = 0;
    
    dropToken = false;
    
    ds = Dragon_Start;
    
    slidesList = CCArray::create();
    slidesList->retain();
    curSlide = 0;
    
    clickToNext = false;
    lockClick = false;
    GameHUD::getThis()->setTutorial = true;
    
    notFirst = false;
    
    connectHouse = false;
    connectGranary = false;
    
    highlightSprite = NULL;
    waitForVillager = false;
    finalObjective = false;
    
    down = true;
    
    debug = false;
    
    setButtonOkForDelay = false;
}

MiniDragon::~MiniDragon()
{
    clearMiniDragon();
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
    
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
}

void MiniDragon::playMiniDraggon()
{
    curSlide = 0;
    if(!debug)
    {
        readTutorialFile();
        constructTutorialSlide();
        TutorialManager::getThis()->lockModule = false;
    }
    else
    {
        if(Senario::getThis()->scenarioState == Tutorial)
        {
            CCLog("########## tes my real apple of range");
            setupScenario();
        }
    }
}

void MiniDragon::readTutorialFile()
{
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
    
    TutorialReader* tr = new TutorialReader();
    
    stringstream ss;
    if(Senario::getThis()->scenarioState == Tutorial)
    {
        ss << "tutorial.xml";
    }
    else if(Senario::getThis()->scenarioState == Tutorial2)
    {
        ss << "tutorialBandits.xml";
    }
    else
    {
        ss << "tutorial.xml";
    }
    
    tr->parseXMLFile(ss.str().c_str());
    
    slidesList = tr->slidesList;
    delete tr;
}

bool MiniDragon::constructTutorialSlide()
{
    /* remove all the animated string for the previous slide */
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        /* the animated string in some slides are kept in the GameHUD layer, not the tutorial layer */
        if(TutorialManager::getThis()->hide)
        {
            GameHUD::getThis()->removeChild(as->getLabel(), true);
        }
        else
        {
            TutorialManager::getThis()->removeChild(as->getLabel(), true);
        }
    }
    
    /* re-initialize the animated string list array */
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
    
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    /* if there is no more slides, finish executing the slides handling code */
    if(curSlide >= slidesList->count())
    {
        return false;
    }
    
    TutorialSlide* ts = (TutorialSlide*) slidesList->objectAtIndex(curSlide);
    
    /* check for whether the slides require to connect the path to the house or farm in tutorial mode */
    if(ts->checkGranary || ts->checkFarm)
    {
        CCArray* allGranaryGhost = BuildingHandler::getThis()->granaryGhostOnMap;
        CCArray* allFarmGhost = BuildingHandler::getThis()->amenityGhostOnMap;
        
        if(allGranaryGhost->count() > 0 && ts->checkGranary)
        {
            Building* gran = (Building*) allGranaryGhost->objectAtIndex(0);
            CCPoint granPos = gran->getWorldPosition();
            granPos = MapHandler::getThis()->tilePosFromLocation(granPos);
            
            Building* townhall = (Building*) BuildingHandler::getThis()->specialOnMap->objectAtIndex(0);
            CCPoint townHallPos = townhall->getWorldPosition();
            townHallPos = MapHandler::getThis()->tilePosFromLocation(townHallPos);
            
            PathFinder *p = new PathFinder();
            p->setDestination(granPos);
            p->setSource(townHallPos);
            
            bool valid = true;
            
            valid = p->hasPath(&townHallPos, &granPos);
            
            if(valid)
            {
                curSlide += 2;
                if(curSlide >= slidesList->count())
                {
                    return false;
                }
                ts = (TutorialSlide*) slidesList->objectAtIndex(curSlide);
            }
            
            delete p;
        }
        
        if(ts->checkFarm && allFarmGhost->count() > 0)
        {
            Building* farm = (Building*) allFarmGhost->objectAtIndex(0);
            CCPoint farmPos = farm->getWorldPosition();
            farmPos = MapHandler::getThis()->tilePosFromLocation(farmPos);
            
            Building* townhall = (Building*) BuildingHandler::getThis()->specialOnMap->objectAtIndex(0);
            CCPoint townHallPos = townhall->getWorldPosition();
            townHallPos = MapHandler::getThis()->tilePosFromLocation(townHallPos);
            
            PathFinder *p = new PathFinder();
            p->setDestination(farmPos);
            p->setSource(townHallPos);
            
            bool valid = true;
            
            valid = p->hasPath(&townHallPos, &farmPos);
            
            if(valid)
            {
                curSlide += 2;
                if(curSlide >= slidesList->count())
                {
                    return false;
                }
                ts = (TutorialSlide*) slidesList->objectAtIndex(curSlide);
            }
            
            delete p;
        }
    }
    
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
    lockClick = ts->lockClick;
    TutorialManager* tm = TutorialManager::getThis();
    delay = ts->delay;
    
    /* control camera position */
    if(cameraMove)
    {
        moveCamera(target);
    }
    
    /* control dragon sprite position */
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
    
    if(ts->highlight && !cameraMove)
    {
        highlightBuilding(ts->highlightBuilding);
    }
    
    notFirst = ts->notFirst;
    connectHouse = ts->connectHouse;
    connectGranary = ts->connectGranary;
    connectFarm = ts->connectFarm;
    waitForVillager = ts->waitForVillager;
    
    if(TutorialManager::getThis()->show)
    {
        bubble->retain();
        dragon->retain();
        GameHUD::getThis()->removeChild(dragon);
        GameHUD::getThis()->removeChild(bubble);
        TutorialManager::getThis()->addChild(dragon, 2);
        TutorialManager::getThis()->addChild(bubble, 1);
        dragon->autorelease();
        bubble->autorelease();
    }
    
    if(ts->triggerDropToken)
    {
        CCArray* sprites = SpriteHandler::getThis()->spritesOnMap;
        for(int i = 0; i < sprites->count(); i++)
        {
            GameSprite* gs = (GameSprite*) sprites->objectAtIndex(i);
            gs->dropToken(true);
        }
        dropToken = true;
    }
    
    if(ts->unlockAll)
    {
        cumulativeTime = 0;
        TutorialManager::getThis()->unlockAll();
        //TutorialManager::getThis()->scheduleOnce(schedule_selector( MiniDragon::scheduleSenario ), 360);
    }
    
    if(ts->hideBubble)
    {
        bubble->setVisible(false);
        dragon->setVisible(false);
        finalObjective = true;
    }
    
    if(ts->hideArrow)
    {
        FloatingArraw::getThis()->hideArrow();
    }
    
    if(ts->showArrow)
    {
        FloatingArraw::getThis()->showArrow(ccp(ts->arrowX, ts->arrowY), ts->arrowAngle, ts->arrowScale, ts->arrowLayer);
    }
    
    if(BuildScroll::getThis() != NULL)
    {
        if(ts->contentOffX != 0 || ts->contentOffY != 0)
        {
            BuildScroll::getThis()->scrollArea->setScrollContentOffset(ccp(-ts->contentOffX, ts->contentOffY));
        }
        
        if(ts->stopScroll)
        {
            BuildScroll::getThis()->scrollArea->stopScroll();
        }
        
        if(ts->stopScroll)
        {
            BuildScroll::getThis()->scrollArea->resumeScroll();
        }
        
        if(ts->hideScroll >= 0)
        {
            ((BuildingCard*)BuildScroll::getThis()->buildingCards->objectAtIndex(ts->hideScroll))->mask->setOpacity((GLubyte) 120);
        }
        
        if(ts->showScroll >= 0)
        {
            ((BuildingCard*)BuildScroll::getThis()->buildingCards->objectAtIndex(ts->showScroll))->mask->setOpacity((GLubyte) 0);
        }
    }
    
    if(ts->hideObjective)
    {
        GameHUD::getThis()->clickObjectiveButton();
    }
    
    if(ts->showObjective)
    {
        ObjectiveHandler::getThis()->playObjective(true);
        GameHUD::getThis()->clickObjectiveButton();
    }
    
    if(ts->setupGuardTowerBar)
    {
        GameHUD::getThis()->setupForGuardTowerBar();
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
            if(BuildScroll::getThis() != NULL)
            {
                if(tm->lockBuildScroll)
                {
                    BuildScroll::getThis()->scrollArea->scrollView->setTouchEnabled(false);
                }
                else
                {
                    BuildScroll::getThis()->scrollArea->scrollView->setTouchEnabled(true);
                }
            }
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
            if(flag == 1)
            {
                tm->lockButtonOk = true;
            }
            else
            {
                if(delay > 0)
                {
                    setButtonOkForDelay = true;
                }
                else
                {
                    tm->lockButtonOk = false;
                }
            }
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
        else if(order.compare("teachBuildGranary") == 0)
        {
            tm->teachBuildGranary = (flag == 1);
        }
        else if(order.compare("teachBuildFarm") == 0)
        {
            tm->teachBuildFarm = (flag == 1);
        }
        else if(order.compare("teachBuildGuardTower") == 0)
        {
            tm->teachBuildGuardTower = (flag == 1);
        }
        else if(order.compare("teachSoldier") == 0)
        {
            tm->teachSoldier = (flag == 1);
        }
        else if(order.compare("teachFighting") == 0)
        {
            tm->teachFighting = (flag == 1);
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
    
    if(ts->addBandit > 0)
    {
        NanhuaGameStaticAPI::banditsAttack(ts->addBandit);
    }
    
    ccColor3B color = ccc3(0, 0, 0);
    
    vector<std::string> tokens = GlobalHelper::split(str, ' ');
    
    float flashTimeGap = 0.05f;
    int flashGapCount = 0;
    
    float offsetY = ts->offY;
    
    for (int i = 0; i < tokens.size(); i++)
    {
        std::string tokenStr = tokens.at(i);
        //CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), "TooneyLoons", 18);
        CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), "Shojumaru-Regular", 16);
        tempLabel->retain();
        
        if (startX + offX + tempLabel->boundingBox().size.width > 420.0f)
        {
            offY = offY + 18.0f;
            offX = 0;
        }
        
        CC_SAFE_RELEASE(tempLabel);
        
        bool hasChinese = false;
        for (int j = 0; j < tokenStr.length(); j++)
        {
            if(tokenStr.at(j) == '^')
            {
                hasChinese = true;
                break;
            }
        }
        
        if(hasChinese)
        {
            int startIndex = 0;
            for (int j = 0; j < tokenStr.length(); j++)
            {
                if(tokenStr.at(j) == '^')
                {
                    string str = tokenStr.substr(startIndex, j - startIndex);
                    
                    /*
                    CCLabelTTF* tempLabel = CCLabelTTF::create(str.c_str(), "Shojumaru-Regular", 16);
                    tempLabel->retain();
                    
                    if (startX + offX + tempLabel->boundingBox().size.width > limitX)
                    {
                        offY = offY + 35.0f;
                        offX = 0;
                    }
                    
                    CC_SAFE_RELEASE(tempLabel);
                    */
                    
                    AnimatedString* as = AnimatedString::create(str, flashTimeGap * flashGapCount, "Shojumaru-Regular", 16, 80.0f);
                    as->getLabel()->setColor(color);
                    as->getLabel()->setAnchorPoint(ccp(0, 1));
                    
                    as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
                    offX += as->label->boundingBox().size.width;
                    
                    TutorialManager::getThis()->addChild(as->getLabel(), 20);
                    animatedStringList->addObject(as);
                    
                    flashGapCount += 1;
                    
                    startIndex = j + 1;
                }
            }
            
            string str = tokenStr.substr(startIndex, tokenStr.length() - startIndex);
            AnimatedString* as = AnimatedString::create(str, flashTimeGap * flashGapCount, "Shojumaru-Regular", 16, 80.0f);
            as->getLabel()->setColor(color);
            as->getLabel()->setAnchorPoint(ccp(0, 1));
            
            as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
            offX += as->label->boundingBox().size.width;
            
            TutorialManager::getThis()->addChild(as->getLabel(), 20);
            animatedStringList->addObject(as);
            
            flashGapCount += 1;
            
            offX += 10;
        }
        else
        {
            for (int j = 0; j < tokenStr.length(); j++)
            {
                string tempStr = tokenStr.substr(j, 1);
                //AnimatedString* as = AnimatedString::create(tempStr, flashTimeGap * (j + flashGapCount), "TooneyLoons", 18, 80.0f);
                AnimatedString* as = AnimatedString::create(tempStr, flashTimeGap * (j + flashGapCount), "Shojumaru-Regular", 16, 80.0f);
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
        if(TutorialManager::getThis()->miniDragon->delay > 0)
        {
            TutorialManager::getThis()->miniDragon->delay -= dt;
            if(TutorialManager::getThis()->miniDragon->delay < 0)
            {
                TutorialManager::getThis()->miniDragon->delay = 0;
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
    dragon->autorelease();
    bubble->autorelease();
    
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        CCLabelTTF* temp = ((AnimatedString*) animatedStringList->objectAtIndex(i))->getLabel();
        temp->retain();
        TutorialManager::getThis()->removeChild(temp);
        GameHUD::getThis()->addChild(temp, 33);
        temp->autorelease();
    }
    
    if(TutorialManager::getThis()->miniDragon->setButtonOkForDelay)
    {
        TutorialManager::getThis()->lockButtonOk = false;
        TutorialManager::getThis()->miniDragon->setButtonOkForDelay = false;
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
    bool up = false;
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        if(vec.y > 0 && down)
        {
            as->getLabel()->setPosition(ccp(as->getLabel()->getPositionX() + vec.x, as->getLabel()->getPositionY() + vec.y));
            up = true;
        }
        else if(vec.y < 0 && !down)
        {
            as->getLabel()->setPosition(ccp(as->getLabel()->getPositionX() + vec.x, as->getLabel()->getPositionY() + vec.y));
            up = false;
        }
        else
        {
            up = !down;
        }
    }
    down = !up;
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
        buildingsOnMap = BuildingHandler::getThis()->specialOnMap;
    }
    else if(b.compare("housing") == 0)
    {
        buildingsOnMap = BuildingHandler::getThis()->housingOnMap;
    }
    else if(b.compare("granary") == 0)
    {
        buildingsOnMap = BuildingHandler::getThis()->granaryOnMap;
    }
    else if(b.compare("amenity") == 0)
    {
        buildingsOnMap = BuildingHandler::getThis()->amenityOnMap;
    }
    else if(b.compare("military") == 0)
    {
        buildingsOnMap = BuildingHandler::getThis()->militaryOnMap;
    }
    else
    {
        if(b.compare("buildButton") == 0)
        {
            GameHUD::getThis()->buildButton->setZOrder(30);
        }
        else if(b.compare("goldLabel") == 0)
        {
            GameHUD::getThis()->infoBackground->setZOrder(30);
            GameHUD::getThis()->moneyBackground->setZOrder(30);
            GameHUD::getThis()->moneyIcon->setZOrder(30);
            GameHUD::getThis()->moneyLabel->setZOrder(31);
            GameHUD::getThis()->clickMoneyLabel();
        }
        else if(b.compare("foodLabel") == 0)
        {
            GameHUD::getThis()->infoBackground->setZOrder(30);
            GameHUD::getThis()->foodBackground->setZOrder(30);
            GameHUD::getThis()->foodIcon->setZOrder(30);
            GameHUD::getThis()->foodLabel->setZOrder(31);
            GameHUD::getThis()->clickFoodLabel();
        }
        else if(b.compare("populationLabel") == 0)
        {
            GameHUD::getThis()->infoBackground->setZOrder(30);
            GameHUD::getThis()->populationBackground->setZOrder(30);
            GameHUD::getThis()->populationIcon->setZOrder(30);
            GameHUD::getThis()->populationLabel->setZOrder(31);
            GameHUD::getThis()->clickPopulationLabel();
        }
        else if(b.compare("reputationLabel") == 0)
        {
            GameHUD::getThis()->statsMenu->setZOrder(31);
            GameHUD::getThis()->reputationBackground->setZOrder(30);
            GameHUD::getThis()->reputationIcon->setZOrder(32);
            GameHUD::getThis()->achivementsLabel->setZOrder(32);
        }
        else if(b.compare("objectiveButton") == 0)
        {
            GameHUD::getThis()->objectiveButton->setZOrder(37);
            GameHUD::getThis()->objectiveButtonBlue->setZOrder(38);
            GameHUD::getThis()->objectiveMenu->setZOrder(36);
            GameHUD::getThis()->clickObjectiveButton();
        }
        else if(b.compare("systemButton") == 0)
        {
            GameHUD::getThis()->systemButton->setZOrder(35);
        }
        else if(b.compare("timeGroup") == 0)
        {
            GameHUD::getThis()->timeMenu->setZOrder(32);
            GameHUD::getThis()->timeBackground->setZOrder(31);
            GameHUD::getThis()->firstWeekLabel->setZOrder(33);
            GameHUD::getThis()->secondWeekLabel->setZOrder(33);
            GameHUD::getThis()->thirdWeekLabel->setZOrder(33);
            GameHUD::getThis()->lastWeekLabel->setZOrder(33);
            GameHUD::getThis()->timeLabel_1->setZOrder(33);
            GameHUD::getThis()->timeLabel_2->setZOrder(33);
            GameHUD::getThis()->scheduleScrollIn();
        }
        else if(b.compare("averageHappiness") == 0)
        {
            GameHUD::getThis()->happinessIcon->setZOrder(35);
        }
        return;
    }
    
    if (buildingsOnMap != NULL && buildingsOnMap->count() > 0)
    {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        TutorialManager::getThis()->highlightedBuilding = (Building*) buildingsOnMap->objectAtIndex(0);
        
        if(highlightSprite != NULL)
        {
            TutorialManager::getThis()->removeChild(highlightSprite, true);
        }
        
        CCTexture2D* tex = TutorialManager::getThis()->highlightedBuilding->buildingRep->getTexture();
        CCRect rec = TutorialManager::getThis()->highlightedBuilding->buildingRep->getTextureRect();
        highlightSprite = CCSprite::createWithTexture(tex);
        highlightSprite->setTextureRect(rec);
        highlightSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        highlightSprite->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
        TutorialManager::getThis()->addChild(highlightSprite);
    }
}

void MiniDragon::deHighlightBuilding(string b)
{
    if(b.compare("buildButton") == 0)
    {
        GameHUD::getThis()->buildButton->setZOrder(2);
        return;
    }
    else if(b.compare("goldLabel") == 0)
    {
        GameHUD::getThis()->infoBackground->setZOrder(3);
        GameHUD::getThis()->moneyBackground->setZOrder(1);
        GameHUD::getThis()->moneyIcon->setZOrder(2);
        GameHUD::getThis()->moneyLabel->setZOrder(2);
        return;
    }
    else if(b.compare("foodLabel") == 0)
    {
        GameHUD::getThis()->infoBackground->setZOrder(3);
        GameHUD::getThis()->foodBackground->setZOrder(1);
        GameHUD::getThis()->foodIcon->setZOrder(2);
        GameHUD::getThis()->foodLabel->setZOrder(2);
        return;
    }
    else if(b.compare("populationLabel") == 0)
    {
        GameHUD::getThis()->infoBackground->setZOrder(3);
        GameHUD::getThis()->populationBackground->setZOrder(1);
        GameHUD::getThis()->populationIcon->setZOrder(2);
        GameHUD::getThis()->populationLabel->setZOrder(2);
        GameHUD::getThis()->clickPopulationLabel();
        return;
    }
    else if(b.compare("reputationLabel") == 0)
    {
        GameHUD::getThis()->statsMenu->setZOrder(2);
        GameHUD::getThis()->reputationBackground->setZOrder(1);
        GameHUD::getThis()->reputationIcon->setZOrder(3);
        GameHUD::getThis()->achivementsLabel->setZOrder(2);
        return;
    }
    else if(b.compare("objectiveButton") == 0)
    {
        GameHUD::getThis()->objectiveButton->setZOrder(7);
        GameHUD::getThis()->objectiveButtonBlue->setZOrder(8);
        GameHUD::getThis()->objectiveMenu->setZOrder(6);
        GameHUD::getThis()->clickObjectiveButton();
        return;
    }
    else if(b.compare("systemButton") == 0)
    {
        GameHUD::getThis()->systemButton->setZOrder(5);
        return;
    }
    else if(b.compare("timeGroup") == 0)
    {
        GameHUD::getThis()->timeMenu->setZOrder(2);
        GameHUD::getThis()->timeBackground->setZOrder(1);
        GameHUD::getThis()->firstWeekLabel->setZOrder(3);
        GameHUD::getThis()->secondWeekLabel->setZOrder(3);
        GameHUD::getThis()->thirdWeekLabel->setZOrder(3);
        GameHUD::getThis()->lastWeekLabel->setZOrder(3);
        GameHUD::getThis()->timeLabel_1->setZOrder(3);
        GameHUD::getThis()->timeLabel_2->setZOrder(3);
        GameHUD::getThis()->scheduleScrollOut();
        return;
    }
    else if(b.compare("averageHappiness") == 0)
    {
        GameHUD::getThis()->happinessIcon->setZOrder(5);
        return;
    }
    
    if (highlightSprite != NULL)
    {
        TutorialManager::getThis()->removeChild(highlightSprite, true);
        highlightSprite = NULL;
    }
}

void MiniDragon::clickNext()
{
    curSlide++;
    
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
    CCDirector::sharedDirector()->purgeCachedData();
    if(constructTutorialSlide())
    {
    }
    else
    {
        clearMiniDragon();
        
        if(Senario::getThis()->scenarioState == Tutorial)
        {
            GameHUD::getThis()->clickScoreButton();
        }
        finalObjective = false;
    }
}

void MiniDragon::setupScenario()
{
    GameHUD::getThis()->pause = true;
    finalObjective = false;
    if(!debug)
    {
        ObjectiveHandler::getThis()->playObjective(false);
    }
    TutorialManager::getThis()->lockAll();
    moveCamera(ccp(4128.0f, 1800.0f));
    
    TutorialManager::getThis()->spritesArray = CCArray::create();
    TutorialManager::getThis()->spritesArray->retain();
    
    CCPoint target = CCPointMake(30,39);
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x += 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x += 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x += 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.y += 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x -= 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x -= 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    target.x -= 1;
    SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE, true);
    
    TutorialManager::getThis()->goNarr = true;
}

void MiniDragon::clearMiniDragon()
{
    CCLog("***** test apple, test apple, test apple");
    TutorialManager::getThis()->active = false;
    TutorialManager::getThis()->unlockAll();
    TutorialManager::getThis()->removeChild(dragon, true);
    TutorialManager::getThis()->removeChild(bubble, true);
    GameHUD::getThis()->removeChild(dragon, true);
    GameHUD::getThis()->removeChild(bubble, true);
    
    TutorialManager::getThis()->unschedule(schedule_selector(MiniDragon::update));
    TutorialManager::getThis()->unschedule(schedule_selector(TutorialManager::moveCamera));
    
    if (highlightSprite != NULL)
    {
        TutorialManager::getThis()->removeChild(highlightSprite, true);
        highlightSprite = NULL;
    }
    
    for (int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        TutorialManager::getThis()->removeChild(as->getLabel(), true);
        GameHUD::getThis()->removeChild(as->getLabel(), true);
    }
    animatedStringList->removeAllObjects();
    slidesList->removeAllObjects();
}

