//
//  Narrator.cpp
//  PopulationTest
//
//  Created by admin on 2/4/14.
//
//

#include "Narrator.h"
#include "TutorialManager.h"
#include "GlobalHelper.h"
#include "AnimatedString.h"
#include "GameScene.h"
#include "GameHUD.h"
#include "Senario.h"

Narrator::Narrator()
{
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    textBackground = CCSprite::create("popup_dialogbox.png");
    textBackground->setScale(screenSize.width / textBackground->boundingBox().size.width * 0.75f);
    textBackground->setAnchorPoint(ccp(0.5, 0.5));
    textBackground->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    TutorialManager::getThis()->addChild(textBackground);
    
    startX = (screenSize.width - textBackground->boundingBox().size.width) / 2.0f + 110.0f;
    startY = (screenSize.height + textBackground->boundingBox().size.height) / 2.0f - 160.0f;
    
    offX = 0;
    offY = 0;
    
    ns = NS_START;
}

Narrator::~Narrator()
{
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
    
    TutorialManager::getThis()->removeChild(textBackground);
}

void Narrator::display()
{
    offX = 0;
    offY = 0;
    
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        TutorialManager::getThis()->removeChild(as->getLabel());
    }
    animatedStringList->removeAllObjects();
    ns = (Narrator_State)((int)ns + 1);
    
    string str = "";
    
    bool goDisplay = true;
    switch (ns)
    {
        case C1T1:
            str = "A group of refugees come to our village!";
            TutorialManager::getThis()->scheduleFadeIn(160.0f, 2);
            TutorialManager::getThis()->active = true;
            GameHUD::getThis()->pause = true;
            break;
        case C1T2:
            goDisplay = false;
            TutorialManager::getThis()->inDisplay = false;
            textBackground->setVisible(false);
            TutorialManager::getThis()->scheduleFadeIn(0.0f, 2);
            TutorialManager::getThis()->active = false;
            setupForScenario();
            break;
        default:
            str = "end";
            break;
    }
    
    if(goDisplay)
    {
        TutorialManager::getThis()->inText = true;
        ccColor3B color = ccc3(255, 255, 255);
        
        vector<std::string> tokens = GlobalHelper::split(str, ' ');
        
        float flashTimeGap = 0.05f;
        int flashGapCount = 0;
        
        for (int i = 0; i < tokens.size(); i++)
        {
            std::string tokenStr = tokens.at(i);
            CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), "TooneyLoons", 36);
            tempLabel->retain();
            
            if (startX + offX + tempLabel->boundingBox().size.width > 860.0f)
            {
                offY = offY + 30.0f;
                offX = 0;
            }
            
            CC_SAFE_RELEASE(tempLabel);
            
            for (int j = 0; j < tokenStr.length(); j++)
            {
                string tempStr = tokenStr.substr(j, 1);
                AnimatedString* as = AnimatedString::create(tempStr, flashTimeGap * (j + flashGapCount), "TooneyLoons", 36, 80.0f);
                as->getLabel()->setColor(color);
                as->getLabel()->setAnchorPoint(ccp(0, 1));
                
                as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
                offX += as->label->boundingBox().size.width;
                
                TutorialManager::getThis()->addChild(as->getLabel(), 20);
                animatedStringList->addObject(as);
            }
            
            flashGapCount += tokenStr.size();
            offX += 10;
        }
        
        TutorialManager::getThis()->schedule( schedule_selector( Narrator::update ), 1.0f / 120.0f );
        //text->setString(ss.str().c_str());
    }
}

void Narrator::finishDisplay()
{
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        as->getLabel()->setOpacity((GLubyte) 255);
    }
}

void Narrator::update(float dt)
{
    bool end = true;
    CCArray* list = TutorialManager::getThis()->narrator->animatedStringList;
    for(int i = 0; i < list->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) list->objectAtIndex(i);
        as->update(dt);
        if(((float)as->getLabel()->getOpacity()) < 255)
        {
            end = false;
        }
    }
    
    if(end)
    {
        TutorialManager::getThis()->unschedule( schedule_selector( Narrator::update ) );
        TutorialManager::getThis()->inText = false;
    }
}

void Narrator::activateSprite(float dt)
{
    CCArray* spritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    cumulativeTime += dt;
    if(cumulativeTime >= 6.0f)
    {
        TutorialManager::getThis()->unschedule(schedule_selector(Narrator::activateSprite));
    }
    else if(cumulativeTime >= 5.0f)
    {
        GameSprite* sp = (GameSprite*) spritesOnMap->objectAtIndex(4);
        CCPoint tilePos = CCPointMake(29,33);
        CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(&tilePos);
        
        sp->spriteRep->setPosition(target);
    }
    else if(cumulativeTime >= 4.0f)
    {
        GameSprite* sp = (GameSprite*) spritesOnMap->objectAtIndex(3);
        CCPoint tilePos = CCPointMake(29,33);
        CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(&tilePos);
        
        sp->spriteRep->setPosition(target);
    }
    else if(cumulativeTime >= 3.0f)
    {
        GameSprite* sp = (GameSprite*) spritesOnMap->objectAtIndex(2);
        CCPoint tilePos = CCPointMake(29,33);
        CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(&tilePos);
        
        sp->spriteRep->setPosition(target);
    }
    else if(cumulativeTime >= 2.0f)
    {
        GameSprite* sp = (GameSprite*) spritesOnMap->objectAtIndex(1);
        CCPoint tilePos = CCPointMake(29,33);
        CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(&tilePos);
        
        sp->spriteRep->setPosition(target);
    }
    else if(cumulativeTime >= 1.0f)
    {
        GameSprite* sp = (GameSprite*) spritesOnMap->objectAtIndex(0);
        CCPoint tilePos = CCPointMake(29,33);
        CCPoint target = GameScene::getThis()->mapHandler->locationFromTilePos(&tilePos);
        
        sp->spriteRep->setPosition(target);
    }
}

void Narrator::setupForScenario()
{
    TutorialManager::getThis()->unlockAll();
    GameHUD::getThis()->pause = true;
    std::string filename = "senario_h.xml";
    Senario::getThis()->playSenario(filename.c_str());
    TutorialManager::getThis()->clearSprites();
}
