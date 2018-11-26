/**
   *@file         picshowctrl.cpp
   *@brief        图片显示控件
   *@author       jimsmorong
   *@date         2014/11/17 21:14:36
*/

#include "stdafx.h"
#include "picshowctrl.h"
#include <math.h>

BOOL CPicShowCtrl::m_bIsRegistered = FALSE;

IMPLEMENT_DYNAMIC(CPicShowCtrl, CScrollView)

CPicShowCtrl::CPicShowCtrl()
{
    CPicShowCtrl::RegisterWindowClass();
    m_dScaleX = 1;
    m_dScaleY = 1;
    VERIFY(m_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));
    m_nStartWidth = 0;
    m_nStartHeight = 0;
    m_bDraw = true;
}

CPicShowCtrl::~CPicShowCtrl()
{

}

BEGIN_MESSAGE_MAP(CPicShowCtrl, CScrollView)
    ON_COMMAND(IDM_PICSHOW_FITIMG,   DoFitWndImg)
    ON_COMMAND(IDM_PICSHOW_SRCIMG,   DoSrcImg)
    ON_COMMAND(IDM_PICSHOW_BIGIMG,   DoBigImg)
    ON_COMMAND(IDM_PICSHOW_SMALLIMG, DoSmallImg)
    ON_COMMAND(IDM_PICSHOW_FITIMG_WITH_SCALE, DoFitWndImgWithScale)
    ON_WM_ERASEBKGND()
    ON_WM_CONTEXTMENU()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEACTIVATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPicShowCtrl::RegisterWindowClass(HINSTANCE hInstance/* = NULL*/)
{
    if (CPicShowCtrl::m_bIsRegistered == FALSE)
    {
        LPCTSTR className = _T("CPicShowCtrlClass");
        WNDCLASS windowclass;
        if(!hInstance)
        {
            //hInstance = AfxGetInstanceHandle();
            hInstance = AfxGetResourceHandle();
        }

        if (!(::GetClassInfo(hInstance, className, &windowclass)))
        {
            windowclass.style = CS_DBLCLKS;
            windowclass.lpfnWndProc = ::DefWindowProc;
            windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
            windowclass.hInstance = hInstance;
            windowclass.hIcon = NULL;
            windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
            windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
            windowclass.lpszMenuName = NULL;
            windowclass.lpszClassName = className;

            if (!AfxRegisterClass(&windowclass))
            {
                AfxThrowResourceException();
                return FALSE;
            }
        }
        CPicShowCtrl::m_bIsRegistered = TRUE;
    }
    return TRUE;
}

#ifdef _DEBUG
void CPicShowCtrl::AssertValid() const
{
    CScrollView::AssertValid();
}

void CPicShowCtrl::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}
#endif //_DEBUG

void CPicShowCtrl::OnDraw(CDC* pDC)
{
    if (!m_bDraw)
    {
        return;
    }
    CRect rect;
    GetClientRect(&rect);

    CSize m_totalDev;
    if (!m_img.IsNull())
    {
        m_totalDev.cx = m_img.GetWidth() * m_dScaleX;
        m_totalDev.cy = m_img.GetHeight() * m_dScaleY;
    }
    else
    {
        m_totalDev.cx = rect.Width() * m_dScaleX;
        m_totalDev.cy = rect.Height() * m_dScaleY;
    }

    rect.right = max(rect.right, m_totalDev.cx);
    rect.bottom = max(rect.bottom, m_totalDev.cy);

    CSize sz(GetTotalSize());
    rect.right = max(rect.right, sz.cx);
    rect.bottom = max(rect.bottom, sz.cy);

    CMemDC mdc(*pDC,this);
    pDC = &mdc.GetDC();

    m_brHatch.UnrealizeObject();
    CPoint pt(0, 0);
    pDC->LPtoDP(&pt);
    pt = pDC->SetBrushOrg(pt.x % 8, pt.y % 8);
    CBrush* old = pDC->SelectObject(&m_brHatch);
    pDC->FillRect(&rect, &m_brHatch);
    pDC->SelectObject(old);

    if (!m_img.IsNull())
    {
        pDC->SetStretchBltMode(STRETCH_HALFTONE);
        double nsw = (double)m_img.GetWidth()*m_dScaleX;
        double nsy = (double)m_img.GetHeight()*m_dScaleY;

        m_nStartWidth = max((rect.Width() - nsw)*0.5, 0);
        m_nStartHeight = max((rect.Height() - nsy)*0.5, 0);

        m_img.Draw(pDC->GetSafeHdc(),
            m_nStartWidth, m_nStartHeight, nsw, nsy,
            0, 0, m_img.GetWidth(), m_img.GetHeight()
        );
    }

    static bool bTestMosPos = false;
    static bool bTestIPos = false;

    if (bTestMosPos)
    {
        pDC->SetTextColor(RGB(0,255,0));
        pDC->SetBkMode(TRANSPARENT);

        CString strMpos;
        strMpos.Format(_T("MousePt:%d,%d"),m_MouseMovePos.x,m_MouseMovePos.y);
        pDC->TextOut(550,550,strMpos);
    }
    
    if (bTestIPos)
    {
        CString strIpos;
        CPoint ptImg;
        GetImageXY(m_MouseMovePos,ptImg);
        strIpos.Format(_T("ImagePos:%d,%d"),ptImg.x,ptImg.y);
        pDC->TextOut(565,565,strIpos);
    }

    return;
}

