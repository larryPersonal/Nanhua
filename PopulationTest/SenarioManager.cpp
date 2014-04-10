//
//  SenarioManager.cpp
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#include "SenarioManager.h"
#include "Slide.h"
#include "Element.h"
#include "GlobalHelper.h"

SenarioManager::SenarioManager(){
    slidesList = CCArray::create();
    slidesList->retain();
    curSlide = 0;
}

SenarioManager::~SenarioManager(){
    delete fr;
}


void SenarioManager::addSlide(Slide* slide)
{
    slidesList->addObject(slide);
}

CCObject* SenarioManager::getSlide(int index){
    return slidesList->objectAtIndex(index);
}

void SenarioManager::releaseSlides()
{
    slidesList->removeAllObjects();
}

void SenarioManager::parseXMLFile(string xml)
{
    slidesList->removeAllObjects();
    slidesList->release();
    slidesList = CCArray::create();
    slidesList->retain();
    fr = new FileReader(xml);
    
    bool inProject = false;
    bool inSlide = false;
    
    bool inSprite = false;
    bool inDialogue = false;
    bool inOption = false;
    bool inOutcome = false;
    
    Slide* slide;
    Element* element;
    
    curSlide = 0;
    
    for(int i = 0; i < fr->mFileContents.size(); i++)
    {
        // get the line first
        string str = fr->mFileContents.at(i);
        
        // if a line contains "<?xml", then this line is the xml header, ignore it and continue;
        if(str.find("<?xml") != std::string::npos)
        {
            continue;
        }
        
        if(!inProject)
        {
            if(str.find("<project>") != std::string::npos)
            {
                slidesList->removeAllObjects();
                inProject = true;
            }
        }
        else if(!inSlide)
        {
            if(str.find("<slide>") != std::string::npos)
            {
                slide = new Slide();
                inSlide = true;
            }
            else if(str.find("</project>") != std::string::npos)
            {
                inProject = false;
            }
        }
        else if(!inSprite && !inDialogue && !inOption)
        {
            if(str.find("<sprite>") != std::string::npos)
            {
                element = new Element();
                element->updateSpriteType();
                inSprite = true;
            }
            else if(str.find("<dialogue>") != std::string::npos)
            {
                element = new Element();
                element->updateDialogueType();
                inDialogue = true;
            }
            else if(str.find("<option>") != std::string::npos)
            {
                element = new Element();
                element->updateOptionType();
                inOption = true;
            }
            else if(str.find("</slide>") != std::string::npos)
            {
                slidesList->addObject(slide);
                inSlide = false;
            }
        }
        else if(inOutcome)
        {
            unsigned start_pos = -1;
            unsigned end_pos = -1;
            string content = "";
            float temp;
            
            if(str.find("<bandit>") != std::string::npos)
            {
                start_pos = str.find("<bandit>");
                end_pos = str.find("</bandit>");
                content = str.substr(start_pos + 8, end_pos - start_pos - 8);
                temp = ::atoi(content.c_str());
                element->banditsModifier = temp;
            }
            else if(str.find("<refugee>") != std::string::npos)
            {
                start_pos = str.find("<refugee>");
                end_pos = str.find("</refugee>");
                content = str.substr(start_pos + 9, end_pos - start_pos - 9);
                temp = ::atoi(content.c_str());
                element->refugeeModifier = temp;
            }
            else if(str.find("<gold>") != std::string::npos)
            {
                start_pos = str.find("<gold>");
                end_pos = str.find("</gold>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                temp = ::atoi(content.c_str());
                element->goldModifier = temp;
            }
            else if(str.find("<food>") != std::string::npos)
            {
                start_pos = str.find("<food>");
                end_pos = str.find("</food>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                temp = ::atoi(content.c_str());
                element->foodModifier = temp;
            }
            else if(str.find("<population>") != std::string::npos)
            {
                start_pos = str.find("<population>");
                end_pos = str.find("</population>");
                content = str.substr(start_pos + 12, end_pos - start_pos - 12);
                temp = ::atoi(content.c_str());
                element->populationModifier = temp;
            }
            else if(str.find("</outcome>") != std::string::npos)
            {
                inOutcome = false;
            }
        }
        else
        {
            unsigned start_pos = -1;
            unsigned end_pos = -1;
            string content = "";
            float temp;
            
            if(str.find("<width>") != std::string::npos)
            {
                start_pos = str.find("<width>");
                end_pos = str.find("</width>");
                content = str.substr(start_pos + 7, end_pos - start_pos - 7);
                temp = ::atof(content.c_str());
                element->width = temp;
            }
            else if(str.find("<left>") != std::string::npos)
            {
                start_pos = str.find("<left>");
                end_pos = str.find("</left>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                temp = ::atof(content.c_str());
                element->left = temp;
            }
            else if(str.find("<top>") != std::string::npos)
            {
                start_pos = str.find("<top>");
                end_pos = str.find("</top>");
                content = str.substr(start_pos + 5, end_pos - start_pos - 5);
                temp = ::atof(content.c_str());
                element->top = temp;
            }
            else if(str.find("<src>") != std::string::npos)
            {
                start_pos = str.find("<src>");
                end_pos = str.find("</src>");
                content = str.substr(start_pos + 5, end_pos - start_pos - 5);
                element->src = content;
            }
            else if(str.find("<srcp>") != std::string::npos)
            {
                start_pos = str.find("<srcp>");
                end_pos = str.find("</srcp>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                element->srcp = content;
            }
            else if(str.find("<dir>") != std::string::npos)
            {
                start_pos = str.find("<dir>");
                end_pos = str.find("</dir>");
                content = str.substr(start_pos + 5, end_pos - start_pos - 5);
                element->dir = content;
            }
            else if(str.find("<name>") != std::string::npos)
            {
                start_pos = str.find("<name>");
                end_pos = str.find("</name>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                element->name = content;
            }
            else if(str.find("<font>") != std::string::npos)
            {
                start_pos = str.find("<font>");
                end_pos = str.find("</font>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                element->font = content;
            }
            else if(str.find("<fontSize>") != std::string::npos)
            {
                start_pos = str.find("<fontSize>");
                end_pos = str.find("</fontSize>");
                content = str.substr(start_pos + 10, end_pos - start_pos - 10);
                int tmp = ::atoi(content.c_str());
                element->fontSize = tmp;
            }
            else if(str.find("<text>") != std::string::npos)
            {
                start_pos = str.find("<text>");
                end_pos = str.find("</text>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                element->text = content;
            }
            else if(str.find("<file>") != std::string::npos)
            {
                start_pos = str.find("<file>");
                end_pos = str.find("</file>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                element->nextFile = content;
            }
            else if(str.find("<background>") != std::string::npos)
            {
                start_pos = str.find("<background>");
                end_pos = str.find("</background>");
                content = str.substr(start_pos + 12, end_pos - start_pos - 12);
                if(content.compare("yes") == 0)
                {
                    element->isBackground = true;
                }
                else
                {
                    element->isBackground = false;
                }
            }
            else if(str.find("<fade_in>") != std::string::npos)
            {
                start_pos = str.find("<fade_in>");
                end_pos = str.find("</fade_in>");
                content = str.substr(start_pos + 9, end_pos - start_pos - 9);
                if(content.compare("yes") == 0)
                {
                    element->fadeIn = true;
                } else {
                    element->fadeIn = false;
                }
            }
            else if(str.find("<fade_out>") != std::string::npos)
            {
                start_pos = str.find("<fade_out>");
                end_pos = str.find("</fade_out>");
                content = str.substr(start_pos + 10, end_pos - start_pos - 10);
                if(content.compare("yes") == 0)
                {
                    element->fadeOut = true;
                } else {
                    element->fadeOut = false;
                }
            }
            else if(str.find("<outcome>") != std::string::npos)
            {
                element->outcome = true;
                inOutcome = true;
            }
            else if(str.find("</dialogue>") != std::string::npos)
            {
                slide->addElement(element);
                inDialogue = false;
            }
            else if(str.find("</sprite>") != std::string::npos)
            {
                slide->addElement(element);
                inSprite = false;
            }
            else if(str.find("</option>") != std::string::npos)
            {
                slide->addElement(element);
                inOption = false;
            }
        }
    }
}