//
//  TutorialReader.cpp
//  PopulationTest
//
//  Created by admin on 10/4/14.
//
//

#include "TutorialReader.h"
#include "GlobalHelper.h"

TutorialReader::TutorialReader(){
    slidesList = CCArray::create();
    slidesList->retain();
    curSlide = 0;
}

TutorialReader::~TutorialReader(){
    delete fr;
}


void TutorialReader::addSlide(TutorialSlide* slide)
{
    slidesList->addObject(slide);
}

CCObject* TutorialReader::getSlide(int index){
    return slidesList->objectAtIndex(index);
}

void TutorialReader::releaseSlides()
{
    slidesList->removeAllObjects();
}

void TutorialReader::parseXMLFile(string xml)
{
    slidesList->removeAllObjects();
    slidesList->release();
    slidesList = CCArray::create();
    slidesList->retain();
    fr = new FileReader(xml);
    
    bool inTutorial = false;
    bool inSlide = false;
    
    bool inLock = false;
    bool inUnlock = false;
    
    TutorialSlide* slide;
    
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
        
        if(!inTutorial)
        {
            if(str.find("<tutorial>") != std::string::npos)
            {
                slidesList->removeAllObjects();
                inTutorial = true;
            }
        }
        else if(!inSlide)
        {
            if(str.find("<slide>") != std::string::npos)
            {
                slide = new TutorialSlide();
                inSlide = true;
            }
            else if(str.find("</tutorial>") != std::string::npos)
            {
                inTutorial = false;
            }
        }
        else if(!inLock && !inUnlock)
        {
            int start_pos = 0;
            int end_pos = 0;
            string content = "";
            vector<string> temps;
            if(str.find("<speech>") != std::string::npos)
            {
                start_pos = str.find("<speech>");
                end_pos = str.find("</speech>");
                content = str.substr(start_pos + 8, end_pos - start_pos - 8);
                slide->speech = content;
            }
            else if(str.find("<cameraMove>") != std::string::npos)
            {
                start_pos = str.find("<cameraMove>");
                end_pos = str.find("</cameraMove>");
                content = str.substr(start_pos + 12, end_pos - start_pos - 12);
                temps = GlobalHelper::split(content, ',');
                stringstream ss1;
                stringstream ss2;
                ss1 << temps.at(0);
                ss2 << temps.at(1);
                slide->cameraMove = true;
                slide->target = ccp(::atof(ss1.str().c_str()), ::atof(ss2.str().c_str()));
            }
            else if(str.find("<move>") != std::string::npos)
            {
                start_pos = str.find("<move>");
                end_pos = str.find("</move>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                temps = GlobalHelper::split(content, ',');
                stringstream ss1;
                stringstream ss2;
                ss1 << temps.at(0);
                ss2 << temps.at(1);
                slide->dragonMove = true;
                slide->moveVector = ccp(::atof(ss1.str().c_str()), ::atof(ss2.str().c_str()));
            }
            else if(str.find("<highlight>") != std::string::npos)
            {
                start_pos = str.find("<highlight>");
                end_pos = str.find("</highlight>");
                content = str.substr(start_pos + 11, end_pos - start_pos - 11);
                slide->highlight = true;
                slide->highlightBuilding = content;
            }
            else if(str.find("<deHighlight>") != std::string::npos)
            {
                slide->deHighlight = true;
                start_pos = str.find("<deHighlight>");
                end_pos = str.find("</deHighlight>");
                content = str.substr(start_pos + 13, end_pos - start_pos - 13);
                slide->deHighlightBuilding = content;
            }
            else if(str.find("<fade_in>") != std::string::npos)
            {
                start_pos = str.find("<fade_in>");
                end_pos = str.find("</fade_in>");
                content = str.substr(start_pos + 9, end_pos - start_pos - 9);
                slide->fade_in = true;
                slide->target_opacity = ::atof(content.c_str());
            }
            else if(str.find("<fade_out>") != std::string::npos)
            {
                start_pos = str.find("<fade_out>");
                end_pos = str.find("</fade_out>");
                content = str.substr(start_pos + 10, end_pos - start_pos - 10);
                slide->fade_out = true;
                slide->target_opacity = ::atof(content.c_str());
            }
            else if(str.find("<clickToNext>") != std::string::npos)
            {
                slide->clickToNext = true;
            }
            else if(str.find("<offY>") != std::string::npos)
            {
                start_pos = str.find("<offY>");
                end_pos = str.find("</offY>");
                content = str.substr(start_pos + 6, end_pos - start_pos - 6);
                slide->offY = ::atof(content.c_str());
            }
            else if(str.find("<addVillager>") != std::string::npos)
            {
                start_pos = str.find("<addVillager>");
                end_pos = str.find("</addVillager>");
                content = str.substr(start_pos + 13, end_pos - start_pos - 13);
                slide->addVillager = ::atoi(content.c_str());
            }
            else if(str.find("<hide>") != std::string::npos)
            {
                slide->hide = true;
            }
            else if(str.find("<show>") != std::string::npos)
            {
                slide->show = true;
            }
            else if(str.find("<triggerDropToken>") != std::string::npos)
            {
                slide->triggerDropToken = true;
            }
            else if(str.find("<unlockAll>") != std::string::npos)
            {
                slide->unlockAll = true;
            }
            else if(str.find("<hideBubble>") != std::string::npos)
            {
                slide->hideBubble = true;
            }
            else if(str.find("<showArrow>") != std::string::npos)
            {
                slide->showArrow = true;
            }
            else if(str.find("<hideArrow>") != std::string::npos)
            {
                slide->hideArrow = true;
            }
            else if(str.find("<arrowX>") != std::string::npos)
            {
                start_pos = str.find("<arrowX>");
                end_pos = str.find("</arrowX>");
                content = str.substr(start_pos + 8, end_pos - start_pos - 8);
                slide->arrowX = ::atof(content.c_str());
            }
            else if(str.find("<arrowY>") != std::string::npos)
            {
                start_pos = str.find("<arrowY>");
                end_pos = str.find("</arrowY>");
                content = str.substr(start_pos + 8, end_pos - start_pos - 8);
                slide->arrowY = ::atof(content.c_str());
            }
            else if(str.find("<arrowAngle>") != std::string::npos)
            {
                start_pos = str.find("<arrowAngle>");
                end_pos = str.find("</arrowAngle>");
                content = str.substr(start_pos + 12, end_pos - start_pos - 12);
                slide->arrowAngle = ::atof(content.c_str());
            }
            else if(str.find("<arrowScale>") != std::string::npos)
            {
                start_pos = str.find("<arrowScale>");
                end_pos = str.find("</arrowScale>");
                content = str.substr(start_pos + 12, end_pos - start_pos - 12);
                slide->arrowScale = ::atof(content.c_str());
            }
            else if(str.find("<lock>") != std::string::npos)
            {
                inLock = true;
            }
            else if(str.find("<unlock>") != std::string::npos)
            {
                inUnlock = true;
            }
            else if(str.find("</slide>") != std::string::npos)
            {
                slidesList->addObject(slide);
                inSlide = false;
            }
        }
        else if(inLock)
        {
            if(str.find("<map>") != std::string::npos)
            {
                slide->commands.push_back("lockMap:1");
            }
            else if(str.find("<villager>") != std::string::npos)
            {
                slide->commands.push_back("lockVillager:1");
            }
            else if(str.find("<active>") != std::string::npos)
            {
                slide->commands.push_back("active:1");
            }
            else if(str.find("<buildButton>") != std::string::npos)
            {
                slide->commands.push_back("lockBuildButton:1");
            }
            else if(str.find("<objectiveButton>") != std::string::npos)
            {
                slide->commands.push_back("lockObjectiveButton:1");
            }
            else if(str.find("<systemButton>") != std::string::npos)
            {
                slide->commands.push_back("lockSystemButton:1");
            }
            else if(str.find("<timeGroup>") != std::string::npos)
            {
                slide->commands.push_back("lockTimeGroup:1");
            }
            else if(str.find("<scroll>") != std::string::npos)
            {
                slide->commands.push_back("lockScroll:1");
            }
            else if(str.find("<buildScroll>") != std::string::npos)
            {
                slide->commands.push_back("lockBuildScroll:1");
            }
            else if(str.find("<buildingButton>") != std::string::npos)
            {
                slide->commands.push_back("lockBuildingButton:1");
            }
            else if(str.find("<popup>") != std::string::npos)
            {
                slide->commands.push_back("lockPopup:1");
            }
            else if(str.find("<buttonOk>") != std::string::npos)
            {
                slide->commands.push_back("lockButtonOk:1");
            }
            else if(str.find("<buttonCancel>") != std::string::npos)
            {
                slide->commands.push_back("lockButtonCancel:1");
            }
            else if(str.find("<buttonClose>") != std::string::npos)
            {
                slide->commands.push_back("lockButtonClose:1");
            }
            else if(str.find("<manpowerSelect>") != std::string::npos)
            {
                slide->commands.push_back("lockManpowerSelect:1");
            }
            else if(str.find("<teachBuildButton>") != std::string::npos)
            {
                slide->commands.push_back("teachBuildButton:1");
            }
            else if(str.find("<teachBuildHouse>") != std::string::npos)
            {
                slide->commands.push_back("teachBuildHouse:1");
            }
            else if(str.find("<teachFarming>") != std::string::npos)
            {
                slide->commands.push_back("teachFarming:1");
            }
            else if(str.find("<teachBuildRoad>") != std::string::npos)
            {
                slide->commands.push_back("teachBuildRoad:1");
            }
            else if(str.find("<pause>") != std::string::npos)
            {
                slide->commands.push_back("pause:1");
            }
            else if(str.find("<spriteInfo>") != std::string::npos)
            {
                slide->commands.push_back("spriteInfo:1");
            }
            else if(str.find("<buildingInfo>") != std::string::npos)
            {
                slide->commands.push_back("buildingInfo:1");
            }
            else if(str.find("<goldLabel>") != std::string::npos)
            {
                slide->commands.push_back("goldLabel:1");
            }
            else if(str.find("<foodLabel>") != std::string::npos)
            {
                slide->commands.push_back("foodLabel:1");
            }
            else if(str.find("<populationLabel>") != std::string::npos)
            {
                slide->commands.push_back("populationLabel:1");
            }
            else if(str.find("</lock>") != std::string::npos)
            {
                inLock = false;
            }
        }
        else
        {
            if(str.find("<map>") != std::string::npos)
            {
                slide->commands.push_back("lockMap:0");
            }
            else if(str.find("<villager>") != std::string::npos)
            {
                slide->commands.push_back("lockVillager:0");
            }
            else if(str.find("<active>") != std::string::npos)
            {
                slide->commands.push_back("active:0");
            }
            else if(str.find("<buildButton>") != std::string::npos)
            {
                slide->commands.push_back("lockBuildButton:0");
            }
            else if(str.find("<objectiveButton>") != std::string::npos)
            {
                slide->commands.push_back("lockObjectiveButton:0");
            }
            else if(str.find("<systemButton>") != std::string::npos)
            {
                slide->commands.push_back("lockSystemButton:0");
            }
            else if(str.find("<timeGroup>") != std::string::npos)
            {
                slide->commands.push_back("lockTimeGroup:0");
            }
            else if(str.find("<scroll>") != std::string::npos)
            {
                slide->commands.push_back("lockScroll:0");
            }
            else if(str.find("<buildScroll>") != std::string::npos)
            {
                slide->commands.push_back("lockBuildScroll:0");
            }
            else if(str.find("<buildingButton>") != std::string::npos)
            {
                slide->commands.push_back("lockBuildingButton:0");
            }
            else if(str.find("<popup>") != std::string::npos)
            {
                slide->commands.push_back("lockPopup:0");
            }
            else if(str.find("<buttonOk>") != std::string::npos)
            {
                slide->commands.push_back("lockButtonOk:0");
            }
            else if(str.find("<buttonCancel>") != std::string::npos)
            {
                slide->commands.push_back("lockButtonCancel:0");
            }
            else if(str.find("<buttonClose>") != std::string::npos)
            {
                slide->commands.push_back("lockButtonClose:0");
            }
            else if(str.find("<manpowerSelect>") != std::string::npos)
            {
                slide->commands.push_back("lockManpowerSelect:0");
            }
            else if(str.find("<teachBuildButton>") != std::string::npos)
            {
                slide->commands.push_back("teachBuildButton:0");
            }
            else if(str.find("<teachBuildHouse>") != std::string::npos)
            {
                slide->commands.push_back("teachBuildHouse:0");
            }
            else if(str.find("<teachFarming>") != std::string::npos)
            {
                slide->commands.push_back("teachFarming:0");
            }
            else if(str.find("<teachBuildRoad>") != std::string::npos)
            {
                slide->commands.push_back("teachBuildRoad:0");
            }
            else if(str.find("<pause>") != std::string::npos)
            {
                slide->commands.push_back("pause:0");
            }
            else if(str.find("<spriteInfo>") != std::string::npos)
            {
                slide->commands.push_back("spriteInfo:0");
            }
            else if(str.find("<buildingInfo>") != std::string::npos)
            {
                slide->commands.push_back("buildingInfo:1");
            }
            else if(str.find("<goldLabel>") != std::string::npos)
            {
                slide->commands.push_back("goldLabel:0");
            }
            else if(str.find("<foodLabel>") != std::string::npos)
            {
                slide->commands.push_back("foodLabel:0");
            }
            else if(str.find("<populationLabel>") != std::string::npos)
            {
                slide->commands.push_back("populationLabel:0");
            }
            else if(str.find("</unlock>") != std::string::npos)
            {
                inUnlock = false;
            }
        }
    }
}