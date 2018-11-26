/** 
   *@file         MapCoordSel.h
   *@brief        MapCoordSel.h
   *@author       jimsmorong
   *@date         2015/11/20 09:48:32
 */
#ifndef MAPCOORDSEL_H___
#define MAPCOORDSEL_H___
#pragma once

class MapCoordObj;
class CDC;
/// MapCoordSel
class MvCoordSel
{
public:
    MvCoordSel();
public:
    MvMapPoint m_ptLT;
    MvMapPoint m_ptRB;
    MvMapPoint ptMoveRef;
public:
    void Draw(CDC* pDC);
public:
    enum ESelectType
    {
        eSelectTypeRange,
        eSelectTypeLine,
    };
    ESelectType m_eSelectType;
public:
    enum ESelectState
    {
        eSelNull,
        eSelCross,
    };
    ESelectState m_eSelectState;
    MapCoordObj* m_pRefHoldObj;
    bool m_bHoldLeftMouse;

    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    BOOL OnMouseWheel(UINT nFlags, SHORT zDelta,CPoint point);
    void OnRButtonDown(UINT nFlags, CPoint point);
    void OnRButtonUp(UINT nFlags, CPoint point);
public:
    enum EMoveType
    {
        eMTPtNULL,
        eMTPtLT,
        eMTPtLTLBM,
        eMTPtLB,
        eMTPtRT,
        eMTPtRTRBM,
        eMTPtRB,
        eMTPtLTRTM,
        eMTPtLBRBM,
        eMTPtAll,
    };
    EMoveType m_eMoveType;
    EMoveType CheckMoveType(MvMapPoint& ptCur);
    void SetMoveTypeCursor(EMoveType eMoveType);
};

#endif //MAPCOORDSEL_H___
