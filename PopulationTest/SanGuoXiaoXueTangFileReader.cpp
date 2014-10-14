//
//  SanGuoXiaoXueTangFileReader.cpp
//  PopulationTest
//
//  Created by admin on 6/8/14.
//
//

#include "SanGuoXiaoXueTangFileReader.h"
#include "Question.h"

SanGuoXiaoXueTangFileReader::SanGuoXiaoXueTangFileReader()
{
    questionList = CCArray::create();
    questionList->retain();
    fr = NULL;
}

SanGuoXiaoXueTangFileReader::~SanGuoXiaoXueTangFileReader()
{
    if(fr != NULL)
    {
        delete fr;
    }
}

void SanGuoXiaoXueTangFileReader::release()
{
    questionList->removeAllObjects();
    CC_SAFE_RELEASE(questionList);
}

void SanGuoXiaoXueTangFileReader::parseXMLFile(string xml)
{
    questionList->removeAllObjects();
    CC_SAFE_RELEASE(questionList);
    questionList = CCArray::create();
    questionList->retain();
    fr = new FileReader(xml);
    
    bool isInProject = false;
    bool isInQuestion = false;
    
    Question* question;
    
    for(int i = 0; i < fr->mFileContents.size(); i++)
    {
        // get the line first
        string str = fr->mFileContents.at(i);
        
        // if a line contains "<?xml", then this line is the xml header, ignore it and continue;
        if(str.find("<?xml") != std::string::npos)
        {
            continue;
        }
        
        if(!isInProject)
        {
            if(str.find("<project>") != std::string::npos)
            {
                questionList->removeAllObjects();
                isInProject = true;
            }
        }
        else if(!isInQuestion)
        {
            if(str.find("<question>") != std::string::npos)
            {
                question = Question::create();
                isInQuestion = true;
            }
            else if(str.find("</project>") != std::string::npos)
            {
                isInProject = false;
            }
        }
        else
        {
            int startPos = -1;
            int endPos = -1;
            string content = "";
            
            if(str.find("<text>") != std::string::npos && str.find("</text>") != std::string::npos)
            {
                startPos = str.find("<text>");
                endPos = str.find("</text>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                question->setQuestionString(content);
            }
            else if(str.find("<intro>") != std::string::npos && str.find("</intro>") != std::string::npos)
            {
                startPos = str.find("<intro>");
                endPos = str.find("</intro>");
                content = str.substr(startPos + 7, endPos - startPos - 7);
                question->setIntroString(content);
                question->setIsIntro(true);
            }
            else if(str.find("<answer>") != std::string::npos && str.find("</answer>") != std::string::npos)
            {
                startPos = str.find("<answer>");
                endPos = str.find("</answer>");
                content = str.substr(startPos + 8, endPos - startPos - 8);
                question->addAnswerString(content);
            }
            else if(str.find("<twoChoice>") != std::string::npos && str.find("</twoChoice>") != std::string::npos)
            {
                question->setTwoChoice(true);
            }
            else if(str.find("<index>") != std::string::npos && str.find("</index>") != std::string::npos)
            {
                startPos = str.find("<index>");
                endPos = str.find("</index>");
                content = str.substr(startPos + 7, endPos - startPos - 7);
                int idx = ::atoi(content.c_str());
                question->answerIndex = idx;
            }
            else if(str.find("</question>") != std::string::npos)
            {
                questionList->addObject(question);
                isInQuestion = false;
            }
        }
    }

}