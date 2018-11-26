
/** 
   *@file         MapCoordRange.cpp
   *@brief        MapCoordRange.cpp
   *@author       jimsmorong
   *@date         2015/11/20 10:42:31
*/
#include "stdafx.h"

#include "MapCoordDraw.h"
#include "MapCoordDrawUtil.h"
#include "MapCoordSel.h"
#include "MapCoordRange.h"
//#include "ControlLogic\LineLaser.h"

///¹¹Ôìº¯Êý
MapCoordRange::MapCoordRange( MvMapView& argMapView ) : MapView(argMapView)
{
    m_nStep = 0;
     m_sel.m_eSelectType = MvCoordSel::eSelectTypeRange;
    m_sel.m_pRefHoldObj = this;
    m_nCheckType = 0;
}



////////////////////////////////////////////////////////////////////////////////////////

void MapCoordRange::Move(double x,double y)
{
    ptLT.Move(x,y);
    ptRB.Move(x,y);
}
void MapCoordRange::Draw(CDC* pDC)
{
    MvMapPoint ptDLT;
    MvMapPoint ptDRT;
    MvMapPoint ptDRB;
    MvMapPoint ptDLB;
    MvMapPoint ptDResult;

    /*
    (0)LT------RT(1)   
    |        |
    |        |
    |        |
    (4)LB------RB(2)
    */
    bool bShowResultPt = Calc();
    

    if (m_eDrawType == EDrawByRPt)
    {
        ptDLT = MapView.R2S(ptLT);
        ptDRT = MapView.R2S(MvMapPoint(ptRB.x,ptLT.y));
        ptDRB = MapView.R2S(ptRB);
        ptDLB = MapView.R2S(MvMapPoint(ptLT.x,ptRB.y));
        if (bShowResultPt)
        {
            ptDResult = MapView.R2S(m_ptResult);
        }
        
    }
    else
    {
        ptDLT = ptLT;
        ptDRT = MvMapPoint(ptRB.x,ptLT.y);
        ptDRB = ptRB;
        ptDLB = MvMapPoint(ptLT.x,ptRB.y);
        if (bShowResultPt)
        {
            ptDResult = MapView.R2S(m_ptResult);
        }
    }
    if (bShowResultPt)
    {
        if (m_nStep == 2)
        {
            MapCoordDrawUtil::DrawRectCrossOnly(pDC,RGB(255,0,0),ptDResult,4);
        }
    }
    //MvMapPoint ptDCT((ptDLT.x+ptDRB.x)/2,(ptDLT.y+ptDRB.y)/2);
    CPen pen;
    pen.CreatePen(PS_SOLID,1,RGB(255,255,0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(ptDLT.x,ptDLT.y);
    pDC->LineTo(ptDRT.x,ptDRT.y);
    pDC->LineTo(ptDRB.x,ptDRB.y);
    pDC->LineTo(ptDLB.x,ptDLB.y);
    pDC->LineTo(ptDLT.x,ptDLT.y);


    pDC->SetBkMode(TRANSPARENT);

    //MapCoordDrawUtil::DrawRectByPt(pDC,ptDCT,5);

    pDC->SelectObject(&pOldPen);
    if (m_nStep == 2)
    {
        m_sel.m_ptLT.x = ptDLT.x;
        m_sel.m_ptLT.y = ptDLT.y;
        m_sel.m_ptRB.x = ptDRB.x;
        m_sel.m_ptRB.y = ptDRB.y;
        m_sel.Draw(pDC);
    }
    return;
}

void MapCoordRange::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_nStep == 1)
    {
        if (m_eDrawType == EDrawBySPt)
        {
            ptRB = point;
        }
        else if (m_eDrawType == EDrawByRPt)
        {
            ptRB = MapView.S2R(point);
        }
    }
    if (m_nStep == 2)
    {
        m_sel.OnMouseMove(nFlags,point);
    }
}


