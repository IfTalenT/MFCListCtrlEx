
// MFCListCtrlExDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCListCtrlEx.h"
#include "MFCListCtrlExDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDM_MENU1 17000
#define IDM_MENU2 17001

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCListCtrlExDlg 对话框

CMFCListCtrlExDlg::CMFCListCtrlExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCListCtrlExDlg::IDD, pParent)
	, m_nRowHeight(40)
	, m_nHeaderHeight(50)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCListCtrlExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_EDIT_4_LIST, m_edit);
	DDX_Control(pDX, IDC_COMBO_4_LIST, m_comboBox);
	DDX_Text(pDX, IDC_EDIT_ROW_HEIGHT, m_nRowHeight);
	DDV_MinMaxInt(pDX, m_nRowHeight, 10, 50);
	DDX_Text(pDX, IDC_EDIT_HEADER_HEIGHT, m_nHeaderHeight);
	DDV_MinMaxInt(pDX, m_nHeaderHeight, 0, 50);
	DDX_Control(pDX, IDC_EDIT1, m_editOutput);
}

BEGIN_MESSAGE_MAP(CMFCListCtrlExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_STYLE1, OnBnClickedButtonStyle1)
	ON_BN_CLICKED(IDC_BUTTON_STYLE2, OnBnClickedButtonStyle2)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_NO_HEADER, &CMFCListCtrlExDlg::OnBnClickedButtonNoHeader)
	ON_BN_CLICKED(IDC_BUTTON_SET_CHECK, &CMFCListCtrlExDlg::OnBnClickedButtonSetCheck)
	ON_MESSAGE(WM_LIST_SHOW_BUDDY_EDIT, OnListShowBuddyEdit)
	ON_MESSAGE(WM_LIST_SHOW_BUDDY_COMBOBOX, OnListShowBuddyComboBox)
	ON_MESSAGE(WM_LIST_BUDDY_EDIT_CHANGED, OnListBuddyEditChanged)
	ON_MESSAGE(WM_LIST_BUDDY_COMBOBOX_CHANGED, OnListBuddyComboBoxChanged)
	ON_MESSAGE(WM_LIST_HEADER_CHECK_CHANGED, OnListHeaderCheckChanged)
	ON_MESSAGE(WM_LIST_CHECK_CHANGED, OnListCheckChanged)
	ON_MESSAGE(WM_LIST_COLUMN_CLICK, OnListColumnClick)
	ON_MESSAGE(WM_LIST_CLICK, OnListClick)
	ON_MESSAGE(WM_LIST_DOUBLE_CLICK, OnListDoubleClick)
	ON_MESSAGE(WM_LIST_SELECT_CHANGED, OnListSelectedRowChanged)
	ON_BN_CLICKED(IDC_BUTTON_SET_SORT_STATE, &CMFCListCtrlExDlg::OnBnClickedButtonSetSortState)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMFCListCtrlExDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMFCListCtrlExDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SORT_COLUMN, &CMFCListCtrlExDlg::OnBnClickedButtonSortColumn)
    ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMFCListCtrlExDlg::OnNMRClickList)
    ON_COMMAND(IDM_MENU1, &CMFCListCtrlExDlg::OnListMenu1)
    ON_COMMAND(IDM_MENU2, &CMFCListCtrlExDlg::OnListMenu2)
END_MESSAGE_MAP()


// CMFCListCtrlExDlg 消息处理程序

