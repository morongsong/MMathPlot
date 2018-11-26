

#include "stdafx.h"
#include "MapCoordDraw.h"
//#include "CImageUtily.h"
#include "MapCoordRange.h"
#include <algorithm>
#if (defined(_MSC_VER) && (_MSC_VER >= 1200))
#pragma warning(push)
#pragma warning(disable : 4786)
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#endif // _MSC_VER >= 1200

void MvCoordAxis::Move(double x,double y)
{
    ptDrawOrg.Move(x,y);
}

void MvCoordAxis::DrawAxis(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(PS_SOLID,1,RGB(192,192,192));
    CPen* pOldPen = pDC->SelectObject(&pen);

    int sx = 0;
    int sy = 0;
    if (m_eDrawType == EDrawByRPt)
    {
        sx = MapView.R2SX(ptDrawOrg.x);
        sy = MapView.R2SY(ptDrawOrg.y);
    }
    else
    {
        sx = ptDrawOrg.x;
        sy = ptDrawOrg.y;
    }


    //Draw X Axis
    pDC->MoveTo(0,sy);
    pDC->LineTo(MapView.m_nSWidth,sy);


    pDC->MoveTo(MapView.m_nSWidth-3,sy);
    pDC->LineTo(MapView.m_nSWidth-8,sy-5);
    pDC->LineTo(MapView.m_nSWidth-8,sy+5);
    pDC->LineTo(MapView.m_nSWidth-3,sy);

    //Draw Y Axis
    pDC->MoveTo(sx,0);
    pDC->LineTo(sx,MapView.m_nSHeight);

    pDC->MoveTo(sx,3);
    pDC->LineTo(sx-5,8);
    pDC->LineTo(sx+5,8);
    pDC->LineTo(sx,3);

    pDC->SelectObject(pOldPen);
}

void MvCoordAxis::DrawKD(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(PS_SOLID,1,RGB(192,192,192));
    CPen* pOldPen = pDC->SelectObject(&pen);

    CPen pen2;
    pen2.CreatePen(PS_DOT,1,RGB(192,192,192));

    CString strKD;

    pDC->SetTextColor(RGB(255,255,0));
    pDC->SetBkMode(TRANSPARENT);

    // 	int scdx = MapView.R2SX(ptDrawOrg.x);
    // 	int scdy = MapView.R2SY(ptDrawOrg.y);
    int scdx = 0;
    int scdy = 0;
    if (m_eDrawType == EDrawByRPt)
    {
        scdx = MapView.R2SX(ptDrawOrg.x);
        scdy = MapView.R2SY(ptDrawOrg.y);
    }
    else
    {
        scdx = ptDrawOrg.x;
        scdy = ptDrawOrg.y;
    }

    double dRDisX = MapView.m_nSWidth / 8  * MapView.m_dScaleX;
    if (dRDisX >= 1)
    {
        int nRDisX = dRDisX * 10 / 10;
        dRDisX = nRDisX;
    }

    //double dRDisX = 2.0;
    double x0 = MapView.S2RX(0);
    double x1 = MapView.S2RX(MapView.m_nSWidth);

    int nStartX = (x0)/dRDisX-1;
    int nEndX = (x1)/dRDisX+1;

    for (int i = nStartX; i <= nEndX; i++)
    {
        double rx = i * dRDisX;
        int sx = MapView.R2SX(rx);
        if (sx < 0)
        {
            continue;
        }
        if (sx > MapView.m_nSWidth)
        {
            continue;
        }

        pDC->SelectObject(&pen);
        pDC->MoveTo(sx,scdy-10);
        pDC->LineTo(sx,scdy+10);

        double abrx = fabs(rx);
        if (abrx >= 1)
        {
            strKD.Format(_T("%.1f"),rx);
        }
        else if ((abrx >= 0.1) && (abrx < 1))
        {
            strKD.Format(_T("%.2f"),rx);
        }
        else 
        {
            strKD.Format(_T("%.4f"),rx);
        }
        if (i == 0)
        {
            strKD.Format(_T("0"));
        }

        pDC->TextOut(sx - 15 ,scdy + 12,strKD);

        pDC->SelectObject(&pen2);
        pDC->MoveTo(sx,MapView.m_nSHeight - 10);
        pDC->LineTo(sx,10);
    }


    double dRDisY = MapView.m_nSHeight / 8 * MapView.m_dScaleY;
    if (dRDisY >= 1)
    {
        int nRDisY = dRDisY * 10 / 10;
        dRDisY = nRDisY;
    }

    double y0 = MapView.S2RY(0);
    double y1 = MapView.S2RY(MapView.m_nSHeight);

    int nStartY = (y1)/dRDisY-1;
    int nEndY = (y0)/dRDisY+1;

    for (int i = nStartY; i <= nEndY; i++)
    {
        double ry = i * dRDisY;
        int sy = MapView.R2SY(ry);
        if (sy < 0)
        {
            continue;
        }
        if (sy > MapView.m_nSHeight)
        {
            continue;
        }

        pDC->SelectObject(&pen);
        pDC->MoveTo(scdx-10,sy);
        pDC->LineTo(scdx+10,sy);

        double abry = fabs(ry);
        if (abry >= 1)
        {
            strKD.Format(_T("%.1f"),ry);
        }
        else if ((abry > 0.1) && (abry < 1))
        {
            strKD.Format(_T("%.2f"),ry);
        }
        else 
        {
            strKD.Format(_T("%.4f"),ry);
        }
        if (i == 0)
        {
            strKD.Format(_T("0"));
        }

        pDC->TextOut(scdx-25,sy-10,strKD);

        pDC->SelectObject(&pen2);
        pDC->MoveTo(10,sy);
        pDC->LineTo(MapView.m_nSWidth - 10,sy);
    }

    pDC->SelectObject(pOldPen);
}

