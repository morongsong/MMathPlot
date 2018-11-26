
/** 
   *@file         MapCoordLine.cpp
   *@brief        MapCoordLine.cpp
   *@author       jimsmorong
   *@date         2015/11/20 11:07:23
*/
#include "stdafx.h"
#include "MapCoordDraw.h"
#include "MapCoordDrawUtil.h"
#include "MapCoordSel.h"
#include "MapCoordRange.h"
#include "MapCoordLine.h"

MapCoordLine::MapCoordLine( MvMapView& argMapView ) : MapView(argMapView)
{
    m_nStep = 0;
    m_ref1 = nullptr;
    m_ref2 = nullptr;
    m_sel.m_eSelectType = MvCoordSel::eSelectTypeLine;
    m_sel.m_pRefHoldObj = this;
}

void MapCoordLine::OnLButtonDown( UINT nFlags, CPoint point )
{
    if (m_nStep == 0)
    {
//         ptLT = point;
//         ptRB = point;
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
        CheckRefObj(m_ref1,MvMapPoint(point));
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
        CheckRefObj(m_ref2,MvMapPoint(point));
        m_nStep++;
    }  

    if (m_nStep == 2)
    {
//         m_sel.m_ptLT = ptLT;
//         m_sel.m_ptRB = ptRB;
        MapView.m_pSelElemt = nullptr;
        m_sel.OnLButtonDown(nFlags,point);
    }
}

void MapCoordLine::Draw( CDC* pDC )
{
    CPen pen;
    pen.CreatePen(PS_SOLID,1,RGB(255,255,0));
    CPen* pOldPen = pDC->SelectObject(&pen);




    MvMapPoint ptDLT;
    MvMapPoint ptDRB;

    /*
    (0)LT
                 RB(2)
    */
    if (m_eDrawType == EDrawByRPt)
    {
        ptDLT = MapView.R2S(ptLT);
        ptDRB = MapView.R2S(ptRB);
    }
    else
    {
        ptDLT = ptLT;
        ptDRB = ptRB;
    }

    if (!MapView.IsExist(m_ref1))
    {
        m_ref1 = nullptr;
    }
    if (!m_ref1)
    {
        CheckRefObj(m_ref1,m_sel.m_ptLT);
    }
    if (m_ref1)
    {
        if (m_ref1->GetDrawPtType() == EDrawType::EDrawBySPt)
        {
            ptDLT = m_ref1->GetMiddlePt();
        }
        else if (m_ref1->GetDrawPtType() == EDrawType::EDrawByRPt)
        {
            ptDLT = MapView.R2S(m_ref1->GetMiddlePt());
        }
    }
    

    if (!MapView.IsExist(m_ref2))
    {
        m_ref2 = nullptr;
    }
    if (!m_ref2)
    {
        CheckRefObj(m_ref2,m_sel.m_ptRB);
    }
    if (m_ref2)
    {
        //ptRB = m_ref2->GetMiddlePt();
        if (m_ref2->GetDrawPtType() == EDrawType::EDrawBySPt)
        {
            ptDRB = m_ref2->GetMiddlePt();
        }
        else if (m_ref2->GetDrawPtType() == EDrawType::EDrawByRPt)
        {
            ptDRB = MapView.R2S(m_ref2->GetMiddlePt());
        }
    }
    if (m_ref1 && m_ref2)
    {
    }
    else
    {
        pDC->MoveTo(ptDLT.x,ptDLT.y);
        pDC->LineTo(ptDRB.x,ptDRB.y);
    }

    pDC->SelectObject(&pOldPen);



    if (m_ref1)
    {
        MapCoordDrawUtil::DrawRectByPtFill(pDC,ptDLT,3);
        if (GetDrawPtType() == EDrawType::EDrawBySPt)
        {
            ptLT = ptDLT;
        }
        else if (GetDrawPtType() == EDrawType::EDrawByRPt)
        {
            ptLT = MapView.S2R(ptDLT);
        }
    }

    if (m_ref2)
    {
        MapCoordDrawUtil::DrawRectByPtFill(pDC,ptDRB,3);
        if (GetDrawPtType() == EDrawType::EDrawBySPt)
        {
            ptRB = ptDRB;
        }
        else if (GetDrawPtType() == EDrawType::EDrawByRPt)
        {
            ptRB = MapView.S2R(ptDRB);
        }
    }

    if (m_ref1 && m_ref2)
    {
        MapCoordRange* pR1 = (MapCoordRange*)m_ref1;
        MapCoordRange* pR2 = (MapCoordRange*)m_ref2;
        if ((pR1->m_nCheckType != 0) && (pR2->m_nCheckType != 0))
        {
            MvMapPoint ptDR1 = MapView.R2S(pR1->m_ptResult);
            MvMapPoint ptDR2 = MapView.R2S(pR2->m_ptResult);
            MvMapPoint ptDR12Ct((ptDR1.x+ptDR2.x)/2, (ptDR1.y+ptDR2.y)/2);

            MapCoordDrawUtil::DrawLine(pDC,PS_DASH,RGB(255,0,0),ptDR1,MvMapPoint(ptDR12Ct.x,ptDR1.y));
            MapCoordDrawUtil::DrawLine(pDC,PS_SOLID,RGB(255,0,0),MvMapPoint(ptDR12Ct.x-5,ptDR1.y),MvMapPoint(ptDR12Ct.x+5,ptDR1.y));

            MapCoordDrawUtil::DrawLine(pDC,PS_DASH,RGB(255,0,0),ptDR2,MvMapPoint(ptDR12Ct.x,ptDR2.y));
            MapCoordDrawUtil::DrawLine(pDC,PS_SOLID,RGB(255,0,0),MvMapPoint(ptDR12Ct.x-5,ptDR2.y),MvMapPoint(ptDR12Ct.x+5,ptDR2.y));

            MapCoordDrawUtil::DrawLine(pDC,PS_DASH,RGB(255,0,0),MvMapPoint(ptDR12Ct.x,ptDR1.y),MvMapPoint(ptDR12Ct.x,ptDR2.y));

            CString strVal;
            strVal.Format(_T("H:%.4f"), fabs(pR1->m_ptResult.y - pR2->m_ptResult.y));
            pDC->TextOut(ptDR12Ct.x,ptDR12Ct.y,strVal);
        }
    }

    if (m_nStep == 2)
    {
        if (GetDrawPtType() == EDrawType::EDrawBySPt)
        {
            m_sel.m_ptLT = ptDLT;
            m_sel.m_ptRB = ptDRB;
        }
        else if (GetDrawPtType() == EDrawType::EDrawByRPt)
        {
            m_sel.m_ptLT = ptDLT;
            m_sel.m_ptRB = ptDRB;
        }
        if (m_ref1 && m_ref2 && (m_nStep == 2))
        {
        }
        else
        {
            m_sel.Draw(pDC);
        }
        
    }
}

