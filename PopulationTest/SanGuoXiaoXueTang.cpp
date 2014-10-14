//
//  SanGuoXiaoXueTang.cpp
//  PopulationTest
//
//  Created by admin on 5/8/14.
//
//

#include "SanGuoXiaoXueTang.h"
#include "SanGuoXiaoXueTangFileReader.h"
#include "Question.h"
#include "GameScene.h"
#include "GameHUD.h"
#include "UIButtonControl.h"
#include "TutorialManager.h"
#include "SoundtrackManager.h"
#include "Senario.h"
#include "GlobalHelper.h"

SanGuoXiaoXueTang* SanGuoXiaoXueTang::SP;

SanGuoXiaoXueTang::SanGuoXiaoXueTang()
{
    SanGuoXiaoXueTang::SP = this;
    active = false;
    curQuestionIndex = 0;
    
    background = NULL;
    
    questionList = CCArray::create();
    questionList->retain();
    
    questionTokens = CCArray::create();
    questionTokens->retain();
    
    answerArray1 = CCArray::create();
    answerArray1->retain();
    
    answerArray2 = CCArray::create();
    answerArray2->retain();
    
    answerArray3 = CCArray::create();
    answerArray3->retain();
    
    answerArray4 = CCArray::create();
    answerArray4->retain();
    
    index = 0;
    isInQuestionStage = false;
    isInRewardStage = false;
    isInIntroStage = false;
    twoChoice = false;
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    correct = 0;
    wrong = 0;
    moneyReward = 0;
    
    startX = screenSize.width * 0.25f;
    maxX = screenSize.width * 1.7f;
    startY = screenSize.height * 1.3f;
    gapY = 60;
    offX = 0;
    offY = 0;
    
    theState = Part_Default;
}

SanGuoXiaoXueTang::~SanGuoXiaoXueTang()
{
    SanGuoXiaoXueTang::SP = NULL;
    
    questionList->removeAllObjects();
    CC_SAFE_RELEASE(questionList);
}

SanGuoXiaoXueTang* SanGuoXiaoXueTang::getThis()
{
    return SP;
}

