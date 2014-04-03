//
//  Composite.h
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#ifndef __PopulationTest__Composite__
#define __PopulationTest__Composite__

#include "Behavior.h"

class Composite : public Behavior
{
public:
    typedef std::vector <Behavior*> Behaviors;
    Behaviors m_Children;
    virtual void onTerminate(Status);
    virtual void onInitialize();
    
    virtual ~Composite()
    {
        
        for (int i = 0; i < m_Children.size(); ++i)
        {
            delete m_Children[i];
        }
        
    }
};

#endif /* defined(__PopulationTest__Composite__) */


