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
    granaryOnMap = NULL;
    amenityOnMap = NULL;
    commerceOnMap = NULL;
    militaryOnMap = NULL;
    educationOnMap = NULL;
    socialOnMap = NULL;
    specialOnMap = NULL;
    decorationOnMap = NULL;
    marketOnMap = NULL;
    
    housingGhostOnMap = NULL;
    granaryGhostOnMap = NULL;
    amenityGhostOnMap = NULL;
    commerceGhostOnMap = NULL;
    militaryGhostOnMap = NULL;
    educationGhostOnMap = NULL;
    socialGhostOnMap = NULL;
    specialGhostOnMap = NULL;
    decorationGhostOnMap = NULL;
    marketGhostOnMap = NULL;
}

BuildingHandler::~BuildingHandler()
{
    housingOnMap->removeAllObjects();
    granaryOnMap->removeAllObjects();
    amenityOnMap->removeAllObjects();
    commerceOnMap->removeAllObjects();
    militaryOnMap->removeAllObjects();
    educationOnMap->removeAllObjects();
    socialOnMap->removeAllObjects();
    specialOnMap->removeAllObjects();
    decorationOnMap->removeAllObjects();
    marketOnMap->removeAllObjects();
    
    housingOnMap->release();
    amenityOnMap->release();
    granaryOnMap->release();
    commerceOnMap->release();
    militaryOnMap->release();
    educationOnMap->release();
    socialOnMap->release();
    specialOnMap->release();
    CC_SAFE_RELEASE(decorationOnMap);
    CC_SAFE_RELEASE(marketOnMap);
    
    housingGhostOnMap->removeAllObjects();
    granaryGhostOnMap->removeAllObjects();
    amenityGhostOnMap->removeAllObjects();
    commerceGhostOnMap->removeAllObjects();
    militaryGhostOnMap->removeAllObjects();
    educationGhostOnMap->removeAllObjects();
    socialGhostOnMap->removeAllObjects();
    specialGhostOnMap->removeAllObjects();
    decorationGhostOnMap->removeAllObjects();
    marketGhostOnMap->removeAllObjects();
    
    CC_SAFE_RELEASE(housingGhostOnMap);
    CC_SAFE_RELEASE(granaryGhostOnMap);
    CC_SAFE_RELEASE(amenityGhostOnMap);
    CC_SAFE_RELEASE(commerceGhostOnMap);
    CC_SAFE_RELEASE(militaryGhostOnMap);
    CC_SAFE_RELEASE(educationGhostOnMap);
    CC_SAFE_RELEASE(socialGhostOnMap);
    CC_SAFE_RELEASE(specialGhostOnMap);
    CC_SAFE_RELEASE(decorationGhostOnMap);
    CC_SAFE_RELEASE(marketGhostOnMap);
    
    if (allBuildingsOnMap)
    {
        if (allBuildingsOnMap->count() > 0)
            allBuildingsOnMap->removeAllObjects();
        allBuildingsOnMap->release();
    }

    allBuildingLayers->removeAllObjects();
    allBuildingLayers->release();
    
    if (allBuildings)
    {
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
                b->baseGID = j;
                
                b->buildingTexture = CCTextureCache::sharedTextureCache()->addImage( layer->getTileSet()->m_sSourceImage.c_str());
                b->buildingTexture->retain();
                b->buildingRect = layer->getTileSet()->rectForGID(b->baseGID);
                
                b->width = b->buildingRect.size.width / 128;
                b->height = b->buildingRect.size.height / 128; //128 being the standard tile size in the TMX file.
                
                CCDictionary* properties = mapPtr->propertiesForGID(b->baseGID);
                if (properties)
                {
                    const CCString* currProperty;
                    
                    
                    currProperty = properties->valueForKey("anim_frames");
                    if (currProperty)
                    {
                        CCString* animfr = CCStringMake(properties->valueForKey("anim_frames")->getCString());
                        b->animframe_count = atoi(animfr->getCString());
                        b->maxGID = b->baseGID + (b->animframe_count - 1);
                    }
                    else
                    {
                        b->animframe_count = 1;
                        b->maxGID = b->baseGID;
                    }
                    
                    
                    currProperty = properties->valueForKey("jobs_available");
                   
                     if (currProperty)
                    {
                        CCString* job = CCStringMake(properties->valueForKey("jobs_available")->getCString());
                        b->number_of_jobs = atoi(job->getCString());
                    }
                    else b->number_of_jobs = 0;
                    //else the jobs array for this building is null or empty.
                    
                    
                    currProperty = properties->valueForKey("name");
                    if (currProperty)
                    {
                        b->buildingName= currProperty->getCString();  //CCStringMake("PlaceHolder");
                    
                    }
                    else
                    {
                        b->buildingName= "No Name";//";  //CCStringMake("PlaceHolder");
                    }
                    
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
                    
                    currProperty = properties->valueForKey("builder_limit");
                    if(currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("builder_limit")->getCString());
                        b->builderLimit = atoi(cost->getCString());
                    }
                    else
                    {
                        b->builderLimit = 0;
                    }
                    
                    // for storage
                    currProperty = properties->valueForKey("storage");
                    if (currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("storage")->getCString());
                        b->currentStorage = atoi(cost->getCString());
                    }
                    else
                    {
                        b->currentStorage = 0;
                    }
                    
                    // for storage limit
                    currProperty = properties->valueForKey("storage_limit");
                    if(currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("storage_limit")->getCString());
                        b->storageLimit = atoi(cost->getCString());
                    }
                    else
                    {
                        b->storageLimit = 0;
                    }
                    
                    currProperty = properties->valueForKey("food_consumption_rate");
                    if(currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("food_consumption_rate")->getCString());
                        b->food_consumption_rate = atoi(cost->getCString());
                    }
                    else
                    {
                        b->food_consumption_rate = 0;
                    }
                    
                    currProperty = properties->valueForKey("recovery_rate");
                    if(currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("recovery_rate")->getCString());
                        b->recovery_rate = atoi(cost->getCString());
                    }
                    else
                    {
                        b->recovery_rate = 0;
                    }
                    
                    // for build unit limit
                    currProperty = properties->valueForKey("build_uint_required");
                    if(currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("build_uint_required")->getCString());
                        b->build_uint_required = atoi(cost->getCString());
                        
                    }
                    else
                    {
                        b->build_uint_required = 0;
                    }
                    
                    // for work unit limit
                    currProperty = properties->valueForKey("work_unit_required");
                    if(currProperty)
                    {
                        CCString* cost = CCStringMake(properties->valueForKey("work_unit_required")->getCString());
                        b->work_unit_required = atoi(cost->getCString());
                    }
                    else
                    {
                        b->work_unit_required = 0;
                    }
                    
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
                        else if (type == "granary")
                        {
                            b->buildingType = GRANARY;
                        }
                        else if (type == "amenity")
                        {
                            b->buildingType = AMENITY;
                        }
                        else if (type == "commerce")
                        {
                            b->buildingType = COMMERCE;
                        }
                        else if (type == "military")
                        {
                            b->buildingType = MILITARY;
                        }
                        else if (type == "education")
                        {
                            b->buildingType = EDUCATION;
                        }
                        else if (type == "social")
                        {
                            b->buildingType = SOCIAL;
                        }
                        else if (type == "special")
                        {
                            b->buildingType = SPECIAL;
                        }
                        else if (type == "decoration")
                        {
                            b->buildingType = DECORATION;
                        }
                        else if (type == "market")
                        {
                            b->buildingType = MARKET;
                        }
                        else if (type == "")
                        {
                            b->buildingType = BUILDINGCATEGORYMAX;
                        }
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
                    
                    
                    currProperty = properties->valueForKey("upgrade_unit");
                    if (currProperty)
                    {
                        CCString* upgrade_unit = CCStringMake(properties->valueForKey("upgrade_unit")->getCString());
                        if (upgrade_unit->length() > 0)
                        {
                            b->upgrade_unit_max = atof(upgrade_unit->getCString());
                        }
                        else
                        {
                            b->upgrade_unit_max = 1000.0f;
                        }
                    }
                    else
                    {
                        b->upgrade_unit_max = 1000.0f;
                    }
                    
                    
                    currProperty = properties->valueForKey("build_uint_required");
                    if (currProperty)
                    {
                        CCString* build = CCStringMake(properties->valueForKey("build_uint_required")->getCString());
                        if (build->length() > 0)
                        {
                            b->build_uint_required = atoi(build->getCString());
                            
                            // construct all buldings at the initialization.
                            // PS: for this approach, unconstruct building is not possible at the begining of the game.
                            b->build_uint_current = b->build_uint_required;

                        }
                        else
                        {
                            b->build_uint_required = 0;
                        }
                    }
                    else
                    {
                        b->build_uint_required = 0;
                    }
                    
                    currProperty = properties->valueForKey("anim_random");
                    if (currProperty)
                    {
                        CCString* isTrue = CCStringMake(properties->valueForKey("anim_random")->getCString());
                        if (isTrue->length() > 0)
                        {
                            if (isTrue->compare("true") == 0)
                                {
                                    b->anim_random = true;
                                }
                                else
                                {
                                    b->anim_random = false;
                                }
                        }
                    }
                    else
                    {
                        b->anim_random = false;
                    }

                    currProperty = properties->valueForKey("anim_random_chance");
                    if (currProperty)
                    {
                        CCString* chance = CCStringMake(properties->valueForKey("anim_random_chance")->getCString());
                        if (chance->length() > 0)
                        {
                            b->anim_random_chance = atof(chance->getCString());
                        }
                    }
                    else
                    {
                        b->anim_random_chance = 1.0f;
                    }
                }
                else
                {
                    b->buildingName= "No Name";
                    b->buildingDesc=  "You forgot to set a property somewhere";
                    b->buildingCost = 9999.0f;
                    b->buildingBuyPrice = 9999.0f;
                    b->buildingType = BUILDINGCATEGORYMAX;
                    b->populationLimit = 0;
                    b->build_uint_required = 0;
                  //  b->populationOverload = 0;
                  //  b->int_mod = 0;
                  //  b->social_mod = 0;
                 //   b->loyalty_mod = 0;
                 //   b->happiness_mod = 0;
                 //   b->cash_mod = -10;
                    b->expToLevel->initWithObject(CCInteger::create(0));
                    
                    // for storage
                    b->currentStorage = 0;
                    b->storageLimit = 0;
                    
                    b->recovery_rate = 0;
                    b->food_consumption_rate = 0;
                    
                    // build unit limit
                    b->build_uint_required = 0;
                     b->expToLevel->initWithObject(CCInteger::create(0));

                }
                
                b->ID = buildingID;
                b->currGID = b->baseGID;
                b->lastGID = b->baseGID;
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
    granaryOnMap= CCArray::create();
    amenityOnMap= CCArray::create();
    commerceOnMap= CCArray::create();
    militaryOnMap= CCArray::create();
    educationOnMap= CCArray::create();
    socialOnMap= CCArray::create();
    specialOnMap= CCArray::create();
    decorationOnMap = CCArray::create();
    marketOnMap = CCArray::create();
    
    housingOnMap->retain();
    granaryOnMap->retain();
    amenityOnMap->retain();
    commerceOnMap->retain();
    militaryOnMap->retain();
    educationOnMap->retain();
    socialOnMap->retain();
    specialOnMap->retain();
    decorationOnMap->retain();
    marketOnMap->retain();
    
    housingGhostOnMap = CCArray::create();
    granaryGhostOnMap = CCArray::create();
    amenityGhostOnMap = CCArray::create();
    commerceGhostOnMap = CCArray::create();
    militaryGhostOnMap = CCArray::create();
    educationGhostOnMap = CCArray::create();
    socialGhostOnMap = CCArray::create();
    specialGhostOnMap = CCArray::create();
    decorationGhostOnMap = CCArray::create();
    marketGhostOnMap = CCArray::create();
    
    housingGhostOnMap->retain();
    granaryGhostOnMap->retain();
    amenityGhostOnMap->retain();
    commerceGhostOnMap->retain();
    militaryGhostOnMap->retain();
    educationGhostOnMap->retain();
    socialGhostOnMap->retain();
    specialGhostOnMap->retain();
    decorationGhostOnMap->retain();
    marketGhostOnMap->retain();
}

