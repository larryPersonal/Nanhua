//
//  SanGuoXiaoXueTang.h
//  PopulationTest
//
//  Created by admin on 5/8/14.
//
//

#ifndef PopulationTest_SanGuoXiaoXueTang_h
#define PopulationTest_SanGuoXiaoXueTang_h

#include "cocos2d.h"

using namespace cocos2d;

class SanGuoXiaoXueTang : public CCLayer
{
public:
    static SanGuoXiaoXueTang* SP;
    
    CCSprite* background;
    
    CCSprite* frame;
    
    CCSpriteBatchNode* myNode;
    
    CCLabelTTF* titleLabel;
    CCLabelTTF* answerLabel1;
    CCLabelTTF* answerLabel2;
    CCLabelTTF* answerLabel3;
    CCLabelTTF* answerLabel4;
    
    CCSprite* buttonAnswer1;
    CCSprite* buttonAnswer2;
    CCSprite* buttonAnswer3;
    CCSprite* buttonAnswer4;
    
    CCLabelTTF* feedbackLabel;
    CCLabelTTF* buttonNextLabel;
    CCSprite* buttonNext;
    
    int curQuestionIndex;
    CCArray* questionList;
    
    bool active;
    bool isInQuestionStage;
    
public:
    int index;
    
private:
    bool init();
    void createUI();
    
    void preloadTextures();
    void clearTextures();
    
    void clear();
    void updateUI();
    
public:
    SanGuoXiaoXueTang();
    ~SanGuoXiaoXueTang();
    
    static SanGuoXiaoXueTang* create();
    static SanGuoXiaoXueTang* getThis();
    
    void showUI();
    void hideUI();
    
    void clickSanGuoXiaoXueTangButton();
    
    bool isActive();
    
    void onButtonAnswer1Pressed();
    void onButtonAnswer2Pressed();
    void onButtonAnswer3Pressed();
    void onButtonAnswer4Pressed();
    
    void onButtonNextPressed();
    
    void readQuestionFiles();
    void loadQuestion();
};

#endif