SanGuoXiaoXueTang* SanGuoXiaoXueTang::create()
{
    SanGuoXiaoXueTang* pRet = new SanGuoXiaoXueTang();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool SanGuoXiaoXueTang::init()
{
    return true;
}

void SanGuoXiaoXueTang::createUI()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    
    background = CCSprite::createWithSpriteFrameName("quizscreen.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->cocos2d::CCNode::setScale(screenSize.width / background->boundingBox().size.width, screenSize.height / background->boundingBox().size.height);
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(background, 2);
     
    buttonAnswer1 = CCSprite::createWithSpriteFrameName("emptybtn.png");
    buttonAnswer2 = CCSprite::createWithSpriteFrameName("emptybtn.png");
    buttonAnswer3 = CCSprite::createWithSpriteFrameName("emptybtn.png");
    buttonAnswer4 = CCSprite::createWithSpriteFrameName("emptybtn.png");
    buttonNext = CCSprite::createWithSpriteFrameName("emptybtn.png");
    
    buttonAnswer1->setScale(1.8f);
    buttonAnswer2->setScale(1.8f);
    buttonAnswer3->setScale(1.8f);
    buttonAnswer4->setScale(1.8f);
    buttonNext->setScale(1.5f);
    
    buttonAnswer1->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonAnswer2->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonAnswer3->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonAnswer4->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonNext->setAnchorPoint(ccp(0.5f, 0.5f));
    
    buttonAnswer1->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.4f));
    buttonAnswer2->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.4f));
    buttonAnswer3->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.2f));
    buttonAnswer4->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.2f));
    buttonNext->setPosition(ccp(screenSize.width * 0.75f, screenSize.height * 0.15f));
    
    this->addChild(buttonAnswer1, 3);
    this->addChild(buttonAnswer2, 3);
    this->addChild(buttonAnswer3, 3);
    this->addChild(buttonAnswer4, 3);
    this->addChild(buttonNext, 3);
    
    feedbackIcon = CCSprite::createWithSpriteFrameName("tick.png");
    feedbackIcon->setScale(0.7f);
    feedbackIcon->setAnchorPoint(ccp(0.5f, 0.5f));
    feedbackIcon->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.41f));
    this->addChild(feedbackIcon, 5);
    
    feedbackLabel = CCLabelTTF::create("正确", "GillSansMT", 52);
    feedbackLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    feedbackLabel->setPosition(ccp(screenSize.width * 0.59f, screenSize.height * 0.41f));
    feedbackLabel->setColor(colorBlack);
    this->addChild(feedbackLabel, 5);
    
    buttonNextLabel = CCLabelTTF::create("继续", "GillSansMT", 68);
    buttonNextLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonNextLabel->setScale(0.5f);
    buttonNextLabel->setPosition(ccp(screenSize.width * 0.75f, screenSize.height * 0.15f));
    buttonNextLabel->setColor(colorBlack);
    this->addChild(buttonNextLabel, 4);
    
    /*
    answerLabel1 = CCLabelTTF::create("Option1: ", "GillSansMT", 52);
    answerLabel1->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel1->setScale(0.5f);
    answerLabel1->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.4f));
    answerLabel1->setColor(colorBlack);
    this->addChild(answerLabel1, 4);
    
    answerLabel2 = CCLabelTTF::create("Option2: ", "GillSansMT", 52);
    answerLabel2->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel2->setScale(0.5f);
    answerLabel2->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.4f));
    answerLabel2->setColor(colorBlack);
    this->addChild(answerLabel2, 4);
    
    answerLabel3 = CCLabelTTF::create("Option3: ", "GillSansMT", 52);
    answerLabel3->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel3->setScale(0.5f);
    answerLabel3->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.2f));
    answerLabel3->setColor(colorBlack);
    this->addChild(answerLabel3, 4);
    
    answerLabel4 = CCLabelTTF::create("Option4: ", "GillSansMT", 52);
    answerLabel4->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel4->setScale(0.5f);
    answerLabel4->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.2f));
    answerLabel4->setColor(colorBlack);
    this->addChild(answerLabel4, 4);
    */
    
    dragon = CCSprite::createWithSpriteFrameName("dragon_c.png");
    dragon->setScale(screenSize.width / dragon->boundingBox().size.width * 0.2f);
    dragon->setAnchorPoint(ccp(0.5f, 0.5f));
    dragon->setPosition(ccp(screenSize.width * 0.4f, screenSize.height * 0.25f));
    dragon->setFlipX(true);
    this->addChild(dragon, 3);
    
    bubble = CCSprite::createWithSpriteFrameName("truefalsebubblespeech.png");
    bubble->cocos2d::CCNode::setScale(screenSize.width / bubble->boundingBox().size.width * 0.25f, screenSize.width / bubble->boundingBox().size.width * 0.18f);
    bubble->setAnchorPoint(ccp(0.5f, 0.5f));
    bubble->setPosition(ccp(screenSize.width * 0.55f, screenSize.height * 0.4f));
    this->addChild(bubble, 4);
    
    resultTitleLabel = CCLabelTTF::create("成绩：", "GillSansMT", 52);
    resultTitleLabel->setAnchorPoint(ccp(0, 0.5f));
    resultTitleLabel->setScale(0.5f);
    resultTitleLabel->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.6f));
    resultTitleLabel->setColor(colorBlack);
    this->addChild(resultTitleLabel, 4);
    
    resultLabel1 = CCLabelTTF::create("正确            X 0", "GillSansMT", 52);
    resultLabel1->setAnchorPoint(ccp(0, 0.5f));
    resultLabel1->setScale(0.5f);
    resultLabel1->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.53f));
    resultLabel1->setColor(colorBlack);
    this->addChild(resultLabel1, 4);
    
    resultLabel2 = CCLabelTTF::create("错误            X 0", "GillSansMT", 52);
    resultLabel2->setAnchorPoint(ccp(0, 0.5f));
    resultLabel2->setScale(0.5f);
    resultLabel2->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.46f));
    resultLabel2->setColor(colorBlack);
    this->addChild(resultLabel2, 4);
    
    rewardTitleLabel = CCLabelTTF::create("你获得了：", "GillSansMT", 52);
    rewardTitleLabel->setAnchorPoint(ccp(0, 0.5f));
    rewardTitleLabel->setScale(0.5f);
    rewardTitleLabel->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.37f));
    rewardTitleLabel->setColor(colorBlack);
    this->addChild(rewardTitleLabel, 4);
    
    rewardLabel = CCLabelTTF::create("Gold            X 0", "GillSansMT", 52);
    rewardLabel->setAnchorPoint(ccp(0, 0.5f));
    rewardLabel->setScale(0.5f);
    rewardLabel->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.3f));
    rewardLabel->setColor(colorBlack);
    this->addChild(rewardLabel, 4);
}

