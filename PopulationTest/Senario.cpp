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
#include "AnimatedSprite.h"
#include "GameHUD.h"

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
    
    animatedSpriteList = CCArray::create();
    animatedSpriteList->retain();
    
    skipButton = NULL;
    skipSlide = false;
    notJump = false;
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
    
    animatedSpriteList->removeAllObjects();
    CC_SAFE_RELEASE(animatedSpriteList);
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
    if(GameScene::getThis()->systemConfig->skipSenario)
    {
        buttonSelect();
    }
    else
    {
        readSenarioFile();
        constructSenarioStage(false);
        createGUI();
        GameScene::getThis()->mapHandler->centerMap();
    }
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

bool Senario::constructSenarioStage(bool skip)
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
    
    CCSprite* blackScreen = CCSprite::create("interiorhouse.png");
    blackScreen->setScale(screenSize.width / blackScreen->boundingBox().size.width);
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(blackScreen, 1);
    spriteList.push_back(blackScreen);
    
    skipButton = CCSprite::create("skip_button.png");
    skipButton->retain();
    skipButton->setScale(1.0f);
    skipButton->setAnchorPoint(ccp(1, 1));
    skipButton->setPosition(ccp(screenSize.width, screenSize.height));
    this->addChild(skipButton, 2);
    spriteList.push_back(skipButton);
    
    if(GameScene::getThis()->systemConfig->hideSkipButton)
    {
        skipButton->setAnchorPoint(ccp(0, 1));
        skipButton->setPosition(ccp(screenSize.width, screenSize.height));
    }
    
    bool isChoosingOption = false;
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
            AnimatedSprite* as = AnimatedSprite::create(ele->src.c_str(), ele->fadeIn, ele->fadeOut);
            stringstream ss;
            CCSprite* cha = as->getSprite();
            CCSize spriteSize = cha->getContentSize();
            cha->setScale(screenSize.width / spriteSize.width * ele->width / 100.0f);
            cha->setFlipX(true);
            
            cha->setAnchorPoint(ccp(0, 1));
            cha->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            this->addChild(cha, 2);
            spriteList.push_back(cha);
            
            animatedSpriteList->addObject(as);
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
            
            float heightOff = 70.0f;
            float widthOff = 80.0f;
            
            displayTexts(ele->text, screenSize.width * (ele->left / 100.0f) + widthOff, screenSize.height * (ele->top / 100.0f) - heightOff, ele->font.c_str(), (float)ele->fontSize, colorBlack);
            
            std::stringstream ss;
            ss << ele->text;
            
            heightOff = 40.0f;
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
            isChoosingOption = true;
            inOption = true;
            notJump = true;
            
            CCMenuItemImage* selectButton = CCMenuItemImage::create(ele->src.c_str(), ele->srcp.c_str(), this, menu_selector(Senario::selectButtonPressed));
            selectButton->setTag(i);
            selectButton->setAnchorPoint(ccp(0, 1));
            selectButton->setScale(0.6f);
            
            CCMenu* menu = CCMenu::create(selectButton, NULL);
            menu->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            this->addChild(menu, 4);
            menuList.push_back(menu);
        }
    }
    
    if(!isChoosingOption && skip)
    {
        return false;
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
    
    if(ele->outcome)
    {
        GameScene::getThis()->globalOutcomeModifier->banditsModifier = ele->banditsModifier;
        GameScene::getThis()->globalOutcomeModifier->refugeesModifier = ele->refugeeModifier;
        GameScene::getThis()->globalOutcomeModifier->goldModifier = ele->goldModifier;
        GameScene::getThis()->globalOutcomeModifier->foodModifier = ele->foodModifier;
        GameScene::getThis()->globalOutcomeModifier->populationModifier = ele->populationModifier;
    }
    
    string fileName = ele->nextFile;
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
    
    SenarioManager* sm = new SenarioManager();
    sm->parseXMLFile(fileName.c_str());
    
    slidesList = sm->slidesList;
    delete sm;
    
    inOption = false;

    curSlide = 0;
    if(!constructSenarioStage(false)){
        buttonSelect();
    }
}

