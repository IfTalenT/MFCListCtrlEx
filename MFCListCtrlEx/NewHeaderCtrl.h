#pragma once

enum ListNotifyMsg
{
	WM_LIST_SHOW_BUDDY_EDIT = WM_APP+1985,
	WM_LIST_SHOW_BUDDY_COMBOBOX,
	WM_LIST_HEADER_CHECK_CHANGED,
	WM_LIST_COLUMN_CLICK,
	WM_LIST_CLICK,
	WM_LIST_DOUBLE_CLICK,
	WM_LIST_SELECT_CHANGED,
	WM_LIST_CHECK_CHANGED,
	WM_LIST_BUDDY_COMBOBOX_CHANGED,
	WM_LIST_BUDDY_EDIT_CHANGED,
};

enum ColumnSortState
{
	StateNotSort,
	StateAsc,
	StateDesc,
};

enum ColumnSortType
{
	NotSort,			//不支持排序
	SortByInt,			//将ItemText转为int值排序
	SortByDouble,		//将ItemText转为double值排序
	SortByString,		//用ItemText排序,区分大小写
	SortByStringNoCase,	//用ItemText排序,不区分大小写
	SortByItemData,		//用ItemData排序,需要使用者提供排序比较函数
};

// CNewHeaderCtrl

class CNewHeaderCtrl : public CHeaderCtrl
{
	friend class CNewListCtrl;
	DECLARE_DYNAMIC(CNewHeaderCtrl)

public:
	CNewHeaderCtrl();
	virtual ~CNewHeaderCtrl();

	void SetItemSortState(int nColumn, ColumnSortState sortState);
	ColumnSortState GetItemSortState(int nColumn);
	void SetHeaderHeight(int nHeight);
	int GetHeaderHeight();
	void SetHeaderFont(CFont& font);
	void SetVerticalLineColor(COLORREF cLine);
	void SetHeaderImage(HBITMAP hBmp);
	void SetHeaderBackColor(COLORREF cBack);
	void SetTextColor(COLORREF cText);
	void SetArrowColor(COLORREF cArrow);
	void SetCheck(BOOL bCheck);
	BOOL GetCheck();

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnLayout(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPaint();
	//afx_msg void OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DoPaint(CDC *pDC);
	virtual void PreSubclassWindow();

	void Init();
private:
	int m_nHeaderHeight;
	int m_nCheckColumn;
	BOOL m_bAllCheck;
	CFont m_font;
	CPen m_penVerticalLine;
	CImage m_imgHeader;
	COLORREF m_colorText;
	CPen m_penArrow;
	CBrush m_brushBack;
};

