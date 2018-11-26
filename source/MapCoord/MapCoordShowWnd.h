#pragma once
#include "MapCoordDraw.h"


// MapCoordShowWnd

class MapCoordShowWnd : public CWnd
{
	DECLARE_DYNAMIC(MapCoordShowWnd)

public:
	MapCoordShowWnd();
	virtual ~MapCoordShowWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
    MvMapView* m_pMvMapView;
    
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    void Init();
};


