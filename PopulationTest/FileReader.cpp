//
//  FileReader.cpp
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#include "FileReader.h"
#include <fstream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

FileReader::FileReader( string pFileName, char pMode ) {
    //mFileContents = vector<string>();
    // Initialize variables needed
    unsigned long fileSize = 0;
    unsigned char * fileContents = NULL;
    string thisLine, result, fullPath, contents;
    
    // Get absolute path of file
    fullPath = CCFileUtils::sharedFileUtils( ) -> fullPathForFilename( pFileName.c_str( ) );
    
    // Get data of file
    fileContents = CCFileUtils::sharedFileUtils( ) -> getFileData( fullPath.c_str( ) , "r", &fileSize );
    if(fileContents != NULL){
        contents.append( ( char * ) fileContents );
    }
        
    // Create a string stream so that we can use getline( ) on it
    istringstream fileStringStream( contents );
    
    // Get file contents line by line
    while ( getline( fileStringStream, thisLine ) ) {
        // Put all lines in vector
        mFileContents.push_back( thisLine );
    }
    
    // After this, mFileContents will have an extra entry and will have the value '\x04'.
    // We should remove this by popping it out the vector.
    if(mFileContents.size() > 0){
        mFileContents.pop_back( );
    }
    
    // Delete buffer created by fileContents. This part is required.
    if ( fileContents ) {
        delete fileContents; //lookout, it's actually a pointer NOT an array... delete [] doesn't work on non-specific arrays. LARRY
        fileContents = NULL;
    }

    contents = "";
    // For testing purposes
    CCLog("[ NOTICE ] Finished opening file: " );
    //CCLog( pFileName.c_str() );
    CCLog(" \n");
    cout << "[ NOTICE ] Finished opening file: " << pFileName.c_str( ) << endl;
}

FileReader::~FileReader()
{
    mFileContents.clear(); //alamak, I think it doesn't clear itself //LARRY
}