//
//  SpriteRow.cpp
//  PopulationTest
//
//  Created by admin on 13/1/14.
//
//

#include "SpriteRow.h"
#include "Possessions.h"
#include "SelectPopulation.h"
#include "GameScene.h"
#include "SpriteInfoMenu.h"
#include "GlobalHelper.h"

SpriteRow::SpriteRow(GameSprite* gs, ScrollArea* sa, Building* building, int ind)
{
    mSpriteRowEnergyCurrent = 0;
    mSpriteRowEnergyRequired = 0;
    mSpriteRowSpriteName = "";
    
    this->gameSprite = gs;
    this->scrollArea = sa;
    this->building = building;
    this->index = ind;
    
    mi = CCArray::create();
    mi->retain();
    
    init();
}

SpriteRow::~SpriteRow()
{
}

SpriteRow* SpriteRow::create(GameSprite* gs, ScrollArea* sa, Building* building, int ind)
{
    SpriteRow *pRet = new SpriteRow(gs, sa, building, ind);
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

bool SpriteRow::init()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    int row = (int) (index / 4);
    int column = (int) (index % 4);
    
    mSpriteRowEnergyCurrent = gameSprite->getPossessions()->energyRating;
    mSpriteRowEnergyRequired = gameSprite->getPossessions()->default_energy_limit;
    mSpriteRowSpriteName = gameSprite->spriteName;
    
    // display the sprite row background
    spriteRowBackground = CCSprite::create("characterbox.png");
    spriteRowBackground->setScale(100.0f / spriteRowBackground->boundingBox().size.width);
    spriteRowBackground->setAnchorPoint(ccp(0, 1));
    scrollArea->addItem(spriteRowBackground, ccp(30.0f + 100.0f * column, 110.0f + 100.0f * row));
    
    //display the image of the sprite
    std::string tempStr = gameSprite->spriteName;
    villagerImage = CCMenuItemImage::create( tempStr.append("_port.png").c_str(), tempStr.c_str(), this, menu_selector(SpriteRow::showSprite) );
    villagerImage->setScale(64.0f / villagerImage->boundingBox().size.width);
    villagerImage->setAnchorPoint(ccp(0, 1));
    scrollArea->addItem(villagerImage, ccp(30.0f + 100.0f * column, 122.0f + 100.0f * row));
    
    // display the energy bar of the sprite
    Possessions* possessions = gameSprite->getPossessions();
    
    // display the energy bar of the sprite
    villagerEnergyBar = new ProgressBar();
    villagerEnergyBar->createProgressBar(CCRectMake(0, 0, 16, 70),
                                   CCRectMake(4, 5, 8, 60),
                                   "worker availablevertbar.png",
                                   "NONE",
                                   "NONE",
                                   "worker availablevertbarinner.png", false);
    villagerEnergyBar->setValue((float)possessions->energyRating / (float)possessions->default_energy_limit);
    scrollArea->addItem(villagerEnergyBar, ccp(106.0f + 100.0f * column, 120.0f + 100.0f * row));
    
    emotionFace = CCSprite::create("happy2.png");
    emotionFace->setScale(spriteRowBackground->boundingBox().size.width / emotionFace->boundingBox().size.width * 0.3f);
    float happinessRate = gameSprite->getPossessions()->happinessRating;
    if(happinessRate >= 70)
    {
        emotionFace->setTexture(CCTextureCache::sharedTextureCache()->addImage("happy2.png"));
    }
    else if(happinessRate >= 40)
    {
        emotionFace->setTexture(CCTextureCache::sharedTextureCache()->addImage("idle1.png"));
    }
    else if(happinessRate >= 10)
    {
        emotionFace->setTexture(CCTextureCache::sharedTextureCache()->addImage("unhappy1.png"));
    }
    else
    {
        emotionFace->setTexture(CCTextureCache::sharedTextureCache()->addImage("tired1.png"));
    }
    scrollArea->addItem(emotionFace, ccp(80.0f + 100.0f * column, 158.0f + 100.0f * row));
    
    // display the mask of the sprite row
    spriteRowMask = CCSprite::create("workers_menu_selectedBG_overlay.png");
    spriteRowMask->setScaleX( spriteRowBackground->boundingBox().size.width / spriteRowMask->boundingBox().size.width );
    spriteRowMask->setScaleY( spriteRowBackground->boundingBox().size.height / spriteRowMask->boundingBox().size.height * 1.2f );
    spriteRowMask->setAnchorPoint(ccp(0, 1));
    scrollArea->addItem(spriteRowMask, ccp(30.0f + 100.0f * column, 100.0f + 100.0f * row));
    spriteRowMask->setVisible(false);
    
    // display the button collider
    buttonCollider = CCMenuItemImage::create( "workers_menu_buttonCollider.png", "workers_menu_buttonCollider.png", this, menu_selector(SpriteRow::clickSprite));
    //buttonCollider = CCMenuItemImage::create( "workers_menu_selectedBG_overlay.png", "workers_menu_selectedBG_overlay.png", this, menu_selector(SpriteRow::clickSprite));
    buttonCollider->setScaleX( spriteRowBackground->boundingBox().size.width / buttonCollider->boundingBox().size.width );
    buttonCollider->setScaleY( spriteRowBackground->boundingBox().size.height / buttonCollider->boundingBox().size.height );
    buttonCollider->setAnchorPoint(ccp(0, 1));
    
    mi->addObject(buttonCollider);
    
    menu = CCMenu::createWithArray(mi);
    menu->setTouchPriority(kCCMenuHandlerPriority -10);
    menu->setPosition(CCPointZero);

    scrollArea->addItem(menu, ccp(30.0f + 100.0f * column, 110.0f + 100.0f * row));
    
    return true;
}

