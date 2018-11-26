
/** 
   *@file         MapCoordSel.cpp
   *@brief        MapCoordSel.cpp
   *@author       jimsmorong
   *@date         2015/11/20 09:48:41
*/
#include "stdafx.h"
#include "MapCoordDrawUtil.h"
#include "MapCoordSel.h"
#include "MapCoordDraw.h"

MvCoordSel::MvCoordSel()
{
    m_pRefHoldObj = nullptr;
    m_eSelectState = eSelNull;
    m_bHoldLeftMouse = false;
    m_eMoveType = eMTPtNULL;
    m_eSelectType = eSelectTypeRange;
}

void MvCoordSel::Draw(CDC* pDC)
{
    if (m_eSelectType == eSelectTypeRange)
    {
        if (m_eSelectState == eSelCross)
        {
            CPen pen;
            pen.CreatePen(PS_SOLID,1,RGB(0,255,0));
            CPen* pOldPen = pDC->SelectObject(&pen);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptLT.x,m_ptLT.y),3);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptRB.x,m_ptLT.y),3);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptRB.x,m_ptRB.y),3);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptLT.x,m_ptRB.y),3);

            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint((m_ptLT.x+m_ptRB.x)/2,m_ptLT.y),3);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint((m_ptLT.x+m_ptRB.x)/2,m_ptRB.y),3);

            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptLT.x,(m_ptLT.y+m_ptRB.y)/2),3);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptRB.x,(m_ptLT.y+m_ptRB.y)/2),3);

            //int nRF = max(min(abs((m_ptLT.x-m_ptRB.x)/4),abs((m_ptLT.y-m_ptRB.y)/4)),8);
            CPoint ptCt((m_ptLT.x+m_ptRB.x)/2,(m_ptLT.y+m_ptRB.y)/2);
            MapCoordDrawUtil::DrawRectByPt(pDC,ptCt,8);

            MvMapPoint ptClose(m_ptRB.x-10,m_ptLT.y-8);
            MapCoordDrawUtil::DrawRectCross(pDC,CPoint(ptClose.x,ptClose.y),6);

            pDC->SelectObject(&pOldPen);
        }
    }
    else if (m_eSelectType == eSelectTypeLine)
    {
        if (m_eSelectState == eSelCross)
        {
            CPen pen;
            pen.CreatePen(PS_SOLID,1,RGB(0,255,0));
            CPen* pOldPen = pDC->SelectObject(&pen);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptLT.x,m_ptLT.y),3);
            MapCoordDrawUtil::DrawRectByPt(pDC,CPoint(m_ptRB.x,m_ptRB.y),3);

            MvMapPoint ptCenter((m_ptLT.x+m_ptRB.x)/2,(m_ptLT.y+m_ptRB.y)/2);
            MapCoordDrawUtil::DrawRectByPt(pDC,ptCenter,5);

            MvMapPoint ptClose((m_ptRB.x+ptCenter.x)/2,(m_ptRB.y+ptCenter.y)/2);
            MapCoordDrawUtil::DrawRectCross(pDC,CPoint(ptClose.x,ptClose.y),5);
            pDC->SelectObject(&pOldPen);
        }
    }


    return;
}

void MvCoordSel::OnLButtonDown(UINT nFlags, CPoint point)
{
    ptMoveRef.x = point.x;
    ptMoveRef.y = point.y;
    m_bHoldLeftMouse = true;

    if (m_eSelectState == eSelCross)
    {
        if (m_eMoveType == eMTPtNULL)
        {
            m_eMoveType = CheckMoveType(MvMapPoint(point));
            SetMoveTypeCursor(m_eMoveType);
        }

        m_pRefHoldObj->GetMapView()->m_pSelElemt = m_pRefHoldObj;

        MvMapPoint ptCur(point.x,point.y);
        if (m_eSelectType == eSelectTypeRange)
        {
            MvMapPoint ptClose(m_ptRB.x-10,m_ptLT.y-8);
            if (MapCoordDrawUtil::IsPointNear(ptClose,ptCur,6))
            {
                m_pRefHoldObj->GetMapView()->m_pNeedDelElemt = m_pRefHoldObj;
            }
        }
        else
        {
            MvMapPoint ptCenter((m_ptLT.x+m_ptRB.x)/2,(m_ptLT.y+m_ptRB.y)/2);
            MvMapPoint ptClose((m_ptRB.x+ptCenter.x)/2,(m_ptRB.y+ptCenter.y)/2);
            if (MapCoordDrawUtil::IsPointNear(ptClose,ptCur,5))
            {
                m_pRefHoldObj->GetMapView()->m_pNeedDelElemt = m_pRefHoldObj;
            }
        }

    }
}

