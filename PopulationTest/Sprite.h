//
//  Sprite.h
//  PopulationTest
//
//  Created by Serious Games on 25/3/13.
//
//

#ifndef __PopulationTest__Sprite__
#define __PopulationTest__Sprite__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "PathFinder.h"
#include "MapTile.h"

#include "Sequence.h"
#include "Selector.h"
#include "Building.h"
#include "Possessions.h"
#include "SpeechBubble.h"
#include "Requirements.h"
#include "ProgressBar.h"

#include <json/json.h>

using namespace cocos2d;
using namespace CocosDenshion;

enum SpriteAppearanceType {
    M_A_VILLAGER = 0, F_A_VILLAGER, M_A_YOUNG_VILLAGER, F_A_YOUNG_GIRL, F_A_GIRL, F_A_FARMER, M_A_FARMER, M_A_OLDMAN, M_A_SOLDIER, F_A_SOLDIER, M_A_BANDIT, F_A_BANDIT, SPRITE_APPEARANCE_TYPE_END = 12
};


enum SpriteAction { IDLE = 0, WALKING, CARRYING, FIGHTING, ESCAPING, EATING, STORING, FARMING, BUILD, RESTING, GET_HOME, GUARD, ROB, ACTION_END = 14};

enum SpriteJob { NONE = 0, BUILDER, FARMER, DELIVERER, SOLDIER = 4 };

enum SpriteCombatState {
    C_IDLE = 0, C_COMBAT, C_ESCAPE = 2
};

enum VillagerClass {
    V_REFUGEE = 0, V_CITIZEN, V_FARMER, V_BUILDER, V_SOLDIER, V_BANDIT, V_CLASS_END = 6
};

class SpriteClass : public CCObject
{
public:
    std::string configContent;
    std::string defaultContent;
    std::string targetClass;
    VillagerClass villagerClass;
    
    SpriteClass()
    {
        configContent = "";
        defaultContent = "";
        targetClass = "";
        villagerClass = V_REFUGEE;
    }
};

class GameSprite: public CCObject
{
private:
    //This ensures that a move instruction other than Wander will not be interrupted by another instruction that resets the path.
    bool isFollowingMoveInstruction;

    int spriteW;
    int spriteH;
    
    int foodCarried;  // indicate the food carried by the farmer to transport into the granary.
    
    //I assume all animations are using the same number of frames.
    int idleFrameCount;
    int walkingFrameCount;
    
    float idleDelay;
    Possessions* possessions;
    
    SpeechBubble* speechBubble;
    
    /*note: clones will always have NULL requirements, only the primary copy inside allSprites has these initialized.*/
    int spriteSpeed;
   
    std::string currentDir;
    
    float cumulativeTime;
    
    //for greedy pathfinding. The sprite must be able to track the last location pathed to before its path was blocked. //
    Building* lastTarget;
    
    float cumulativeTime_happiness;
    
    float cumulativeTime_attack;
    
    // token part
    bool is_token_drop_cooldown;
    float token_drop_cooldown_time;
    float token_drop_cooldown_treshold;
    float token_drop_rate;
    
public:
    SpriteAppearanceType appearanceType;
    VillagerClass villagerClass;
    
    GameSprite* enermy;
    GameSprite* preEnermy;
    
    CCPoint nextTile;
    CCPoint currentTile;
    CCPoint targetLocation;
    
    // bool stopAction;
    bool stopAction;
    bool isMoving;
    
    // bandit's escape flag
    bool tryEscape;
    
    int mGameCurrentEndurance;
    int mGameMaxEndurance;
    
    bool mGameWarMode;
    
    // the variables that the bandits stole from the village.
    int current_money_rob;
    int current_food_rob;
    
    std::string config_doc;
    std::string defaults_doc;
    
    Behavior* behaviorTree;
    
    Json::Value root;
    Json::Value defaultsRoot;
    Json::Reader reader;

    Behavior* buildTreeWithJsonValue(Json::Value json);
    
    bool shouldStopNextSquare;
    bool wanderFlag;
    
    CCPoint currPos;
    MapTile* currTile;
    
    bool shouldSetVisibleNextFrame;
    
    void ReplaceSpriteRep();
    
    CCCallFuncN* callback;
    
    // jerry added
    Building* jobLocation;
    SpriteJob job;
    bool isDoingJob;
    
    SpriteCombatState combatState;
    
    //this is due to the greedy logic where the sprite still attempts to go to its destination, even when the destination is unreachable.
    //the pathing must resume.
   
    bool isLeavingNextUpdate;
    /* End unused*/
    bool isInBuilding;
    
    static GameSprite* create();
    
    //thanks to there being gender-specitic sprites.
    char gender;
    char race; //human or alien
    
    std::string spriteName;
    std::string spriteClass;
    std::string spriteDisplayedName;
    
    float movementSpeed;
    
    GameSprite();
    virtual ~GameSprite();
    
    void followPath();
    
    virtual GameSprite* copyWithZone(CCZone *pZone);

    SpriteAction lastFrameAction;
    SpriteAction currAction;
    SpriteAction nextAction;
    
