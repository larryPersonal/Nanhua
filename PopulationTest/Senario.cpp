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
#include "GlobalHelper.h"
#include "AnimatedString.h"

Senario* Senario::SP;

Senario::Senario()
{
    Senario::SP = this;
    active = false;
    inOption = false;
    init();
    curSlide = 0;
    slidesList = CCArray::create();
    slidesList->retain();
    
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    skipButton = NULL;
}

Senario::~Senario()
{
    Senario::SP = NULL;
    spriteList.clear();
    menuList.clear();
    labelList.clear();
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
    
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
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
    curSlide = 0;
    active = true;
    GameScene::getThis()->setTouchEnabled(true);
    readSenarioFile();
    constructSenarioStage();
    //onOrientationChanged();
    createGUI();
}

void Senario::readSenarioFile()
{
    slidesList->removeAllObjects();
    slidesList->release();
    
    SenarioManager* sm = new SenarioManager();
    sm->parseXMLFile("senario_h.xml");
    
    slidesList = sm->slidesList;
    delete sm;
}

bool Senario::constructSenarioStage()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B colorBlack = ccc3(0, 0, 0);
   // ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorBlue = ccc3(0, 0, 255);
    clearElements();
    
    if(slidesList->count() <= curSlide)
    {
        return false;
    }
    
    Slide* slide = (Slide*)slidesList->objectAtIndex(curSlide);
    
    CCArray* elementArray = slide->elementList;
    
    CCSprite* blackScreen = CCSprite::create("blackscreen.png");
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(0, 0));
    this->addChild(blackScreen, 1);
    spriteList.push_back(blackScreen);
    
    skipButton = CCSprite::create("skip_button.png");
    skipButton->retain();
    skipButton->setScale(1.0f);
    skipButton->setAnchorPoint(ccp(1, 1));
    skipButton->setPosition(ccp(screenSize.width, screenSize.height));
    this->addChild(skipButton, 2);
    spriteList.push_back(skipButton);
    
    for(int i = 0; i < elementArray->count(); i++)
    {
        Element* ele = (Element*)elementArray->objectAtIndex(i);
        
        /*
         * In the senario stage, there are two types of elements -> sprite and dialogue, treat these two types of elements differently in order to make this display them correctly!
         */
        
        if(ele->type == Element::sprite)
        {
            /*
             * Sprite: only has a picture of the sprite shown on the screen
             * Future: may include animation of the sprite shown on the screen (probably a gif?)
             */
            
            //CCSprite* cha = CCSprite::create(ele->src.c_str(), CCRectMake(0, 0, screenSize.width * ele->width / 100.0f, screenSize.height * ele->height / 100.0f));
            
            CCSprite* cha = CCSprite::create(ele->src.c_str());
            CCSize spriteSize = cha->getContentSize();
            cha->setScale(screenSize.width / spriteSize.width * ele->width / 100.0f);
            
            cha->setAnchorPoint(ccp(0, 2));
            cha->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            this->addChild(cha, 2);
            spriteList.push_back(cha);
        }
        else if(ele->type == Element::dialogue)
        {
            /*
             * Dialogue: contains a picture of the background of that dialogue, a name label, a dialogue text and a proceed button
             * Future: may include dialogue functionalities like save, load, auto proceed, text jumper, etc....
             */
            
            CCSprite* chbox = CCSprite::create(ele->src.c_str());
            CCSize boxSize = chbox->getContentSize();
            chbox->setScale(screenSize.width / boxSize.width * ele->width / 100.0f);
            chbox->setAnchorPoint(ccp(0, 1));
            chbox->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            
            if(ele->isBackground)
            {
                this->addChild(chbox, 3);
                spriteList.push_back(chbox);
                continue;
            }
            
            float heightOff = 60.0f;
            float widthOff = 80.0f;
            
            displayTexts(ele->text, screenSize.width * (ele->left / 100.0f) + widthOff, screenSize.height * (ele->top / 100.0f) - heightOff, ele->font.c_str(), (float)ele->fontSize, colorBlack);
            
            std::stringstream ss;
            ss << ele->text;
            
            heightOff = 30.0f;
            widthOff = 80.0f;
            
            ss.str(std::string());
            ss << ele->name;
            CCLabelTTF* chboxName = CCLabelTTF::create(ss.str().c_str(), ele->font.c_str(), ele->fontSize, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
            chboxName->setColor(colorBlue);
            chboxName->setAnchorPoint(ccp(0, 1));
            chboxName->setPosition(ccp(screenSize.width * (ele->left / 100.0f) + widthOff, screenSize.height * (ele->top / 100.0f) - heightOff));
            
            this->addChild(chbox, 3);
            this->addChild(chboxName, 4);
            spriteList.push_back(chbox);
            labelList.push_back(chboxName);
        }
        else if(ele->type == Element::option)
        {
            inOption = true;
            stringstream ss;
            ss << ele->text;
            CCLabelTTF* optionLabel = CCLabelTTF::create(ss.str().c_str(), ele->font.c_str(), ele->fontSize);
            optionLabel->setColor(colorBlack);
            optionLabel->setAnchorPoint(ccp(0, 1));
            optionLabel->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            labelList.push_back(optionLabel);
            this->addChild(optionLabel, 4);
            
            CCMenuItemImage* selectButton = CCMenuItemImage::create("assign_menu_accept.png", "assign_menu_accept.png", this, menu_selector(Senario::selectButtonPressed));
            selectButton->setTag(i);
            selectButton->setAnchorPoint(ccp(0, 1));
            selectButton->setScale(0.3f);
            
            CCMenu* menu = CCMenu::create(selectButton, NULL);
            menu->setPosition(ccp(screenSize.width * (ele->left / 100.0f) + optionLabel->boundingBox().size.width + 20.0f, screenSize.height * (ele->top / 100.0f)));
            this->addChild(menu, 4);
            menuList.push_back(menu);
        }
    }
    
    this->schedule(schedule_selector(Senario::update), 1.0f/60.0f);
    
    return true;
}