void SpriteRow::clickSprite()
{
    if(spriteRowMask->isVisible())
    {
        // sprite row mask is visible -> the sprite row has been selected, then unselect it
        SelectPopulation::getThis()->unselectSprite(gameSprite, this);
    }
    else
    {
        // sprite row mask is not visible -> the sprite row has not been selected, then select it
        SelectPopulation::getThis()->selectSprite(gameSprite, this);
    }
}

void SpriteRow::construction()
{
    //SelectPopulation::getThis()->closeMenu(true);
    // get all sprite type
    if(gameSprite != NULL && building != NULL)
    {
        gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_BUILDER));
        
        gameSprite->setJob(BUILDER);
        gameSprite->setJobLocation(building);
        gameSprite->setTargetLocation(building);
        gameSprite->GoBuild(building);
        
        gameSprite->futureAction1 = EATING;
        gameSprite->futureAction2 = RESTING;
        
        unlinkChildren();
        
        building->memberSpriteList->addObject(gameSprite);
        
        SelectPopulation::getThis()->addMemberRow(gameSprite, this);
    }
}

void SpriteRow::recover()
{
    if(gameSprite != NULL && building != NULL)
    {
        if(gameSprite->villagerClass == V_REFUGEE)
        {
            return;
        }
        
        gameSprite->setTargetLocation(building);
        gameSprite->GoRest(building);
    }
}

void SpriteRow::eatFood()
{
    if(gameSprite != NULL && building != NULL)
    {
        gameSprite->setTargetLocation(building);
        gameSprite->GoEat(building);
        
        unlinkChildren();
        
        SelectPopulation::getThis()->addMemberRow(gameSprite, this);
    }
}

void SpriteRow::farming()
{
    if(gameSprite != NULL && building != NULL)
    {
        
        gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_FARMER));
        
        gameSprite->setJob(FARMER);
        gameSprite->setJobLocation(building);
        gameSprite->setTargetLocation(building);
        
        building->farmState = FARM;
        
        gameSprite->GoFarming(building);
        
        gameSprite->futureAction1 = EATING;
        gameSprite->futureAction2 = RESTING;
        
        building->memberSpriteList->addObject(gameSprite);
        
        unlinkChildren();
        
        SelectPopulation::getThis()->addMemberRow(gameSprite, this);
    }
}

void SpriteRow::assignHome()
{
    if(gameSprite != NULL && building != NULL)
    {
        if(!hasVacancy())
        {
            // TODO: if no vacancy, warning player this!
            return;
        }
        gameSprite->setHome(building);
        
        gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_CITIZEN));
        
        unlinkChildren();
        
        SelectPopulation::getThis()->addMemberRow(gameSprite, this);
    }
}

void SpriteRow::resignHome()
{
    if(gameSprite != NULL && building != NULL)
    {
        gameSprite->setHome(NULL);
        
        quitHome();
        quitJob();
        
        gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_REFUGEE));
        
        gameSprite->setTargetLocation(NULL);
        
        unlinkChildren();
        
        SelectPopulation::getThis()->addVillagerRow(gameSprite, this);
    }
}

void SpriteRow::doJob()
{
}

void SpriteRow::cancelJob()
{
    if(gameSprite != NULL)
    {
        if(building->buildingType == HOUSING && building->build_uint_current >= building->build_uint_required)
        {
            // if it is a resident housing, cancel rest
            quitHome();
            
            gameSprite->setTargetLocation(NULL);
        } else {
            quitJob();
            gameSprite->changeClassTo(GlobalHelper::getSpriteClassByVillagerClass(V_CITIZEN));
            
            gameSprite->setTargetLocation(NULL);
            
            unlinkChildren();
            
            SelectPopulation::getThis()->addVillagerRow(gameSprite, this);
        }
        
    }
}

void SpriteRow::quitHome()
{
    gameSprite->currAction = IDLE;
    gameSprite->nextAction = IDLE;
    gameSprite->setIsFollowingMovementInstruction(false);
}