void CPicShowCtrl::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    CRect rectClient;
    GetClientRect(rectClient);

    CSize sizeTotal;
    sizeTotal.cx = rectClient.Width();
    sizeTotal.cy = rectClient.Height();
    SetScrollSizes(MM_TEXT, sizeTotal);

    if (m_Menu.m_hMenu == NULL)
    {
        m_Menu.CreatePopupMenu();
        m_Menu.AppendMenu(MF_STRING, IDM_PICSHOW_FITIMG, _T("适合窗口"));
        m_Menu.AppendMenu(MF_STRING, IDM_PICSHOW_FITIMG_WITH_SCALE, _T("适应窗口(等比例)"));
        m_Menu.AppendMenu(MF_STRING, IDM_PICSHOW_SRCIMG, _T("原始大小"));
        m_Menu.AppendMenu(MF_STRING, IDM_PICSHOW_BIGIMG, _T("放大图片"));
        m_Menu.AppendMenu(MF_STRING, IDM_PICSHOW_SMALLIMG, _T("缩小图片"));
    }
}

void CPicShowCtrl::PostNcDestroy()
{

}

BOOL CPicShowCtrl::OnEraseBkgnd(CDC* pDC)
{
    if (!m_bDraw)
    {
        return TRUE;
    }
    return TRUE;
    return CScrollView::OnEraseBkgnd(pDC);
}

void CPicShowCtrl::DoFitWndImg()
{
    if (m_img.IsNull())
    {
        return;
    }
    if (!m_img.IsNull())
    {
        CRect rectClient;
        GetClientRect(rectClient);

        CSize sizeTotal;
        sizeTotal.cx = rectClient.Width();
        sizeTotal.cy = rectClient.Height();
        SetScrollSizes(MM_TEXT, sizeTotal);

        CRect rectWin;
        GetWindowRect(rectWin);
        ScreenToClient(&rectWin);

        m_dScaleX = (double)rectWin.Width()/(double)m_img.GetWidth();
        m_dScaleY = (double)rectWin.Height()/(double)m_img.GetHeight();

        if (m_Menu.m_hMenu != NULL)
        {
            CString strBuf;
            strBuf.Format(_T("适合窗口(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
            m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_CHECKED|MF_BYCOMMAND|MF_GRAYED,IDM_PICSHOW_FITIMG,strBuf);

            m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG_WITH_SCALE,_T("适应窗口(等比例)"));
            m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SRCIMG,_T("原始大小"));
            m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,_T("放大图片"));
            m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,_T("缩小图片"));
        }
        RedrawWindow();
    }
}

void CPicShowCtrl::DoFitWndImgWithScale()
{
    if (m_img.IsNull())
    {
        return;
    }
    if (!m_img.IsNull())
    {
        CRect rectClient;
        GetClientRect(rectClient);

        CSize sizeTotal;
        sizeTotal.cx = rectClient.Width();
        sizeTotal.cy = rectClient.Height();
        SetScrollSizes(MM_TEXT, sizeTotal);

        CRect rectWin;
        GetWindowRect(rectWin);
        ScreenToClient(&rectWin);

        m_dScaleX = (double)rectWin.Width()/(double)m_img.GetWidth();
        m_dScaleY = (double)rectWin.Height()/(double)m_img.GetHeight();

        if (m_dScaleX > m_dScaleY)
        {
            m_dScaleX = m_dScaleY;
        }
        else if (m_dScaleX < m_dScaleY)
        {
            m_dScaleY = m_dScaleX;
        }
        if (m_Menu.m_hMenu != NULL)
        {
            CString strBuf;
            strBuf.Format(_T("适合窗口(等比例)(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
            m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_CHECKED|MF_BYCOMMAND|MF_GRAYED,IDM_PICSHOW_FITIMG_WITH_SCALE,strBuf);

            m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG,_T("适应窗口"));
            m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SRCIMG,_T("原始大小"));
            m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,_T("放大图片"));
            m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,_T("缩小图片"));
        }

        RedrawWindow();
    }
}

void CPicShowCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
    if (GetFocus() != this)
        SetFocus();

    // Check for pop-up menu from keyboard
    if (point.x == -1 && point.y == -1)
    {
        point = CPoint(5,5);
        ClientToScreen(&point);
    }
    m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CPicShowCtrl::DoSrcImg()
{
    if (m_img.IsNull())
    {
        return;
    }
    m_dScaleX = 1;
    m_dScaleY = 1;

    CSize sizeTotal;
    sizeTotal.cx = m_img.GetWidth();
    sizeTotal.cy = m_img.GetHeight();
    SetScrollSizes(MM_TEXT, sizeTotal);

    if (m_Menu.m_hMenu != NULL)
    {
        CString strBuf;
        strBuf.Format(_T("原始大小(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
        m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_CHECKED|MF_BYCOMMAND|MF_GRAYED,IDM_PICSHOW_SRCIMG,strBuf);

        m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG,_T("适合窗口"));
        m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG_WITH_SCALE,_T("适应窗口(等比例)"));
        m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,_T("放大图片"));
        m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,_T("缩小图片"));
    }
    RedrawWindow();
}

void CPicShowCtrl::DoBigImg()
{
    if (m_img.IsNull())
    {
        return;
    }
    m_dScaleX += 0.05;
    m_dScaleY += 0.05;

    if (!m_img.IsNull())
    {
        CSize sizeTotal;
        sizeTotal.cx = m_img.GetWidth()  * m_dScaleX;
        sizeTotal.cy = m_img.GetHeight() * m_dScaleY;
        SetScrollSizes(MM_TEXT, sizeTotal);
    }

    if (m_Menu.m_hMenu != NULL)
    {
        CString strBuf;
        strBuf.Format(_T("放大图片(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
        m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_CHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,strBuf);

        m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG,_T("适合窗口"));
        m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SRCIMG,_T("原始大小"));
        m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG_WITH_SCALE,_T("适应窗口(等比例)"));
        m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,_T("缩小图片"));
    }

    RedrawWindow();
}

void CPicShowCtrl::DoSmallImg()
{
    if (m_img.IsNull())
    {
        return;
    }
    m_dScaleX -= 0.05;
    m_dScaleY -= 0.05;

    if (m_img.GetWidth()  * m_dScaleX < 10)
    {
        m_dScaleX = (double)10/(double)m_img.GetWidth();
    }
    if (m_img.GetHeight()  * m_dScaleY < 10)
    {
        m_dScaleY = (double)10/(double)m_img.GetHeight();
    }

    if (!m_img.IsNull())
    {
        CSize sizeTotal;
        sizeTotal.cx = m_img.GetWidth()  * m_dScaleX;
        sizeTotal.cy = m_img.GetHeight() * m_dScaleY;
        SetScrollSizes(MM_TEXT, sizeTotal);
    }

    if (m_Menu.m_hMenu != NULL)
    {
        CString strBuf;
        strBuf.Format(_T("缩小图片(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
        m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_CHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,strBuf);

        m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG,_T("适合窗口"));
        m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SRCIMG,_T("原始大小"));
        m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,_T("放大图片"));
        m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG_WITH_SCALE,_T("适应窗口(等比例)"));
    }
    RedrawWindow();
}


void CPicShowCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (GetFocus() != this)
        SetFocus();
    m_RefScroll = GetScrollPosition();
    m_RefPoint.x = point.x;
    m_RefPoint.y = point.y;
    m_eMouseState = eMouseScrollMove;
    SetCapture();

    CScrollView::OnLButtonDown(nFlags, point);
}



void CPicShowCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    ::ReleaseCapture();
    m_eMouseState = eMouseNormal;
    CScrollView::OnLButtonUp(nFlags, point);
}


void CPicShowCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    m_MouseMovePos = point;
    if(m_eMouseState == eMouseScrollMove)
    {
        CSize sz(GetTotalSize());
        CRect rClient;
        GetClientRect(&rClient);

        if ((sz.cx > rClient.Width()) || (sz.cy > rClient.Height()))
        {
            SetCursor(LoadCursor(0,IDC_SIZEALL));
            if (sz.cx>rClient.right) SetScrollPos(SB_HORZ,m_RefScroll.x - point.x + m_RefPoint.x);
            else SetScrollPos(SB_HORZ,0);
            if (sz.cy>rClient.bottom) SetScrollPos(SB_VERT,m_RefScroll.y - point.y + m_RefPoint.y);
            else SetScrollPos(SB_VERT,0);
        }
    }
    RedrawWindow();
    CScrollView::OnMouseMove(nFlags, point);
}

