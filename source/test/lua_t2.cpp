

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <process.h>
#include "LCalc.h"





int _tmain(int argc,TCHAR** argv)
{


    //luaL_dostring(L,"print \"hello world\"");


    
    LCalc aCalc;
    std::string strError;
    double dResult = 0;
   
    if (aCalc.DoCalc("1-3",dResult,strError))
    {
        printf("1-3=%.4f\n",dResult);
    }
    
    if (aCalc.DoCalc("sin(3.1415926/4)",dResult,strError))
    {
        printf("sin(3.1415926/4)=%.4f\n",dResult);
    }

    if (aCalc.DoCalc("strlen(\"ffhhh指针\")",dResult,strError))
    {
        printf("strlen(\"ffhhh指针\")=%.4f\n",dResult);
    }

    if (aCalc.DoCalc("strlen(指针)",dResult,strError))
    {
        printf("strlen(指针)=%.4f\n",dResult);
    }
    else
    {
        printf("strlen(指针)= error: %s \n", strError.c_str());
    }

    system("pause");
    return 0 ;
}

