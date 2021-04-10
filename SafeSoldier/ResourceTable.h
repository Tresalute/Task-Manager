#pragma once
#include "MyPe.h"

// ResourceTable 对话框

class ResourceTable : public CDialogEx
{
	DECLARE_DYNAMIC(ResourceTable)

public:
	ResourceTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ResourceTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESOURCE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//提供PE接口
	MyPe * m_MyPe = nullptr;
	// Tree
	CTreeCtrl m_ResouceTree;
	// 根目录 条目名称
	CEdit m_RootName;
	// 根目录 ID条目
	CEdit m_RootID;
	// 选中的条目的名称
	CEdit m_ChoiceName;
	// 选中的条目的ID
	CEdit m_ChoiceID;
	// 选中的条目的RVA
	CEdit m_ChoiceRVA;
	// 选中的项目偏移
	CEdit m_ChoiceOffset;
	// 选中的项目的大小
	CEdit m_ChoiceSize;
	//
	char resourceType[0x10][0x10] = {
		"鼠标指针",
		"位图",
		"图标",
		"菜单",
		"对话框",
		"字符串列表",
		"字符目录",
		"字体",
		"快捷键",
		"非格式化资源",
		"消息列表",
		"鼠标指针数组",
		"图标组",
		"版本信息"
	};
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//填充信息
	void GetResourceInfo(DWORD dwResourceTableRva, HTREEITEM treeRoot = (HTREEITEM)4294901760U);
	void GetResourceInfo();
};
//WCHAR* DwToWchar(DWORD dwNum,int radix);