BOOL CMFCListCtrlExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_comboBox.AddString(TEXT("hi1"));
	m_comboBox.AddString(TEXT("hi2"));
	m_comboBox.AddString(TEXT("hi3"));

	// TODO: 在此添加额外的初始化代码
	//DWORD dwExtentStyle = m_listCtrl.GetExtendedStyle();
	//m_listCtrl.SetExtendedStyle(dwExtentStyle|LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER); //设为双缓冲
	m_listCtrl.SetBuddyEdit(&m_edit);
	m_listCtrl.SetBuddyComboBox(&m_comboBox);

	m_listCtrl.InsertColumn(0, TEXT("Int"));
	m_listCtrl.SetColumnWidth(0,80);
	m_listCtrl.InsertColumn(1, TEXT("String(edit)"));
	m_listCtrl.SetColumnWidth(1,80);
	m_listCtrl.InsertColumn(2, TEXT("StringNoCase(Combo)"));
	m_listCtrl.SetColumnWidth(2,150);
	m_listCtrl.InsertColumn(3, TEXT("ItemDataSort"));
	m_listCtrl.SetColumnWidth(3,100);
	m_listCtrl.InsertColumn(4, TEXT("OnlyChangeSortState"));
	m_listCtrl.SetColumnWidth(4,150);
	m_listCtrl.InsertColumn(5, TEXT("NotSort"));
	m_listCtrl.SetColumnWidth(5,100);


	m_listCtrl.SetColumSortType(0, SortByInt);
	m_listCtrl.SetColumSortType(1, SortByString);
	m_listCtrl.SetColumSortType(2, SortByStringNoCase);
	m_listCtrl.SetColumSortType(3, SortByItemData); //如果设置了TypeSortByItemData，必须设置SetSortFun
	m_listCtrl.SetColumSortType(4, SortByItemData);
	m_listCtrl.SetColumSortType(5, NotSort);
	m_listCtrl.SetSortFun(SortColumn); //只针对设置了SortByItemData的列有效

	m_listCtrl.SetColumnControlType(0, ListColumnCheckBox);
	m_listCtrl.SetColumnControlType(1, ListColumnEdit);
	m_listCtrl.SetColumnControlType(2, ListColumnComboBox);

	srand((unsigned)time(NULL));
	CString cstrText;
	int nIndex = 0;
	int nAddCount = 0;//rand()%5+5;
	for (int i=0; i< nAddCount; ++i)
	{
		int nCount = m_listCtrl.GetItemCount();
		CString cstrText;
		cstrText.Format(TEXT("%d"), nCount);
		int nIndex = m_listCtrl.InsertItem(nCount, cstrText);
		m_listCtrl.SetItemData(nIndex, nCount);
		cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
		m_listCtrl.SetItemText(nIndex, 1, cstrText);
		cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
		m_listCtrl.SetItemText(nIndex, 2, cstrText);
		cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
		m_listCtrl.SetItemText(nIndex, 3, cstrText);
		cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
		m_listCtrl.SetItemText(nIndex, 4, cstrText);
		cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
		m_listCtrl.SetItemText(nIndex, 5, cstrText);
	}

	//m_listCtrl.SetItemImage(0, m_hIcon);
	//m_listCtrl.SetItemImage(2, m_hIcon);

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCListCtrlExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCListCtrlExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCListCtrlExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCListCtrlExDlg::OnBnClickedButtonSet()
{
	UpdateData(TRUE);
	if (m_nRowHeight < 10)
	{
		m_nRowHeight = 10;
		UpdateData(FALSE);
	}
	else if (m_nRowHeight > 50)
	{
		m_nRowHeight = 50;
		UpdateData(FALSE);
	}

	if (m_nHeaderHeight < 0)
	{
		m_nHeaderHeight = 0;
		UpdateData(FALSE);
	}
	else if (m_nHeaderHeight > 50)
	{
		m_nHeaderHeight = 50;
		UpdateData(FALSE);
	}
	m_listCtrl.SetHeaderHeight(m_nHeaderHeight);
	m_listCtrl.SetRowHeight(m_nRowHeight);
}


void CMFCListCtrlExDlg::OnBnClickedButtonStyle1()
{
	HBITMAP hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_HEADER_BACK2));
	m_listCtrl.SetHeaderBackImage(hBmp);
	m_listCtrl.SetHeaderTextColor(RGB(255,255,255));
	m_listCtrl.SetHeaderArrowColor(RGB(255,255,255));
	m_listCtrl.SetOddRowBackColor(RGB(248,233,240));
	m_listCtrl.SetEvenRowBackColor(RGB(242,213,228));
	m_listCtrl.SetSelectedRowBackColor(RGB(211,210,238));
	m_listCtrl.SetVerticalLineColor(RGB(225,235,235));
	m_listCtrl.SetHorizontalLineColor(RGB(200,200,200));
	m_listCtrl.SetTextColor(RGB(255,0,120));
	m_listCtrl.SetHeaderHeight(40);
	m_listCtrl.SetRowHeight(36);
	m_listCtrl.Invalidate();
}


void CMFCListCtrlExDlg::OnBnClickedButtonStyle2()
{
	HBITMAP hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_HEADER_BACK3));
	m_listCtrl.SetHeaderBackImage(hBmp);
	m_listCtrl.SetOddRowBackColor(RGB(203,223,239));
	m_listCtrl.SetEvenRowBackColor(RGB(179,208,232));
	m_listCtrl.SetSelectedRowBackColor(RGB(240,200,220));
	m_listCtrl.SetVerticalLineColor(RGB(196,215,213));
	m_listCtrl.SetHorizontalLineColor(RGB(100,100,100));
	m_listCtrl.SetTextColor(RGB(0,0,0));
	m_listCtrl.SetHeaderHeight(36);
	m_listCtrl.SetRowHeight(26);

	m_listCtrl.Invalidate();
}


