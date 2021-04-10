#pragma once
#include "MyPe.h"

// ExportTable 对话框

class ExportTable : public CDialogEx
{
	DECLARE_DYNAMIC(ExportTable)

public:
	ExportTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ExportTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//提供PE处理接口
	MyPe* m_MyPe = nullptr;
	// 输出表偏移
	CEdit m_Offset;
	// 特征值
	CEdit m_Trait;
	// 序号基址
	CEdit m_ImageBase;
	// 名称
	CEdit m_Name;
	// 名称字符串
	CEdit m_NameStr;
	// 函数数量
	CEdit m_NumOfFun;
	// 函数名数量
	CEdit m_NumOfFunName;
	// 函数地址表
	CEdit m_FunAddress;
	// 函数名地址表
	CEdit m_FunNameAdd;
	// 函数名序号地址表
	CEdit m_FunNameIndexAdd;
	// List输出框
	CListCtrl m_ExportList;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//获取并填充输出表信息
	void GetExportTableInfo();
	virtual BOOL OnInitDialog();
};
