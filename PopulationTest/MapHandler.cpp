//
//  MapHandler.cpp
//  PopulationTest
//
//  Created by Serious Games on 7/3/13.
//
//

#include "MapHandler.h"
#include "GameScene.h"
#include "GameHUD.h"
#include "GlobalHelper.h"
#include "SelectPopulation.h"
#include "TutorialManager.h"
#include "PathFinder.h"
#include "SoundtrackManager.h"
#include "FileReader.h"
#include "UserProfile.h"

MapHandler* MapHandler::SP;

MapHandler::MapHandler()
{
    MapHandler::SP = this;
    mapPtr = NULL;
    scalePanLayer = NULL;
}

MapHandler::~MapHandler()
{
    MapHandler::SP = NULL;
    
    // delete mapPtr;
    UnBuildPreview();
    
    for (int i = 0; i < mapTiles->count(); ++i)
    {
        ((MapTile*)mapTiles->objectAtIndex(i))->UnBuild();
        
    }
    
    mapTiles->removeAllObjects();
    mapTiles->release();
    
    pathTiles->removeAllObjects();
    pathTiles->release();
}

MapHandler* MapHandler::getThis()
{
    return SP;
}

void MapHandler::UnBuildEndGame()
{
    for (int i = 0; i < mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; ++j)
        {
            if (getTileAt(i, j) != NULL)
            {
                if (getTileAt(i, j)->hasBuilding())
                {
                    CCPoint p = ccp(i,j);
                    ForceUnbuild(p);
                }
            }
        }
    }
}

void MapHandler::updatePlayArea(cocos2d::CCPoint min, cocos2d::CCPoint max)
{
    playarea_min = min;
    playarea_max = max;
    
    
    CCRect playAreaRect = CCRectMake(playarea_min.x, playarea_min.y,
                                     playarea_max.x - playarea_min.x,
                                     playarea_max.y - playarea_min.y);
    
    CCTMXLayer* groundzero = mapPtr->layerNamed("Ground_0");
    for (int i = 0; i < mapPtr->getMapSize().width; i++)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; j++)
        {
            if (!playAreaRect.containsPoint(ccp(i,j)))
            {
                groundzero->tileAt(ccp(i, j))->setColor(ccGRAY);
            }
            else
            {
                groundzero->tileAt(ccp(i, j))->setColor(ccWHITE);
            }
        }
    }
    
    
}


void MapHandler::setupTiles()
{
    if (!mapPtr) return;
    
    CCTMXLayer* groundzero = mapPtr->layerNamed("Ground_0");
    
    mapTiles = CCArray::create();
    
      for (int i = 0; i < mapPtr->getMapSize().height; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().width; ++j)
        {
            
            MapTile* tile = new MapTile();
            tile->autorelease();
            tile->tileGID = groundzero->tileGIDAt(ccp(j,i));
            tile->xpos = i;
            tile->ypos = j;
            mapTiles->addObject(tile);
        }
        
        
    }
    mapTiles->retain();
    
    pathTiles = CCArray::create();
    pathTiles->retain();
    
    
    updatePlayArea(playarea_min, playarea_max);
}

float MapHandler::getInverseScale()
{
    return 1.0f / MapHandler::getThis()->scalePanLayer->getScale();
    
}

MapTile* MapHandler::getTileAt(int X, int Y)
{
    int targetIndex = MapHandler::getThis()->mapPtr->getMapSize().width * X + Y;
    if (targetIndex >= MapHandler::getThis()->mapTiles->count())
            return NULL;
    
    
    return  (MapTile*)MapHandler::getThis()->mapTiles->objectAtIndex( targetIndex );
}

void MapHandler::initTiles(const char* mapName)
{
    // As we are going to load a map from map file, a valid map name is required.
    if (!mapName){
        return;
    }
    
    MapHandler::getThis()->mapPtr = CCTMXTiledMap::create(mapName);
    
    CCTMXLayer* metaLayer = MapHandler::getThis()->mapPtr->layerNamed("Metadata");
    
    if (metaLayer){
        metaLayer->setVisible(false);
    }
    
    MapHandler::getThis()->playarea_min = GameManager::getThis()->getMinArea();
    MapHandler::getThis()->playarea_max = GameManager::getThis()->getMaxArea();
    
    MapHandler::getThis()->setupTiles();
    
    MapHandler::getThis()->scalePanLayer = CCLayer::create();
    
    MapHandler::getThis()->currBuildingPreview = NULL;
    MapHandler::getThis()->previewTileHighlight = NULL;
    MapHandler::getThis()->pathPreview = NULL;
    MapHandler::getThis()->pathPreviewTileHighlight = NULL;
}

CCLayer* MapHandler::getScaleLayer()
{
    return MapHandler::getThis()->scalePanLayer;
}

CCTMXTiledMap* MapHandler::getMap()
{
    return MapHandler::getThis()->mapPtr;
}

void MapHandler::scaleTo(float scaleFactor)
{
    scalePanLayer->setScale(scaleFactor);
    moveMapBy(0, 0);
    rescaleScrollLimits();
}

/* type indicate which screen point is used for checking
 * the equation for checking the corner points are based on the map implementation! (so if map has been changed, change the checking equation accordingly
 * 0 -> left bottom corner of the screen, checking equation: y = -0.5x + 2208, check above the checking line
 * 1 -> left top corner of the screen, checking eqaution: y = 0.5x + 1696, check below the checking line
 * 2 -> right top corner of the screen, checking equation: y = -0.5x + 5984, check below the checking line
 * 3 -> right bottom corner of the screen, checking equation: y = 0.5x - 2528, check above the checking line
 */
bool MapHandler::checkPoint(CCPoint newCornerPos, int type, float moveX, float moveY)
{
    switch (type)
    {
        // left bottom corner, check above line
        case 0:
        {
            float expectedY = -0.5 * newCornerPos.x + 2208.0f;
            if (newCornerPos.y >= expectedY)
            {
                return true;
            }
        }
            break;
        // left top corner, check below line
        case 1:
        {
            float expectedY = 0.5 * newCornerPos.x + 1632.0f;
            if (newCornerPos.y <= expectedY) // (moveX >= 0 && moveY <= 0) || (moveX >= 0 && moveY >= 0 && moveX / moveY >= 0.5f) || (moveX <= 0 && moveY <= 0 && moveX / moveY <= 0.5))
            {
                return true;
            }
        }
            break;
        // right top corner, check below line
        case 2:
        {
            float expectedY = -0.5 * newCornerPos.x + 5984.0f;
            if (newCornerPos.y <= expectedY)
            {
                return true;
            }
        }
            break;
        // right bottom corner, check above line
        case 3:
        {
            float expectedY = 0.5 * newCornerPos.x - 2528.0f;
            if (newCornerPos.y >= expectedY) //|| (moveX <= 0 && moveY >= 0) || (moveX >= 0 && moveY >= 0 && moveX / moveY <= 0.5f) || (moveX <= 0 && moveY <= 0 && moveX / moveY >= 0.5f))
            {
                return true;
            }
        }
            break;
        default:
            break;
    }
    
    
    if(GameScene::getThis()->isInDeccelerating)
    {
        GameScene::getThis()->isInDeccelerating = false;
        GameScene::getThis()->unschedule(schedule_selector(GameScene::decceleratingDragging));
    }
    
    return false;
}

