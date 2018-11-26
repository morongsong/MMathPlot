/**
   *@file         picshowctrl.hpp
   *@brief        图片显示控件
   *@author       jimsmorong
   *@date         2014/11/17 21:14:11
 */
#ifndef PICSHOWCTRL_HPP_
#define PICSHOWCTRL_HPP_
#pragma once

#include <atlimage.h>
#include <vector>
#include <afxwin.h>

#define IDM_PICSHOW_FITIMG   22330             ///< 适应窗口
#define IDM_PICSHOW_SRCIMG   22331             ///< 原始大小
#define IDM_PICSHOW_BIGIMG   22332             ///< 放大图像
#define IDM_PICSHOW_SMALLIMG 22333             ///< 缩小图像
#define IDM_PICSHOW_FITIMG_WITH_SCALE   22334  ///< 适应窗口(等比例)

/// 显示图片控件
class CPicShowCtrl  : public CScrollView
{
public:
    DECLARE_DYNAMIC(CPicShowCtrl)
    CPicShowCtrl();
    virtual ~CPicShowCtrl();
    static BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);
    static BOOL m_bIsRegistered;

public:
    void OpenFileDlg2LoadImg();
    void LoadImg(LPCTSTR pszFile);
    void DoInit();
    void DoFitWndImg();
    void DoFitWndImgWithScale();
    void DoSrcImg();
    void DoBigImg();
    void DoSmallImg();
    CImage& GetImg()
    {
        return m_img;
    }
public:
    void DrawElp(RECT& rc);
    void DrawRect(RECT& rc);
public:
    double GetScaleX(){return m_dScaleX;}
    double GetScaleY(){return m_dScaleY;}
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
protected:
    virtual void OnDraw(CDC* pDC);
    virtual void PostNcDestroy();
    virtual void OnInitialUpdate();
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()
private:
    CImage m_img;
    double m_dScaleX;
    double m_dScaleY;
    CMenu m_Menu;
    void GetImageXY(CPoint ptMouse,CPoint& ptImg);
    void GetImageXYAt(double dScollX, double dScollY, CPoint ptMouse, double& dImgX, double& dImgY);
private:
    CBrush m_brHatch;
    CPoint m_RefScroll;
    CPoint m_RefPoint;
    CPoint m_MouseMovePos;
    enum EMouseState
    {
        eMouseNormal,
        eMouseScrollMove,
        eMouseEnd,
    };
    EMouseState m_eMouseState;
    bool m_bDraw;
    double m_nStartWidth;
    double m_nStartHeight;
public:
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif //PICSHOWCTRL_HPP_
