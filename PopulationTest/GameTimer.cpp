//
//  GameTimer.cpp
//  PopulationTest
//
//  Created by admin on 24/4/13.
//
//

#include "GameTimer.h"
#include "GameHUD.h"
#include "GameScene.h"

GameTimer* GameTimer::SP;


GameTimer::GameTimer()
{
    SP = this;
    init();
    
    currentDay = currentMonth = currentWeek = currentYear = 0;
}

GameTimer::~GameTimer()
{
    //timeLabel->release();
    SP = NULL;
}

GameTimer* GameTimer::getThis()
{
    return SP;
}

GameTimer* GameTimer::create()
{
        GameTimer *pRet = new GameTimer();
        if (pRet)
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

void GameTimer::cleanup()
{
    CCLog("cleaning gametimer");
}

bool GameTimer::init()
{
    //Rmb to init timeElapsed to load from saved data
    timeElapsed = 0;
    lastTime = 0;
    
    CCString* timeString = CCString::createWithFormat("Year: %i  Month: %i  Week: %i  Day: %i", initialDate, initialDate, initialDate, initialDate);
    timeLabel = CCLabelTTF::create(timeString->getCString(), "Droidiga", 25);
    timeLabel->setAnchorPoint(ccp(-0.05f, 0.5f));
    addChild(timeLabel);
    timeLabel->setString(timeString->getCString());
    
    //this->schedule(schedule_selector(GameTimer::updateTimer), 1.0f/60.0f);
    
    return true;
}

void GameTimer::updateTimer(float deltaTime){

    timeElapsed += deltaTime;
    GameManager::getThis()->currentSecsElapsed = timeElapsed;
    
    if (timeElapsed - lastTime < secToDayRatio) {
        return;
    }
    
    lastTime = timeElapsed;
    int secs = (int)timeElapsed;
    
    currentDay++;// = ((secs / secToDayRatio) % dayToWeekRatio) + initialDate;
    currentWeek = secs/secToDayRatio/dayToWeekRatio%weekToMonthRatio + initialDate;
    currentMonth = secs/secToDayRatio/dayToWeekRatio/weekToMonthRatio%monthToYearRatio + initialDate;
    currentYear = secs/secToDayRatio/dayToWeekRatio/weekToMonthRatio/monthToYearRatio + initialDate;

    
    CCString* timeString = CCString::createWithFormat("Year: %i  Month: %i  Week: %i  Day: %i", currentYear, currentMonth, currentWeek, currentDay);
    timeLabel->setString(timeString->getCString());
}

void GameTimer::OnOrientationChanged(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    timeLabel->setPosition(ccp(screenSize.width*0.7, screenSize.height*0.95));
}

int GameTimer::getCurrentDay(){
    return 0;
}

int GameTimer::getCurrentWeek(){
    return 0;
}

int GameTimer::getCurrentMonth(){
    return 0;
}

int GameTimer::getCurrentYear(){
    return 0;
}

void GameTimer::setTime(float elapsed, float last)
{
    timeElapsed = elapsed;
    lastTime = last;
}

int GameTimer::getTimeElapsed()
{
    return timeElapsed;
    
}

int GameTimer::getLastTime()
{
    return lastTime;
}