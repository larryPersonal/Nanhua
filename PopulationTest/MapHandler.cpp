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

MapHandler::MapHandler()
{
    combatTiles = CCArray::create();
    combatTiles->retain();
}

MapHandler::~MapHandler()
{
   // delete mapPtr;
  /*
    for (int i = 0; i < mapTiles->count(); ++i)
    {
        
    }
    */
    UnBuildPreview();
    
    for (int i = 0; i < mapTiles->count(); ++i)
    {
        ((MapTile*)mapTiles->objectAtIndex(i))->UnBuild();
        
    }
   /*
    for (int i = 0; i < mapTiles->count(); ++i)
    {
        mapTiles->objectAtIndex(i)->release();
        
    }*/
    mapTiles->removeAllObjects();
    mapTiles->release();
    
    pathTiles->removeAllObjects();
    pathTiles->release();
    
    combatTiles->removeAllObjects();
    combatTiles->release();
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
    return 1.0f / scalePanLayer->getScale();
    
}


MapTile* MapHandler::getTileAt(int X, int Y)
{
    int targetIndex = mapPtr->getMapSize().width * X + Y;
    if (targetIndex >= mapTiles->count())
            return NULL;
    
    
    return  (MapTile*)mapTiles->objectAtIndex( targetIndex );
}

void MapHandler::initTiles(const char* mapName)
{
    if (!mapName) return;
    mapPtr = CCTMXTiledMap::create(mapName);
    CCTMXLayer* metaLayer = mapPtr->layerNamed("Metadata");
    if (metaLayer) metaLayer->setVisible(false);
    
    
    playarea_min = GameManager::getThis()->getMinArea();
    playarea_max = GameManager::getThis()->getMaxArea();
    
    setupTiles();
    
    scalePanLayer = CCLayer::create();
    
    
    currBuildingPreview = NULL;
    previewTileHighlight = NULL;
    pathPreview = NULL;
    pathPreviewTileHighlight = NULL;
}

CCLayer* MapHandler::getScaleLayer()
{
    return scalePanLayer;
}

CCTMXTiledMap* MapHandler::getMap()
{
    return mapPtr;
}

void MapHandler::scaleTo(float scaleFactor)
{
    scalePanLayer->setScale(scaleFactor);
    rescaleScrollLimits();
    
}

void MapHandler::moveMapBy(float moveX, float moveY)
{
    //mapPtr moves. ScalePanLayer scales. 
    CCPoint pos =CCPointMake(
                mapPtr->getPosition().x + moveX,
                mapPtr->getPosition().y + moveY
                             );
    mapPtr->setPosition(forceBoundsConstraints(pos));
  //  CCLog("%f, %f",mapPtr->getPosition().x, mapPtr->getPosition().y );
  //  scalePanLayer->setPosition(forceBoundsConstraints(pos));
}

void MapHandler::centerMap()
{
    CCPoint pos = CCPointMake(-5736, -1592);
    mapPtr->setPosition(pos.x, pos.y);
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
    CCPoint pos = ccpSub(location, mapPtr->getPosition());
    
    float halfMapWidth = mapPtr->getMapSize().width * 0.5f ;
	float mapHeight = mapPtr->getMapSize().height;
	float tileWidth = mapPtr->getTileSize().width;
	float tileHeight = mapPtr->getTileSize().height;
	
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
    
    float halfMapWidth = mapPtr->getMapSize().width * 0.5f;
	float mapHeight = mapPtr->getMapSize().height;
	float tileWidth = mapPtr->getTileSize().width;
    float tileHeight = mapPtr->getTileSize().height;
	
	CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
    float mapHeightDiff = mapHeight - tilePosDiv.y;
  
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
	int posX = (mapHeightDiff + tilePosDiv.x - halfMapWidth);
	int posY = (mapHeightDiff - tilePosDiv.x + halfMapWidth);
    
	return CCPointMake(posX, posY);
}

