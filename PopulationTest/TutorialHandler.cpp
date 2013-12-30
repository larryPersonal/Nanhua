	//
//  TutorialHandler.cpp
//  PopulationTest
//
//  Created by Serious Games on 4/6/13.
//
//

#include "TutorialHandler.h"
#include "GameHUD.h"
#include "PopupMenu.h"
#include "AlertBox.h"
#include "Tutorial.h"

/*TutorialPiece*/
TutorialPiece::TutorialPiece()
{
    isComplete = false;
    correctActionReported = false;
}

void TutorialPiece::ReportAction(std::string &action, std::string &targetType, std::string &targetName)
{
    if (action.compare(updateWaitAction) == 0 &&
        targetType.compare(updateTargetType) == 0)
    {
        
        if (action.compare("count")==0)
        {
            int tname = atoi(targetName.c_str());
            int tc = atoi(updateTargetContent.c_str());
            
            if (tname >= tc) correctActionReported = true;
            
        }
        else
        {
            if (targetName.compare(updateTargetContent) == 0)
                correctActionReported = true;
        }
    }
}

void TutorialPiece::PerformAction(std::string &action, std::string &targetType, std::string &targetName)
{
    //I don't support constructing buildings. There's no where to put a coordinate.
    
    if (action.compare("message") == 0)
    {
        if (targetType.compare("hint") == 0)
            GameHUD::getThis()->showHint(targetName);
        else
            TutorialHandler::getThis()->ShowMessage(targetName);
        
        return;

    }
    
    if (action.compare("system") == 0)
    {
        if (targetType.compare("spawning") == 0)
        {
            if (targetName.compare("off") == 0)
            {
                GameScene::getThis()->policyHandler->pauseSpawn();
            }
            else
            {
                GameScene::getThis()->policyHandler->startSpawn();
            }
            
        }
        
        if (targetType.compare("policymenu") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(1, true);
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(1, false);
            }
        }
        
        if (targetType.compare("researchmenu") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(0, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(0, false);
                
            }
        }
        
        if (targetType.compare("birthratepolicy") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(2, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(2, false);
                
            }
        }
        
        if (targetType.compare("taxratepolicy") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(6, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(6, false);
                
            }
        }
        
        if (targetType.compare("culturalpolicy") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(5, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(5, false);
                
            }
        }
        
        if (targetType.compare("importaliencitizen") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(3, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(3, false);
                
            }
        }
        
        
        if (targetType.compare("importalienresearcher") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(4, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(4, false);
                
            }
        }
        
        if (targetType.compare("alienpoplimit") == 0)
        {
            if (targetName.compare("enable") == 0)
            {
                GameManager::getThis()->fliplockMenuItem(7, true);
                
            }
            else
            {
                GameManager::getThis()->fliplockMenuItem(7, false);
                
            }
            
        }
        return;
    }
    
    
    if (action.compare("makeresearchable")==0)
    {
        Building* b = NULL;
        BuildingCategory bc = HOUSING;
        if (targetType.compare("HOUSING") == 0)
            bc = HOUSING;
    
        if (targetType.compare("AMENITY") == 0)
            bc = AMENITY;
        
        if (targetType.compare("COMMERCE") == 0)
            bc = COMMERCE;
        
        if (targetType.compare("MILITARY") == 0)
            bc = MILITARY;
        
        if (targetType.compare("EDUCATION") == 0)
            bc = EDUCATION;
        
        if (targetType.compare("SOCIAL") == 0)
            bc = SOCIAL;
        
        if (targetName.length() > 0)
        {
            b = GameScene::getThis()->buildingHandler->getBuilding(targetName);
        
        }
        else
        {
            
            b = GameScene::getThis()->buildingHandler->getFirstBuildingOfCategory(bc);
        }
    
        
        if (b!= NULL) b->unlockToResearch();
        
        
        return;
    }
    
    if (action.compare("makebuildable") == 0)
    {
        Building* b = NULL;
        BuildingCategory bc = HOUSING;
        if (targetType.compare("HOUSING") == 0)
            bc = HOUSING;
        
        if (targetType.compare("AMENITY") == 0)
            bc = AMENITY;
        
        if (targetType.compare("COMMERCE") == 0)
            bc = COMMERCE;
        
        if (targetType.compare("MILITARY") == 0)
            bc = MILITARY;
        
        if (targetType.compare("EDUCATION") == 0)
            bc = EDUCATION;
        
        if (targetType.compare("SOCIAL") == 0)
            bc = SOCIAL;
        
        
        if (targetName.length() > 0)
        {
            b = GameScene::getThis()->buildingHandler->getBuilding(targetName);
            
        }
        else
        {
            
            b = GameScene::getThis()->buildingHandler->getFirstBuildingOfCategory(bc);
        }
        
        if (b!= NULL) b->unlockToBuild();
       // GameManager::getThis()->UpdateUnlocks();
        
        return;
    }
    
    
    
    if (action.compare("spawnimmediate") == 0)
    {
        /*
        SpriteType st;
        int gender = rand() % 2;
        if (targetType.compare("alien") == 0)
        {
            st = M_ALIEN_CITIZEN;
            if (targetName.compare("researcher") == 0)
            {
                if (gender == 1)
                {
                    st = M_ALIEN_RESEARCHER;
                }
                else
                {
                    st = F_ALIEN_RESEARCHER;
                }
            }
            else if (targetName.compare("citizen") == 0)
            {
                if (gender == 1)
                {
                    st = M_ALIEN_CITIZEN;
                }
                else
                {
                    st = F_ALIEN_CITIZEN;
                }
            }
            GameScene::getThis()->policyHandler->SpawnImmediately(st);
            
            return;
        }

        if (targetType.compare("mayan") == 0)
        {
            st = M_MAYAN_CITIZEN;
            if (targetName.compare("warrior") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_WARRIOR;
                }
                else
                {
                    st = F_MAYAN_WARRIOR;
                }
            }
            else if (targetName.compare("citizen") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_CITIZEN;
                }
                else
                {
                    st = F_MAYAN_CITIZEN;
                }
            }
            else if (targetName.compare("farmer") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_FARMER;
                }
                else
                {
                    st = F_MAYAN_FARMER;
                }
            }
            else if (targetName.compare("merchant") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_MERCHANT;
                }
                else
                {
                    st = F_MAYAN_MERCHANT;
                }
            }
            GameScene::getThis()->policyHandler->SpawnImmediately(st);
            return;
         
        }
     
        */
        
    }
    
    if (action.compare("spawn") == 0)
    {
        /*
        SpriteType st;
        int gender = rand() % 2;
        if (targetType.compare("alien") == 0)
        {
            st = M_ALIEN_CITIZEN;
            if (targetName.compare("researcher") == 0)
            {
                if (gender == 1)
                {
                    st = M_ALIEN_RESEARCHER;
                }
                else
                {
                    st = F_ALIEN_RESEARCHER;
                }
            }
            else if (targetName.compare("citizen") == 0)
            {
                if (gender == 1)
                {
                    st = M_ALIEN_CITIZEN;
                }
                else
                {
                    st = F_ALIEN_CITIZEN;
                }
            }
            GameScene::getThis()->policyHandler->SetNextAlienSpawn(st);
            
            return;
        }
        
        if (targetType.compare("mayan") == 0)
        {
            st = M_MAYAN_CITIZEN;
            if (targetName.compare("warrior") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_WARRIOR;
                }
                else
                {
                    st = F_MAYAN_WARRIOR;
                }
            }
            else if (targetName.compare("citizen") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_CITIZEN;
                }
                else
                {
                    st = F_MAYAN_CITIZEN;
                }
            }
            else if (targetName.compare("farmer") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_FARMER;
                }
                else
                {
                    st = F_MAYAN_FARMER;
                }
            }
            else if (targetName.compare("merchant") == 0)
            {
                if (gender == 1)
                {
                    st = M_MAYAN_MERCHANT;
                }
                else
                {
                    st = F_MAYAN_MERCHANT;
                }
            }
            GameScene::getThis()->policyHandler->SetNextSpawn(st);
            return;
            
        }
        
        
     */   
    }

    
}