void BuildingHandler::addBuildingToMap(Building *b)
{
    BuildingCategory cat = b->buildingType;
    
    if (b->buildingName.length() == 0)
    {
        //DO NOT ADD NAMELESS BUILDINGS!
        return;
    }
    
    if (cat == BUILDINGCATEGORYMAX){
        //DO NOT ADD BUILDINGS THAT HAVE NO CATEGORY!
        return;
    }
    
    switch (cat)
    {
        case HOUSING:
            housingOnMap->addObject(b);
            break;
        case GRANARY:
            granaryOnMap->addObject(b);
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
        case SPECIAL:
            specialOnMap->addObject(b);
            break;
        case DECORATION:
            decorationOnMap->addObject(b);
            break;
        case MARKET:
            marketOnMap->addObject(b);
            break;
        default:
            decorationOnMap->addObject(b);
            break;
    }
    allBuildingsOnMap->addObject(b);
    
    //for animation
    b->BeginAnim();

    
}

void BuildingHandler::removeBuildingFromMap(Building *b)
{
    b->EndAnim();
    EmptyBuildingFirst(b);
    BuildingCategory cat = b->buildingType;
    switch (cat)
    {
        case HOUSING:
            housingOnMap->removeObject(b);
            break;
        case GRANARY:
            granaryOnMap->removeObject(b);
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
        case SPECIAL:
            specialOnMap->removeObject(b);
            break;
        case DECORATION:
            decorationOnMap->removeObject(b);
            break;
        case MARKET:
            marketOnMap->removeObject(b);
            break;
        default:
            decorationOnMap->removeObject(b);
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
        if (currBuilding->baseGID == GID) return currBuilding;
    }
    return NULL;
}

