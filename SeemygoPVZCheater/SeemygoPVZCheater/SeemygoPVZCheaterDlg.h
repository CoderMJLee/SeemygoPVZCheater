
// SeemygoPVZCheaterDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CSeemygoPVZCheaterDlg 对话框
class CSeemygoPVZCheaterDlg : public CDialogEx
{
	friend DWORD WINAPI MonitoringThreadProc(LPVOID);
// 构造
public:
	CSeemygoPVZCheaterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEEMYGOPVZCHEATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	HCURSOR  m_copyrightCursor;
	RECT  m_copyrightRect;
private:
	afx_msg void OnBnClickedCoin();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedUnbreakable();
	afx_msg void OnBnClickedKill();
	afx_msg void OnBnClickedBackground();
	afx_msg void OnBnClickedCd();
	afx_msg void OnBnClickedZoombieUnbreakable();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCourse();
	CButton m_sunshine;
	CButton m_unbreakable;
	CButton m_coin;
	CButton m_cancel;
	CButton m_kill;
	CButton m_background;
	CButton m_zoombieUnbreakable;
	CButton m_cd;
	CStatic m_copyright;
};
