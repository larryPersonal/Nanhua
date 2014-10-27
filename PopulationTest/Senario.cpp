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
#include "WrapperClass.h"
#include "Config.h"
#include "TutorialManager.h"
#include "AnimatedDialogue.h"
#include "SoundtrackManager.h"
#include "UserProfile.h"
#include "UIButtonControl.h"
#include "SanGuoXiaoXueTang.h"

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
    
    animatedDialogueList = CCArray::create();
    animatedDialogueList->retain();
    
    skipButton = NULL;
    skipSlide = false;
    notJump = false;
    
    backgroundImage = "interiorhouse.png";
    
    scenarioState = Introduction;
    
    mixNode = NULL;
}

Senario::~Senario()
{
    Senario::SP = NULL;
    
    clearElements();
    
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
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
    UIButtonControl::pauseGame();
    loadTextures();
    GameScene::getThis()->setTouchEnabled(true);
    if(UserProfile::getThis()->systemConfig->skipSenario)
    {
        buttonSelect();
    }
    else
    {
        readSenarioFile(senario);
        CCLog("let's construct senario stage");
        constructSenarioStage(false);
        MapHandler::getThis()->centerMap();
    }
}

void Senario::readSenarioFile(const char* senario)
{
    //Config *c = Config::getConfig();
    
    slidesList->removeAllObjects();
    slidesList->release();
    
    SenarioManager* sm = new SenarioManager();
    sm->parseXMLFile(senario);
    
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
    
    if(slide->isScene)
    {
        
        CCTextureCache::sharedTextureCache()->removeAllTextures();
        CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
        CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
        CCDirector::sharedDirector()->purgeCachedData();
        
        backgroundImage = slide->scene_src;
    }
    
    CCSprite* blackScreen = CCSprite::create(backgroundImage.c_str());
    blackScreen->setScale(screenSize.width / blackScreen->boundingBox().size.width);
    blackScreen->setAnchorPoint(ccp(0.5, 0.5));
    blackScreen->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->addChild(blackScreen, 1);
    spriteList.push_back(blackScreen);
    
    skipButton = CCSprite::createWithSpriteFrameName("skip_button.png");
    skipButton->retain();
    skipButton->setScale(1.0f);
    skipButton->setAnchorPoint(ccp(1, 1));
    skipButton->setPosition(ccp(screenSize.width, screenSize.height));
    this->addChild(skipButton, 2);
    spriteList.push_back(skipButton);
    
    if(slide->hasVideo)
    {
        //Config *c = Config::getConfig();
        if(slide->video_clip.compare("") != 0)
        {
            WrapperClass::getShared()->playVideo(slide->video_clip.c_str());
        }
    }
    
    if(slide->playBGM)
    {
        if(slide->bgm_clip.compare("") != 0)
        {
            SoundtrackManager::PlayBGM(slide->bgm_clip);
        }
    }
    
    if(slide->playSFX)
    {
        if(slide->sfx_clip.compare("") != 0)
        {
            SoundtrackManager::PlaySFX(slide->sfx_clip);
        }
    }
    
    if(UserProfile::getThis()->systemConfig->hideSkipButton)
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
            
            AnimatedSprite* as = AnimatedSprite::create(ele->src.c_str(), ele->fadeIn, ele->fadeOut, true);
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
            
            AnimatedDialogue* ad = AnimatedDialogue::create(ele->src.c_str(), ele->fadeIn, ele->fadeOut);
            CCSprite* chbox = ad->getSprite();
            CCSize boxSize = chbox->getContentSize();
            chbox->setScale(screenSize.width / boxSize.width * ele->width / 100.0f);
            chbox->setAnchorPoint(ccp(0, 1));
            chbox->setPosition(ccp(screenSize.width * (ele->left / 100.0f), screenSize.height * (ele->top / 100.0f)));
            
            if(ele->isBackground)
            {
                this->addChild(chbox, 3);
                spriteList.push_back(chbox);
                animatedDialogueList->addObject(ad);
                continue;
            }
            
            float heightOff = 80.0f - ele->textOffY;
            float widthOff = 80.0f;
            
            displayTexts(ele->text, screenSize.width * (ele->left / 100.0f) + widthOff, screenSize.height * (ele->top / 100.0f) - heightOff, ele->font.c_str(), (float)ele->fontSize, colorBlack, ele->limitX);
            
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
            animatedDialogueList->addObject(ad);
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
        GlobalOutcomeModifier::getThis()->banditsModifier = ele->banditsModifier;
        GlobalOutcomeModifier::getThis()->refugeesModifier = ele->refugeeModifier;
        GlobalOutcomeModifier::getThis()->goldModifier = ele->goldModifier;
        GlobalOutcomeModifier::getThis()->foodModifier = ele->foodModifier;
        GlobalOutcomeModifier::getThis()->populationModifier = ele->populationModifier;
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

void Senario::displayTexts(std::string str, float startX, float startY, string font, float fontSize, ccColor3B color, float limitX)
{
    vector<std::string> tokens = GlobalHelper::split(str, ' ');
    float offX = 0;
    float offY = 0;
    float flashTimeGap = 0.05f;
    int flashGapCount = 0;
    
    for (int i = 0; i < tokens.size(); i++)
    {
        std::string tokenStr = tokens.at(i);
        
        bool hasChinese = false;
        for (int j = 0; j < tokenStr.length(); j++)
        {
            if(tokenStr.at(j) == '^')
            {
                hasChinese = true;
                break;
            }
        }
        
        if (hasChinese)
        {
            int startIndex = 0;
            for (int j = 0; j < tokenStr.length(); j++)
            {
                if(tokenStr.at(j) == '^')
                {
                    string str = tokenStr.substr(startIndex, j - startIndex);
                    
                    CCLabelTTF* tempLabel = CCLabelTTF::create(str.c_str(), font.c_str(), fontSize);
                    tempLabel->retain();
                    
                    if (startX + offX + tempLabel->boundingBox().size.width > limitX)
                    {
                        offY = offY + 35.0f;
                        offX = 0;
                    }
                    
                    CC_SAFE_RELEASE(tempLabel);
                    
                    AnimatedString* as = AnimatedString::create(str, flashTimeGap * flashGapCount, font, fontSize, 80.0f);
                    as->getLabel()->setColor(color);
                    as->getLabel()->setAnchorPoint(ccp(0, 1));
                    
                    as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
                    offX += as->label->boundingBox().size.width;
                    
                    this->addChild(as->getLabel(), 20);
                    animatedStringList->addObject(as);
                    
                    flashGapCount += 1;
                    
                    startIndex = j + 1;
                }
            }
            
            string str = tokenStr.substr(startIndex, tokenStr.length() - startIndex);
            AnimatedString* as = AnimatedString::create(str, flashTimeGap * flashGapCount, font, fontSize, 80.0f);
            as->getLabel()->setColor(color);
            as->getLabel()->setAnchorPoint(ccp(0, 1));
            
            as->getLabel()->setPosition(ccp(startX + offX, startY - offY));
            offX += as->label->boundingBox().size.width;
            
            this->addChild(as->getLabel(), 20);
            animatedStringList->addObject(as);
            
            flashGapCount += 1;
            
            offX += 10;
        }
        else
        {
            CCLabelTTF* tempLabel = CCLabelTTF::create(tokenStr.c_str(), font.c_str(), fontSize);
            tempLabel->retain();
            
            if (startX + offX + tempLabel->boundingBox().size.width > limitX)
            {
                offY = offY + 35.0f;
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
}

void Senario::nextButtonPressed(bool skip)
{
    if(inOption)
    {
        return;
    }
    
    bool stopFadeIn = true;
    for (int i = 0; i < animatedDialogueList->count(); i++)
    {
        AnimatedDialogue* ad = (AnimatedDialogue*) animatedDialogueList->objectAtIndex(i);
        
        if(ad->fadeIn)
        {
            stopFadeIn = false;
            break;
        }
    }
    
    if(!stopFadeIn)
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
        this->removeChild(spriteList.at(i), true);
    }
    
    for(int i = 0; i < menuList.size(); i++)
    {
        this->removeChild(menuList.at(i), true);
    }
    
    for(int i = 0; i < labelList.size(); i++)
    {
        this->removeChild(labelList.at(i), true);
    }
    
    spriteList.clear();
    menuList.clear();
    labelList.clear();
    
    for(int i = 0; i < animatedStringList->count(); i++)
    {
        AnimatedString* as = (AnimatedString*) animatedStringList->objectAtIndex(i);
        this->removeChild(as->getLabel(), true);
    }
    
    animatedStringList->removeAllObjects();
    CC_SAFE_RELEASE(animatedStringList);
    
    animatedStringList = CCArray::create();
    animatedStringList->retain();
    
    for(int i = 0; i < animatedSpriteList->count(); i++)
    {
        AnimatedSprite* as = (AnimatedSprite*) animatedSpriteList->objectAtIndex(i);
        this->removeChild(as->getSprite(), true);
    }
    
    animatedSpriteList->removeAllObjects();
    CC_SAFE_RELEASE(animatedSpriteList);
    
    animatedSpriteList = CCArray::create();
    animatedSpriteList->retain();
    
    for(int i = 0; i < animatedDialogueList->count(); i++)
    {
        AnimatedDialogue* ad = (AnimatedDialogue*) animatedDialogueList->objectAtIndex(i);
        this->removeChild(ad->getSprite(), true);
    }
    
    animatedDialogueList->removeAllObjects();
    CC_SAFE_RELEASE(animatedDialogueList);
    
    animatedDialogueList = CCArray::create();
    animatedDialogueList->retain();
    
    if(skipButton != NULL)
    {
        CC_SAFE_RELEASE(skipButton);
        skipButton = NULL;
    }
}

void Senario::buttonSelect()
{
    CCLog("button select has been triggered 1 time");
    active = false;
    GameScene::getThis()->enableTouch();
    cumulativeTime = 0;
    lastTime = 0;
    
    clearScenario();
    releaseTextures();
    
    UIButtonControl::resumeGame();
    if(scenarioState == Introduction)
    {
        this->scenarioState = Tutorial;
        SanGuoXiaoXueTang::getThis()->theState = Part_1;
        SanGuoXiaoXueTang::getThis()->showUI();
        // TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
    }
    else if(scenarioState == Scenario2)
    {
        scenarioState = Tutorial2;
        TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
    }
    else if(scenarioState == Scenario3)
    {
        scenarioState = Scenario4;
        TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
    }
    else if(scenarioState == Scenario4)
    {
        scenarioState = Scenario5;
        TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
    }
    else if(scenarioState == Scenario5)
    {
        scenarioState = Scenario6;
        TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
    }
    else if(scenarioState == Scenario6)
    {
        scenarioState = Scenario6;
        TutorialManager::getThis()->scheduleOnce(schedule_selector(TutorialManager::setupForTutorial), 1.0f);
    }
    else
    {
        if(GlobalOutcomeModifier::getThis()->refugeesModifier > 0)
        {
            this->schedule(schedule_selector(Senario::activateRefugee), 1.0f / 120.0f);
        }
        TutorialManager::getThis()->lockModule = false;
    }
}

void Senario::clearScenario()
{
    active = false;
    inOption = false;
    notJump = false;
    
    curSlide = 0;
    
    GlobalHelper::clearCache();
    
    clearElements();
    
    slidesList->removeAllObjects();
    CC_SAFE_RELEASE(slidesList);
    
    slidesList = CCArray::create();
    slidesList->retain();
    
    skipSlide = false;
    cumulativeTime = 0;
}

void Senario::stopGame()
{
    clearScenario();
    delete this;
}

void Senario::activateRefugee(float dt)
{
    cumulativeTime += dt;
    CCPoint target = CCPointMake(39,60);
    if(cumulativeTime >= lastTime + 1)
    {
        SpriteHandler::getThis()->addSpriteToMap(target, V_REFUGEE);
        lastTime++;
        if(lastTime >= GlobalOutcomeModifier::getThis()->refugeesModifier)
        {
            this->unschedule(schedule_selector(Senario::activateRefugee));
            GlobalOutcomeModifier::getThis()->refugeesModifier = 0;
        }
    }
}

void Senario::onOrientationChanged(){
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
    
    if(animatedDialogueList != NULL)
    {
        for (int i = 0; i < animatedDialogueList->count(); i++)
        {
            AnimatedDialogue* ad = (AnimatedDialogue*) animatedDialogueList->objectAtIndex(i);
            ad->update(time);
        }
    }
}

void Senario::loadTextures()
{
    if(mixNode != NULL)
    {
        releaseTextures();
    }
    mixNode = CCSpriteBatchNode::create("mix.png");
    this->addChild(mixNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("mix.plist");
}

void Senario::releaseTextures()
{
    if(mixNode != NULL)
    {
        this->removeChild(mixNode, true);
        mixNode = NULL;
    }
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("mix.plist");
}