void MapCoordRange::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_nStep == 0)
    {
        //ptLT = point;
        //ptRB = point;
        if (m_eDrawType == EDrawBySPt)
        {
            ptLT = point;
        }
        else if (m_eDrawType == EDrawByRPt)
        {
            ptLT = MapView.S2R(point);
        }
        if (m_eDrawType == EDrawBySPt)
        {
            ptRB = point;
        }
        else if (m_eDrawType == EDrawByRPt)
        {
            ptRB = MapView.S2R(point);
        }
        m_nStep++;
    }
    else if (m_nStep == 1)
    {
        //ptRB = point;
        if (m_eDrawType == EDrawBySPt)
        {
            ptRB = point;
        }
        else if (m_eDrawType == EDrawByRPt)
        {
            ptRB = MapView.S2R(point);
        }
        m_nStep++;
    }  

    if (m_nStep == 2)
    {
        m_sel.OnLButtonDown(nFlags,point);
    }

}

void MapCoordRange::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_nStep == 2)
    {
        m_sel.m_pRefHoldObj = this;
        m_sel.OnLButtonUp(nFlags,point);
    }
}

void MapCoordRange::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_nStep == 2)
    {
        m_sel.OnRButtonDown(nFlags,point);
    }
}
void MapCoordRange::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (m_nStep == 2)
    {
        m_sel.OnRButtonUp(nFlags,point);
    }
}

void MapCoordRange::Rote(double r)
{
    ptLT.Rote(r);
    ptRB.Rote(r);
}
void MapCoordRange::Scale(double sx,double sy)
{
    ptLT.Scale(sx,sy);
    ptRB.Scale(sx,sy);
}

#define MVMAPPOINT4R_CHANGE_RS_X(ptXX,mx) \
if (m_eDrawType == MvCoordAxis::EDrawByRPt)\
{\
    ptXX.x += MapView.R2SXLen(mx);\
}\
else if (m_eDrawType == MvCoordAxis::EDrawBySPt)\
{\
    ptXX.x += mx;\
}

#define MVMAPPOINT4R_CHANGE_RS_Y(ptXX,my) \
if (m_eDrawType == MvCoordAxis::EDrawByRPt)\
{\
    ptXX.y -= MapView.R2SYLen(my);\
}\
else if (m_eDrawType == MvCoordAxis::EDrawBySPt)\
{\
    ptXX.y += my;\
}

void MapCoordRange::Resize( int ntype,int mx,int my )
{
    if (ntype == MvCoordSel::eMTPtRTRBM)
    {
//         if (m_eDrawType == MvCoordAxis::EDrawByRPt)
//         {
//             ptRB.x += MapView.R2SXLen(mx);
//         }
//         else if (m_eDrawType == MvCoordAxis::EDrawBySPt)
//         {
//             ptRB.x += mx;
//         }
        MVMAPPOINT4R_CHANGE_RS_X(ptRB,mx);
    }
    if (ntype == MvCoordSel::eMTPtLTLBM)
    {
        MVMAPPOINT4R_CHANGE_RS_X(ptLT,mx);
    }
    if (ntype == MvCoordSel::eMTPtLTRTM)
    {
        //ptLT.y += my;
        MVMAPPOINT4R_CHANGE_RS_Y(ptLT,my);
    }
    if (ntype == MvCoordSel::eMTPtLBRBM)
    {
        //ptRB.y += my;
        MVMAPPOINT4R_CHANGE_RS_Y(ptRB,my);
    }
    if (ntype == MvCoordSel::eMTPtLT)
    {
//         ptLT.x += mx;
//         ptLT.y += my;
        MVMAPPOINT4R_CHANGE_RS_X(ptLT,mx);
        MVMAPPOINT4R_CHANGE_RS_Y(ptLT,my);
    }
    if (ntype == MvCoordSel::eMTPtRB)
    {
//         ptRB.x += mx;
//         ptRB.y += my;
        MVMAPPOINT4R_CHANGE_RS_X(ptRB,mx);
        MVMAPPOINT4R_CHANGE_RS_Y(ptRB,my);
    }
    if (ntype == MvCoordSel::eMTPtRT)
    {
//         ptLT.y += my;
//         ptRB.x += mx;
        MVMAPPOINT4R_CHANGE_RS_Y(ptLT,my);
        MVMAPPOINT4R_CHANGE_RS_X(ptRB,mx);
    }
    if (ntype == MvCoordSel::eMTPtLB)
    {
//         ptLT.x += mx;
//         ptRB.y += my;
        MVMAPPOINT4R_CHANGE_RS_X(ptLT,mx);
        MVMAPPOINT4R_CHANGE_RS_Y(ptRB,my);
    }
    if (ntype == MvCoordSel::eMTPtAll)
    {
        if (m_eDrawType == MvCoordAxis::EDrawByRPt)
        {
            ptLT.x += MapView.R2SXLen(mx);
            ptLT.y -= MapView.R2SYLen(my);
            ptRB.x += MapView.R2SXLen(mx);
            ptRB.y -= MapView.R2SYLen(my);
        }
        else
        {
            ptLT.x += mx;
            ptLT.y += my;
            ptRB.x += mx;
            ptRB.y += my;
        }

    }

    if (m_eDrawType == MvCoordAxis::EDrawByRPt)
    {
         m_sel.m_ptLT = MapView.R2S(ptLT);
         m_sel.m_ptRB = MapView.R2S(ptRB);
    }
    else
    {
        m_sel.m_ptLT = (ptLT);
        m_sel.m_ptRB = (ptRB);
    }
}