CCPoint MapHandler::locationFromTilePos (CCPoint *location)
{
    CCTMXLayer *grass = mapPtr->layerNamed("Ground_0");
	CCSprite *tile = grass->tileAt(*location);//  [grass tileAt:tilePos];
    
    float x = tile->getPosition().x + mapPtr->getTileSize().width * 0.5f;
    float y = tile->getPosition().y + mapPtr->getTileSize().height ;// * 0.5f);
    //	float y = tile->getPosition().y - mapPtr->getTileSize().height * mapPtr->getScale() + (mapPtr->getTileSize().height *mapPtr->getScale() * 0.5f);
	return CCPointMake(x,y);
}

bool MapHandler::isTilePosWithinBounds(CCPoint &tilePos)
{
    if ((tilePos.x < playarea_min.x) ||
       (tilePos.x > playarea_max.x) ||
       (tilePos.y < playarea_min.y) ||
       (tilePos.y > playarea_max.y))
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
    mapPtr->setPosition(CCPointMake(-mapPtr->boundingBox().size.width * 0.4f,
                                    -mapPtr->boundingBox().size.height * 0.75f));
    
}

void MapHandler::rescaleScrollLimits()
{
    // Some complex instructions here...
    if (mapPtr == NULL) return;
    
    //The next person who decides to ngeh ngeh lai and cannot explain his code will get it from me mmmmkay? - Larry
    /*
     
    float qScreenW = CCDirector::sharedDirector()->getWinSize().width * 0.25f;
    float qScreenH = CCDirector::sharedDirector()->getWinSize().height * 0.25f;
    
    float halfTileW = mapPtr->getTileSize().width / 2.0f * scalePanLayer->getScale();
    float halfTileH = mapPtr->getTileSize().height / 2.0f * scalePanLayer->getScale();
    
    float maxH = (playarea_max.x + playarea_max.y) * halfTileH;
    float minH = (playarea_min.x + playarea_min.y) * halfTileH;
    
    float halfW = ((playarea_max.x - playarea_min.x) + (playarea_max.y - playarea_min.y)) * halfTileW / 2.0f;
    float offsetW = (((playarea_max.x + playarea_min.x) / 2) - ((playarea_max.y + playarea_min.y) / 2)) * halfTileW;
    
    mapScroll_max = CCPointMake(halfW - offsetW - qScreenW, maxH - qScreenH);
    mapScroll_min = CCPointMake(-halfW - offsetW + qScreenW, minH + qScreenH);
    */
    //center map first! Or it will take the wrong position values.
    int centerW = mapPtr->getTileSize().width * 0.5f * getScaleLayer()->getScale();
    int centerH = mapPtr->getTileSize().height * 0.5f * getScaleLayer()->getScale();
    
    CCPoint mapPos = mapPtr->getPosition();
    //Note: 30 and 5 are offsets in tile amounts. A scroll position based on tiles will mean the maximum places a tile at the edge of a screen.
    //offsetting tries to solve the issue by putting a limit at X tiles before the edge. 
    int playAreaW = ((playarea_max.x - playarea_min.x) - 30) * mapPtr->getTileSize().width * 0.5f * getScaleLayer()->getScale();
    int playAreaH = ((playarea_max.y - playarea_min.y) - 5) * mapPtr->getTileSize().height * 0.5f * getScaleLayer()->getScale();
    //HARDCODE ALERT -5736, -1592, based on the stuff in centerMap
    mapScroll_max = ccp(mapPos.x + playAreaW, mapPos.y + playAreaH);
    mapScroll_min = ccp(mapPos.x - playAreaW, mapPos.y - playAreaH);
    
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

CCPoint MapHandler::pointOnMapFromTouchLocation(cocos2d::CCPoint &location)
{
    CCPoint centerOffset = ccpSub(location, GameScene::getThis()->screenCenter->getPosition());
    centerOffset = ccpSub(centerOffset, scalePanLayer->getPosition());
    centerOffset.x *= getInverseScale();
    centerOffset.y *= getInverseScale();
    CCPoint pos = ccpSub(centerOffset, mapPtr->getPosition());
    pos = ccpSub(pos, ccpMult(scalePanLayer->getAnchorPointInPoints(), getInverseScale() - 1.0f));
    
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
    if (targetTile->hasBuilding() || targetTile->isPath)
    {
        CCLog("Target tile isOccupied or isPath");
        return false;
    }
    
    return true;
}


//Note: ignore this for destination.
bool MapHandler::isTileBlocked(cocos2d::CCPoint &tilePos, bool tryEscape)
{
    if (!isTilePosWithinBounds(tilePos)){
        return true;
    }
    
    
    MapTile* targetTile = getTileAt(tilePos.x, tilePos.y);
    if (!targetTile->isPath  || (targetTile->isInCombat && !tryEscape)){
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
    bool shouldObeyPlayArea = (building->buildingType != DECORATION);
    
    for (int i = 0; i < building->height; i++)
        for (int j = 0; j < building->width; j++)
        {
            CCPoint tilePos = ccp(target.x + j, target.y + i);
            if (!isTileBuildable(tilePos, shouldObeyPlayArea))
                return false;
        }
    return true;
}

void MapHandler::Populate(CCArray* layers)
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
                
                Building* targetBuilding = GameScene::getThis()->buildingHandler->getBuildingWithGID(gid);
                if (!targetBuilding) continue;
                if (targetBuilding->buildingType != BUILDINGCATEGORYMAX)
                {
                    
                    Build(tgtPoint, targetBuilding, true);
                }
                
            }
        }
    
    
    //again for paths
    //Path layer is named Ground_Road
    CCTMXLayer* pLayer = mapPtr->layerNamed("Ground_Road");
    
    for (int i = 0; i < mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; ++j)
        {
            MapTile* tile = this->getTileAt(i,j);
            if (tile == NULL) continue;
            tile->tileGID = pLayer->tileGIDAt(ccp(i,j));
            if (tile->tileGID > 0)
            {
                tile->pathHere();
                pathTiles->addObject(tile);
            }
        }
    }

    //now for environment //WARNING Ground_1 is now the tile layer
    //putting stuff back in Ground_0 for now
    pLayer = mapPtr->layerNamed("Ground_1");
    for (int i = 0; i < mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; ++j)
        {
            CCPoint tilePos = ccp(i, j);
            CCSprite* environment = pLayer->tileAt(tilePos);
            if (environment != NULL)
            {
                // Remove environment tile from TMXLayer, and add to map instead
                environment->retain();
                environment->removeFromParent();
                getMap()->addChild(environment, calcZIndex(tilePos));
                getTileAt(i, j)->setEnvironment(environment);
            }
        }
    }
    pLayer->setVisible(false);
    /*
    pLayer = mapPtr->layerNamed("Ground_River");
    for (int i = 0; i < mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; ++j)
        {
            CCPoint tilePos = ccp(i, j);
            CCSprite* environment = pLayer->tileAt(tilePos);
            if (environment != NULL)
            {
                // Remove environment tile from TMXLayer, and add to map instead
                environment->retain();
                environment->removeFromParent();
                
                //FUKKIN OFFSET FOR LARGE TILES
                //I ASSUME 2048x2048. anything else CHANGE.
                CCPoint worldPos = MapHandler::locationFromTilePos(&tilePos);
                worldPos.x -= 960;
                worldPos.y += 960;
                environment->setPosition(worldPos);
                
                getMap()->addChild(environment, calcZIndex(tilePos));
                getTileAt(i, j)->setEnvironment(environment);
            }
        }
    }
    
    */
  //  pLayer->setVisible(false);
    /*
    pLayer = mapPtr->layerNamed("Ground_Border");
    for (int i = 0; i < mapPtr->getMapSize().width; ++i)
    {
        for (int j = 0; j < mapPtr->getMapSize().height; ++j)
        {
            CCPoint tilePos = ccp(i, j);
            CCSprite* environment = pLayer->tileAt(tilePos);
            if (environment != NULL)
            {
                // Remove environment tile from TMXLayer, and add to map instead
                environment->retain();
                environment->removeFromParent();
                getMap()->addChild(environment, calcZIndex(tilePos));
                getTileAt(i, j)->setEnvironment(environment);
            }
        }
    }
    pLayer->setVisible(false);
    */
    
    
}


