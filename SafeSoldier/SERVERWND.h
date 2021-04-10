#pragma once


// SERVERWND 对话框

class SERVERWND : public CDialogEx
{
	DECLARE_DYNAMIC(SERVERWND)

public:
	SERVERWND(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SERVERWND();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//菜单
	CMenu m_Menu;
	// Serveer List
	CListCtrl m_SerVerList;
	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStartServer();
	afx_msg void OnSuspendserver();
	afx_msg void OnStopserver();
	afx_msg void OnNMRClickServerList(NMHDR *pNMHDR, LRESULT *pResult);
	//获取服务信息
	void SerVerControl( );
	afx_msg void OnRefreshserver();
};
