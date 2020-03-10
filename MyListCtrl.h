#if !defined(AFX_CMYLISTCTRL_H__INCLUDED_)
#define AFX_CMYLISTCTRL_H__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();
	int m_x;
	int m_y;
	int m_cx;
	int m_cy;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CMyListBox : public CListCtrl
{
// Construction
public:
CMyListBox();
void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct);
void AddString(CString StItem);
//int AddString(LPCTSTR lpszItem);
//int AddString(LPCTSTR lpszItem, CString ListData);
void ResetContent();
void SetCurSel(int pos);
int GetCurSel();
int GetCount();
void GetItemRect(int sel, CRect* rt);
void SetStyle(DWORD NewStile);
LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);

CMyEdit	 m_LVEdit;
BOOL	 m_nEdit;
CString  m_StNonNumeric;
CString  m_IniPath;

// Generated message map functions
protected:
	//void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);	
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CMYLISTCTRL_H__INCLUDED_)
