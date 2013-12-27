//
//  BuildingHandler.cpp
//  PopulationTest
//
//  Created by Serious Games on 12/3/13.
//
//

#include "BuildingHandler.h"

#include "GameScene.h"
#include "GlobalHelper.h"
#include <sstream>
#include <string>
#include <iostream>

BuildingHandler::BuildingHandler()
{
    allBuildingsOnMap = NULL;
    
    housingOnMap = NULL;
    amenityOnMap = NULL;
    commerceOnMap = NULL;
    militaryOnMap = NULL;
    educationOnMap = NULL;
    socialOnMap = NULL;
}

BuildingHandler::~BuildingHandler()
{
    housingOnMap->removeAllObjects();
        
    amenityOnMap->removeAllObjects();
    commerceOnMap->removeAllObjects();
    militaryOnMap->removeAllObjects();
    educationOnMap->removeAllObjects();
    socialOnMap->removeAllObjects();
    specialOnMap->removeAllObjects();
    
    housingOnMap->release();
    amenityOnMap->release();
    commerceOnMap->release();
    militaryOnMap->release();
    educationOnMap->release();
    socialOnMap->release();
    specialOnMap->release();
    
    if (allBuildingsOnMap)
    {
        /*
        for (int i = 0; i < allBuildingsOnMap->count(); ++i)
        {
           allBuildingsOnMap->objectAtIndex(i)->release();
        }*/
        if (allBuildingsOnMap->count() > 0)
            allBuildingsOnMap->removeAllObjects();
        allBuildingsOnMap->release();
    }

    allBuildingLayers->removeAllObjects();
    allBuildingLayers->release();
    
    if (allBuildings)
    {
        
        /*
        for (int i = 0; i < allBuildings->count(); ++i)
        {
            allBuildings->objectAtIndex(i)->release();
        }*/
        if (allBuildings->count() > 0)
            allBuildings->removeAllObjects();
        allBuildings->release();
    }

}


Building* BuildingHandler::BinarySearch(cocos2d::CCArray *targetArray, int targetID)
{
    int bottom = 0;
    int top = targetArray->count() -1;
    
    Building* target = NULL;
    while(bottom <= top)
    {
        int midpt = bottom + (top - bottom) * 0.5f;
        target = (Building*)targetArray->objectAtIndex(midpt);
        
        if (target->ID == targetID) break;
        
        
        if (target->ID < targetID)
            bottom = midpt + 1;
         else
             top = midpt -1;
        
        
    }
    if (target == NULL)
        return NULL;
    if (target->ID != targetID)
        return NULL;
    
    
    return target;
}

