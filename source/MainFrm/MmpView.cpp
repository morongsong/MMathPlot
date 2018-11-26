// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MmpView.cpp : CMmpView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMmpView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CMmpView 构造/析构

CMmpView::CMmpView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMmpView 绘制

void CMmpView::OnDraw(CDC* /*pDC*/)
{
	CMmpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMmpView 打印


void CMmpView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMmpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMmpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMmpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CMmpView 诊断

#ifdef _DEBUG
void CMmpView::AssertValid() const
{
	CView::AssertValid();
}

void CMmpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMmpDoc* CMmpView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMmpDoc)));
	return (CMmpDoc*)m_pDocument;
}
#endif //_DEBUG


// CMmpView 消息处理程序


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
