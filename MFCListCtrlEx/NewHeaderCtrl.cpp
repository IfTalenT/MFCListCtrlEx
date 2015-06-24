// NewHeaderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "NewHeaderCtrl.h"


// CNewHeaderCtrl

IMPLEMENT_DYNAMIC(CNewHeaderCtrl, CHeaderCtrl)

CNewHeaderCtrl::CNewHeaderCtrl()
	: m_nHeaderHeight(1)
	, m_nCheckColumn(-1)
	, m_bAllCheck(FALSE)
	, m_colorText(RGB(0,0,0))
{
    m_penVerticalLine.DeleteObject();
	m_penVerticalLine.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	//m_imgHeader.LoadFromResource(AfxGetResourceHandle(), IDB_BITMAP_HEADER_BACK);
    m_penArrow.DeleteObject();
	m_penArrow.CreatePen(PS_SOLID, 1, RGB(0,0,0));
    m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(RGB(180,180,180));
}

CNewHeaderCtrl::~CNewHeaderCtrl()
{
}

BEGIN_MESSAGE_MAP(CNewHeaderCtrl, CHeaderCtrl)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CNewHeaderCtrl::SetHeaderFont(CFont& font)
{
	LOGFONT logFont;
	font.GetLogFont(&logFont);
    m_font.DeleteObject();
	m_font.CreateFontIndirect(&logFont);
}

void CNewHeaderCtrl::SetHeaderHeight(int nHeight)
{
	m_nHeaderHeight = nHeight;
}

int CNewHeaderCtrl::GetHeaderHeight()
{
	return m_nHeaderHeight;
}

void CNewHeaderCtrl::SetVerticalLineColor(COLORREF cLine)
{
	m_penVerticalLine.DeleteObject();
	m_penVerticalLine.CreatePen(PS_SOLID, 1, cLine);
}

void CNewHeaderCtrl::SetHeaderImage(HBITMAP hBmp)
{
	m_imgHeader.Destroy();
	m_imgHeader.Attach(hBmp);
}

void CNewHeaderCtrl::SetHeaderBackColor(COLORREF cBack)
{
	m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(cBack);
}

void CNewHeaderCtrl::SetTextColor(COLORREF cText)
{
	m_colorText = cText;
}

void CNewHeaderCtrl::SetArrowColor(COLORREF cArrow)
{
	m_penArrow.DeleteObject();
	m_penArrow.CreatePen(PS_SOLID, 1, cArrow);
}

void CNewHeaderCtrl::SetCheck(BOOL bCheck)
{
	m_bAllCheck = bCheck;
	Invalidate(FALSE);
}

BOOL CNewHeaderCtrl::GetCheck()
{
	return m_bAllCheck;
}

void CNewHeaderCtrl::SetItemSortState(int nColumn, ColumnSortState sortState)
{
	ASSERT(m_hWnd != NULL && IsWindow(m_hWnd));

	int nItemCount = GetItemCount();
	ASSERT(nColumn >= 0 && nColumn < nItemCount);

	HDITEM hditem;
	hditem.mask = HDI_FORMAT;
	GetItem(nColumn, &hditem);

	if (sortState == StateAsc)
	{
		hditem.fmt &= ~HDF_SORTDOWN;
		hditem.fmt |= HDF_SORTUP;
	}
	else if (sortState == StateDesc)
	{
		hditem.fmt &= ~HDF_SORTUP;
		hditem.fmt |= HDF_SORTDOWN;
	}
	else
	{
		hditem.fmt &= ~HDF_SORTUP;
		hditem.fmt &= ~HDF_SORTDOWN;
	}
	SetItem(nColumn, &hditem);

	CRect rect;
	GetItemRect(nColumn, &rect);
	InvalidateRect(&rect);
}

ColumnSortState CNewHeaderCtrl::GetItemSortState(int nColumn)
{
	ASSERT(m_hWnd != NULL && IsWindow(m_hWnd));
	int nItemCount = GetItemCount();
	ASSERT(nColumn >= 0 && nColumn < nItemCount);

	HDITEM hditem;
	hditem.mask = HDI_FORMAT;
	GetItem(nColumn, &hditem);

	if (hditem.fmt & HDF_SORTUP)
	{
		return StateAsc;
	}
	else if (hditem.fmt & HDF_SORTDOWN)
	{
		return StateDesc;
	}
	else
	{
		return StateNotSort;
	}
}

LRESULT CNewHeaderCtrl::OnLayout(WPARAM wParam,LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *(HD_LAYOUT *) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 
	pwpos->cy = m_nHeaderHeight; 
	prc->top = m_nHeaderHeight;
	return lResult;
}

void CNewHeaderCtrl::Init()
{
	ModifyStyle(0, HDS_FLAT);
}

void CNewHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DoPaint(&dc);
}

