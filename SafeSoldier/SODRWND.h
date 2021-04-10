#pragma once
#include "Md5.h"
#include "ServerPort.h"
#include <vector>
// SODRWND 对话框

class SODRWND : public CDialogEx
{
	DECLARE_DYNAMIC(SODRWND)

public:
	SODRWND(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SODRWND();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SODR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//提供客户端网络服务接口
	ServerPort m_ClientServer;
	// 待删除文件筐
	CListCtrl m_SoDrList;
	// 需查杀的文件路径
	CEdit m_FilePath;
	// 白名单文件
	std::vector<CString> vecWhite;
	// 疑是病毒的文件集合
	std::vector<CString> vecString;
	//储存文件路径
	std::vector<CString> m_vecFilePath;
	//存储病毒的MD5集合
	std::vector<CString> m_vecVirusMD5;
	//已查明病毒数量
	int m_VirusBaseNum = 0;
	//菜单
	CMenu m_Menu;
	// 查询文件时 显示
	CEdit m_Traversal;

	afx_msg void OnBnClickedMd5Button();
	afx_msg void OnBnClickedChoicefileButton();
	//文件遍历 查找相同MD5值文件并输出
	void FindVirus();
	//遍历获取文件
	void EnumFile(CString FilePath);

	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedGoodButton();
	afx_msg void OnNMRClickSodrList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnWhitelist();
	afx_msg void OnBnClickedVirusButton();

	afx_msg void OnDelWhiteList();
};
//病毒文件查找
DWORD WINAPI FindVirusProc(LPVOID lpThreadParameter);
//服务器消息回调
DWORD CALLBACK recvProc(LPVOID arg);
//分割
std::vector<CString> SplitCString(CString strSource, char ch);
//病毒文件删除
DWORD WINAPI DelVirusProc(LPVOID lpThreadParameter);