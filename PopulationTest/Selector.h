//
//  Selector.h
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#ifndef __PopulationTest__Selector__
#define __PopulationTest__Selector__

#include "Composite.h"

class Selector : public Composite
{
public:
    virtual void onInitialize();

    
    virtual Status update();
    
    
    Behaviors::iterator m_CurrentChild;
    
    virtual ~Selector();
    
};

#endif /* defined(__PopulationTest__Selector__) */
