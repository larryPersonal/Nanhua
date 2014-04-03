//
//  Sequence.h
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#ifndef __PopulationTest__Sequence__
#define __PopulationTest__Sequence__


#include "Composite.h"
class Sequence : public Composite
{
public:
    
    virtual void onInitialize();
    
    virtual Status update();
    
    
    Behaviors::iterator m_CurrentChild;
    
    virtual ~Sequence();
};


#endif /* defined(__PopulationTest__Sequence__) */
