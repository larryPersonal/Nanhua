//
//  GameTimer.h
//  PopulationTest
//
//  Created by admin on 24/4/13.
//
//

#ifndef __PopulationTest__GameTimer__
#define __PopulationTest__GameTimer__

#include "cocos2d.h"

using namespace cocos2d;


class GameTimer: public CCLayer
{
    static GameTimer* SP;
    float timeElapsed, lastTime;
    CCLabelTTF* timeLabel;
    
    int secToDayRatio = 1;
    int dayToWeekRatio = 7;
    int weekToMonthRatio = 4;
    int monthToYearRatio = 12;
    
    int currentDay;
    int currentWeek;
    int currentMonth;
    int currentYear;
    const int initialDate = 1;
    
public:
    GameTimer();
    ~GameTimer();
    
    static GameTimer* create();
    static GameTimer* getThis();
    
    virtual void cleanup();
    
    bool init();
    void setTime(float elapsed, float last);
    void updateTimer(float deltaTime);
    void OnOrientationChanged();
    int getCurrentDay();
    int getCurrentWeek();
    int getCurrentMonth();
    int getCurrentYear();
    
    int getTimeElapsed();
    int getLastTime();
};

#endif /* defined(__PopulationTest__GameTimer__) */
