//
//  Sequence.cpp
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#include "Sequence.h"

Sequence::~Sequence()
{
  /*
    for (int i = 0; i < m_Children.size(); ++i)
    {
        delete m_Children[i];
    }
    */
}

void Sequence::onInitialize()
{
    for (int i = 0; i < m_Children.size(); ++i)
    {
        m_Children[i]->onInitialize();
    }
    m_CurrentChild = m_Children.begin();

}

Status Sequence::update()
{
   while (true)
   {
       if (m_CurrentChild == m_Children.end()) return BH_SUCCESS;
       
       Status s = (*m_CurrentChild)->tick();
        if (s != BH_SUCCESS)
        {
            return s;
        }
       
       ++m_CurrentChild;

   }
   return BH_INVALID;
}