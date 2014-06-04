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
    
    textOffY = 0;
    
    limitX = -1;
}

Element::~Element(){
    
}

Element* Element::create(){
    Element *pRet = new Element();
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
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


