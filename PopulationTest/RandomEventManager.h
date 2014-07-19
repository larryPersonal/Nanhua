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
#include "RandomEvent.h"

using namespace cocos2d;

class RandomEventManager : public CCLayer
{
public:
    static RandomEventManager* SP;
    
    CCSprite* background;
    
    CCMenuItem* okButton;
    CCMenu* menu;
    
    CCLabelTTF* titleLabel;
    
    CCSprite* blackScreen;
    
    CCArray* randomEvents;
    
    RandomEvent* theRandomEvent;
    CCSprite* image;
    
    CCLabelTTF* descriptionLabel;
    
    bool active;
    
public:
    RandomEventManager();
    ~RandomEventManager();
    
    static RandomEventManager* create();
    static RandomEventManager* getThis();
    
    bool init();
    
    void createUI();
    
    void showUI();
    void hideUI();
    
    void constructRandomEventContent();
    void removeRandomEventContent();
    
    void clickSystemButton();
    
    void readRandomEventsFile();
    
    void applyRandomEventEffects();
};

#endif