void MvCoordAxis::Draw(CDC* pDC)
{
    DrawAxis(pDC);
    DrawKD(pDC);
}

void MvCoordAxis::Rote(double r)
{
    ptDrawOrg.Rote(r);
    dAngle = r;
}

void MvCoordAxis::Scale(double sx,double sy)
{
    ptDrawOrg.Scale(sx,sy);
}

////////////////////////////////////////////////////////////////////////////////////////

void MvMapPoint8B::Move(double x,double y)
{
    for (int i = 0 ; i < 1000; i++)
    {
        pt8B[i].Move(x,y);
    }
}

void MvMapPoint8B::Draw(CDC* pDC)
{
    if (m_nDrawFlag == MvMapPoint8B::eDrawNull)
    {
        return;
    }
    CPen pen;
    pen.CreatePen(PS_SOLID,1, m_DrawClr);
    CPen* pOldPen = pDC->SelectObject(&pen);
    if (m_nDrawFlag== MvMapPoint8B::eDrawPots)
    {
        for (int i = 0; i < 1000; i++)
        {
            MvMapPoint& pt = pt8B[i];
            double sx = MapView.R2SX(pt.x);
            double sy = MapView.R2SY(pt.y);
            pDC->Ellipse(sx - 1, sy - 1, sx + 1, sy + 1);
        }
    }
    else if (m_nDrawFlag == MvMapPoint8B::eDrawLines)
    {
        for (int i = 0; i < (1000-1); i++)
        {
            MvMapPoint& pt = pt8B[i];
            double sx = MapView.R2SX(pt.x);
            double sy = MapView.R2SY(pt.y);
            MvMapPoint& pt2 = pt8B[i+1];
            double sx2 = MapView.R2SX(pt2.x);
            double sy2 = MapView.R2SY(pt2.y);
            pDC->MoveTo(sx, sy);
            pDC->LineTo(sx2, sy2);
            //pDC->Ellipse(sx - 1, sy - 1, sx + 1, sy + 1);
        }
    }

    pDC->SelectObject(pOldPen);
}

void MvMapPoint8B::Rote(double r)
{
    for (int i = 0 ; i < 800; i++)
    {
        pt8B[i].Rote(r);
    }
}

