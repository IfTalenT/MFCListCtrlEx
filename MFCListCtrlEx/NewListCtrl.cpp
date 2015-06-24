// NewListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "NewListCtrl.h"

IMPLEMENT_DYNAMIC(CNewListCtrl, CListCtrl)

CNewListCtrl::CNewListCtrl()
	: m_nRowHeight(1)
	, m_nRealRowHeight(1)
	, m_nSortColumn(-1)
	, m_sortState(StateNotSort)
	, m_sortFun(NULL)
	, m_pBuddyEdit(NULL)
	, m_pBuddyComboBox(NULL)
	, m_nSelectedRow(-1)
	, m_colorText(RGB(0,0,0))
{
    m_brushOddBack.DeleteObject();
	m_brushOddBack.CreateSolidBrush(RGB(250, 250, 250));
    m_brushEvenBack.DeleteObject();
	m_brushEvenBack.CreateSolidBrush(RGB(243, 243, 243));
    m_brushSelectedBack.DeleteObject();
	m_brushSelectedBack.CreateSolidBrush(RGB(203, 223, 239));
    m_penVerticalLine.DeleteObject();
	m_penVerticalLine.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    m_penHorizontalLine.DeleteObject();
	m_penHorizontalLine.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	//m_penSelectedRowBottomLine.CreatePen(PS_SOLID, 1, RGB(255,255,255));
}

CNewListCtrl::~CNewListCtrl()
{
	for (auto it=m_vecNItemData.begin(); it!=m_vecNItemData.end(); ++it)
	{
		delete *it;
	}
	m_vecNItemData.clear();
}

BEGIN_MESSAGE_MAP(CNewListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CNewListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CNewListCtrl::OnLvnItemchanged)
	ON_NOTIFY(HDN_ITEMCHANGEDA, 0, &CNewListCtrl::OnHdnItemchanged)
	ON_NOTIFY(HDN_ITEMCHANGEDW, 0, &CNewListCtrl::OnHdnItemchanged)
    ON_NOTIFY(HDN_ENDTRACK, 0, &CNewListCtrl::OnHdnEndTrack)
    ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, &CNewListCtrl::OnHdnDividerDblClick)
	ON_NOTIFY_REFLECT(LVN_ENDSCROLL, &CNewListCtrl::OnLvnEndScroll)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CNewListCtrl::OnLvnColumnclick)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()

void CNewListCtrl::SetListFont(CFont& font)
{
	LOGFONT logFont;
	font.GetLogFont(&logFont);
    m_font.DeleteObject();
	m_font.CreateFontIndirect(&logFont);
}

void CNewListCtrl::SetHeaderFont(CFont& font)
{
	m_header.SetHeaderFont(font);
}

void CNewListCtrl::SetHeaderHeight(int nHeaderHeight)
{
	if (nHeaderHeight < 0)
	{
		nHeaderHeight = 0;
	}
	m_header.SetHeaderHeight(nHeaderHeight);
	SetRowHeight(m_nRowHeight); //测试发现，对RowHeight改变才会触发header改变，否则无效果
}

void CNewListCtrl::SetHeaderBackImage(HBITMAP hBmp)
{
	m_header.SetHeaderImage(hBmp);
}

void CNewListCtrl::SetHeaderBackColor(COLORREF cBack)
{
	m_header.SetHeaderBackColor(cBack);
}

void CNewListCtrl::SetHeaderTextColor(COLORREF cHeaderText)
{
	m_header.SetTextColor(cHeaderText);
}

void CNewListCtrl::SetHeaderArrowColor(COLORREF cArrow)
{
	m_header.SetArrowColor(cArrow);
}

void CNewListCtrl::SetRowHeight(int nRowHeight)
{
	if (nRowHeight < 10)
	{
		nRowHeight = 10;
	}
	m_nRowHeight = nRowHeight;
	m_nRealRowHeight = nRowHeight;

	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfWeight = FW_NORMAL;
	logfont.lfCharSet = GB2312_CHARSET;
	_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, TEXT("宋体"));
	logfont.lfHeight = -(m_nRowHeight-4);
	CFont font;
	font.CreateFontIndirect(&logfont);
	SetFont(&font);//HeaderControl会收到HDM_LAYOUT消息
	font.Detach();
}

void CNewListCtrl::SetOddRowBackColor(COLORREF cOdd)
{
	m_brushOddBack.DeleteObject();
	m_brushOddBack.CreateSolidBrush(cOdd);
}

void CNewListCtrl::SetEvenRowBackColor(COLORREF cEven)
{
	m_brushEvenBack.DeleteObject();
	m_brushEvenBack.CreateSolidBrush(cEven);
}

