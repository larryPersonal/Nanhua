//
//  Behavior.h
//  PopulationTest
//
//  Created by Serious Games on 5/4/13.
//
//

#ifndef __PopulationTest__Behavior__
#define __PopulationTest__Behavior__
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

extern "C"
{
    #include "lua.h"
    #include "luaconf.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include "cocos2d.h"
//#include "LuaBindFunctions.h"

//Note: possessions includes both sprite.h and behavior.h
//#include "Sprite.h"
class GameSprite;

enum Status
{
    BH_INVALID,
    BH_SUCCESS,
    BH_FAILURE,
    BH_RUNNING,
};


using namespace cocos2d;

class Behavior
{
    GameSprite* sp; //suppplied by its parent sprite. DO NOT CLEAN THIS UP, That's what the parent sprite should be doing to itself!
    void lua_pushvariables(const char* func_name);
public:
    virtual ~Behavior();
    
    lua_State* L;
    virtual Status update();
    virtual void onInitialize();
    virtual void onTerminate(Status);

    int lua_processfunc(lua_State* L);

    
    Behavior();
    Behavior(Status (*food)(std::string));
   
    void BehaviorInit();
    
   virtual  Status tick();
    
    void setSprite (GameSprite* ptrSprite);
    

    /*function pointer*/
    Status (*foo)(std::string);
    
    std::string name;
    std::string file_name;
    std::string path;
    std::string func_name;
    Status retStatus;
    
private:
    Status m_eStatus;
    void registerFunctions(lua_State* L);
    

};

#endif /* defined(__PopulationTest__Behavior__) */