Building* BuildingHandler::getNearestStorage(GameSprite* sp)
{
    if (sp == NULL) return NULL;
    if (granaryOnMap->count() == 0) return NULL;
    CCPoint spritePosition = sp->spriteRep->getPosition();
 
    float shortestDist = 9999.0f;
    
    Building* currBuilding;
    Building* shortedstDistBuilding = NULL;
    for (int i = 0; i < granaryOnMap->count(); ++i)
    {
        currBuilding = (Building*)granaryOnMap->objectAtIndex(i);
        CCPoint buildingPos = currBuilding->buildingRep->getPosition();
        
        float dist = ccpDistanceSQ(buildingPos, spritePosition);
        if (dist < shortestDist)
        {
            shortestDist = dist;
            shortedstDistBuilding = currBuilding;
        }
    }
    
    return shortedstDistBuilding;
}

//I definitely need to optimize this, but CCArray no have searching. Meh.
Building* BuildingHandler::getBuilding(int withID)
{
    if (!allBuildings) return NULL;
    if (allBuildings->count() == 0) return NULL;
    
    /*binary search fixed, changed*/
    Building* currBuilding = BinarySearch(allBuildings, withID);
    
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
            currBuilding->buildingName.compare("No Name") == 0)
        {
            continue;
        }
        
        if (currBuilding->buildingName.compare(buildingName) == 0)
        {
            return currBuilding;
        }
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
    /*
    SpriteHandler* sh = GameScene::getThis()->spriteHandler;
    
    if (b->buildingType == HOUSING)
    {
        for (int i = 0; i < sh->spritesOnMap->count(); ++i)
        {
            //GameSprite *spr = (GameSprite*)sh->spritesOnMap->objectAtIndex(i);
     //       Possessions* p = spr->getPossessions();
        }
    }*/
}

int BuildingHandler::getHighestBuildingID()
{
    if (allBuildingsOnMap->count() == 0) return -1;
    
    return ((Building*)allBuildingsOnMap->objectAtIndex(allBuildingsOnMap->count() -1))->ID;
}

Building* BuildingHandler::getRandomBuildingWithName(std::string name)
{
    if (!allBuildings) return NULL;
    if (allBuildings->count() == 0) return NULL;
    
    std::vector<Building*> foundBuildings; //this should deallocate the moment I exit this function
    Building* currBuilding = NULL;
    for (int i = 0; i < allBuildings->count(); ++i)
    {
        currBuilding = (Building*)allBuildings->objectAtIndex(i);
        std::string bname = currBuilding->buildingName;
        //CCLog("%s %s", name.c_str(), bname.c_str());
        if (bname.find(name) != std::string::npos) foundBuildings.push_back(currBuilding);
    }
   
    if (foundBuildings.size() == 0) return NULL;
    
    int targetIdx = rand() % foundBuildings.size();
    
    return foundBuildings[targetIdx];
}