void CNewListCtrl::SetSelectedRowBackColor(COLORREF cSelected)
{
	m_brushSelectedBack.DeleteObject();
	m_brushSelectedBack.CreateSolidBrush(cSelected);
}

void CNewListCtrl::SetVerticalLineColor(COLORREF cLine)
{
	m_penVerticalLine.DeleteObject();
	m_penVerticalLine.CreatePen(PS_SOLID, 1, cLine);
	m_header.SetVerticalLineColor(cLine);
}

void CNewListCtrl::SetHorizontalLineColor(COLORREF cLine)
{
	m_penHorizontalLine.DeleteObject();
	m_penHorizontalLine.CreatePen(PS_SOLID, 1, cLine);
}

//void CNewListCtrl::SetSelectedRowBottomLineColor(COLORREF cLine)
//{
//	m_penSelectedRowBottomLine.DeleteObject();
//	m_penSelectedRowBottomLine.CreatePen(PS_SOLID, 1, cLine);
//}

void CNewListCtrl::SetColumSortType(int nColumn, ColumnSortType colSortType)
{
	if (nColumn >= 0)
	{
		m_mapColumnSortType[nColumn] = colSortType;
	}
}

int CNewListCtrl::GetSortedColumn()
{
	return m_nSortColumn;
}

BOOL CNewListCtrl::SetTextColor(COLORREF cText)
{
	m_colorText = cText;
	return TRUE;
}

void CNewListCtrl::SetSortFun(PFNLVCOMPARE sortFun)
{
	m_sortFun = sortFun;
}

void CNewListCtrl::SetColumnControlType(int nColumn, ColumnControlType colCtrlType)
{
	if (nColumn >= 0)
	{
		m_mapColumnControlType[nColumn] = colCtrlType;
		if (colCtrlType == ListColumnCheckBox)
		{
			m_header.m_nCheckColumn = nColumn;
		}
	}
}

void CNewListCtrl::SetBuddyEdit(CBuddyEdit* pBuddyEdit)
{
	m_pBuddyEdit = pBuddyEdit;
	m_pBuddyEdit->m_pList = this;
	m_pBuddyEdit->SetParent(this);
	m_pBuddyEdit->ShowWindow(SW_HIDE);
}

void CNewListCtrl::SetBuddyComboBox(CBuddyComboBox* pBuddyCombo)
{
	m_pBuddyComboBox = pBuddyCombo;
	m_pBuddyComboBox->m_pList = this;
	m_pBuddyComboBox->SetParent(this);
	m_pBuddyComboBox->ShowWindow(SW_HIDE);
}

void CNewListCtrl::PreSubclassWindow()
{
	Init();
	CListCtrl::PreSubclassWindow();
	ModifyStyle(WS_BORDER, 0); //todo 没有效果，需要在资源里把Border设为False
	DWORD dwExtentStyle = GetExtendedStyle();
	SetExtendedStyle(dwExtentStyle|LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER);

	LOGFONT logFont;
	GetFont()->GetLogFont(&logFont);
    m_font.DeleteObject();
	m_font.CreateFontIndirect(&logFont);
	SetRowHeight(26);
}


void CNewListCtrl::Init()
{
	CHeaderCtrl *pHeaderCtrl = CListCtrl::GetHeaderCtrl();
	if (pHeaderCtrl!=NULL)
	{
		m_header.SubclassWindow(pHeaderCtrl->GetSafeHwnd());
		m_header.SetHeaderHeight(28);
	}
}

CNewHeaderCtrl* CNewListCtrl::GetHeaderCtrl()
{
	return &m_header;
}

void CNewListCtrl::OnSelectedRowChanged(int nRow)
{
	if (m_nSelectedRow != nRow)
	{
		m_nSelectedRow = nRow;
		::SendMessage(::GetParent(m_hWnd), WM_LIST_SELECT_CHANGED, m_nSelectedRow, NULL);
	}
}

void CNewListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lpnmcd = (LPNMLVCUSTOMDRAW) pNMHDR;
	if (lpnmcd ->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult =  CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		return;
	}
	else if (lpnmcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		CRect rSubItem, rectClient;
		int nColumnCount = GetHeaderCtrl()->GetItemCount();
		if (nColumnCount>0)
		{
			GetSubItemRect(lpnmcd->nmcd.dwItemSpec,nColumnCount-1, LVIR_LABEL,rSubItem);
			GetClientRect(&rectClient);
			rSubItem.left = rSubItem.right;
			rSubItem.right = rectClient.right;
			if (m_nRealRowHeight != rSubItem.Height())
			{
				m_nRealRowHeight = rSubItem.Height();
				Invalidate(TRUE);
			}
			if (rSubItem.left < rSubItem.right)
			{
				//画最右边的列
				bool bSelected = false;
				if (GetItemState(lpnmcd->nmcd.dwItemSpec, LVIS_SELECTED))
				{
					bSelected = true;
				}

				bool bFocus = false;
				//HWND hWndFocus = ::GetFocus();
				//if (::IsChild(m_hWnd,hWndFocus) || m_hWnd == hWndFocus)
				//{
				//	bFocus = true;
				//}
				CDC dc;
				dc.Attach(lpnmcd->nmcd.hdc);
				draw_row_bg(&dc, rSubItem, bSelected , bFocus, (int)lpnmcd->nmcd.dwItemSpec, nColumnCount);
				dc.Detach();
			}
		}
		TRACE("CDDS_ITEMPREPAINT col count %d, item %d, rowH %d realrowH %d, top %d\n", 
			nColumnCount, lpnmcd->nmcd.dwItemSpec, m_nRowHeight, m_nRealRowHeight, rSubItem.top);
		*pResult =  CDRF_NOTIFYSUBITEMDRAW;
		return;
	}
	else if (lpnmcd ->nmcd.dwDrawStage == (CDDS_SUBITEM | CDDS_ITEMPREPAINT))
	{
		int iItem = lpnmcd->nmcd.dwItemSpec;
		int iSubItem = lpnmcd->iSubItem;
		TRACE("CDDS_SUBITEM | CDDS_ITEMPREPAINT row %d col %d\n", iItem, iSubItem);

		if (iItem >= 0 && iSubItem >= 0)
		{
			CRect rSubItem;
			HDC hDC = lpnmcd->nmcd.hdc;
			GetSubItemRect(iItem, iSubItem, LVIR_LABEL, rSubItem);
			if (iSubItem == 0)
			{
				rSubItem.left -= 4;
				//if (GetExtendedStyle() & LVS_EX_CHECKBOXES)
				//{
				//	rSubItem.left -= 20;
				//}
			}

			bool bSelected = false;
			if (GetItemState(iItem, LVIS_SELECTED))
			{
				OnSelectedRowChanged(iItem);
				bSelected = true;
			}

			bool bFocus = false;
			//CWnd *pWndFocus = GetFocus();
			//if (IsChild(pWndFocus) || pWndFocus == this)
			//{
			//	bFocus = true;
			//}

			rSubItem.NormalizeRect();
			CDC dc;
			dc.Attach(lpnmcd->nmcd.hdc);
			DrawSubItem(&dc,iItem,iSubItem,rSubItem,bSelected,bFocus);
			dc.Detach();
			*pResult =  CDRF_SKIPDEFAULT;
			return;
		}
	}
	else if (lpnmcd ->nmcd.dwDrawStage == CDDS_POSTPAINT)
	{
		TRACE("CDDS_POSTPAINT top %d\n", lpnmcd->nmcd.rc.top);
		//if (m_header.GetHeaderHeight() != lpnmcd->nmcd.rc.top)
		//{
		//	Invalidate(TRUE);
		//}
		DrawRemainSpace(lpnmcd);
		*pResult =  CDRF_SKIPDEFAULT;
		return;
	}

	*pResult = 0;

}

// overwrite:
void CNewListCtrl::DrawSubItem(CDC *pDC, int nItem, int nSubItem, CRect& rSubItem, bool bSelected, bool bFocus)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colorText);

	CFont* pOldFont = pDC->SelectObject(&m_font);

	CString strText;
	strText = GetItemText(nItem, nSubItem);
	draw_row_bg(pDC, rSubItem, bSelected, bFocus, nItem, nSubItem);
	rSubItem.DeflateRect(4, 0);
	if (ListColumnCheckBox == m_mapColumnControlType[nSubItem])
	{
		CRect rcCheck = rSubItem;
		rcCheck.right = rcCheck.left + 12;
		rcCheck.top = rcCheck.top + rcCheck.Height()/2 - 5;
		rcCheck.bottom = rcCheck.top + 12;
		pDC->Rectangle(&rcCheck);
		if (GetCheck(nItem))
		{
			int nArray[] = {1,7,1,8, 2,6,2,8, 3,7,3,9, 4,8,4,10, 5,8,5,10, 6,6,6,9, 7,4,7,7, 8,2,8,5, 9,2,9,3};
			int nCount = sizeof(nArray) / sizeof(nArray[0]) / 4;
			int nPot = 0;
			for (int n=0; n<nCount; ++n)
			{
				nPot = n * 4;
				pDC->MoveTo(rcCheck.left+nArray[nPot], rcCheck.top+nArray[nPot+1]);
				pDC->LineTo(rcCheck.left+nArray[nPot+2], rcCheck.top+nArray[nPot+3]);
			}
		}
		rSubItem.left = rcCheck.right + 2;
	}
	if (0 == nSubItem)
	{
		HICON hIcon = GetItemImage(nItem);
		if (hIcon)
		{
			CRect rcImg = rSubItem;
			rcImg.right = rcImg.left + 16;
			rcImg.DeflateRect(0, (rcImg.Height()-16)/2);
			::DrawIconEx(pDC->m_hDC, rcImg.left, rcImg.top, hIcon, 16, 16, 0, NULL, DI_NORMAL);
			rSubItem.left = rcImg.right + 2;
		}
	}
	pDC->DrawText(strText, strText.GetLength(), &rSubItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
	pDC->SelectObject(pOldFont);
}

