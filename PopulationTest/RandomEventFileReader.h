//
//  RandomEventFileReader.h
//  PopulationTest
//
//  Created by admin on 9/7/14.
//
//

#ifndef PopulationTest_RandomEventFileReader_h
#define PopulationTest_RandomEventFileReader_h

#include "cocos2d.h"
#include "FileReader.h"

using namespace cocos2d;
using namespace std;

class RandomEventFileReader
{
public:
    CCArray* randomEventsList;
    FileReader* fr;
    
public:
    RandomEventFileReader();
    ~RandomEventFileReader();
    
    // interact with the xml file;
    void parseXMLFile(string);
    
    void release();
};

#endif
