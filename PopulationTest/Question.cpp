//
//  Question.cpp
//  PopulationTest
//
//  Created by admin on 6/8/14.
//
//

#include "Question.h"

Question* Question::create(){
    Question* pRet = new Question();
    if(pRet)
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

Question::Question()
{
    questionStr = "";
    answerIndex = 0;
    
    introStr = "";
    isIntro = false;
    
    twoChoice = false;
}

Question::~Question()
{
    
}

void Question::setQuestionString(std::string qStr)
{
    questionStr = qStr;
}

std::string Question::getQuestionString()
{
    return questionStr;
}

void Question::addAnswerString(std::string aStr)
{
    answerStrs.push_back(aStr);
}

std::vector<std::string> Question::getAnswers()
{
    return answerStrs;
}

void Question::setIntroString(std::string iStr)
{
    introStr = iStr;
}

std::string Question::getIntroString()
{
    return introStr;
}

void Question::setIsIntro(bool flag)
{
    isIntro = flag;
}

bool Question::getIsIntro()
{
    return isIntro;
}

void Question::setTwoChoice(bool flag)
{
    twoChoice = flag;
}

bool Question::getTwoChoice()
{
    return twoChoice;
}