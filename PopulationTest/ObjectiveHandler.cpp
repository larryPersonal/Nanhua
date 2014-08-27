//
//  ObjectiveHandler.cpp
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#include "ObjectiveHandler.h"
#include "ObjectiveManager.h"
#include "GameHUD.h"
#include "GlobalHelper.h"
#include "TutorialManager.h"
#include "NotificationPopup.h"

ObjectiveHandler* ObjectiveHandler::SP;

ObjectiveHandler::ObjectiveHandler()
{
    ObjectiveHandler::SP = this;
    
    objectives = CCArray::create();
    objectives->retain();
    
    obj = NULL;
    
    nextID = 0;
    progressNumber = 0;
    
    startPopulation = 0;
    targetPopulation = 0;
}

ObjectiveHandler::~ObjectiveHandler()
{
    ObjectiveHandler::SP = NULL;
    
    objectives->removeAllObjects();
    CC_SAFE_RELEASE(objectives);
}

ObjectiveHandler* ObjectiveHandler::getThis()
{
    return SP;
}

ObjectiveHandler* ObjectiveHandler::create()
{
    ObjectiveHandler* sPtr = new ObjectiveHandler();
    if (sPtr)
    {
        sPtr->autorelease();
        return sPtr;
    }
    else
    {
        CC_SAFE_DELETE(sPtr);
        return NULL;
    }
}

void ObjectiveHandler::loadObjective()
{
    objectives->removeAllObjects();
    CC_SAFE_RELEASE(objectives);
    
    if(GameManager::getThis()->getLevel() == 0)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_tutorial.xml");
    }
    else if(GameManager::getThis()->getLevel() == 1)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_1.xml");
    }
    else if(GameManager::getThis()->getLevel() == 2)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_2.xml");
    }
    else if(GameManager::getThis()->getLevel() == 3)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_3.xml");
    }
    else if(GameManager::getThis()->getLevel() == 4)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_4.xml");
    }
    else if(GameManager::getThis()->getLevel() == 5)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_5.xml");
    }
    else if(GameManager::getThis()->getLevel() == 6)
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_6.xml");
    }
    else
    {
        nextID = 0;
        progressNumber = 0;
        objectives = ObjectiveManager::parseXMLFile("objective_scenario_1.xml");
    }
}

