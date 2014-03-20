//
//  ObjectiveManager.cpp
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#include "ObjectiveManager.h"
#include "GameManager.h"

ObjectiveManager::ObjectiveManager()
{
}

ObjectiveManager::~ObjectiveManager()
{
}



CCArray* ObjectiveManager::parseXMLFile(string xml)
{
    CCArray* objectives = CCArray::create();
    objectives->retain();
    
    FileReader* fr = new FileReader(xml);
    
    Objective* objective;
    
    int level = GameManager::getThis()->getLevel();
    stringstream ss;
    ss << "<level" << level << ">";
    
    bool isInLevel = false;
    bool isInObjective = false;
    bool isInReward = false;
    
    for(int i = 0; i < fr->mFileContents.size(); i++)
    {
        // get the line first
        string str = fr->mFileContents.at(i);
        
        // if a line contains "<?xml", then this line is the xml header, ignore it and continue;
        if(str.find("<?xml") != std::string::npos)
        {
            continue;
        }
        
        if(!isInLevel)
        {
            if(str.find(ss.str().c_str()) != std::string::npos)
            {
                isInLevel = true;
            }
        }
        else if(!isInObjective)
        {
            ss.str(std::string());
            ss << "</level" << level << ">";
            if(str.find("<objective>") != std::string::npos)
            {
                objective = Objective::create();
                isInObjective = true;
            }
            else if(str.find(ss.str().c_str()) != std::string::npos)
            {
                isInLevel = false;
            }
        }
        else if(!isInReward)
        {
            unsigned startPos = -1;
            unsigned endPos = -1;
            string content = "";
            int temp;
            
            if(str.find("<id>") != std::string::npos)
            {
                startPos = str.find("<id>");
                endPos = str.find("</id>");
                content = str.substr(startPos + 4, endPos - startPos - 4);
                temp = ::atoi(content.c_str());
                objective->oid = temp;
            }
            else if(str.find("<type>") != std::string::npos)
            {
                startPos = str.find("<type>");
                endPos = str.find("</type>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                if(content.compare("POPULATION") == 0)
                {
                    objective->oType = PopulationGoal;
                }
                else if(content.compare("REPUTATION") == 0)
                {
                    objective->oType = ReputationGoal;
                }
                else if(content.compare("TARGET_BUILDING") == 0)
                {
                    objective->oType = BuildBuilding;
                }
                else if(content.compare("GOLD") == 0)
                {
                    objective->oType = GoldGoal;
                }
                else if(content.compare("FOOD") == 0)
                {
                    objective->oType = FoodGoal;
                }
                else
                {
                    objective->oType = DefaultGoal;
                }
            }
            else if(str.find("<value>") != std::string::npos)
            {
                startPos = str.find("<value>");
                endPos = str.find("</value>");
                content = str.substr(startPos + 7, endPos - startPos - 7);
                temp = ::atoi(content.c_str());
                objective->value = temp;
            }
            else if(str.find("<active>") != std::string::npos)
            {
                startPos = str.find("<active>");
                endPos = str.find("</active>");
                content = str.substr(startPos + 8, endPos - startPos - 8);
                if(content.compare("yes") == 0)
                {
                    objective->active = true;
                }
                else
                {
                    objective->active = false;
                }
            }
            else if(str.find("<pre>") != std::string::npos)
            {
                startPos = str.find("<pre>");
                endPos = str.find("</pre>");
                content = str.substr(startPos + 5, endPos - startPos - 5);
                int preIndex = 0;
                string tempStr = "";
                for(int i = 0; i < content.length(); i++)
                {
                    if(content[i] == ',')
                    {
                        tempStr = content.substr(preIndex, i - preIndex);
                        temp = ::atoi(tempStr.c_str());
                        objective->preConditions.push_back(temp);
                        preIndex = i + 1;
                    }
                }
            }
            else if(str.find("<reward>") != std::string::npos)
            {
                isInReward = true;
            }
            else if(str.find("</objective>") != std::string::npos)
            {
                isInObjective = false;
                objectives->addObject(objective);
            }
        }
        else
        {
            unsigned startPos = -1;
            unsigned endPos = -1;
            string content = "";
            int temp;
            
            if(str.find("<gold>") != std::string::npos)
            {
                startPos = str.find("<gold>");
                endPos = str.find("</gold>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                temp = ::atoi(content.c_str());
                objective->goldReward = temp;
            }
            else if(str.find("<food>") != std::string::npos)
            {
                startPos = str.find("<food>");
                endPos = str.find("</food>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                temp = ::atoi(content.c_str());
                objective->foodReward = temp;
            }
            else if(str.find("<population>") != std::string::npos)
            {
                startPos = str.find("<population>");
                endPos = str.find("</population>");
                content = str.substr(startPos + 12, endPos - startPos - 12);
                temp = ::atoi(content.c_str());
                objective->goldReward = temp;
            }
            else if(str.find("<reputation>") != std::string::npos)
            {
                startPos = str.find("<reputation>");
                endPos = str.find("</reputation>");
                content = str.substr(startPos + 12, endPos - startPos - 12);
                temp = ::atoi(content.c_str());
                objective->goldReward = temp;
            }
            else if(str.find("</reward>") != std::string::npos)
            {
                isInReward = false;
            }
        }
    }
    
    return objectives;
}