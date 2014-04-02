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

SpriteClass* GlobalHelper::getSpriteClassByVillagerClass(VillagerClass villagerClass)
{
    CCArray* allSpriteClass = GameScene::getThis()->spriteHandler->allSpriteClass;
    for (int i = 0; i < allSpriteClass->count(); i++)
    {
        SpriteClass* sc = (SpriteClass*) allSpriteClass->objectAtIndex(i);
        if(sc->villagerClass == villagerClass)
        {
            return sc;
        }
    }
    return NULL;
}

GameSprite* GlobalHelper::getSpriteByVillagerClass(GameSprite* gameSprite, VillagerClass villagerClass)
{
    CCArray* allSprites = GameScene::getThis()->spriteHandler->allSprites;
    bool isMale = (gameSprite->gender == 'm');
    for (int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* gs = (GameSprite*) allSprites->objectAtIndex(i);
        if (gs->villagerClass == villagerClass && (isMale == (gs->gender == 'm')))
        {
            return gs;
        }
    }
    return NULL;
}

vector<std::string> GlobalHelper::split(std::string text, char delimiter)
{
    int startIndex = -1;
    int endIndex = -1;
    vector<std::string> tokens;
    stringstream ss;
    
    for (int i = 0; i < text.length(); i++)
    {
        if(text[i] == delimiter)
        {
            if(startIndex < 0)
            {
            }
            else if(startIndex == endIndex)
            {
                ss.str(string());
                ss << text[startIndex];
                tokens.push_back(ss.str());
            }
            else
            {
                ss.str(string());
                for (int j = startIndex; j <= endIndex; j++)
                {
                    ss << text[j];
                }
                tokens.push_back(ss.str());
            }
            startIndex = -1;
            endIndex = -1;
        }
        else
        {
            if(startIndex < 0)
            {
                startIndex = i;
                endIndex = i;
            }
            else
            {
                endIndex = i;
            }
        }
    }
    
    if(startIndex >= 0 && endIndex >= startIndex)
    {
        ss.str(string());
        for(int i = startIndex; i <= endIndex; i++)
        {
            ss << text[i];
        }
        tokens.push_back(ss.str());
    }
    
    return tokens;
}







