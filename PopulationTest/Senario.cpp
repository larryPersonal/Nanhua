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

Senario* Senario::SP;

Senario::Senario()
{
    Senario::SP = this;
    active = false;
    init();
    curSlide = 0;
    slidesList = CCArray::create();
    slidesList->retain();
}

Senario::~Senario()
{
    Senario::SP = NULL;
    spriteList.clear();
    menuList.clear();
    labelList.clear();
    slidesList->removeAllObjects();
    slidesList->release();
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
            
            cha->setAnchorPoint(ccp(0, 1));
            cha->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            this->addChild(cha);
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
                this->addChild(chbox);
                spriteList.push_back(chbox);
                continue;
            }
            
            std::stringstream ss;
            ss << ele->text;
            
            createTexts(ss.str().c_str(), 100, 200, ele->font.c_str(), (float)ele->fontSize, colorBlack);
            /*
            CCLabelTTF* chboxLabel = CCLabelTTF::create(ss.str().c_str(), ele->font.c_str(), ele->fontSize, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
            chboxLabel->setColor(colorBlack);
            chboxLabel->setAnchorPoint(ccp(0, 1));
            chboxLabel->setPosition(ccp((screenSize.width - chbox->boundingBox().size.width) / 2.0f + 30.0f, (chbox->boundingBox().size.height / 2.0f) + (chbox->getPositionY() - chbox->boundingBox().size.height)));
            */
            
            float heightOff = 0.0f;
            float widthOff = 0.0f;
            ss.str(std::string());
            ss << ele->name;
            CCLabelTTF* chboxName = CCLabelTTF::create(ss.str().c_str(), ele->font.c_str(), ele->fontSize, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
            chboxName->setColor(colorBlue);
            chboxName->setAnchorPoint(ccp(0.5, 1));
            if(GlobalHelper::isHorizontal())
            {
                heightOff = -25.0f + screenSize.height * (ele->top / 100.0f) - chbox->boundingBox().size.height;
            }
            if(ele->dir.compare("left") == 0)
            {
                if(GlobalHelper::isHorizontal())
                {
                    widthOff = 20.0f;
                }
                chboxName->setPosition(ccp((screenSize.width - chbox->boundingBox().size.width) / 2.0f + chboxName->boundingBox().size.width / 2.0f + 28.0f + widthOff, chbox->boundingBox().size.height + chboxName->boundingBox().size.height / 2.0f - heightOff));
            }
            else
            {
                if(GlobalHelper::isHorizontal())
                {
                    widthOff = 10.0f;
                }
                chboxName->setPosition(ccp(chbox->boundingBox().size.width - 16.0f - widthOff, chbox->boundingBox().size.height + chboxName->boundingBox().size.height / 2.0f - heightOff));
            }
            
            CCMenuItemImage* nextButton = CCMenuItemImage::create("nextButton.png", "nextButton.png", this, menu_selector(Senario::nextButtonPressed));
            nextButton->setAnchorPoint(ccp(0.5, 0.5));
            nextButton->setScale(0.3f);
            
            this->addChild(chbox);
            //this->addChild(chboxLabel);
            this->addChild(chboxName);
            spriteList.push_back(chbox);
            //labelList.push_back(chboxLabel);
            labelList.push_back(chboxName);
            
            CCMenu* menu = CCMenu::create(nextButton, NULL);
            menu->setPosition(ccp(screenSize.width * 0.86f, screenSize.height * 0.08f));
            
            this->addChild(menu, 1);
            
            
            menuList.push_back(menu);
        }
        else if(ele->type == Element::option)
        {
            stringstream ss;
            ss << ele->text;
            CCLabelTTF* optionLabel = CCLabelTTF::create(ss.str().c_str(), ele->font.c_str(), ele->fontSize);
            optionLabel->setColor(colorBlack);
            optionLabel->setAnchorPoint(ccp(0, 1));
            optionLabel->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            labelList.push_back(optionLabel);
            this->addChild(optionLabel);
            
            CCMenuItemImage* selectButton = CCMenuItemImage::create("assign_menu_accept.png", "assign_menu_accept.png", this, menu_selector(Senario::selectButtonPressed));
            selectButton->setTag(i);
            selectButton->setAnchorPoint(ccp(0, 1));
            selectButton->setScale(0.3f);
            
            CCMenu* menu = CCMenu::create(selectButton, NULL);
            menu->setPosition(ccp(screenSize.width * (ele->left / 100.0f) + optionLabel->boundingBox().size.width + 20.0f, screenSize.height * (ele->top / 100.0f)));
            this->addChild(menu, 1);
            menuList.push_back(menu);
        }
    }
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

    curSlide = 0;
    if(!constructSenarioStage()){
        startGameMenu->setVisible(true);
    }
}

void Senario::createTexts(std::string str, float startX, float startY, string font, float fontSize, ccColor3B color)
{
    vector<std::string> tokens = split(str, ' ');
    float offX = 0;
    float offY = 0;
    
    for (int i = 0; i < tokens.size(); i++)
    {
        string tempStr = tokens.at(i);
        CCLabelTTF* tempLabel = CCLabelTTF::create(tempStr.c_str(), font.c_str(), fontSize);
        tempLabel->setColor(color);
        tempLabel->setAnchorPoint(ccp(0, 1));
        
        if(startX + offX + tempLabel->boundingBox().size.width > 900.0f)
        {
            offY = offY + 20.0f;
            offX = 0;
        }
        tempLabel->setPosition(ccp(startX + offX, startY - offY));
        offX += tempLabel->boundingBox().size.width + 5.0f;
        this->addChild(tempLabel, 20);
        labelList.push_back(tempLabel);
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

void Senario::nextButtonPressed()
{
    curSlide++;
    if(!constructSenarioStage()){
        startGameMenu->setVisible(true);
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