void SpriteRow::quitJob()
{
    gameSprite->setJob(NONE);
    gameSprite->setJobLocation(NULL);
    gameSprite->setAction(IDLE);
    gameSprite->setIsDoingJob(false);
    gameSprite->setIsFollowingMovementInstruction(false);
}

// check the building, to see whether there is vacancy for assign new sprite
bool SpriteRow::hasVacancy()
{
    int count = 0;
    CCArray* allSpritesOnMap = GameScene::getThis()->spriteHandler->spritesOnMap;
    for(int i = 0; i < allSpritesOnMap->count(); i++)
    {
        GameSprite* gs = (GameSprite*)allSpritesOnMap->objectAtIndex(i);
        if(gs->getHome() == building)
        {
            count++;
        }
    }
    return count < building->populationLimit;
}

void SpriteRow::unlinkChildren()
{
    scrollArea->removeI((CCNode*) villagerImage);
    
    scrollArea->removeI((CCNode*) villagerEnergyBar);
 //   scrollArea->removeI((CCNode*) villagerEnergyLabel);
    
    scrollArea->removeI((CCNode*) menu);
}

void SpriteRow::rearrange(int index)
{
    resetPosition((CCNode*) villagerImage, ccp(10.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
    
    resetPosition((CCNode*) villagerEnergyBar, ccp(150.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyBar->boundingBox().size.height / 2.0f + 5.0f));
 //   resetPosition((CCNode*) villagerEnergyLabel, ccp(235.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));

    resetPosition((CCNode*) menu, ccp(580.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
}

void SpriteRow::resetPosition(cocos2d::CCNode * node, CCPoint position)
{
    position.y = scrollArea->getLayer()->getContentSize().height - position.y;
    node->setPosition(position);
}

std::string SpriteRow::getJobString(SpriteJob sj)
{
    if(sj == NONE)
    {
        return "NONE";
    }
    else if(sj == BUILDER)
    {
        return "Builder";
    }
    else if(sj == SOLDIER)
    {
        return "Guard";
    }
    else if(sj == FARMER)
    {
        return "Farmer";
    }
    else
    {
        return "NONE";
    }
}

std::string SpriteRow::getActionString(SpriteAction sa)
{
    if(sa == IDLE)
    {
        return "IDLE";
    }
    else if(sa == WALKING)
    {
        return "Walking";
    }
    else if(sa == CARRYING)
    {
        return "Carrying";
    }
    else if(sa == FIGHTING)
    {
        return "Fighting";
    }
    else if(sa == ESCAPING)
    {
        return "Escaping";
    }
    else if(sa == EATING)
    {
        return "Eating";
    }
    else if(sa == STORING)
    {
        return "Storing";
    }
    else if(sa == FARMING)
    {
        return "Farming";
    }
    else if(sa == BUILD)
    {
        return "Build";
    }
    else if(sa == RESTING)
    {
        return "Resting";
    }
    else if(sa == ACTION_END)
    {
        return "END";
    }
    else
    {
        return "IDLE";
    }
}

void SpriteRow::showSprite()
{
    //CCLog("Hello World");
}

void SpriteRow::refreshAllMenuItems()
{
    std::stringstream ss;
    
    if(mSpriteRowEnergyCurrent != gameSprite->getPossessions()->energyRating || mSpriteRowEnergyRequired != gameSprite->getPossessions()->default_energy_limit)
    {
        mSpriteRowEnergyCurrent = gameSprite->getPossessions()->energyRating;
        mSpriteRowEnergyRequired = gameSprite->getPossessions()->default_energy_limit;
        ss.str(std::string());
        ss << mSpriteRowEnergyCurrent << "/" << mSpriteRowEnergyRequired;
      //  villagerEnergyLabel->setString(ss.str().c_str());
        villagerEnergyBar->setValue((float) mSpriteRowEnergyCurrent / (float) mSpriteRowEnergyRequired);
    }
    
    if(mSpriteRowSpriteName.compare(gameSprite->spriteName) != 0)
    {
        mSpriteRowSpriteName = gameSprite->spriteName;
        
        std::string tempStr = gameSprite->spriteName.append("_port.png");
        CCMenuItemImage* tempImage = CCMenuItemImage::create( tempStr.c_str(), tempStr.c_str(), this, menu_selector(SpriteRow::showSprite) );
        tempImage->setScale(48.0f / villagerImage->boundingBox().size.width);
        
        villagerImage->setNormalImage(tempImage);
        
        villagerImage->setScale(48.0f / villagerImage->boundingBox().size.width);
        villagerImage->setAnchorPoint(ccp(0, 1));

    }
}

CCSprite* SpriteRow::getMask()
{
    return spriteRowMask;
}

int SpriteRow::getIndex()
{
    return index;
}

