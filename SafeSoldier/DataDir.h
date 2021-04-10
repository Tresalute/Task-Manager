#pragma once
#include "MyPe.h"
#include "ExportTable.h"
#include "ImportTable.h"
#include "ResourceTable.h"
#include "ReLoc.h"
#include "TLSTable.h"



// DataDir 对话框

class DataDir : public CDialogEx
{
	DECLARE_DYNAMIC(DataDir)

public:
	DataDir(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DataDir();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATADIR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//TLS窗口信息
	TLSTable* m_MyTLS = nullptr;
	//重定位窗口信息
	ReLoc* m_MyReLoc = nullptr;
	//资源表窗口信息
	ResourceTable* m_MyResourceTable = nullptr;
	//导入表窗口信息
	ImportTable* m_MyImportTable = nullptr;
	//导出表窗口对象
	ExportTable* m_MyExportTable = nullptr;
	//填充信息
	void GetDataInfo();
	//提供PE接口
	MyPe* m_MyPe = nullptr;
	//EXPORT
	CEdit m_ExportRva;
	CEdit m_ExportSize;
	//IMPORT
	CEdit m_ImportRva;
	CEdit m_ImportSize;
	//RESOURCE
	CEdit m_ResourceRva;
	CEdit m_ResourceSize;
	//EXCEPTION
	CEdit m_ExcepRva;
	CEdit m_ExcepSize;
	//SECURITY
	CEdit m_SecurityRva;
	CEdit m_SecuritySize;
	//BASERELOC
	CEdit m_BaseReLocRva;
	CEdit m_BaseRelocSize;
	//DEBUG
	CEdit m_DebugRva;
	CEdit m_DebugSize;
	//ARCHITECTURE
	CEdit m_ArchiRva;
	CEdit m_ArchiSize;
	//CLOBALPTR
	CEdit m_GlobalPtrRva;
	CEdit m_GlobalPtrSize;
	//TLS
	CEdit m_TLSRva;
	CEdit m_TLSSize;
	//LOAD
	CEdit m_LoadRva;
	CEdit m_LoadSize;
	//BOUND
	CEdit m_BoundRva;
	CEdit m_BoundSize;
	//IAT
	CEdit m_IATRva;
	CEdit m_IATSize;
	//DELAU
	CEdit m_DelauRva;
	CEdit m_DelauSize;
	//COM
	CEdit m_ComRva;
	CEdit m_ComSize;
	//OTHER
	CEdit m_OtherRva;
	CEdit m_OtherSize;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedExportButton();
	afx_msg void OnBnClickedImportButton2();
	afx_msg void OnBnClickedResourceButton();
	afx_msg void OnBnClickedRelocButton();
	afx_msg void OnBnClickedTlsButton();
	afx_msg void OnBnClickedDelayButton();
};
