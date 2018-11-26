/** 
   *@file         xDoc.h
   *@brief        xDoc.h
   *@author       jimsmorong
   *@date         2017/11/02 08:43:07
 */
#ifndef XDOC_H___
#define XDOC_H___
#pragma once

/// xDoc
class xDoc
{
public:
    xDoc();
    virtual ~xDoc();
private:
    xDoc(const xDoc& rhs);
    xDoc& operator=(const xDoc& rhs);
public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
public:
    CString m_strExpression;
    CString m_strExpression2;
};

#endif //XDOC_H___