float MapHandler::calcZIndex(CCPoint &point, int offset)
{
    float lowestZ = 0;// mapPtr->getMapSize().width + mapPtr->getMapSize().height;
    float currZ = point.x + point.y + offset;
    return (lowestZ + currZ) + mapPtr->layerNamed("Ground_1")->getZOrder();
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
    if (pathTiles->count() == 0)
    {
        return CCPointMake(-1, -1);
    }
    
    // get a random map tile from all path tiles.
    int targetIdx = rand() % pathTiles->count();
    MapTile* tgtTile = (MapTile*)pathTiles->objectAtIndex(targetIdx);
    
    // if the tile is not a path, cannot go there.
    if (!tgtTile->isPath){
        return CCPointMake(-1,-1);
    }
    
    // if the tile is in combat, avoid to go there.
    if(tgtTile->isInCombat){
        return CCPointMake(-1, -1);
    }
    
    return CCPointMake(tgtTile->xpos, tgtTile->ypos);
}

bool MapHandler::Build(cocos2d::CCPoint &target, Building* building, bool skipConstruction, std::string withDetails)
{
    if (!building)
    {
        return false;
    }
    
    /*Note: do NOT use the pointer directly! The pointer points to the main instance of the building. */
    Building* cloneBuilding = (Building*) building->copy();
    if (cloneBuilding->buildingType == BUILDINGCATEGORYMAX)
    {
        return false;
    }
    
    // Don't build if tiles are occupied. EDIT: Ignore this rule if Building is a decoration, which allows it to build on OOB tiles and over each other.
    if (!isBuildableOnTile(target, cloneBuilding))
    {
        return false;
    }

    cloneBuilding->buildingRep = CCSprite::create();
    cloneBuilding->buildingRep->initWithTexture(cloneBuilding->buildingTexture, cloneBuilding->buildingRect);
    CCPoint tilePos = GameScene::getThis()->mapHandler->locationFromTilePos(&target);
    cloneBuilding->buildingRep->setPosition(tilePos);
    
    getMap()->addChild(cloneBuilding->buildingRep, calcZIndex(target, cloneBuilding->width)); //force buildings to be drawn always on top
    
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
    
    if (skipConstruction)
    {
        cloneBuilding->ID = GameScene::getThis()->buildingHandler->getHighestBuildingID() + 1; //the clone buildings will reuse the IDs as an instance tracker.
        
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
        
        GameScene::getThis()->buildingHandler->addBuildingToMap(cloneBuilding);
    }
    else
    {
        //GameHUD::getThis()->buyBuilding(cloneBuilding->buildingCost);
        //GameScene::getThis()->buildingHandler->addBuildingToMap(cloneBuilding);
        GameScene::getThis()->constructionHandler->addConstructingBuilding(cloneBuilding);
    }
    
    return true;
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
    
    CCPoint tilePos = GameScene::getThis()->mapHandler->locationFromTilePos(&target);
    
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
    getMap()->addChild(currBuildingPreview->buildingRep, calcZIndex(target, currBuildingPreview->width)); //force buildings to be drawn always on top
    
    // Show red-tinted preview and return false if tiles are occupied.
    if (!isBuildableOnTile(target, currBuildingPreview))
    {
        currBuildingPreview->buildingRep->setColor(ccc3(255, 64, 64));
        return false;
    }
    return true;
}