void CNewHeaderCtrl::DoPaint(CDC *pDC)
{
	CRect rect, rcItem;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap bmp;
	memDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(m_colorText);
	memDC.SetStretchBltMode(HALFTONE);

	CFont* pOldFont = memDC.SelectObject(&m_font);

	// 填充背景
	if (m_imgHeader.IsNull())
	{
		memDC.FillRect(&rect, &m_brushBack);
	}
	else
	{
		m_imgHeader.Draw(memDC, rect);
	}

	CPen *pOldPen;

	int nItems = GetItemCount();
	for (int i = 0; i < nItems; ++i)
	{
		TCHAR buf[256];
		HDITEM hditem;

		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem(i, &hditem);
		GetItemRect(i, &rcItem);

		if (rcItem.IsRectEmpty())
		{
			continue;
		}

		// 画分割线
		pOldPen = memDC.SelectObject(&m_penVerticalLine);
		memDC.MoveTo(rcItem.right-1, 0);
		memDC.LineTo(rcItem.right-1, rcItem.bottom);
		memDC.SelectObject(pOldPen);
		//image.LoadFromResource(AfxGetResourceHandle(), IDB_BITMAP_HEADER_SPLIT);
		//image.Draw(memDC, rcItem.right - 1, rcItem.top);
		//image.Destroy();

		CRect rcText = rcItem;
		rcText.DeflateRect(4, 0, 10, 0);
		if (i == m_nCheckColumn)
		{
			CPen pen(PS_SOLID, 1, RGB(0,0,0));
			pOldPen = memDC.SelectObject(&pen);
			CRect rcCheck = rcText;
			rcCheck.right = rcCheck.left + 12;
			rcCheck.top = rcCheck.top + rcCheck.Height()/2 - 5;
			rcCheck.bottom = rcCheck.top + 12;
			memDC.Rectangle(&rcCheck);
			if (m_bAllCheck)
			{
				int nArray[] = {1,7,1,8, 2,6,2,8, 3,7,3,9, 4,8,4,10, 5,8,5,10, 6,6,6,9, 7,4,7,7, 8,2,8,5, 9,2,9,3};
				int nCount = sizeof(nArray) / sizeof(nArray[0]) / 4;
				int nPot = 0;
				for (int n=0; n<nCount; ++n)
				{
					nPot = n * 4;
					memDC.MoveTo(rcCheck.left+nArray[nPot], rcCheck.top+nArray[nPot+1]);
					memDC.LineTo(rcCheck.left+nArray[nPot+2], rcCheck.top+nArray[nPot+3]);
				}
			}
			memDC.SelectObject(pOldPen);
			rcText.left = rcCheck.right + 2;
		}

		// 画文字和排序箭头
		UINT uFormat = DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX;
		if (hditem.fmt & HDF_RIGHT)
		{
			uFormat |= DT_RIGHT;
		}
		else if (hditem.fmt & HDF_CENTER)
		{
			uFormat |= DT_CENTER;
		}

		memDC.DrawText(buf, static_cast<int> (_tcslen(buf)), &rcText, uFormat);
		rcText.right = rcItem.right;
		rcText.left = rcText.right - 15;
		if (rcText.left >= rcItem.left)
		{
			if (hditem.fmt & HDF_SORTUP)
			{
				pOldPen = memDC.SelectObject(&m_penArrow);
				CPoint pt = rcText.CenterPoint();
				pt.Offset(0, -2);
				for (int j=0; j<5; ++j)
				{
					memDC.MoveTo(pt.x-j, pt.y);
					memDC.LineTo(pt.x+j, pt.y);
					++pt.y;
				}
				memDC.SelectObject(pOldPen);
			}
			else if (hditem.fmt & HDF_SORTDOWN)
			{
				pOldPen = memDC.SelectObject(&m_penArrow);
				CPoint pt = rcText.CenterPoint();
				pt.Offset(0, 2);
				for (int j=0; j<5; ++j)
				{
					memDC.MoveTo(pt.x-j, pt.y);
					memDC.LineTo(pt.x+j, pt.y);
					--pt.y;
				}
				memDC.SelectObject(pOldPen);
			}
		}
	}

	memDC.SelectObject(pOldFont);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBmp);
}

void CNewHeaderCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	Init();
	CHeaderCtrl::PreSubclassWindow();
	LOGFONT logFont;
	GetFont()->GetLogFont(&logFont);
    m_font.DeleteObject();
	m_font.CreateFontIndirect(&logFont);
}

//void CNewHeaderCtrl::OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	//Invalidate();
//	*pResult = 0;
//}

BOOL CNewHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CNewHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nCheckColumn >= 0)
	{
		CRect rcItem;
		int nItems = GetItemCount();
		for (int i = 0; i < nItems; ++i)
		{
			GetItemRect(i, &rcItem);
			rcItem.right = rcItem.left + 16;
			if (i == m_nCheckColumn && rcItem.PtInRect(point))
			{
				m_bAllCheck = !m_bAllCheck;
				Invalidate(FALSE);
				::PostMessage(::GetParent(::GetParent(m_hWnd)), WM_LIST_HEADER_CHECK_CHANGED, i, m_bAllCheck);
				return;
			}
		}
	}
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CNewHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	Invalidate(FALSE);
}