void BuildingHandler::init(cocos2d::CCTMXTiledMap *mapPtr, JobCollection* jc)
{
    /* I'm gonna get all layers and all buildings from here. If you wanna restrict layers, DIY, but for now I'm treating every tile type as a building,
     even grass. */
    
    /*Note: the layer must exist on the map through a BUILDING already placed on it. I bypassed this by adding tiles marked "ignore_this_blank" on each layer, then adding it to the map. */
    
    int buildingID = 0;
    allBuildings = CCArray::create();
   // for (int i = 0; i < mapPtr->)
    
 //   JobCollection *jc = GameScene::getThis()->jobCollection;
    
    
    
    allBuildingLayers= CCArray::create();
    for (int i = 0; i < mapPtr->getChildrenCount(); i++)
    {
        
        CCTMXLayer* layer = (CCTMXLayer*)mapPtr->getChildren()->objectAtIndex(i);
         
        if (strncmp(layer->getLayerName(), "Building", strlen("Building")) == 0)
        {
            allBuildingLayers->addObject(layer);
            layer->setVisible(false);
            int startIndex = layer->getTileSet()->m_uFirstGid + atoi(layer->propertyNamed("start_tile_offset")->getCString());
            int endIndex = startIndex +  atoi(layer->propertyNamed("number_of_tiles")->getCString());
                                              
            
            
            for (int j = startIndex; j < endIndex ; ++j)
            {
                Building* b = Building::create();
                b->targetLayerName = layer->getLayerName();
                b->targetGUID = j;
                
                b->buildingTexture = CCTextureCache::sharedTextureCache()->addImage( layer->getTileSet()->m_sSourceImage.c_str());
                b->buildingTexture->retain();
                b->buildingRect = layer->getTileSet()->rectForGID(b->targetGUID);
                
                b->width = b->buildingRect.size.width / 128;
                b->height = b->buildingRect.size.height / 128; //128 being the standard tile size in the TMX file.
                
                CCDictionary* properties = mapPtr->propertiesForGID(b->targetGUID);
                if (properties)
                {
                    const CCString* currProperty;
                    /*
                    currProperty = properties->valueForKey("jobs_available");
                   
                     if (currProperty)
                    {
                        b->jobs = currProperty->getCString();
              
                        
                    }
                    else b->jobs = "";
                    //else the jobs array for this building is null or empty.
                    */
                    
                    currProperty = properties->valueForKey("name");
                    if (currProperty)
                    {
                        b->buildingName= currProperty->getCString();  //CCStringMake("PlaceHolder");
                        if (b->buildingName.compare("Farm")==0)
                        {
                            
                            CCLog("test");
                        }
                    }
                    else
                        b->buildingName= "No Name";//";  //CCStringMake("PlaceHolder");
                    
                    currProperty = properties->valueForKey("description");
                    if (currProperty)
                        b->buildingDesc= currProperty->getCString();  //CCStringMake("PlaceHolder");
                    else
                        b->buildingDesc= "No Name";//";  //CCStringMake("PlaceHolder");
                    
                    currProperty = properties->valueForKey("cost");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("cost")->getCString());
                        b->buildingCost = atoi(cost->getCString());
                     
                    }
                    else
                        b->buildingCost = 9999.0f;
              
                    
                    currProperty = properties->valueForKey("research_cost");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("research_cost")->getCString());
                        b->researchCost = atoi(cost->getCString());
                    }
                    else
                        b->researchCost = 100.0f;
                    
                    currProperty = properties->valueForKey("own_cost");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("own_cost")->getCString());
                        b->buildingBuyPrice = atoi(cost->getCString());
                        
                    }
                    else
                        b->buildingBuyPrice = 9999.0f;
                    /*
                    currProperty = properties->valueForKey("rent_cost");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("rent_cost")->getCString());
                        b->buildingRentPrice = atoi(cost->getCString());
                        //b->cash_mod = b->buildingRentPrice;
                    }
                    else
                    {
                        b->buildingRentPrice = 9999.0f;
                        //b->cash_mod = 0;
                    }*/
                    /*
                    currProperty = properties->valueForKey("mod_cash");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("mod_cash")->getCString());
                        b->cash_mod = atoi(cost->getCString());
                    }
                    else
                    {
                        b->cash_mod = -10;
                    }*/
                    
                    
                    currProperty = properties->valueForKey("maint_cost");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("maint_cost")->getCString());
                        b->maintCost = atoi(cost->getCString());
                    }
                    else
                    {
                        b->maintCost = 0.0f;
                    }
                    
                    currProperty = properties->valueForKey("population_limit");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("population_limit")->getCString());
                        b->populationLimit = atoi(cost->getCString());
                        
                    }
                    else
                        b->populationLimit = 0;
                    
                    
                    /*currProperty = properties->valueForKey("population_overload");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("population_overload")->getCString());
                        b->populationOverload = atoi(cost->getCString());
                        
                    }
                    else
                        b->populationOverload = 0;
                    
                    
                    currProperty = properties->valueForKey("mod_int");
                    if (currProperty)
                    {
                        CCString* modInt = CCStringMake(properties->valueForKey("mod_int")->getCString());
                        b->int_mod = atoi(modInt->getCString());
                    }
                    else
                        b->int_mod = 0;
                    
                    
                    currProperty = properties->valueForKey("mod_soc");
                    if (currProperty)
                    {
                        CCString* modSoc = CCStringMake(properties->valueForKey("mod_soc")->getCString());
                        b->social_mod = atoi(modSoc->getCString());
                    }
                    else
                        b->social_mod = 0;
                    
                    
                    currProperty = properties->valueForKey("mod_loy");
                    if (currProperty)
                    {
                        CCString* modLoy = CCStringMake(properties->valueForKey("mod_loy")->getCString());
                        b->loyalty_mod = atoi(modLoy->getCString());
                    }
                    else
                        b->loyalty_mod = 0;

                    
                    currProperty = properties->valueForKey("mod_hap");
                    if (currProperty)
                    {
                        CCString* modHap = CCStringMake(properties->valueForKey("mod_hap")->getCString());
                        b->happiness_mod = atoi(modHap->getCString());
                    }
                    else
                        b->happiness_mod = 0;
                    
                    */
                    // Example format in Tiled: (level_exp_req) 100, 120, 140, 160, 0
                    currProperty = properties->valueForKey("level_exp_req");
                 //   b->expToLevel = CCArray::create();
                 //   b->expToLevel->retain();
                    if (currProperty->length() > 0)
                    {
                        
                        std::string str = properties->valueForKey("level_exp_req")->m_sString;
                        str = GlobalHelper::stringRemoveDelim(str, ' ');    //remove all whitespaces
                        vector<string> vec;
                        GlobalHelper::splitString(str, ',', vec);
                        
                        for (std::vector<string>::iterator it = vec.begin() ; it != vec.end(); ++it)
                            b->expToLevel->addObject(CCInteger::create(CCString::create(*it)->intValue()));
                    }
                    else
                        b->expToLevel->initWithObject(CCInteger::create(0));
                    
                    
                    // Example format in Tiled: (level_gain_population_limit) 2:1, 4:1
                    b->levelGainVacancy = CCDictionary::create();
                    b->levelGainVacancy->retain();
                    currProperty = properties->valueForKey("level_gain_population_limit");
                    if (currProperty->length() > 0)
                    {
                        std::string str = properties->valueForKey("level_gain_population_limit")->m_sString;
                        str = GlobalHelper::stringRemoveDelim(str, ' ');    //remove all whitespaces
                        vector<string> vec;
                        GlobalHelper::splitString(str, ',', vec);
                        
                        for (std::vector<string>::iterator it = vec.begin() ; it != vec.end(); ++it)
                        {
                            vector<string> elemVec;
                            GlobalHelper::splitString(*it, ':', elemVec);
                            b->levelGainVacancy->setObject(CCInteger::create(CCString::create(elemVec[1])->intValue()), atoi(elemVec[0].c_str()));
                        }
                    }
                    
                    // Example format in Tiled: (level_gain_population_overload) 2:1, 4:1
                    b->levelGainPopOverload = CCDictionary::create();
                    b->levelGainPopOverload->retain();
                    currProperty = properties->valueForKey("level_gain_population_overload");
                    if (currProperty->length() > 0)
                    {
                        std::string str = properties->valueForKey("level_gain_population_overload")->m_sString;
                        str = GlobalHelper::stringRemoveDelim(str, ' ');    //remove all whitespaces
                        vector<string> vec;
                        GlobalHelper::splitString(str, ',', vec);
                        
                        for (std::vector<string>::iterator it = vec.begin() ; it != vec.end(); ++it)
                        {
                            vector<string> elemVec;
                            GlobalHelper::splitString(*it, ':', elemVec);
                            b->levelGainPopOverload->setObject(CCInteger::create(CCString::create(elemVec[1])->intValue()), atoi(elemVec[0].c_str()));
                        }
                    }
                    
                    // Example format in Tiled: (level_gain_cost) 3:20, 4:20
                    b->levelGainPrice = CCDictionary::create();
                    b->levelGainPrice->retain();
                    currProperty = properties->valueForKey("level_gain_cost");
                    if (currProperty->length() > 0)
                    {
                        std::string str = properties->valueForKey("level_gain_cost")->m_sString;
                        str = GlobalHelper::stringRemoveDelim(str, ' ');    //remove all whitespaces
                        vector<string> vec;
                        GlobalHelper::splitString(str, ',', vec);
                        
                        for (std::vector<string>::iterator it = vec.begin() ; it != vec.end(); ++it)
                        {
                            vector<string> elemVec;
                            GlobalHelper::splitString(*it, ':', elemVec);
                            b->levelGainPrice->setObject(CCInteger::create(CCString::create(elemVec[1])->intValue()), atoi(elemVec[0].c_str()));
                        }
                    }
                    
                    currProperty = properties->valueForKey("unlocked_at_start");
                    if (currProperty)
                    {
                       if (currProperty->compare("yes") == 0)
                       {
                           b->unlocked = true;
                       }
                       else
                       {
                           b->unlocked = false;
                       }
                    }
                    else
                    {
                        b->unlocked = false;
                    }
                    
                    currProperty = properties->valueForKey("building_type");
                    if (currProperty)
                    {
                        
                             /*I don't have a way to shorten this. Fortunately I only need to do this at the beginning of the game.*/
                            std::string type = currProperty->getCString();
                            if (type == "housing")
                            {
                                b->buildingType = HOUSING;
                            }
                            if (type == "amenity")
                            {
                                b->buildingType = AMENITY;
                            }
                            if (type == "commerce")
                            {
                                b->buildingType = COMMERCE;
                            }
                            if (type == "military")
                            {
                                b->buildingType = MILITARY;
                            }
                            if (type == "education")
                            {
                                b->buildingType = EDUCATION;
                            }
                            if (type == "social")
                            {
                                b->buildingType = SOCIAL;
                            }
                            if (type == "special")
                            {
                                b->buildingType = SPECIAL;
                            }
                            if (type == "")
                                b->buildingType = BUILDINGCATEGORYMAX;
                        
                    }
                    else
                    {
                        b->buildingType = BUILDINGCATEGORYMAX;
                    }                    
                    
                    
                    /*requirements*/
                    currProperty = properties->valueForKey("req_population");
                    if (currProperty)
                    {
                        CCString* reqPop = CCStringMake(properties->valueForKey("req_population")->getCString());
                        b->required_population = atoi(reqPop->getCString());
                    }
                    
                    currProperty = properties->valueForKey("prereqs");
                    if (currProperty)
                    {
                        CCString* reqs = CCStringMake(properties->valueForKey("prereqs")->getCString());
                        b->prereq = reqs->getCString();
                    }
                    
                    
                    currProperty = properties->valueForKey("req_capita");
                    if (currProperty)
                    {
                        CCString* reqCap = CCStringMake(properties->valueForKey("req_capita")->getCString());
                        b->required_capita = atoi(reqCap->getCString());
                    }

                    currProperty = properties->valueForKey("req_building_count");
                    if (currProperty)
                    {
                        CCString* reqB = CCStringMake(properties->valueForKey("req_building_count")->getCString());
                        b->required_building_count = atoi(reqB->getCString());
                    }
                    
                    
                    currProperty = properties->valueForKey("research_time");
                    if (currProperty)
                    {
                        CCString* research = CCStringMake(properties->valueForKey("research_time")->getCString());
                        if (research->length() > 0)
                        {
                            b->researchTime = atof(research->getCString());
                        }
                        else
                            b->researchTime = 10.0f;
                    }
                    else
                    {
                        b->researchTime = 10.0f;
                    }
                    
                    currProperty = properties->valueForKey("build_time");
                    if (currProperty)
                    {
                        CCString* research = CCStringMake(properties->valueForKey("build_time")->getCString());
                        if (research->length() > 0)
                            b->constructionTime = atof(research->getCString());
                        else
                            b->constructionTime = 10.0f;
                    }
                    else
                    {
                        b->constructionTime = 10.0f;
                    }

                                                        
                }
                else
                {
                    b->buildingName= "No Name";
                    b->buildingDesc=  "You forgot to set a property somewhere";
                    b->buildingCost = 9999.0f;
                    b->buildingBuyPrice = 9999.0f;
                 //   b->buildingRentPrice = 9999.0f;
                    b->buildingType = BUILDINGCATEGORYMAX;
                    b->populationLimit = 0;
                  //  b->populationOverload = 0;
                  //  b->int_mod = 0;
                  //  b->social_mod = 0;
                 //   b->loyalty_mod = 0;
                 //   b->happiness_mod = 0;
                 //   b->cash_mod = -10;
                    b->expToLevel->initWithObject(CCInteger::create(0));
                
                }
                
                b->ID = buildingID;
                allBuildings->addObject(b);
                ++buildingID;
            }
            
            
            
         }
     }
    allBuildings->retain();
    allBuildingLayers->retain();
    allBuildingsOnMap = CCArray::create();
    allBuildingsOnMap->retain();
    
    housingOnMap= CCArray::create();
    amenityOnMap= CCArray::create();
    commerceOnMap= CCArray::create();
    militaryOnMap= CCArray::create();
    educationOnMap= CCArray::create();
    socialOnMap= CCArray::create();
    
    specialOnMap=CCArray::create();
    
    housingOnMap->retain();
    amenityOnMap->retain();
    commerceOnMap->retain();
    militaryOnMap->retain();
    educationOnMap->retain();
    socialOnMap->retain();
    
    specialOnMap->retain();
}

