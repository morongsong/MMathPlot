/** 
   *@file         MapCoordLine.h
   *@brief        MapCoordLine.h
   *@author       jimsmorong
   *@date         2015/11/20 11:07:17
 */
#ifndef MAPCOORDLINE_H___
#define MAPCOORDLINE_H___
#pragma once

#include <windows.h>
#include <StlLock.h>
#include <atlimage.h>
#include <vector>
#include "MapCoordLine2d.h"
#include "MapCoordDraw.h"
#include "MapCoordObj.h"


/// MapCoordLine
class MapCoordLine : public MapCoordObj
{
public:
    MapCoordLine(MvMapView& argMapView);
    MvMapView& MapView;
public:
    EDrawType m_eDrawType;
    MvMapPoint ptLT;
    MvMapPoint ptRB;
    MvCoordSel m_sel;

    MapCoordObj* m_ref1;
    MapCoordObj* m_ref2;
public:
    virtual EDrawType GetDrawPtType() { return m_eDrawType;}
    void CheckRefObj(MapCoordObj*& m_refObj,MvMapPoint& ptS);
public:
    int m_nStep;
    void Init(){}
    virtual MvMapView* GetMapView(){return &MapView;}
    void Move(double x,double y){ptLT.Move(x,y),ptRB.Move(x,y);}
    void Draw(CDC* pDC);
    void Rote(double r){ptLT.Rote(r),ptRB.Rote(r);}
    void Scale(double sx,double sy){ptLT.Rote(sx),ptRB.Rote(sy);}
public:
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    virtual void Resize(int ntype,int mx,int my);
    virtual void SetSelStatus(int nType);
    void ChangePtType(EDrawType dtype);
    virtual bool IsConFinish(){return m_nStep == 2;}
    virtual EMapCoordObjTpye GetElementType(){return EMapCoordLine;}
public:
    void Calc()
    {

    }
};

#endif //MAPCOORDLINE_H___