void CMFCListCtrlExDlg::OnBnClickedButtonChange()
{
	if (m_listCtrl.GetItemCount())
	{
		m_listCtrl.SetItemText(0, 1, TEXT("hello hi hi hi"));
	}
}

LRESULT CMFCListCtrlExDlg::OnListShowBuddyEdit(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	CBuddyEdit* pBuddyEdit = (CBuddyEdit*)notifyData.pWnd;
	//在此加入处理CBuddyEdit代码
	CString cstrDebug;
	cstrDebug.Format(TEXT("Row %d, Col %d Edit was showed, text is %s"), notifyData.nRow, notifyData.nCol, notifyData.cstrText);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListShowBuddyComboBox(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	CBuddyComboBox* pBuddyComboBox = (CBuddyComboBox*)notifyData.pWnd;
	//在此加入处理CBuddyComboBox代码
	CString cstrDebug;
	cstrDebug.Format(TEXT("Row %d, Col %d ComboBox was showed, text is %s"), notifyData.nRow, notifyData.nCol, notifyData.cstrText);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListBuddyEditChanged(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	CBuddyEdit* pBuddyComboBox = (CBuddyEdit*)notifyData.pWnd;
	//在此加入处理CBuddyComboBox代码
	CString cstrDebug;
	cstrDebug.Format(TEXT("Row %d, Col %d Edit text changed to %s"), notifyData.nRow, notifyData.nCol, notifyData.cstrText);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListBuddyComboBoxChanged(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	CBuddyComboBox* pBuddyComboBox = (CBuddyComboBox*)notifyData.pWnd;
	//在此加入处理CBuddyComboBox代码
	int nComboIndex = pBuddyComboBox->GetStringIndex(notifyData.cstrText);
	CString cstrDebug;
	cstrDebug.Format(TEXT("Row %d, Col %d ComboBox selected %d, text changed to %s"), notifyData.nRow, notifyData.nCol, nComboIndex, notifyData.cstrText);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListHeaderCheckChanged(WPARAM wParam, LPARAM lParam)
{
	int nColumn = wParam;
	BOOL bCheck = lParam;
	CString cstrDebug;
	cstrDebug.Format(TEXT("Header column %d CheckBox state changed to %d"), nColumn, bCheck);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListCheckChanged(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	//在此加入处理check代码
	BOOL bCheck = lParam;
	CString cstrDebug;
	cstrDebug.Format(TEXT("Row %d, Col %d CheckBox state changed to %d"), notifyData.nRow, notifyData.nCol, bCheck);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListColumnClick(WPARAM wParam, LPARAM lParam)
{
	int nColumn = wParam;
	ColumnSortState curState = (ColumnSortState)lParam;
	//在此加入处理单击代码
	CString cstrDebug;
	cstrDebug.Format(TEXT("You clicked header %d, current sort state is %d"), nColumn, curState);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListClick(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	//在此加入处理单击代码
	CString cstrDebug;
	cstrDebug.Format(TEXT("You clicked row %d, col %d, text is %s"), notifyData.nRow, notifyData.nCol, notifyData.cstrText);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	ListNotifyData notifyData = *(ListNotifyData*)wParam;
	//在此加入处理双击代码
	CString cstrDebug;
	cstrDebug.Format(TEXT("You double clicked row %d, col %d, text is %s"), notifyData.nRow, notifyData.nCol, notifyData.cstrText);
	DebugText(cstrDebug);

	return S_OK;
}

LRESULT CMFCListCtrlExDlg::OnListSelectedRowChanged(WPARAM wParam, LPARAM lParam)
{
	//在此加入选择行改变处理代码
	int nSelectRow = wParam;
	CString cstrDebug;
	cstrDebug.Format(TEXT("Row %d was selected"), nSelectRow);
	DebugText(cstrDebug);

	return S_OK;
}

void CMFCListCtrlExDlg::OnBnClickedButtonNoHeader()
{
	m_listCtrl.SetHeaderHeight(0);
}

void CMFCListCtrlExDlg::OnBnClickedButtonSetCheck()
{
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
	}
	else
	{
		while (pos)
		{
			int nItem = m_listCtrl.GetNextSelectedItem(pos);
			m_listCtrl.SetCheck(nItem, TRUE);
		}
	}
	m_listCtrl.GetHeaderCtrl()->SetCheck(TRUE);
}

void CMFCListCtrlExDlg::DebugText(CString& cstrText)
{
	cstrText += TEXT("\n");
	TRACE(cstrText);
	int nLength = m_editOutput.GetWindowTextLength();
	m_editOutput.SetSel(nLength, nLength);
	m_editOutput.ReplaceSel(cstrText);
}

void CMFCListCtrlExDlg::OnBnClickedButtonSetSortState()
{
	ColumnSortState curState = m_listCtrl.GetHeaderCtrl()->GetItemSortState(4);
	if (curState == StateAsc)
	{
		curState = StateDesc;
	}
	else
	{
		curState = StateAsc;
	}
	m_listCtrl.GetHeaderCtrl()->SetItemSortState(4, curState);
}

void CMFCListCtrlExDlg::OnBnClickedButtonAdd()
{
	m_listCtrl.SetRedraw(FALSE);
	int nCount = m_listCtrl.GetItemCount();
	CString cstrText;
	cstrText.Format(TEXT("%d"), nCount);
	int nIndex = m_listCtrl.InsertItem(nCount, cstrText);
	m_listCtrl.SetItemData(nIndex, nCount);
	cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
	m_listCtrl.SetItemText(nIndex, 1, cstrText);
	cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
	m_listCtrl.SetItemText(nIndex, 2, cstrText);
	cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
	m_listCtrl.SetItemText(nIndex, 3, cstrText);
	cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
	m_listCtrl.SetItemText(nIndex, 4, cstrText);
	cstrText.Format(rand()%2?TEXT("hi%d"):TEXT("HI%d"), rand()%100);
	m_listCtrl.SetItemText(nIndex, 5, cstrText);

	//是插入的行按当前方式排序
	int nSortedColumn = m_listCtrl.GetSortedColumn();
	if (nSortedColumn >= 0 && nSortedColumn <= 3)
	{
		m_listCtrl.SortItems(CNewListCtrl::ListCompare, (DWORD_PTR)&m_listCtrl);
	}
	m_listCtrl.SetRedraw(TRUE);
}

void CMFCListCtrlExDlg::OnBnClickedButtonDelete()
{
	int nSel = m_listCtrl.GetSelectionMark();
	if (nSel >= 0)
	{
		m_listCtrl.DeleteItem(nSel);
	}
}

void CMFCListCtrlExDlg::OnBnClickedButtonSortColumn()
{
	int nColumn = m_listCtrl.GetSortedColumn();
	ColumnSortState state = StateNotSort;
	if (-1 == nColumn)
	{
		nColumn = 0;
		state = StateAsc;
	}
	else
	{
		state = m_listCtrl.GetHeaderCtrl()->GetItemSortState(nColumn);
		if (StateAsc == state)
		{
			state = StateDesc;
		}
		else
		{
			state = StateAsc;
		}
	}
	m_listCtrl.SortColumn(nColumn, state);
}

static int CALLBACK SortColumn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nColumn = lParamSort;
	if (3 == nColumn)
	{
		int n1 = lParam1;
		int n2 = lParam2;
		if (n1 > n2)
		{
			return 1;
		}
		else if (n1 == n2)
		{
			return 0;
		}
		return -1;
	}

	//由于第5列是 OnlyChangeSortState，返回0
	return 0;
}






void CMFCListCtrlExDlg::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    if (m_listCtrl.GetSelectedCount() > 0)
    {
        CMenu menu;
        //CMenu *pPopup;
        //menu.LoadMenu(IDR_MENU_CARD_INFO);
        //pPopup = menu.GetSubMenu(0);
        //pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,GetParent());

        menu.CreatePopupMenu();
        menu.AppendMenu(MF_STRING, IDM_MENU1, TEXT("MENU1"));
        menu.AppendMenuW(MF_STRING, IDM_MENU2, TEXT("MENU2"));
        CPoint myPoint;
        ClientToScreen(&myPoint);
        GetCursorPos(&myPoint); //鼠标位置
        menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this); //or set TPM_RETURNCMD, return vlaue is the menu id
        //menu.DestroyMenu();
    }

    *pResult = 0;
}

void CMFCListCtrlExDlg::OnListMenu1()
{
    MessageBox(TEXT("Menu1"), TEXT("tip"), MB_OK);
}

void CMFCListCtrlExDlg::OnListMenu2()
{
    MessageBox(TEXT("Menu2"), TEXT("tip"), MB_OK);
}

