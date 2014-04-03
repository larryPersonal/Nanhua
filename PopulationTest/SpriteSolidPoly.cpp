//
//  SpriteSolidPoly.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 14/5/13.
//
//

#include "SpriteSolidPoly.h"

SpriteSolidPoly::SpriteSolidPoly()
{
    color = ccc4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void SpriteSolidPoly::draw()
{
    CCNode::draw();
    
    if (points.size() > 0)
        ccDrawSolidPoly(&points[0], points.size(), color);
}

void SpriteSolidPoly::addVertex(CCPoint point)
{
    points.push_back(point);
}

void SpriteSolidPoly::setVertexAt(int index, float x, float y)
{
    if (index < 0 || index >= points.size())
        return;
    
    points.at(index).setPoint(x, y);
}

void SpriteSolidPoly::setColor(ccColor4F color)
{
    this->color = color;
}