void BuildingHandler::addBuildingToMap(Building *b)
{
    BuildingCategory cat = b->buildingType;
    if (b->buildingName.length() == 0) return; //DO NOT ADD NAMELESS BUILDINGS!
    if (cat == BUILDINGCATEGORYMAX) return; //DO NOT ADD BUILDINGS THAT HAVE NO CATEGORY!
    switch (cat)
    {
        case HOUSING:
            housingOnMap->addObject(b);
            break;
        case AMENITY:
            amenityOnMap->addObject(b);
            break;
        case MILITARY:
            militaryOnMap->addObject(b);
            break;
        case COMMERCE:
            commerceOnMap->addObject(b);
            break;
        case EDUCATION:
            educationOnMap->addObject(b);
            break;
        case SOCIAL:
            socialOnMap->addObject(b);
            break;
        default:
            specialOnMap->addObject(b);
            break;
    }
  //  b->initializeJobs();
    allBuildingsOnMap->addObject(b);
}

void BuildingHandler::removeBuildingFromMap(Building *b)
{
    EmptyBuildingFirst(b);
    BuildingCategory cat = b->buildingType;
    switch (cat)
    {
        case HOUSING:
            housingOnMap->removeObject(b);
            break;
        case AMENITY:
            amenityOnMap->removeObject(b);
            break;
        case MILITARY:
            militaryOnMap->removeObject(b);
            break;
        case COMMERCE:
            commerceOnMap->removeObject(b);
            break;
        case EDUCATION:
            educationOnMap->removeObject(b);
            break;
        case SOCIAL:
            socialOnMap->removeObject(b);
            break;
        default:
            specialOnMap->removeObject(b);
            break;
    }
    allBuildingsOnMap->removeObject(b);
}

