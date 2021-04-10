#pragma once
#include <Psapi.h>

// WNDEUM 对话框

class WNDEUM : public CDialogEx
{
	DECLARE_DYNAMIC(WNDEUM)

public:
	WNDEUM(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WNDEUM();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WNDEMU_DIALOG	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void OutPutWndList();

	DECLARE_MESSAGE_MAP()
public:
	//WNDLIST
	CListCtrl m_WndList;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

BOOL CALLBACK WndEnumProc(HWND, LPARAM);
void GetProcessFilePath(HANDLE hProcess, CString& sFilePath);