void Senario::displayTexts(std::string str, float startX, float startY, string font, float fontSize, ccColor3B color)
{
    vector<std::string> tokens = GlobalHelper::split(str, ' ');
    float offX = 0;
    float offY = 0;
    float flashTimeGap = 0.05f;
    int flashGapCount = 0;
    
    for (int i = 0; i < tokens.size(); i++)
    {
                std::string tokenStr = tokens.at(i);
        CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), font.c_str(), fontSize);
        tempLabel->retain();
        
        if (startX + offX + tempLabel->boundingBox().size.width > 1000.0f)
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

void Senario::nextButtonPressed(bool skip)
{
    if(inOption)
    {
        return;
    }
    bool allStopFade = true;
    for (int i = 0; i < animatedStringList->count(); i++){
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        
        if(!as->stopFade){
            allStopFade = false;
            break;
        }
    }
    
    if(allStopFade || skip){
        bool hasFade = false;
        for (int i = 0; i < animatedSpriteList->count(); i++){
            AnimatedSprite* as = (AnimatedSprite*) animatedSpriteList->objectAtIndex(i);
            
            if(as->fadeOut || as->fadeIn)
            {
                hasFade = true;
            }
            else if(as->hasFadeOutAnimation){
                as->triggerFadeOut();
                hasFade = true;
            }
        }
        skipSlide = skip;
        
        if(!hasFade){
            goNextSlide();
        }
    } else {
        if(notJump)
        {
            notJump = false;
            return;
        }
        for (int i = 0; i < animatedStringList->count(); i++){
            AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
            
            as->getLabel()->setOpacity((GLubyte) 255);
            as->stopFade = true;
        }
    }
}

void Senario::goNextSlide(){
    if(!skipSlide)
    {
        curSlide++;
        this->unschedule(schedule_selector(Senario::update));
        if(!constructSenarioStage(skipSlide)){
            buttonSelect();
        }
    }
    else
    {
        curSlide = slidesList->count() - 1;
        this->unschedule(schedule_selector(Senario::update));
        if(!constructSenarioStage(skipSlide)){
            buttonSelect();
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
    
    for(int i = 0; i < animatedSpriteList->count(); i++)
    {
        AnimatedSprite* as = (AnimatedSprite*) animatedSpriteList->objectAtIndex(i);
        this->removeChild(as->getSprite());
    }
    
    animatedSpriteList->removeAllObjects();
    CC_SAFE_RELEASE(animatedSpriteList);
    
    animatedSpriteList = CCArray::create();
    animatedSpriteList->retain();
    
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
   // CCLabelTTF* chatboxLabel = CCLabelTTF::create(ss.str().c_str(), "TooneyLoons" , 128, CCSizeMake(ss.str().length() * 100.0f, 5.0f), kCCTextAlignmentLeft);

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
    active = false;
    this->setVisible(false);
    GameScene::getThis()->enableTouch();
    GameHUD::getThis()->pause = false;
    cumulativeTime = 0;
    lastTime = 0;
    if(GameScene::getThis()->globalOutcomeModifier->refugeesModifier > 0)
    {
        this->schedule(schedule_selector(Senario::activateRefugee), 1.0f / 120.0f);
    }
}

void Senario::activateRefugee(float dt)
{
    cumulativeTime += dt;
    CCPoint target = CCPointMake(29,33);
    if(cumulativeTime >= lastTime + 1)
    {
        GameScene::getThis()->spriteHandler->addSpriteToMap(target, V_REFUGEE);
        lastTime++;
        if(lastTime >= GameScene::getThis()->globalOutcomeModifier->refugeesModifier)
        {
            this->unschedule(schedule_selector(Senario::activateRefugee));
            GameScene::getThis()->globalOutcomeModifier = 0;
        }
    }
}

void Senario::onOrientationChanged(){
    readSenarioFile();
    constructSenarioStage(false);
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
    
    if(animatedSpriteList != NULL)
    {
        for (int i = 0; i < animatedSpriteList->count(); i++)
        {
            AnimatedSprite* as = (AnimatedSprite*) animatedSpriteList->objectAtIndex(i);
            as->update(time);
        }
    }
}