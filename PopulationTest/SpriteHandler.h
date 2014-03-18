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

class SpriteHandler
{
private:
    void AddToCache(CCSpriteBatchNode* spritesheet, std::string animName);
    
    float cumulatedTime;
    float cumulatedTime_energy;
public:
    ~SpriteHandler();
    
    CCArray* tokensOnMap;
    
    CCArray* allSprites;
    CCArray* allSpriteSheets;
    
    CCArray* allClassRequirements;
    
    void initialize();
    

    CCArray* spritesOnMap;
    
    /*just because I might need this soon enough*/
    CCArray* aliensOnMap;
    CCArray* localsOnMap;
    
    
    GameSprite* getSpriteByType(SpriteType type);
    
    GameSprite* getSpriteTemplate(const char* classname, char gender, char race);
    void addSpriteToMap(CCPoint &tilePos, SpriteType type);
    void loadSpriteToMap(CCPoint &tilePos, GameSprite* sp, std::string details);
    
    void removeSpriteFromMap(GameSprite *sprite);
    
    void NewJobLocationCreated();
    void NewHousingLocationCreated();
    
    int getAlienCount();

    //All because we didn't decide the ranking of various sprite classes from the get go. Now I need a conversion function because nothing stores the rank by default.
    int getRank(std::string spriteclass);
    
    void update(float);
    
    float getCumulatedTime();
    void setCumulatedTime(float);
};


#endif /* defined(__PopulationTest__SpriteHandler__) */

