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

//this should be tied directly to GID. You will have to change this if any layer is added *below* the road layer
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
public:
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
    // call this only when dealing with a game sprite that isn't normally part of the tmx layer.
    // CCPoint forceTileOffset(CCPoint &spritePos);
    
    CCPoint forceBoundsConstraints(CCPoint &tilePos);
    
    bool isTilePosWithinBounds(CCPoint &tilePos);
    bool isTilePosWithinMap(CCPoint &tilePos); //ignores playarea
    bool isTileBlocked(CCPoint &tilePos);
    bool isTileBuildable(CCPoint &tilePos, bool obey_playarea = true);
    bool isBuildableOnTile(CCPoint &target, Building* building);

    void originateMapToTile();
    void rescaleScrollLimits();
    void updatePlayArea(CCPoint min, CCPoint max);
    void Populate(CCArray* layers);
    void PopulateForLoadingGame(CCArray* layers);
    
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
    Building* BuildOnMap(CCPoint &target, Building* building);
    bool Build(CCPoint &target, Building* building, bool isNewBuilding, bool skipConstruction=false, std::string withDetails = "", bool inGame=false);
    bool BuildPreview(CCPoint &target, Building* building);
    void ForceUnbuild(CCPoint &target);
    void UnBuild(CCPoint &target);
    void UnBuildPreview();
    
    SpriteSolidPoly* createTileHighlight(CCPoint& pos);
    
    CCPoint pointOnMapFromTouchLocation(CCPoint &location);
    CCPoint pointRelativeToCenterFromLocation(CCPoint &location);
    
    float calcZIndex(CCPoint &tilePos, int offset = 0, bool isSprite = false, GameSprite* = NULL); //I ASSUME BUILDING CAN BE PLACED.
    
    CCPoint getRandomTileLocation();
    CCPoint getRandomPathTile();
    CCPoint getRandomTile();
    
    void update(float dt);
    
    CCPoint getNearestNoneBuildingTile(CCPoint sourcePos);
    CCPoint getNearestPathTile(Building* building);
    
    bool isSpriteInBuilding(GameSprite* gameSprite, Building* building);
    
    void moveToPosition(CCPoint target, CCPoint current);
    bool checkPoint(CCPoint newCornerPos, int type, float moveX, float moveY);
    bool checkExtremePosition(float moveX, float moveY);
    
    CCPoint forceBounds(CCPoint &tilePos);
};



#endif
