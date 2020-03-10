#include "stdafx.h"
#include "MyListCtrl.h"
#include "DjVu Sep.h"

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_x=0;m_y=0;
	m_cx=0;m_cy=0;

	////CDC* pdc = GetDC();
	//COLORREF crOldBkColor = pdc->GetBkColor();
	//pdc->SetBkColor(::GetSysColor(RGB(255,0,0)));
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

void CMyEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	//AfxMessageBox("CMyEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)");

	lpwndpos->x=m_x;
	lpwndpos->y=m_y;
	if (lpwndpos->cx<m_cx)lpwndpos->cx=m_cx;
	if (lpwndpos->cy<m_cy)lpwndpos->cy=m_cy;

	CEdit::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here	
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMyListBox, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)	
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CMyListBox::CMyListBox()
{
	m_nEdit=-1;
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::SetStyle(DWORD NewStile)
{
	ASSERT(::IsWindow(m_hWnd ));
	DWORD a2 = GetWindowLong(m_hWnd,GWL_STYLE);
	if ((a2 & LVS_TYPEMASK) != NewStile)
		SetWindowLong(m_hWnd, GWL_STYLE , (a2 & ~LVS_TYPEMASK) | NewStile);
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// http://www.firststeps.ru/mfc/steps/help/r.php?948

	int nItem = (int)lpDrawItemStruct->itemID;
	
	if (nItem < 0) return; 
	
	CString Bold = (LPCTSTR) lpDrawItemStruct->itemData;	
	CRect rcItem = &lpDrawItemStruct->rcItem;
	
	ASSERT(Bold.IsEmpty() == FALSE);	
	
	CString Flag = Bold.Right(1);
	
	Bold = Bold.Left(Bold.GetLength()-1);
	
	int bold_flag = atoi(Flag);
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	
	
	
	// Сохраните это значение, чтобы восстановить их при выполнено рисунок.
	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor = pDC->GetBkColor();
	
	// Если этот элемент выбран, установите фоновый цвет 
	// И текстовый цвет, чтобы приспособить значения. Также, стирание
	// Rect,  заполняя это с фоновым цветом.
	
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		//pDC->SetBkColor(::GetSysColor(RGB(0,0,255)));
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, 
			::GetSysColor(COLOR_HIGHLIGHT));
			//::GetSysColor(RGB(0,0,255)));
	}
	else
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	
	// Создать жирный шрифт и выбрать его в контекст
	CFont* pOldFont = pDC->GetCurrentFont();
	LOGFONT lf;
	pOldFont->GetLogFont(&lf);

	long old_weight = lf.lfWeight;
	
	if (bold_flag)
		lf.lfWeight = FW_BOLD;
	
	CFont NewFont;
	NewFont.CreateFontIndirect(&lf);	
	pDC->SelectObject(NewFont);

	Bold = MakeShortString(pDC, Bold,
				GetColumnWidth(0), 0);
	
	// Нарисовать текст
	pDC->DrawText(
		Bold,
		Bold.GetLength(),
		&rcItem,
		DT_SINGLELINE|DT_VCENTER);

