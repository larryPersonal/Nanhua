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
    
    stringstream ss;
    
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
        
        if(!isInObjective)
        {
            ss.str(std::string());
            if(str.find("<objective>") != std::string::npos)
            {
                objective = Objective::create();
                isInObjective = true;
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
                else if(content.compare("DISPLAY") == 0)
                {
                    objective->oType = DisplayGoal;
                }
                else if(content.compare("RAISE_POPULATION") == 0)
                {
                    objective->oType = RaisePopulationGoal;
                }
                else if(content.compare("ARMY") == 0)
                {
                    objective->oType = ArmyGoal;
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
            else if(str.find("<time>") != std::string::npos)
            {
                startPos = str.find("<time>");
                endPos = str.find("</time>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                temp = ::atof(content.c_str());
                objective->timeLimit = temp;
            }
            else if(str.find("<next>") != std::string::npos)
            {
                startPos = str.find("<next>");
                endPos = str.find("</next>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                temp = ::atoi(content.c_str());
                objective->nid = temp;
            }
            else if(str.find("<title>") != std::string::npos)
            {
                startPos = str.find("<title>");
                endPos = str.find("</title>");
                content = str.substr(startPos + 7, endPos - startPos - 7);
                objective->title = content;
            }
            else if(str.find("<content>") != std::string::npos)
            {
                startPos = str.find("<content>");
                endPos = str.find("</content>");
                content = str.substr(startPos + 9, endPos - startPos - 9);
                objective->content = content;
            }
            else if(str.find("<progress>") != std::string::npos)
            {
                startPos = str.find("<progress>");
                endPos = str.find("</progress>");
                content = str.substr(startPos + 10, endPos - startPos - 10);
                objective->progress = content;
            }
            else if(str.find("<scenario>") != std::string::npos)
            {
                startPos = str.find("<scenario>");
                endPos = str.find("</scenario>");
                content = str.substr(startPos + 10, endPos - startPos - 10);
                objective->scenarioTime = ::atof(content.c_str()) * 60.0f;
                objective->scheduleScenario = true;
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
    
    CCLog("%d", objectives->count());
    return objectives;
}