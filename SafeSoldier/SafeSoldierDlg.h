
// SafeSoldierDlg.h: 头文件
//

#pragma once
#include "PEWND.h"
#include "PROCESSWND.h"
#include "SERVERWND.h"
#include "SODRWND.h"
#include "CLEARWND.h"
#include <powrprof.h>
#pragma commend(lib,"PowrProf.lib");

#define WNDNUM 5
#define KeyID 1001

// CSafeSoldierDlg 对话框
class CSafeSoldierDlg : public CDialogEx
{
// 构造
public:
	CSafeSoldierDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAFESOLDIER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CStatusBar m_StatusBar;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//菜单
	CMenu m_Menu;
	// 主窗口Table控件
	CTabCtrl m_MyTable;
	//子窗口数组
	CDialogEx* TabWnd[WNDNUM] = { 0 };
	//窗口属性
	CRect m_Rect;
	//选择显示Table窗口
	void ShowTabWnd(size_t index);
	//改变大小
	void ChangeSize(CWnd * pWnd, int cx, int cy);
	//Table控件选项
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	//改变大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//设置状态栏 内存信息
	void SetStatusBar();
	//获取CPU信息
	int GetCpuUsage();
	//时间转换
	double FileTime2Double(const _FILETIME& fileTime);
	//以管理员权限启动程序
	void AdminReStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnShutdown();
	afx_msg void OnRestar();
	afx_msg void OnDormany();
	afx_msg void OnProtecte();
};
