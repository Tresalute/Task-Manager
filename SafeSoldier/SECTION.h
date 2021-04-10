#pragma once
#include <stdlib.h>
#include "MyPe.h"

// SECTION 对话框

class SECTION : public CDialogEx
{
	DECLARE_DYNAMIC(SECTION)

public:
	SECTION(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SECTION();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//提供PE接口
	MyPe* m_MyPe = nullptr;
	//文件路径
	WCHAR* m_FPath = nullptr;
	// 区段LIst
	CListCtrl m_SectionList;
	virtual BOOL OnInitDialog();
	void GetSectionInfo();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
