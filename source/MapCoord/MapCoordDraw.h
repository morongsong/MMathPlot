/** 
   *@file         MapCoordDraw.h
   *@brief        MapCoordDraw.h
 */
#ifndef MAPCOORDDRAW_H___
#define MAPCOORDDRAW_H___
#pragma once


#include <windows.h>
#include <StlLock.h>
#include <atlimage.h>
#include <vector>
#include "MapCoordLine2d.h"
#include "MapCoordDrawUtil.h"
#include "MapCoordSel.h"
#include "MapCoordRange.h"
#include "MapCoordObj.h"
#include "MapCoordLine.h"


//////////////////////////////////////////////////////////////////////////////////////

class MvMapPoint8B : public MapCoordObj
{
public:
	MvMapPoint8B(MvMapView& argMapView);
	MvMapView& MapView;
public:
	MvMapPoint pt8B[1000];
public:
	void Move(double x,double y);
	void Draw(CDC* pDC);
	void Rote(double r);
	void Scale(double sx,double sy);
public:
    enum eDrawFlag
    {
        eDrawNull,
        eDrawPots,
        eDrawLines
    };
    COLORREF m_DrawClr;
    eDrawFlag m_nDrawFlag;
    static double Change800XTo8(int iVal) {return (iVal - 400) * 0.02;}
    static int Change8XTo800(double dVal) {return (int)(dVal*50+400);}
};


//////////////////////////////////////////////////////////////////////////////////////
/// MvCoordAxis
class MvCoordAxis : public MapCoordObj
{
public:
	MvCoordAxis(MvMapView& argMapView): MapView(argMapView) {}
	MvMapView& MapView;
	~MvCoordAxis() {}
public:
	MvMapPoint ptDrawOrg;
	double dAngle;
    enum EDrawType
    {
        EDrawByRPt,
        EDrawBySPt,
    };
    EDrawType m_eDrawType;
public:
	void Move(double x,double y);
	void Draw(CDC* pDC);
	void Rote(double r);
	void Scale(double sx,double sy);
private:
	void DrawAxis(CDC* pDC);
	void DrawKD(CDC* pDC);
};


//////////////////////////////////////////////////////////////////////////////////////
/// MvMapView
class MvMapView
{
public:
    MvMapView();
    ~MvMapView();
public:
	void MoveSOrg(int x,int y);
	void ScaleSRView( int sx,int sy );
	void Rote(double r);
    void Draw();
    CImage& GetImg(){return m_oImage;}
public:
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point) ;
    void OnMouseMove(UINT nFlags, CPoint point) ;
    BOOL OnMouseWheel(UINT nFlags, SHORT zDelta,CPoint point);
    void OnRButtonDown(UINT nFlags, CPoint point);
    void OnRButtonUp(UINT nFlags, CPoint point);
    CPoint m_ptRefLBOrg;
    bool m_bHoldLeftMouse;
public:
    void ClearObjs();
    void EraseObj(MapCoordObj* pobj);
public:
    void SetSelObj(MapCoordObj* pobj);

    bool IsExist(MapCoordObj* pobj);

    bool ClearNoFinsh();

    int GetRangCount();
    /// Native Count Which That Count 1 is 1;
    MapCoordRange* GetRangPtr(int nIndex);

    int GetObjCount() {return m_objs.size();}
public:
    MvCoordAxis m_CoordAxis;
    MvMapPoint8B m_MapPoint8B;
    MvMapPoint8B m_MapPoint8B2;
public:
    MapCoordObj* m_pSelElemt;
    MapCoordObj* m_pNeedDelElemt;
    CWnd* m_pHoldWnd;
public:
    /// 屏幕宽
    int m_nSWidth;

    /// 屏幕高
    int m_nSHeight;
    
    /// 世界中心原点在屏幕所在的位置X
    int m_nSOrgX;

    /// 世界中心原点在屏幕所在的位置Y
    int m_nSOrgY;

    /// 屏幕像素与实际值的比例 R=S*m_dScaleX;
    double m_dScaleX;

    /// 屏幕像素与实际值的比例 R=S*m_dScaleX;
    double m_dScaleY;
public:
	void R2SX(const double RX,double& SX);
	void R2SY(const double RY,double& SY);
	void S2RX(const double SX,double& RX);
	void S2RY(const double SY,double& RY);
	double R2SX(const double RX);
	double R2SY(const double RY);
	double S2RX(const double SX);
	double S2RY(const double SY);
public:
    MvMapPoint R2S(MvMapPoint& pt);
    MvMapPoint S2R(MvMapPoint& pt);
    MvMapPoint S2R(CPoint& pt);
public:
    void R2SXLen(const double RXLen,double& SX);
    void R2SYLen(const double RYLen,double& SY);
    double R2SXLen(const double RXLen);
    double R2SYLen(const double RYLen);
public:
	std::vector<MapCoordObj*> m_objs;
private:
    CImage m_oImage;
};



#endif //MAPCOORDDRAW_H___
