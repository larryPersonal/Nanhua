//
//  AnimatedRain.cpp
//  PopulationTest
//
//  Created by GAO YU on 27/4/14.
//
//

#include "AnimatedRain.h"
#include "GameScene.h"

AnimatedRain* AnimatedRain::create()
{
    AnimatedRain* ar = new AnimatedRain;
    if(ar)
    {
        ar->retain();
        return ar;
    }
    else
    {
        CC_SAFE_DELETE(ar);
        return NULL;
    }
}

AnimatedRain::AnimatedRain()
{
    frameWidth = 164;
    frameHeight = 164;
    
    frameNo = 0;
    maxFrameNo = 2;
    
    xOffset = 0;
    yOffset = 0;
    
    delay_offset = 10.0f;
    delay = 10.0f;
    
    movementSpeed = 0.1f;
    
    nextPos = CCPointZero;
    
    isMoving = false;
    isInAction = false;
    
    rainTexture = CCTextureCache::sharedTextureCache()->addImage("raining.png");
    rainRect = CCRectMake(0, 0, frameWidth, frameHeight);
    
    rainSprite = CCSprite::createWithTexture(rainTexture, rainRect);
    rainSprite->setScale(1.0f);
    rainSprite->setAnchorPoint(ccp(0.5, 0.5));
    
    CCPoint rainPos = CCPointMake(30, 23);
    rainPos = GameScene::getThis()->mapHandler->locationFromTilePos(&rainPos);
    
    GameScene::getThis()->mapHandler->getMap()->addChild(rainSprite, 999999);
    rainSprite->setPosition(rainPos);
    
    path = CCArray::create();
    path->retain();
}

AnimatedRain::~AnimatedRain()
{
    path->removeAllObjects();
    CC_SAFE_RELEASE(path);
    
    CC_SAFE_RELEASE(this);
}

void AnimatedRain::update(float deltaTime)
{
    // change appearance
    if(delay > 0)
    {
        delay -= deltaTime;
    }
    else
    {
        frameNo++;
        if(frameNo >= maxFrameNo)
        {
            frameNo = 0;
        }
        
        xOffset = 0;
        yOffset = frameNo;
        
        rainRect.setRect(xOffset * frameWidth, yOffset * frameHeight, frameWidth, frameHeight);
        rainSprite->setTextureRect(rainRect);
        
        delay = delay_offset;
    }
    
    // move the animated rain
    if(!isMoving)
    {
        CCPoint mapTile = GameScene::getThis()->mapHandler->getRandomTile();
        if(mapTile.x == -1 || mapTile.y == -1)
        {
            return;
        }
        
        CCPoint startPos = getWorldPosition();
        startPos = GameScene::getThis()->mapHandler->tilePosFromLocation(startPos);
        
        if (CreatePath(startPos, mapTile))
        {
            isMoving = true;
        }
        else
        {
            isMoving = false;
        }
    }
    
    if(isMoving)
    {
        if(!isInAction)
        {
            if(path->count() > 0)
            {
                PathfindingNode* node = (PathfindingNode*)path->objectAtIndex(0);
                path->removeObject(node);
                
                nextPos = GameScene::getThis()->mapHandler->locationFromTilePos(&(node->tilepos));
                
                isInAction = true;
            }
            else
            {
                isMoving = false;
            }
        }
    }
    
    move(deltaTime);
}

void AnimatedRain::move(float deltaTime)
{
    if(!isInAction)
    {
        return;
    }
    
    CCPoint thisPos = rainSprite->getPosition();
    
    float xDiff = nextPos.x - thisPos.x;
    float yDiff = nextPos.y - thisPos.y;
    
    float xSpeed = 0;
    float ySpeed = 0;
    
    bool stop = false;
    
    if(xDiff > 0)
    {
        xSpeed = movementSpeed;
        ySpeed = xSpeed / xDiff * yDiff;
        
        if(xSpeed > xDiff)
        {
            xSpeed = xDiff;
            ySpeed = yDiff;
            stop = true;
        }
    }
    else if(xDiff < 0)
    {
        xSpeed = -movementSpeed;
        ySpeed = xSpeed / xDiff * yDiff;
        
        if(xSpeed < xDiff)
        {
            xSpeed = xDiff;
            ySpeed = yDiff;
            stop = true;
        }
    }
    else
    {
        xSpeed = xDiff;
        ySpeed = yDiff;
        stop = true;
    }
    
    rainSprite->setPosition(ccp(thisPos.x + xSpeed, thisPos.y + ySpeed));
    
    if(stop)
    {
        isInAction = false;
    }
}

CCPoint AnimatedRain::getWorldPosition()
{
    if (!rainSprite){
        return CCPointMake(-1,-1);
    }
    
    return ccpAdd(rainSprite->getPosition(), GameScene::getThis()->mapHandler->getMap()->getPosition());
}

bool AnimatedRain::CreatePath(CCPoint from, CCPoint to)
{
    if (from.equals(to))
    {
        isMoving = false;
        return false;
    }
    
    PathFinder *p = new PathFinder();
    p->setDestination(to);
    p->setSource(from);
    if (path != NULL)
    {
        if (path->count() > 0)
        {
            path->removeAllObjects();
            path->release();
            path = NULL;
        }
    }
    
    path = p->makeRainPath(&from, &to);
    
    path->retain();
    
    delete p;
    return true;
}
