//
//  Senario.cpp
//  PopulationTest
//
//  Created by admin on 12/12/13.
//
//

#include "Senario.h"
#include "GameScene.h"
#include "Slide.h"

Senario* Senario::SP;

Senario::Senario()
{
    Senario::SP = this;
    active = false;
    init();
    curSlide = 0;
}

Senario::~Senario()
{
    Senario::SP = NULL;
    spriteList.clear();
    menuList.clear();
    
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

void Senario::playSenario(char* senario)
{
    readSenarioFile(senario);
    createGUI();
}

void Senario::readSenarioFile(char *senario)
{
    curSlide = 0;
    SenarioManager* sm = new SenarioManager();
    sm->parseXMLFile("sessionT_start.xml");
    
    slidesList = sm->slidesList;
    
    constructSenarioStage();
}

void Senario::constructSenarioStage()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    clearElements();
    
    if(slidesList->count() <= curSlide)
    {
        return;
    }
    
    Slide* slide = (Slide*)slidesList->objectAtIndex(curSlide);
    
    CCArray* elementArray = slide->elementList;
    
    for(int i = 0; i < elementArray->count(); i++)
    {
        Element* ele = (Element*)elementArray->objectAtIndex(i);
        
        if(ele->type == Element::sprite)
        {
            //CCSprite* cha = CCSprite::create(ele->src.c_str(), CCRectMake(0, 0, screenSize.width * ele->width / 100.0f, screenSize.height * ele->height / 100.0f));
            
            CCSprite* cha = CCSprite::create(ele->src.c_str());
            CCSize spriteSize = cha->getContentSize();
            cha->setScaleX(screenSize.width / spriteSize.width * ele->width / 100.0f);
            cha->setScaleY(screenSize.height / spriteSize.height * ele->height / 100.0f);
            
            cha->setPosition(ccp(screenSize.width * (ele->left / 100.0f + ele->width / 200.0f), screenSize.height * (ele->top / 100.0f + ele->height / 200.0f)));
            this->addChild(cha);
            spriteList.push_back(cha);
        }
        else if(ele->type == Element::dialogue)
        {
            CCMenuItem* chbox = CCMenuItemImage::create(ele->src.c_str(), ele->src.c_str(), this, menu_selector(Senario::test));
            CCSize boxSize = chbox->getContentSize();
            chbox->setScaleX(screenSize.width / boxSize.width * ele->width / 100.0f);
            chbox->setScaleY(screenSize.height / boxSize.height * ele->height / 100.0f);
            
            CCLabelTTF* chboxLabel = CCLabelTTF::create(ele->text.c_str(), "MBBlockType", ele->fontSize, chbox->boundingBox().size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            chboxLabel->setPosition(ccp(chbox->boundingBox().size.width, chbox->boundingBox().size.height));
            chbox->addChild(chboxLabel);
            //chboxLabel->setFontName(ele->font.c_str());
            
            chbox->setPosition(ccp(screenSize.width * (ele->left / 100.0f + ele->width / 200.0f), screenSize.height * (ele->top / 100.0f + ele->height / 200.0f)));
            
            CCMenu* menu = CCMenu::create(chbox, NULL);
            this->addChild(menu, 1);
            
            menuList.push_back(menu);
        }
    }
}

void Senario::test()
{
    curSlide++;
    //clearElements();
    constructSenarioStage();
}

void Senario::clearElements()
{
    for(int i = 0; i < spriteList.size(); i++)
    {
        this->removeChild(spriteList.at(i));
    }
    
    for(int i = 0; i < menuList.size(); i++)
    {
        this->removeChild(menuList.at(i));
    }
    
    spriteList.clear();
    menuList.clear();
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
