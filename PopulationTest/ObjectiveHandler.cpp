//
//  ObjectiveHandler.cpp
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#include "ObjectiveHandler.h"
#include "ObjectiveManager.h"

ObjectiveHandler::ObjectiveHandler()
{
    objectives = CCArray::create();
    objectives->retain();
}

ObjectiveHandler::~ObjectiveHandler()
{
    objectives->removeAllObjects();
    CC_SAFE_RELEASE(objectives);
}

void ObjectiveHandler::loadObjective()
{
    objectives->removeAllObjects();
    objectives->release();
    
    objectives = ObjectiveManager::parseXMLFile("objective.xml");
}

CCObject* ObjectiveHandler::getObjective(int index)
{
    for (int i = 0; i < objectives->count(); i++)
    {
        Objective* objective = (Objective*) objectives->objectAtIndex(i);
        if(objective->oid == index)
        {
            return objective;
        }
    }
    return NULL;
}