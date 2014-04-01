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
    
    CCMenuItem* chatbox;
    
    char* text;
    
    bool active;
    bool inOption;
    
    int curSlide;
    
    vector<CCSprite*> spriteList;
    vector<CCMenu*> menuList;
    vector<CCLabelTTF*> labelList;
    
    CCSprite* skipButton;
    
    CCArray* slidesList;
    CCArray* animatedStringList;
    CCArray* animatedSpriteList;
    
    CCMenu* startGameMenu;
    
    bool skipSlide;
    
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
    
    bool constructSenarioStage(bool);
    
    void nextButtonPressed(bool);
    
    void clearElements();
    
    void onOrientationChanged();
    
    void selectButtonPressed(CCObject* pSender);
    
    void displayTexts(std::string, float, float, string, float, ccColor3B);
    
    void update(float time);
    
    void goNextSlide();
    
    vector<std::string> split(std::string, char);
};

#endif