void MvMapPoint8B::Scale(double sx,double sy)
{
    for (int i = 0 ; i < 800; i++)
    {
        pt8B[i].Scale(sx,sy);
    }
}

MvMapPoint8B::MvMapPoint8B( MvMapView& argMapView ) : MapView(argMapView)
{
    m_nDrawFlag = eDrawPots;
    m_DrawClr = RGB(0, 255, 0);
}



////////////////////////////////////////////////////////////////////////////////////////


#define MVMAPVIEW_OBJ_FOREACH_DONE(TheDoneCodeS)\
for (std::vector<MapCoordObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); iter++)\
{\
    MapCoordObj* pobj = *iter;\
    pobj->TheDoneCodeS;\
}

#define MR_STL_FOR_EACH_BEGIN(Type,Val,ContainVal)\
{\
    struct for_each_opt\
    {\
        void operator()(Type& Val)\
        {

#define MR_STL_FOR_EACH_END(Type,Val,ContainVal)\
        }\
    };\
    for_each_opt fobj;\
    std::for_each (ContainVal.begin(),ContainVal.end(),fobj);\
}
////////////////////////////////////////////////////////////////////////////////////////

MvMapView::MvMapView() : m_CoordAxis(*this),m_MapPoint8B(*this), m_MapPoint8B2(*this)
{
    m_bHoldLeftMouse = false; 
    m_pSelElemt = nullptr;
    m_pNeedDelElemt = nullptr;
    m_pHoldWnd = nullptr;
}

MvMapView::~MvMapView()
{
    MR_STL_FOR_EACH_BEGIN(MapCoordObj*,pItem,m_objs)
    {
        delete pItem;
    }
    MR_STL_FOR_EACH_END(MapCoordObj*,pItem,m_objs)
}

void MvMapView::Draw()
{
    if (m_oImage.IsNull())
    {
        m_oImage.Create(m_nSWidth,m_nSHeight,24);
    }
    if ((m_nSHeight != m_oImage.GetHeight()) || (m_nSWidth != m_oImage.GetWidth()) )
    {
        m_oImage.Destroy();
        m_oImage.Create(m_nSWidth,m_nSHeight,24);
    }
    //CImageUtily::FillImage(m_oImage);
    {
        int ny = m_oImage.GetHeight();
        int nx = m_oImage.GetWidth() * (m_oImage.GetBPP() / 8);
        for (int iy = 0; iy < ny; iy++)
        {
            BYTE* pdst = (BYTE*)m_oImage.GetPixelAddress(0, iy);
            for (int ix = 0; ix < nx; ix++)
            {
                *pdst++ = 0;
            }
        }
    }


    HDC pImgDC = m_oImage.GetDC();

    CDC oImgdc;
    oImgdc.Attach(pImgDC);

    m_CoordAxis.Draw(&oImgdc);
    m_MapPoint8B.Draw(&oImgdc);
    m_MapPoint8B2.Draw(&oImgdc);

    if (m_pNeedDelElemt)
    {
        if (m_pSelElemt == m_pNeedDelElemt)
        {
            m_pSelElemt =  nullptr;
        }


        EraseObj(m_pNeedDelElemt);
        m_pNeedDelElemt = nullptr;
    }
    MVMAPVIEW_OBJ_FOREACH_DONE(Draw(&oImgdc););
    oImgdc.Detach();
    m_oImage.ReleaseDC();
    return;
}

void MvMapView::MoveSOrg(int x,int y)
{
    m_nSOrgX += x;
    m_nSOrgY += y;
}

void MvMapView::ScaleSRView( int sx,int sy )
{
    m_dScaleX += (double)sx * 0.01;
    m_dScaleY += (double)sy * 0.01;

    if (m_dScaleY < 0.000001)
    {
        m_dScaleY = 0.000001;
    }
    if (m_dScaleX < 0.000001)
    {
        m_dScaleX = 0.000001;
    }
}