// 画剩余部分
void CNewListCtrl::DrawRemainSpace(LPNMLVCUSTOMDRAW lpnmcd)
{
	int nTop = lpnmcd->nmcd.rc.top;
	int nCount = GetItemCount();
	if (nCount > 0)
	{
		CRect rcItem;
		GetItemRect(nCount - 1, &rcItem, LVIR_LABEL);
		nTop = rcItem.bottom;
	}

	CRect rectClient;
	GetClientRect(&rectClient);
	if (nTop < lpnmcd->nmcd.rc.bottom) // 有剩余
	{
		CRect rcRemain = lpnmcd->nmcd.rc;
		rcRemain.top = nTop;
		rcRemain.right = rectClient.right;
		int nRemainItem = rcRemain.Height() / m_nRealRowHeight;
		if (rcRemain.Height() % m_nRealRowHeight)
		{
			nRemainItem++;
		}

		int pos = GetScrollPos(SB_HORZ);
		CDC dc;
		dc.Attach(lpnmcd->nmcd.hdc);
		int nColumnCount = GetHeaderCtrl()->GetItemCount();
		CRect  rcSubItem;
		CRect rcItem;
		for (int i = 0; i < nRemainItem; ++i)
		{

			rcItem.top = rcRemain.top + i * m_nRealRowHeight;
			rcItem.left = rcRemain.left;
			rcItem.right = rcRemain.right;
			rcItem.bottom = rcItem.top + m_nRealRowHeight;
			for (int j = 0; j < nColumnCount; ++j)
			{
				GetHeaderCtrl()->GetItemRect(j, &rcSubItem);
				rcSubItem.top = rcItem.top;
				rcSubItem.bottom = rcItem.bottom;
				rcSubItem.OffsetRect(-pos, 0);
				if(rcSubItem.right < rcRemain.left || rcSubItem.left > rcRemain.right)
					continue;
				draw_row_bg(&dc, rcSubItem, false, false, i + nCount, j);
			}
			if (rcSubItem.right<rectClient.right)//画最右边的列
			{
				rcSubItem.left=rcSubItem.right;
				rcSubItem.right=rectClient.right;
				draw_row_bg(&dc, rcSubItem, false, false, i+nCount, nColumnCount);
			}
		}

		dc.Detach();
	}
}

void CNewListCtrl::draw_row_bg(CDC *pDC, CRect& rect, bool bSelected, bool bFocus,int nRow, int nCol)
{
	bool bOdd = (nRow % 2 == 0 ? true : false);

	if (rect.Width() == 0)
	{
		return;
	}

	draw_row_bg(pDC, rect, bSelected, bFocus, bOdd, nCol);
}

void CNewListCtrl::draw_row_bg(CDC *pDC, CRect& rect, bool bSelected, bool bFocus, bool bOdd, int nCol)
{
	int nSave = pDC->SaveDC();
	if (bSelected)
	{
		pDC->FillRect(&rect, &m_brushSelectedBack);

		//if (bFocus)
		//{
		//	CBrush selectBrush;
		//	selectBrush.CreateSolidBrush(RGB(203, 223, 239));
		//	pDC->FillRect(&rc, &selectBrush);
		//}
		//else
		//{
		//	CBrush selectNoFocusBrush;
		//	selectNoFocusBrush.CreateSolidBrush(RGB(206, 206, 206));
		//	pDC->FillRect(&rc, &selectNoFocusBrush);
		//}
	}
	else if (bOdd)
	{
		pDC->FillRect(&rect, &m_brushOddBack);
	}
	else
	{
		pDC->FillRect(&rect, &m_brushEvenBack);
	}

	BOOL bDrawHLine = FALSE;
	CPen* pOldPen;
	// 画选中的底部分割线
	//if (bSelected)
	//{
	//	if (m_penSelectedRowBottomLine.GetSafeHandle())
	//	{
	//		pOldPen = pDC->SelectObject(&m_penSelectedRowBottomLine);
	//		pDC->MoveTo(rect.left, rect.bottom - 1);
	//		pDC->LineTo(rect.right, rect.bottom - 1);
	//		pDC->SelectObject(pOldPen);
	//		bDrawVLine = TRUE;
	//	}
	//	else if (m_penHorizontalLine.GetSafeHandle())
	//	{

	//	}
	//}
	//else
	if (m_penHorizontalLine.GetSafeHandle())
	{
		pOldPen = pDC->SelectObject(&m_penHorizontalLine);
		pDC->MoveTo(rect.left, rect.bottom - 1);
		pDC->LineTo(rect.right, rect.bottom - 1);
		pDC->SelectObject(pOldPen);
		bDrawHLine = TRUE;
	}

	// 画竖线
	if (nCol < GetHeaderCtrl()->GetItemCount())
	{
		pOldPen = pDC->SelectObject(&m_penVerticalLine);
		pDC->MoveTo(rect.right - 1, rect.top);
		pDC->LineTo(rect.right - 1, bDrawHLine ? rect.bottom - 1 : rect.bottom);
		pDC->SelectObject(pOldPen);
	}

	pDC->RestoreDC(nSave);
}

void CNewListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	Invalidate(FALSE);
	*pResult = 0;
}

//void CNewListCtrl::InvalidateItem(int nItem)
//{
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	CRect rcItem;
//	GetItemRect(nItem, &rcItem, LVIR_BOUNDS);
//
//	rcItem.left = rcClient.left;
//	rcItem.right = rcClient.right;
//	InvalidateRect(&rcItem,FALSE);
//}

//void CNewListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//
//}

void CNewListCtrl::OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	Default();
	Invalidate(FALSE);
	*pResult = 0;
}

void CNewListCtrl::OnHdnEndTrack(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    if (phdr->pitem->cxy < 20)
    {
        phdr->pitem->cxy = 20;
    }
    Default();
    Invalidate(FALSE);
    *pResult = 0;
}

void CNewListCtrl::OnHdnDividerDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    *pResult = 1;
}

void CNewListCtrl::OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 5.5 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0560。
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	Invalidate(FALSE);
	*pResult = 0;
}

void CNewListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (NotSort == m_mapColumnSortType[pNMListView->iSubItem])
	{
		::SendMessage(::GetParent(m_hWnd), WM_LIST_COLUMN_CLICK, pNMListView->iSubItem, NotSort);
		*pResult = 0;
		return;
	}

	if(pNMListView->iSubItem == m_nSortColumn)
	{
		if (m_sortState == StateNotSort)
		{
			m_sortState = StateAsc;
		}
		else if (m_sortState == StateAsc)
		{
			m_sortState = StateDesc;
		}
		else
		{
			m_sortState = StateAsc;
		}
	}
	else
	{
		if (m_nSortColumn >= 0)
		{
			m_header.SetItemSortState(m_nSortColumn, StateNotSort);
		}
		m_sortState = StateAsc;
		m_nSortColumn = pNMListView->iSubItem;
	}
	m_header.SetItemSortState(m_nSortColumn, m_sortState);
	SortItems(ListCompare, (DWORD)this);
	::SendMessage(::GetParent(m_hWnd), WM_LIST_COLUMN_CLICK, m_nSortColumn, m_sortState);
	*pResult = 0;
}

void CNewListCtrl::SortColumn(int nColumn, ColumnSortState state)
{
	if (nColumn >= 0 && nColumn < m_header.GetItemCount())
	{
        if (m_nSortColumn >= 0 && m_nSortColumn != nColumn)
        {
            m_header.SetItemSortState(m_nSortColumn, StateNotSort);
        }
		m_nSortColumn = nColumn;
		m_sortState = state;
		m_header.SetItemSortState(nColumn, state);
		SortItems(ListCompare, (DWORD_PTR)this);
	}
}

void CNewListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (nSBCode == SB_ENDSCROLL)
    {
        Invalidate();
    }
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}


