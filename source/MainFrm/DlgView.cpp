// DlgView.cpp : 实现文件
//

#include "stdafx.h"
#include "mmp.h"
#include "DlgView.h"
#include "afxdialogex.h"
#include "resource.h"
#include <lua/lua.hpp>
#include "xFrame/xApp.h"

// CDlgView 对话框

IMPLEMENT_DYNAMIC(CDlgView, CDialogEx)

CDlgView::CDlgView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW, pParent)
{
    g_xApp.m_pDlgView = this;
    m_nT = 0;
}

CDlgView::~CDlgView()
{
}

void CDlgView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_MMPOLT, m_stcMMPlot);
    DDX_Control(pDX, IDC_EDIT1, m_edtSpt);
    DDX_Control(pDX, IDC_EDIT2, m_edtSpt2);
}


BEGIN_MESSAGE_MAP(CDlgView, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_NCHITTEST()
    ON_BN_CLICKED(IDC_BTN_RUN, &CDlgView::OnBnClickedBtnRun)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgView 消息处理程序


BOOL CDlgView::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_oResizeCtrl.Create(GetSafeHwnd());

    CRect rct;
    GetClientRect(&rct);
    m_oResizeCtrl.SetInitialSize(rct.Size());
    m_oResizeCtrl.SetMinimumTrackingSize();

    CResizeInfo oResizeInfo[] =
    {
        { IDC_STATIC_LINE, 100, 0, 0, 100 },
        { IDC_STATIC_MMPOLT, 0, 0, 100, 100 },
        { IDC_EDIT1, 100, 0, 0, 0 },
        { IDC_EDIT2, 100, 0, 0, 0 },
        { IDC_BTN_RUN, 100, 0, 0, 0 },
        { IDC_STATIC_T, 100, 0, 0, 0 },
    };
    m_oResizeCtrl.Add(oResizeInfo);

    m_stcMMPlot.Init();
    m_nT = 0;
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


LRESULT  CDlgView::OnNcHitTest(CPoint point)
{
    int ret = CDialog::OnNcHitTest(point);

    //if语句的前两行是用来禁止改变大小的，最后一行是用来禁止移动的

    if (HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret || HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret || HTTOPRIGHT == ret || HTCAPTION == ret)

        return HTCLIENT;

    return 0;
}


void  CDlgView::OnSysCommand(UINT nID, LPARAM lParam)
{
    if (nID == SC_MOVE || nID == 0xF012)

        return ;

    else

         CDialog::OnSysCommand(nID, lParam);
}


#define err_exit(num,fmt,...)  \
    do{printf("[%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__);exit(num);} while(0)
#define err_return(num,fmt,...)  \
    do{printf("[%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__);return(num);} while(0)


////lua->stack，得到全局数组，位置-1
//lua_getglobal(L, "global_c_read_array");
//
////得到数组长度
//lua_Integer array_len = luaL_len(L, -1);
//for (lua_Integer i = 1; i <= array_len; ++i) {
//    //lua->stack,全局数组位置-1,然后i为对应的索引值，将返回值放到-1的位置
//    int ret_type = lua_rawgeti(L, -1, i);
//
//    //--------------被替代的原操作table函数-------------
//    //lua_pushinteger(L, i);
//    //int ret_type = lua_gettable(L, -2); //也可以使用int ret_type = lua_rawget(L, -2);    
//
//    //stack->c
//    if (ret_type == LUA_TNUMBER) {
//        if (lua_isinteger(L, -1)) {
//            printf("%lld\n", lua_tointeger(L, -1));
//        }
//        else if (lua_isnumber(L, -1)) {
//            printf("%g\n", lua_tonumber(L, -1));
//        }
//    }
//    else if (ret_type == LUA_TSTRING) {
//        printf("%s\n", lua_tostring(L, -1));
//    }
//
//    lua_pop(L, 1);
//}

void CDlgView::OnBnClickedBtnRun()
{
    static bool bStart = false;
    if (!bStart)
    {
        bStart = true;
        SetTimer(1, 10, 0);
        GetDlgItem(IDC_BTN_RUN)->SetWindowText(L"Stop");
    }
    else
    {
        bStart = false;
        GetDlgItem(IDC_BTN_RUN)->SetWindowText(L"Run");
        KillTimer(1);
    }

    return;
}


BOOL CDlgView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && ((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE)))
	{
        if (pMsg->hwnd == m_hWnd)
        {
            return TRUE;
        }
		//return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgView::StrToPots(CString& strExp, double x0, double x1, MvMapPoint* pts)
{

    CString strExpT = strExp;
    //m_nT = 0;

    CString xx;
    xx.Format(_T("function calcy(x) \n %s \n return y \n end"), strExp);

    CStringA xxa = CT2A(xx);

    double dSx = x0;
    double dSx2 = x1;

    CStringA strErrA;
    lua_State * L = luaL_newstate();        //创建lua运行环境
    if (L == NULL)
    {
        //err_return(-1, "luaL_newstat() failed");
    }
    luaL_openlibs(L);

    int ret = 0;
    ret = luaL_loadstring(L, xxa);      //加载lua脚本文件
    if (ret != 0)
    {
        strErrA.Format("lua_pcall failed:%s", lua_tostring(L, -1));
        //err_return(-1, "luaL_loadfile failed");
    }
    int t = m_nT;

    lua_pushnumber(L, t);
    lua_setglobal(L, "t");

    ret = lua_pcall(L, 0, 0, 0);
    if (ret != 0)
    {
        strErrA.Format("lua_pcall failed:%s", lua_tostring(L, -1));
        //err_return(-1, "lua_pcall failed:%s", lua_tostring(l, -1));
    }

    double dScale = (dSx2 - dSx) / 1000;
    for (int i = 0; i < 1000; i++)
    {
        double x = dSx + dScale*i;
        //double x = 12;
        //调用lua中的函数sum
        lua_getglobal(L, "calcy");
        lua_pushnumber(L, x);
        ret = lua_pcall(L, 1, 1, 0);
        if (ret != 0)
        {
            strErrA.Format("lua_pcall failed:%s", lua_tostring(L, -1));
            //err_return(-1, "lua_pcall failed:%s", lua_tostring(l, -1));
        }
        //printf("sum:%d + %d = %ld\n", a, b, lua_tointeger(l, -1));
        double y = lua_tonumber(L, -1);
        lua_pop(L, 1);

        pts[i].x = x;
        pts[i].y = y;
    }

    lua_close(L);
}

void CDlgView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        KillTimer(1);

        m_nT++;

        CString strT;
        strT.Format(_T("t 表示 开始执行计数 当前t=%d"), m_nT);
        GetDlgItem(IDC_STATIC_T)->SetWindowText(strT);
        

        CString strText, strText2;
        m_edtSpt.GetWindowText(strText);
        m_edtSpt2.GetWindowText(strText2);

        CRect rcClient;
        m_stcMMPlot.GetClientRect(rcClient);

        //CString xx = _T("function calcy(x) \n y = x \n return y \n end");
        //CStringA xxa = CT2A(xx);
        CString xx;
        xx.Format(_T("function calcy(x) \n %s \n return y \n end"), strText);

        //y = math.cos(x / 20) * 100 + 200
        //y = x

        CStringA xxa = CT2A(xx);

        double dSx = m_stcMMPlot.m_pMvMapView->S2RX(-10);
        double dSx2 = m_stcMMPlot.m_pMvMapView->S2RX(rcClient.Width() + 10);

        if (strText.GetLength())
        {
            m_stcMMPlot.m_pMvMapView->m_MapPoint8B.m_nDrawFlag = MvMapPoint8B::eDrawLines;
            StrToPots(strText, dSx, dSx2, m_stcMMPlot.m_pMvMapView->m_MapPoint8B.pt8B);
        }
        else
        {
            m_stcMMPlot.m_pMvMapView->m_MapPoint8B.m_nDrawFlag = MvMapPoint8B::eDrawNull;
        }

        if (strText2.GetLength())
        {
            m_stcMMPlot.m_pMvMapView->m_MapPoint8B2.m_nDrawFlag = MvMapPoint8B::eDrawLines;

            m_stcMMPlot.m_pMvMapView->m_MapPoint8B2.m_DrawClr = RGB(20, 10, 250);
            StrToPots(strText2, dSx, dSx2, m_stcMMPlot.m_pMvMapView->m_MapPoint8B2.pt8B);
        }
        else
        {
            m_stcMMPlot.m_pMvMapView->m_MapPoint8B2.m_nDrawFlag = MvMapPoint8B::eDrawNull;
        }
        m_stcMMPlot.RedrawWindow();

        SetTimer(1, 10, 0);
    }

    return ;
}
