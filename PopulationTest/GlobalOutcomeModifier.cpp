//
//  GlobalOutcomeModifier.cpp
//  PopulationTest
//
//  Created by admin on 3/4/14.
//
//

#include "GlobalOutcomeModifier.h"

GlobalOutcomeModifier* GlobalOutcomeModifier::SP;

GlobalOutcomeModifier::GlobalOutcomeModifier()
{
    GlobalOutcomeModifier::SP = this;
    banditsModifier = 0;
    refugeesModifier = 0;
    goldModifier = 0;
    foodModifier = 0;
    populationModifier = 0;
}

GlobalOutcomeModifier::~GlobalOutcomeModifier()
{
    GlobalOutcomeModifier::SP = NULL;
}

GlobalOutcomeModifier* GlobalOutcomeModifier::getThis()
{
    return SP;
}