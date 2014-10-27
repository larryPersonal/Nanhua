//
//  TutorialManager.h
//  PopulationTest
//
//  Created by admin on 1/4/14.
//
//

#ifndef PopulationTest_TutorialManager_h
#define PopulationTest_TutorialManager_h

#include "cocos2d.h"
#include "MiniDragon.h"
#include "Narrator.h"

using namespace cocos2d;

class TutorialManager : public CCLayer
{
public:
    static TutorialManager* SP;
    
    bool lockMap;
    bool lockVillager;
    
    bool inDisplay;
    bool inText;
    
    bool inDraggon;
    
    CCPoint target;
    
    Narrator* narrator;
    MiniDragon* miniDragon;
    
    float targetOpacity;
    float fadeSpeed;
    
    bool active;
    
    bool lockBuildButton;
    bool lockObjectiveButton;
    bool lockSystemButton;
    bool lockTimeGroup;
    bool lockScroll;
    bool lockBuildScroll;
    bool lockBuildingButton;
    bool lockPopup;
    bool lockButtonOk;
    bool lockButtonCancel;
    bool lockButtonClose;
    bool lockManpowerSelect;
    bool lockSpriteInfo;
    bool lockBuildingInfo;
    bool lockGoldLabel;
    bool lockFoodLabel;
    bool lockPopulationLabel;
    bool lockDropTokens;
    
    bool teachBuildButton;
    bool teachBuildHouse;
    bool teachFarming;
    bool teachBuildRoad;
    bool teachBuildGranary;
    bool teachBuildFarm;
    bool teachBuildGuardTower;
    bool teachSoldier;
    bool teachFighting;
    
    bool clickable;
    Building* highlightedBuilding;
    
    int villagersToBeAdded;
    float cumulativeTime;
    
    bool show;
    bool hide;
    
    bool goNarr;
    
    bool freeBuilding;
    
    bool lockModule;
    
    CCArray* spritesArray;
    
public:
    TutorialManager();
    ~TutorialManager();
    
    static TutorialManager* getThis();
    static TutorialManager* create();
    
    void setupForTutorial();
    
    void moveCamera(float);
    
    void fadeOut(float);
    void fadeIn(float);
    
    void scheduleFadeIn(float, float);
    void scheduleFadeOut(float, float);
    
    void setupNarrator();
    
    void setupMiniDragon();
    
    void lockAll();
    void unlockAll();
    
    void addVillagers(int);
    
    void scheduleVillagers(float);
    
    void clearSprites();
    
    void setupForScenario();
    
    void scheduleForScenario(float time);
    
    void annouceForScenario1();
    
    //void saySpeech(std::string());
};

#endif
