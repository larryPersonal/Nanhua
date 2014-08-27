//
//  SplashScene.cpp
//  PopulationTest
//
//  Created by Serious Games on 18/3/13.
//
//

#include "SplashScene.h"
#include "MainMenuScene.h"
#include "GameManager.h"
#include "GlobalHelper.h"
#include "Senario.h"
#include "TutorialManager.h"
#include "ObjectiveHandler.h"
#include "RandomEventManager.h"
#include "SanGuoXiaoXueTang.h"
#include "ConstructionHandler.h"
#include "BanditsAttackHandler.h"
#include "MapHandler.h"
#include "SpriteHandler.h"
#include "BuildingHandler.h"
#include "GlobalOutcomeModifier.h"
#include "GameHUD.h"
#include "NotificationPopup.h"

SplashScene* SplashScene::SP;


SplashScene::SplashScene()
{
    SplashScene::SP = this;
}

SplashScene::~SplashScene()
{
    SplashScene::SP = NULL;
}

SplashScene* SplashScene::getThis()
{
    return SP;
}

CCScene* SplashScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SplashScene *layer = SplashScene::create();
    
    //People, please initialize properly! C++ does not do this for you, and NULL != 0. - Larry
    layer->transitionState = 0;
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    Senario* senlayer = Senario::create();
    senlayer->retain();
    
    TutorialManager* tutlayer = TutorialManager::create();
    tutlayer->retain();
    
    ObjectiveHandler* objlayer = ObjectiveHandler::create();
    objlayer->retain();
    
    RandomEventManager* remlayer = RandomEventManager::create();
    remlayer->retain();
    
    SanGuoXiaoXueTang* sanlayer = SanGuoXiaoXueTang::create();
    sanlayer->retain();
    
    GameHUD* hudlayer = GameHUD::create();
    hudlayer->retain();
    
    NotificationPopup* notlayer = NotificationPopup::create();
    notlayer->retain();
    
    ConstructionHandler* conhandler = new ConstructionHandler();
    
    BanditsAttackHandler* banhandler = new BanditsAttackHandler();
    
    MapHandler* maphandler = new MapHandler();
    
    SpriteHandler* sprhandler = new SpriteHandler();
    
    BuildingHandler* buihandler = new BuildingHandler();
    
    GlobalOutcomeModifier* glomodifier = new GlobalOutcomeModifier();
    
    // return the scene
    return scene;
}

bool SplashScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    preloadTextures();
    
    bool isHori = GlobalHelper::isHorizontal();
    
    splashImage = CCSprite::create("newsplashpage.png");
    CCSize spriteSize = splashImage->getContentSize();
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    if(isHori)
    {
        splashImage->setScaleX(screenSize.width / spriteSize.width);
        splashImage->setScaleY(screenSize.height / spriteSize.height);
    }
    else
    {
        splashImage->cocos2d::CCNode::setScale(screenSize.height / spriteSize.width, screenSize.width / spriteSize.height);
    }
    
    CCSize newSize = splashImage->boundingBox().size;
    
    splashImage->setAnchorPoint(ccp(0.5, 0.5));
    splashImage->setPosition(ccp(screenSize.width/2, screenSize.height/2));
    this->addChild(splashImage, -1);
    
    this->initWithColor(ccc4(0, 0, 0, 255), screenSize.width, screenSize.height);
    this->schedule(schedule_selector(SplashScene::update), 1/60);
    
    new GameManager();
    
    // set the teacher management account
    stringstream ss;
    ss << "UserManagement_Account";
    
    CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), "nanhua");
    
    ss.str(std::string());
    ss << "UserManagement_Password";
    CCUserDefault::sharedUserDefault()->setStringForKey(ss.str().c_str(), "nanhua");
    
    return true;
}

void SplashScene::update(float dt){
    float alpha = this->getOpacity();
    switch (transitionState) {
        case 0:
            alpha -= 5;
            if (alpha <= 0) {
                transitionState++;
            }
            break;
        case 1:
            alpha += 5;
            if (alpha >= 255) {
                this->unschedule(schedule_selector(SplashScene::update));
                this->removeChild(splashImage, true);
                GlobalHelper::clearCache();
                clearTextureNode();
                CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
            }
            break;
            
        default:
            break;
    }
    
    this->setOpacity(alpha);
}

void SplashScene::onOrientationChanged()
{
}