void MapHandler::ForceUnbuild(cocos2d::CCPoint &target)
{
    Building* targetB = getTileAt(target.x, target.y)->building;
    if (targetB == NULL){
        CCLog("Building is null leh");
        return;
    }
    
    // Try remove from constructionHandler
    GameScene::getThis()->constructionHandler->removeConstructingBuilding(targetB);
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(targetB->buildingRep, true);
    GameScene::getThis()->buildingHandler->removeBuildingFromMap(targetB);
    
    for (int i = 0; i < targetB->height; i++)
        for (int j = 0; j < targetB->width; j++)
            getTileAt(target.x + j, target.y + i)->UnBuild();

}


void MapHandler::UnBuild(cocos2d::CCPoint &target)
{
    Building* targetB = getTileAt(target.x, target.y)->building;
    if (targetB == NULL){
        CCLog("Building is null leh");
        return;
    }
    
    int targetID = targetB->ID;
    for (int i = 0; i < GameScene::getThis()->buildingHandler->specialOnMap->count(); ++i)
    {
        Building* b = (Building*)GameScene::getThis()->buildingHandler->specialOnMap->objectAtIndex(i);
        if (b->ID == targetID)
        {
            CCLog("SPECIALS cannot be destroyed");
            return;
        }
    }
    
    // Try remove from constructionHandler
    GameScene::getThis()->constructionHandler->removeConstructingBuilding(targetB);
    
    GameScene::getThis()->mapHandler->getMap()->removeChild(targetB->buildingRep, true);
    GameScene::getThis()->buildingHandler->removeBuildingFromMap(targetB);
    
    
    for (int i = 0; i < targetB->height; i++)
        for (int j = 0; j < targetB->width; j++)
            getTileAt(target.x + j, target.y + i)->UnBuild();
    
    targetB->buildingTexture->release();
    
}

