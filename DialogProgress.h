#if !defined(AFX_DIALOGPROGRESS_H__52857CCB_0D84_49D3_8D52_CD6701FBEAF9__INCLUDED_)
#define AFX_DIALOGPROGRESS_H__52857CCB_0D84_49D3_8D52_CD6701FBEAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DialogProgress dialog

class DialogProgress : public CDialog
{
// Construction
public:
	DialogProgress(CWnd* pParent = NULL);   // standard constructor

	int m_range;

	int m_pos;

	HWND m_hSepDlg;

	void DisplayProgress(void);

	HANDLE m_hEvent;

	HANDLE m_hStopEvent;

	int m_doubleclick;
	
	int m_num_pages;

	CString m_TextProcessed;

	CString m_TextOf;

	CString m_ProgressName;

	CString m_TextStatus;

	CString m_Cancel;

// Dialog Data
	//{{AFX_DATA(DialogProgress)
	enum { IDD = IDD_DIALOG_PROGRESS };
	CStatic	m_ProcessedPages;
	CButton	m_ButtonCancel;
	CStatic	m_StaticStatus;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DialogProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DialogProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEncCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPROGRESS_H__52857CCB_0D84_49D3_8D52_CD6701FBEAF9__INCLUDED_)
