// DialogOptions.cpp : implementation file
//

#include "stdafx.h"
#include "DjVu Sep.h"
#include "DialogOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_RANGE 120

/////////////////////////////////////////////////////////////////////////////
// DialogOptions dialog


DialogOptions::DialogOptions(CWnd* pParent /*=NULL*/)
: CDialog(DialogOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(DialogOptions)
	//}}AFX_DATA_INIT
}


void DialogOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogOptions)
	DDX_Control(pDX, IDC_CHECK_RECOGNIZE_ST_FILENAMES, m_CheckRecognizeSTFilenames);
	DDX_Control(pDX, IDC_NONE2, m_RadioNone2);
	DDX_Control(pDX, IDC_CHECK_DPI, m_CheckDpi);
	DDX_Control(pDX, IDC_COMBO_DPI, m_ComboDpi);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_ComboLanguage);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND, m_StaticBackground);
	DDX_Control(pDX, IDC_STATIC_FILE_ADDON2, m_StaticFileAddon2);
	DDX_Control(pDX, IDC_SUFFIX2, m_RadioSuffix2);	
	DDX_Control(pDX, IDC_PREFIX2, m_RadioPrefix2);
	DDX_Control(pDX, IDC_EDIT_FILE_ADDON2, m_EditFileAddon2);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_FILENAME, m_CheckCustomFilename);
	DDX_Control(pDX, IDC_CHECK_OUTPUT_DESKTOP, m_CheckOutputDesktop);
	DDX_Control(pDX, IDC_CHECK_OPEN_INSERTED, m_CheckOpenInserted);
	DDX_Control(pDX, IDC_EDIT_OUT_SUFFIX, m_EditOutSuffix);
	DDX_Control(pDX, IDC_CHECK_OPEN_CONVERTED, m_CheckOpenConverted);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogOptions, CDialog)
