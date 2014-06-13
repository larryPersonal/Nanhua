//
//  SpriteHandler.h
//  PopulationTest
//
//  Created by Serious Games on 25/3/13.
//
//

#ifndef __PopulationTest__SpriteHandler__
#define __PopulationTest__SpriteHandler__

#include "cocos2d.h"
#include "Sprite.h"
#include <vector.h>

class SpriteHandler
{
private:
    void AddToCache(CCSpriteBatchNode* spritesheet, std::string animName);
    
    float cumulatedTime;
    float cumulatedTime_energy;
public:
    CCArray* allSpriteClass;
    
    SpriteHandler();
    ~SpriteHandler();
    
    CCArray* tokensOnMap;
    
    CCArray* allSprites;
    CCArray* allSpriteSheets;
    
    CCArray* allClassRequirements;
    
    void initialize();

    CCArray* spritesOnMap;
    
    CCArray* fishAnimOnMap;
    
    GameSprite* getSpriteByVillagerClass(VillagerClass villagerClass);
    
    void addSpriteToMap(CCPoint &tilePos, VillagerClass, bool tutorial = false);
    void loadSpriteToMap(CCPoint &tilePos, GameSprite* sp, std::string details);
    
    GameSprite* getSpriteToMap(CCPoint &tilePos, VillagerClass villagerClass);
    
    void removeSpriteFromMap(GameSprite *sprite);
    
    void update(float);
    
    float getCumulatedTime();
    void setCumulatedTime(float);
};


#endif /* defined(__PopulationTest__SpriteHandler__) */

