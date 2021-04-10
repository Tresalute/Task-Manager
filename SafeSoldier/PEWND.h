#pragma once
#include < AtlBase.h>
#include < stdlib.h>
#include "MyPe.h"
#include "SECTION.h"
#include "DataDir.h"

// PEWND 对话框

class PEWND : public CDialogEx
{
	DECLARE_DYNAMIC(PEWND)

public:
	PEWND(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PEWND();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//目录信息窗口
	DataDir* m_MyDataDir = nullptr;
	//区段信息窗口
	SECTION* m_MySection = nullptr;
	// 文件名
	TCHAR m_FPath[_MAX_PATH];
	//PE类指针
	MyPe* m_MyPe = nullptr;
	// 入口点
	CEdit m_lianew;
	// OEP
	CEdit m_OEP;
	// ImageBase
	CEdit m_ImageBase;
	// SectionAlignment
	CEdit m_SectionAlig;
	// FileAlignment
	CEdit m_FileAlig;
	// SizeOfImage
	CEdit m_SizeOfImage;
	// SizeOfHeader
	CEdit m_SizeOfHeader;
	// NumOfSection
	CEdit m_NumOfSection;
	// SizeofOptionalHeader
	CEdit m_SizeofOpHerader;
	// Characteristics
	CEdit m_Character;
	virtual BOOL OnInitDialog();
	void SetInfomation();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSectionButton();
	afx_msg void OnBnClickedDirdataButton();
};