void SanGuoXiaoXueTang::showUI()
{
    // pause the game play when san guo xiao xue tang comes out......
    UIButtonControl::pauseGame();
    
    correct = 0;
    wrong = 0;
    moneyReward = 0;
    
    active = true;
    preloadTextures();
    createUI();
    curQuestionIndex = 0;
    isInQuestionStage = true;
    isInRewardStage = false;
    readQuestionFiles();
    loadQuestion();
    updateUI();
}

void SanGuoXiaoXueTang::hideUI()
{
    active = false;
    clear();
    // san guo xiao xue tang has been completed, resume the game play
    UIButtonControl::resumeGame();
    
    switch (theState) {
        case Part_1:
        {
            SoundtrackManager::PlayBGM("Ishikari Lore.mp3");
            TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
        }
            break;
        case Part_2:
        {
            string filename = "scenario2.xml";
            Senario::getThis()->scenarioState = Scenario2;
            Senario::getThis()->playSenario(filename.c_str());
            ObjectiveHandler::getThis()->playObjective();
        }
            break;
        case Part_3:
        {
            string filename = "senario_h4.xml";
            Senario::getThis()->scenarioState = Scenario4;
            Senario::getThis()->playSenario(filename.c_str());
            ObjectiveHandler::getThis()->playObjective();
        }
            break;
        case Part_4:
        {
            string filename = "senario_h5.xml";
            Senario::getThis()->scenarioState = Scenario5;
            Senario::getThis()->playSenario(filename.c_str());
            ObjectiveHandler::getThis()->playObjective();
        }
            break;
        default:
            break;
    }
    
    GameHUD::getThis()->scheduleAddMoney(moneyReward);
    // GameHUD::getThis()->clickScoreButton();
}

void SanGuoXiaoXueTang::readQuestionFiles()
{
    questionList->removeAllObjects();
    CC_SAFE_RELEASE(questionList);
    
    SanGuoXiaoXueTangFileReader* sgxxtFr = new SanGuoXiaoXueTangFileReader();
    
    switch(theState)
    {
        case Part_1:
            sgxxtFr->parseXMLFile("questions_part1.xml");
            break;
        case Part_2:
            sgxxtFr->parseXMLFile("questions_part2.xml");
            break;
        case Part_3:
            sgxxtFr->parseXMLFile("questions_part3.xml");
            break;
        case Part_4:
            sgxxtFr->parseXMLFile("questions_part4.xml");
            break;
        default:
            sgxxtFr->parseXMLFile("questions_part1.xml");
            break;
    }
    questionList = sgxxtFr->questionList;
    delete sgxxtFr;
}