void Senario::selectButtonPressed(CCObject* pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int index = pMenuItem->getTag();
    
    Slide* slide = (Slide*)slidesList->objectAtIndex(curSlide);
    CCArray* elementArray = slide->elementList;
    Element* ele = (Element*)elementArray->objectAtIndex(index);
    
    string fileName = ele->nextFile;
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
    
    SenarioManager* sm = new SenarioManager();
    sm->parseXMLFile(fileName.c_str());
    
    slidesList = sm->slidesList;
    delete sm;
    
    inOption = false;

    curSlide = 0;
    if(!constructSenarioStage()){
        startGameMenu->setVisible(true);
    }
}

void Senario::displayTexts(std::string str, float startX, float startY, string font, float fontSize, ccColor3B color)
{
    vector<std::string> tokens = split(str, ' ');
    float offX = 0;
    float offY = 0;
    float flashTimeGap = 0.05f;
    int flashGapCount = 0;
    
    for (int i = 0; i < tokens.size(); i++)
    {
        std::string tokenStr = tokens.at(i);
        CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), font.c_str(), fontSize);
        tempLabel->retain();
        
        if (startX + offX + tempLabel->boundingBox().size.width > 900.0f)
        {
            offY = offY + 30.0f;
            offX = 0;
        }
        
        CC_SAFE_RELEASE(tempLabel);
        
        for (int j = 0; j < tokenStr.length(); j++)
        {
            string tempStr = tokenStr.substr(j, 1);
            AnimatedString* as = AnimatedString::create(tempStr, flashTimeGap * (j + flashGapCount), font, fontSize, 80.0f);
            as->getLabel()->setColor(color);
            as->getLabel()->setAnchorPoint(ccp(0, 1));
            
            as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
            offX += as->label->boundingBox().size.width;
            
            this->addChild(as->getLabel(), 20);
            animatedStringList->addObject(as);
        }
        
        flashGapCount += tokenStr.size();
        offX += 10;
    }
}

