//
//  Slide.cpp
//  PopulationTest
//
//  Created by admin on 23/12/13.
//
//

#include "Slide.h"
#include "Element.h"

Slide::Slide(){
    elementList = CCArray::create();
    elementList->retain();
    
    isScene = false;
    scene_src = "";
    
    hasVideo = false;
    video_clip = "";
    
    clickToNext = false;
}

Slide::~Slide(){
    elementList->removeAllObjects();
    elementList->release();
}

Slide* Slide::create(){
    Slide *pRet = new Slide();
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

Slide* Slide::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    Slide* pCopy = NULL;
    
    if(pZone)
    {
        //in case of being called at sub class
        pCopy = (Slide*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = Slide::create();
        
        pNewZone = new CCZone(pCopy);
    }
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void Slide::addElement(Element* elePtr)
{
    elementList->addObject(elePtr);
    elePtr->release();
}

CCObject* Slide::getElement(int index)
{
    return elementList->objectAtIndex(index);
}