void SanGuoXiaoXueTang::loadQuestion()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
    Question* question = (Question*) questionList->objectAtIndex(curQuestionIndex);
    
    clearQuestion();
    offX = 0;
    offY = 0;
    
    vector<std::string> tokens;
    
    if(question->getIsIntro())
    {
        isInIntroStage = true;
        tokens = GlobalHelper::split(question->getIntroString(), '^');
    }
    else
    {
        isInIntroStage = false;
        tokens = GlobalHelper::split(question->getQuestionString(), '^');
    }
    
    for(int i = 0; i < tokens.size(); i++)
    {
        CCLabelTTF* theLabel = CCLabelTTF::create(tokens.at(i).c_str(), "GillSansMT", 52);
        theLabel->setAnchorPoint(ccp(0, 1.0f));
        theLabel->setColor(colorBlack);
        
        if(startX + offX > maxX)
        {
            offX = 0;
            offY += gapY;
        }
        
        theLabel->setPosition(ccp(startX + offX, startY - offY));
        offX += theLabel->boundingBox().size.width;
        background->addChild(theLabel);
        questionTokens->addObject(theLabel);
    }
    
    twoChoice = question->getTwoChoice();
    
    if(isInIntroStage)
    {
        return;
    }
    
    if(question->getAnswers().at(0).find("^") != std::string::npos)
    {
        vector<std::string> answerTokens = GlobalHelper::split(question->getAnswers().at(0), '^');
        
        CCLabelTTF* answerLabel1 = CCLabelTTF::create(answerTokens.at(0).c_str(), "GillSansMT", 52);
        answerLabel1->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel1->setScale(0.5f);
        answerLabel1->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.4f + 20));
        answerLabel1->setColor(colorBlack);
        this->addChild(answerLabel1, 4);
        answerArray1->addObject(answerLabel1);
        
        CCLabelTTF* answerLabel2 = CCLabelTTF::create(answerTokens.at(1).c_str(), "GillSansMT", 52);
        answerLabel2->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel2->setScale(0.5f);
        answerLabel2->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.4f - 20));
        answerLabel2->setColor(colorBlack);
        this->addChild(answerLabel2, 4);
        answerArray1->addObject(answerLabel2);
        
    }
    else
    {
        CCLabelTTF* answerLabel = CCLabelTTF::create(question->getAnswers().at(0).c_str(), "GillSansMT", 52);
        answerLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel->setScale(0.5f);
        answerLabel->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.4f));
        answerLabel->setColor(colorBlack);
        this->addChild(answerLabel, 4);
        answerArray1->addObject(answerLabel);
    }
    
    if(question->getAnswers().at(1).find("^") != std::string::npos)
    {
        vector<std::string> answerTokens = GlobalHelper::split(question->getAnswers().at(1), '^');
        
        CCLabelTTF* answerLabel1 = CCLabelTTF::create(answerTokens.at(0).c_str(), "GillSansMT", 52);
        answerLabel1->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel1->setScale(0.5f);
        answerLabel1->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.4f + 20));
        answerLabel1->setColor(colorBlack);
        this->addChild(answerLabel1, 4);
        answerArray2->addObject(answerLabel1);
        
        CCLabelTTF* answerLabel2 = CCLabelTTF::create(answerTokens.at(1).c_str(), "GillSansMT", 52);
        answerLabel2->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel2->setScale(0.5f);
        answerLabel2->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.4f - 20));
        answerLabel2->setColor(colorBlack);
        this->addChild(answerLabel2, 4);
        answerArray2->addObject(answerLabel2);
    }
    else
    {
        CCLabelTTF* answerLabel = CCLabelTTF::create(question->getAnswers().at(1).c_str(), "GillSansMT", 52);
        answerLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel->setScale(0.5f);
        answerLabel->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.4f));
        answerLabel->setColor(colorBlack);
        this->addChild(answerLabel, 4);
        answerArray2->addObject(answerLabel);
    }
    
    if(question->getAnswers().at(2).find("^") != std::string::npos)
    {
        vector<std::string> answerTokens = GlobalHelper::split(question->getAnswers().at(2), '^');
        
        CCLabelTTF* answerLabel1 = CCLabelTTF::create(answerTokens.at(0).c_str(), "GillSansMT", 52);
        answerLabel1->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel1->setScale(0.5f);
        answerLabel1->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.2f + 20));
        answerLabel1->setColor(colorBlack);
        this->addChild(answerLabel1, 4);
        answerArray3->addObject(answerLabel1);
        
        CCLabelTTF* answerLabel2 = CCLabelTTF::create(answerTokens.at(1).c_str(), "GillSansMT", 52);
        answerLabel2->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel2->setScale(0.5f);
        answerLabel2->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.2f - 20));
        answerLabel2->setColor(colorBlack);
        this->addChild(answerLabel2, 4);
        answerArray3->addObject(answerLabel2);
        
    }
    else
    {
        CCLabelTTF* answerLabel = CCLabelTTF::create(question->getAnswers().at(2).c_str(), "GillSansMT", 52);
        answerLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel->setScale(0.5f);
        answerLabel->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.2f));
        answerLabel->setColor(colorBlack);
        this->addChild(answerLabel, 4);
        answerArray3->addObject(answerLabel);
    }
    
    if(question->getAnswers().at(3).find("^") != std::string::npos)
    {
        vector<std::string> answerTokens = GlobalHelper::split(question->getAnswers().at(3), '^');
        
        CCLabelTTF* answerLabel1 = CCLabelTTF::create(answerTokens.at(0).c_str(), "GillSansMT", 52);
        answerLabel1->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel1->setScale(0.5f);
        answerLabel1->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.2f + 20));
        answerLabel1->setColor(colorBlack);
        this->addChild(answerLabel1, 4);
        answerArray4->addObject(answerLabel1);
        
        CCLabelTTF* answerLabel2 = CCLabelTTF::create(answerTokens.at(1).c_str(), "GillSansMT", 52);
        answerLabel2->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel2->setScale(0.5f);
        answerLabel2->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.2f - 20));
        answerLabel2->setColor(colorBlack);
        this->addChild(answerLabel2, 4);
        answerArray4->addObject(answerLabel2);
        
    }
    else
    {
        CCLabelTTF* answerLabel = CCLabelTTF::create(question->getAnswers().at(3).c_str(), "GillSansMT", 52);
        answerLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        answerLabel->setScale(0.5f);
        answerLabel->setPosition(ccp(screenSize.width * 0.7f, screenSize.height * 0.2f));
        answerLabel->setColor(colorBlack);
        this->addChild(answerLabel, 4);
        answerArray4->addObject(answerLabel);
    }
    
    index = question->answerIndex;
}