bool MapHandler::checkExtremePosition(float moveX, float moveY)
{
    CCPoint mapPos = mapPtr->getPosition();
    return true;
}

void MapHandler::moveMapBy(float moveX, float moveY)
{
    //mapPtr moves. ScalePanLayer scales.
    CCPoint mapPos = MapHandler::getThis()->mapPtr->getPosition();
    
    // the new map position, it will be used only the screen points pass the valilidity check!
    CCPoint newPos =CCPointMake(
                             MapHandler::getThis()->mapPtr->getPosition().x + moveX,
                             MapHandler::getThis()->mapPtr->getPosition().y + moveY
                             );
    
    //mapPtr->setPosition(forceBoundsConstraints(newPos));
    MapHandler::getThis()->mapPtr->setPosition(forceBounds(newPos));
    
    // CCLog("%f, %f",mapPtr->getPosition().x, mapPtr->getPosition().y );
    // scalePanLayer->setPosition(forceBoundsConstraints(pos));
}

void MapHandler::moveToPosition(CCPoint target, CCPoint current)
{
    CCPoint targetPos = locationFromTilePos(&target);
    CCPoint currentPos = locationFromTilePos(&current);
    
    // get the horizontal and vertical length difference!
    float xDiff = targetPos.x - currentPos.x;
    float yDiff = targetPos.y - currentPos.y;
    
    mapPtr->setPosition(mapPtr->getPositionX() - xDiff, mapPtr->getPositionY() - yDiff);
    //moveMapBy(-xDiff, -yDiff);
}

void MapHandler::centerMap()
{
    CCPoint pos = CCPointMake(-4600, -2600);
    MapHandler::getThis()->mapPtr->setPosition(pos.x, pos.y);
}

void MapHandler::ResetPositionAndScale()
{
    mapPtr->setScale(1.0f);
    scalePanLayer->setScale(1.0f);
    
    rescaleScrollLimits();
    originateMapToTile();
}


CCPoint MapHandler::tilePosFromLocation(CCPoint &location)
{
    CCPoint pos = ccpSub(location, MapHandler::getThis()->mapPtr->getPosition());
    
    float halfMapWidth = MapHandler::getThis()->mapPtr->getMapSize().width * 0.5f ;
	float mapHeight = MapHandler::getThis()->mapPtr->getMapSize().height;
	float tileWidth = MapHandler::getThis()->mapPtr->getTileSize().width;
	float tileHeight = MapHandler::getThis()->mapPtr->getTileSize().height;
    
	CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
    
	float mapHeightDiff = mapHeight - tilePosDiv.y;
	
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
	int posX = (mapHeightDiff + tilePosDiv.x - halfMapWidth);
	int posY = (mapHeightDiff - tilePosDiv.x + halfMapWidth);
	return CCPointMake(posX, posY);
}


CCPoint MapHandler::tilePosFromTouchLocation(cocos2d::CCPoint &location)
{
    CCPoint pos = pointOnMapFromTouchLocation(location);
    
    float halfMapWidth = MapHandler::getThis()->mapPtr->getMapSize().width * 0.5f;
	float mapHeight = MapHandler::getThis()->mapPtr->getMapSize().height;
	float tileWidth = MapHandler::getThis()->mapPtr->getTileSize().width;
    float tileHeight = MapHandler::getThis()->mapPtr->getTileSize().height;
	
	CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
    float mapHeightDiff = mapHeight - tilePosDiv.y;
  
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
	int posX = (mapHeightDiff + tilePosDiv.x - halfMapWidth);
	int posY = (mapHeightDiff - tilePosDiv.x + halfMapWidth);
    
	return CCPointMake(posX, posY);
}

CCPoint MapHandler::locationFromTilePos (CCPoint *location)
{
    CCTMXLayer *grass = MapHandler::getThis()->mapPtr->layerNamed("Ground_0");
	CCSprite *tile = grass->tileAt(*location);//  [grass tileAt:tilePos];
    
    float x = tile->getPosition().x + MapHandler::getThis()->mapPtr->getTileSize().width * 0.5f;
    float y = tile->getPosition().y + MapHandler::getThis()->mapPtr->getTileSize().height ;// * 0.5f);
    //	float y = tile->getPosition().y - mapPtr->getTileSize().height * mapPtr->getScale() + (mapPtr->getTileSize().height *mapPtr->getScale() * 0.5f);
	return CCPointMake(x,y);
}

bool MapHandler::isTilePosWithinBounds(CCPoint &tilePos)
{
    if ((tilePos.x < MapHandler::getThis()->playarea_min.x) ||
       (tilePos.x > MapHandler::getThis()->playarea_max.x) ||
       (tilePos.y < MapHandler::getThis()->playarea_min.y) ||
       (tilePos.y > MapHandler::getThis()->playarea_max.y))
    {
       return false;
    }
    
    return true;
}
//ignores playarea settings which generally mean "smaller than map"
bool MapHandler::isTilePosWithinMap(CCPoint & tilePos)
{
    if ((tilePos.x < 0) ||
        (tilePos.x > mapPtr->getMapSize().width) ||
        (tilePos.y < 0) ||
        (tilePos.y > mapPtr->getMapSize().height))
    {
        return false;
    }
    
    return true;
}

void MapHandler::originateMapToTile()
{
    MapHandler::getThis()->mapPtr->setPosition(CCPointMake(-MapHandler::getThis()->mapPtr->boundingBox().size.width * 0.4f,
                                                           -MapHandler::getThis()->mapPtr->boundingBox().size.height * 0.75f));
}

void MapHandler::rescaleScrollLimits()
{
    // first, make sure map is existed, game will crash?
    if (MapHandler::getThis()->mapPtr == NULL){
        return;
    }
    
    // center map first! Or it will take the wrong position values.
    // int centerW = mapPtr->getTileSize().width * 0.5f * getScaleLayer()->getScale();
    // int centerH = mapPtr->getTileSize().height * 0.5f * getScaleLayer()->getScale();
    
    CCPoint mapPos = MapHandler::getThis()->mapPtr->getPosition();
    
    //Note: 30 and 5 are offsets in tile amounts. A scroll position based on tiles will mean the maximum places a tile at the edge of a screen.
    //offsetting tries to solve the issue by putting a limit at X tiles before the edge. 
    int playAreaW = ((MapHandler::getThis()->playarea_max.x - MapHandler::getThis()->playarea_min.x) - 30) * MapHandler::getThis()->mapPtr->getTileSize().width * 0.5f * getScaleLayer()->getScale();
    int playAreaH = ((MapHandler::getThis()->playarea_max.y - MapHandler::getThis()->playarea_min.y) - 5) * MapHandler::getThis()->mapPtr->getTileSize().height * 0.5f * getScaleLayer()->getScale();
    
    //HARDCODE ALERT -5736, -1592, based on the stuff in centerMap
    MapHandler::getThis()->mapScroll_max = ccp(mapPos.x + playAreaW, mapPos.y + playAreaH);
    MapHandler::getThis()->mapScroll_min = ccp(mapPos.x - playAreaW, mapPos.y - playAreaH);
}

