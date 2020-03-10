#if !defined(AFX_DIALOGOPTIONS_H__AD5CD49E_71C2_4A2B_9011_5E328B4D626B__INCLUDED_)
#define AFX_DIALOGOPTIONS_H__AD5CD49E_71C2_4A2B_9011_5E328B4D626B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOptions.h : header file
//

#include "DjVu SepDlg.h"

/////////////////////////////////////////////////////////////////////////////
// DialogOptions dialog

class CLString
{
 public:
	CString name;
	CString path;	
};

typedef CArray<CLString, CLString> CLArray;


class DialogOptions : public CDialog
{
// Construction
public:
	DialogOptions(CWnd* pParent = NULL);   // standard constructor

	CDjVuSepDlg* m_pDlgMain;

	int m_addon2;

	void SetAddon2(void);

	void EnableFileAddon(BOOL en);

	CLArray m_LangList;

	int m_lang_pos;


	void SetLanguage(CString LangPath);

	void SetEnglish(void);

	void SetEngItem2(int id1, int id2);

	void SetLangItem2(int id1, char* st2);	

	CString LangItem(CString st);

	CString EngItem(int id);

	CString GetUniLangString2(int id, char* st);

// Dialog Data
	//{{AFX_DATA(DialogOptions)
	enum { IDD = IDD_DIALOG_OPTIONS };
	CButton	m_CheckRecognizeSTFilenames;
	CButton	m_RadioNone2;
	CButton	m_CheckDpi;
	CComboBox	m_ComboDpi;
	CComboBox	m_ComboLanguage;
	CButton	m_StaticBackground;
	CStatic	m_StaticFileAddon2;
	CButton	m_RadioSuffix2;
	CButton	m_RadioPrefix2;
	CEdit	m_EditFileAddon2;
	CButton	m_CheckCustomFilename;
	CButton	m_CheckOutputDesktop;
	CButton	m_CheckOpenInserted;
	CEdit	m_EditOutSuffix;
	CButton	m_CheckOpenConverted;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DialogOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DialogOptions)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckOpenConverted();
	afx_msg void OnClose();
	afx_msg void OnCheckOpenInserted();
	afx_msg void OnCheckOutputDesktop();
	afx_msg void OnCheckCustomFilename();
	afx_msg void OnPrefix2();
	afx_msg void OnSuffix2();
	afx_msg void OnSelendokComboLanguage();
	afx_msg void OnCheckDpi();
	afx_msg void OnSelendokComboDpi();
	afx_msg void OnNone2();
	afx_msg void OnCheckRecognizeStFilenames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOPTIONS_H__AD5CD49E_71C2_4A2B_9011_5E328B4D626B__INCLUDED_)
