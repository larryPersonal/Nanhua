//
//  RandomEventManager.h
//  PopulationTest
//
//  Created by admin on 8/7/14.
//
//

#ifndef PopulationTest_RandomEventManager_h
#define PopulationTest_RandomEventManager_h

#include "cocos2d.h"

using namespace cocos2d;

class RandomEventManager : public CCLayer
{
public:
    static RandomEventManager* SP;
    
    CCSprite* background;
    
    CCRect* imageRect;
    CCRect* effectRect;
    
    CCMenuItem* okButton;
    CCMenuItem* cancelButton;
    
    CCLabelTTF* titleLabel;
    
public:
    RandomEventManager();
    ~RandomEventManager();
    
    static RandomEventManager* create();
    static RandomEventManager* getThis();
    
    bool init();
    
    void createUI();
    
    void showUI();
    void hideUI();
};

#endif
