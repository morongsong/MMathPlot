// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MmpDoc.cpp : CMmpDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Mmp.h"
#endif

#include "MmpDoc.h"

#include <propkey.h>
#include "xFrame\xApp.h"
#include "DlgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMmpDoc

IMPLEMENT_DYNCREATE(CMmpDoc, CDocument)

BEGIN_MESSAGE_MAP(CMmpDoc, CDocument)
END_MESSAGE_MAP()


// CMmpDoc ����/����

CMmpDoc::CMmpDoc()
{
	// TODO: �ڴ����һ���Թ������
    g_xApp.m_pMmpDoc = this;
}

CMmpDoc::~CMmpDoc()
{
}




// CMmpDoc ���л�

void CMmpDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CMmpDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CMmpDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CMmpDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMmpDoc ���

#ifdef _DEBUG
void CMmpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMmpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMmpDoc ����

BOOL CMmpDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;


    g_xApp.m_pxDoc->OnNewDocument();



    return TRUE;
}



BOOL CMmpDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;
    g_xApp.m_pxDoc->OnOpenDocument(lpszPathName);

    //CString strExp;
    //strExp = g_xApp.m_pxDoc->m_strExpression;
    //g_xApp.m_pDlgView->m_edtSpt.SetWindowText(strExp);
    

    return TRUE;
}


BOOL CMmpDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    CString strExp;
    g_xApp.m_pDlgView->m_edtSpt.GetWindowText(strExp);
    g_xApp.m_pxDoc->m_strExpression = strExp;

    

    //CDocument::OnSaveDocument(lpszPathName);
    g_xApp.m_pxDoc->OnSaveDocument(lpszPathName);
    return TRUE;
}


void CMmpDoc::OnCloseDocument()
{

    CDocument::OnCloseDocument();
}