CCPoint MapHandler::forceBoundsConstraints(CCPoint &tilePos)
{
    // make sure coordinates are within bounds of the playable area, correcting them if not
    tilePos.x = MIN(mapScroll_max.x, tilePos.x);
	tilePos.x = MAX(mapScroll_min.x, tilePos.x);
	tilePos.y = MIN(mapScroll_max.y, tilePos.y);
	tilePos.y = MAX(mapScroll_min.y, tilePos.y);
	    
	return tilePos;
}

CCPoint MapHandler::forceBounds(CCPoint &tilePos)
{
    CCPoint maxBounds = ccp(-3600, -2000);
    CCPoint minBounds = ccp(-6000, -3200);
    
    tilePos.x = MIN(maxBounds.x, tilePos.x);
	tilePos.x = MAX(minBounds.x, tilePos.x);
	tilePos.y = MIN(maxBounds.y, tilePos.y);
	tilePos.y = MAX(minBounds.y, tilePos.y);
    
    // CCLog("map position is (%f, %f)", tilePos.x, tilePos.y);
    
    return tilePos;
}

CCPoint MapHandler::pointOnMapFromTouchLocation(cocos2d::CCPoint &location)
{
    CCPoint centerOffset = ccpSub(location, GameScene::getThis()->screenCenter->getPosition());
    centerOffset = ccpSub(centerOffset, MapHandler::getThis()->scalePanLayer->getPosition());
    centerOffset.x *= getInverseScale();
    centerOffset.y *= getInverseScale();
    CCPoint pos = ccpSub(centerOffset, MapHandler::getThis()->mapPtr->getPosition());
    pos = ccpSub(pos, ccpMult(MapHandler::getThis()->scalePanLayer->getAnchorPointInPoints(), getInverseScale() - 1.0f));
    
    return pos;
}

CCPoint MapHandler::pointRelativeToCenterFromLocation(cocos2d::CCPoint &location)
{
    return ccpSub(mapPtr->getPosition(), location);
}

bool MapHandler::isTileBuildable(cocos2d::CCPoint &tilePos, bool obey_playarea)
{
    if (obey_playarea)
    {
        if (!isTilePosWithinBounds(tilePos))
        {
            CCLog("outofBounds");
            return false;
        }
    }
    else
    {
        if (!isTilePosWithinMap(tilePos))
        {
            CCLog("outofMap");
            return false;
        }
        
    }
    MapTile* targetTile = getTileAt(tilePos.x, tilePos.y);
    if (targetTile == NULL) return false;
    
    if (obey_playarea)
    {
        if (targetTile->hasBuilding() || targetTile->isPath)
        {
            CCLog("Target tile isOccupied or isPath");
            return false;
        }
    }
    
    return true;
}


//Note: ignore this for destination.
bool MapHandler::isTileBlocked(cocos2d::CCPoint &tilePos)
{
    if (!isTilePosWithinBounds(tilePos)){
        return true;
    }
    
    
    MapTile* targetTile = getTileAt(tilePos.x, tilePos.y);
    if(targetTile->hasBuilding())
    {
        if(targetTile->building)
        {
            if(targetTile->building->buildingType == DECORATION)
            {
                return true;
            }
        }
    }
    
    if (!targetTile->isPath && !targetTile->hasBuilding()){
        return true;
    }
    
    //if the first test passes, check if its blocked by the metalayer
    CCTMXLayer* meta = mapPtr->layerNamed("Metadata");
    if (!meta){
        return false;
    }
    int metaGID = meta->tileGIDAt(tilePos);
    if (metaGID > 0)
    {
        CCDictionary* properties = mapPtr->propertiesForGID(metaGID);
        if (properties)
        {
            //has to be const, because return value of vForKey is const
            const CCString* col = properties->valueForKey("impassable");
            if (strcmp(col->getCString(),"1")){
                return true;
            }
        }
    }
   
    return false;
}

bool MapHandler::isBuildableOnTile(CCPoint &target, Building* building)
{
    // Check if tiles are occupied
    bool shouldObeyPlayArea = building->buildingType != DECORATION && true;
    
    for (int i = 0; i < building->height; i++)
        for (int j = 0; j < building->width; j++)
        {
            CCPoint tilePos = ccp(target.x + j, target.y + i);
            if (!isTileBuildable(tilePos, shouldObeyPlayArea))
            {
                return false;
            }
        }
    return true;
}

void MapHandler::Populate(CCArray* layers)
{
        for (int i = 0; i < MapHandler::getThis()->mapPtr->getMapSize().width; ++i)
        {
            for (int j = 0; j < MapHandler::getThis()->mapPtr->getMapSize().height; ++j)
            {
                int gid = 0;
                CCPoint tgtPoint = CCPointMake(i,j);
                
                for (int k = layers->count() -1; k>= 0; --k)
                {
                    CCTMXLayer* layer = (CCTMXLayer*)(layers->objectAtIndex(k));
                    
                   
                    if (strncmp(layer->getLayerName(), "Building", strlen("Building")) == 0)
                    {
                        
                        gid = layer->tileGIDAt(tgtPoint);
                        if (gid != 0)
                            break;
                    }
           
                    
                }
                
                if (gid == 0) continue;
                if (!GameScene::getThis()) continue;
                
                // CCLog("************** the gid is %d", gid);
                Building* targetBuilding = BuildingHandler::getThis()->getBuildingWithGID(gid);
                if (!targetBuilding)
                {
                    continue;
                }
                
                if (targetBuilding->buildingType != BUILDINGCATEGORYMAX)
                {
                    if (targetBuilding->buildingType == HOUSING)
                    {
                        targetBuilding = BuildingHandler::getThis()->getRandomBuilding(targetBuilding);
                    }
                    
                    MapHandler::getThis()->Build(tgtPoint, targetBuilding, true, true);
                }
                
            }
        }
    
    //again for paths
    //Path layer is named Ground_Road
    CCTMXLayer* pLayer = MapHandler::getThis()->mapPtr->layerNamed("Ground_Road");
    
    /*
    for (int i = 0; i < MapHandler::getThis()->mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < MapHandler::getThis()->mapPtr->getMapSize().height; ++j)
        {
            MapTile* tile = this->getTileAt(i,j);
            if (tile == NULL) continue;
            tile->tileGID = pLayer->tileGIDAt(ccp(i,j));
            if (tile->tileGID > 0)
            {
                CCLog("%d", tile->tileGID);
                tile->pathHere();
                MapHandler::getThis()->pathTiles->addObject(tile);
            }
        }
    }
    */

    //now for environment //WARNING Ground_1 is now the tile layer
    //putting stuff back in Ground_0 for now
    pLayer = MapHandler::getThis()->mapPtr->layerNamed("Ground_1");
    for (int i = 0; i < MapHandler::getThis()->mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < MapHandler::getThis()->mapPtr->getMapSize().height; ++j)
        {
            CCPoint tilePos = ccp(i, j);
            CCSprite* environment = pLayer->tileAt(tilePos);
            if (environment != NULL)
            {
                // Remove environment tile from TMXLayer, and add to map instead
                environment->retain();
                environment->removeFromParent();
                MapHandler::getThis()->getMap()->addChild(environment, calcZIndex(tilePos));
                getTileAt(i, j)->setEnvironment(environment);
            }
        }
    }
    pLayer->setVisible(false);
}