vector<std::string> Senario::split(std::string text, char delimiter)
{
    int startIndex = -1;
    int endIndex = -1;
    vector<std::string> tokens;
    stringstream ss;
    
    for (int i = 0; i < text.length(); i++)
    {
        if(text[i] == delimiter)
        {
            if(startIndex < 0)
            {
            }
            else if(startIndex == endIndex)
            {
                ss.str(string());
                ss << text[startIndex];
                tokens.push_back(ss.str());
            }
            else
            {
                ss.str(string());
                for (int j = startIndex; j <= endIndex; j++)
                {
                    ss << text[j];
                }
                tokens.push_back(ss.str());
            }
            startIndex = -1;
            endIndex = -1;
        }
        else
        {
            if(startIndex < 0)
            {
                startIndex = i;
                endIndex = i;
            }
            else
            {
                endIndex = i;
            }
        }
    }
    
    if(startIndex >= 0 && endIndex >= startIndex)
    {
        ss.str(string());
        for(int i = startIndex; i <= endIndex; i++)
        {
            ss << text[i];
        }
        tokens.push_back(ss.str());
    }
    
    return tokens;
}

void Senario::nextButtonPressed(bool skip)
{
    if(!skip)
    {
        curSlide++;
        this->unschedule(schedule_selector(Senario::update));
        if(!constructSenarioStage()){
            startGameMenu->setVisible(true);
        }
    }
    else
    {
        curSlide = slidesList->count() - 1;
        this->unschedule(schedule_selector(Senario::update));
        if(!constructSenarioStage()){
            startGameMenu->setVisible(true);
        }
    }
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
    
    for(int i = 0; i < labelList.size(); i++)
    {
        this->removeChild(labelList.at(i));
    }
    
    spriteList.clear();
    menuList.clear();
    labelList.clear();
    
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        this->removeChild(as->getLabel());
    }
    
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
    
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    if(skipButton != NULL)
    {
        CC_SAFE_RELEASE(skipButton);
        skipButton = NULL;
    }
}

void Senario::createGUI(){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    /*
    character = CCSprite::create("exit_pressed.png", CCRectMake(0, 0, 125, 154) );
    character->setPosition( ccp(character->getContentSize().width/2, screenSize.height/2) );
    this->addChild(character);
    */
    
    chatbox = CCMenuItemImage::create("text_box.jpg", "text_box.jpg", this, menu_selector(Senario::buttonSelect));
    chatbox->setScale(0.2f);
    
    std::stringstream ss;
    ss << "Start Game!";
    CCLabelTTF* chatboxLabel = CCLabelTTF::create(ss.str().c_str(), "Shojumaru-Regular" , 128, CCSizeMake(ss.str().length() * 100.0f, 5.0f), kCCTextAlignmentLeft);

    chatboxLabel->setAnchorPoint(ccp(0, 0));
    chatboxLabel->setPosition( ccp(chatbox->boundingBox().size.width / 2.0f, chatbox->boundingBox().size.height));
    chatbox->addChild(chatboxLabel);
    chatboxLabel->setColor(ccc3(255,189,68));
    
    startGameMenu = CCMenu::create(chatbox, NULL);
    startGameMenu->setAnchorPoint(ccp(0.5f, 0.5f));
    startGameMenu->setPosition(screenSize.width * 0.5, screenSize.height * 0.5);
    startGameMenu->setVisible(false);
    this->addChild(startGameMenu, 1);
}

void Senario::buttonSelect()
{
    //if(active)
    //{
        active = false;
        this->setVisible(false);
        GameScene::getThis()->enableTouch();
    //}
}

void Senario::onOrientationChanged(){
    readSenarioFile();
    constructSenarioStage();
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    startGameMenu->setPosition(screenSize.width * 0.5, screenSize.height * 0.5);
}

void Senario::update(float time)
{
    if(animatedStringList != NULL)
    {
        for (int i = 0; i < animatedStringList->count(); i++)
        {
            AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
            as->update(time);
        }
    }
}