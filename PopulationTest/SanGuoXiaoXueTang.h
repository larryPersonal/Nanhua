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

enum SanGuoXiaoXueTangState
{
    Part_Default, Part_1, Part_2, Part_3, Part_4
};

class SanGuoXiaoXueTang : public CCLayer
{
public:
    static SanGuoXiaoXueTang* SP;
    
    CCSprite* background;
    
    CCSpriteBatchNode* myNode;
    
    /*
    CCLabelTTF* titleLabel;
    CCLabelTTF* answerLabel1;
    CCLabelTTF* answerLabel2;
    CCLabelTTF* answerLabel3;
    CCLabelTTF* answerLabel4;
    */
    
    CCSprite* buttonAnswer1;
    CCSprite* buttonAnswer2;
    CCSprite* buttonAnswer3;
    CCSprite* buttonAnswer4;
    
    CCLabelTTF* resultTitleLabel;
    CCLabelTTF* resultLabel1;
    CCLabelTTF* resultLabel2;
    CCLabelTTF* rewardTitleLabel;
    CCLabelTTF* rewardLabel;
    
    int correct;
    int wrong;
    int moneyReward;
    
    CCSprite* feedbackIcon;
    CCLabelTTF* feedbackLabel;
    CCLabelTTF* buttonNextLabel;
    CCSprite* buttonNext;
    
    CCSprite* dragon;
    CCSprite* bubble;
    
    int curQuestionIndex;
    CCArray* questionList;
    
    bool active;
    bool isInQuestionStage;
    bool isInRewardStage;
    bool isInIntroStage;
    bool twoChoice;
    
    CCArray* questionTokens;
    CCArray* answerArray1;
    CCArray* answerArray2;
    CCArray* answerArray3;
    CCArray* answerArray4;
    
    float startX;
    float startY;
    float maxX;
    float gapY;
    float offX;
    float offY;
    
    SanGuoXiaoXueTangState theState;
    
    bool lockModule;
    
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
    void clearQuestion();
};

#endif
