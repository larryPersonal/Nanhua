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
#include "JobCollection.h"
#include "GameConfig.h"
#include "BanditsAttackHandler.h"
#include "ObjectiveHandler.h"

using namespace cocos2d;
using namespace CocosDenshion;

class GameScene : public CCLayer
{
public:
    static GameScene* SP;               // script pointer (GameScene)
    static CCScene* thisScene;          // scene pointer  (CCScene)
    
    float currScale;                    // current scale factor -> control the view degree scale
    
    CCSprite* gameBG;                   // background image of the game, use to decorate the game map
    
    float _currentTime;                 // register to hold the current system time
    float _previousTime;                // register to hold the previous system time
    struct cc_timeval now;              // time interval stucture use to store the time value
    
    float _speedX;                      // horizontal (x) speed of the scrolling action
    float _speedY;                      // vertical (y) speed of the scroll action
    
    float _previous_pos_x;
    float _previous_pos_y;
    
    float _current_pos_x;
    float _current_pos_y;
    
    bool _post_drag_effect;             // flag to check the elasticity effect
    
    float cumulatedTime;
    
    ConfigSettings* configSettings;
    SettingsLevel* settingsLevel;
    
    bool isOpenBuildScroll;
    
public:
    
    CCNode* screenCenter;
    CCSize sz; 

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
    
    MapHandler* mapHandler;
    SpriteHandler* spriteHandler;
    BuildingHandler* buildingHandler;
    JobCollection* jobCollection;
    ConstructionHandler* constructionHandler;
    BanditsAttackHandler* banditsAttackHandler;
    ObjectiveHandler* objectiveHandler;
    
    CCPoint lastTilePosPreview;
    CCPoint firstPathPosPreview;
    CCPoint lastPathPosPreview;
    int buildPathDistance;
    bool isThisTapCounted;
    
    //allows the scene to be reinitialized
    void setupScene();
    void deleteScene();

    // touch controller
    void ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
    void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    //CREATE_FUNC(GameScene);
    
    void FirstRunPopulate();
    virtual void update(float time);
    
    // internal touch functions
    bool handleTouchTokens(CCPoint touchLoc);
    bool handleTouchSprite(CCPoint touchLoc);
    bool handleTouchBuilding(CCPoint touchLoc, CCPoint tilePos);
    
    void resetBuildMode();
    void setBuildPathDistance(int dist);
    
    void lostGame(CCObject* psender);
    
    // jerry added
    void enableTouch();
    virtual void move(float time);
    virtual void postDrag(float time);
    
    CREATE_FUNC(GameScene);
};

#endif