int CALLBACK CNewListCtrl::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CNewListCtrl* pList=(CNewListCtrl*)lParamSort;
	ColumnSortType colSortType = pList->m_mapColumnSortType[pList->m_nSortColumn];
	int nComp = 0;

	if (SortByItemData == colSortType)
	{
		if (pList->m_sortFun)
		{
			nComp = pList->m_sortFun(lParam1, lParam2, pList->m_nSortColumn);
			if (pList->m_sortState == StateDesc)
			{
				nComp *= -1;
			}
		}
		return nComp;
	}

	int nItem1, nItem2;
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;    // 指定查找方式
	FindInfo.lParam = lParam1;
	nItem1 = pList->FindItem(&FindInfo, -1); // 得到对应Item索引
	FindInfo.lParam = lParam2;
	nItem2 = pList->FindItem(&FindInfo, -1);

	if((nItem1 == -1) || (nItem2 == -1))
	{
		TRACE("无法找到!/n");
		return 0;
	}

	CString str1,str2;
	str1 = pList->GetItemText(nItem1, pList->m_nSortColumn); // 得到排序列的Text
	str2 = pList->GetItemText(nItem2, pList->m_nSortColumn);

	switch(colSortType)
	{
	case SortByInt:
		{
			int i1,i2;
			i1 = _ttoi( str1 );
			i2 = _ttoi( str2 );
			if( i1 > i2)
				nComp = 1;
			else if( i1 == i2 )
				nComp = 0;
			else
				nComp = -1;
		}
		break;
	case SortByDouble:
		{
			double i1,i2;
			i1 = _ttof( str1 );
			i2 = _ttof( str2 );
			if( i1 > i2)
				nComp = 1;
			else if( i1 == i2 )
				nComp = 0;
			else
				nComp = -1;
		}
		break;
	case SortByString:
		{
			if( str1 > str2)
				nComp = 1;
			else if( str1 == str2 )
				nComp = 0;
			else
				nComp = -1;
			break;
		}
	case SortByStringNoCase:
		{
			nComp = str1.CompareNoCase(str2);
		}
		break;
	case SortByItemData: //不会进入此分支
	default:
		{
			return 0;
		}
		break;
	}

	if(pList->m_sortState == StateAsc)
		return nComp;
	else if (pList->m_sortState == StateDesc)
		return nComp*-1;

	return 0;
}


void CNewListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO lvHitTestInfo;
	CRect rcItem;
	lvHitTestInfo.pt = point;
	int nRow = -1;
	int nCol = -1;
	CRect rcSubItem;
	if (HitTest(&lvHitTestInfo) >= 0 && lvHitTestInfo.iItem >= 0)
	{
		nRow = lvHitTestInfo.iItem;
		int nColCnt= m_header.GetItemCount();
		for (int n=0; n<nColCnt; ++n)
		{
			if (GetSubItemRect(nRow, n, LVIR_LABEL, rcSubItem))
			{
				if (n == 0)
				{
					rcSubItem.left -= 4;
					//if (GetExtendedStyle() & LVS_EX_CHECKBOXES)
					//{
					//	rcSubItem.left -= 20;
					//}
				}
				if (rcSubItem.PtInRect(point))
				{
					nCol = n;
					break;
				}
			}
		}
	}

	CListCtrl::OnLButtonDown(nFlags, point);
	OnSelectedRowChanged(nRow);

	// If the SHIFT or CTRL key is down call the base class
	// Check the high bit of GetKeyState to determine whether SHIFT or CTRL key is down
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		return;
	}

	ListNotifyData notifyData;
	notifyData.nRow = nRow;
	notifyData.nCol = nCol;
	notifyData.rcItem = rcSubItem;
	notifyData.pWnd = this;

	if (m_pBuddyEdit && m_pBuddyEdit->IsWindowVisible())
	{
		m_pBuddyEdit->ShowWindow(SW_HIDE);
		Invalidate(FALSE);
	}

	if (nRow >= 0 && nCol >= 0)
	{
		CString cstrItemText = GetItemText(nRow, nCol);
		notifyData.cstrText = cstrItemText;

		if (ListColumnCheckBox == m_mapColumnControlType[nCol])
		{
			if (point.x < rcSubItem.left + 16) //16为check区域宽度
			{
				BOOL bCheck = !GetCheck(nRow);
				SetCheck(nRow, bCheck);
				::SendMessage(::GetParent(m_hWnd), WM_LIST_CHECK_CHANGED, (WPARAM)&notifyData, bCheck);
			}
		}
		else if (ListColumnEdit == m_mapColumnControlType[nCol])
		{
			if (m_pBuddyEdit)
			{
				m_pBuddyEdit->m_nRow = nRow;
				m_pBuddyEdit->m_nCol = nCol;
				m_pBuddyEdit->m_cstrInit = cstrItemText;
				m_pBuddyEdit->SetWindowText(cstrItemText);
				m_pBuddyEdit->SetSel(0, -1);
				m_pBuddyEdit->MoveWindow(rcSubItem.left, rcSubItem.top, rcSubItem.Width(), rcSubItem.Height(), FALSE);

				notifyData.pWnd = m_pBuddyEdit;
				m_pBuddyEdit->ShowWindow(SW_SHOW);
				m_pBuddyEdit->SetFocus();
				::SendMessage(::GetParent(m_hWnd), WM_LIST_SHOW_BUDDY_EDIT, (WPARAM)&notifyData, NULL);
			}
		}
		else if (ListColumnComboBox == m_mapColumnControlType[nCol])
		{
			if (m_pBuddyComboBox)
			{
				m_pBuddyComboBox->m_cstrInit = cstrItemText;
				int nIndex = m_pBuddyComboBox->GetStringIndex(cstrItemText);
				if (nIndex >= 0)
				{
					m_pBuddyComboBox->SetCurSel(nIndex);
				}
				else
				{
					if (m_pBuddyComboBox->GetStyle() & CBS_DROPDOWNLIST)
					{
						m_pBuddyComboBox->SetCurSel(0);
					}
					else
					{
						m_pBuddyComboBox->SetCurSel(-1);
					}
					m_pBuddyComboBox->SetWindowText(cstrItemText);
				}
				m_pBuddyComboBox->m_nRow = nRow;
				m_pBuddyComboBox->m_nCol = nCol;
				m_pBuddyComboBox->MoveWindow(rcSubItem.left, rcSubItem.top, rcSubItem.Width(), rcSubItem.Height() * 3, FALSE);

				notifyData.pWnd = m_pBuddyComboBox;
				m_pBuddyComboBox->ShowWindow(SW_SHOW);
				m_pBuddyComboBox->SendMessage(CB_SHOWDROPDOWN, (WPARAM)TRUE, 0);
				m_pBuddyComboBox->SetFocus();
				::SendMessage(::GetParent(m_hWnd), WM_LIST_SHOW_BUDDY_COMBOBOX, (WPARAM)&notifyData, NULL);
			}
		}
	}

	::SendMessage(::GetParent(m_hWnd), WM_LIST_CLICK, (WPARAM)&notifyData, NULL);
}

void CNewListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO lvHitTestInfo;
	CRect rcItem;
	lvHitTestInfo.pt = point;
	int nRow = -1;
	int nCol = -1;
	CRect rcSubItem;
	if (HitTest(&lvHitTestInfo) >= 0 && lvHitTestInfo.iItem >= 0)
	{
		nRow = lvHitTestInfo.iItem;
		int nColCnt= m_header.GetItemCount();
		for (int n=0; n<nColCnt; ++n)
		{
			if (GetSubItemRect(nRow, n, LVIR_LABEL, rcSubItem))
			{
				if (rcSubItem.PtInRect(point))
				{
					nCol = n;
					break;
				}
			}
		}
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);

	// If the SHIFT or CTRL key is down call the base class
	// Check the high bit of GetKeyState to determine whether SHIFT or CTRL key is down
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		return;
	}

	ListNotifyData notifyData;
	notifyData.nRow = nRow;
	notifyData.nCol = nCol;
	notifyData.rcItem = rcSubItem;
	if (nRow >= 0 && nCol >= 0)
	{
		notifyData.cstrText = GetItemText(nRow, nCol);
	}
	::SendMessage(::GetParent(m_hWnd), WM_LIST_DOUBLE_CLICK, (WPARAM)&notifyData, NULL);
}

BOOL CNewListCtrl::SetCheck(int nItem, BOOL bCheck)
{
	if (nItem >=0 && nItem < GetItemCount())
	{
		NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
		if (pData)
		{
			if (pData->bCheck != bCheck)
			{
				pData->bCheck = bCheck;
				Invalidate(FALSE);
			}
			return TRUE;
		}
		else
		{
			pData = new NItemData();
			if (pData)
			{
				if (pData->bCheck != bCheck)
				{
					pData->bCheck = bCheck;
					Invalidate(FALSE);
				}
				if (CListCtrl::SetItemData(nItem, (DWORD_PTR)pData))
				{
					m_vecNItemData.push_back(pData);
					return TRUE;
				}
				else
				{
					delete pData;
					pData = NULL;
				}
			}
		}
	}
	return FALSE;
}

BOOL CNewListCtrl::GetCheck(int nItem)
{
	NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
	if (pData)
	{
		return pData->bCheck;
	}
	return FALSE;
}

BOOL CNewListCtrl::SetItemData(int nItem, DWORD_PTR dwData)
{
	NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
	if (pData)
	{
		pData->dwData = dwData;
		return TRUE;
	}
	else
	{
		pData = new NItemData();
		if (pData)
		{
			pData->dwData = dwData;
			if (CListCtrl::SetItemData(nItem, (DWORD_PTR)pData))
			{
				m_vecNItemData.push_back(pData);
				return TRUE;
			}
			else
			{
				delete pData;
				pData = NULL;
			}
		}
	}
	return FALSE;
}

DWORD_PTR CNewListCtrl::GetItemData(int nItem)
{
	NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
	if (pData)
	{
		return pData->dwData;
	}
	return NULL;
}

