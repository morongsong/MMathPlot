

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <process.h>
#include <lua/lua.hpp>
#include <string>
using namespace std;


#define err_exit(num,fmt,...)  \
    do{printf("[%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__);exit(num);} while(0)
#define err_return(num,fmt,...)  \
    do{printf("[%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__);return(num);} while(0)

//lua�е��õ�c��������,ʵ�ּӷ�
int csum(lua_State* l)
{
    int a = lua_tointeger(l,1) ;
    int b = lua_tointeger(l,2) ;
    lua_pushinteger(l,a+b) ;
    return 1 ;
}

int _tmain(int argc,TCHAR** argv)
{
    lua_State * l = luaL_newstate() ;        //����lua���л���
    if ( l == NULL ) err_return(-1,"luaL_newstat() failed"); 
    int ret = 0 ;
    ret = luaL_loadfile(l,"func.lua") ;      //����lua�ű��ļ�
    if ( ret != 0 ) err_return(-1,"luaL_loadfile failed") ;
    ret = lua_pcall(l,0,0,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;

    lua_getglobal(l,"width");              //��ȡlua�ж���ı���
    lua_getglobal(l,"height");
    printf("height:%ld width:%ld\n",lua_tointeger(l,-1),lua_tointeger(l,-2)) ;
    lua_pop(l,1) ;                        //�ָ�lua��ջ

    int a = 11 ;
    int b = 12 ;
    lua_getglobal(l,"sum");               //����lua�еĺ���sum
    lua_pushinteger(l,a) ;
    lua_pushinteger(l,b) ;
    ret = lua_pcall(l,2,1,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    printf("sum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;
    lua_pop(l,1) ;

    const char str1[] = "hello" ;
    const char str2[] = "world" ;
    lua_getglobal(l,"mystrcat");          //����lua�еĺ���mystrcat
    lua_pushstring(l,str1) ;
    lua_pushstring(l,str2) ;
    ret = lua_pcall(l,2,1,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    printf("mystrcat:%s%s = %s\n",str1,str2,lua_tostring(l,-1)) ;
    lua_pop(l,1) ;

    lua_pushcfunction(l,csum) ;         //ע����lua��ʹ�õ�c����
    lua_setglobal(l,"csum") ;           //�󶨵�lua�е�����csum

    lua_getglobal(l,"mysum");           //����lua�е�mysum�������ú������ñ������ж����csum����ʵ�ּӷ�
    lua_pushinteger(l,a) ;
    lua_pushinteger(l,b) ;
    ret = lua_pcall(l,2,1,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;
    lua_pop(l,1) ;

    lua_close(l) ;                     //�ͷ�lua���л���

    system("pause");
    return 0 ;
}

