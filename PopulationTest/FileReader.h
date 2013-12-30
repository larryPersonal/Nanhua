//
//  FileReader.h
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#ifndef PopulationTest_FileReader_h
#define PopulationTest_FileReader_h

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class FileReader {
public:
    vector< string > mFileContents;
    
public:
    FileReader( string pFile, char pMoe = 'r' );
};

#endif
