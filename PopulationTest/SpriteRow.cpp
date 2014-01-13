//
//  SpriteRow.cpp
//  PopulationTest
//
//  Created by admin on 13/1/14.
//
//

#include "SpriteRow.h"

SpriteRow::SpriteRow(GameSprite* gs, ScrollArea* sa, int ind)
{
    this->gameSprite = gs;
    this->scrollArea = sa;
    this->index = ind;
    init();
}

SpriteRow::~SpriteRow()
{
    delete villagerImage;
}

SpriteRow* SpriteRow::create(GameSprite* gs, ScrollArea* sa, int ind)
{
    SpriteRow *pRet = new SpriteRow(gs, sa, ind);
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

void SpriteRow::init()
{
    villagerImage = CCSprite::create( gameSprite->spriteName.append("_port.png").c_str() );
    villagerImage->setScale(64.0f / villagerImage->boundingBox().size.width);
    villagerImage->setAnchorPoint(ccp(0, 1));
    scrollArea->addItem(villagerImage, ccp(20.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
    
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    
    std::stringstream ss;
    
    ss << gameSprite->spriteName;
    
    villagerNameLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerNameLabel->setColor(colorGreen);
    villagerNameLabel->setAnchorPoint(ccp(0, 1));
    scrollArea->addItem(villagerNameLabel, ccp(20.0f + villagerImage->boundingBox().size.width + 10.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
}

void SpriteRow::clear(){
    
}