void MapHandler::PopulateForLoadingGame(CCArray* layers)
{
    for (int i = 0; i < mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; ++j)
        {
            int gid = 0;
            CCPoint tgtPoint = CCPointMake(i,j);
            
            for (int k = layers->count() -1; k>= 0; --k)
            {
                CCTMXLayer* layer = (CCTMXLayer*)(layers->objectAtIndex(k));
                
                
                if (strncmp(layer->getLayerName(), "Building", strlen("Building")) == 0)
                {
                    
                    gid = layer->tileGIDAt(tgtPoint);
                    if (gid != 0)
                        break;
                }
                
                
            }
            
            if (gid == 0) continue;
            if (!GameScene::getThis()) continue;
            
            // CCLog("************** the gid is %d", gid);
            Building* targetBuilding = BuildingHandler::getThis()->getBuildingWithGID(gid);
            if (!targetBuilding)
            {
                continue;
            }
            
            if (targetBuilding->buildingType != BUILDINGCATEGORYMAX && targetBuilding->buildingType != HOUSING && targetBuilding->buildingType != AMENITY && targetBuilding->buildingType != GRANARY && targetBuilding->buildingType != MARKET && targetBuilding->buildingType != MILITARY && targetBuilding->buildingType != SPECIAL && targetBuilding->buildingType != EDUCATION && targetBuilding->buildingType != SOCIAL)
            {
                Build(tgtPoint, targetBuilding, true, true);
            }
            
        }
    }
}


float MapHandler::calcZIndex(CCPoint &point, int offset, bool isSprite, GameSprite* gs)
{
    float lowestZ = 0;// mapPtr->getMapSize().width + mapPtr->getMapSize().height;
    float currZ = (point.x + point.y) * 2 + offset;
    if(isSprite)
    {
        /*
        if(gs->getCurrentDir().compare("DR") == 0)
        {
            //currZ += 1;
        }
        else if(gs->getCurrentDir().compare("DL") == 0)
        {
            //currZ += 1;
        }
        else if(gs->getCurrentDir().compare("UL") == 0)
        {
            currZ -= 1;
        }
        else if(gs->getCurrentDir().compare("UR") == 0)
        {
            currZ -= 1;
        }
        */
    }
    else
    {
        currZ += 1;
    }
    
    return (lowestZ + currZ) + MapHandler::getThis()->mapPtr->layerNamed("Ground_1")->getZOrder();
}

CCPoint MapHandler::getRandomTileLocation()
{
    CCPoint tgt = CCPointMake( rand() % (int)mapPtr->getMapSize().width, rand() % (int)mapPtr->getMapSize().height);
    return tgt;
}

// get a random path tile for all the wandering villages when they are in idle state
CCPoint MapHandler::getRandomPathTile()
{
    // if no path tile on the map! (how this will happen?)
    if (MapHandler::getThis()->pathTiles->count() == 0)
    {
        return CCPointMake(-1, -1);
    }
    
    // get a random map tile from all path tiles.
    int targetIdx = rand() % MapHandler::getThis()->pathTiles->count();
    MapTile* tgtTile = (MapTile*)MapHandler::getThis()->pathTiles->objectAtIndex(targetIdx);
    
    // if the tile is not a path, cannot go there.
    if (!tgtTile->isPath){
        return CCPointMake(-1,-1);
    }
    
    return CCPointMake(tgtTile->xpos, tgtTile->ypos);
}

// get a random tile for the rain
CCPoint MapHandler::getRandomTile()
{
    if(mapTiles->count() == 0)
    {
        return CCPointMake(-1, -1);
    }
    
    // get a random map tile
    int targetIdx = rand() % mapTiles->count();
    MapTile* mapTile = (MapTile*)mapTiles->objectAtIndex(targetIdx);
    
    return CCPointMake(mapTile->xpos, mapTile->ypos);
}

Building* MapHandler::BuildOnMap(cocos2d::CCPoint &target, Building* building)
{
    if (!building)
    {
        return NULL;
    }
    
    /*Note: do NOT use the pointer directly! The pointer points to the main instance of the building. */
    
    Building* cloneBuilding = (Building*) building->copy();
    
    if (cloneBuilding->buildingType == BUILDINGCATEGORYMAX)
    {
        return NULL;
    }
    
    // Don't build if tiles are occupied. EDIT: Ignore this rule if Building is a decoration, which allows it to build on OOB tiles and over each other.
    if (!isBuildableOnTile(target, cloneBuilding))
    {
        return NULL;
    }
    
    cloneBuilding->buildingRep = CCSprite::create();
    cloneBuilding->buildingRep->initWithTexture(cloneBuilding->buildingTexture, cloneBuilding->buildingRect);
    CCPoint tilePos = MapHandler::getThis()->locationFromTilePos(&target);
    cloneBuilding->buildingRep->setPosition(tilePos);
    getMap()->addChild(cloneBuilding->buildingRep, calcZIndex(target, (cloneBuilding->width - 1) * 2.0f)); //force buildings to be drawn always on
    
    MapTile* master = getTileAt(target.x, target.y);
    for (int i = 0; i < cloneBuilding->height; i++)
    {
        for (int j = 0; j < cloneBuilding->width; j++)
        {
            if (i == 0 && j == 0)
            {
                master->Build(cloneBuilding);
            }
            else
            {
                getTileAt(target.x + j, target.y + i)->setMaster(master);
            }
        }
    }
    
    cloneBuilding->ID = BuildingHandler::getThis()->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
    
    cloneBuilding->build_uint_current = cloneBuilding->build_uint_required;
    
    BuildingHandler::getThis()->addBuildingToMap(cloneBuilding);
    
    return cloneBuilding;
}

