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
};

#endif /* defined(__PopulationTest__GlobalHelper__) */
