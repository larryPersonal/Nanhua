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

SpriteRow::SpriteRow(GameSprite* gs, ScrollArea* sa, Building* building, int ind)
{
    this->gameSprite = gs;
    this->scrollArea = sa;
    this->building = building;
    this->index = ind;
   
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

void SpriteRow::init()
{
    //display the image of the sprite
    std::string tempStr = gameSprite->spriteName;
    villagerImage = CCSprite::create( tempStr.append("_port.png").c_str() );
    
    villagerImage->setScale(64.0f / villagerImage->boundingBox().size.width);
    villagerImage->setAnchorPoint(ccp(0, 1));
    
    
    ccColor3B colorBlack = ccc3(0, 0, 0);
    ccColor3B colorYellow = ccc3(225, 219, 108);
    ccColor3B colorGreen = ccc3(81, 77, 2);
    
    std::stringstream ss;
    
    // display the name of the sprite
    ss << gameSprite->spriteDisplayedName;
    
    villagerNameLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerNameLabel->setColor(colorGreen);
    villagerNameLabel->setAnchorPoint(ccp(0, 1));
    
    
    // display the loyalth and hapiness of the sprite
    Possessions* possessions = gameSprite->getPossessions();
    
    /*
    ss.str(std::string());
    ss << possessions->loyaltyRating;
    
    villagerLoyLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerLoyLabel->setColor(colorGreen);
    villagerLoyLabel->setAnchorPoint(ccp(0, 1));
    
    ss.str(std::string());
    ss << possessions->happinessRating;
    
    villagerHapLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerHapLabel->setColor(colorGreen);
    villagerHapLabel->setAnchorPoint(ccp(0, 1));
    
    // display the level of the sprite
    ss.str(std::string());
    ss << "1";
    
    villagerLevelLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerLevelLabel->setColor(colorGreen);
    villagerLevelLabel->setAnchorPoint(ccp(0, 1));
    
     
    // display the movement speed of the sprite
    ss.str(std::string());
    ss << possessions->movementSpeed;
    
    movementSpeedLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
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
    villagerEnergyBar->setValue((float)possessions->energyRating / (float)possessions->defaultEnergy);
    
    // display the energy label of the sprite
    ss.str(std::string());
    ss << possessions->energyRating << "/" << possessions->defaultEnergy;
    
    villagerEnergyLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    villagerEnergyLabel->setColor(colorGreen);
    villagerEnergyLabel->setAnchorPoint(ccp(0, 1));
    
    // display the job of the sprite
    ss.str(std::string());
    ss << getJobString(gameSprite->getJob());
    
    jobLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
    jobLabel->setColor(colorGreen);
    jobLabel->setAnchorPoint(ccp(0, 1));
    
    // display the action of the sprite
    ss.str(std::string());
    ss << getActionString(gameSprite->getAction());
    
    actionLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
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
    
    isDongingJobLabel = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 24, CCSizeMake(ss.str().length() * 20.0f, 5.0f), kCCTextAlignmentLeft);
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
    else
    {
        // job button
        villagerSelectButton = CCMenuItemImage::create("job.png", "job.png", this, menu_selector(SpriteRow::doJob));
    }
    villagerSelectButton->setScale(64.0f / villagerSelectButton->boundingBox().size.width);
    villagerSelectButton->setAnchorPoint(ccp(0, 1));
    
    // the cancel job button!
    villagerCancelButton = CCMenuItemImage::create("cancel.png", "cancel.png", this, menu_selector(SpriteRow::cancelJob));
    villagerCancelButton->setScale(64.0f / villagerCancelButton->boundingBox().size.width);
    villagerCancelButton->setAnchorPoint(ccp(0, 1));
    
    menuItems->addObject(villagerSelectButton);
    menuItems->addObject(villagerCancelButton);
    
    if(gameSprite->getJob() == NONE)
    {
        villagerCancelButton->setVisible(false);
    }
    else
    {
        villagerSelectButton->setVisible(false);
    }
    
    menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // register all parts to the scroll area.
    scrollArea->addItem(villagerImage, ccp(20.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
    scrollArea->addItem(villagerNameLabel, ccp(20.0f + villagerImage->boundingBox().size.width + 10.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    /*
    scrollArea->addItem(villagerLoyLabel, ccp(200.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(villagerHapLabel, ccp(240.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(villagerLevelLabel, ccp(280.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(movementSpeedLabel, ccp(200.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    */
    
    scrollArea->addItem(villagerEnergyBar, ccp(200.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerEnergyBar->boundingBox().size.height / 2.0f));
    scrollArea->addItem(villagerEnergyLabel, ccp(280.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(jobLabel, ccp(380.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(actionLabel, ccp(460.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(isDongingJobLabel, ccp(550.0f, (5.0f + villagerImage->boundingBox().size.height) * index + villagerImage->boundingBox().size.height / 2.0f - villagerNameLabel->boundingBox().size.height / 2.0f));
    scrollArea->addItem(menu, ccp(620.0f, (5.0f + villagerImage->boundingBox().size.height) * index));
    
}

void SpriteRow::construction()
{
    //SelectPopulation::getThis()->closeMenu(true);
    gameSprite->setJob(BUILDER);
    gameSprite->setJobLocation(building);
    gameSprite->GoBuild(building);
    
    villagerSelectButton->setVisible(false);
    villagerCancelButton->setVisible(true);
}

void SpriteRow::doJob()
{
    villagerSelectButton->setVisible(false);
    villagerCancelButton->setVisible(true);
}

void SpriteRow::cancelJob()
{
    gameSprite->setJob(NONE);
    gameSprite->setJobLocation(NULL);
    gameSprite->setAction(IDLE);
    gameSprite->setIsDoingJob(false);
    
    villagerSelectButton->setVisible(true);
    villagerCancelButton->setVisible(false);
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