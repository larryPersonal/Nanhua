//
//  PolicyMenu.cpp
//  PopulationTest
//
//  Created by Serious Games on 27/5/13.
//
//

#include "PolicyMenu.h"
#include "PolicyHandler.h"
#include "GameScene.h"
#include "GameHUD.h"
#include "GameDefaults.h"
#include "GlobalHelper.h"

#include "PolicyPopup.h"

PolicyMenu::~PolicyMenu()
{
    
    scrollArea->removeAllChildren();
    scrollArea->release();
    
    
}


void PolicyMenu::createMenuItems()
{
    GameManager* m = GameManager::getThis();
    
    //initialize the default number to import when the import x number of foreigners is clicked
    numberToInstantlyImport = 5;
    
    currTax = GameScene::getThis()->policyHandler->percentTax;
    deltaTax = 0;
    
    currPop = GameScene::getThis()->policyHandler->maxAlienPopulation;
    deltaPop = 0;
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float yPosition = 0.0f;
    
    std::stringstream ss;
    
    /*policy menu*/
    menuItems = CCArray::createWithCapacity(9);
    menuItems->retain();
    menuItemPositions = CCPointArray::create(menuItems->capacity());
    menuItemPositions->retain();
        
    CCPoint labelPosition;
    CCPoint label2Position;
    CCPoint label3Position;
    CCPoint label4Position;
    CCPoint timeLeftPosition;
    
    //Local Birth Rate
    bool isEnabled = m->isMenuItemUnlocked("menu_birthrate");
    if (isEnabled)
    {
        CCSprite* localBirthImg =     CCSprite::create("demolish.png");
        localBirthImg->               setScale(128 / localBirthImg->boundingBox().size.width);
        CCMenuItemSprite* localBirthItem =    CCMenuItemSprite::create(localBirthImg, localBirthImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
    
        localBirthItem->  setAnchorPoint(ccp(0, 1));
        localBirthItem->  setContentSize(localBirthImg->boundingBox().size);
        localBirthItem->setTag(1);
    
        CCLabelTTF* localBirthLabel =     CCLabelTTF::create("Promote Birth", "Arial", 30);
        localBirthNum =     CCLabelTTF::create(("Birth rate is " + GlobalHelper::Convert(GameScene::getThis()->policyHandler->monthly_localBirthRate)).c_str(), "Arial", 30);
    
    
        labelPosition = ccp(localBirthItem->boundingBox().size.width*2.3,
                            localBirthItem->boundingBox().size.height*0.3);
        label2Position = ccp(localBirthItem->boundingBox().size.width*2.3,
                                localBirthItem->boundingBox().size.height*0.6);

        timeLeftPosition = ccp(localBirthItem->boundingBox().size.width*2.3,
                               localBirthItem->boundingBox().size.height*0.8);

   
        if (GameScene::getThis()->policyHandler->getBirthRateDurationLeft() > 1)
        {
            localBirthTimeLeft =  CCLabelTTF::create((GlobalHelper::Convert(GameScene::getThis()->policyHandler->getBirthRateDurationLeft()) + " Months Left").c_str(), "Arial", 25);
        
        }
        else
        {
            localBirthTimeLeft =  CCLabelTTF::create("Final Month", "Arial", 25);
        }
    
        localBirthItem->  setPosition(CCPointMake(0, yPosition));
        localBirthLabel->     setPosition(labelPosition);
        localBirthNum->setPosition(label2Position);
    
        localBirthTimeLeft->setPosition(timeLeftPosition);
   
        localBirthItem->addChild(localBirthLabel);
        localBirthItem->addChild(localBirthNum);
        if (GameScene::getThis()->policyHandler->isImplementingBirthRatePolicy())
            localBirthItem->addChild(localBirthTimeLeft);


    
        menuItems->addObject(localBirthItem);

        yPosition -= (localBirthImg->boundingBox().size.height + 40);
    }
    /*
    if (!isEnabled)
    {
        localBirthItem->setEnabled(false);
        localBirthLabel->setColor(ccGRAY);
    }*/
    
    
    
    //Cultural Exchange Policy
    isEnabled = m->isMenuItemUnlocked("menu_culturalex");
    
    if (isEnabled)
    {
        CCSprite* culturalExImg =     CCSprite::create("demolish.png");
        culturalExImg->               setScale(128 / culturalExImg->boundingBox().size.width);
        CCMenuItemSprite* culturalExItem =    CCMenuItemSprite::create(culturalExImg, culturalExImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
    
        culturalExItem->  setAnchorPoint(ccp(0, 1));
        culturalExItem->  setContentSize(culturalExImg->boundingBox().size);
        culturalExItem->setTag(2);
        //ss << GameScene::getThis()->policyHandler->monthly_localBirthRate;
        CCLabelTTF* culturalExLabel =     CCLabelTTF::create("Promote Exchange", "Arial", 30);
        culturalExNum =     CCLabelTTF::create(("Cultural Exchange Rate Is " + GlobalHelper::Convert(GameScene::getThis()->policyHandler->percentCulturalExchangeRate) + "%").c_str(), "Arial", 30);
    
        labelPosition = ccp(culturalExItem->boundingBox().size.width*2.3,
                                culturalExItem->boundingBox().size.height*0.3);
        label2Position = ccp(culturalExItem->boundingBox().size.width*2.3,
                                 culturalExItem->boundingBox().size.height*0.6);
    
        timeLeftPosition = ccp(culturalExItem->boundingBox().size.width*2.3,
                               culturalExItem->boundingBox().size.height*0.8);
    
    
        if (GameScene::getThis()->policyHandler->getCulturalExchangeDurationLeft() > 1)
        {
            culturalExTimeLeft =  CCLabelTTF::create((GlobalHelper::Convert(GameScene::getThis()->policyHandler->getCulturalExchangeDurationLeft()) + " Months Left").c_str(), "Arial", 25);
            
        }
        else
        {
            culturalExTimeLeft =  CCLabelTTF::create("Final Month", "Arial", 25);
        }
    
    
        culturalExItem->  setPosition(CCPointMake(0, yPosition));
        culturalExLabel->     setPosition(labelPosition);
        culturalExNum->setPosition(label2Position);
    
        culturalExTimeLeft->setPosition(timeLeftPosition);
    
    
        culturalExItem->addChild(culturalExLabel);
        culturalExItem->addChild(culturalExNum);
        if (GameScene::getThis()->policyHandler->isImplementingCulturalExchangePolicy())
            culturalExItem->addChild(culturalExTimeLeft);

    
        menuItems->addObject(culturalExItem);
    
        yPosition -= (culturalExImg->boundingBox().size.height + 40);
    }
    /*
    if (!isEnabled)
    {
        culturalExItem->setEnabled(false);
        culturalExLabel->setColor(ccGRAY);
    }
    */
    //Tax rate modification.
    
    isEnabled = m->isMenuItemUnlocked("menu_taxrate");
    if (isEnabled)
    {
        CCSprite* taxRateImg =     CCSprite::create("demolish.png");
        taxRateImg->               setScale(128 / taxRateImg->boundingBox().size.width);
        CCMenuItemSprite* taxRateItem =    CCMenuItemSprite::create(NULL, NULL, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        
        taxRateItem->  setAnchorPoint(ccp(0, 1));
        taxRateItem->  setContentSize(taxRateImg->boundingBox().size);
        
        CCLabelTTF* taxRateLabel =     CCLabelTTF::create("Tax Rate", "Arial", 30);
        taxRateNum =     CCLabelTTF::create( (GlobalHelper::Convert(GameScene::getThis()->policyHandler->percentTax) + "%").c_str()  , "Arial", 30);
        
        taxRateChangeNum = CCLabelTTF::create(("+" + GlobalHelper::Convert(deltaTax)).c_str(), "Arial", 30);
        
        taxRateCost = CCLabelTTF::create((GlobalHelper::Convert(0)+" G").c_str(), "Arial", 30);
        
        
        labelPosition = ccp(taxRateItem->boundingBox().size.width*2.3,
                            taxRateItem->boundingBox().size.height*0.3);
        label2Position = ccp(taxRateItem->boundingBox().size.width*2.3,
                             taxRateItem->boundingBox().size.height*0.6);
        
        label3Position = ccp(taxRateItem->boundingBox().size.width * 2.5,
                                     taxRateItem->boundingBox().size.height * 0.1);
        label4Position = ccp(taxRateItem->boundingBox().size.width * 2.1,
                                     taxRateItem->boundingBox().size.height * 0.1);
        taxRateItem->  setPosition(CCPointMake(0, yPosition));
        taxRateLabel->     setPosition(labelPosition);
        taxRateNum->setPosition(label2Position);
        taxRateChangeNum->setPosition(label3Position);
        taxRateCost->setPosition(label4Position);
        
        taxRateItem->addChild(taxRateLabel);
        taxRateItem->addChild(taxRateNum);
        taxRateItem->addChild(taxRateChangeNum);
        taxRateItem->addChild(taxRateCost);
        
        
        //Up arrow for increment
        CCSprite* up2Img =     CCSprite::create("arrow-up2.png");
        CCSprite* up2SelImg =   CCSprite::create("arrow-up.png");
        up2Img->               setScale(128 / (up2Img->boundingBox().size.width * 2));
        up2SelImg->             setScale(up2Img->getScale());
        CCMenuItemSprite* taxRUpItem =    CCMenuItemSprite::create(up2Img, up2SelImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        taxRUpItem->  setAnchorPoint(ccp(0, 1));
        taxRUpItem->  setContentSize(up2Img->boundingBox().size);
        taxRUpItem->setPosition(ccp (labelPosition.x -(up2Img->boundingBox().size.width * 4), yPosition + up2Img->boundingBox().size.height * 0.25f));
        taxRUpItem->setTag(3);
        
        //Down arrow for decrement
        CCSprite* down2Img =     CCSprite::create("arrow-down2.png");
        CCSprite* down2SelImg =   CCSprite::create("arrow-down.png");
        down2Img->               setScale(128 / (down2Img->boundingBox().size.width * 2));
        down2SelImg->             setScale(down2Img->getScale());
        CCMenuItemSprite* taxRDownItem =    CCMenuItemSprite::create(down2Img, down2SelImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        taxRDownItem->  setAnchorPoint(ccp(0, 1));
        taxRDownItem->  setContentSize(down2Img->boundingBox().size);
        taxRDownItem->setPosition(ccp (labelPosition.x -(up2Img->boundingBox().size.width * 4), yPosition - up2Img->boundingBox().size.height * 0.75f));
        taxRDownItem->setTag(4);
        
        
        CCSprite* taxConfirmImg =     CCSprite::create("demolish.png");
        taxConfirmImg->               setScale(128 / (taxConfirmImg->boundingBox().size.width));
        CCMenuItemSprite* taxConfirmItem =    CCMenuItemSprite::create(taxConfirmImg, taxConfirmImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        taxConfirmItem->setPosition(ccp (labelPosition.x -(up2Img->boundingBox().size.width * 2), yPosition  - up2Img->boundingBox().size.height * 0.75f));
        taxConfirmItem->setTag(7);
        
        
        menuItems->addObject(taxRUpItem);
        menuItems->addObject(taxRDownItem);
        menuItems->addObject(taxRateItem);
        menuItems->addObject(taxConfirmItem);
        
        yPosition -= (taxRateImg->boundingBox().size.height + 40);
 
    }

    /*
    if (!isEnabled)
    {
        taxRUpItem->setEnabled(false);
        taxRDownItem->setEnabled(false);
        taxRateItem->setEnabled(false);
        taxConfirmItem->setEnabled(false);
        taxRateLabel->setColor(ccGRAY);
        taxRateNum->setColor(ccGRAY);
        taxRateChangeNum->setColor(ccGRAY);
        taxRateCost->setColor(ccGRAY);
        
    }*/
    
    
    //Alien population limit modification.
    isEnabled = m->isMenuItemUnlocked("menu_alienpoplimit");
    if (isEnabled)
    {
        CCSprite* aPopImg =     CCSprite::create("demolish.png");
        aPopImg->               setScale(128 / aPopImg->boundingBox().size.width);
        CCMenuItemSprite* aPopItem =    CCMenuItemSprite::create(NULL, NULL, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        
        aPopItem->  setAnchorPoint(ccp(0, 1));
        aPopItem->  setContentSize(aPopImg->boundingBox().size);
        
        CCLabelTTF* aPopLabel =     CCLabelTTF::create("Xenos Limit", "Arial", 30);
        aPopNum =     CCLabelTTF::create( GlobalHelper::Convert(GameScene::getThis()->policyHandler->maxAlienPopulation).c_str()  , "Arial", 30);
        
        aPopChangeNum = CCLabelTTF::create(("+" + GlobalHelper::Convert(deltaPop)).c_str(), "Arial", 30);
        
        aPopCost = CCLabelTTF::create((GlobalHelper::Convert(0) +" G").c_str(), "Arial", 30);
        
        
        labelPosition = ccp(aPopItem->boundingBox().size.width*2.3,
                            aPopItem->boundingBox().size.height*0.3);
        label2Position = ccp(aPopItem->boundingBox().size.width*2.3,
                             aPopItem->boundingBox().size.height*0.6);
        
        label3Position = ccp(aPopItem->boundingBox().size.width * 2.5,
                             aPopItem->boundingBox().size.height * 0.1);
        label4Position = ccp(aPopItem->boundingBox().size.width * 2.1,
                             aPopItem->boundingBox().size.height * 0.1);
        aPopItem->  setPosition(CCPointMake(0, yPosition));
        aPopLabel->     setPosition(labelPosition);
        aPopNum->setPosition(label2Position);
        aPopChangeNum->setPosition(label3Position);
        aPopCost->setPosition(label4Position);
        
        aPopItem->addChild(aPopLabel);
        aPopItem->addChild(aPopNum);
        aPopItem->addChild(aPopChangeNum);
        aPopItem->addChild(aPopCost);
        
        
        //Up arrow for increment
        CCSprite* up3Img =     CCSprite::create("arrow-up2.png");
        CCSprite* up3SelImg =   CCSprite::create("arrow-up.png");
        up3Img->               setScale(128 / (up3Img->boundingBox().size.width * 2));
        up3SelImg->             setScale(up3Img->getScale());
        CCMenuItemSprite* aPopUpItem =    CCMenuItemSprite::create(up3Img, up3SelImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        aPopUpItem->  setAnchorPoint(ccp(0, 1));
        aPopUpItem->  setContentSize(up3Img->boundingBox().size);
        aPopUpItem->setPosition(ccp (labelPosition.x -(up3Img->boundingBox().size.width * 4), yPosition + up3Img->boundingBox().size.height * 0.25f));
        aPopUpItem->setTag(8);
        
        //Down arrow for decrement
        CCSprite* down3Img =     CCSprite::create("arrow-down2.png");
        CCSprite* down3SelImg =   CCSprite::create("arrow-down.png");
        down3Img->               setScale(128 / (down3Img->boundingBox().size.width * 2));
        down3SelImg->             setScale(down3Img->getScale());
        CCMenuItemSprite* aPopDownItem =    CCMenuItemSprite::create(down3Img, down3SelImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        aPopDownItem->  setAnchorPoint(ccp(0, 1));
        aPopDownItem->  setContentSize(down3Img->boundingBox().size);
        aPopDownItem->setPosition(ccp (labelPosition.x -(up3Img->boundingBox().size.width * 4), yPosition - up3Img->boundingBox().size.height * 0.75f));
        aPopDownItem->setTag(9);
        
        
        CCSprite* aPopConfirmImg =     CCSprite::create("demolish.png");
        aPopConfirmImg->               setScale(128 / (aPopConfirmImg->boundingBox().size.width));
        CCMenuItemSprite* aPopConfirmItem =    CCMenuItemSprite::create(aPopConfirmImg, aPopConfirmImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        aPopConfirmItem->setPosition(ccp (labelPosition.x -(up3Img->boundingBox().size.width * 2), yPosition  - up3Img->boundingBox().size.height * 0.75f));
        aPopConfirmItem->setTag(7);
        
        
        
        menuItems->addObject(aPopUpItem);
        menuItems->addObject(aPopDownItem);
        menuItems->addObject(aPopItem);
        menuItems->addObject(aPopConfirmItem);
        
        yPosition -= (aPopImg->boundingBox().size.height + 40);

    }
    
     /* import citizen */
    isEnabled = m->isMenuItemUnlocked("menu_aliencitizen");
    
    if (isEnabled)
    {
        CCSprite* importCImg =     CCSprite::create("demolish.png");
        importCImg->               setScale(128 / importCImg->boundingBox().size.width);
        CCMenuItemSprite* importCItem =    CCMenuItemSprite::create(importCImg, NULL, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        
        importCItem->  setAnchorPoint(ccp(0, 1));
        importCItem->  setContentSize(importCImg->boundingBox().size);
        
        ss.str(std::string());
        ss << GameScene::getThis()->policyHandler->monthly_alienImportRate;
        CCLabelTTF* importCLabel =     CCLabelTTF::create("Import Xenos Citizen", "Arial", 30);
        CCLabelTTF* importCCost = CCLabelTTF::create((GlobalHelper::Convert(import_alien_citizen_cost) +" G").c_str(), "Arial", 30);
        
        labelPosition = ccp(importCItem->boundingBox().size.width*2.3,
                            importCItem->boundingBox().size.height*0.3);
        label2Position = ccp(importCItem->boundingBox().size.width*2.3,
                             importCItem->boundingBox().size.height*0.6);
        
        importCItem->  setPosition(CCPointMake(0, yPosition));
        importCLabel->     setPosition(label2Position);
        importCCost->setPosition(labelPosition);
        importCItem->addChild(importCLabel);
        importCItem->addChild(importCCost);
        
        menuItems->addObject(importCItem);
        importCItem->setTag(5);
        
        yPosition -= (importCImg->boundingBox().size.height + 40);

    }
    
    
    /* import researcher */
    isEnabled = m->isMenuItemUnlocked("menu_alienresearcher");
    if (isEnabled)
    {
        
        CCSprite* importRImg =     CCSprite::create("demolish.png");
        importRImg->               setScale(128 / importRImg->boundingBox().size.width);
        CCMenuItemSprite* importRItem =    CCMenuItemSprite::create(importRImg, NULL, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        
        importRItem->  setAnchorPoint(ccp(0, 1));
        importRItem->  setContentSize(importRImg->boundingBox().size);
        
        // ss.str(std::string());
        // ss << GameScene::getThis()->policyHandler->monthly_alienImportRate;
        CCLabelTTF* importRLabel =     CCLabelTTF::create("Import Xenos Researcher", "Arial", 30);
        CCLabelTTF* importRCost = CCLabelTTF::create((GlobalHelper::Convert(import_alien_researcher_cost) +" G").c_str(), "Arial", 30);
        
        labelPosition = ccp(importRItem->boundingBox().size.width*2.3,
                            importRItem->boundingBox().size.height*0.3);
        label2Position = ccp(importRItem->boundingBox().size.width*2.3,
                             importRItem->boundingBox().size.height*0.6);
        
        importRItem->  setPosition(CCPointMake(0, yPosition));
        importRLabel->     setPosition(label2Position);
        importRCost->setPosition(labelPosition);
        importRItem->addChild(importRLabel);
        importRItem->addChild(importRCost);
        
        menuItems->addObject(importRItem);
        importRItem->setTag(6);
        yPosition -= (importRImg->boundingBox().size.height + 40);
        

    }
    
    
    isEnabled = testMode;
    if (isEnabled)
    {
        
        CCSprite* a2PopImg =     CCSprite::create("demolish.png");
        a2PopImg->               setScale(128 / a2PopImg->boundingBox().size.width);
        CCMenuItemSprite* a2PopItem =    CCMenuItemSprite::create(NULL, NULL, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        
        a2PopItem->  setAnchorPoint(ccp(0, 1));
        a2PopItem->  setContentSize(a2PopImg->boundingBox().size);
        
        CCLabelTTF* a2PopLabel =     CCLabelTTF::create("Import Citizens", "Arial", 30);
        a2PopNum =     CCLabelTTF::create( GlobalHelper::Convert(toSpawn).c_str()  , "Arial", 30);
        
      
        
        labelPosition = ccp(a2PopItem->boundingBox().size.width*2.3,
                            a2PopItem->boundingBox().size.height*0.3);
        label2Position = ccp(a2PopItem->boundingBox().size.width*2.3,
                             a2PopItem->boundingBox().size.height*0.6);
        
        a2PopItem->  setPosition(CCPointMake(0, yPosition));
        a2PopLabel->     setPosition(labelPosition);
        a2PopNum->setPosition(label2Position);
        
        a2PopItem->addChild(a2PopLabel);
        a2PopItem->addChild(a2PopNum);
        
        
        //Up arrow for increment
        CCSprite* up4Img =     CCSprite::create("arrow-up2.png");
        CCSprite* up4SelImg =   CCSprite::create("arrow-up.png");
        up4Img->               setScale(128 / (up4Img->boundingBox().size.width * 2));
        up4SelImg->             setScale(up4Img->getScale());
        CCMenuItemSprite* a2PopUpItem =    CCMenuItemSprite::create(up4Img, up4SelImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        a2PopUpItem->  setAnchorPoint(ccp(0, 1));
        a2PopUpItem->  setContentSize(up4Img->boundingBox().size);
        a2PopUpItem->setPosition(ccp (labelPosition.x -(up4Img->boundingBox().size.width * 4), yPosition + up4Img->boundingBox().size.height * 0.25f));
        a2PopUpItem->setTag(10);
        
        //Down arrow for decrement
        CCSprite* down4Img =     CCSprite::create("arrow-down2.png");
        CCSprite* down4SelImg =   CCSprite::create("arrow-down.png");
        down4Img->               setScale(128 / (down4Img->boundingBox().size.width * 2));
        down4SelImg->             setScale(down4Img->getScale());
        CCMenuItemSprite* a2PopDownItem =    CCMenuItemSprite::create(down4Img, down4SelImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        a2PopDownItem->  setAnchorPoint(ccp(0, 1));
        a2PopDownItem->  setContentSize(down4Img->boundingBox().size);
        a2PopDownItem->setPosition(ccp (labelPosition.x -(up4Img->boundingBox().size.width * 4), yPosition - up4Img->boundingBox().size.height * 0.75f));
        a2PopDownItem->setTag(11);
        
        CCSprite* a2PopConfirmImg =     CCSprite::create("demolish.png");
        a2PopConfirmImg->               setScale(128 / (a2PopConfirmImg->boundingBox().size.width));
        CCMenuItemSprite* a2PopConfirmItem =    CCMenuItemSprite::create(a2PopConfirmImg, a2PopConfirmImg, this, menu_selector(PolicyMenu::onMenuItemSelected) );
        a2PopConfirmItem->setPosition(ccp (labelPosition.x -(up4Img->boundingBox().size.width * 2), yPosition  - up4Img->boundingBox().size.height * 0.75f));
        a2PopConfirmItem->setTag(12);
        
        menuItems->addObject(a2PopUpItem);
        menuItems->addObject(a2PopDownItem);
        menuItems->addObject(a2PopItem);
        menuItems->addObject(a2PopConfirmItem);
        
        yPosition -= (a2PopImg->boundingBox().size.height + 40);

        
    }
    
    
    CCMenu* menu = CCMenu::createWithArray(menuItems);
    menu->setPosition(CCPointZero);
    
    // Build Menu UI vars
    CCSize PolicyMenuSize = CCSizeMake(622.0f, 370.0f);
    //ccColor3B colorYellow = ccc3(225, 219, 108);
    
    // Scroll Area
    scrollArea = new ScrollArea();
    scrollArea->createScrollArea(PolicyMenuSize, CCSizeMake(0.0f, -(yPosition - 200)));
    scrollArea->addItem(menu, ccp(20.0f, 0.0f));
    scrollArea->enableScrollVertical(10, "bar.png", "bar.png");
    scrollArea->hideScroll();
    scrollArea->reposition();
    scrollArea->updateScrollBars();
    
    
      // Tabbed View
    tabbedView = new TabbedView();
    
    // Tab Menu Background
    CCSprite* tabbedViewBg = CCSprite::create("outerbox.png");
    tabbedViewBg->setAnchorPoint(CCPointZero);
    tabbedViewBg->setScale(700.0f / tabbedViewBg->getContentSize().width);
    
    CCSprite* tabbedViewBgInner = CCSprite::create("innerbox-policy.png");
    tabbedViewBgInner->setAnchorPoint(CCPointZero);
    tabbedViewBgInner->setPosition(ccp(10, 10));
    tabbedViewBgInner->setScale(tabbedViewBg->getScale());
    // Tab Layers
    tabLayerPolicies = CCLayer::create();
    tabLayerPolicies->setContentSize(PolicyMenuSize);
    tabLayerPolicies->addChild(scrollArea);
    
    // Tab Items
    tabItemPolicies= CCMenuItemImage::create(NULL, NULL);
    //tabItemPolicies->setAnchorPoint(CCPointZero);
    tabItemPolicies->setVisible(false);
    // Create tabbed view and add its items
    tabbedView->createTabbedView(ccp(0.0f, tabbedViewBg->getContentSize().height * tabbedViewBg->getScale()),
                                 ccp(42.0f, 55.0f));
    tabbedView->addChild(tabbedViewBg);
    tabbedView->addChild(tabbedViewBgInner);
    tabbedView->addTabWithContent(tabItemPolicies, ccp(218.0f, -20.0f), tabLayerPolicies);
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    
    this->addChild(tabbedView);
    
    TitleLabel = CCLabelTTF::create("Policy", "Vinland", 64);
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
    this->addChild(TitleLabel,4);
    
}

void PolicyMenu::onMenuItemSelected(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    
    
    switch (tag)
    {
       //note: do not close menus 
        case 1:
        {
            if (!GameScene::getThis()->policyHandler->isImplementingBirthRatePolicy())
            {
                PolicyPopup* p = PolicyPopup::create();
                p->useAsTopmostPopupMenu();
                p->setupBirthPolicy();
            }
        }
        break;
        
        case 2:
        {
            if (!GameScene::getThis()->policyHandler->isImplementingCulturalExchangePolicy())
            {
            
            
                PolicyPopup* p = PolicyPopup::create();
                p->useAsTopmostPopupMenu();
                p->setupInteractPolicy();
            }
            
        }
        break;
            
            
            
        case 3:
        {
            if (currTax + deltaTax < max_tax_rate_percent)
            {
                deltaTax += 1;
            }
            //if ( > 100) currTax = 100;
            
            
            if (deltaTax < 0)
                taxRateChangeNum->setString(GlobalHelper::Convert(deltaTax).c_str());
            else
                taxRateChangeNum->setString(("+" + GlobalHelper::Convert(deltaTax)).c_str());
            
            int cost = abs(deltaTax) * cost_per_tax_rate_percent;
             taxRateCost->setString(("$"+GlobalHelper::Convert(cost)).c_str());
            
            /*
            int newBirthRate = GameScene::getThis()->policyHandler->monthly_alienImportRate;
            newBirthRate = (newBirthRate >= 10)? newBirthRate: newBirthRate+1;
            GameScene::getThis()->policyHandler->monthly_alienImportRate = newBirthRate;
            std::stringstream ss;
            ss << newBirthRate;
            taxRateNum->setString(ss.str().c_str());
            */
        }
        break;
            
        case 4:
        {
            if (currTax + deltaTax > min_tax_rate_percent)
            {
                deltaTax -= 1;
            }
            //if ( > 100) currTax = 100;
            
            if (deltaTax < 0)
                taxRateChangeNum->setString(GlobalHelper::Convert(deltaTax).c_str());
            else
                taxRateChangeNum->setString(("+" + GlobalHelper::Convert(deltaTax)).c_str());
            
            int cost = abs(deltaTax) * cost_per_tax_rate_percent;
            taxRateCost->setString(("$"+GlobalHelper::Convert(cost)).c_str());
            
            
            /*
            int newBirthRate = GameScene::getThis()->policyHandler->monthly_alienImportRate;
            newBirthRate = (newBirthRate <= 0)? newBirthRate : newBirthRate-1;
            GameScene::getThis()->policyHandler->monthly_alienImportRate = newBirthRate;
            std::stringstream ss;
            ss << newBirthRate;
            taxRateNum->setString(ss.str().c_str());
            */
        }
            break;
        case 5:
        {
//            std::string classname = "citizen";
            int alienPop = GameScene::getThis()->spriteHandler->getAlienCount();
            
            
            
            if (alienPop < GameScene::getThis()->policyHandler->maxAlienPopulation)
            {
                if (GameManager::getThis()->currentMoney >= import_alien_citizen_cost)
                {
                
                
                    GameManager::getThis()->currentMoney -= import_alien_citizen_cost;
                    GameHUD::getThis()->updateMoneyLabel();

                    int gender = rand() % 2;
                    if (gender > 0)
                        GameScene::getThis()->policyHandler->SpawnImmediately(M_ALIEN_CITIZEN);
                    else
                        GameScene::getThis()->policyHandler->SpawnImmediately(F_ALIEN_CITIZEN);
            
                    this->closeMenu();
                }
            }
            else
            {
                
            }
        }
            break;
        
        case 6:
        {
  //          std::string classname = "researcher";
            int alienPop = GameScene::getThis()->spriteHandler->getAlienCount();
            
            
            
            if (alienPop < GameScene::getThis()->policyHandler->maxAlienPopulation)
            {
                if (GameManager::getThis()->currentMoney >= import_alien_researcher_cost)
                {
                    
                    
                    GameManager::getThis()->currentMoney -= import_alien_researcher_cost;
                    GameHUD::getThis()->updateMoneyLabel();
                    int gender = rand() % 2;
                    if (gender > 0)
                        GameScene::getThis()->policyHandler->SpawnImmediately(M_ALIEN_RESEARCHER);
                    else
                        GameScene::getThis()->policyHandler->SpawnImmediately(F_ALIEN_RESEARCHER);
                    
                    
                    this->closeMenu();
                    
                }
            }
            else
            {
                
            }
            
            
        }
         break;
            
        case 7:
        {
            int cost = abs(deltaTax) * cost_per_tax_rate_percent;
            if (GameManager::getThis()->currentMoney >= cost)
            {
                GameManager::getThis()->currentMoney -= cost;
                
                currTax += deltaTax;
                GameScene::getThis()->policyHandler->percentTax = currTax;
                deltaTax = 0;
                
                taxRateCost->setString(("$"+ GlobalHelper::Convert(0)).c_str());
                taxRateChangeNum->setString(GlobalHelper::Convert(0).c_str());
                taxRateNum->setString((GlobalHelper::Convert(currTax)+ "%").c_str());
                
                GameHUD::getThis()->updateMoneyLabel();
                
            }
            else
            {
                
            }
            
            
            cost = abs(deltaPop) * cost_per_1_alien_limit_adj;
            if (GameManager::getThis()->currentMoney >= cost)
            {
                GameManager::getThis()->currentMoney -= cost;
                
                currPop += deltaPop;
                GameScene::getThis()->policyHandler->maxAlienPopulation = currPop;
                deltaPop = 0;
                
                aPopCost->setString(("$"+ GlobalHelper::Convert(0)).c_str());
                aPopChangeNum->setString(GlobalHelper::Convert(0).c_str());
                aPopNum->setString((GlobalHelper::Convert(currPop)).c_str());
                
                GameHUD::getThis()->updateMoneyLabel();
                
            }
            else
            {
                
            }
            
            
            
        }
            break;
            
            
            
            
        case 8:
        {
            //no maximum alien population
            /*
             if (currPop + deltaPop < max_ali)
             {
             deltaPop += 1;
             }*/
            //if ( > 100) currTax = 100;
            deltaPop += 1;
            
            if (deltaPop < 0)
                aPopChangeNum->setString(GlobalHelper::Convert(deltaPop).c_str());
            else
                aPopChangeNum->setString(("+" + GlobalHelper::Convert(deltaPop)).c_str());
            
            int cost = abs(deltaPop) * cost_per_1_alien_limit_adj;
            aPopCost->setString(("$"+GlobalHelper::Convert(cost)).c_str());
            
            
            
        }
            break;
            
        case 9:
        {
           
            
            if (currPop + deltaPop > 0)
            {
                deltaPop -= 1;
            }
            
            if (deltaPop < 0)
                aPopChangeNum->setString(GlobalHelper::Convert(deltaPop).c_str());
            else
                aPopChangeNum->setString(("+" + GlobalHelper::Convert(deltaPop)).c_str());
            
            int cost = abs(deltaPop) * cost_per_1_alien_limit_adj;
            aPopCost->setString(("$"+GlobalHelper::Convert(cost)).c_str());
            
            
        }
            break;
            
            
        case 10:
            toSpawn += 1;
            if (toSpawn > 10) toSpawn = 10;
            a2PopNum->setString(GlobalHelper::Convert(toSpawn).c_str());
            break;
        case 11:
            toSpawn -= 1;
            if (toSpawn < 1) toSpawn = 1;
            a2PopNum->setString(GlobalHelper::Convert(toSpawn).c_str());
            
            break;
        case 12:
            GameScene::getThis()->policyHandler->ImportForeignersInBuilding((Building*)GameScene::getThis()->buildingHandler->specialOnMap->objectAtIndex(0), toSpawn);
            break;
    }

        
        
}

void PolicyMenu::onOrientationChanged()
{
    PopupMenu::onOrientationChanged();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    tabbedView->setPosition((screenSize.width - 700.0f) / 2.0f, (screenSize.height - 600.0f) / 2.0f);
    if (screenSize.width > screenSize.height) // landscape
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
    else
        TitleLabel->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.65f));
    
}

