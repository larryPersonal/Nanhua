//
//  MapHandler.h
//  PopulationTest
//
//  Created by Serious Games on 7/3/13.
//
//

#ifndef PopulationTest_MapHandler_h
#define PopulationTest_MapHandler_h

#include "cocos2d.h"
#include "MapTile.h"
#include "SpriteSolidPoly.h"

//this should be tied directly to GID. I hope.
enum TileType
{
    N = 0,
    NESE = 21,
    NESESW = 22,
    SESW = 23,
    NESENW = 24,
    NESESWNW = 25,
    SESWNW = 26,
    NENW = 27,
    NESWNW = 28,
    SWNW = 29,
    NESW = 30,
    SENW = 31,
};



class MapHandler
{
    CCTMXTiledMap* mapPtr;
  
    CCLayer* scalePanLayer;
    
    void setupTiles();
    
    
    float getInverseScale();
    
    Building* currBuildingPreview;
    SpriteSolidPoly* previewTileHighlight;
    
    SpriteSolidPoly* pathPreview;
    SpriteSolidPoly* pathPreviewTileHighlight;
     
public:
    MapHandler();
    ~MapHandler();
    void UnBuildEndGame();
    //2D array, where X and Y is position. To get the correct index, supply an index of X * y.
    //This is to keep things simple.
    
     void initTiles(const char* mapName);
    
    //moving map
    void moveMapBy(float moveX, float moveY);
    void scaleTo(float scaleFactor);
    void ResetPositionAndScale();
    
    void centerMap();
    
    CCArray* mapTiles;
    CCArray* pathTiles; //this array keeps all mapTiles that are marked as Path.
    CCArray* combatTiles;
    
    MapTile* getTileAt(int X, int Y);
    CCTMXTiledMap* getMap();
    CCLayer* getScaleLayer();
    
    CCPoint playarea_min;
    CCPoint playarea_max;

    CCPoint mapScroll_min;
    CCPoint mapScroll_max;
    
    CCPoint tilePosFromLocation(CCPoint &location);
    
    CCPoint tilePosFromTouchLocation(CCPoint &location); //Apparently, while I don't need to scale the sprite's tile location, I need to scale the TOUCH location.
    
    CCPoint locationFromTilePos(CCPoint *tilePos);
    //call this only when dealing with a game sprite that isn't normally part of the tmx layer.
  //  CCPoint forceTileOffset(CCPoint &spritePos);
    
    CCPoint forceBoundsConstraints(CCPoint &tilePos);
    
    bool isTilePosWithinBounds(CCPoint &tilePos);
    bool isTileBlocked(CCPoint &tilePos, bool);
    bool isTileBuildable(CCPoint &tilePos);
    bool isBuildableOnTile(CCPoint &target, Building* building);

    void originateMapToTile();
    void rescaleScrollLimits();
    void updatePlayArea(CCPoint min, CCPoint max);
    void Populate(CCArray* layers);
    
    /*path*/
    void Path(CCPoint &target);
    TileType PathTileUpdate(CCPoint &target, int propogate, CCTMXLayer* groundpath, bool destroymode);
    void PathLine(CCPoint &startTarget, CCPoint &endTarget);
    void PathPreview(CCPoint &target);
    CCPoint PathPreviewLineExtend(CCPoint &target, int max);
    void UnPath(CCPoint &target);
    void UnPathPreview();
    void UnPathPreviewLineExtend();
    
    /*building*/
    bool Build(CCPoint &target, Building* building, bool skipConstruction=false, std::string withDetails = "");
    bool BuildPreview(CCPoint &target, Building* building);
    void ForceUnbuild(CCPoint &target);
    void UnBuild(CCPoint &target);
    void UnBuildPreview();
    
    SpriteSolidPoly* createTileHighlight(CCPoint& pos);
    
    CCPoint pointOnMapFromTouchLocation(CCPoint &location);
    CCPoint pointRelativeToCenterFromLocation(CCPoint &location);
    
    float calcZIndex(CCPoint &tilePos);
    
    
    CCPoint getRandomTileLocation();
    CCPoint getRandomPathTile();
    
    void update(float dt);
};



#endif
