/** 
   *@file         xApp.cpp
   *@brief        xApp.cpp
   *@author       jimsmorong
   *@date         2017/11/02 09:07:00
*/

#include "stdafx.h"
#include "xApp.h"
#include "xDoc.h"

xApp g_xApp;

xApp::xApp()
{
    m_pMmpDoc = nullptr;
    m_pDlgView = nullptr;
    m_pxDoc = new xDoc;
}

xApp::~xApp()
{
    delete m_pxDoc;
}


