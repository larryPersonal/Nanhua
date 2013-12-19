//
//  OptionsScene.h
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#ifndef __PopulationTest__OptionsScene__
#define __PopulationTest__OptionsScene__

#include "cocos2d.h"

using namespace cocos2d;


class OptionsScene : public cocos2d::CCLayer
{
    static OptionsScene* SP;
    
    CCMenuItemImage* buttonBack;
    
public:
    OptionsScene();
    ~OptionsScene();
    static OptionsScene* getThis();
    
    virtual bool init();
    static CCScene* scene();
    
    CREATE_FUNC(OptionsScene);
    
    void onButtonBackPressed(CCObject* pSender);
};

#endif /* defined(__PopulationTest__OptionsScene__) */