//
//  SplashScene.h
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#ifndef __PopulationTest__SplashScene__
#define __PopulationTest__SplashScene__

#include "cocos2d.h"

using namespace cocos2d;


class SplashScene : public cocos2d::CCLayerColor
{
    static SplashScene* SP;
    
    CCSprite* splashImage;
    int transitionState;
    
public:
    SplashScene();
    ~SplashScene();
    static SplashScene* getThis();
    
    virtual bool init();
    static CCScene* scene();
    virtual void update(float dt);
    
    void onOrientationChanged();
    
    CREATE_FUNC(SplashScene);
};

#endif /* defined(__PopulationTest__SplashScene__) */