//
//  ObjectiveManager.h
//  PopulationTest
//
//  Created by admin on 18/3/14.
//
//

#ifndef PopulationTest_ObjectiveManager_h
#define PopulationTest_ObjectiveManager_h

#include "ObjectiveHandler.h"
#include "Objective.h"
#include "FileReader.h"

using namespace cocos2d;

class ObjectiveManager {
public:
    CCArray* objectives;
    FileReader* fr;
    
public:
    ObjectiveManager();
    ~ObjectiveManager();
    
    void addObjective(Objective*);
    CCObject* getObjective(int index);
    
    // interact with the xml file;
    void parseXMLFile(string);
};

#endif