Building* MapHandler::Build(cocos2d::CCPoint &target, Building* building, bool isNewBuilding, bool skipConstruction, std::string withDetails, bool inGame)
{
    if (!building)
    {
        return NULL;
    }
    
    /*Note: do NOT use the pointer directly! The pointer points to the main instance of the building. */
    
    Building* cloneBuilding = (Building*) building->copy();
    
    if (cloneBuilding->buildingType == BUILDINGCATEGORYMAX)
    {
        return NULL;
    }
    
    // Don't build if tiles are occupied. EDIT: Ignore this rule if Building is a decoration, which allows it to build on OOB tiles and over each other.
    if (!isBuildableOnTile(target, cloneBuilding))
    {
        return NULL;
    }

    cloneBuilding->buildingRep = CCSprite::create();
    cloneBuilding->buildingRep->initWithTexture(cloneBuilding->buildingTexture, cloneBuilding->buildingRect);
    CCPoint tilePos = MapHandler::getThis()->locationFromTilePos(&target);
    cloneBuilding->buildingRep->setPosition(tilePos);
    getMap()->addChild(cloneBuilding->buildingRep, calcZIndex(target, (cloneBuilding->width - 1) * 2.0f)); //force buildings to be drawn always on top
    
    MapTile* master = getTileAt(target.x, target.y);
    for (int i = 0; i < cloneBuilding->height; i++)
    {
        for (int j = 0; j < cloneBuilding->width; j++)
        {
            if (i == 0 && j == 0)
            {
                master->Build(cloneBuilding);
            }
            else
            {
                getTileAt(target.x + j, target.y + i)->setMaster(master);
            }
        }
    }
    
    if(isNewBuilding)
    {
        string username = UserProfile::getThis()->username;
        stringstream sss;
        sss << username << "_building_unique_id";
        int buildingUniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(sss.str().c_str(), 0);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(sss.str().c_str(), buildingUniqueID + 1);
        // CCLog("Building unique id: %d", buildingUniqueID);
    
        cloneBuilding->uniqueID = buildingUniqueID;
    }
    
    if (skipConstruction)
    {
        cloneBuilding->ID = BuildingHandler::getThis()->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
        
        if (withDetails.length() > 0)
        {
            stringstream ss(withDetails);
            std::string buffer;
            std::vector<std::string>tokens;
            while (ss.peek() != EOF)
            {
                std::getline(ss, buffer, '|' );
                
                tokens.push_back(buffer);
                
                buffer = "";
                
            }
            
            if (tokens.size() == 9)
            {
            
                cloneBuilding->ID = atoi(tokens[8].c_str());
                cloneBuilding->currentExp = atoi(tokens[6].c_str());
                cloneBuilding->currentLevel = atoi(tokens[7].c_str());
            }
        }
        
        cloneBuilding->build_uint_current = cloneBuilding->build_uint_required;
        
        BuildingHandler::getThis()->addBuildingToMap(cloneBuilding);
    }
    else
    {
        //GameHUD::getThis()->buyBuilding(cloneBuilding->buildingCost);
        //GameScene::getThis()->buildingHandler->addBuildingToMap(cloneBuilding);
        
        SoundtrackManager::PlaySFX("construction.wav");
        
        ConstructionHandler::getThis()->addConstructingBuilding(cloneBuilding);
    }
    
    if(inGame)
    {
        PopupMenu::backupCurrentPopupMenu();
        SelectPopulation* selectPopulation = SelectPopulation::create(cloneBuilding);
        selectPopulation->useAsTopmostPopupMenu();
        
        if(TutorialManager::getThis()->active && (TutorialManager::getThis()->teachBuildHouse || TutorialManager::getThis()->teachBuildGranary || TutorialManager::getThis()->teachBuildFarm))
        {
            selectPopulation->setZOrder(35);
        }
        
        if(TutorialManager::getThis()->active && (TutorialManager::getThis()->teachBuildHouse || TutorialManager::getThis()->teachBuildGranary || TutorialManager::getThis()->teachBuildFarm))
        {
            TutorialManager::getThis()->miniDragon->clickNext();
        }
    }
    return cloneBuilding;
}

bool MapHandler::BuildPreview(cocos2d::CCPoint &target, Building* building)
{
    if (!building)
    {
        return false;
    }
    
    // Can only have one building preview at a time.
    if (currBuildingPreview != NULL){
        UnBuildPreview();
    }
    
    /*Note: do NOT use the pointer directly! The pointer points to the main instance of the building. */
    currBuildingPreview = (Building*) building->copy();
    if (currBuildingPreview->buildingType == BUILDINGCATEGORYMAX)
    {
        return false;
    }
    
    CCPoint tilePos = MapHandler::getThis()->locationFromTilePos(&target);
    
    // Add tile highlight
    previewTileHighlight = createTileHighlight(tilePos);
    getMap()->addChild(previewTileHighlight, playarea_max.x + playarea_max.y + 1);
    
    // Okay, build.
    currBuildingPreview->retain();
    currBuildingPreview->buildingRep = CCSprite::create();
    currBuildingPreview->buildingRep->initWithTexture(currBuildingPreview->buildingTexture, currBuildingPreview->buildingRect);
    currBuildingPreview->buildingRep->setPosition(tilePos);
    currBuildingPreview->buildingRep->setOpacity(75);
    
    //I assume buildings are SQUARE.
    getMap()->addChild(currBuildingPreview->buildingRep, calcZIndex(target, (currBuildingPreview->width - 1) * 2.0f)); //force buildings to be drawn always on top
    
    // Show red-tinted preview and red-tinted tile and return false if tiles are occupied.
    if (!isBuildableOnTile(target, currBuildingPreview))
    {
        previewTileHighlight->setColor(ccc4f(255/255.0f, 64/255.0f, 64/255.0f, 0.25f));
        currBuildingPreview->buildingRep->setColor(ccc3(255, 64, 64));
        return false;
    }
    return true;
}


void MapHandler::ForceUnbuild(cocos2d::CCPoint &target)
{
    Building* targetB = getTileAt(target.x, target.y)->building;
    if (targetB == NULL){
        //CCLog("Building is null leh");
        return;
    }
    
    // Try remove from constructionHandler
    ConstructionHandler::getThis()->removeConstructingBuilding(targetB);
    BuildingHandler::getThis()->removeBuildingFromMap(targetB);
    
    MapHandler::getThis()->getMap()->removeChild(targetB->buildingRep, true);
    
    
    for (int i = 0; i < targetB->height; i++)
        for (int j = 0; j < targetB->width; j++)
            getTileAt(target.x + j, target.y + i)->UnBuild();

}


void MapHandler::UnBuild(cocos2d::CCPoint &target)
{
    Building* targetB = getTileAt(target.x, target.y)->building;
    if (targetB == NULL){
        //CCLog("Building is null leh");
        return;
    }
    
    int targetID = targetB->ID;
    for (int i = 0; i < BuildingHandler::getThis()->specialOnMap->count(); ++i)
    {
        Building* b = (Building*)BuildingHandler::getThis()->specialOnMap->objectAtIndex(i);
        if (b->ID == targetID)
        {
            CCLog("SPECIALS cannot be destroyed");
            return;
        }
    }
    
    // Try remove from constructionHandler
    ConstructionHandler::getThis()->removeConstructingBuilding(targetB);
    
    MapHandler::getThis()->getMap()->removeChild(targetB->buildingRep, true);
    BuildingHandler::getThis()->removeBuildingFromMap(targetB);
    
    
    for (int i = 0; i < targetB->height; i++)
        for (int j = 0; j < targetB->width; j++)
            getTileAt(target.x + j, target.y + i)->UnBuild();
    
    // this may result to increase of the memory used (when destroy the building, the texture of the building is not release?), I am not sure, keep to check it!
    // CC_SAFE_RELEASE(targetB);
}

