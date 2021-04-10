#pragma once
#include "MyPe.h"

// ReLoc 对话框

class ReLoc : public CDialogEx
{
	DECLARE_DYNAMIC(ReLoc)

public:
	ReLoc(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ReLoc();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELOC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//提供PE接口
	MyPe* m_MyPe = nullptr;
	// 块显示
	CListCtrl m_BlockList;
	// 块项目
	CListCtrl m_ItemList;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//填充信息
	void GetReLocInfo();
	//填充块项目信息
	void GetItemInfo(DWORD dwIndex);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickBlockList(NMHDR *pNMHDR, LRESULT *pResult);
};
