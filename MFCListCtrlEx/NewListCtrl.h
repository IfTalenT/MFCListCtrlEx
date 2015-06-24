#pragma once
#include "NewHeaderCtrl.h"
#include <map>
#include <vector>

struct ListNotifyData
{
	int nRow;
	int nCol;
	CString cstrText;
	CRect rcItem;
	CWnd* pWnd;

	ListNotifyData()
		: nRow(-1)
		, nCol(-1)
		, pWnd(NULL)
	{

	}
};

enum ColumnControlType
{
	ListColumnText,
	ListColumnEdit,
	ListColumnCheckBox, // 不要使用ListCtrl默认的LVS_EX_CHECKBOXES风格
	ListColumnComboBox,
};

class CBuddyEdit;
class CBuddyComboBox;

class CNewListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CNewListCtrl)

public:
	CNewListCtrl();
	virtual ~CNewListCtrl();

	//设置header背景，由listctrl拥有该bitmap
	void SetHeaderBackImage(HBITMAP hBmp);
	void SetHeaderBackColor(COLORREF cBack);
	void SetHeaderTextColor(COLORREF cHeaderText);
	void SetHeaderArrowColor(COLORREF cArrow);
	void SetOddRowBackColor(COLORREF cOdd);
	void SetEvenRowBackColor(COLORREF cEven);
	void SetSelectedRowBackColor(COLORREF cSelected);
	void SetVerticalLineColor(COLORREF cLine);
	void SetHorizontalLineColor(COLORREF cLine);
	//void SetSelectedRowBottomLineColor(COLORREF cLine);
	void SetColumSortType(int nColumn, ColumnSortType colSortType);
	void SetSortFun(PFNLVCOMPARE sortFun);
	void SetColumnControlType(int nColumn, ColumnControlType colCtrlType); // 目前仅支持一列为CheckBox
	void SetBuddyEdit(CBuddyEdit* pBuddyEdit);
	void SetBuddyComboBox(CBuddyComboBox* pBuddyCombo);
	void SetHeaderFont(CFont& font);
	void SetListFont(CFont& font); // 不要用CListCtrl::SetFont
	int GetSortedColumn();
	virtual BOOL SetTextColor(COLORREF cText);

	//以下函数必须listctrl创建成功才能调用,否则会挂
	void SetHeaderHeight(int nHeaderHeight); // 如果高度为0，相当于隐藏Header
	void SetRowHeight(int nRowHeight);
	int GetRowHeight(); //获取行高，和SetRowHeight的值一样
	int GetRealRowHeight(); //获取真正行高，可能和SetRowHeight的值不一样
	void SetItemImage(int nItem, HICON hIcon, BOOL bInvalidate = FALSE); //仅支持对第一列设置图标，CNewListCtrl不负责删除hIcon
	HICON GetItemImage(int nItem);
	virtual BOOL SetCheck(int nItem, BOOL bCheck = TRUE);
	virtual BOOL GetCheck(int nItem);
	virtual BOOL SetItemData(int nItem, DWORD_PTR dwData);
	virtual DWORD_PTR GetItemData(int nItem);
	virtual BOOL DeleteItem(int nItem);
	virtual BOOL DeleteAllItems();
	virtual CNewHeaderCtrl* GetHeaderCtrl();
	void SortColumn(int nColumn, ColumnSortState state);
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	DECLARE_MESSAGE_MAP()

	void Init();
	void OnSelectedRowChanged(int nRow);
	virtual void PreSubclassWindow();
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawSubItem(CDC *pDC, int nItem, int nSubItem, CRect& rSubItem, bool bSelected, bool bFocus);
	virtual void DrawRemainSpace(LPNMLVCUSTOMDRAW lpnmcd);
	virtual void draw_row_bg(CDC *pDC, CRect& rect, bool bSelected, bool bFocus, int nRow, int nCol);
	virtual void draw_row_bg(CDC *pDC, CRect& rect, bool bSelected, bool bFocus, bool bOdd, int nCol);
	//void InvalidateItem(int nItem);

	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnEndTrack(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnDividerDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	struct NItemData
	{
		BOOL bCheck;
		HICON hIcon;
		DWORD_PTR dwData;
		NItemData()
			: bCheck(FALSE)
			, hIcon(NULL)
			, dwData(NULL)
		{
		}
	};
	CNewHeaderCtrl m_header;
	CFont m_font;
	int m_nRowHeight;
	int m_nRealRowHeight;
	CBrush m_brushOddBack;
	CBrush m_brushEvenBack;
	CBrush m_brushSelectedBack;
	CPen m_penVerticalLine;
	CPen m_penHorizontalLine;
	//CPen m_penSelectedRowBottomLine;
	int m_nSortColumn;
	ColumnSortState m_sortState;
	PFNLVCOMPARE m_sortFun;
	std::map<int, ColumnSortType> m_mapColumnSortType;
	std::map<int, ColumnControlType> m_mapColumnControlType;
	std::map<int, DWORD> m_mapColumnAlign;
	CBuddyEdit* m_pBuddyEdit;
	CBuddyComboBox* m_pBuddyComboBox;
	int m_nSelectedRow;
	std::vector<NItemData*> m_vecNItemData;
	COLORREF m_colorText;
};

class CBuddyComboBox : public CComboBox
{
public:
	friend class CNewListCtrl;
	CBuddyComboBox();
	int GetStringIndex(CString& cstrText);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();

	DECLARE_MESSAGE_MAP()
private:
	int m_nRow;
	int m_nCol;
	BOOL m_bESC;
	CNewListCtrl* m_pList;
	CString m_cstrInit;
};

class CBuddyEdit : public CEdit
{
public:
	friend class CNewListCtrl;
	CBuddyEdit();
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnCloseup();

	DECLARE_MESSAGE_MAP()
private:
	int m_nRow;
	int m_nCol;
	BOOL m_bESC;
	CNewListCtrl* m_pList;
	CString m_cstrInit;
};

