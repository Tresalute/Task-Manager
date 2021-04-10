
// ProtectProcessDlg.h: 头文件
//

#pragma once


// CProtectProcessDlg 对话框
class CProtectProcessDlg : public CDialogEx
{
// 构造
public:
	CProtectProcessDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROTECTPROCESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 输入框
	CEdit m_Out;
	afx_msg void OnBnClickedProtectButton();
	//提升为DeBug权限
	BOOL EnableDeBugPrivilege();
	//远程线程注入
	BOOL InjectDLL(DWORD dwProcessID, CHAR* dllpath);
	//获取需要注入程序的PID 
	DWORD GetPID(WCHAR* name);
	//创建内存共享并写入PID 
	BOOL CreaetMap(CString PID);

};