void MvCoordSel::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bHoldLeftMouse = false;
    if (m_eSelectState == eSelCross)
    {
        m_eMoveType = eMTPtNULL;
        m_pRefHoldObj->GetMapView()->m_pSelElemt = nullptr;
    }
}

void MvCoordSel::OnRButtonDown(UINT nFlags, CPoint point)
{

}

void MvCoordSel::OnRButtonUp(UINT nFlags, CPoint point)
{
    m_bHoldLeftMouse = false;
    m_eMoveType = eMTPtNULL;
    m_eSelectState = eSelNull;
    m_pRefHoldObj->GetMapView()->m_pSelElemt = nullptr;
}


void MvCoordSel::OnMouseMove(UINT nFlags, CPoint point)
{
    MvMapPoint ptCur(point);
    bool bNearResult = false;

    if (m_eSelectType == eSelectTypeRange)
    {
        MvMapPoint ptCenter((m_ptLT.x+m_ptRB.x)/2,(m_ptLT.y+m_ptRB.y)/2);

        MvMapPoint ptClose(m_ptRB.x-10,m_ptLT.y-8);

        bool bNearSel1 = MapCoordDrawUtil::IsPointNearRectEdge(m_ptLT,m_ptRB,ptCur,3);

        bool bNearSel2 = MapCoordDrawUtil::IsPointNear(ptCenter,ptCur,8);

        bool bNearSel3 = MapCoordDrawUtil::IsPointNear(ptClose,ptCur,8);
        bNearResult = bNearSel1 | bNearSel2 | bNearSel3 ;
    }
    else if (m_eSelectType == eSelectTypeLine)
    {
        MvMapPoint ptCenter((m_ptLT.x+m_ptRB.x)/2,(m_ptLT.y+m_ptRB.y)/2);
        MvMapPoint ptClose((m_ptRB.x+ptCenter.x)/2,(m_ptRB.y+ptCenter.y)/2);

        MapCoordLine2d line;
        line.Init2Pt(m_ptLT.x,m_ptLT.y,m_ptRB.x,m_ptRB.y);
        bool bNearSel5 = (line.ptDisLine(ptCur.x,ptCur.y) < 3);
        //bool bNearSel5 = false;
        bool bNearSel1 = MapCoordDrawUtil::IsPointNear(m_ptLT,ptCur,3);
        bool bNearSel2 = MapCoordDrawUtil::IsPointNear(m_ptRB,ptCur,3);
        bool bNearSel3 = MapCoordDrawUtil::IsPointNear(ptCenter,ptCur,5);
        bool bNearSel4 = MapCoordDrawUtil::IsPointNear(ptClose,ptCur,5);
        bNearResult = bNearSel1 | bNearSel2 | bNearSel3 | bNearSel4 | bNearSel5;
    }
    //TRACE("m_eSelectState %d\n",m_eSelectState);
    if (m_eSelectState == eSelNull)
    {
        if (bNearResult)
        {
            m_eSelectState = eSelCross;
            m_pRefHoldObj->GetMapView()->m_pSelElemt = m_pRefHoldObj;
        }
    }
    else if (m_eSelectState == eSelCross)
    {
        if ((!bNearResult) && (!m_bHoldLeftMouse))
        {
            m_eSelectState = eSelNull;
             m_pRefHoldObj->GetMapView()->m_pSelElemt = nullptr;
        }
        else
        {
            if ( (!m_bHoldLeftMouse) || (m_eMoveType == eMTPtNULL))
            {
                m_eMoveType = CheckMoveType(MvMapPoint(point));
                SetMoveTypeCursor(m_eMoveType);
            }
            else if ((m_bHoldLeftMouse) && (m_eMoveType != eMTPtNULL))
            {
                SetMoveTypeCursor(m_eMoveType);
                int mx = point.x - ptMoveRef.x;
                int my = point.y - ptMoveRef.y;
                if (m_eMoveType != eMTPtNULL)
                {
                    m_pRefHoldObj->GetMapView()->m_pSelElemt = m_pRefHoldObj;
                    m_pRefHoldObj->Resize(m_eMoveType,mx,my);
                }
            }   
        }
    }
    ptMoveRef.x = point.x;
    ptMoveRef.y = point.y;
}

