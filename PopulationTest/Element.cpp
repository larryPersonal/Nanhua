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
    
    fadeIn = false;
    fadeOut = false;
    
    outcome = false;
    banditsModifier = 0;
    refugeeModifier = 0;
    foodModifier = 0;
    goldModifier = 0;
    populationModifier = 0;
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


