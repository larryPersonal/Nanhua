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

ObjectiveHandler* ObjectiveHandler::SP;

ObjectiveHandler::ObjectiveHandler()
{
    ObjectiveHandler::SP = this;
    
    objectives = CCArray::create();
    objectives->retain();
    
    objectiveStrs = CCArray::create();
    objectiveStrs->retain();
    
    nextID = 0;
    progressNumber = 0;
}

ObjectiveHandler::~ObjectiveHandler()
{
    ObjectiveHandler::SP = NULL;
    
    objectives->removeAllObjects();
    CC_SAFE_RELEASE(objectives);
    
    objectiveStrs->removeAllObjects();
    CC_SAFE_RELEASE(objectiveStrs);
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
    objectives->release();
    
    objectives = ObjectiveManager::parseXMLFile("objective.xml");
}

void ObjectiveHandler::playObjective()
{
    if(objectives->count() <= 0)
    {
        return;
    }
    
    for(int i = 0; i < objectiveStrs->count(); i++)
    {
        CCLabelTTF* tempLabel = (CCLabelTTF*) objectiveStrs->objectAtIndex(i);
        GameHUD::getThis()->removeChild(tempLabel);
    }
    objectiveStrs->removeAllObjects();
    CC_SAFE_RELEASE(objectiveStrs);
    
    objectiveStrs = CCArray::create();
    objectiveStrs->retain();
    
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
            if(gs->villagerClass != V_CLASS_END && gs->getHome() != NULL)
            {
                num++;
            }
        }
        ss2 << "(" << num << "/" << obj->value << ")";
        progressNumber = num;
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
    else
    {
        ss << "OBJECTIVE";
        ss1 << "There is no objectives currently!";
        ss2 << "";
        progressNumber = -1;
    }
    
    GameHUD::getThis()->objectiveTitle->setString(ss.str().c_str());
    GameHUD::getThis()->objectiveDescription->setString(ss1.str().c_str());
    GameHUD::getThis()->objectiveProgress->setString(ss2.str().c_str());
    nextID = obj->nid;
}

void ObjectiveHandler::update(float dt)
{
    if(obj == NULL)
    {
        return;
    }
    
    stringstream ss;
    
    bool hasUpdate = false;
    
    if(obj->oType == GoldGoal)
    {
        if(progressNumber != GameHUD::getThis()->money)
        {
            progressNumber = GameHUD::getThis()->money;
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
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
    }
    else if(obj->oType == PopulationGoal)
    {
        CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
        int num = 0;
        for (int i = 0; i < spritesOnMap->count(); i++)
        {
            GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
            if(gs->villagerClass != V_CLASS_END && gs->getHome() != NULL)
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
    }
    else if(obj->oType == ReputationGoal)
    {
        if(progressNumber != GameHUD::getThis()->reputation)
        {
            progressNumber = GameHUD::getThis()->reputation;
            ss << "(" << progressNumber << "/" << obj->value << ")";
            hasUpdate = true;
        }
    }
    else if(obj->oType == BuildBuilding)
    {
        ss << "Build a Building";
        //ss1 << "Build a Building: " << obj->strValue << "!";
        //ss2 << "(Build: Incomplete)";
        progressNumber = 0;
        hasUpdate = true;
    }
    else
    {
        ss << "OBJECTIVE";
        progressNumber = -1;
        hasUpdate = true;
    }
    
    if(hasUpdate)
    {
        GameHUD::getThis()->objectiveProgress->setString(ss.str().c_str());
    }
}