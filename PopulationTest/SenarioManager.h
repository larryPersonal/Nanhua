//
//  SenarioManager.h
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#ifndef PopulationTest_SenarioManager_h
#define PopulationTest_SenarioManager_h

#include "Slide.h"
#include "FileReader.h"
#include "Senario.h"
using namespace cocos2d;

class SenarioManager {
public:
    CCArray* slidesList;
    FileReader* fr;
    
    int curSlide;
    
public:
    SenarioManager();
    ~SenarioManager();
    
    void addSlide(Slide*);
    CCObject* getSlide(int index);
    
    // interact with the xml file;
    void parseXMLFile(string);
    
    void releaseSlides();
};


#endif
