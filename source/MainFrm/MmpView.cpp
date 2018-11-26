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

// MmpView.cpp : CMmpView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Mmp.h"
#endif

#include "MmpDoc.h"
#include "MmpView.h"
#include "xFrame\xApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMmpView

IMPLEMENT_DYNCREATE(CMmpView, CView)

BEGIN_MESSAGE_MAP(CMmpView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMmpView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CMmpView ����/����

CMmpView::CMmpView()
{
	// TODO: �ڴ˴���ӹ������
    m_pDlgView = nullptr;
}

CMmpView::~CMmpView()
{
	if (m_pDlgView)
	{
		delete m_pDlgView;
	}
}

BOOL CMmpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMmpView ����

void CMmpView::OnDraw(CDC* /*pDC*/)
{
	CMmpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMmpView ��ӡ


void CMmpView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMmpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMmpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMmpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMmpView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMmpView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
}


// CMmpView ���

#ifdef _DEBUG
void CMmpView::AssertValid() const
{
	CView::AssertValid();
}

void CMmpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMmpDoc* CMmpView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMmpDoc)));
	return (CMmpDoc*)m_pDocument;
}
#endif //_DEBUG


// CMmpView ��Ϣ�������


int CMmpView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rcClient;
    GetClientRect(rcClient);

    //m_oPicShowCtrl.Create(_T("CPicShowCtrlClass"),_T(""), WS_CHILD | WS_VISIBLE, rcClient, this, 55535);
    //m_oPicShowCtrl.DoInit();
    //m_oPicShowCtrl.MoveWindow(rcClient);
    //m_oPicShowCtrl.ShowWindow(SW_SHOW);

    
    //m_oDlgView.MoveWindow(rcClient);
    //m_oDlgView.ShowWindow(SW_SHOW);
    return 0;
}


void CMmpView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    CRect rcClient;
    GetClientRect(rcClient);


    if (!m_pDlgView)
    {
        m_pDlgView = new CDlgView(this);
        m_pDlgView->Create(IDD_VIEW,this);
    }
    if (m_pDlgView && m_pDlgView->GetSafeHwnd())
    {
        CRect rcPic(-5, -5, rcClient.Width() + 5, rcClient.Height() + 5);
        m_pDlgView->MoveWindow(rcPic);
        m_pDlgView->ShowWindow(SW_SHOW);
    }


    CString strExp;
    strExp = g_xApp.m_pxDoc->m_strExpression;
    g_xApp.m_pDlgView->m_edtSpt.SetWindowText(strExp);


    CString strExp2;
    strExp2 = g_xApp.m_pxDoc->m_strExpression2;
    g_xApp.m_pDlgView->m_edtSpt2.SetWindowText(strExp2);


    //g_xApp.m_pDlgView->OnBnClickedBtnRun();


    return;
}


void CMmpView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    CRect rcClient;
    GetClientRect(rcClient);

    
    //CRect rcPic(1, 1, rcClient.Width() -1, rcClient.Height() - 1);
    //m_oPicShowCtrl.MoveWindow(rcPic);
    //m_oPicShowCtrl.ShowWindow(SW_SHOW);

    if (m_pDlgView && m_pDlgView->GetSafeHwnd())
    {
        CRect rcPic(-5, -5, rcClient.Width() + 5, rcClient.Height() + 5);
        m_pDlgView->MoveWindow(rcPic);
        m_pDlgView->ShowWindow(SW_SHOW);
    }

}
