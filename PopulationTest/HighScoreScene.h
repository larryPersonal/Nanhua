//
//  HighScoreScene.h
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#ifndef __PopulationTest__HighScoreScene__
#define __PopulationTest__HighScoreScene__

#include "cocos2d.h"

using namespace cocos2d;


class HighScoreScene : public cocos2d::CCLayer
{
    static HighScoreScene* SP;
    
    CCMenuItemImage* buttonBack;
    
public:
    HighScoreScene();
    ~HighScoreScene();
    static HighScoreScene* getThis();
    
    virtual bool init();
    static CCScene* scene();
    
    CREATE_FUNC(HighScoreScene);
    
    void onButtonBackPressed(CCObject* pSender);
};

#endif /* defined(__PopulationTest__HighScoreScene__) */