/** 
   *@file         MapCoordDrawUtil.cpp
   *@brief        MapCoordDrawUtil.cpp
*/

#include "stdafx.h"
#include "MapCoordDrawUtil.h"


void MvMapPoint::Rote(double r) 
{
    double cosv = cos(r) ; 
    double sinv = sin(r) ;
    double xc = x ;
    double yc = y ;
    double px = xc * cosv - yc * sinv ;
    double py = sinv * xc + cosv * yc ;
    x = px;
    y = py;
    return ;
}

void MvMapPoint::Rote(double x,double y,double r)
{
    double cosv = cos(r) ; 
    double sinv = sin(r) ;
    double xc = x ;
    double yc = y ;
    double px = xc * cosv - yc * sinv + (1- cosv) * x + y * sinv ;
    double py = sinv * xc + cosv * yc + (1- cosv) * y - sinv * x ;
    x = px;
    y = py;
    return ;
}



///构造函数
MapCoordDrawUtil::MapCoordDrawUtil()
{
   
}
///析构函数 
MapCoordDrawUtil::~MapCoordDrawUtil()
{

}

bool MapCoordDrawUtil::IsPointNear(MvMapPoint& pt1,MvMapPoint& pt2,int nNear/* = 3*/)
{
    bool bResult = false;
    if (abs(pt1.x-pt2.x) <= nNear)
    {
        if (abs(pt1.y-pt2.y) <= nNear)
        {
            bResult = true;
        }
    }
    return bResult;
}

bool MapCoordDrawUtil::IsPointNearRectEdge(MvMapPoint& ptLT,MvMapPoint& ptRB,MvMapPoint& point,int nNear/* = 3*/)
{
    bool bResult = false;
    if (ptLT.y < ptRB.y)
    {
        if ((point.y >= ptLT.y) && (point.y <= ptRB.y))
        {
            if( (abs(point.x-ptLT.x)<=nNear) || (abs(point.x-ptRB.x)<=nNear))
            {
                bResult = true;
            }
        }
    }
    else
    {
        if ((point.y <= ptLT.y) && (point.y >= ptRB.y))
        {
            if( (abs(point.x-ptLT.x)<=nNear) || (abs(point.x-ptRB.x)<=nNear))
            {
                bResult = true;
            }
        }
    }

    if (ptLT.x < ptRB.x)
    {
        if ((point.x >= ptLT.x) && (point.x <= ptRB.x))
        {
            if ( (abs(point.y-ptLT.y)<=nNear) || (abs(point.y-ptRB.y)<=nNear))
            {
                bResult = true;
            }
        }
    }
    else
    {
        if ((point.x <= ptLT.x) && (point.x >= ptRB.x))
        {
            if ( (abs(point.y-ptLT.y)<=nNear) || (abs(point.y-ptRB.y)<=nNear))
            {
                bResult = true;
            }
        }
    }

    return bResult;
}

void MapCoordDrawUtil::DrawRectByPt(CDC* pDC,CPoint& pt,int nsize)
{
    pDC->MoveTo(pt.x-nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y+nsize);
    pDC->LineTo(pt.x-nsize, pt.y+nsize);
    pDC->LineTo(pt.x-nsize, pt.y-nsize);
}

void MapCoordDrawUtil::DrawRectByPt(CDC* pDC,MvMapPoint& pt,int nsize)
{
    pDC->MoveTo(pt.x-nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y+nsize);
    pDC->LineTo(pt.x-nsize, pt.y+nsize);
    pDC->LineTo(pt.x-nsize, pt.y-nsize);
}


void MapCoordDrawUtil::DrawRectByPtFill(CDC* pDC,CPoint& pt,int nsize)
{
    CRect rc(pt.x-nsize, pt.y-nsize,pt.x+nsize, pt.y+nsize);
    CBrush brush;
    brush.CreateSolidBrush(RGB(192,192,192));
    pDC->FillRect(&rc,&brush);
}

void MapCoordDrawUtil::DrawRectByPtFill(CDC* pDC,MvMapPoint& pt,int nsize)
{
    CRect rc(pt.x-nsize, pt.y-nsize,pt.x+nsize, pt.y+nsize);
    CBrush brush;
    brush.CreateSolidBrush(RGB(192,192,192));
    pDC->FillRect(&rc,&brush);
}

void MapCoordDrawUtil::DrawRectCross(CDC* pDC,CPoint& pt,int nsize)
{
    pDC->MoveTo(pt.x-nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y+nsize);
    pDC->LineTo(pt.x-nsize, pt.y+nsize);
    pDC->LineTo(pt.x-nsize, pt.y-nsize);

    pDC->LineTo(pt.x+nsize, pt.y+nsize);

    pDC->MoveTo(pt.x+nsize, pt.y-nsize);
    pDC->LineTo(pt.x-nsize, pt.y+nsize);
}

void MapCoordDrawUtil::DrawRectCrossOnly(CDC* pDC,COLORREF clr,MvMapPoint& pt,int nsize)
{
    CPen pen;
    pen.CreatePen(PS_SOLID,2,clr);
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(pt.x-nsize, pt.y-nsize);
    pDC->LineTo(pt.x+nsize, pt.y+nsize);

    pDC->MoveTo(pt.x+nsize, pt.y-nsize);
    pDC->LineTo(pt.x-nsize, pt.y+nsize);

    pDC->SelectObject(pOldPen);
    return ;
}

void MapCoordDrawUtil::DrawLine(CDC* pDC,int nPenType,COLORREF clr,MvMapPoint& pt1,MvMapPoint& pt2)
{
    CPen pen;
    pen.CreatePen(nPenType,1,clr);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(pt1.x, pt1.y);
    pDC->LineTo(pt2.x, pt2.y);

    pDC->SelectObject(pOldPen);
    return ;
}



bool MapCoordDrawUtil::IsPtNearPt(CPoint point,CPoint point2,int nNear/*=3*/)
{
    if (abs(point.x-point2.x) <  nNear)
    {
        if (abs(point.y-point2.y) <  nNear)
        {
            return true;
        }
    }
    return false;
}