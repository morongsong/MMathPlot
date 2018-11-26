


#include <lua/lua.hpp>
#include "LCalc.h"



LCalc::LCalc()
{
    m_L = luaL_newstate(); 
    luaL_openlibs(m_L);
    lua_register(m_L,"sin",math_sin);
    lua_register(m_L,"strlen",strlen);
}

bool LCalc::DoCalc(const char* szExp,double& dResult,std::string& strError )
{
    std::string strExpModify = "a=";
    strExpModify += szExp;
    int ret = luaL_dostring(m_L,strExpModify.c_str());
    if(ret != 0)
    {
        const char* strErr = lua_tostring(m_L,-1);
        strError = strErr;
        return false; 
    }

    lua_getglobal(m_L,"a");
    double a = lua_tonumber(m_L,-1);
    lua_pop(m_L,1) ;
    dResult = a;
    return true;
}

int LCalc::math_sin( lua_State *L )
{
    lua_pushnumber(L, l_mathop(sin)(luaL_checknumber(L, 1)));
    return 1;
}

int LCalc::strlen(lua_State *L)
{
//     size_t l;
 //    luaL_checklstring(L, 1, &l);


    size_t len=0;
    const char *s = lua_tolstring(L, 1, &len);
    if (s)
    {
        lua_pushinteger(L, (lua_Integer)len);
    }
    return 1;
}

LCalc::~LCalc()
{
    lua_close(m_L);
}