void MvMapView::Rote(double r)
{
    // 	for (std::vector<MvMapObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); iter++)
    // 	{
    // 		MvMapObj*& pobj = *iter;
    // 		pobj->Rote(r);
    // 	}
}

void MvMapView::R2SX(const double RX,double& SX)
{
    double dResult = RX / m_dScaleX + m_nSOrgX;
    SX = dResult;
    return;
}

double MvMapView::R2SX( const double RX )
{
    return RX / m_dScaleX + m_nSOrgX;
}

void MvMapView::R2SY(const double RY,double& SY)
{
    double dResult = m_nSOrgY - RY / m_dScaleY ;
    SY = dResult;
    return;
}

double MvMapView::R2SY( const double RY )
{
    return m_nSOrgY - RY / m_dScaleY ;
}

void MvMapView::S2RX(const double SX,double& RX)
{
    double dResult = (SX - m_nSOrgX) * m_dScaleX;
    RX = dResult;
    return;
}

void MvMapView::S2RY(const double SY,double& RY)
{
    double dResult = -(SY - m_nSOrgY) * m_dScaleY;
    RY = dResult;
    return;
}

double MvMapView::S2RX(const double SX)
{
    return (SX - m_nSOrgX) * m_dScaleX;
}

double MvMapView::S2RY(const double SY)
{
    return -(SY - m_nSOrgY) * m_dScaleY;
}

MvMapPoint MvMapView::R2S(MvMapPoint& pt)
{
    MvMapPoint ptResult;
    ptResult.x = R2SX(pt.x);
    ptResult.y = R2SY(pt.y);
    return ptResult;
}

MvMapPoint MvMapView::S2R(MvMapPoint& pt)
{
    MvMapPoint ptResult;
    ptResult.x = S2RX(pt.x);
    ptResult.y = S2RY(pt.y);
    return ptResult;
}

MvMapPoint MvMapView::S2R( CPoint& pt )
{
    MvMapPoint ptResult;
    ptResult.x = S2RX(pt.x);
    ptResult.y = S2RY(pt.y);
    return ptResult;
}

void MvMapView::R2SXLen(const double RXLen,double& SX)
{
    SX = RXLen * m_dScaleX;
}

void MvMapView::R2SYLen(const double RYLen,double& SY)
{
    SY = RYLen * m_dScaleY;
}

double MvMapView::R2SXLen(const double RXLen)
{
    return RXLen * m_dScaleX;
}

double MvMapView::R2SYLen(const double RYLen)
{
    return RYLen* m_dScaleY;
}


void MvMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bHoldLeftMouse = true;
    m_ptRefLBOrg = point;
    MVMAPVIEW_OBJ_FOREACH_DONE(OnLButtonDown(nFlags,point));
}

void MvMapView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    m_bHoldLeftMouse = false;
    MVMAPVIEW_OBJ_FOREACH_DONE(OnLButtonUp(nFlags,point));

}

void MvMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
    MVMAPVIEW_OBJ_FOREACH_DONE(OnRButtonDown(nFlags,point));
}

void MvMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
    MVMAPVIEW_OBJ_FOREACH_DONE(OnRButtonUp(nFlags,point));
}

void MvMapView::OnMouseMove(UINT nFlags, CPoint point) 
{
    {
       
        MVMAPVIEW_OBJ_FOREACH_DONE(OnMouseMove(nFlags,point));

        if (!m_pSelElemt)
        {
            if (m_bHoldLeftMouse)
            {
                int nMoveX = point.x - m_ptRefLBOrg.x;
                int nMoveY = point.y - m_ptRefLBOrg.y;
                MoveSOrg(nMoveX,nMoveY);
                m_ptRefLBOrg = point;
            }
        }

    }
    return;
}

BOOL MvMapView::OnMouseWheel(UINT nFlags, SHORT zDelta,CPoint point)
{
    MVMAPVIEW_OBJ_FOREACH_DONE(OnMouseWheel(nFlags,zDelta,point));
    if (!m_pSelElemt)
    {
        int s = -1 * (zDelta / 120);
        ScaleSRView(s,s);
    }
    return TRUE;
}

