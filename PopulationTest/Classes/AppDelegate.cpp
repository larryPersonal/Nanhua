//
//  PopulationTestAppDelegate.cpp
//  PopulationTest
//
//  Created by Serious Games on 5/3/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SplashScene.h" //change to splash.h when finalised

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate* AppDelegate::SP;

AppDelegate::AppDelegate()
{
    SP = this;
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    
    CCLOG("starting application");
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setProjection(kCCDirectorProjection2D);
    
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCSize sz = pDirector->getWinSize();
    width = MIN(sz.width, sz.height);
    height = MAX(sz.width, sz.height);

    //@*&^@^$ retina deciding to half the size of everything even if everything is in retina resolution.
    //enough of this, I'm forcing it to take its base from the original iPad resolution.
    //Note that my default rotation is portrait, but I'll still set up for landscape mode launch. 
    appWidth = 768;
    appHeight = 1024;
    pDirector->setContentScaleFactor(1.0f);
    
    if (sz.width > sz.height) //landscape
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(appHeight, appWidth, kResolutionExactFit);
    else
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(appWidth, appHeight, kResolutionExactFit);
    
    

    // turn off display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = SplashScene::scene();
    
    // run
    pDirector->runWithScene(pScene);
    return true;
}

AppDelegate* AppDelegate::getThis()
{
    return SP;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}