void TutorialPiece::prepare()
{

    PerformAction(prepareAction, prepareTargetType, prepareTargetContent);

}

void TutorialPiece::update()
{
    if (updateWaitAction.length() == 0) correctActionReported = true; //skip this action if there is nothing to wait for.
    
    if(correctActionReported)
        complete();
    
}

void TutorialPiece::complete()
{

    
    PerformAction(completeAction, completeType, completeContent);
    
    isComplete = true;
    
    
}


/*TutorialHandler*/
TutorialHandler* TutorialHandler::SP;

TutorialHandler::TutorialHandler()
{
    SP = this;
    isActive = false;
    currSequenceIndex = 0;
    
    
    bool parsingSuccessful = reader.parse(tutorialOrder,root);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse tutorial\n" << reader.getFormatedErrorMessages() << std::endl;
        
    }
    else
    {
        //std::string encoding = root.get("encoding", "UTF-8" ).asString();
        for (int i = 0; i<root["items"].size(); i++)
        {
            //std::string name = root["items"].operator[](i)["unlock_name"].asString();
            //std::size_t found = name.find("area_");
            //if (found==std::string::npos) continue;
            
            TutorialPiece p;
            p.prepareAction = root["items"].operator[](i)["pre_action"].asString();
            p.prepareTargetType = root["items"].operator[](i)["pre_targettype"].asString();
            p.prepareTargetContent = root["items"].operator[](i)["pre_content"].asString();
            p.updateWaitAction = root["items"].operator[](i)["wait_action"].asString();
            p.updateTargetType = root["items"].operator[](i)["wait_targettype"].asString();
            p.updateTargetContent = root["items"].operator[](i)["wait_content"].asString();
            p.completeAction = root["items"].operator[](i)["post_action"].asString();
            p.completeType = root["items"].operator[](i)["post_targettype"].asString();
            p.completeContent = root["items"].operator[](i)["post_content"].asString();
            
            
            tutorialSequence.push_back(p);
        }
    }
    
    root.clear();
    
    
}

