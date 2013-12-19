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

using namespace cocos2d;

class Senario : public CCLayer
{
public:
    static Senario* SP;
    
    CCSprite* character;
    CCMenuItem* chatbox;
    
    char* text;
    
    bool active;
    
public:
    Senario();
    ~Senario();
    
    static Senario* create();
    static Senario* getThis();
    
    bool init();
    
    void playSenario(char* senario);
    
    void readSenarioFile(char* senario);
    
    void createGUI();
    
    void buttonSelect();
    
    void updateGameScene();
};

#endif
