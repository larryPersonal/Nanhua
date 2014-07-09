//
//  RandomEvent.cpp
//  PopulationTest
//
//  Created by admin on 9/7/14.
//
//

#include "RandomEvent.h"

RandomEvent* RandomEvent::create(){
    RandomEvent* pRet = new RandomEvent();
    if(pRet)
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

RandomEvent::RandomEvent()
{
    event_id = 0;
    event_name = "";
    event_image = "";
    event_description = "";
}

RandomEvent::~RandomEvent()
{
    
}