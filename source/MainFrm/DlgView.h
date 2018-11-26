#pragma once
#include "UICtrlEx\ResizeCtrl.h"
#include "MapCoord\MapCoordShowWnd.h"


// CDlgView 对话框

class CDlgView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgView)

public:
	CDlgView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    CResizeCtrl m_oResizeCtrl;
public:
    virtual BOOL OnInitDialog();
    LRESULT OnNcHitTest(CPoint point);
    void OnSysCommand(UINT nID, LPARAM lParam);
    CEdit m_edtSpt;
    CEdit m_edtSpt2;
    MapCoordShowWnd m_stcMMPlot;
    afx_msg void OnBnClickedBtnRun();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
    void StrToPots(CString& strExp,double x0,double x1, MvMapPoint* pts);

    afx_msg void OnTimer(UINT nIDEvent);

    int m_nT;
};
