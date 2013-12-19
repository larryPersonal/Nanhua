//
//  SoundtrackManager.h
//  PopulationTest
//
//  Created by Serious Games on 19/8/13.
//
//

#ifndef __PopulationTest__SoundtrackManager__
#define __PopulationTest__SoundtrackManager__

#include <iostream>

class SoundtrackManager
{
public:
//    SoundtrackManager();
//    ~SoundtrackManager();
    
    static SoundtrackManager* SP;
    
      
    static void PlayBGM(std::string bgmName);
    
    static void StopBGM();
};


#endif /* defined(__PopulationTest__SoundtrackManager__) */