MvCoordSel::EMoveType MvCoordSel::CheckMoveType(MvMapPoint& ptCur)
{
    EMoveType eMoveTy = eMTPtNULL;
    //if (m_eSelectType == eSelectTypeRange)
    {
        if (MapCoordDrawUtil::IsPointNear(m_ptLT,ptCur,3))
        {
            eMoveTy = eMTPtLT;      
        }
        if (MapCoordDrawUtil::IsPointNear(m_ptRB,ptCur,3))
        {
            eMoveTy = eMTPtRB;
        }

        MvMapPoint ptRT(m_ptRB.x,m_ptLT.y);
        if (MapCoordDrawUtil::IsPointNear(ptRT,ptCur,3))
        {
            eMoveTy = eMTPtRT;
        }
        MvMapPoint ptLB(m_ptLT.x,m_ptRB.y);
        if (MapCoordDrawUtil::IsPointNear(ptLB,ptCur,3))
        {
            eMoveTy = eMTPtLB;
        }

        MvMapPoint ptLTLBM(m_ptLT.x,(m_ptLT.y+m_ptRB.y)/2);
        if (MapCoordDrawUtil::IsPointNear(ptLTLBM,ptCur,3) )
        {
            eMoveTy = eMTPtLTLBM;
        }

        MvMapPoint ptRTRBM(m_ptRB.x,(m_ptLT.y+m_ptRB.y)/2);
        if (MapCoordDrawUtil::IsPointNear(ptRTRBM,ptCur,3))
        {
            eMoveTy = eMTPtRTRBM;
        }

        MvMapPoint ptLTRTM((m_ptLT.x+m_ptRB.x)/2,m_ptLT.y);

        if (MapCoordDrawUtil::IsPointNear(ptLTRTM,ptCur,3))
        {
            eMoveTy = eMTPtLTRTM;
        }

        MvMapPoint ptLBRBM((m_ptLT.x+m_ptRB.x)/2,m_ptRB.y);
        if (MapCoordDrawUtil::IsPointNear(ptLBRBM,ptCur,3))
        {
            eMoveTy = eMTPtLBRBM;
        }
        MvMapPoint ptCt((m_ptLT.x+m_ptRB.x)/2,(m_ptLT.y+m_ptRB.y)/2);
        int nRF = max(min(abs((m_ptLT.x-m_ptRB.x)/4),abs((m_ptLT.y-m_ptRB.y)/4)),8);
        if (MapCoordDrawUtil::IsPointNear(ptCt,ptCur,nRF))
        {
            eMoveTy = eMTPtAll;
        }
    }


    return eMoveTy; 
}

void  MvCoordSel::SetMoveTypeCursor(MvCoordSel::EMoveType eMoveType)
{
    EMoveType eMoveTy = eMoveType;
    if(eMoveTy == eMTPtLT)      
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));

    if(eMoveTy == eMTPtRB)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));

    if(eMoveTy == eMTPtRT)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));

    if(eMoveTy == eMTPtLB)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));

    if(eMoveTy == eMTPtLTLBM)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

    if(eMoveTy == eMTPtRTRBM)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

    if(eMoveTy == eMTPtLTRTM)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));

    if(eMoveTy == eMTPtLBRBM)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));

    if(eMoveTy == eMTPtAll)
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
    return;
}

BOOL MvCoordSel::OnMouseWheel(UINT nFlags, SHORT zDelta,CPoint point)
{
    return TRUE;
}