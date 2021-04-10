#pragma once
#include <vector>
#include <TlHelp32.h>



// ThreadAndMoudle 对话框

class ThreadAndMoudle : public CDialogEx
{
	DECLARE_DYNAMIC(ThreadAndMoudle)

public:
	ThreadAndMoudle(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ThreadAndMoudle();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TANDM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

	//CListCtrl m_ThreadList;
	//CListCtrl m_ModuleList;

	std::vector<THREADENTRY32> m_ThreadInfo;
	std::vector<MODULEENTRY32> m_ModuleInfo;
	typedef struct PROCESSINFO
	{
		std::vector<THREADENTRY32> _ThreadInfo;
		std::vector<MODULEENTRY32> _ModuleInfo;
	}ProcessInfo, *pProcessInfo;
	pProcessInfo ProInfo;

	void PrtThreadInfo();
	void PrtModuleInfo();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	// 线程LIST
	CListCtrl m_ThreadList;
	// 模块LIST	// 模块LIST
	CListCtrl m_ModuleList;
};