// draw labels for extra columns

	//lf.lfWeight = old_weight;
	//NewFont.CreateFontIndirect(&lf);	
	//pDC->SelectObject(NewFont);
	
	static _TCHAR szBuff[MAX_PATH];

	int nRetLen = GetItemText(nItem, 1,
					szBuff, sizeof(szBuff));
	if (nRetLen == 0)
		return;

	UINT nJustify = DT_LEFT;
	
	rcItem.left += GetColumnWidth(0);
	rcItem.right = rcItem.left + GetColumnWidth(1);

	//pDC->SetBkColor(::GetSysColor(RGB(255,0,0)));

	//pDC->FillSolidRect(&lpDrawItemStruct->rcItem, 
			//::GetSysColor(COLOR_HIGHLIGHT));
			//::GetSysColor(RGB(255,0,0)));

	pDC->SelectObject(*pOldFont);

	pDC->DrawText(szBuff, -1, rcItem,
		nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
	
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBkColor);
	
	DeleteObject(NewFont);	
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::AddString(CString StItem)
{
	int new_pos = InsertItem(GetItemCount(), StItem);

	int pos = StItem.Find(" ");

	StItem = StItem.Mid(pos+2);	

	SetItemText(new_pos,1,st(GetNumOutFilename(StItem,m_IniPath)));	
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::ResetContent()
{
	DeleteAllItems();
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::SetCurSel(int nItem)
{
	// http://forum.sources.ru/index.php?showtopic=73196
	SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED); SetSelectionMark(nItem);
}

/////////////////////////////////////////////////////////////////////////////

int CMyListBox::GetCurSel()
{
	return GetSelectionMark();
}

/////////////////////////////////////////////////////////////////////////////

int CMyListBox::GetCount()
{
	return GetItemCount();
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::GetItemRect(int sel, CRect* rt)
{
	CListCtrl::GetItemRect(sel, rt, LVIR_LABEL);
}

/////////////////////////////////////////////////////////////////////////////

void CMyListBox::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{	
	// TODO: Add your control notification handler code here

    CPoint posMouse;

    GetCursorPos(&posMouse);

    ScreenToClient(&posMouse);

	LVHITTESTINFO htest;

	htest.pt=posMouse;

	htest.flags=0;   
	
	SubItemHitTest(&htest);

	if (htest.flags & LVHT_ONITEMLABEL && htest.iSubItem)
	{		
		//SetStyle(LVS_REPORT|LVS_OWNERDRAWFIXED|LVS_EDITLABELS);		

		int shift = 1;
		
		HWND hWnd=(HWND)SendMessage(LVM_GETEDITCONTROL);
		ASSERT(hWnd!=NULL);
		VERIFY(m_LVEdit.SubclassWindow(hWnd));

		CDC* pdc = m_LVEdit.GetDC();
		COLORREF crOldBkColor = pdc->GetBkColor();
		pdc->SetBkColor(::GetSysColor(RGB(255,0,0)));

		CRect rcItem;
		
		GetSubItemRect(htest.iItem,htest.iSubItem,LVIR_LABEL,rcItem);
		
		m_LVEdit.m_x=rcItem.left-shift;
		m_LVEdit.m_y=rcItem.top;
		m_LVEdit.m_cy=rcItem.Height();
		m_LVEdit.m_cx=rcItem.Width()+shift;
		
		m_LVEdit.MoveWindow(rcItem,true);

		CRect rcItem2;

		rcItem2.top = m_LVEdit.m_y;
		rcItem2.left = m_LVEdit.m_x;
		rcItem2.bottom = m_LVEdit.m_cy;
		rcItem2.right = m_LVEdit.m_cx;
/*
		CDC* pdc = m_LVEdit.GetDC();
	
		//COLORREF crOldTextColor = pdc->GetTextColor();	
		COLORREF crOldBkColor = pdc->GetBkColor();

		//pdc->SetTextColor(::GetSysColor(RGB(0,0,0)));
		pdc->SetBkColor(::GetSysColor(RGB(255,0,0)));
		//pdc->FillSolidRect(&rcItem, ::GetSysColor(RGB(255,0,0)));
*/
		m_LVEdit.SetWindowText(GetItemText(htest.iItem,htest.iSubItem));

		POINT point;
		point.x = m_LVEdit.m_x;
		point.y = m_LVEdit.m_y;

		//m_LVEdit.GetCaretPos();

		//m_LVEdit.SetCaretPos(point);

		//m_LVEdit.HideCaret();

		//m_LVEdit.SetSel(-1);
		//::SendMessage(m_LVEdit.m_hWnd, EM_SETSEL, -1, 0);
		//m_LVEdit.Clear();

		//pdc->SetTextColor(crOldTextColor);	
		pdc->SetBkColor(crOldBkColor);

		m_nEdit=htest.iSubItem;		
	}
	else
		SetStyle(LVS_REPORT|LVS_OWNERDRAWFIXED);

    *pResult=0;	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CMyListBox::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

    CString sEdit=pDispInfo->item.pszText;

	if(sEdit.IsEmpty()) return;
	else
	{
		//if(!atoi(sEdit)) 

		if (atoi(sEdit) < 0) return;

		int letter_found = 0;

		for (int i=0;i<sEdit.GetLength();i++)

			if (!isdigit(sEdit[i]))
			{
				letter_found=1; break;
			}

		if (letter_found)
		{
			AfxMessageBox(m_StNonNumeric);

			return;
		}		

		SetItemText(pDispInfo->item.iItem,m_nEdit,sEdit);
	}	

	m_nEdit=-1;
    VERIFY(m_LVEdit.UnsubclassWindow()!=NULL);
	SetItemState(pDispInfo->item.iItem,0,LVNI_FOCUSED|LVNI_SELECTED);

	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CMyListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		
	SendMessage(LVM_EDITLABEL ,(WPARAM)HitTest(point, NULL),0);
	// Или так, 
	//EditLabel(HitTest(point, NULL));
}

////////////////////////////////////////////////////////////////////////////////////////////////

LPCTSTR CMyListBox::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static _TCHAR szThreeDots[] = _T("...");

	int nStringLen = lstrlen(lpszLong);

	if(nStringLen == 0 ||
		(pDC->GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	static _TCHAR szShort[MAX_PATH];

	lstrcpy(szShort,lpszLong);
	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i = nStringLen-1; i > 0; i--)
	{
		szShort[i] = 0;
		if((pDC->GetTextExtent(szShort, i).cx + nOffset + nAddLen)
			<= nColumnLen)
		{
			break;
		}
	}	

	CString Ret = lpszLong;
	CString Short = szShort;
	CString ThreeDots = szThreeDots;

	Ret = Ret.Right(Short.GetLength());
	Ret = ThreeDots + Ret;

	int rst = read_param_int("General","CheckRecognizeSTFilenames", m_IniPath);

	if (rst)	
	lstrcpy(szShort, Ret.GetBuffer(Ret.GetLength()));
	else
	lstrcat(szShort, szThreeDots);
	return(szShort);	
}

////////////////////////////////////////////////////////////////////////////////////////////////

