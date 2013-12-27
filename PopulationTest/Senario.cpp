//
//  Senario.cpp
//  PopulationTest
//
//  Created by admin on 12/12/13.
//
//

#include "Senario.h"
#include "GameScene.h"

Senario* Senario::SP;

Senario::Senario()
{
    Senario::SP = this;
    active = false;
    init();
}

Senario::~Senario()
{
    Senario::SP = NULL;
}

Senario* Senario::getThis()
{
    return SP;
}

Senario* Senario::create()
{
    Senario* sPtr = new Senario();
    if (sPtr && sPtr->init())
    {
        sPtr->autorelease();
        return sPtr;
    }
    else
    {
        CC_SAFE_DELETE(sPtr);
        return NULL;
    }
}

bool Senario::init()
{
    return true;
}

void Senario::playSenario(const char* senario)
{
    readSenarioFile(senario);
    createGUI();
}

void Senario::readSenarioFile(const char *senario)
{
    
}

void Senario::createGUI(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    character = CCSprite::create("exit_pressed.png", CCRectMake(0, 0, 125, 154) );
    character->setPosition( ccp(character->getContentSize().width/2, screenSize.height/2) );
    this->addChild(character);
    
    chatbox = CCMenuItemImage::create("text_box.jpg", "text_box.jpg", this, menu_selector(Senario::buttonSelect));
    chatbox->setScale(0.5f);
    
    CCLabelTTF* chatboxLabel = CCLabelTTF::create("Hello World!", "MBBlockType" ,64, chatbox->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    // chatboxLabel->setAnchorPoint(ccp(0.5f, 0.5));
    chatboxLabel->setPosition( ccp(chatbox->boundingBox().size.width, chatbox->boundingBox().size.height));
    chatbox->addChild(chatboxLabel);
    chatboxLabel->setColor(ccc3(255,189,68));
    
    CCMenu* menu = CCMenu::create(chatbox, NULL);
    menu->setAnchorPoint(ccp(0.5f, 0.5f));
    menu->setPosition(screenSize.width * 0.5, screenSize.height * 0.6);
    this->addChild(menu, 1);
}

void Senario::buttonSelect()
{
    if(active)
    {
        active = false;
        this->setVisible(false);
        GameScene::getThis()->enableTouch();
    }
}
