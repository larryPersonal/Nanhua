//
//  RandomEventFileReader.cpp
//  PopulationTest
//
//  Created by admin on 9/7/14.
//
//

#include "RandomEventFileReader.h"
#include "RandomEvent.h"

RandomEventFileReader::RandomEventFileReader()
{
    randomEventsList = CCArray::create();
    randomEventsList->retain();
    fr = NULL;
}

RandomEventFileReader::~RandomEventFileReader()
{
    if(fr != NULL)
    {
        delete fr;
    }
}

void RandomEventFileReader::release()
{
    randomEventsList->removeAllObjects();
    CC_SAFE_RELEASE(randomEventsList);
}

void RandomEventFileReader::parseXMLFile(string xml)
{
    randomEventsList->removeAllObjects();
    CC_SAFE_RELEASE(randomEventsList);
    randomEventsList = CCArray::create();
    randomEventsList->retain();
    fr = new FileReader(xml);
    
    bool isInProject = false;
    bool isInEvent = false;
    bool isInEffects = false;
    
    RandomEvent* randomEvent;
    
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
                randomEventsList->removeAllObjects();
                isInProject = true;
            }
        }
        else if(!isInEvent)
        {
            if(str.find("<evt>") != std::string::npos)
            {
                randomEvent = RandomEvent::create();
                isInEvent = true;
            }
            else if(str.find("</project>") != std::string::npos)
            {
                isInProject = false;
            }
        }
        else if(!isInEffects)
        {
            int startPos = -1;
            int endPos = -1;
            string content = "";
            
            if(str.find("<id>") != std::string::npos && str.find("</id>") != std::string::npos)
            {
                startPos = str.find("<id>");
                endPos = str.find("</id>");
                content = str.substr(startPos + 4, endPos - startPos - 4);
                int temp = ::atoi(content.c_str());
                randomEvent->event_id = temp;
            }
            else if(str.find("<image>") != std::string::npos && str.find("</image>") != std::string::npos)
            {
                startPos = str.find("<image>");
                endPos = str.find("</image>");
                content = str.substr(startPos + 7, endPos - startPos - 7);
                randomEvent->event_image = content.c_str();
            }
            else if(str.find("<name>") != std::string::npos && str.find("</name>") != std::string::npos)
            {
                startPos = str.find("<name>");
                endPos = str.find("</name>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                randomEvent->event_name = content.c_str();
            }
            else if(str.find("<description>") != std::string::npos && str.find("</description>") != std::string::npos)
            {
                startPos = str.find("<description>");
                endPos = str.find("</description>");
                content = str.substr(startPos + 13, endPos - startPos - 13);
                randomEvent->event_description = content.c_str();
            }
            else if(str.find("<percentage>") != std::string::npos && str.find("</percentage>") != std::string::npos)
            {
                startPos = str.find("<percentage>");
                endPos = str.find("</percentage>");
                content = str.substr(startPos + 12, endPos - startPos - 12);
                randomEvent->percentage = ::atoi(content.c_str());
            }
            else if(str.find("<effects>") != std::string::npos)
            {
                isInEffects = true;
            }
            else if(str.find("</evt>") != std::string::npos)
            {
                randomEventsList->addObject(randomEvent);
                isInEvent = false;
            }
        }
        else
        {
            int startPos = -1;
            int endPos = -1;
            string content = "";
            
            if(str.find("<ef>") != std::string::npos && str.find("</ef>") != std::string::npos)
            {
                startPos = str.find("<ef>");
                endPos = str.find("</ef>");
                content = str.substr(startPos + 4, endPos - startPos - 4);
                randomEvent->event_effects.push_back(content);
            }
            else if(str.find("</effects>") != std::string::npos)
            {
                isInEffects = false;
            }
        }
    }
    
}