void SanGuoXiaoXueTang::clearQuestion()
{
    for (int i = 0; i < questionTokens->count(); i++)
    {
        CCLabelTTF* theLabel = (CCLabelTTF*)questionTokens->objectAtIndex(i);
        background->removeChild(theLabel, true);
    }
    questionTokens->removeAllObjects();
    
    for (int i = 0; i < answerArray1->count(); i++)
    {
        CCLabelTTF* theLabel = (CCLabelTTF*)answerArray1->objectAtIndex(i);
        background->removeChild(theLabel, true);
    }
    answerArray1->removeAllObjects();
    
    for (int i = 0; i < answerArray2->count(); i++)
    {
        CCLabelTTF* theLabel = (CCLabelTTF*)answerArray2->objectAtIndex(i);
        background->removeChild(theLabel, true);
    }
    answerArray2->removeAllObjects();
    
    for (int i = 0; i < answerArray3->count(); i++)
    {
        CCLabelTTF* theLabel = (CCLabelTTF*)answerArray3->objectAtIndex(i);
        background->removeChild(theLabel, true);
    }
    answerArray3->removeAllObjects();
    
    for (int i = 0; i < answerArray4->count(); i++)
    {
        CCLabelTTF* theLabel = (CCLabelTTF*)answerArray4->objectAtIndex(i);
        background->removeChild(theLabel, true);
    }
    answerArray4->removeAllObjects();
}

void SanGuoXiaoXueTang::clickSanGuoXiaoXueTangButton()
{
    if(active)
    {
        hideUI();
    }
    else
    {
        showUI();
    }
}

bool SanGuoXiaoXueTang::isActive()
{
    return active;
}

