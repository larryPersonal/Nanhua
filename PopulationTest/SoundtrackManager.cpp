//
//  SoundtrackManager.cpp
//  PopulationTest
//
//  Created by Serious Games on 19/8/13.
//
//

#include "SoundtrackManager.h"
//#include "CocosDenshion.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
/*
SoundtrackManager* SoundtrackManager::SP;


SoundtrackManager::SoundtrackManager()
{
    SP = this;
}

SoundtrackManager::~SoundtrackManager()
{
    StopBGM();
}
*/


void SoundtrackManager::PlayBGM(std::string bgmName)
{
    
    if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        StopBGM();
    
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(bgmName.c_str(), true);
    
    

    
}

void SoundtrackManager::StopBGM()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
}


void SoundtrackManager::PlaySFX(std::string sfxName)
{
    SimpleAudioEngine::sharedEngine()->playEffect(sfxName.c_str(), false);
    
}
/*
void SoundtrackManager::StopSFX()
{
    
    
}*/