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