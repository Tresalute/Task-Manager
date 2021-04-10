#pragma once
#include <vector>
// CLEARWND 对话框

class CLEARWND : public CDialogEx
{
	DECLARE_DYNAMIC(CLEARWND)

public:
	CLEARWND(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLEARWND();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEAR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 待清除垃圾文件LIST
	CListCtrl m_FPathList;
	// In_FPath EDIT
	CEdit m_In_FPath;
	// FSuf EDIT
	CEdit m_FSuf;
	// 删除后缀
	CString m_Suffix;
	// 获取需要删除的文件
	CButton m_GetDelFile;
	// 删除文件
	CButton m_DelFile;
	// 需删除文件信息
	CListCtrl m_DelFileInfo;
	//需删除文件名集合
	std::vector<CString> m_DelFilePath;
	//遍历文件 获取需要删除的文件
	void EnumFile(CString FilePath);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGetfileButton();
	afx_msg void OnBnClickedDelfileButton();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCleartrashButton();
	afx_msg void OnBnClickedClearmemButton();
};
//清理回调
DWORD WINAPI ClearProc(LPVOID lpThreadParameter);
DWORD WINAPI Cleartrash(LPVOID lpThreadParameter);