void MvMapView::ClearObjs()
{
    for (std::vector<MapCoordObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); iter++)
    {
        MapCoordObj* pobj = *iter;
        delete pobj;
    }
    m_objs.clear();
}




void MvMapView::EraseObj(MapCoordObj* pobj)
{
    std::vector<MapCoordLine*> refLinePtrs;
    for (std::vector<MapCoordObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); iter++)
    {
        MapCoordObj* pitemCur = *iter;
        if (pitemCur->GetElementType() == MapCoordObj::EMapCoordLine)
        {
            MapCoordLine* pLine = (MapCoordLine*)pitemCur;
            if ((pLine->m_ref1 == pobj) || (pLine->m_ref2 == pobj))
            {
                refLinePtrs.push_back(pLine);
            }
        }
    }

    for (std::vector<MapCoordObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); iter++)
    {
        MapCoordObj* pobjitem = *iter;
        if (pobjitem == pobj)
        {
            m_objs.erase(iter);
            delete pobj;
            break;
        }
    }

    for (std::vector<MapCoordLine*>::iterator iterLine = refLinePtrs.begin(); iterLine != refLinePtrs.end(); iterLine++)
    {
        MapCoordLine* pobjitemLine = *iterLine;
        for (std::vector<MapCoordObj*>::iterator iterObj = m_objs.begin(); iterObj != m_objs.end();)
        {
            MapCoordObj* pobjitem = *iterObj;
            if (pobjitem == pobjitemLine)
            {
                iterObj = m_objs.erase(iterObj);
                delete pobjitem;
                continue;
            }
            else
            {
                 iterObj++;
            }
        }
    }
    return;
}

void MvMapView::SetSelObj( MapCoordObj* pobj )
{
    //if (m_pSelElemt)
    //{
        for (std::vector<MapCoordObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); iter++)
        {
            MapCoordObj* pobj = *iter;
            if (pobj != m_pSelElemt)
            {
                pobj->SetSelStatus(0);
            }
        }
        m_pSelElemt = pobj;
        m_pSelElemt->SetSelStatus(2);
    //}
}




bool MvMapView::IsExist(MapCoordObj* pobj)
{
    bool bExist = false;
    for (int i = 0,nSize = m_objs.size(); i < nSize; i++)
    {
        MapCoordObj* pObjItem = m_objs.at(i);
        if (pobj == pObjItem)
        {
            bExist = true;
            break;
        }
    }
    return bExist;
}

bool MvMapView::ClearNoFinsh()
{
    for (std::vector<MapCoordObj*>::iterator iter = m_objs.begin(); iter != m_objs.end(); )
    {
        MapCoordObj* pobjitem = *iter;
        if (!pobjitem->IsConFinish())
        {
            iter = m_objs.erase(iter);
            delete pobjitem;
            continue;
        }
        else
        {
            iter++;
        }
    }
    return true;
}

int MvMapView::GetRangCount()
{
    int nCount = 0;
    for (int i = 0,nSize = m_objs.size(); i < nSize; i++)
    {
        MapCoordObj* pObjItem = m_objs.at(i);
        if (pObjItem->GetElementType() == MapCoordObj::EMapCoordRange)
        {
            nCount++;
        }
    }
    return nCount;
}
MapCoordRange* MvMapView::GetRangPtr(int nIndex)
{
    int nCount = 0;
    for (int i = 0,nSize = m_objs.size(); i < nSize; i++)
    {
        MapCoordObj* pObjItem = m_objs.at(i);
        if (pObjItem->GetElementType() == MapCoordObj::EMapCoordRange)
        {
            nCount++;
        }
        if (nCount == nIndex)
        {
            MapCoordRange* pResult = (MapCoordRange*)pObjItem;
            return pResult;
        }
    }
    return nullptr;
}






#if (defined(_MSC_VER) && (_MSC_VER >= 1200))
#pragma warning(pop)
#endif // _MSC_VER >= 1200