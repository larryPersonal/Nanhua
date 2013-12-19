//
//  SpriteSolidCircle.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 14/5/13.
//
//

#include "SpriteSolidCircle.h"
#include "GlobalHelper.h"

SpriteSolidCircle::SpriteSolidCircle()
{
    radius = 10.0f;
    scaleX = 1.0f;
    scaleY = 1.0f;
    color = ccc4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void SpriteSolidCircle::draw()
{
    CCNode::draw();
    
    if (radius <= 0.0f ||
        scaleX <= 0.0f ||
        scaleY <= 0.0f)
        return;
    
    GlobalHelper::drawSolidCircle(CCPointZero, radius, radius/2, scaleX, scaleY, color);
}

void SpriteSolidCircle::setRadius(float radius)
{
    this->radius = radius;
}

void SpriteSolidCircle::setScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}

void SpriteSolidCircle::setColor(ccColor4F color)
{
    this->color = color;
}