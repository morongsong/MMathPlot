/** 
   *@file         xDoc.cpp
   *@brief        xDoc.cpp
   *@author       jimsmorong
   *@date         2017/11/02 08:43:19
*/
#include "stdafx.h"
#include "xDoc.h"
#include "Untily/MtIni.h"

xDoc::xDoc()
{
   
}

xDoc::~xDoc()
{

}

BOOL xDoc::OnNewDocument()
{
    m_strExpression = _T("y = math.cos((t / 30)*x) * 100");

    m_strExpression2 = _T("y=math.cos(x/50) * 100");

    //m_strExpression2 = _T("y=math.cos(x/50) * 100+100");
    //m_strExpression2 = _T("y = math.cos((t / 30)*x / 50) * 100");
    

    return TRUE;
}

BOOL xDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    MtIni oIni;
    oIni.LoadFile(lpszPathName, false, true);
    m_strExpression = oIni.GetString(_T("strExpression"), _T("y=math.cos(x/50) * 100"));
    m_strExpression2 = oIni.GetString(_T("strExpression2"), _T("y=math.cos(x/50) * 100+100"));
    return TRUE;
}


BOOL xDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    MtIni oIni;
    oIni.LoadFile(lpszPathName, true, true);
    oIni.SetString(_T("strExpression"), m_strExpression);
    oIni.SetString(_T("strExpression2"), m_strExpression2);
    return TRUE;
}
