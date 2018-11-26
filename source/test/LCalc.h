

#ifndef __LCALC_H__
#define __LCALC_H__


#include <string>
struct lua_State;
class LCalc
{
public:
    LCalc();
    ~LCalc();
public:
    bool DoCalc(const char* szExp,double& dResult,std::string& strError);
protected:
    static int math_sin (lua_State *L);

    static int strlen(lua_State *L);
private:
    lua_State* m_L;
};

#endif //LCalc_h