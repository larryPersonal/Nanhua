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
        sprite, dialogue, option
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
    
    std::string nextFile;
    
    bool isBackground;
    
    bool fadeIn;
    bool fadeOut;
    
    int banditsModifier;
    int refugeeModifier;
    int goldModifier;
    int foodModifier;
    int populationModifier;
    bool outcome;
    
public:
    Element();
    ~Element();
    
    void updateSpriteType();
    void updateDialogueType();
    void updateOptionType();
    
};


#endif
