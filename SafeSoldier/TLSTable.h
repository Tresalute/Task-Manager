#pragma once
#include "MyPe.h"

// TLSTable 对话框

class TLSTable : public CDialogEx
{
	DECLARE_DYNAMIC(TLSTable)

public:
	TLSTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TLSTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 提供PE 接口
	MyPe* m_MyPe = nullptr;
	// StartVA
	CEdit m_StartVA;
	// EndVA
	CEdit m_EndVA;
	// m_IndexVA
	CEdit m_IndexVA;
	// CallBack
	CEdit m_CallBack;
	// Zero
	CEdit m_Zero;
	// Trait
	CEdit m_Trait;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//填充信息
	void GetTLSInfo();
};
