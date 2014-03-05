//
//  Behavior.cpp
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#include "Behavior.h"
#include "Sprite.h"
#include "Building.h"
#include "GameScene.h"
#include "GameManager.h"
//#include "cocos2d.h"

/*Glue functions. Defined here to avoid double include problem existing in Sprite.h*/
static GameSprite* sprite = NULL;

/******************************************** lua static functions *************************************/

/*forces the sprite to pick a random destination. Range ignored! Valid destination ignored!*/
static int PickRandomDestination_Glue(lua_State *L)
{
    if (!sprite)
    {
        lua_pushboolean(L, false);
    }
    else
    {
       // CCLog(sprite->spriteName->getCString());
        if(sprite->getAction() == IDLE)
        {
            lua_pushboolean(L, sprite->Wander());
        }
    }
    return 1;
}


/******************************************* behaviour default functions ********************************/


void Behavior::lua_pushvariables(const char* func_name)
{
    //not sure if I wanna put things in here. We'll see.
}

/*behavior functions*/
Behavior::Behavior()
{
    L = luaL_newstate();
}

Behavior::Behavior(Status (*food)(std::string))
{
    foo = food;
}

Behavior::~Behavior()
{
    if (sp != NULL)
    {
        sp = NULL;
    }
    lua_close(L);
}

void Behavior::BehaviorInit()
{
    luaL_openlibs(L);
    
    registerFunctions(L);
    
    path = CCFileUtils::sharedFileUtils()->fullPathForFilename(file_name.c_str());
    
    int result = luaL_dofile(L, path.c_str());
    if (result != 0)
    {
        CCLog("%s", lua_tostring(L, -1));
        CCLog("something went wrong.");
    }
}

Status Behavior::update()
{
    return BH_SUCCESS;
}

void Behavior::onInitialize()
{
    
}

void Behavior::onTerminate(Status)
{
    
    int result = lua_processfunc(L);
    
    if (result == 0)
    {
        retStatus = BH_SUCCESS;
    }
    else
    {
        retStatus = BH_FAILURE;
    }
    
    m_eStatus = retStatus;
}

int  Behavior::lua_processfunc(lua_State* L)
{
    int retVal = 0;
    
    //set the correct GameSprite pointer for LuaBindFunctions to operate on.
    sprite = sp;
    lua_getglobal(L, func_name.c_str());
    
    lua_call(L, 0, 1);
    retVal = (int)lua_tointeger(L, -1);
    lua_pop(L,1);
    return retVal;
}

Status Behavior::tick()
{
    if (m_eStatus == BH_INVALID)
    {
        onInitialize();
    }
    m_eStatus = update();
    
    if (m_eStatus != BH_RUNNING)
    {
        onTerminate(m_eStatus);
    }
    return m_eStatus;
}

void Behavior::setSprite(GameSprite *ptrSprite)
{
    sp = ptrSprite;
}

void Behavior::registerFunctions(lua_State* L)
{
    lua_register(L, "GoWander", PickRandomDestination_Glue);
}




