/** 
   *@file         MapCoordDrawUtil.h
   *@brief        MapCoordDrawUtil.h
 */
#ifndef MAPCOORDDRAWUTIL_H___
#define MAPCOORDDRAWUTIL_H___
#pragma once

#include <windows.h>
#include <StlLock.h>
#include <atlimage.h>
#include <vector>
#include "MapCoordLine2d.h"

/// MvMapPoint
class MvMapPoint
{
public:
    MvMapPoint(){x=y=0;}
    MvMapPoint(const CPoint& pt){x=pt.x;y=pt.y;}
    MvMapPoint(double ax,double ay){x=ax;y=ay;}
public:
    double x,y;
public:
    void Scale(double sx,double sy) {x *= sx; y*= sy;}
    void Move(double mx,double my){x+=mx;y+=my;}
    void Rote(double r);
    void Rote(double x,double y,double r);
public:
    operator CPoint() const {return CPoint((int)x, (int)y);}
};

/// MapCoordDrawUtil
class MapCoordDrawUtil
{
public:
    ///构造函数
    MapCoordDrawUtil();
    ///析构函数 
    virtual ~MapCoordDrawUtil();
public:
    static bool IsPointNear(MvMapPoint& pt1,MvMapPoint& pt2,int nNear = 3);
    static bool IsPtNearPt(CPoint point,CPoint point2,int nNear/*=3*/);
    static bool IsPointNearRectEdge(MvMapPoint& ptLT,MvMapPoint& ptRB,MvMapPoint& point,int nNear = 3);
public:
    static void DrawRectByPt(CDC* pDC,CPoint& pt,int nsize);
    static void DrawRectByPt(CDC* pDC,MvMapPoint& pt,int nsize);
    static void DrawRectByPtFill(CDC* pDC,CPoint& pt,int nsize);
    static void DrawRectByPtFill(CDC* pDC,MvMapPoint& pt,int nsize);
    static void DrawRectCross(CDC* pDC,CPoint& pt,int nsize);
    static void DrawRectCrossOnly(CDC* pDC,COLORREF clr,MvMapPoint& pt,int nsize);
    static void DrawLine(CDC* pDC,int nPenType,COLORREF clr,MvMapPoint& pt1,MvMapPoint& pt2);
protected:
private:
};

#endif //MAPCOORDDRAWUTIL_H___