//{{AFX_MSG_MAP(DialogOptions)
ON_BN_CLICKED(IDC_CHECK_OPEN_CONVERTED, OnCheckOpenConverted)
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_CHECK_OPEN_INSERTED, OnCheckOpenInserted)
ON_BN_CLICKED(IDC_CHECK_OUTPUT_DESKTOP, OnCheckOutputDesktop)
ON_BN_CLICKED(IDC_CHECK_CUSTOM_FILENAME, OnCheckCustomFilename)
ON_BN_CLICKED(IDC_PREFIX2, OnPrefix2)
ON_BN_CLICKED(IDC_SUFFIX2, OnSuffix2)
ON_CBN_SELENDOK(IDC_COMBO_LANGUAGE, OnSelendokComboLanguage)
	ON_BN_CLICKED(IDC_CHECK_DPI, OnCheckDpi)
	ON_CBN_SELENDOK(IDC_COMBO_DPI, OnSelendokComboDpi)
	ON_BN_CLICKED(IDC_NONE2, OnNone2)
	ON_BN_CLICKED(IDC_CHECK_RECOGNIZE_ST_FILENAMES, OnCheckRecognizeStFilenames)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogOptions message handlers


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL DialogOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_CheckOpenConverted.SetCheck(read_param_int("General","CheckOpenConverted", m_pDlgMain->m_IniPath));
	
	m_CheckOpenInserted.SetCheck(read_param_int("General","CheckOpenInserted", m_pDlgMain->m_IniPath));	
	
	m_CheckOutputDesktop.SetCheck(read_param_int("General","CheckOutputDesktop", m_pDlgMain->m_IniPath));	
	
	m_EditOutSuffix.SetWindowText(read_param("General","EditOutSuffix", m_pDlgMain->m_IniPath));	
	
	m_EditOutSuffix.SetLimitText(4);
	
	m_EditFileAddon2.SetLimitText(4);
	
	SetAddon2();

	int state = read_param_int("General","m_addon2", m_pDlgMain->m_IniPath);

	if (state == 1)

		m_RadioPrefix2.SetCheck(1);

	if (state == 2)

		m_RadioSuffix2.SetCheck(1);

	if (state == 3)

		m_RadioNone2.SetCheck(1);

	OnNone2();
	
	///
	//  language
	//
	////////////////
	
	CLString LangItem;
	
	m_ComboLanguage.ResetContent();
	
	m_LangList.RemoveAll();
	
	m_ComboLanguage.AddString("English");
	
	LangItem.name = "";
	
	LangItem.path = "";
	
	m_LangList.Add(LangItem);
	
	
	CString Path = m_pDlgMain->m_LangPath;
	
	char path[MAX_PATH];
	
	if (Path[Path.GetLength()-1] != '\\') Path += "\\";
	
	strcpy(path,Path.GetBuffer(Path.GetLength()));
	
	strcat(path,"*.*");
	
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile (path, &fd);
	
	
	CString FilePath;	
	
	if (hFind != INVALID_HANDLE_VALUE)
	{		
		do {
			if (
				!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				&& strncmp(fd.cFileName,".",1)
				)
			{				
				FilePath = Path + fd.cFileName;
				
				if (Get_Ext(FilePath) == "ini")
				{
					m_ComboLanguage.AddString(read_param("General","Language", FilePath));
					
					LangItem.name = Get_Filename_no_ext(FilePath);
					
					LangItem.path = FilePath;
					
					m_LangList.Add(LangItem);
				}
			}
		}
		while (::FindNextFile (hFind, &fd));
		
		::FindClose (hFind);
	}
	
	m_lang_pos = 0;
	
	CString Lang = read_param("General","Language", m_pDlgMain->m_IniPath);
	
	for (int pos = 0;pos < m_LangList.GetUpperBound()+1;pos++)
	{			
		if (m_LangList[pos].name == Lang) 
		{
			m_lang_pos = pos;
			
			break;
		}
	}
	
	m_ComboLanguage.SetCurSel(m_lang_pos);

	if (read_param_int("General","CheckDpi", m_pDlgMain->m_IniPath)) 
			
		m_ComboDpi.EnableWindow();

	m_ComboDpi.AddString("100");
	m_ComboDpi.AddString("150");
	m_ComboDpi.AddString("200");
	m_ComboDpi.AddString("300");
	m_ComboDpi.AddString("400");
	m_ComboDpi.AddString("500");
	m_ComboDpi.AddString("600");
	m_ComboDpi.AddString("1200");
	
	m_ComboDpi.SetCurSel(read_param_int("General","ComboDpi", m_pDlgMain->m_IniPath));	

	state = read_param_int("General","CheckDpi", m_pDlgMain->m_IniPath);
		
	m_CheckDpi.SetCheck(state);

	state = read_param_int("General","CheckRecognizeSTFilenames", m_pDlgMain->m_IniPath);

	m_CheckRecognizeSTFilenames.SetCheck(state);

	if (state) 	
		
		m_ComboDpi.EnableWindow();
	
	else

		m_ComboDpi.EnableWindow(0);	


	if (m_pDlgMain->m_LangFilePath.IsEmpty())  // English	
		
		SetEnglish();
	
	else
		
		SetLanguage(m_pDlgMain->m_LangFilePath);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnCheckOpenConverted() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_CheckOpenConverted.GetCheck();
	
	write_param("General","CheckOpenConverted", state, m_pDlgMain->m_IniPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	
	CString OutSuffix;
	
	m_EditOutSuffix.GetWindowText(OutSuffix);
	
	write_param("General","EditOutSuffix", OutSuffix, m_pDlgMain->m_IniPath);
	
	
	CString FileAddon2;
	
	m_EditFileAddon2.GetWindowText(FileAddon2);
	
	if (FileAddon2.IsEmpty()) 
	{
		//AfxMessageBox("The background filename addon field is empty.");
		AfxMessageBox(GetUniLangString(STRING_EMPTY_BACK_ADDON));
		
		return;
	}
	
	if (!isalpha(FileAddon2[0]))
	{
		//AfxMessageBox("The background filename addon is a digit - not a string.");
		AfxMessageBox(GetUniLangString(STRING_BAD_BACK_ADDON));
		
		return;
	}
	
	write_param("General","EditFileAddon2", FileAddon2, m_pDlgMain->m_IniPath);	
	
	CDialog::OnClose();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnCheckOpenInserted() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_CheckOpenInserted.GetCheck();
	
	write_param("General","CheckOpenInserted", state, m_pDlgMain->m_IniPath);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnCheckOutputDesktop() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_CheckOutputDesktop.GetCheck();
	
	write_param("General","CheckOutputDesktop", state, m_pDlgMain->m_IniPath);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnCheckCustomFilename() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_CheckCustomFilename.GetCheck();
	
	write_param("General","CheckCustomFilename", state, m_pDlgMain->m_IniPath);
	
	if (state)
		
		EnableFileAddon(TRUE);
	
	else
		
		EnableFileAddon(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnPrefix2() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_RadioPrefix2.GetCheck();
	
	if (state)
	{
		write_param("General","m_addon2", 1, m_pDlgMain->m_IniPath);
		
		m_EditFileAddon2.EnableWindow();
	}
}

void DialogOptions::OnSuffix2() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_RadioSuffix2.GetCheck();
	
	if (state) 
	{
		write_param("General","m_addon2", 2, m_pDlgMain->m_IniPath);
		
		m_EditFileAddon2.EnableWindow();
	}
}