Building* BuildingHandler::getBuildingWithGID(int GID)
{
    //allBuildings->
    if (!allBuildings) return NULL;
    if (allBuildings->count() == 0) return NULL;
    
    Building* currBuilding;
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        currBuilding = (Building*)allBuildings->objectAtIndex(i);
        if (currBuilding->targetGUID == GID) return currBuilding;
    }
    return NULL;
}

//I definitely need to optimize this, but CCArray no have searching. Meh.
Building* BuildingHandler::getBuilding(int withID)
{
    if (!allBuildings) return NULL;
    if (allBuildings->count() == 0) return NULL;
    
    /*binary search fixed, changed*/
    Building* currBuilding = BinarySearch(allBuildings, withID);
    
    /*
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        currBuilding = (Building*)allBuildings->objectAtIndex(i);
        if (currBuilding->ID == withID) return currBuilding;
    }*/
    //CCLog("Building completely isn't on the map.");
    return currBuilding;
}

Building* BuildingHandler::getBuilding(std::string& buildingName)
{
    return NULL;
    if (!allBuildings) return NULL;
    if (allBuildings->count() == 0) return NULL;
    
    Building* currBuilding;
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        currBuilding = (Building*)allBuildings->objectAtIndex(i);
        if (currBuilding->buildingName.length() == 0 ||
            currBuilding->buildingName.compare("No Name") == 0) continue;
        
        
        if (currBuilding->buildingName.compare(buildingName) == 0)
            
            return currBuilding;
    }
    return NULL;
    
}

