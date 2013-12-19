//
//  SenarioChooseScene.h
//  PopulationTest
//
//  Created by SGI on 12/12/13.
//
//

#ifndef PopulationTest_SenarioChooseScene_h
#define PopulationTest_SenarioChooseScene_h

#include "cocos2d.h"

using namespace cocos2d;

class SenarioChooseScene : public cocos2d::CCLayer
{
public:
    static SenarioChooseScene* SP;
    
    CCSprite* backgroundImage;
    CCSprite* backgroundDemonstrationPanel;
    CCSprite* backgroundChoosePanel;
    
    CCMenuItem* senarioButtonTutorial;
    CCMenuItem* senarioButtonS1;
    CCMenuItem* senarioButtonS2;
    CCMenuItem* senarioButtonS3;
    CCMenuItem* senarioButtonS4;
    CCMenuItem* senarioButtonS5;
    CCMenuItem* senarioButtonS6;
    
public:
    SenarioChooseScene();
    ~SenarioChooseScene();
    
    static SenarioChooseScene* getThis();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    void chooseTutorial();
    void chooseSenario1();
    void chooseSenario2();
    void chooseSenario3();
    void chooseSenario4();
    void chooseSenario5();
    void chooseSenario6();
    
    void onOrientateChange();
    void onOrientationChangedToPortrait();
    void onOrientationChangedToLandscape();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(SenarioChooseScene);
};


#endif
