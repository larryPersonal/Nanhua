//
//  GlobalHelper.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 6/5/13.
//
//

#include "GlobalHelper.h"
#include <sstream>
#include <vector>
#include "GameScene.h"

string GlobalHelper::stringToUpper(string str)
{
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

vector<string>& GlobalHelper::splitString(const std::string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string GlobalHelper::stringRemoveDelim(string str, char delim)
{
    str.erase(std::remove (str.begin(), str.end(), delim), str.end());
    return str;
}

void GlobalHelper::drawSolidCircle(const CCPoint& center, float radius, unsigned int segments,
                                   float scaleX, float scaleY, ccColor4F color)
{
    const float coef = 2.0f * (float)M_PI/segments;
    
    std::vector<CCPoint> points(0);
    points.push_back(center);
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads) * scaleX + center.x;
        GLfloat k = radius * sinf(rads) * scaleY + center.y;
        
        points.push_back(ccp(j, k));
    }
    
    ccDrawSolidPoly(&points[0], (GLsizei)points.size(), color);
}

float GlobalHelper::vectorDotProduct(const CCPoint& vec1, const CCPoint& vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

void GlobalHelper::vectorMultiplication(const CCPoint& vec, float scalar, CCPoint& result)
{
    result.x = vec.x * scalar;
    result.y = vec.y * scalar;
}

void GlobalHelper::vectorProjection(CCPoint& vec, const CCPoint& onto)
{
    vectorMultiplication(onto, (vectorDotProduct(vec, onto) / vectorDotProduct(onto, onto)), vec);
}



std::string GlobalHelper::Convert(float number)
{
    std::ostringstream buffer;
    buffer << number;
    return buffer.str();
}

std::string GlobalHelper::Convert(int number)
{
    std::ostringstream buffer;
    buffer << number;
    return buffer.str();
}

std::string GlobalHelper::getActionString(SpriteAction sa)
{
    if(sa == IDLE)
    {
        return "IDLE";
    }
    else if(sa == WALKING)
    {
        return "Walking";
    }
    else if(sa == CARRYING)
    {
        return "Carrying";
    }
    else if(sa == FIGHTING)
    {
        return "Fighting";
    }
    else if(sa == ESCAPING)
    {
        return "Escaping";
    }
    else if(sa == EATING)
    {
        return "Eating";
    }
    else if(sa == STORING)
    {
        return "Storing";
    }
    else if(sa == FARMING)
    {
        return "Farming";
    }
    else if(sa == BUILD)
    {
        return "Build";
    }
    else if(sa == RESTING)
    {
        return "Resting";
    }
    else if(sa == ACTION_END)
    {
        return "END";
    }
    else
    {
        return "IDLE";
    }
}

bool GlobalHelper::isHorizontal()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    return screenSize.width > screenSize.height;
}

GameSprite* GlobalHelper::getSpriteType(GameSprite* gameSprite, SpriteType mst, SpriteType fst)
{
    CCArray* allSprites = GameScene::getThis()->spriteHandler->allSprites;
    bool isMale = (gameSprite->gender == 'm');
    for (int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* sprite = (GameSprite*)allSprites->objectAtIndex(i);
        if(isMale)
        {
            if(sprite->type == mst)
            {
                return sprite;
            }
        }
        else
        {
            if(sprite->type == fst)
            {
                return sprite;
            }
        }
    }
    return NULL;
}