void ObjectiveHandler::playObjective(bool showNotification)
{
    //CCLog("new objective received!");
    //CCLog("number of objectives: %d", objectives->count());
    if(objectives->count() <= 0)
    {
        return;
    }
    
    for(int i = 0; i < GameHUD::getThis()->objectiveDescriptions->count(); i++)
    {
        CCLabelTTF* tempLabel = (CCLabelTTF*) GameHUD::getThis()->objectiveDescriptions->objectAtIndex(i);
        GameHUD::getThis()->objectiveMenu->removeChild(tempLabel, true);
    }
    
    if(nextID == 0)
    {
        obj = (Objective*) objectives->objectAtIndex(0);
    }
    else
    {
        obj = NULL;
        for(int i = 0; i < objectives->count(); i++)
        {
            Objective* temp = (Objective*) objectives->objectAtIndex(i);
            if(temp->oid == nextID)
            {
                obj = temp;
            }
        }
    }
    
    if(obj == NULL)
    {
        if(GameManager::getThis()->getLevel() == 4)
        {
            NotificationPopup::getThis()->showScenario4Congratulations();
        }
        else if(GameManager::getThis()->getLevel() == 5)
        {
            NotificationPopup::getThis()->showScenario5Congratulations();
        }
        return;
    }
    
    stringstream ss;
    stringstream ss1;
    stringstream ss2;
    
    if(obj->oType == GoldGoal)
    {
        ss << "Cumulate Gold";
        ss1 << "Collect gold of " << obj->value << "!";
        ss2 << "(" << GameHUD::getThis()->money << "/" << obj->value << ")";
        progressNumber = GameHUD::getThis()->money;
    }
    else if(obj->oType == FoodGoal)
    {
        ss << "Cumulate Food";
        ss1 << "Collect food of " << obj->value << "!";
        ss2 << "(" << GameHUD::getThis()->foodLabel->getString() << "/" << obj->value << ")";
        progressNumber = ::atoi(GameHUD::getThis()->foodLabel->getString());
    }
    else if(obj->oType == PopulationGoal)
    {
        ss << "Sustain Population";
        ss1 << "Raise your population to " << obj->value << "!";
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        int num = 0;
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            if(gs->villagerClass != V_CLASS_END && gs->villagerClass != V_BANDIT && gs->getHome() != NULL)
            {
                num++;
            }
        }
        ss2 << "(" << num << "/" << obj->value << ")";
        progressNumber = num;
    }
    else if(obj->oType == RaisePopulationGoal)
    {
        ss << "Raise Population";
        ss1 << "Raise your population by " << obj->value << "!";
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        int num = 0;
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            if(gs->villagerClass != V_CLASS_END && gs->villagerClass != V_BANDIT)
            {
                num++;
            }
        }
        startPopulation = num;
        targetPopulation = startPopulation + obj->value;
        int currentPopulation = obj->value - (targetPopulation - startPopulation);
        if(currentPopulation < 0)
        {
            currentPopulation = 0;
        }
        else if(currentPopulation > obj->value)
        {
            currentPopulation = obj->value;
        }
        
        ss2 << "(" << obj->value - (targetPopulation - startPopulation) << "/" << obj->value << ")";
        progressNumber = obj->value - (targetPopulation - startPopulation);
    }
    else if(obj->oType == ReputationGoal)
    {
        ss << "Achieve Reputation";
        ss1 << "Raise your reputation to " << obj->value << "!";
        ss2 << "(" << GameHUD::getThis()->reputation << "/" << obj->value << ")";
        progressNumber = GameHUD::getThis()->reputation;
    }
    else if(obj->oType == BuildBuilding)
    {
        ss << "Build a Building";
        ss1 << "Build a Building: " << obj->strValue << "!";
        ss2 << "(Build: Incomplete)";
        progressNumber = 0;
    }
    else if(obj->oType == DisplayGoal)
    {
        ss << obj->title;
        ss1 << obj->content;
        ss2 << obj->progress;
    }
    else
    {
        ss << "OBJECTIVE";
        ss1 << "There is no objectives currently!";
        ss2 << "";
        progressNumber = -1;
    }
    
    GameHUD::getThis()->objectiveTitle->setString(ss.str().c_str());
    GameHUD::getThis()->objectiveProgress->setString(ss2.str().c_str());
    
    string objectiveDescription = ss1.str();
    vector<string> objectiveDescriptionTokens = GlobalHelper::split(objectiveDescription, ' ');
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorWhite = ccc3(255, 255, 255);
    
    float startX = screenSize.width * 0.11f;
    float startY = screenSize.height - 510;
    float offX = 0;
    float offY = 0;
    float limit = 700;
    
    
    string tempString = "";
    string previousString = "";
    for (int i = 0; i < objectiveDescriptionTokens.size(); i++)
    {
        previousString = tempString;
        string temp = objectiveDescriptionTokens.at(i);
        if (i > 0)
        {
            tempString = tempString + " ";
        }
        tempString = tempString + temp;
        CCLabelTTF* tempLabel = CCLabelTTF::create(tempString.c_str(), "Shojumaru-Regular", 28);
        tempLabel->retain();
        if(startX + tempLabel->boundingBox().size.width > limit)
        {
            CCLabelTTF* theLabel = CCLabelTTF::create(previousString.c_str(), "Shojumaru-Regular", 28);
            theLabel->setAnchorPoint(ccp(0, 1));
            theLabel->setPosition(ccp(startX + offX, startY + offY));
            theLabel->setColor(colorWhite);
            GameHUD::getThis()->objectiveDescriptions->addObject(theLabel);
            GameHUD::getThis()->objectiveMenu->addChild(theLabel);
            tempString = temp;
            offY -= 25;
        }
        CC_SAFE_RELEASE(tempLabel);
    }
    
    CCLabelTTF* tempLabel = CCLabelTTF::create(tempString.c_str(), "Shojumaru-Regular", 28);
    tempLabel->setAnchorPoint(ccp(0, 1));
    tempLabel->setPosition(ccp(startX + offX, startY + offY));
    tempLabel->setColor(colorWhite);
    GameHUD::getThis()->objectiveDescriptions->addObject(tempLabel);
    GameHUD::getThis()->objectiveMenu->addChild(tempLabel);
    
    nextID = obj->nid;
    
    if(obj->timeLimit > 0)
    {
        ss.str(std::string());
        if(obj->timeLimit < 10)
        {
            ss << "0";
        }
        ss << obj->timeLimit << ":" << "00";
        
        GameHUD::getThis()->objectiveTime->setString(ss.str().c_str());
        GameHUD::getThis()->objectiveTime->setVisible(true);
        GameHUD::getThis()->targetTime = obj->timeLimit * 60.0f;
        GameHUD::getThis()->currentTime = 0;
        GameHUD::getThis()->hasTimer = true;
    }
    
    stringstream sss;
    sss << "You receive a new objective!";
    GameHUD::getThis()->addNewNotification(sss.str());
    
    if(showNotification)
    {
        GameHUD::getThis()->scheduleShowNewObjectiveNotification();
    }
    
    if(obj->scheduleScenario)
    {
        TutorialManager::getThis()->scheduleForScenario(obj->scenarioTime);
    }
    
    if(obj->finalObjective)
    {
        GameHUD::getThis()->finalObjective = true;
    }
}

