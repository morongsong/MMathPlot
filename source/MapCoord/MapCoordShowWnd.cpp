// ShowWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MapCoordShowWnd.h"
#include "MapCoordDraw.h"
#include "xFrame\xApp.h"

// MapCoordShowWnd

IMPLEMENT_DYNAMIC(MapCoordShowWnd, CWnd)

MapCoordShowWnd::MapCoordShowWnd()
{
    m_pMvMapView = nullptr;

    if (!m_pMvMapView)
    {
        m_pMvMapView = new MvMapView;
        m_pMvMapView->m_pHoldWnd = this;

        //m_pMvMapView->m_nSOrgX = 30;
        //m_pMvMapView->m_nSOrgY = rcClient.Height() - 30;
        //m_pMvMapView->m_nSWidth = rcClient.Width();
        //m_pMvMapView->m_nSHeight = rcClient.Height();
        m_pMvMapView->m_dScaleX = 1;
        m_pMvMapView->m_dScaleY = 1;

        m_pMvMapView->m_CoordAxis.m_eDrawType = MvCoordAxis::EDrawBySPt;
        //m_pMvMapView->m_CoordAxis.ptDrawOrg.x = 30;
        //m_pMvMapView->m_CoordAxis.ptDrawOrg.y = rcClient.Height() - 30;

        //double Profile[800];
        //for (int i = 0; i < 800; i++)
        //{
        //    m_pMvMapView->m_MapPoint8B.pt8B[i].x = i*0.5;// (i - 400) * 0.02;
        //    m_pMvMapView->m_MapPoint8B.pt8B[i].y = i*0.5;// sin(i*0.017453293);
        //}
    }
    
}

MapCoordShowWnd::~MapCoordShowWnd()
{
    delete m_pMvMapView;
}


BEGIN_MESSAGE_MAP(MapCoordShowWnd, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEWHEEL()
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_CREATE()
END_MESSAGE_MAP()



// MapCoordShowWnd message handlers




BOOL MapCoordShowWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    return TRUE;
    return CWnd::OnEraseBkgnd(pDC);
}


void MapCoordShowWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages
    CRect rcClient;
    GetClientRect(&rcClient);
    //dc.MoveTo(rcClient.left,rcClient.top);
    //dc.LineTo(rcClient.right,rcClient.top);
    //dc.LineTo(rcClient.right,rcClient.bottom);
    //dc.LineTo(rcClient.left,rcClient.bottom);
    //dc.LineTo(rcClient.left,rcClient.top);

    m_pMvMapView->m_nSWidth = rcClient.Width();
    m_pMvMapView->m_nSHeight = rcClient.Height();

    CDC* pDC = nullptr;
    CMemDC mdc(dc,this);
    pDC = &mdc.GetDC();

    m_pMvMapView->Draw();
    CImage& oImage = m_pMvMapView->GetImg();
    pDC->SetStretchBltMode(COLORONCOLOR);
    oImage.Draw(pDC->GetSafeHdc(),
        0,0,(int)(oImage.GetWidth()),(int)(oImage.GetHeight())
        );

}


void MapCoordShowWnd::OnMouseMove(UINT nFlags, CPoint point)
{


    m_pMvMapView->OnMouseMove(nFlags, point);


    //g_xApp.m_pDlgView->OnBnClickedBtnRun();


    RedrawWindow();
    CWnd::OnMouseMove(nFlags, point);
}


void MapCoordShowWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus() != this)
    {
        SetFocus();
    }
    
    m_pMvMapView->OnLButtonDown(nFlags, point);

    //g_xApp.m_pDlgView->OnBnClickedBtnRun();
    RedrawWindow();
    CWnd::OnLButtonDown(nFlags, point);
}


void MapCoordShowWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_pMvMapView->OnLButtonUp(nFlags, point);

    //g_xApp.m_pDlgView->OnBnClickedBtnRun();
    RedrawWindow();
    CWnd::OnLButtonUp(nFlags, point);
}


void MapCoordShowWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_pMvMapView->OnRButtonDown(nFlags, point);

    //g_xApp.m_pDlgView->OnBnClickedBtnRun();
    RedrawWindow();
    CWnd::OnRButtonDown(nFlags, point);
}


void MapCoordShowWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_pMvMapView->OnRButtonUp(nFlags, point);

    //g_xApp.m_pDlgView->OnBnClickedBtnRun();
    RedrawWindow();
    CWnd::OnRButtonUp(nFlags, point);
}


BOOL MapCoordShowWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_pMvMapView->OnMouseWheel(nFlags, zDelta, pt);

    //g_xApp.m_pDlgView->OnBnClickedBtnRun();
    RedrawWindow();
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}



int MapCoordShowWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    //CRect rcClient;
    //GetClientRect(&rcClient);

    //m_pMvMapView->m_nSOrgX = 30;
    //m_pMvMapView->m_nSOrgY = rcClient.Height() - 30;
    //m_pMvMapView->m_nSWidth = rcClient.Width();
    //m_pMvMapView->m_nSHeight = rcClient.Height();


    //m_pMvMapView->m_CoordAxis.ptDrawOrg.x = 30;
    //m_pMvMapView->m_CoordAxis.ptDrawOrg.y = rcClient.Height() - 30;

    //double Profile[800];
    //for (int i = 0; i < 800; i++)
    //{
    //    m_pMvMapView->m_MapPoint8B.pt8B[i].x = i*0.5;// (i - 400) * 0.02;
    //    m_pMvMapView->m_MapPoint8B.pt8B[i].y = i*0.5;// sin(i*0.017453293);
    //}
    return 0;
}

void MapCoordShowWnd::Init()
{

    CRect rcClient;
    GetClientRect(&rcClient);

    m_pMvMapView->m_nSOrgX = 30;
    m_pMvMapView->m_nSOrgY = rcClient.Height() - 30;
    m_pMvMapView->m_nSWidth = rcClient.Width();
    m_pMvMapView->m_nSHeight = rcClient.Height();

    m_pMvMapView->m_CoordAxis.m_eDrawType = MvCoordAxis::EDrawBySPt;
    m_pMvMapView->m_CoordAxis.ptDrawOrg.x = 30;
    m_pMvMapView->m_CoordAxis.ptDrawOrg.y = rcClient.Height() - 30;

    m_pMvMapView->m_MapPoint8B.m_nDrawFlag = MvMapPoint8B::eDrawNull;
    m_pMvMapView->m_MapPoint8B2.m_nDrawFlag = MvMapPoint8B::eDrawNull;
    //double Profile[1000];
    //for (int i = 0; i < 1000; i++)
    //{
    //    m_pMvMapView->m_MapPoint8B.pt8B[i].x = i*0.5;// (i - 400) * 0.02;
    //    m_pMvMapView->m_MapPoint8B.pt8B[i].y = i*0.5;// sin(i*0.017453293);
    //}
    return;
}