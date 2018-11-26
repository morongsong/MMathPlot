/** 
   *@file         xApp.h
   *@brief        xApp.h
   *@author       jimsmorong
   *@date         2017/11/02 09:06:53
 */
#ifndef XAPP_H___
#define XAPP_H___
#pragma once
#include "DlgView.h"
#include "xDoc.h"
#include "MmpDoc.h"


/// xApp
class xApp
{
public:
    xApp();
    virtual ~xApp();
private:
    xApp(const xApp& rhs);
    xApp& operator=(const xApp& rhs);

public:
    CMmpDoc* m_pMmpDoc;
    xDoc* m_pxDoc;
    CDlgView* m_pDlgView;
};

extern xApp g_xApp;

#endif //XAPP_H___
