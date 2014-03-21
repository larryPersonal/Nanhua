//
//  Element.cpp
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#include "Element.h"

Element::Element(){
    isBackground = false;
}

Element::~Element(){
    
}

void Element::updateSpriteType()
{
    type = sprite;
}

void Element::updateDialogueType()
{
    type = dialogue;
}

void Element::updateOptionType()
{
    type = option;
}