TutorialHandler::~TutorialHandler()
{
    SP= NULL;
    tutorialSequence.clear();
    isActive = false;
}

TutorialHandler* TutorialHandler::getThis()
{
    return SP;
}

void TutorialHandler::ShowMessage(std::string& message)
{
    AlertBox* newAlert = AlertBox::create();
    newAlert->useAsExclusivePopupMenu();
    newAlert->setDisplayText(message);
    newAlert->addButton(0, "Ok");
}

void TutorialHandler::ReportAction(std::string action, std::string type, std::string name)
{
    if (!isActive) return;
    
    tutorialSequence[currSequenceIndex].ReportAction(action,type, name);
    
}

bool TutorialHandler::IsActive()
{
    return isActive;
}

void TutorialHandler::SetActive(bool active)
{
    isActive = active;
    
    if (isActive == false)
    {
        GameManager::getThis()->fliplockMenuItem(0, true);
        GameManager::getThis()->fliplockMenuItem(1, true);
        
        //unravel the tutorial since we aren't playing it
        for (int i = 0; i < tutorialSequence.size(); ++i)
        {
            if (tutorialSequence[i].prepareAction.compare("message") != 0)
                tutorialSequence[i].PerformAction(tutorialSequence[i].prepareAction, tutorialSequence[i].prepareTargetType, tutorialSequence[i].prepareTargetContent);
            if (tutorialSequence[i].completeAction.compare("message") != 0)
                
            tutorialSequence[i].PerformAction(tutorialSequence[i].completeAction, tutorialSequence[i].completeType, tutorialSequence[i].completeContent);

        }
        
    }
}


void TutorialHandler::BeginTutorial()
{
    if (tutorialSequence.size() == 0)
    {
        isActive = false;
        return;
    }
    
    isActive = true;
    
    tutorialSequence[currSequenceIndex].prepare();
    //update() takes over from here on.
    
}

void TutorialHandler::update()
{
    if (!isActive) return;
    
    if (tutorialSequence.size() > 0 && currSequenceIndex < tutorialSequence.size())
    {
        tutorialSequence[currSequenceIndex].update();
        if (tutorialSequence[currSequenceIndex].isComplete)
        {
            currSequenceIndex+= 1;
            if (currSequenceIndex >= tutorialSequence.size())
            {
                //tutorial over!
                tutorialSequence.clear();
             //   currSequenceIndex = 0;
                isActive = false;
                //to kick off the normal unlock logic
                GameManager::getThis()->UpdateUnlocks();
            }
            else
            {
                tutorialSequence[currSequenceIndex].prepare();
            }
        }
    }
}

int TutorialHandler::getSequenceIndex()
{
    return currSequenceIndex;
}

void TutorialHandler::setSequenceIndex(int idx)
{
    if (idx >= tutorialSequence.size())
        currSequenceIndex = tutorialSequence.size() -1;
    else
        currSequenceIndex = idx;
}