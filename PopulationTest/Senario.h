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

class Senario : public CCLayer
{
public:
    static Senario* SP;
    
    //CCSprite* character;
    CCMenuItem* chatbox;
    
    char* text;
    
    bool active;
    
    int curSlide;
    
    vector<CCSprite*> spriteList;
    vector<CCMenu*> menuList;
    vector<CCLabelTTF*> labelList;
    
    CCArray* slidesList;
    
    CCMenu* startGameMenu;
    
public:
    Senario();
    ~Senario();
    
    static Senario* create();
    static Senario* getThis();
    
    bool init();
    
    void playSenario(const char* senario);
    
    void readSenarioFile();
    
    void createGUI();
    
    void buttonSelect();
    
    void updateGameScene();
    
    bool constructSenarioStage();
    
    void nextButtonPressed();
    
    void clearElements();
    
    bool isHorizontal();
    
    void onOrientationChanged();
};

#endif
