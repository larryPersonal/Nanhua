//
//  Senario.h
//  PopulationTest
//
//  Created by SGI on 12/12/13.
//
//

#ifndef PopulationTest_Senario_h
#define PopulationTest_Senario_h

#include "cocos2d.h"
#include "GameScene.h"
#include "SenarioManager.h"

using namespace cocos2d;

enum ScenarioState
{
    Introduction, Tutorial, Scenario1, Scenario2, Tutorial2
};

class Senario : public CCLayer
{
public:
    static Senario* SP;
    
    char* text;
    
    bool active;
    bool inOption;
    bool notJump;
    
    int curSlide;
    
    vector<CCSprite*> spriteList;
    vector<CCMenu*> menuList;
    vector<CCLabelTTF*> labelList;
    
    CCSprite* skipButton;
    
    CCArray* slidesList;
    CCArray* animatedStringList;
    CCArray* animatedSpriteList;
    CCArray* animatedDialogueList;
    
    bool skipSlide;
    
    float cumulativeTime;
    int lastTime;
    
    std::string backgroundImage;
    
    ScenarioState scenarioState;
    
public:
    Senario();
    ~Senario();
    
    static Senario* create();
    static Senario* getThis();
    
    bool init();
    
    void playSenario(const char* senario);
    
    void readSenarioFile(const char* senario);
    
    void buttonSelect();
    
    void updateGameScene();
    
    bool constructSenarioStage(bool);
    
    void nextButtonPressed(bool);
    
    void clearElements();
    
    void onOrientationChanged();
    
    void selectButtonPressed(CCObject* pSender);
    
    void displayTexts(std::string, float, float, string, float, ccColor3B, float);
    
    void update(float time);
    
    void goNextSlide();
    
    void activateRefugee(float);
    
    void clearScenario();
};

#endif