void ObjectiveHandler::update(float dt)
{
    if(obj == NULL)
    {
        return;
    }
    
    stringstream ss;
    
    bool hasUpdate = false;
    bool completeObjective = false;
    
    if(obj->oType == GoldGoal)
    {
        if(progressNumber != GameHUD::getThis()->money)
        {
            progressNumber = GameHUD::getThis()->money;
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
        }
        else
        {
            progressNumber = GameHUD::getThis()->money;
        }
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    else if(obj->oType == FoodGoal)
    {
        if(progressNumber != ::atoi(GameHUD::getThis()->foodLabel->getString()))
        {
            progressNumber = ::atoi(GameHUD::getThis()->foodLabel->getString());
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
        }
        else
        {
            progressNumber = ::atoi(GameHUD::getThis()->foodLabel->getString());
        }
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    else if(obj->oType == PopulationGoal)
    {
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        int num = 0;
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            if(gs->villagerClass != V_CLASS_END && gs->villagerClass != V_BANDIT && gs->getHome() != NULL)
            {
                num++;
            }
        }
        
        if(progressNumber != num)
        {
            progressNumber = num;
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
        }
        else
        {
            progressNumber = num;
        }
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    else if(obj->oType == RaisePopulationGoal)
    {
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        int num = 0;
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            if(gs->villagerClass != V_CLASS_END && gs->villagerClass != V_BANDIT && gs->getHome() != NULL)
            {
                num++;
            }
        }
        
        int currentPopulation = obj->value - (targetPopulation - num);
        
        if(currentPopulation < 0)
        {
            currentPopulation = 0;
        }
        else if(currentPopulation > obj->value)
        {
            currentPopulation = obj->value;
        }
        
        if(progressNumber != currentPopulation)
        {
            progressNumber = currentPopulation;
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
        }
        else
        {
            progressNumber = currentPopulation;
        }
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    else if(obj->oType == ReputationGoal)
    {
        if(progressNumber != GameHUD::getThis()->reputation)
        {
            progressNumber = GameHUD::getThis()->reputation;
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
        }
        else
        {
            progressNumber = GameHUD::getThis()->reputation;
        }
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    else if(obj->oType == BuildBuilding)
    {
        ss << "Build a Building";
        //ss1 << "Build a Building: " << obj->strValue << "!";
        //ss2 << "(Build: Incomplete)";
        progressNumber = 0;
        hasUpdate = true;
        
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    else if(obj->oType == DisplayGoal)
    {
        
    }
    else
    {
        ss << "OBJECTIVE";
        progressNumber = -1;
        hasUpdate = true;
        
        
        if(progressNumber >= obj->value)
        {
            completeObjective = true;
        }
    }
    
    if(hasUpdate)
    {
        GameHUD::getThis()->objectiveProgress->setString(ss.str().c_str());
    }
    
    if(completeObjective)
    {
        playObjective();
    }
}