Building* BuildingHandler::getFirstBuildingOfCategory(BuildingCategory bc)
{
    if (!allBuildings) return NULL;
    if (allBuildings->count() == 0) return NULL;
    
    Building* currBuilding;
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        currBuilding = (Building*)allBuildings->objectAtIndex(i);
        if (currBuilding->buildingType == bc ) return currBuilding;
    }
    return NULL;
}


Building* BuildingHandler::getBuildingOnMapWithID(int withID)
{
    if (!allBuildingsOnMap)
    {
        return NULL;
    }
    if (allBuildingsOnMap->count() == 0)
    {
        return NULL;
    }
    
   Building* currBuilding = BinarySearch(allBuildingsOnMap, withID);
    /*
    Building* currBuilding = NULL;
    for (int i = 0; i < allBuildingsOnMap->count(); ++i)
    {
        currBuilding = (Building*)allBuildingsOnMap->objectAtIndex(i);
        if (currBuilding->ID == withID) return currBuilding;
    }*/
    //CCLog("Building completely isn't on the map.");
    return currBuilding;
}


Building* BuildingHandler::getBuildingOnMapWithName(std::string name)
{
    if (!allBuildingsOnMap)
    {
        return NULL;
    }
    if (allBuildingsOnMap->count() == 0)
    {
        return NULL;
    }
    
     Building* currBuilding = NULL;
     for (int i = 0; i < allBuildingsOnMap->count(); ++i)
     {
         currBuilding = (Building*)allBuildingsOnMap->objectAtIndex(i);
         std::string bname = currBuilding->buildingName;
         
         std::transform(name.begin(), name.end(), name.begin(), ::tolower);
         std::transform(bname.begin(), bname.end(), bname.begin(), ::tolower);
         
         if (bname.compare(name) == 0) return currBuilding;
     }
    //CCLog("Building completely isn't on the map.");
    return NULL; //building not found.
    
}

void BuildingHandler::EmptyBuildingFirst(Building *b)
{
    SpriteHandler* sh = GameScene::getThis()->spriteHandler;
    
    if (b->buildingType == HOUSING)
    {
        for (int i = 0; i < sh->spritesOnMap->count(); ++i)
        {
            GameSprite *spr = (GameSprite*)sh->spritesOnMap->objectAtIndex(i);
            Possessions* p = spr->getPossessions();
            if (p->homeLocation == b) spr->SellHouse();
            
        }
    }
    /*
    else
    {
        for (int i = 0; i < sh->spritesOnMap->count(); ++i)
        {
            GameSprite *spr = (GameSprite*)sh->spritesOnMap->objectAtIndex(i);
            Possessions* p = spr->getPossessions();
            if (p->jobLocation == b)
                spr->QuitJob();
            
        }
        
    }*/
    /*
    for (int i = 0; i < sh->spritesOnmap()
    */
}

int BuildingHandler::getHighestBuildingID()
{
    if (allBuildingsOnMap->count() == 0) return -1;
    
    return ((Building*)allBuildingsOnMap->objectAtIndex(allBuildingsOnMap->count() -1))->ID;
}