    SpriteAction futureAction1;
    SpriteAction futureAction2;
    
    /*note: The following line defines which layer the sprite would be batched with.  Do NOT populate this manually! The SpriteHandler should populate and depopulate this from the
     list of layers it has. */
    int batchLayerIndex;
    
    CCSprite* spriteRep;
    ProgressBar* barHP;
    
    CCArray* hpLabels;
    
    /*holds a path. use pathfinding to deal with it*/
    CCArray* path;
    
    CCAnimation* animation;
    CCAction* spriteAnimAction;
    CCSequence* spriteRunAction;
    
    CCPoint getWorldPosition();
    
    void changeAnimation(std::string direction);
    void moveComplete(CCObject* pSender);
    
    SpriteAction getAction();
    void setAction(SpriteAction action);
    void setFrameCount(int idleFrames, int walkFrames);
    
    bool CreatePath(CCPoint start, CCPoint end);
    bool CreatePathEscape(CCPoint start, CCPoint end);
    
    void initAI(bool isUpgrade = false);
    
    void makeSprite(CCPoint* tilePos);
    void unmakeSprite();
    void unmakeSpriteEndGame();
    
    void moveSpritePosition(CCPoint targert, CCObject* pSender);
    
    void updateSprite(float dt);
    void updateZIndex();

    /*setters for the spritehandler constructor*/
    void setAIConfig(std::string config);
    void setDefaultsConfig(std::string config);
    
    Possessions* getPossessions();
    void addPossessionsToMap();
    void removePossessionsFromMap();
    void increasePossession(PossessionStats statType, int value);
    
    //Wrapped speech bubble to show text
    void saySpeech(const char* text, float timeInSeconds);
    void saySpeech(SpeechMood s, float timeInSeconds);
    
    /*movement*/
    bool Wander();
    void StopMoving();
    
    /*standard pathing*/
    bool PathToHome();
    bool PathToWork();
    
    void PathToHighTemple();
    bool PathToBuilding(int building_id);
    bool PathToBuildingOverride(int building_id);
    
    bool PathToResources();
    bool PathToExit();
    bool PathToBuild();
    
    bool GoBuild(Building *b);
    
    
    //destinations shold always be buildings.
    bool isDestinationInRange(int buildingID);
    
    void changeSpriteTo(GameSprite*, SpriteClass*);
    void changeClassTo(SpriteClass*);
    
    // jerry added
    Building* getHome();
    void setHome(Building*);
    
    SpriteJob getJob();
    void setJob(SpriteJob);
    
    Building* getJobLocation();
    void setJobLocation(Building*);
    
    Building* getTargetLocation();
    void setTargetLocation(Building*);
    
    bool getIsDoingJob();
    void setIsDoingJob(bool);
    
    int getTargetHungry();
    void setTargetHungry(int);
    
    void setIsFollowingMovementInstruction(bool);
    
    bool GoBuilding(Building* b);
    bool GoLocation(CCPoint, bool);
    
    bool GoRest(Building* b);
    bool GoEat(Building* b);
    bool GoFarming(Building* b);
    bool GoGranary(Building* b);
    bool returnFarm(Building* b);
    bool GoHome(Building* b);
    
    void updateIdleDelay(float delay);
    
    // jerry edited
    void changeToCitizen();
    
    void setFoodCarried(int);
    int getFoodCarried();
    
    Building* findNearestGranary(bool);
    
    bool loadClassSetup();
    void loadSpritePossessions();
    void loadClassPossessions();
    void clearSetup();
    
    bool isHungry();
    
    void setCumulativeTime(float);
    float getCumulativeTime();
    
    void goToEat();
    void goToSleep();
    void goToOccupyHome(Building*);
    
    bool hasEmptyHouse();
    bool findNearestHome();
    Building* findNearestHouse();
    
    int getPathDistance(CCPoint, CCPoint);
    
    void updateHungry(float);
    void updateEndurance(float);
    
    // token part
    void scheduleToken(float dt);
    void dropToken(bool);
    void checkDropTresholdTime();
    void checkDropRate();
    
    bool attack();
    bool hasValidTarget();
    Building* checkTarget(int mode);
    int getDistance(Building*);
    bool escape();
    
    void damaged(int);
    
    bool hasBandit(CCArray*, CCPoint);
    bool hasSoldier(CCArray*, CCPoint);
    
    void updatePath(Building*);
    void updatePath(CCPoint);
    
    bool isFarmer();
    bool isCitizen();
    bool isRefugee();
    bool isBandit();
    bool isSoldier();
    bool isBuilder();
    
    string getCurrentDir();
    
public:
    bool isInAttackAction;
    
    CCTexture2D* spriteTexture;
    CCRect spriteRect;
    
    float frameWidth;
    float frameHeight;
    int currentFrameNumber;
    int maxFrameNumber;
    int xOffset;
    int yOffset;
    
    float delay_animFrame;
    float delay_curr;
    
public:
    void playAttackAction();
};


#endif /* defined(__PopulationTest__Sprite__) */