void CPicShowCtrl::GetImageXYAt(double dScollX, double dScollY, CPoint ptMouse, double& dImgX, double& dImgY)
{
    double fxAf = (double)(ptMouse.x - m_nStartWidth + dScollX) / m_dScaleX;
    double fyAf = (double)(ptMouse.y - m_nStartHeight + dScollY) / m_dScaleY;
    dImgX = fxAf;
    dImgY = fyAf;
}

BOOL CPicShowCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (!m_img.IsNull())
    {
        m_bDraw = false;

        double dScaleSOldX = m_dScaleX;
        double dScaleSOldY = m_dScaleY;

        CPoint ptMouse;
        GetCursorPos(&ptMouse);
        ScreenToClient(&ptMouse);

        CPoint ptscroll_before = GetScrollPosition();
        double fx_before = (double)(ptMouse.x - m_nStartWidth + ptscroll_before.x)/ m_dScaleX;
        double fy_before = (double)(ptMouse.y - m_nStartHeight + ptscroll_before.y)/ m_dScaleY;
        
        int s = zDelta / 120;
        m_dScaleX += s * 0.05;
        m_dScaleY += s * 0.05;

        if ((m_img.GetWidth()  * m_dScaleX < 16) || (m_img.GetHeight()  * m_dScaleY < 16))
        {
            m_dScaleX = dScaleSOldX;
            m_dScaleY = dScaleSOldY;
        }

        if ((m_img.GetWidth()  * m_dScaleX > 12800) || (m_img.GetHeight()  * m_dScaleY > 12800))
        {
            m_dScaleX = dScaleSOldX;
            m_dScaleY = dScaleSOldY;
        }


        CSize sizeTotal;
        sizeTotal.cx = m_img.GetWidth()  * m_dScaleX;
        sizeTotal.cy = m_img.GetHeight() * m_dScaleY;
        if (sizeTotal.cx < 0)
        {
            sizeTotal.cx = 1;
        }
        if (sizeTotal.cy < 0)
        {
            sizeTotal.cy = 1;
        }



        if (m_Menu.m_hMenu != NULL)
        {
            if ((m_dScaleX > 1) && (m_dScaleY > 1))
            {
                CString strBuf;
                strBuf.Format(_T("放大图片(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
                m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_CHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,strBuf);

                m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG,_T("适合窗口"));
                m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SRCIMG,_T("原始大小"));
                m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG_WITH_SCALE,_T("适应窗口(等比例)"));
                m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,_T("缩小图片"));
            }
            else
            {
                CString strBuf;
                strBuf.Format(_T("缩小图片(%d%%,%d%%)"),int(m_dScaleX*100), int(m_dScaleY*100));
                m_Menu.ModifyMenu(IDM_PICSHOW_SMALLIMG,MF_CHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SMALLIMG,strBuf);
                m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG,_T("适合窗口"));
                m_Menu.ModifyMenu(IDM_PICSHOW_SRCIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_SRCIMG,_T("原始大小"));
                m_Menu.ModifyMenu(IDM_PICSHOW_BIGIMG,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_BIGIMG,_T("放大图片"));
                m_Menu.ModifyMenu(IDM_PICSHOW_FITIMG_WITH_SCALE,MF_UNCHECKED|MF_BYCOMMAND|MF_ENABLED,IDM_PICSHOW_FITIMG_WITH_SCALE,_T("适应窗口(等比例)"));
            }
        }
        CSize szBefore(GetTotalSize());
        
        CPoint ptBefore = GetScrollPosition();
        
        SetScrollSizes(MM_TEXT, sizeTotal);

        CSize sz(GetTotalSize());
        CRect rClient;
        GetClientRect(&rClient);

        if ((sz.cx > rClient.Width()) || (sz.cy > rClient.Height()))
        {
            SCROLLINFO siHor;
            ZeroMemory(&siHor, sizeof(siHor));
            GetScrollInfo(SB_HORZ, &siHor);

            SCROLLINFO siVer;
            ZeroMemory(&siVer, sizeof(siVer));
            GetScrollInfo(SB_VERT, &siVer);

            double ptNowScroll_x,ptNowScroll_y;
            ptNowScroll_x = ptBefore.x * (sz.cx / (double)szBefore.cx);
            ptNowScroll_y = ptBefore.y * (sz.cy / (double)szBefore.cy);

            double fx_now(0), fy_now(0);

            double dMin = DBL_MAX;
            for (size_t i = 0; i < sz.cx; i++)
            {
                double fx_now = (double)(ptMouse.x - m_nStartWidth + i) / m_dScaleX;
                double fx_sub = fabs(fx_now - fx_before);
                if (fx_sub<dMin)
                {
                    dMin = fx_sub;
                    ptNowScroll_x = i;
                }
            }
            dMin = DBL_MAX;
            for (size_t i = 0; i < sz.cy; i++)
            {
                double fy_now = (double)(ptMouse.y - m_nStartHeight + i) / m_dScaleY;
                double fy_sub = fabs(fy_now - fy_before);
                if (fy_sub < dMin)
                {
                    dMin = fy_sub;
                    ptNowScroll_y = i;
                }
            }

            SetScrollPos(SB_HORZ, ptNowScroll_x);
            SetScrollPos(SB_VERT, ptNowScroll_y);
        }
    }


    m_bDraw = true;
    RedrawWindow();
    return TRUE;
    return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CPicShowCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    CScrollView::OnRButtonDown(nFlags, point);
}


void CPicShowCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
    CScrollView::OnRButtonUp(nFlags, point);
}

void CPicShowCtrl::DoInit()
{
    OnInitialUpdate();
}

void CPicShowCtrl::LoadImg(LPCTSTR pszFile)
{
    if (!m_img.IsNull())
    {
        m_img.Destroy();
    }
    m_img.Load(pszFile);
}

void CPicShowCtrl::GetImageXY(CPoint ptMouse,CPoint& ptImg)
{
    CPoint point=GetScrollPosition();
    double fx =((double)(ptMouse.x - m_nStartWidth + point.x))/ m_dScaleX;
    double fy =((double)(ptMouse.y - m_nStartHeight + point.y)) / m_dScaleY;

    ptImg.x = (long)fx;
    ptImg.y = (long)fy;
    return;
}

void CPicShowCtrl::DrawElp(RECT& rc)
{
    HDC pImgDC = m_img.GetDC();
    CDC oImgdc;
    oImgdc.Attach(pImgDC);

    oImgdc.SetBkMode(TRANSPARENT);

    CPen pen;
    pen.CreatePen(PS_SOLID,2,RGB(255,0,0));
    CPen* pOldPen = oImgdc.SelectObject(&pen);

    oImgdc.Ellipse(& rc);

    oImgdc.SelectObject(pOldPen);
    oImgdc.Detach();
    m_img.ReleaseDC();
}


void CPicShowCtrl::DrawRect(RECT& rc)
{
    HDC pImgDC = m_img.GetDC();
    CDC oImgdc;
    oImgdc.Attach(pImgDC);

    oImgdc.SetBkMode(TRANSPARENT);

    CPen pen;
    pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
    CPen* pOldPen = oImgdc.SelectObject(&pen);

    oImgdc.MoveTo(rc.left, rc.top);
    oImgdc.LineTo(rc.right, rc.top);
    oImgdc.LineTo(rc.right, rc.bottom);
    oImgdc.LineTo(rc.left, rc.bottom);
    oImgdc.LineTo(rc.left, rc.top);

    oImgdc.SelectObject(pOldPen);
    oImgdc.Detach();
    m_img.ReleaseDC();
}

void CPicShowCtrl::OpenFileDlg2LoadImg()
{
    CString strFilter;
    CSimpleArray<GUID> aguidFileTypes;
    HRESULT hResult;

    hResult = CImage::GetExporterFilterString(strFilter,aguidFileTypes);
    if (FAILED(hResult))
    {
        CString fmt;
        fmt.Format(_T("GetExporterFilter failed:\n%x"), hResult/*, _com_error(hResult).ErrorMessage()*/);
        ::AfxMessageBox(fmt);
        return ;
    }
    CString szFilterAll = _T("所有格式图像 (*.*)|*.bmp;*jpg;*.tif;*.tiff;*.pgm;*.jpeg;*.png|");
    CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilterAll+strFilter);
    static int nDefFilterLoad = 0;
    dlg.m_ofn.nFilterIndex = nDefFilterLoad;
    hResult = (int)dlg.DoModal();
    if(FAILED(hResult))
    {
        return ;
    }

    nDefFilterLoad = dlg.m_ofn.nFilterIndex;

    CString mPath = dlg.GetPathName();
    LoadImg(mPath);
    return ;
}


int CPicShowCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    return CScrollView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CPicShowCtrl::OnSize(UINT nType, int cx, int cy)
{
    CScrollView::OnSize(nType, cx, cy);
    RedrawWindow();
}


