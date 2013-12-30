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

#include <json/json.h>
//#include <json/reader.h>
//#include <json/writer.h>
//#include <json/value.h>
//#include <json/config.h>
//#include <json/features.h>
//#include <json/autolink.h>

using namespace cocos2d;
using namespace CocosDenshion;

/*
enum SpriteType { F_ALIEN_CITIZEN = 0, M_ALIEN_CITIZEN, F_ALIEN_RESEARCHER, M_ALIEN_RESEARCHER, F_MAYAN_CITIZEN, F_MAYAN_FARMER, F_MAYAN_WARRIOR, M_MAYAN_CITIZEN, M_MAYAN_FARMER, M_MAYAN_WARRIOR, ALIEN_CHIEF, MAYAN_CHIEF, M_MAYAN_MERCHANT, F_MAYAN_MERCHANT, M_ALIEN_MERCHANT, F_ALIEN_MERCHANT, SPRITETYPE_END = 16};
*/

enum SpriteType { M_REFUGEE = 0, F_REFUGEE,
                M_CITIZEN, F_CITIZEN,
                M_BUILDER, F_BUILDER,
                M_FARMER, F_FARMER,
                M_SOLDIER, F_SOLDIER,
                M_BANDIT, F_BANDIT,
                SPRITETYPE_END = 12};


enum SpriteAction { IDLE = 0, WALKING, CARRYING, FIGHTING, ACTION_END = 4};




class GameSprite: public CCObject
{
private:
    //This ensures that a move instruction other than Wander will not be interrupted by another instruction that resets the path.
    bool isFollowingMoveInstruction;

    int spriteW;
    int spriteH;
    
    //I assume all animations are using the same number of frames.
    int idleFrameCount;
    int walkingFrameCount;
    
    float idleDelay;
    Possessions* possessions;
    SpeechBubble* speechBubble;
    
    /*note: clones will always have NULL requirements, only the primary copy inside allSprites has these initialized.*/
    int spriteSpeed;
   
    std::string currentDir;
    
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
    
    //this is to prevent the nohouse and nojob mods from kicking in immediately after the sprite quits/sells house.
    bool justSoldHouse;
    bool justQuitJob;
    bool shouldUpgrade;
    
    CCCallFuncN* callback;
public:
    
    /*unused*/
    int fdaysLeft;
    int internal_rank;
   
    bool isLeavingNextUpdate;
    bool isInteractingSocial;
    /* End unused*/
    bool isInBuilding;
    
    static GameSprite* create();
    
    //thanks to there being gender-specitic sprites.
    char gender;
    char race; //human or alien
    int spawncost; //for spawning via the policy menu. I'm probably using this for aliens only.
    
    std::string spriteName;
    std::string spriteClass;
    std::string spriteDisplayedName;
    
    
    GameSprite();
    virtual ~GameSprite();
    
    
    void followPath();
    
    
    virtual GameSprite* copyWithZone(CCZone *pZone);

    SpriteType type;
    SpriteAction lastFrameAction;
    SpriteAction currAction;
    
    /*note: The following line defines which layer the sprite would be batched with.  Do NOT populate this manually! The SpriteHandler should populate and depopulate this from the
     list of layers it has. */
    int batchLayerIndex;
    
    CCSprite* spriteRep;
    
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
   // void setRequirementsConfig(std::string config);
    
    Possessions* getPossessions();
    void addPossessionsToMap();
    void removePossessionsFromMap();
    void increasePossession(PossessionStats statType, int value);
    
    //Wrapped speech bubble to show text
    void saySpeech(const char* text, float timeInSeconds);
    
    /*transactions*/
    bool BuyHouse(int instanceID); //note: doesnt' take into account population squeeze limit.
    bool BuySqueezeHouse(int InstanceID); //will take into account squeeze limit.
    bool SellHouse();
    
    bool FindJobAt(int instanceID);
    
    /*movement*/
    bool Wander();
    void StopMoving();
    
    /*standard pathing*/
    bool PathToHome();
    bool PathToWork();
    
    void PathToHighTemple();
    bool PathToBuilding(int building_id);
    bool PathToBuildingOverride(int building_id);
    
    //destinations shold always be buildings.
    bool isDestinationInRange(int buildingID);
    
    void ChangeSpriteTo(GameSprite* sp);
    
    int getLevel();
    
    //A sprite takes stock of his day whenever he hits home. 
    void TakeStockOfDay();
    

    void CallbackDayPassed();
};


#endif /* defined(__PopulationTest__Sprite__) */
