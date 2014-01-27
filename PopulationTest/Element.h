//
//  Element.h
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#ifndef PopulationTest_Element_h
#define PopulationTest_Element_h

#include "cocos2d.h"
using namespace cocos2d;

class Element: public CCObject {
    
public:
    
    enum ElementType {
        sprite, dialogue
    };
    
    ElementType type;
    
    float width;
    float left;
    float top;
    
    std::string src;
    std::string name;
    std::string font;
    int fontSize;
    std::string text;
    
    std::string dir;
    
public:
    Element();
    ~Element();
    
    void updateSpriteType();
    void updateDialogueType();
    
};


#endif
