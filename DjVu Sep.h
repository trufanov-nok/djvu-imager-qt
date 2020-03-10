// DjVu Sep.h : main header file for the DJVU SEP application
//

#if !defined(AFX_DJVUSEP_H__706DF8E4_27C9_478D_85B9_F0040722E6A5__INCLUDED_)
#define AFX_DJVUSEP_H__706DF8E4_27C9_478D_85B9_F0040722E6A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "afxtempl.h"

#include <fcntl.h>

#include "FreeImage.h"

// concatenation:
#define SetEngItem(x) SetEngItem2(x, x ## 2) 

// string evaluation:
#define SetLangItem(x) SetLangItem2(x, (#x))

// language-independent identifier
#define GetUniLangString(x) GetUniLangString2(x, (#x))

char* get_ext(char* filename, char* ext);
int IsGraphicalFile(char* filename);
int IsGraphicalFile(CString filename);
CString Get_FilenameExt(CString Filename);
CString Get_Filename_no_ext(CString Filename);
CString Get_Ext(CString Filename);
CString Cut_Ext(CString Filename);
CString Prefixed(CString FilePath, BOOL fore_basis, CString m_IniPath);
CString Suffixed(CString FilePath, BOOL fore_basis, CString IniPath);
CString Noned(CString FilePath, BOOL fore_basis, CString IniPath);
CString GetLayerFilename(CString FilePath, int choice, int layer, CString IniPath);
BOOL IsBaseLayerFilename(CString FilePath, CString IniPath);

class CDString
{
 public:
	CString name;
	CString path;
};

typedef CArray<CDString, CDString> CDArray;

class CDSortArray : public CDArray {
public:
   void Sort();
   int FindString(CString fst);
private:
   BOOL CompareAndSwap(int pos);
};


CString st(long n);

int Is_SK_BackgroundFile(char* filename);
int Is_SK_BackgroundFile(CString filename);
extern int MakeSep(CString csepdjvu_path, CString sep_path, CString dib_path, CString bsf, CString st_chunks, CString st_dpi, CString djvu_path);
CString GetPath(CString Filename);
CString read_param(CString section, CString param, CString ini_path);
int read_param_int(CString section, CString param, CString ini_path);
void write_param(CString section, CString param, CString value, CString ini_path);
void write_param(CString section, CString param, int value, CString ini_path);

void mb(char* mes);
void mb(CString mes);
void mb(long mes);

int IsDjvuFile(char* filename);
int IsDjvuFile(CString filename);
CString GetDjvuNum(CString filename);
int DeleteFolder(CString Path);
int ParseProfile(CString StProfile, int& p1, int& p2, int& p3, int& p4);
CString SetCurrent(int p1, int p2, int p3, int p4);
void GetCurrent(CString St, int& p1, int& p2, int& p3, int& p4);
CString SetRadioAll(CString St, int p);
int GetRadioAll(CString St);
int CopyFile(CString Path, CString NewPath);
CString GetDesktopPath(void);
long GetFileSize(CString Path);
CString GetSuffixedFilter(CString LayerFiles, CString sep);
CString GetPrefixedFilter(CString LayerFiles, CString sep);
CString FormChunksString(int val);
BOOL ConvertCP(UINT uiIN, UINT uiOUT, LPSTR lpString);
int ChangeDPI(CString InPath, CString StDPI);
int GetNumOutFilename(CString FileName, CString IniPath);
int GetDjvuSizes(CString InPath, int& djvu_image_width, int& djvu_image_height);
BOOL CheckSubSizes(int w, int h, int sw, int sh);
int GetDjvuPagesNum(CString DjvuPath);

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepApp:
// See DjVu Sep.cpp for the implementation of this class
//

class CDjVuSepApp : public CWinApp
{
public:
	CDjVuSepApp();	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDjVuSepApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDjVuSepApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DJVUSEP_H__706DF8E4_27C9_478D_85B9_F0040722E6A5__INCLUDED_)