void MapHandler::UnBuildPreview()
{
    if (MapHandler::getThis()->currBuildingPreview == NULL){
        return;
    }
    
    MapHandler::getThis()->getMap()->removeChild(MapHandler::getThis()->previewTileHighlight, true);
    MapHandler::getThis()->getMap()->removeChild(MapHandler::getThis()->currBuildingPreview->buildingRep, true);
    
    //currBuildingPreview->buildingRep->release();
   // currBuildingPreview->buildingRep = NULL;
   // delete currBuildingPreview;
//    previewTileHighlight->release();
    delete MapHandler::getThis()->previewTileHighlight;
    MapHandler::getThis()->previewTileHighlight = NULL;
    MapHandler::getThis()->currBuildingPreview = NULL;
}

SpriteSolidPoly* MapHandler::createTileHighlight(CCPoint& pos)
{
    SpriteSolidPoly* preview = new SpriteSolidPoly();
    preview->addVertex(CCPointZero);
    preview->addVertex(ccp(-mapPtr->getTileSize().width / 2.0f, -mapPtr->getTileSize().height / 2.0f));
    preview->addVertex(ccp(0.0f, -mapPtr->getTileSize().height));
    preview->addVertex(ccp(mapPtr->getTileSize().width / 2.0f, -mapPtr->getTileSize().height / 2.0f));
    preview->setColor(ccc4f(128/255.0f, 80/255.0f, 155/255.0f, 0.25f));
    preview->setPosition(pos);
    return preview;
}

void MapHandler::Path(cocos2d::CCPoint &target)
{
    MapTile* targetTile = getTileAt(target.x, target.y);
    if (targetTile->hasBuilding() || targetTile->isOccupied()) return;
    
    targetTile->pathHere();
    pathTiles->addObject(targetTile); //I assume we only need to declare this new tile a path.
    CCTMXLayer* groundpath = mapPtr->layerNamed("Ground_Road");
    
    //PathTileUpdate(target, 0);
    //1. Assume that only adjacent tiles will chance.
    //2. Assume adjacent tiles already have the correct connections aside from the new tile.
    //3. Do not change any tile but adjaacent tiles.
    //4.set start tile to non-zero value first, otherwise, propogate logic won't work.
    groundpath->setTileGID(SWNW, target);
    PathTileUpdate(target, 2, groundpath, false);
    
    
    
    
}

//returns a GID for the tile
//but sets the tile to the correct GID first.
//Recursive! Use propogate to limit the number of times called.
TileType MapHandler::PathTileUpdate(cocos2d::CCPoint &target, int propogate, CCTMXLayer* groundpath, bool destroymode = false)
{
     if (!MapHandler::isTilePosWithinBounds(target)) return N; //edge case. You can NOT build beyond the edge.
    
    int currGID = groundpath->tileGIDAt(target);
    if (propogate == 0)
    {
 
        
        return (TileType)currGID; //just return the GID for anything we aren't supposed to change.
        
    }
    if (propogate == 1 && currGID == 0) //probably not supposed to update a 0 value tile.
    {
        return N;
    }
    --propogate;
    
    //Note: isometric! Only 1 dimension should be added to.
    CCPoint NW = ccp(target.x -1, target.y);
    CCPoint SW = ccp(target.x, target.y + 1);
    CCPoint NE = ccp(target.x, target.y - 1);
    CCPoint SE = ccp(target.x +1, target.y);
    TileType tiles[4] = {N,N, N, N};
    tiles[0] = PathTileUpdate(NE, propogate, groundpath);
    tiles[1] = PathTileUpdate(SE, propogate, groundpath);
    tiles[2] = PathTileUpdate(SW, propogate, groundpath);
    tiles[3] = PathTileUpdate(NW, propogate, groundpath);
    
    MapTile* NWTile = getTileAt(NW.x, NW.y);
    MapTile* SWTile = getTileAt(SW.x, SW.y);
    MapTile* NETile = getTileAt(NE.x, NE.y);
    MapTile* SETile = getTileAt(SE.x, SE.y);
    
    MapTile* currentTile = getTileAt(target.x, target.y);
    
    int total = 0;
    
    if(NETile->isPath || NETile->hasBuilding())
    {
        total += 1;
    }
    
    if(SETile->isPath || SETile->hasBuilding())
    {
        total += 2;
    }
    
    if(SWTile->isPath || SWTile->hasBuilding())
    {
        total += 4;
    }
    
    if(NWTile->isPath || NWTile->hasBuilding())
    {
        total += 8;
    }
    
    if(total <= 0)
    {
        return (TileType)currentTile->tileGID;
    }
    
    
    //4-way
    // CCLog("TOTAL %d", total);
    TileType ttile = NESW;
    if (destroymode)
        ttile = N;
    
    if (total == 15) //there are exits everywhere.
    {
        ttile = NESESWNW;
    }
    
    //straight lines
    if (total == 1 || total == 4 || total == 5)
    {
        ttile = NESW;
    }
    
    if (total == 2 || total == 8 || total == 10)
    {
    
        ttile = SENW;
    }
   
    //corners
    if (total == 3) //corner
    {
        
        ttile = NESE;
    }
    
    if (total == 6) //another corner
    {
        ttile = SESW;
    }
    
    if (total == 9) //third corner
    {
        ttile = NENW;
    }
    
    if (total == 12) //last corner
    {
        ttile = SWNW;
    }
    
    //3-way
    if (total == 7)
    {
        ttile = NESESW;
    }
    if (total == 11)
    {
        ttile = NESENW;
    }
    if (total == 13)
    {
        ttile = NESWNW;
    }
    if (total == 14)
    {
        ttile = SESWNW;
    }
    
    if (currGID != ttile && !destroymode)
        groundpath->setTileGID(ttile, target);
    return ttile;
}

void MapHandler::PathLine(CCPoint &startTarget, CCPoint &endTarget)
{
    bool isModifierX = (startTarget.x != endTarget.x ? true : false);
    int modifierValue = isModifierX ? endTarget.x - startTarget.x : endTarget.y - startTarget.y;
    CCPoint modifierPos = startTarget;
    
    for (int i = 0; i != modifierValue + (modifierValue > 0 ? 1 : -1); modifierValue > 0 ? i++ : i--)
    {
        if (isModifierX)
            modifierPos.x = startTarget.x + i;
        else
            modifierPos.y = startTarget.y + i;
        
        Path(modifierPos);
    }
}

void MapHandler::PathPreview(cocos2d::CCPoint &target)
{
    CCPoint tileLoc = locationFromTilePos(&target);
    
    if (pathPreview != NULL)
    {
        delete pathPreview;
        delete pathPreviewTileHighlight;
        
        UnPathPreviewLineExtend();
        pathPreview->setPosition(tileLoc);
        pathPreviewTileHighlight->setPosition(tileLoc);
        return;
    }
    
    pathPreview = createTileHighlight(tileLoc);
    pathPreview->setColor(ccc4f(100/255.0f, 55/255.0f, 130/255.0f, 0.15f));
    pathPreviewTileHighlight = createTileHighlight(tileLoc);
    getMap()->addChild(pathPreview, playarea_max.x + playarea_max.y + 1);
    getMap()->addChild(pathPreviewTileHighlight, playarea_max.x + playarea_max.y + 1);
}

