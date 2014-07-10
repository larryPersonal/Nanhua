//
//  GlobalHelper.h
//  PopulationTest
//
//  Created by Ong Heng Le on 6/5/13.
//
//

#ifndef __PopulationTest__GlobalHelper__
#define __PopulationTest__GlobalHelper__

#include <string>
using namespace std;

#include "SpriteHandler.h"
#include "cocos2d.h"
#include <wchar.h>
using namespace cocos2d;

class GlobalHelper
{
public:
    
    // String funcs
    static string stringToUpper(string str);
    static vector<string>& splitString(const std::string &s, char delim, vector<string> &elems);
    static string stringRemoveDelim(string str, char delim);
    
    // GL Draw funcs
    static void drawSolidCircle(const CCPoint& center, float radius, unsigned int segments,
                                float scaleX, float scaleY, ccColor4F color);
    
    // Vector funcs
    static float vectorDotProduct(const CCPoint& vec1, const CCPoint& vec2);
    static void vectorMultiplication(const CCPoint& vec, float scalar, CCPoint& result);
    static void vectorProjection(CCPoint& vec, const CCPoint& onto);
    
    static std::string Convert (float number);
    static std::string Convert (int number);
    
    static std::string getActionString(SpriteAction);
    
    static bool isHorizontal();
    
    static SpriteClass* getSpriteClassByVillagerClass(VillagerClass villagerClass);
    static GameSprite* getSpriteByVillagerClass(GameSprite*, VillagerClass);
    
    static vector<std::string> split(std::string, char);
    static vector<wchar_t*> split(const char*);
    static bool compareCCPoint(CCPoint, CCPoint);
    
    static float getDirectDistance(CCPoint startPos, CCPoint endPos);
    
    /* mbcs conversion */
    static wchar_t * MBCS2Unicode(wchar_t * buff, const char * str);
    static char * Unicode2MBCS(char * buff, const wchar_t * str);
    static wstring str2wstr(string str);
    static string wstr2str(wstring wstr);
    static int wputs(wstring wstr);
    static int wputs(const wchar_t * wstr);
    
    static std::wstring stringToWstring( std::string str );
    static std::string WstringTostring( std::wstring wStr );
    
    /* clear cache function */
    static void clearCache();
    static void clearPreloadedTexture();
    
    /* resume villagers */
    static void resumeAllVillagers();
};

#endif /* defined(__PopulationTest__GlobalHelper__) */
