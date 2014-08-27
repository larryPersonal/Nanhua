//
//  SanGuoXiaoXueTangFileReader.h
//  PopulationTest
//
//  Created by admin on 6/8/14.
//
//

#ifndef PopulationTest_SanGuoXiaoXueTangFileReader_h
#define PopulationTest_SanGuoXiaoXueTangFileReader_h

#include "cocos2d.h"
#include "FileReader.h"

using namespace cocos2d;
using namespace std;

class SanGuoXiaoXueTangFileReader
{
public:
    CCArray* questionList;
    FileReader* fr;
    
public:
    SanGuoXiaoXueTangFileReader();
    ~SanGuoXiaoXueTangFileReader();
    
    void parseXMLFile(string);
    void release();
};


#endif