void SanGuoXiaoXueTang::preloadTextures()
{
    myNode = CCSpriteBatchNode::create("SanGuoXiaoXueTang.png");
    this->addChild(myNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SanGuoXiaoXueTang.plist");
}

void SanGuoXiaoXueTang::clearTextures()
{
    this->removeChild(myNode, true);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("SanGuoXiaoXueTang.plist");
}

void SanGuoXiaoXueTang::clear()
{
    clearQuestion();
    
    /*
    buttonAnswer1->removeChild(answerLabel1, true);
    buttonAnswer2->removeChild(answerLabel2, true);
    buttonAnswer3->removeChild(answerLabel3, true);
    buttonAnswer4->removeChild(answerLabel4, true);
    */
    buttonNext->removeChild(buttonNextLabel, true);
    
    this->removeChild(buttonAnswer1, true);
    this->removeChild(buttonAnswer2, true);
    this->removeChild(buttonAnswer3, true);
    this->removeChild(buttonAnswer4, true);
    this->removeChild(buttonNext, true);
    /*
    this->removeChild(answerLabel1, true);
    this->removeChild(answerLabel2, true);
    this->removeChild(answerLabel3, true);
    this->removeChild(answerLabel4, true);
    */
    this->removeChild(buttonNextLabel, true);
    this->removeChild(dragon, true);
    this->removeChild(bubble, true);
    this->removeChild(feedbackIcon, true);
    this->removeChild(feedbackLabel, true);
    this->removeChild(resultTitleLabel, true);
    this->removeChild(resultLabel1, true);
    this->removeChild(resultLabel2, true);
    this->removeChild(rewardTitleLabel, true);
    this->removeChild(rewardLabel, true);
    
    this->removeChild(background, true);
    
    questionList->removeAllObjects();
    CC_SAFE_RELEASE(questionList);
    
    questionList = CCArray::create();
    questionList->retain();
    
    background = NULL;
    
    clearTextures();
}

void SanGuoXiaoXueTang::updateUI()
{
    if(isInIntroStage)
    {
        buttonNext->setVisible(true);
        buttonNextLabel->setVisible(true);
        feedbackIcon->setVisible(false);
        feedbackLabel->setVisible(false);
        dragon->setVisible(false);
        bubble->setVisible(false);
        
        buttonAnswer1->setVisible(false);
        buttonAnswer2->setVisible(false);
        buttonAnswer3->setVisible(false);
        buttonAnswer4->setVisible(false);
        
        /*
         answerLabel1->setVisible(false);
         answerLabel2->setVisible(false);
         answerLabel3->setVisible(false);
         answerLabel4->setVisible(false);
         */
        for (int i = 0; i < answerArray1->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray1->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray2->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray2->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray3->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray3->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray4->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray4->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        resultTitleLabel->setVisible(false);
        resultLabel1->setVisible(false);
        resultLabel2->setVisible(false);
        rewardTitleLabel->setVisible(false);
        rewardLabel->setVisible(false);
    }
    else if(isInRewardStage)
    {
        buttonNext->setVisible(true);
        buttonNextLabel->setVisible(true);
        feedbackIcon->setVisible(false);
        feedbackLabel->setVisible(false);
        dragon->setVisible(false);
        bubble->setVisible(false);
        
        buttonAnswer1->setVisible(false);
        buttonAnswer2->setVisible(false);
        buttonAnswer3->setVisible(false);
        buttonAnswer4->setVisible(false);
        
        /*
        answerLabel1->setVisible(false);
        answerLabel2->setVisible(false);
        answerLabel3->setVisible(false);
        answerLabel4->setVisible(false);
        */
        for (int i = 0; i < answerArray1->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray1->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray2->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray2->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray3->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray3->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray4->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray4->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        resultTitleLabel->setVisible(true);
        resultLabel1->setVisible(true);
        resultLabel2->setVisible(true);
        rewardTitleLabel->setVisible(true);
        rewardLabel->setVisible(true);
    }
    else if(isInQuestionStage)
    {
        buttonNext->setVisible(false);
        buttonNextLabel->setVisible(false);
        feedbackIcon->setVisible(false);
        feedbackLabel->setVisible(false);
        dragon->setVisible(false);
        bubble->setVisible(false);
        
        buttonAnswer1->setVisible(true);
        buttonAnswer2->setVisible(true);
        
        if(twoChoice)
        {
            buttonAnswer3->setVisible(false);
            buttonAnswer4->setVisible(false);
        }
        else
        {
            buttonAnswer3->setVisible(true);
            buttonAnswer4->setVisible(true);
        }
        
        /*
        answerLabel1->setVisible(true);
        answerLabel2->setVisible(true);
        answerLabel3->setVisible(true);
        answerLabel4->setVisible(true);
        */
        for (int i = 0; i < answerArray1->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray1->objectAtIndex(i);
            theLabel->setVisible(true);
        }
        
        for (int i = 0; i < answerArray2->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray2->objectAtIndex(i);
            theLabel->setVisible(true);
        }
        
        for (int i = 0; i < answerArray3->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray3->objectAtIndex(i);
            theLabel->setVisible(true);
        }
        
        for (int i = 0; i < answerArray4->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray4->objectAtIndex(i);
            theLabel->setVisible(true);
        }
        
        resultTitleLabel->setVisible(false);
        resultLabel1->setVisible(false);
        resultLabel2->setVisible(false);
        rewardTitleLabel->setVisible(false);
        rewardLabel->setVisible(false);
    }
    else
    {
        buttonNext->setVisible(true);
        buttonNextLabel->setVisible(true);
        feedbackIcon->setVisible(true);
        feedbackLabel->setVisible(true);
        dragon->setVisible(true);
        bubble->setVisible(true);
        
        buttonAnswer1->setVisible(false);
        buttonAnswer2->setVisible(false);
        buttonAnswer3->setVisible(false);
        buttonAnswer4->setVisible(false);
        
        /*
        answerLabel1->setVisible(false);
        answerLabel2->setVisible(false);
        answerLabel3->setVisible(false);
        answerLabel4->setVisible(false);
        */
        for (int i = 0; i < answerArray1->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray1->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray2->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray2->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray3->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray3->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        for (int i = 0; i < answerArray4->count(); i++)
        {
            CCLabelTTF* theLabel = (CCLabelTTF*) answerArray4->objectAtIndex(i);
            theLabel->setVisible(false);
        }
        
        resultTitleLabel->setVisible(false);
        resultLabel1->setVisible(false);
        resultLabel2->setVisible(false);
        rewardTitleLabel->setVisible(false);
        rewardLabel->setVisible(false);
    }
}

void SanGuoXiaoXueTang::onButtonAnswer1Pressed()
{
    if(index == 1 || index == 0)
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tick.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("正确");
        correct++;
        moneyReward += 200;
    }
    else
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wrong.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("错误");
        wrong++;
    }
    isInQuestionStage = false;
    isInRewardStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonAnswer2Pressed()
{
    if(index == 2 || index == 0)
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tick.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("正确");
        correct++;
        moneyReward += 200;
    }
    else
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wrong.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("错误");
        wrong++;
    }
    isInQuestionStage = false;
    isInRewardStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonAnswer3Pressed()
{
    if(index == 3 || index == 0)
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tick.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("正确");
        correct++;
        moneyReward += 200;
    }
    else
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wrong.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("错误");
        wrong++;
    }
    isInQuestionStage = false;
    isInRewardStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonAnswer4Pressed()
{
    if(index == 4 || index == 0)
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tick.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("正确");
        correct++;
        moneyReward += 200;
    }
    else
    {
        CCSpriteFrame* theFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wrong.png");
        feedbackIcon->setTexture(theFrame->getTexture());
        feedbackIcon->setTextureRect(theFrame->getRect());
        feedbackLabel->setString("错误");
        wrong++;
    }
    isInQuestionStage = false;
    isInRewardStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonNextPressed()
{
    curQuestionIndex++;
    if(curQuestionIndex < questionList->count())
    {
        loadQuestion();
        isInQuestionStage = true;
        updateUI();
    }
    else if(curQuestionIndex == questionList->count())
    {
        isInQuestionStage = false;
        isInRewardStage = true;
        clearQuestion();
        
        stringstream ss;
        ss << "正确            X " << correct;
        resultLabel1->setString(ss.str().c_str());
        
        ss.str(std::string());
        ss << "错误            X " << wrong;
        resultLabel2->setString(ss.str().c_str());
        
        ss.str(std::string());
        ss << "Gold           X " << moneyReward;
        rewardLabel->setString(ss.str().c_str());
        
        updateUI();
    }
    else
    {
        isInRewardStage = false;
        hideUI();
    }
}