void MapCoordLine::CheckRefObj( MapCoordObj*& m_refObj,MvMapPoint& ptS )
{
    for (int i = 0,nSize = MapView.m_objs.size(); i < nSize; i++)
    {
        MapCoordObj* pObj = MapView.m_objs.at(i);
        if (pObj == this)
        {
            continue;;
        }
        MvMapPoint ptCenterS;
        if (pObj->GetDrawPtType() == EDrawType::EDrawBySPt)
        {
            ptCenterS = pObj->GetMiddlePt();
        }
        else if (pObj->GetDrawPtType() == EDrawType::EDrawByRPt)
        {
            ptCenterS = MapView.R2S(pObj->GetMiddlePt());
        }
        if ((ptCenterS.x != 0) && (ptCenterS.y != 0))
        {
            if (MapCoordDrawUtil::IsPointNear(ptCenterS,ptS,8))
            {
                m_refObj = pObj;
            }
        }
    }
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

void MapCoordLine::Resize( int ntype,int mx,int my )
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
    if (!m_ref1)
    {
        CheckRefObj(m_ref1,m_sel.m_ptLT);
    }
    if (!m_ref2)
    {
        CheckRefObj(m_ref2,m_sel.m_ptRB);
    }
}

void MapCoordLine::OnLButtonUp( UINT nFlags, CPoint point )
{
    if (m_nStep == 2)
    {
        m_sel.OnLButtonUp(nFlags,point);
    }
}

void MapCoordLine::OnMouseMove( UINT nFlags, CPoint point )
{
    if (m_nStep == 1)
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
        //CheckRefObj(m_ref2,MvMapPoint(point));
    }

    if (m_nStep == 2)
    {       
        m_sel.OnMouseMove(nFlags,point);
    }
}

void MapCoordLine::SetSelStatus( int nType )
{
    m_sel.m_eSelectState = (MvCoordSel::ESelectState)nType;
}



void MapCoordLine::ChangePtType( EDrawType dtype )
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
    return;
}
