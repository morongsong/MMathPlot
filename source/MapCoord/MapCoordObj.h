/** 
   *@file         MapCoordObj.h
   *@brief        MapCoordObj.h
   *@author       jimsmorong
   *@date         2015/11/20 10:49:51
 */
#ifndef MAPCOORDOBJ_H___
#define MAPCOORDOBJ_H___
#pragma once

#include "MapCoordDrawUtil.h"

class MvMapView;

enum EMapCoordObjTpye
{

};

/// 元件基类
class MapCoordObj
{
public:
    enum EMapCoordObjTpye
    {
        EMapCoordNull,
        EMapCoordRange,
        EMapCoordLine,
    };
public:
    MapCoordObj(){}
    virtual ~MapCoordObj(){}
public:
    virtual void Draw(CDC* pDC) {}
    virtual void Move(double x,double y) {}
    virtual void Scale(double sx,double sy){}
    virtual void Rote(double r){}
    virtual void Resize(int ntype,int mx,int my){}
public:
    virtual void OnLButtonDown(UINT nFlags, CPoint point){}
    virtual void OnLButtonUp(UINT nFlags, CPoint point){}
    virtual void OnMouseMove(UINT nFlags, CPoint point){}
    virtual BOOL OnMouseWheel(UINT nFlags, SHORT zDelta,CPoint point){return TRUE;}
    virtual void OnRButtonDown(UINT nFlags, CPoint point){}
    virtual void OnRButtonUp(UINT nFlags, CPoint point){}
    virtual MvMapView* GetMapView(){return nullptr;}
    virtual void SetSelStatus(int nType){}
    virtual MvMapPoint GetMiddlePt(){ return MvMapPoint(0,0);}
    virtual EMapCoordObjTpye GetElementType(){return EMapCoordNull;}
public:
    enum EDrawType
    {
        EDrawByRPt,
        EDrawBySPt,
    };
    virtual EDrawType GetDrawPtType() { return EDrawBySPt;}
    virtual bool IsConFinish(){return false;}
};

#endif //MAPCOORDOBJ_H___