void DialogOptions::OnNone2() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_RadioNone2.GetCheck();
	
	if (state) 
	{
		write_param("General","m_addon2", 3, m_pDlgMain->m_IniPath);
		
		m_EditFileAddon2.EnableWindow(0);
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::SetAddon2(void)
{
	m_CheckCustomFilename.SetCheck(read_param_int("General","CheckCustomFilename", m_pDlgMain->m_IniPath));
	
	m_EditFileAddon2.SetWindowText(read_param("General","EditFileAddon2", m_pDlgMain->m_IniPath));
	
	OnCheckCustomFilename();	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::EnableFileAddon(BOOL en)
{
	m_RadioPrefix2.EnableWindow(en);
	
	m_RadioSuffix2.EnableWindow(en);

	m_RadioNone2.EnableWindow(en);
	
	m_StaticFileAddon2.EnableWindow(en);
	
	m_EditFileAddon2.EnableWindow(en);
	
	m_StaticBackground.EnableWindow(en);
	
	OnNone2();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnSelendokComboLanguage() 
{
	// TODO: Add your control notification handler code here
	
	m_lang_pos = m_ComboLanguage.GetCurSel();
	
	write_param("General","Language", m_LangList[m_lang_pos].name, m_pDlgMain->m_IniPath);
	
	if (!m_lang_pos)
	{	
		m_pDlgMain->m_LangFilePath.Empty();

		m_pDlgMain->SetEnglish();

		SetEnglish();
	}
	
	else
	{		
		m_pDlgMain->SetLanguage(m_LangList[m_lang_pos].path);

		SetLanguage(m_LangList[m_lang_pos].path);
	}

	m_pDlgMain->ShowFileSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::SetLangItem2(int id1, char* st2)
{
	CString Buf;
	
	CString Buf2(st2);
	
	Buf = read_param("DialogOptions", Buf2, m_pDlgMain->m_LangFilePath);
	
	GetDlgItem(id1)->SetWindowText(Buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::SetEngItem2(int id1, int id2)
{
	char buf[MAX_PATH] = {0};
	
	LoadString(m_pDlgMain->m_hInstance, id2, buf, sizeof(buf));
	GetDlgItem(id1)->SetWindowText(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::SetLanguage(CString LangPath)
{
	CString Buf = read_param("DialogOptions", "IDD_DIALOG_OPTIONS", m_pDlgMain->m_LangFilePath);	
	this->SetWindowText(Buf);
	
	////////////

	SetLangItem(IDC_STATIC_MAIN_OPTIONS);
	SetLangItem(IDC_CHECK_OPEN_CONVERTED);
	SetLangItem(IDC_CHECK_OPEN_INSERTED);
	SetLangItem(IDC_CHECK_OUTPUT_DESKTOP);
	SetLangItem(IDC_STATIC_OUT_SUFFIX);
	SetLangItem(IDC_CHECK_CUSTOM_FILENAME);
	SetLangItem(IDC_STATIC_BACKGROUND);
	SetLangItem(IDC_STATIC_FILE_ADDON2);
	SetLangItem(IDC_PREFIX2);
	SetLangItem(IDC_SUFFIX2);
	SetLangItem(IDC_NONE2);

	SetLangItem(IDC_CHECK_RECOGNIZE_ST_FILENAMES);	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::SetEnglish()
{
	char buf[MAX_PATH] = {0};	
	LoadString(m_pDlgMain->m_hInstance, IDD_DIALOG_OPTIONS, buf, sizeof(buf));
	this->SetWindowText(buf);

	////////////

	SetEngItem(IDC_STATIC_MAIN_OPTIONS);
	SetEngItem(IDC_CHECK_OPEN_CONVERTED);
	SetEngItem(IDC_CHECK_OPEN_INSERTED);
	SetEngItem(IDC_CHECK_OUTPUT_DESKTOP);
	SetEngItem(IDC_STATIC_OUT_SUFFIX);
	SetEngItem(IDC_CHECK_CUSTOM_FILENAME);
	SetEngItem(IDC_STATIC_BACKGROUND);
	SetEngItem(IDC_STATIC_FILE_ADDON2);
	SetEngItem(IDC_PREFIX2);
	SetEngItem(IDC_SUFFIX2);
	SetEngItem(IDC_NONE2);

	SetEngItem(IDC_CHECK_RECOGNIZE_ST_FILENAMES);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString DialogOptions::LangItem(CString st)
{
	return read_param("DialogOptions", st, m_pDlgMain->m_LangFilePath);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString DialogOptions::EngItem(int id)
{
	char buf[MAX_PATH] = {0};
	
	HINSTANCE hInstance=GetModuleHandle(NULL);
	
	LoadString(hInstance, id, buf, sizeof(buf));
	
	return CString(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString DialogOptions::GetUniLangString2(int id, char* st)
{
	CString Res;

	if (m_pDlgMain->m_LangFilePath.IsEmpty())  // English
			
		Res = EngItem(id);
		
	else
			
		Res = LangItem(st);

	return Res;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnCheckDpi() 
{
	// TODO: Add your control notification handler code here

	int state = m_CheckDpi.GetCheck();
	
	write_param("General","CheckDpi", state, m_pDlgMain->m_IniPath);

	OnSelendokComboDpi();

	if (state) 	
		
		m_ComboDpi.EnableWindow();
	
	else

		m_ComboDpi.EnableWindow(0);	
}

void DialogOptions::OnSelendokComboDpi() 
{
	// TODO: Add your control notification handler code here

	write_param("General","ComboDpi", m_ComboDpi.GetCurSel(), m_pDlgMain->m_IniPath);

	CString Buf;

	m_ComboDpi.GetLBText(m_ComboDpi.GetCurSel(), Buf);

	write_param("General","Dpi", Buf, m_pDlgMain->m_IniPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void DialogOptions::OnCheckRecognizeStFilenames() 
{
	// TODO: Add your control notification handler code here	

	int state = m_CheckRecognizeSTFilenames.GetCheck();
	
	write_param("General","CheckRecognizeSTFilenames", state, m_pDlgMain->m_IniPath);	
}

////////////////////////////////////////////////////////////////////////////////////////////////