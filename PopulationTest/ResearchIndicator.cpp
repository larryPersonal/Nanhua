//
//  ResearchIndicator.cpp
//  PopulationTest
//
//  Created by Ong Heng Le on 10/5/13.
//
//

#include "ResearchIndicator.h"
#include "GameManager.h"

ResearchIndicator::ResearchIndicator()
{
    buildingIcon = NULL;
    progressBar = NULL;
    researchTimeTotal = 0;
}

ResearchIndicator::~ResearchIndicator()
{
}

void ResearchIndicator::createResearchIndicator()
{
    map<int, float>::iterator it = GameManager::getThis()->buildingIdResearchEndTime.begin();
    
    // Do nothing if no building is being researched
    if (it == GameManager::getThis()->buildingIdResearchEndTime.end())
        return;
    
    Building* building = GameScene::getThis()->buildingHandler->getBuilding(it->first);
    buildingIcon = CCSprite::createWithTexture(building->buildingTexture, building->buildingRect);
    buildingIcon->setScale(50.0f / buildingIcon->getContentSize().height);
    buildingIcon->setAnchorPoint(CCPointZero);
    
    progressBar = new ProgressBar();
    progressBar->createProgressBar(CCRectMake(0, 0, 250, 50),
                                   CCRectMake(5, 5, 240, 40),
                                   "loadingbar-empty.png",
                                   "loadingbar-left.png",
                                   "loadingbar-right.png",
                                   "loadingbar-full.png");
    
    researchTimeTotal = building->researchTime;     // get buildingID total research time
    float researchTimeElapsed = researchTimeTotal - (it->second - GameManager::getThis()->getCurrentTime());
    progressBar->setValue(researchTimeElapsed / researchTimeTotal);
    progressBar->setPosition(buildingIcon->boundingBox().size.width + 5.0f, 0.0f);
    
    this->addChild(buildingIcon);
    this->addChild(progressBar);
    
    this->schedule(schedule_selector(ResearchIndicator::update), 0.25f);
}

void ResearchIndicator::update(float deltaTime)
{
    map<int, float>::iterator it = GameManager::getThis()->buildingIdResearchEndTime.begin();
    
    // If it is empty, it means that the research has ended
    if (it == GameManager::getThis()->buildingIdResearchEndTime.end())
    {
        this->removeAllChildrenWithCleanup(true);
        this->unschedule(schedule_selector(ResearchIndicator::update));
        
        return;
    }
    
   // float researchTimeTotal = ;     // get buildingID total research time
    float researchTimeElapsed = researchTimeTotal - (it->second - GameManager::getThis()->getCurrentTime());
    progressBar->setValue(researchTimeElapsed / researchTimeTotal);
}












