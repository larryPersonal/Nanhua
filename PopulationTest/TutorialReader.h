//
//  TutorialReader.h
//  PopulationTest
//
//  Created by admin on 10/4/14.
//
//

#ifndef PopulationTest_TutorialReader_h
#define PopulationTest_TutorialReader_h

#include "cocos2d.h"
#include "FileReader.h"
#include "TutorialSlide.h"

using namespace cocos2d;
using namespace std;

class TutorialReader
{
public:
    CCArray* slidesList;
    FileReader* fr;
    
    int curSlide;
    
public:
    TutorialReader();
    ~TutorialReader();
    
    void addSlide(TutorialSlide*);
    CCObject* getSlide(int index);
    
    // interact with the xml file;
    void parseXMLFile(string);
    
    void releaseSlides();
};

#endif
