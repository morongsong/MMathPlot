/** 
   *@file         MapCoordRange.h
   *@brief        MapCoordRange.h
   *@author       jimsmorong
   *@date         2015/11/20 10:42:24
 */
#ifndef MAPCOORDRANGE_H___
#define MAPCOORDRANGE_H___
#pragma once

#include <windows.h>
#include <StlLock.h>
#include <atlimage.h>
#include <vector>
#include "MapCoordLine2d.h"
#include "MapCoordDraw.h"
#include "MapCoordObj.h"


/// MapCoordRange
class MapCoordRange : public MapCoordObj
{
public:
    MapCoordRange(MvMapView& argMapView);
    MvMapView& MapView;
public:
    EDrawType m_eDrawType;
    MvMapPoint ptLT;
    MvMapPoint ptRB;
    MvCoordSel m_sel;
    int m_nStep;
    MvMapPoint m_ptResult;//it sure is real value
public:
    void Init();
    void Move(double x,double y);
    void Draw(CDC* pDC);
    void Rote(double r);
    void Scale(double sx,double sy);
public:
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    BOOL OnMouseWheel(UINT nFlags, SHORT zDelta,CPoint point){return TRUE;}
    void OnRButtonDown(UINT nFlags, CPoint point);
    void OnRButtonUp(UINT nFlags, CPoint point);
    virtual void Resize(int ntype,int mx,int my);
    virtual MvMapView* GetMapView(); 
    virtual void SetSelStatus(int nType)
    {
        m_sel.m_eSelectState = (MvCoordSel::ESelectState)nType;
    }
    virtual MvMapPoint GetMiddlePt()
    {
        return MvMapPoint((ptLT.x+ptRB.x)/2,(ptLT.y+ptRB.y)/2);
    }
    void ChangePtType(EDrawType dtype);
    virtual EDrawType GetDrawPtType() { return m_eDrawType;}
    virtual bool IsConFinish(){return m_nStep == 2;}
    virtual EMapCoordObjTpye GetElementType(){return EMapCoordRange;}

    MvMapPoint GetSPtLT();
    MvMapPoint GetSPtRB();

    MvMapPoint GetRPtLT();
    MvMapPoint GetRPtRB();

public:
    /// Check Laser Point Type, 0=null,1=hight,2=low,3=avg
    int m_nCheckType;

public:
    bool Calc();
};


#endif //MAPCOORDRANGE_H___
