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

SanGuoXiaoXueTang* SanGuoXiaoXueTang::SP;

SanGuoXiaoXueTang::SanGuoXiaoXueTang()
{
    SanGuoXiaoXueTang::SP = this;
    active = false;
    curQuestionIndex = 0;
    
    background = NULL;
    
    questionList = CCArray::create();
    questionList->retain();
    
    index = 0;
    isInQuestionStage = false;
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
    
    background = CCSprite::createWithSpriteFrameName("quizscreen.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->cocos2d::CCNode::setScale(screenSize.width / background->boundingBox().size.width, screenSize.height / background->boundingBox().size.height);
    background->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(background, 2);
    
    frame = CCSprite::createWithSpriteFrameName("scrollerbreaknews.png");
    frame->setAnchorPoint(ccp(0.5f, 0.5f));
    frame->setScale(2.0f);
    frame->setPosition(ccp(screenSize.width, screenSize.height / 1.4f));
    background->addChild(frame);
    
    titleLabel = CCLabelTTF::create("Sample Question: ", "GillSansMT", 48);
    titleLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    titleLabel->setPosition(ccp(screenSize.width, screenSize.height * 1.1f));
    background->addChild(titleLabel);
    
    buttonAnswer1 = CCSprite::create("button_gy.png");
    buttonAnswer2 = CCSprite::create("button_gy.png");
    buttonAnswer3 = CCSprite::create("button_gy.png");
    buttonAnswer4 = CCSprite::create("button_gy.png");
    buttonNext = CCSprite::create("button_gy.png");
    
    buttonAnswer1->setScale(0.25f);
    buttonAnswer2->setScale(0.25f);
    buttonAnswer3->setScale(0.25f);
    buttonAnswer4->setScale(0.25f);
    buttonNext->setScale(0.25f);
    
    buttonAnswer1->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonAnswer2->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonAnswer3->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonAnswer4->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonNext->setAnchorPoint(ccp(0.5f, 0.5f));
    
    buttonAnswer1->setPosition(ccp(screenSize.width * 0.35f, screenSize.height * 0.4f));
    buttonAnswer2->setPosition(ccp(screenSize.width * 0.65f, screenSize.height * 0.4f));
    buttonAnswer3->setPosition(ccp(screenSize.width * 0.35f, screenSize.height * 0.25f));
    buttonAnswer4->setPosition(ccp(screenSize.width * 0.65f, screenSize.height * 0.25f));
    buttonNext->setPosition(ccp(screenSize.width * 0.65f, screenSize.height * 0.25f));
    
    this->addChild(buttonAnswer1, 3);
    this->addChild(buttonAnswer2, 3);
    this->addChild(buttonAnswer3, 3);
    this->addChild(buttonAnswer4, 3);
    this->addChild(buttonNext, 3);
    
    feedbackLabel = CCLabelTTF::create("正确", "GillSansMT", 68);
    feedbackLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    feedbackLabel->setPosition(ccp(screenSize.width, screenSize.height * 0.75f));
    background->addChild(feedbackLabel);
    
    buttonNextLabel = CCLabelTTF::create("继续", "GillSansMT", 68);
    buttonNextLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    buttonNextLabel->setPosition(ccp(screenSize.width / 4.0f + 100.0f, screenSize.height / 4.0f));
    buttonNext->addChild(buttonNextLabel);
    
    answerLabel1 = CCLabelTTF::create("Option1: ", "GillSansMT", 68);
    answerLabel1->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel1->setPosition(ccp(screenSize.width / 4.0f + 100.0f, screenSize.height / 4.0f));
    buttonAnswer1->addChild(answerLabel1);
    
    answerLabel2 = CCLabelTTF::create("Option2: ", "GillSansMT", 68);
    answerLabel2->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel2->setPosition(ccp(screenSize.width / 4.0f + 100.0f, screenSize.height / 4.0f));
    buttonAnswer2->addChild(answerLabel2);
    
    answerLabel3 = CCLabelTTF::create("Option3: ", "GillSansMT", 68);
    answerLabel3->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel3->setPosition(ccp(screenSize.width / 4.0f + 100.0f, screenSize.height / 4.0f));
    buttonAnswer3->addChild(answerLabel3);
    
    answerLabel4 = CCLabelTTF::create("Option4: ", "GillSansMT", 68);
    answerLabel4->setAnchorPoint(ccp(0.5f, 0.5f));
    answerLabel4->setPosition(ccp(screenSize.width / 4.0f + 100.0f, screenSize.height / 4.0f));
    buttonAnswer4->addChild(answerLabel4);
}

void SanGuoXiaoXueTang::showUI()
{
    // pause the game play when san guo xiao xue tang comes out......
    UIButtonControl::pauseGame();
    
    active = true;
    preloadTextures();
    createUI();
    curQuestionIndex = 0;
    isInQuestionStage = true;
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
    
    GameHUD::getThis()->clickScoreButton();
}

void SanGuoXiaoXueTang::readQuestionFiles()
{
    questionList->removeAllObjects();
    CC_SAFE_RELEASE(questionList);
    
    SanGuoXiaoXueTangFileReader* sgxxtFr = new SanGuoXiaoXueTangFileReader();
    sgxxtFr->parseXMLFile("questions.xml");
    questionList = sgxxtFr->questionList;
    delete sgxxtFr;
}

void SanGuoXiaoXueTang::loadQuestion()
{
    Question* question = (Question*) questionList->objectAtIndex(curQuestionIndex);
    titleLabel->setString(question->getQuestionString().c_str());
    
    answerLabel1->setString(question->getAnswers().at(0).c_str());
    answerLabel2->setString(question->getAnswers().at(1).c_str());
    answerLabel3->setString(question->getAnswers().at(2).c_str());
    answerLabel4->setString(question->getAnswers().at(3).c_str());
    
    index = question->answerIndex;
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
    buttonAnswer1->removeChild(answerLabel1, true);
    buttonAnswer2->removeChild(answerLabel2, true);
    buttonAnswer3->removeChild(answerLabel3, true);
    buttonAnswer4->removeChild(answerLabel4, true);
    buttonNext->removeChild(buttonNextLabel, true);
    
    background->removeChild(frame, true);
    background->removeChild(titleLabel, true);
    this->removeChild(buttonAnswer1, true);
    this->removeChild(buttonAnswer2, true);
    this->removeChild(buttonAnswer3, true);
    this->removeChild(buttonAnswer4, true);
    this->removeChild(buttonNext, true);
    background->removeChild(feedbackLabel, true);
    
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
    if(isInQuestionStage)
    {
        buttonNext->setVisible(false);
        feedbackLabel->setVisible(false);
        
        buttonAnswer1->setVisible(true);
        buttonAnswer2->setVisible(true);
        buttonAnswer3->setVisible(true);
        buttonAnswer4->setVisible(true);
    }
    else
    {
        buttonNext->setVisible(true);
        feedbackLabel->setVisible(true);
        
        buttonAnswer1->setVisible(false);
        buttonAnswer2->setVisible(false);
        buttonAnswer3->setVisible(false);
        buttonAnswer4->setVisible(false);
    }
}

void SanGuoXiaoXueTang::onButtonAnswer1Pressed()
{
    if(index == 1)
    {
        feedbackLabel->setString("正确");
    }
    else
    {
        feedbackLabel->setString("错误");
    }
    isInQuestionStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonAnswer2Pressed()
{
    if(index == 2)
    {
        feedbackLabel->setString("正确");
    }
    else
    {
        feedbackLabel->setString("错误");
    }
    isInQuestionStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonAnswer3Pressed()
{
    if(index == 3)
    {
        feedbackLabel->setString("正确");
    }
    else
    {
        feedbackLabel->setString("错误");
    }
    isInQuestionStage = false;
    updateUI();
}

void SanGuoXiaoXueTang::onButtonAnswer4Pressed()
{
    if(index == 4)
    {
        feedbackLabel->setString("正确");
    }
    else
    {
        feedbackLabel->setString("错误");
    }
    isInQuestionStage = false;
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
    else
    {
        hideUI();
    }
}