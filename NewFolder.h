#if !defined(AFX_NEWFOLDER_H__52B4CEED_080E_48EB_B7B3_6C7996BBC4C8__INCLUDED_)
#define AFX_NEWFOLDER_H__52B4CEED_080E_48EB_B7B3_6C7996BBC4C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFolder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NewFolder dialog

class NewFolder : public CDialog
{
// Construction
public:
	NewFolder(CWnd* pParent = NULL);   // standard constructor

	CString m_StNewFolderName;

	CString m_StNewFolder;

	CString m_StCancel;

// Dialog Data
	//{{AFX_DATA(NewFolder)
	enum { IDD = IDD_DIALOG_NEW_FOLDER };
	CEdit	m_EditNewFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewFolder)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWFOLDER_H__52B4CEED_080E_48EB_B7B3_6C7996BBC4C8__INCLUDED_)