BOOL CNewListCtrl::DeleteItem(int nItem)
{
	NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
	if (CListCtrl::DeleteItem(nItem))
	{
		if (pData)
		{
			for (auto it=m_vecNItemData.begin(); it!=m_vecNItemData.end(); ++it)
			{
				if (*it == pData)
				{
					m_vecNItemData.erase(it);
					break;
				}
			}
			delete pData;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CNewListCtrl::DeleteAllItems()
{
	BOOL bRet = TRUE;
	int nCount = GetItemCount();
	for (int n=nCount-1; n>=0; --n)
	{
		bRet &= DeleteItem(n);
	}
	return bRet;
}

void CNewListCtrl::SetItemImage(int nItem, HICON hIcon, BOOL bInvalidate)
{
	if (nItem >=0 && nItem < GetItemCount())
	{
		NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
		if (pData)
		{
			pData->hIcon = hIcon;
			if (bInvalidate)
			{
				Invalidate(FALSE);
			}
		}
		else
		{
			pData = new NItemData();
			if (pData)
			{
				pData->hIcon = hIcon;
				if (bInvalidate)
				{
					Invalidate(FALSE);
				}
				if (CListCtrl::SetItemData(nItem, (DWORD_PTR)pData))
				{
					m_vecNItemData.push_back(pData);
				}
				else
				{
					delete pData;
					pData = NULL;
				}
			}
		}
	}
}

HICON CNewListCtrl::GetItemImage(int nItem)
{
	NItemData* pData = (NItemData*)CListCtrl::GetItemData(nItem);
	if (pData)
	{
		return pData->hIcon;
	}
	return NULL;
}


//combobox

CBuddyComboBox::CBuddyComboBox()
	: m_nRow(-1)
	, m_nCol(-1)
	, m_bESC(FALSE)
	, m_pList(NULL)
{

}

BEGIN_MESSAGE_MAP(CBuddyComboBox, CComboBox)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
END_MESSAGE_MAP()

int CBuddyComboBox::GetStringIndex(CString& cstrText)
{
	int nIndex = -1;
	CString cstrCombo;
	for (int i=0; i<GetCount(); ++i)
	{
		GetLBText(i, cstrCombo);
		if (cstrCombo == cstrText)
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}

BOOL CBuddyComboBox::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			//::TranslateMessage(pMsg);
			//::DispatchMessage(pMsg);
			//return TRUE;                
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			m_bESC = TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CBuddyComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(VK_RETURN == nChar)
	{
		OnCloseup();
		return;
	}
	else if (VK_ESCAPE == nChar)
	{
		m_bESC = TRUE;
		OnCloseup();
		return;
	}

	return CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CBuddyComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
}

void CBuddyComboBox::OnCloseup() 
{
	if (!m_bESC)
	{
		CString cstrText;
		GetWindowText(cstrText);
		int nSel = GetCurSel();
		if (nSel >= 0)
		{
			GetLBText(nSel, cstrText);
		}
		if (m_pList)
		{
			m_pList->SetItemText(m_nRow, m_nCol, cstrText);
			if (m_cstrInit != cstrText)
			{
				ListNotifyData notifyData;
				notifyData.nRow = m_nRow;
				notifyData.nCol = m_nCol;
				notifyData.cstrText = cstrText;
				notifyData.pWnd = this;
				::SendMessage(::GetParent(m_pList->m_hWnd), WM_LIST_BUDDY_COMBOBOX_CHANGED, (WPARAM)&notifyData, NULL);
			}
		}
	}
	//GetParent()->SetFocus();
	ShowWindow(SW_HIDE);
	m_bESC = FALSE;
}

//edit

CBuddyEdit::CBuddyEdit()
	: m_nRow(-1)
	, m_nCol(-1)
	, m_bESC(FALSE)
	, m_pList(NULL)
{

}

BEGIN_MESSAGE_MAP(CBuddyEdit, CEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
END_MESSAGE_MAP()

BOOL CBuddyEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnCloseup();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			m_bESC = TRUE;
			OnCloseup();
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CBuddyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(VK_RETURN == nChar)
	{
		OnCloseup();
		return;
	}
	else if (VK_ESCAPE == nChar)
	{
		m_bESC = TRUE;
		OnCloseup();
		return;
	}

	return CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CBuddyEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	OnCloseup();
}

void CBuddyEdit::OnCloseup() 
{
	if (!m_bESC)
	{
		CString cstrText;
		GetWindowText(cstrText);
		if (m_pList)
		{
			m_pList->SetItemText(m_nRow, m_nCol, cstrText);
			if (m_cstrInit != cstrText)
			{
				ListNotifyData notifyData;
				notifyData.nRow = m_nRow;
				notifyData.nCol = m_nCol;
				notifyData.cstrText = cstrText;
				notifyData.pWnd = this;
				::SendMessage(::GetParent(m_pList->m_hWnd), WM_LIST_BUDDY_EDIT_CHANGED, (WPARAM)&notifyData, NULL);
			}
		}
	}
	//GetParent()->SetFocus();
	ShowWindow(SW_HIDE);
	m_bESC = FALSE;
}