CCPoint MapHandler::PathPreviewLineExtend(cocos2d::CCPoint &touchLoc, int max)
{
    // Create pathPreview if not yet created
    if (pathPreview == NULL)
    {
        CCPoint target = tilePosFromTouchLocation(touchLoc);
        PathPreview(target);
        return target;
    }
    
    // Get start and end locations
    CCPoint extendedLoc = pointOnMapFromTouchLocation(touchLoc);
    CCPoint tileLoc = pathPreview->getPosition();
    tileLoc.y -= mapPtr->getTileSize().height / 2.0f;
    
    // Do a vector projection to get the desired preview path
    CCPoint vec = ccpSub(extendedLoc, tileLoc);
    GlobalHelper::vectorProjection(vec, ccp(vec.x >= 0 ? mapPtr->getTileSize().width : -mapPtr->getTileSize().width,
                                            vec.y >= 0 ? mapPtr->getTileSize().height : -mapPtr->getTileSize().height));
    vec = ccpAdd(vec, tileLoc);
    
    // Get start and end tile positions
    CCPoint startTilePos = ccpAdd(tileLoc, mapPtr->getPosition());
    startTilePos = tilePosFromLocation(startTilePos);
    CCPoint endTilePos = ccpAdd(vec, mapPtr->getPosition());
    endTilePos = tilePosFromLocation(endTilePos);
    
    // Do a loop from start to end, stopping if hit a block
    bool isModifierX = (startTilePos.x != endTilePos.x ? true : false);
    int modifierValue = isModifierX ? endTilePos.x - startTilePos.x : endTilePos.y - startTilePos.y;
    CCPoint modifierPos = startTilePos;
    // Set maximum distance
    max--;
    max = MAX(0, max);
    modifierValue = MIN(modifierValue, max);
    modifierValue = MAX(modifierValue, -max);
    
    for (int i = 0; i != modifierValue; modifierValue > 0 ? i++ : i--)
    {
        if (isModifierX)
            modifierPos.x = startTilePos.x + i + (modifierValue > 0 ? 1 : -1);
        else
            modifierPos.y = startTilePos.y + i + (modifierValue > 0 ? 1 : -1);
        
        if (!isTileBuildable(modifierPos))
            break;
        
        endTilePos = modifierPos;
    }
    
    // Get the end tile offset in location
    CCPoint endTileLocOffset = locationFromTilePos(&endTilePos);
    endTileLocOffset.x -= pathPreview->getPositionX();
    endTileLocOffset.y -= pathPreview->getPositionY();
    
    // Move the vertices of current pathPreview to create a line
    if (modifierValue > 0)
        pathPreview->setVertexAt(2, endTileLocOffset.x, endTileLocOffset.y - mapPtr->getTileSize().height);
    else
        pathPreview->setVertexAt(0, endTileLocOffset.x, endTileLocOffset.y);
    
    if (isModifierX ^ (modifierValue > 0))
        pathPreview->setVertexAt(1, endTileLocOffset.x - mapPtr->getTileSize().width / 2.0f,
                                    endTileLocOffset.y - mapPtr->getTileSize().height / 2.0f);
    else
        pathPreview->setVertexAt(3, endTileLocOffset.x + mapPtr->getTileSize().width / 2.0f,
                                    endTileLocOffset.y - mapPtr->getTileSize().height / 2.0f);
    
    // Set pathPreviewTileHighlight to the last tile
    pathPreviewTileHighlight->setPosition(locationFromTilePos(&endTilePos));
    
    return endTilePos;
}

void MapHandler::UnPath(cocos2d::CCPoint &target)
{
    MapTile* targetTile = getTileAt(target.x, target.y);
    pathTiles->removeObject(targetTile);
    targetTile->unpathHere();
    CCTMXLayer* groundpath = mapPtr->layerNamed("Ground_Road");
    
    groundpath->setTileGID(0, target);
    
    PathTileUpdate(target, 2, groundpath, true);
    
    
}

void MapHandler::UnPathPreview()
{
    if (MapHandler::getThis()->pathPreview == NULL)
        return;
    
    MapHandler::getThis()->getMap()->removeChild(MapHandler::getThis()->pathPreview, true);
    MapHandler::getThis()->getMap()->removeChild(MapHandler::getThis()->pathPreviewTileHighlight, true);
    MapHandler::getThis()->pathPreview = NULL;
    MapHandler::getThis()->pathPreviewTileHighlight = NULL;
}

void MapHandler::UnPathPreviewLineExtend()
{
    if (pathPreview == NULL)
        return;
    
    MapHandler::getThis()->pathPreview->setVertexAt(0, 0, 0);
    MapHandler::getThis()->pathPreview->setVertexAt(1, -MapHandler::getThis()->mapPtr->getTileSize().width / 2.0f, -MapHandler::getThis()->mapPtr->getTileSize().height / 2.0f);
    MapHandler::getThis()->pathPreview->setVertexAt(2, 0, -MapHandler::getThis()->mapPtr->getTileSize().height);
    MapHandler::getThis()->pathPreview->setVertexAt(3, MapHandler::getThis()->mapPtr->getTileSize().width / 2.0f, -MapHandler::getThis()->mapPtr->getTileSize().height / 2.0f);
}

void MapHandler::update(float dt)
{
}

CCPoint MapHandler::getNearestNoneBuildingTile(CCPoint sourcePos)
{
    PathFinder* pf = new PathFinder();
    CCPoint targetPos = pf->getNearestNoneBuildingPos(&sourcePos);
    delete pf;
    return targetPos;
}

CCPoint MapHandler::getNearestPathTile(Building* building)
{
    CCPoint buildingPos = building->getWorldPosition();
    // CCLog("the building World position is: (%f, %f)", buildingPos.x, buildingPos.y);
    CCPoint buildingTilePos = tilePosFromLocation(buildingPos);
    // CCLog("the building tile position is (%f, %f)", buildingTilePos.x, buildingTilePos.y);
    int width = building->width;
    // CCLog("the building width is: %d", width);
    
    CCPoint pathTilePos = ccp(-1, -1);
    for (int i = buildingTilePos.x - 1; i < buildingTilePos.x + width + 1; i++)
    {
        MapTile* mapTile = getTileAt(i, buildingTilePos.y - 1);
        if(mapTile->isPath)
        {
            pathTilePos = ccp(mapTile->xpos, mapTile->ypos);
            return pathTilePos;
        }
    }
    
    for (int j = buildingTilePos.y; j < buildingTilePos.y + building->height + 1; j++)
    {
        MapTile* mapTile = getTileAt(buildingTilePos.x - 1, j);
        if(mapTile->isPath)
        {
            pathTilePos = ccp(mapTile->xpos, mapTile->ypos);
            return pathTilePos;
        }
        
        mapTile = getTileAt(buildingTilePos.x + width, j);
        if(mapTile->isPath)
        {
            pathTilePos = ccp(mapTile->xpos, mapTile->ypos);
            return pathTilePos;
        }
    }
    
    for (int i = buildingTilePos.x; i < buildingTilePos.x + width; i++)
    {
        MapTile* mapTile = getTileAt(i, buildingTilePos.y + building->height);
        if(mapTile->isPath)
        {
            pathTilePos = ccp(mapTile->xpos, mapTile->ypos);
            return pathTilePos;
        }
    }
    return pathTilePos;
}

