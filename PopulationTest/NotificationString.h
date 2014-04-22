//
//  NotificationString.h
//  PopulationTest
//
//  Created by admin on 21/4/14.
//
//

#ifndef PopulationTest_NotificationString_h
#define PopulationTest_NotificationString_h

#include "cocos2d.h"
using namespace cocos2d;

class NotificationString : public CCObject
{
public:
    CCLabelTTF* notificationLabel;
    float borderX;
    float borderY;
    float startY;
    
public:
    static NotificationString* create(std::string, float);
    NotificationString(std::string, float);
    ~NotificationString();
    
    void scheduleSlideIn();
    void scheduleSlideOut();
    void scheduleSlideUp();
    
    void slideIn(float);
    void slideOut(float);
    void slideUp(float);
};

#endif