void SplashScene::clearTextureNode()
{
    this->removeChild(teacherManagementNode, true);
    this->removeChild(mainGamePageNode, true);
    this->removeChild(extraTextureNode, true);
    this->removeChild(mainMenuSceneNode, true);
    this->removeChild(gameHUDNode, true);
    this->removeChild(buildingInforMenuNode, true);
    this->removeChild(buildScrollNode, true);
    this->removeChild(packedPortraitsNode, true);
    
    this->removeChild(appearAnimationNode, true);
    this->removeChild(disappearAnimationNode, true);
    this->removeChild(tokenAnimationNode, true);
    
    this->removeChild(injuredEmoteNode, true);
    this->removeChild(stealMoneyNode, true);
    this->removeChild(findBanditNode, true);
    this->removeChild(homelessNode, true);
    this->removeChild(noPathNode, true);
    this->removeChild(stealFoodNode, true);
    this->removeChild(commonNode, true);
    this->removeChild(workNode, true);
    this->removeChild(bubbleNode, true);
    this->removeChild(happinessNode, true);
    this->removeChild(starvingNode, true);
    this->removeChild(buildNode, true);
    this->removeChild(defendNode, true);
    this->removeChild(noFoodNode, true);
    
    teacherManagementNode = NULL;
    mainGamePageNode = NULL;
    extraTextureNode = NULL;
    mainMenuSceneNode = NULL;
    gameHUDNode = NULL;
    buildingInforMenuNode = NULL;
    buildScrollNode = NULL;
    packedPortraitsNode = NULL;
    
    appearAnimationNode = NULL;
    disappearAnimationNode = NULL;
    tokenAnimationNode = NULL;
    
    injuredEmoteNode = NULL;
    stealMoneyNode = NULL;
    findBanditNode = NULL;
    homelessNode = NULL;
    noPathNode = NULL;
    stealFoodNode = NULL;
    commonNode = NULL;
    workNode = NULL;
    bubbleNode = NULL;
    happinessNode = NULL;
    starvingNode = NULL;
    buildNode = NULL;
    defendNode = NULL;
    noFoodNode = NULL;
}

void SplashScene::preloadTextures()
{
    // teacher management node
    teacherManagementNode = CCSpriteBatchNode::create("teacherManagement.png");
    this->addChild(teacherManagementNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("teacherManagement.plist");
    
    // main game page texture node
    mainGamePageNode = CCSpriteBatchNode::create("MainGamePageTexture.png");
    this->addChild(mainGamePageNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("MainGamePageTexture.plist");
    
    // extra texture node
    extraTextureNode = CCSpriteBatchNode::create("Extra.png");
    this->addChild(extraTextureNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Extra.plist");
    
    // level select scene
    mainMenuSceneNode = CCSpriteBatchNode::create("LevelSelectSceneSpriteSheet.png");
    this->addChild(mainMenuSceneNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("LevelSelectSceneSpriteSheet.plist");
    
    // game hud node
    gameHUDNode = CCSpriteBatchNode::create("GameHUD.png");
    this->addChild(gameHUDNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameHUD.plist");
    
    // building infor menu node
    buildingInforMenuNode = CCSpriteBatchNode::create("BuildingInforMenuUI.png");
    this->addChild(buildingInforMenuNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BuildingInforMenuUI.plist");
    
    // build scroll node
    buildScrollNode = CCSpriteBatchNode::create("BuildScrollUI.png");
    this->addChild(buildScrollNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BuildScrollUI.plist");
    
    // packed portraits node
    packedPortraitsNode = CCSpriteBatchNode::create("packedportraits.png");
    this->addChild(packedPortraitsNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("packedportraits.plist");
    
    
    /* animation nodes */
    appearAnimationNode = CCSpriteBatchNode::create("appearAnim.png");
    this->addChild(appearAnimationNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("appearAnim.plist");
    
    disappearAnimationNode = CCSpriteBatchNode::create("disappearAnim.png");
    this->addChild(disappearAnimationNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("disappearAnim.plist");
    
    tokenAnimationNode = CCSpriteBatchNode::create("tokenAnimation.png");
    this->addChild(tokenAnimationNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("tokenAnimation.plist");
    
    
    /* sprites emotions! */
    injuredEmoteNode = CCSpriteBatchNode::create("injuredEmote.png");
    this->addChild(injuredEmoteNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("injuredEmote.plist");
    
    stealMoneyNode = CCSpriteBatchNode::create("stealMoneyEmote.png");
    this->addChild(stealMoneyNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("stealMoneyEmote.plist");
    
    findBanditNode = CCSpriteBatchNode::create("findBanditEmote.png");
    this->addChild(findBanditNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("findBanditEmote.plist");
    
    homelessNode = CCSpriteBatchNode::create("homelessEmote.png");
    this->addChild(homelessNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("homelessEmote.plist");
    
    noPathNode = CCSpriteBatchNode::create("noPathEmote.png");
    this->addChild(noPathNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("noPathEmote.plist");
    
    stealFoodNode = CCSpriteBatchNode::create("stealFoodEmote.png");
    this->addChild(stealFoodNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("stealFoodEmote.plist");
    
    commonNode = CCSpriteBatchNode::create("commonEmote.png");
    this->addChild(commonNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("commonEmote.plist");
    
    workNode = CCSpriteBatchNode::create("workEmote.png");
    this->addChild(workNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("workEmote.plist");
    
    bubbleNode = CCSpriteBatchNode::create("bubbleEmote.png");
    this->addChild(bubbleNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bubbleEmote.plist");
    
    happinessNode = CCSpriteBatchNode::create("happinessEmote.png");
    this->addChild(happinessNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("happinessEmote.plist");
    
    starvingNode = CCSpriteBatchNode::create("starvingEmote.png");
    this->addChild(starvingNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("starvingEmote.plist");
    
    buildNode = CCSpriteBatchNode::create("buildEmote.png");
    this->addChild(buildNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("buildEmote.plist");
    
    defendNode = CCSpriteBatchNode::create("defendEmote.png");
    this->addChild(defendNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("defendEmote.plist");
    
    noFoodNode = CCSpriteBatchNode::create("noFoodEmote.png");
    this->addChild(noFoodNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("noFoodEmote.plist");
}
