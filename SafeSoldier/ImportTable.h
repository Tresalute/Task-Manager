#pragma once
#include "MyPe.h"

// ImportTable 对话框

class ImportTable : public CDialogEx
{
	DECLARE_DYNAMIC(ImportTable)

public:
	ImportTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ImportTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 提供PE接口
	MyPe* m_MyPe = nullptr;
	// 导出表DLL信息
	CListCtrl m_ImDllLidt;
	// 导入表DLL函数信息
	CListCtrl m_ImFunInfo;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	// 填充导入表DLL信息
	void GetImPortDllInfo();
	// 填充导入表FUN信息
	void GetImPortFunInfo(DWORD Thunk);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkImdllinfoList(NMHDR *pNMHDR, LRESULT *pResult);
};

