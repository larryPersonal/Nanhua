//
//  Question.h
//  PopulationTest
//
//  Created by admin on 6/8/14.
//
//

#ifndef PopulationTest_Question_h
#define PopulationTest_Question_h

#include "cocos2d.h"

using namespace cocos2d;

class Question : public CCObject
{
private:
    std::string questionStr;
    std::vector<std::string> answerStrs;
    
public:
    int answerIndex;
    
public:
    Question();
    ~Question();
    
    static Question* create();
    
    void setQuestionString(std::string);
    std::string getQuestionString();
    
    void addAnswerString(std::string);
    std::vector<std::string> getAnswers();
};

#endif
