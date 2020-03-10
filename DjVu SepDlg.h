// DjVu SepDlg.h : header file
//

#if !defined(AFX_DJVUSEPDLG_H__CF18CAC5_04E9_4DAA_976B_6DAC7239ED05__INCLUDED_)
#define AFX_DJVUSEPDLG_H__CF18CAC5_04E9_4DAA_976B_6DAC7239ED05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <process.h>
#include <direct.h>
#include "DialogProgress.h"

#include "FreeImage.h"

BOOL FGetDirectory(LPTSTR szDir, LPTSTR szPrompt);

int FI_ConvertRGB24To_Color_RLE(FIBITMAP* dib1, FIBITMAP* dib2, int bsf, CString sep_path);
int FI_ConvertGrey8To_Color_RLE(FIBITMAP* dib1, FIBITMAP* dib2, int bsf, CString sep_path);
int FI_ConvertTo_BW_RLE(FIBITMAP* dib1, FIBITMAP* dib2, int bsf, CString sep_path);

#include "MyListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepDlg dialog

class CDjVuSepDlg : public CDialog
{
// Construction
public:
	CDjVuSepDlg(CWnd* pParent = NULL);	// standard constructor

	CString m_MainPath;
	CString m_c44_path;
	CString m_IniPath;
	CString m_TmpImagesPath;
	CString m_TmpFilesPath;
	CString m_sep_path;
	CString m_djvm_path;
	CString m_HelpPath;

	CString	m_StRasterFiles;
	CString	m_StFiles;

	HINSTANCE m_hInstance;	
	CDSortArray m_DFileList;

	DialogProgress m_DlgProg;

	CString m_StProgress;
	CString m_StProcessingInProgress;

	CString m_ListColumn0;	
	CString m_ListColumn1;

	CString m_StNotEqual1;
	CString m_StNotEqual2;
	CString m_StNotEqual3;
	CString m_StNotEqual4;

	CString m_StNotNumber1;
	CString m_StNotNumber2;
	CString m_StNotNumber3;
	CString m_StNotNumber4;

	void ChCurr(int num, int val);

	void CheckDjvuPicsReady();

	void ShowFileSize(void);

	BOOL ConvertToDjvu(void);

	CString m_st_chunks;

	int m_fore_flag;	

	void OpenFolder(CString Path);
	
	void CheckOpenedFiles(void);

	CString m_LangFilePath;

	CString m_LangPath;

	
	void SetLanguage(CString LangPath);

	void SetEnglish(void);

	void SetEngItem2(int id1, int id2);

	void SetLangItem2(int id1, char* st2);

	CString LangItem(CString st);

	CString EngItem(int id);

	CString GetUniLangString2(int id, char* st);

	void InsertDjvu(void);

// Dialog Data
	//{{AFX_DATA(CDjVuSepDlg)
	enum { IDD = IDD_DJVUSEP_DIALOG };
	CButton	m_CheckBg;
	CStatic	m_StaticBg;
	CSliderCtrl	m_SliderBg;
	CStatic	m_StaticSize;
	CButton	m_ButtonReset;
	CButton	m_ButtonInputDjvu;
	CEdit	m_EditInputDjvu;
	CButton	m_StaticParameters;
	CButton	m_StaticApplyTo;
	CButton	m_RadioCurrent;
	CButton	m_RadioAll;
	CMyListBox	m_ListFiles;
	CEdit	m_EditOutputDjvu;
	CComboBox	m_ComboBsf;
	CButton	m_CheckBsf;
	CButton	m_ButtonView;
	CButton	m_ButtonRemove;
	CButton	m_ButtonOutputDjvu;
	CButton	m_ButtonInsertDjvu;
	CButton	m_ButtonCurrent;
	CButton	m_ButtonConvert;
	CButton	m_ButtonAdd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDjVuSepDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDjVuSepDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonConvert();
	afx_msg void OnButtonCurrent();
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonInsertDjvu();
	afx_msg void OnButtonOpenFiles();
	afx_msg void OnButtonOpenFolder();
	afx_msg void OnButtonOptions();
	afx_msg void OnButtonOutputDjvu();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonView();
	afx_msg void OnCheckBsf();
	afx_msg void OnRadioCurrent();
	afx_msg void OnSelchangeListFiles();
	afx_msg void OnSelendokComboBsf();
	afx_msg void OnRadioAll();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonInputDjvu();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckBg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DJVUSEPDLG_H__CF18CAC5_04E9_4DAA_976B_6DAC7239ED05__INCLUDED_)
