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

SpriteRow::SpriteRow(GameSprite* gs, ScrollArea* sa, Building* building, int ind, bool isMember)
{
    mSpriteRowEnergyCurrent = 0;
    mSpriteRowEnergyRequired = 0;
    mSpriteRowHungryCurrent = 0;
    mSpriteRowHungryRequired = 0;
    mSpriteRowAction = IDLE;
    mSpriteRowIsDoingJob = false;
    mSpriteRowSpriteName = "";
    
    this->gameSprite = gs;
    this->scrollArea = sa;
    this->building = building;
    this->index = ind;
    
    this->isMember = isMember;
   
    init();
    
}

SpriteRow::~SpriteRow()
{
}

SpriteRow* SpriteRow::create(GameSprite* gs, ScrollArea* sa, Building* building, int ind, bool isMember)
{
    SpriteRow *pRet = new SpriteRow(gs, sa, building, ind, isMember);
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
    mSpriteRowEnergyCurrent = gameSprite->getPossessions()->energyRating;
    mSpriteRowEnergyRequired = gameSprite->getPossessions()->default_energy_limit;
    mSpriteRowHungryCurrent = gameSprite->getPossessions()->currentHungry;
    mSpriteRowHungryRequired = gameSprite->getPossessions()->default_hungry_limit;
    mSpriteRowAction = gameSprite->currAction;
    mSpriteRowIsDoingJob = gameSprite->isDoingJob;
    mSpriteRowSpriteName = gameSprite->spriteName;
    
    //display the image of the sprite
    std::string tempStr = gameSprite->spriteName;
    villagerImage = CCMenuItemImage::create( tempStr.append("_port.png").c_str(), tempStr.c_str(), this, menu_selector(SpriteRow::showSprite) );
    
    villagerImage->setScale(48.0f / villagerImage->boundingBox().size.width);
    villagerImage->setAnchorPoint(ccp(0, 1));
    
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    
    std::stringstream ss;
    
    // display the name of the sprite
    ss << gameSprite->spriteDisplayedName;
    
    villagerNameLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 20, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerNameLabel->setColor(colorGreen);
    villagerNameLabel->setAnchorPoint(ccp(0, 1));
    
    
    // display the loyalth and hapiness of the sprite
    Possessions* possessions = gameSprite->getPossessions();
    
    /*
    ss.str(std::string());
    ss << possessions->loyaltyRating;
    
    villagerLoyLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerLoyLabel->setColor(colorGreen);
    villagerLoyLabel->setAnchorPoint(ccp(0, 1));
    
    ss.str(std::string());
    ss << possessions->happinessRating;
    
    villagerHapLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerHapLabel->setColor(colorGreen);
    villagerHapLabel->setAnchorPoint(ccp(0, 1));
    
    // display the level of the sprite
    ss.str(std::string());
    ss << "1";
    
    villagerLevelLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerLevelLabel->setColor(colorGreen);
    villagerLevelLabel->setAnchorPoint(ccp(0, 1));
    
     
    // display the movement speed of the sprite
    ss.str(std::string());
    ss << possessions->default_movement_speed;
    
    movementSpeedLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    movementSpeedLabel->setColor(colorGreen);
    movementSpeedLabel->setAnchorPoint(ccp(0, 1));
    */
    
    // display the energy bar of the sprite
    villagerEnergyBar = new ProgressBar();
    villagerEnergyBar->createProgressBar(CCRectMake(0, 0, 80, 20),
                                   CCRectMake(5, 5, 70, 10),
                                   "loadingbar-empty.png",
                                   "loadingbar-left.png",
                                   "loadingbar-right.png",
                                   "loadingbar-full.png");
    villagerEnergyBar->setValue((float)possessions->energyRating / (float)possessions->default_energy_limit);
    
    // display the energy label of the sprite
    ss.str(std::string());
    ss << possessions->energyRating << "/" << possessions->default_energy_limit;
    
    villagerEnergyLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerEnergyLabel->setColor(colorGreen);
    villagerEnergyLabel->setAnchorPoint(ccp(0, 1));
    
    // display the hungry bar of the sprite
    villagerHungryBar = new ProgressBar();
    villagerHungryBar->createProgressBar(CCRectMake(0, 0, 80, 20),
                                         CCRectMake(5, 5, 70, 10),
                                         "loadingbar-empty.png",
                                         "loadingbar-left.png",
                                         "loadingbar-right.png",
                                         "loadingbar-full.png");
    villagerHungryBar->setValue((float)possessions->currentHungry / (float)possessions->default_hungry_limit);
    
    // display the hungry label of the sprite
    ss.str(std::string());
    ss << possessions->currentHungry << "/" << possessions->default_hungry_limit;
    
    villagerHungryLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerHungryLabel->setColor(colorGreen);
    villagerHungryLabel->setAnchorPoint(ccp(0, 1));
    
    /*
    // display the job of the sprite
    ss.str(std::string());
    ss << getJobString(gameSprite->getJob());
    
    jobLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    jobLabel->setColor(colorGreen);
    jobLabel->setAnchorPoint(ccp(0, 1));
    */
    
    // display the action of the sprite
    ss.str(std::string());
    ss << getActionString(gameSprite->getAction());
    
    actionLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    actionLabel->setColor(colorGreen);
    actionLabel->setAnchorPoint(ccp(0, 1));
    
    // display whether the sprite is doning job
    ss.str(std::string());
    if(gameSprite->getIsDoingJob())
    {
        ss << "YES";
    }
    else
    {
        ss << "NO";
    }
    
    isDongingJobLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 18, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    isDongingJobLabel->setColor(colorGreen);
    isDongingJobLabel->setAnchorPoint(ccp(0, 1));
    
    // Check whether the building is under construction so that the job is either to construct the building or play a job inside the building
    bool underConstruction = (building->build_uint_current < building->build_uint_required);
    
    // Menu items
    menuItems = CCArray::create();
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
    
    // the assgin job button for the villagers
    // builder is a special job that does not coexist with other jobs in the same phase.
    if(underConstruction)
    {
        // builder button
        villagerSelectButton = CCMenuItemImage::create("construction.png", "construction.png", this, menu_selector(SpriteRow::construction));
    }
    else if(building->buildingType == HOUSING)
    {
        if(gameSprite->getHome() == building)
        {
            villagerSelectButton = CCMenuItemImage::create("rest.png", "rest.png", this, menu_selector(SpriteRow::recover));
        }
        else
        {
            villagerSelectButton = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(SpriteRow::assignHome));
        }
    }
    else if(building->buildingType == AMENITY)
    {
        if(building->food_consumption_rate > 0){
            villagerSelectButton = CCMenuItemImage::create("eat.png", "eat.png", this, menu_selector(SpriteRow::eatFood));
        }
        else
        {
            villagerSelectButton = CCMenuItemImage::create("farm.png", "farm.png", this, menu_selector(SpriteRow::farming));
        }
    }
    else
    {
        // job button
        villagerSelectButton = CCMenuItemImage::create("job.png", "job.png", this, menu_selector(SpriteRow::doJob));
    }
    villagerSelectButton->setScale(48.0f / villagerSelectButton->boundingBox().size.width);
    villagerSelectButton->setAnchorPoint(ccp(0, 1));
    
    // the cancel job button!
    villagerCancelButton = CCMenuItemImage::create("cancel.png", "cancel.png", this, menu_selector(SpriteRow::cancelJob));
    villagerCancelButton->setScale(48.0f / villagerCancelButton->boundingBox().size.width);
    villagerCancelButton->setAnchorPoint(ccp(0, 1));
    
    // the resign button for home
    villagerResignButton = CCMenuItemImage::create("dismiss.jpg", "dismiss.jpg", this, menu_selector(SpriteRow::resignHome));
    villagerResignButton->setScale(48.0f / villagerResignButton->boundingBox().size.width);
    villagerResignButton->setAnchorPoint(ccp(0, 1));
    
    menuItems->addObject(villagerImage);
    
    menuItems->addObject(villagerSelectButton);
    menuItems->addObject(villagerCancelButton);
    menuItems->addObject(villagerResignButton);
    
    if(gameSprite->getJob() == NONE && gameSprite->currAction != RESTING && gameSprite->nextAction != RESTING)
    {
        villagerCancelButton->setVisible(false);
    }
    else
    {
        villagerSelectButton->setVisible(false);
    }
    
    if(gameSprite->type == M_REFUGEE || gameSprite->type == F_REFUGEE || gameSprite->getHome() != building)
    {
        villagerResignButton->setVisible(false);
    }
    
    if(isMember)
    {
        villagerSelectButton->setVisible(false);
        villagerCancelButton->setVisible(false);
        villagerResignButton->setVisible(false);
    }
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // register all parts to the scroll area.
    villagerImage->setPosition(ccp(-570.0f, 0));
    scrollArea->addItem(villagerNameLabel, ccp(14.0f + villagerImage->boundingBox().size.width, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f + 3.0f));
    
    /*
    scrollArea->addItem(villagerLoyLabel, ccp(150.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(villagerHapLabel, ccp(175.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(villagerLevelLabel, ccp(203.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(movementSpeedLabel, ccp(225.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    */
    
    scrollArea->addItem(villagerEnergyBar, ccp(150.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyBar->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(villagerEnergyLabel, ccp(235.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    
    scrollArea->addItem(villagerHungryBar, ccp(310.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyBar->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(villagerHungryLabel, ccp(395.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    
    /*
    scrollArea->addItem(jobLabel, ccp(410.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    */
    
    scrollArea->addItem(actionLabel, ccp(468.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(isDongingJobLabel, ccp(540.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    scrollArea->addItem(menu, ccp(580.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
    
    villagerResignButton->setPosition(ccp(villagerSelectButton->boundingBox().size.width, 0));
    
}

void SpriteRow::construction()
{
    //SelectPopulation::getThis()->closeMenu(true);
    // get all sprite type
    if(gameSprite != NULL && building != NULL)
    {
        gameSprite->ChangeSpriteTo(getSpriteType(M_BUILDER, F_BUILDER));
        
        gameSprite->setJob(BUILDER);
        gameSprite->setJobLocation(building);
        gameSprite->setTargetLocation(building);
        gameSprite->GoBuild(building);
        
        gameSprite->futureAction1 = EATING;
        gameSprite->futureAction2 = RESTING;
        
        villagerSelectButton->setVisible(false);
        villagerCancelButton->setVisible(true);
        
        unlinkChildren();
        
        building->memberSpriteList->addObject(gameSprite);
        
        SelectPopulation::getThis()->addMemberRow(gameSprite, this);
    }
}

GameSprite* SpriteRow::getSpriteType(SpriteType mst, SpriteType fst)
{
    CCArray* allSprites = GameScene::getThis()->spriteHandler->allSprites;
    bool isMale = (gameSprite->gender == 'm');
    for (int i = 0; i < allSprites->count(); i++)
    {
        GameSprite* sprite = (GameSprite*)allSprites->objectAtIndex(i);
        if(isMale)
        {
            if(sprite->type == mst)
            {
                return sprite;
            }
        }
        else
        {
            if(sprite->type == fst)
            {
                return sprite;
            }
        }
    }
}

void SpriteRow::recover()
{
    if(gameSprite != NULL && building != NULL)
    {
        if(gameSprite->type == M_REFUGEE || gameSprite->type == F_REFUGEE)
        {
            return;
        }
        villagerSelectButton->setVisible(false);
        villagerCancelButton->setVisible(true);
        
        gameSprite->setTargetLocation(building);
        gameSprite->GoRest(building);
    }
}

void SpriteRow::eatFood()
{
    if(gameSprite != NULL && building != NULL)
    {
        villagerSelectButton->setVisible(false);
        villagerCancelButton->setVisible(true);
        
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
        villagerSelectButton->setVisible(false);
        villagerCancelButton->setVisible(true);
        
        gameSprite->ChangeSpriteTo(getSpriteType(M_FARMER, F_FARMER));
        
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
        
        gameSprite->ChangeSpriteTo(getSpriteType(M_CITIZEN, F_CITIZEN));
        
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
        
        gameSprite->ChangeSpriteTo(getSpriteType(M_REFUGEE, F_REFUGEE));
        
        gameSprite->setTargetLocation(NULL);
        
        unlinkChildren();
        
        SelectPopulation::getThis()->addVillagerRow(gameSprite, this);
    }
}

void SpriteRow::doJob()
{
    villagerSelectButton->setVisible(false);
    villagerCancelButton->setVisible(true);
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
            villagerSelectButton->setVisible(true);
            villagerCancelButton->setVisible(false);
        } else {
            quitJob();
            gameSprite->ChangeSpriteTo(getSpriteType(M_CITIZEN, F_CITIZEN));
            
            gameSprite->setTargetLocation(NULL);
            villagerSelectButton->setVisible(true);
            villagerCancelButton->setVisible(false);
            
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
    scrollArea->removeI((CCNode*) villagerNameLabel);
    
    /*
    scrollArea->removeI((CCNode*) villagerLoyLabel);
    scrollArea->removeI((CCNode*) villagerHapLabel);
    scrollArea->removeI((CCNode*) villagerLevelLabel);
    scrollArea->removeI((CCNode*) movementSpeedLabel);
    */
    
    scrollArea->removeI((CCNode*) villagerEnergyBar);
    scrollArea->removeI((CCNode*) villagerEnergyLabel);
    
    scrollArea->removeI((CCNode*) villagerHungryBar);
    scrollArea->removeI((CCNode*) villagerHungryLabel);
    
    /*
    scrollArea->removeI((CCNode*) jobLabel);
    */
    
    scrollArea->removeI((CCNode*) actionLabel);
    scrollArea->removeI((CCNode*) isDongingJobLabel);
    scrollArea->removeI((CCNode*) villagerSelectButton);
    scrollArea->removeI((CCNode*) villagerCancelButton);
    scrollArea->removeI((CCNode*) villagerResignButton);
    scrollArea->removeI((CCNode*) menu);
}

void SpriteRow::rearrange(int index)
{
    resetPosition((CCNode*) villagerImage, ccp(10.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
    resetPosition((CCNode*) villagerNameLabel, ccp(14.0f + villagerImage->boundingBox().size.width, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f + 3.0f));
    
    /*
    resetPosition((CCNode*) villagerLoyLabel, ccp(150.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    resetPosition((CCNode*) villagerHapLabel, ccp(175.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    resetPosition((CCNode*) villagerLevelLabel, ccp(203.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    resetPosition((CCNode*) movementSpeedLabel, ccp(225.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerLoyLabel->boundingBox().size.height / 2.0f + 5.0f));
    */
    
    resetPosition((CCNode*) villagerEnergyBar, ccp(150.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyBar->boundingBox().size.height / 2.0f + 5.0f));
    resetPosition((CCNode*) villagerEnergyLabel, ccp(235.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    
    resetPosition((CCNode*) villagerHungryBar, ccp(310.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyBar->boundingBox().size.height / 2.0f + 5.0f));
    resetPosition((CCNode*) villagerHungryLabel, ccp(395.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    
    /*
    resetPosition((CCNode*) jobLabel, ccp(410.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    */
    
    
    resetPosition((CCNode*) actionLabel, ccp(468.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
    resetPosition((CCNode*) isDongingJobLabel, ccp(540.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyLabel->boundingBox().size.height / 2.0f + 5.0f));
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
    else if(sj == GUARD)
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
        villagerEnergyLabel->setString(ss.str().c_str());
        villagerEnergyBar->setValue((float) mSpriteRowEnergyCurrent / (float) mSpriteRowEnergyRequired);
    }
    
    if(mSpriteRowHungryCurrent != gameSprite->getPossessions()->currentHungry || mSpriteRowHungryRequired != gameSprite->getPossessions()->default_hungry_limit)
    {
        mSpriteRowHungryCurrent = gameSprite->getPossessions()->currentHungry;
        mSpriteRowHungryRequired = gameSprite->getPossessions()->default_hungry_limit;
        ss.str(std::string());
        ss << mSpriteRowHungryCurrent << "/" << mSpriteRowHungryRequired;
        villagerHungryLabel->setString(ss.str().c_str());
        villagerHungryBar->setValue((float) mSpriteRowHungryCurrent / (float) mSpriteRowHungryRequired);
    }
    
    if(mSpriteRowAction != gameSprite->currAction)
    {
        mSpriteRowAction = gameSprite->currAction;
        ss.str(std::string());
        ss << getActionString(mSpriteRowAction);
        actionLabel->setString(ss.str().c_str());
    }
    
    if(mSpriteRowIsDoingJob != gameSprite->isDoingJob)
    {
        mSpriteRowIsDoingJob = gameSprite->isDoingJob;
        ss.str(std::string());
        if(mSpriteRowIsDoingJob)
        {
            ss << "YES";
        }
        else
        {
            ss << "NO";
        }
        isDongingJobLabel->setString(ss.str().c_str());
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