bool MapHandler::isSpriteInBuilding(GameSprite* gameSprite, Building* building)
{
    CCPoint spritePos = gameSprite->getWorldPosition();
    CCPoint spriteTilePos = tilePosFromLocation(spritePos);
    
    CCPoint buildingPos = building->getWorldPosition();
    CCPoint buildingTilePos = tilePosFromLocation(buildingPos);
    
    int width = building->width;
    int height = building->height;
    
    return spriteTilePos.x >= buildingTilePos.x && spriteTilePos.x < (buildingTilePos.x + width) && spriteTilePos.y >= buildingTilePos.y && spriteTilePos.y < (buildingTilePos.y + height);
}

void MapHandler::loadLevelBuildings(int level)
{
    string xmlFile = "";
    switch (level)
    {
        case 0:
            xmlFile = "level0_building.xml";
            break;
        case 1:
            xmlFile = "level1_building.xml";
            break;
        case 2:
            xmlFile = "level2_building.xml";
            break;
        case 3:
            xmlFile = "level3_building.xml";
            break;
        case 4:
            xmlFile = "level4_building.xml";
            break;
        case 5:
            xmlFile = "level5_building.xml";
            break;
        case 6:
            xmlFile = "level6_building.xml";
            break;
        default:
            xmlFile = "level0_building.xml";
            break;
    }
    
    FileReader* fr = new FileReader(xmlFile);
    
    stringstream ss;
    
    bool isInProject = false;
    bool isInBuilding = false;
    
    string type = "";
    int posX = 0;
    int posY = 0;
    
    for(int i = 0; i < fr->mFileContents.size(); i++)
    {
        // get the line first
        string str = fr->mFileContents.at(i);
        
        // CCLog("**** %s", str.c_str());
        
        // if a line contains "<?xml", then this line is the xml header, ignore it and continue;
        if(str.find("<?xml") != std::string::npos)
        {
            continue;
        }
        
        if(!isInProject)
        {
            ss.str(std::string());
            if(str.find("<project>") != std::string::npos)
            {
                isInProject = true;
            }
        }
        else if(!isInBuilding)
        {
            ss.str(std::string());
            if(str.find("<building>") != std::string::npos)
            {
                isInBuilding = true;
            }
            else if(str.find("</project>") != std::string::npos)
            {
                isInProject = false;
            }
        }
        else
        {
            unsigned startPos = -1;
            unsigned endPos = -1;
            string content = "";
            int temp = 0;
            
            ss.str(std::string());
            if(str.find("<type>") != std::string::npos && str.find("</type>") != std::string::npos)
            {
                startPos = str.find("<type>");
                endPos = str.find("</type>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                type = content;
            }
            else if(str.find("<posX>") != std::string::npos && str.find("</posX>") != std::string::npos)
            {
                startPos = str.find("<posX>");
                endPos = str.find("</posX>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                temp = ::atoi(content.c_str());
                posX = temp;
            }
            else if(str.find("<posY>") != std::string::npos && str.find("</posY>") != std::string::npos)
            {
                startPos = str.find("<posY>");
                endPos = str.find("</posY>");
                content = str.substr(startPos + 6, endPos - startPos - 6);
                temp = ::atoi(content.c_str());
                posY = temp;
            }
            else if(str.find("</building>") != std::string::npos)
            {
                buildTheBuildingWhenLoadingMap(type, posX, posY);
                isInBuilding = false;
                type = "";
                posX = 0;
                posY = 0;
            }
        }
    }
    delete fr;
}

void MapHandler::buildTheBuildingWhenLoadingMap(string type, int posX, int posY)
{
    BuildingCategory bc = HOUSING;
    if(type.compare("housing") == 0)
    {
        bc = HOUSING;
    }
    else if(type.compare("granary") == 0)
    {
        bc = GRANARY;
    }
    else if(type.compare("amenity") == 0)
    {
        bc = AMENITY;
    }
    else if(type.compare("commerce") == 0)
    {
        bc = COMMERCE;
    }
    else if(type.compare("military") == 0)
    {
        bc = MILITARY;
    }
    else if(type.compare("education") == 0)
    {
        bc = EDUCATION;
    }
    else if(type.compare("social") == 0)
    {
        bc = SOCIAL;
    }
    else if(type.compare("special") == 0)
    {
        bc = SPECIAL;
    }
    else if(type.compare("decoration") == 0)
    {
        bc = DECORATION;
    }
    else if(type.compare("market") == 0)
    {
        bc = MARKET;
    }
    else if(type.compare("river") == 0)
    {
        bc = RIVER;
    }
    else if(type.compare("road") == 0)
    {
        bc = BUILDINGCATEGORYMAX;
        
        CCPoint tilePos = CCPointMake(posX, posY);
        MapTile* selectedTile = MapHandler::getThis()->getTileAt(tilePos.x, tilePos.y);
        if(!selectedTile->isPath)
        {
            Path(tilePos);
        }
    }
    else if(type.compare("sprite") == 0)
    {
        bc = BUILDINGCATEGORYMAX;
        
        if (!CCUserDefault::sharedUserDefault()->getBoolForKey("isLoadingGame"))
        {
            CCPoint spritePos = CCPointMake(posX, posY);
            SpriteHandler::getThis()->addSpriteToMap(spritePos, V_REFUGEE);
        }
    }
    else
    {
        bc = BUILDINGCATEGORYMAX;
    }
    
    if(bc != BUILDINGCATEGORYMAX)
    {
        Building* targetBuilding = BuildingHandler::getThis()->getFirstBuildingOfCategory(bc);
        if (!targetBuilding)
        {
            return;
        }
        
        CCPoint tgtPoint = CCPointMake(posX, posY);
        
        if (targetBuilding->buildingType != BUILDINGCATEGORYMAX)
        {
            if (targetBuilding->buildingType == HOUSING)
            {
                targetBuilding = BuildingHandler::getThis()->getRandomBuilding(targetBuilding);
            }
            
            Build(tgtPoint, targetBuilding, true, true);
        }

    }
}

void MapHandler::UnBuildAllPath()
{
    while(MapHandler::getThis()->pathTiles->count() > 0)
    {
        MapTile* mapTile = (MapTile*)MapHandler::getThis()->pathTiles->objectAtIndex(0);
        CCPoint tilePos = ccp(mapTile->xpos, mapTile->ypos);
        MapHandler::getThis()->UnPath(tilePos);
    }
}

void MapHandler::setUniqueBuildingID(Building* bui)
{
    // set the unique id for that building
    string username = UserProfile::getThis()->username;
    stringstream ss;
    ss << username << "_building_unique_id";
    
    int uniqueID = CCUserDefault::sharedUserDefault()->getIntegerForKey(ss.str().c_str(), 0);
    bui->uniqueID = uniqueID;
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ss.str().c_str(), uniqueID + 1);
}


