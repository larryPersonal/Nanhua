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
    
    ObjectiveManager* om = new ObjectiveManager();
    om->parseXMLFile("objective.xml");
    
    objectives = om->objectives;
    delete om;
}