void MapHandler::UnBuildPreview()
{
    if (currBuildingPreview == NULL){
        return;
    }
    
    getMap()->removeChild(previewTileHighlight, true);
    getMap()->removeChild(currBuildingPreview->buildingRep, true);
    
    //currBuildingPreview->buildingRep->release();
   // currBuildingPreview->buildingRep = NULL;
   // delete currBuildingPreview;
//    previewTileHighlight->release();
    delete previewTileHighlight;
    previewTileHighlight = NULL;
    currBuildingPreview = NULL;
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
    
    int total = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (tiles[i] != N)
        {
            switch (i)
            {
                case 0: total += 1;
                    break;
                case 1: total += 2;
                    break;
                case 2: total += 4;
                    break;
                case 3: total += 8;
                    break;
                default:
                    break;
            }
        }
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
    if (pathPreview == NULL)
        return;
    
    getMap()->removeChild(pathPreview, true);
    getMap()->removeChild(pathPreviewTileHighlight, true);
    pathPreview = NULL;
    pathPreviewTileHighlight = NULL;
}

void MapHandler::UnPathPreviewLineExtend()
{
    if (pathPreview == NULL)
        return;
    
    pathPreview->setVertexAt(0, 0, 0);
    pathPreview->setVertexAt(1, -mapPtr->getTileSize().width / 2.0f, -mapPtr->getTileSize().height / 2.0f);
    pathPreview->setVertexAt(2, 0, -mapPtr->getTileSize().height);
    pathPreview->setVertexAt(3, mapPtr->getTileSize().width / 2.0f, -mapPtr->getTileSize().height / 2.0f);
}

void MapHandler::update(float dt)
{
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    
    for(int i = 0; i < combatTiles->count(); i++)
    {
        MapTile* tile = (MapTile*) combatTiles->objectAtIndex(i);
        tile->isInCombat = false;
    }
    
    combatTiles->removeAllObjects();
    
    for(int i = 0; i < spritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*) spritesOnMap->objectAtIndex(i);
        
        if(gs->combatState == C_COMBAT)
        {
            gs->currTile->isInCombat = true;
            combatTiles->addObject(gs->currTile);
        }
    }
}









