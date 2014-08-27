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
    
    CCSpriteBatchNode* teacherManagementNode;
    CCSpriteBatchNode* mainGamePageNode;
    CCSpriteBatchNode* extraTextureNode;
    CCSpriteBatchNode* mainMenuSceneNode;
    CCSpriteBatchNode* gameHUDNode;
    CCSpriteBatchNode* buildingInforMenuNode;
    CCSpriteBatchNode* buildScrollNode;
    CCSpriteBatchNode* packedPortraitsNode;
    
    CCSpriteBatchNode* appearAnimationNode;
    CCSpriteBatchNode* disappearAnimationNode;
    CCSpriteBatchNode* tokenAnimationNode;
    
    CCSpriteBatchNode* injuredEmoteNode;
    CCSpriteBatchNode* stealMoneyNode;
    CCSpriteBatchNode* findBanditNode;
    CCSpriteBatchNode* homelessNode;
    CCSpriteBatchNode* noPathNode;
    CCSpriteBatchNode* stealFoodNode;
    CCSpriteBatchNode* commonNode;
    CCSpriteBatchNode* workNode;
    CCSpriteBatchNode* bubbleNode;
    CCSpriteBatchNode* happinessNode;
    CCSpriteBatchNode* starvingNode;
    CCSpriteBatchNode* buildNode;
    CCSpriteBatchNode* defendNode;
    CCSpriteBatchNode* noFoodNode;
    
public:
    SplashScene();
    ~SplashScene();
    static SplashScene* getThis();
    
    virtual bool init();
    static CCScene* scene();
    virtual void update(float dt);
    
    void onOrientationChanged();
    
    void preloadTextures();
    
    void clearTextureNode();
    
    CREATE_FUNC(SplashScene);
};

#endif /* defined(__PopulationTest__SplashScene__) */