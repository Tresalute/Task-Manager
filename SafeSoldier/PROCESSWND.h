#pragma once
#include "ThreadAndMoudle.h"
#include "WNDEUM.h"
#include <TlHelp32.h>
#include <vector>

// PROCESSWND 对话框

class PROCESSWND : public CDialogEx
{
	DECLARE_DYNAMIC(PROCESSWND)

public:
	PROCESSWND(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PROCESSWND();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	// 进程线程List控件
	CListCtrl m_PrsList;
	//菜单
	CMenu m_Menu;
	//窗口属性 通用
	CRect m_Rect;
	//线程模块窗口
	ThreadAndMoudle m_TAndM;
	//窗口枚举信息
	WNDEUM m_WndEum;
	//快照
	std::vector<PROCESSENTRY32> nowPe32;
	std::vector<MODULEENTRY32> ModuleInfo;
	std::vector<THREADENTRY32> ThreadInfo;
	//模块窗口结构体
	typedef struct PROCESSINFO
	{
		std::vector<THREADENTRY32> _ThreadInfo;
		std::vector<MODULEENTRY32> _ModuleInfo;
	}ProcessInfo, *pProcessInfo;
	virtual BOOL OnInitDialog();
	//获取进程消息
	void GetProcessInfo();
	//线程消息
	int PrtThreadInfo(DWORD swPid);
	//模块消息
	void PrtModuleInfo(DWORD swPid);
	//弹出菜单
	afx_msg void OnNMRClickPrsList(NMHDR *pNMHDR, LRESULT *pResult);
	//结束线程
	afx_msg void OnProcessKill_1();
	//挂起进程
	afx_msg void OnSuspendThread_1();
	//恢复进程
	afx_msg void ResumeThread_1();
	//改变窗口大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//双击弹出模块窗口
	afx_msg void OnNMDblclkPrsList(NMHDR *pNMHDR, LRESULT *pResult);
	//定时更新进程窗口
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnWndEum();
	afx_msg void OnProtect();
};
//判断是否为有效线程
bool IsJudProcee(std::vector<PROCESSENTRY32> pVec32, DWORD pid);