MvMapView* MapCoordRange::GetMapView()
{
    return &MapView;
}

void MapCoordRange::ChangePtType( EDrawType dtype )
{
    if (m_eDrawType != dtype)
    {
        if ((dtype == EDrawByRPt) && (m_eDrawType == EDrawBySPt))
        {
            ptLT = MapView.S2R(ptLT);
            ptRB = MapView.S2R(ptRB);
        }
        else if ((dtype == EDrawBySPt) && (m_eDrawType == EDrawByRPt))
        {
            ptLT = MapView.R2S(ptLT);
            ptRB = MapView.R2S(ptRB);
        }
        m_eDrawType = dtype;
    }
}

bool MapCoordRange::Calc()
{
    if (m_nCheckType == 0)
    {
        return false;
    }
    double dData[1000];
    for (int i = 0; i<  1000; i++)
    {
        dData[i] = MapView.m_MapPoint8B.pt8B[i].y;
    }

    MvMapPoint ptRPtLT = GetRPtLT();
    MvMapPoint ptRPtRB = GetRPtRB();
    double x1 = MvMapPoint8B::Change8XTo800(ptRPtLT.x);
    double x2 = MvMapPoint8B::Change8XTo800(ptRPtRB.x);

    int x3 = 0;
    double avgy = 0;

    if (m_nCheckType == 1)
    {
        //x3 = CLineLaser::GetProfileMaxVal(dData,x1,x2);
        m_ptResult.x = MvMapPoint8B::Change800XTo8(x3);
        m_ptResult.y = dData[x3];
    }
    else if (m_nCheckType == 2)
    {
        //x3 = CLineLaser::GetProfileMinVal(dData,x1,x2);
        m_ptResult.x = MvMapPoint8B::Change800XTo8(x3);
        m_ptResult.y = dData[x3];
    }    
    else if (m_nCheckType == 3)
    {
        //avgy = CLineLaser::GetProfileAverageVal(dData,x1,x2);
        m_ptResult.x = MvMapPoint8B::Change800XTo8((x1+x2)/2);
        m_ptResult.y = avgy;
    }    


    return true;
}

MvMapPoint MapCoordRange::GetSPtLT()
{
    MvMapPoint ptDLT;
    if (m_eDrawType == EDrawByRPt)
    {
        ptDLT = MapView.R2S(ptLT);
    }
    else
    {
        ptDLT = ptLT;
    }
    return ptDLT;
}

MvMapPoint MapCoordRange::GetSPtRB()
{
    MvMapPoint ptDRB;
    if (m_eDrawType == EDrawByRPt)
    {
        ptDRB = MapView.R2S(ptRB);
    }
    else
    {
        ptDRB = ptRB;
    }
    return ptDRB;
}

MvMapPoint MapCoordRange::GetRPtLT()
{
    MvMapPoint ptRLT;
    if (m_eDrawType == EDrawByRPt)
    {
        ptRLT = ptLT;
    }
    else
    {
        ptRLT = MapView.S2R(ptLT);
    }
    return ptRLT;
}

MvMapPoint MapCoordRange::GetRPtRB()
{
    MvMapPoint ptRRB;
    if (m_eDrawType == EDrawByRPt)
    {
        ptRRB = ptRB;
    }
    else
    {
        ptRRB = MapView.S2R(ptRB);
    }
    return ptRRB;
}