//
//  GameScene.h
//  PopulationTest
//
//  Created by Serious Games on 7/3/13.
//
//

#ifndef PopulationTest_GameScene_h
#define PopulationTest_GameScene_h


#include "cocos2d.h"
#include "MapHandler.h"
#include "BuildingHandler.h"
#include "SpriteHandler.h"
#include "ConstructionHandler.h"
#include "SimpleAudioEngine.h"
#include "GameConfig.h"
#include "BanditsAttackHandler.h"
#include "ObjectiveHandler.h"
#include "GlobalOutcomeModifier.h"
#include "AnimatedRain.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define MOVE_INCH            7.0f/160.0f
#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f

static float convertDistanceFromPointToInch(float pointDis)
{
    float factor = ( CCEGLView::sharedOpenGLView()->getScaleX() + CCEGLView::sharedOpenGLView()->getScaleY() ) / 2;
    return pointDis * factor / CCDevice::getDPI();
}

class GameScene : public CCLayer
{
public:
    static GameScene* SP;               // script pointer (GameScene)
    
    float currScale;                    // current scale factor -> control the view degree scale
    
    bool hasBeenDragged;
    bool isInDeccelerating;
    CCPoint scrollDistance;
    
    bool tapped;
    
    CCSprite* loadingScreen;
    CCSprite* loadingLabel;
    
    Building* targetBuilding;
    AnimatedRain* animatedRain;
    
    float clearCacheTime;
    float clearCacheTimeLimit;
    
public:
    
    CCNode* screenCenter;

    GameScene();
    ~GameScene();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    static GameScene* getThis();
    //I need the base scene layer for zooming.
    static CCScene* getThisScene();
    
    // control orientation of the ipad
    void initOrientationChange();
    void onOrientationChanged();
    
    CCPoint lastTilePosPreview;
    CCPoint firstPathPosPreview;
    CCPoint lastPathPosPreview;
    int buildPathDistance;
    bool isThisTapCounted;
    
    //allows the scene to be reinitialized
    void setupScene();
    void deleteScene();

    // touch controller
    void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
    void ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
    void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    //CREATE_FUNC(GameScene);
    
    void FirstRunPopulate();
    virtual void update(float time);
    
    // internal touch functions
    bool handleTouchTokens(CCPoint touchLoc);
    bool handleTouchSprite(CCPoint touchLoc);
    bool handleTouchBandits(CCPoint touchLoc);
    bool handleTouchBuilding(CCPoint touchLoc, CCPoint tilePos);
    
    void resetBuildMode();
    void setBuildPathDistance(int dist);
    
    void decceleratingDragging(float dt);
    
    // jerry added
    void enableTouch();
    virtual void move(float time);
    
    void centerCamera(Building*, bool instant = true);
    void scrollToCenter(float deltaTime);
    
    void enableLoadingScreen();
    void goToMainMenu();
    
    void checkTeachBuildRoad();
    
    void loadLevelBuildings();
    
    CREATE_FUNC(GameScene);
    
public:
    // auto save
    float autoSaveTimeLeft;
    float autoSaveTimeInterval;
    
    float randomEventCumulativeTime;
    float randomEventTresholdTime;
    
    float randomEventTresholdTimeMax;
    float randomEventTresholdTimeMin;
    
    bool isEndingGame;
    bool isInGame;
    
public:
    void checkAutoSave(float deltaTime);
    
    void reSetupLevel(bool newGame);
    void configSkipData();
    void configLevelData(bool newGame);
    
    void startGame();
    void loadGame();
    void stopGame();
    
    void configLevelAtStart();
    
public:
    void constructGameScene();
    void resetupAllData();
    bool isActive();
};

#endif
