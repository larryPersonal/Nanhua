//
//  Selector.cpp
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#include "Selector.h"

Selector::~Selector()
{
  /*
    for (int i = 0; i < m_Children.size(); ++i)
    {
        delete m_Children[i];
    }
    */
}


void Selector::onInitialize()
{
   // CCLog("selector init");
    for (int i = 0; i < m_Children.size(); ++i)
    {
        m_Children[i]->onInitialize();
    }
    m_CurrentChild = m_Children.begin();
}

Status Selector::update()
{
    while (true)
    {
        Status s = (*m_CurrentChild)->tick();
        
        if (s != BH_FAILURE) {
            
            return s;
        }
        
        if(++m_CurrentChild == m_Children.end())
        {
            return BH_FAILURE;
        }
    }
   return BH_INVALID;
}


