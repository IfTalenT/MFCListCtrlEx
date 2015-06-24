
// MFCListCtrlExDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "NewListCtrl.h"
#include "afxwin.h"

// CMFCListCtrlExDlg 对话框
class CMFCListCtrlExDlg : public CDialogEx
{
// 构造
public:
	CMFCListCtrlExDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCLISTCTRLEX_DIALOG };

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
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonStyle1();
	afx_msg void OnBnClickedButtonStyle2();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonNoHeader();
	afx_msg void OnBnClickedButtonSetCheck();
	afx_msg void OnBnClickedButtonSetSortState();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEnChangeEdit4List();
	afx_msg void OnBnClickedButtonSortColumn();

	//处理ListCtrl消息
	afx_msg LRESULT OnListShowBuddyEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListShowBuddyComboBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListBuddyEditChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListBuddyComboBoxChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListHeaderCheckChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListCheckChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListColumnClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListSelectedRowChanged(WPARAM wParam, LPARAM lParam);

    afx_msg void OnListMenu1();
    afx_msg void OnListMenu2();

	DECLARE_MESSAGE_MAP()

	void DebugText(CString& cstrText);
private:
	CNewListCtrl m_listCtrl;
	CBuddyEdit m_edit;
	CBuddyComboBox m_comboBox;
	CEdit m_editOutput;
	int m_nRowHeight;
	int m_nHeaderHeight;
public:
    afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
};

static int CALLBACK SortColumn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
