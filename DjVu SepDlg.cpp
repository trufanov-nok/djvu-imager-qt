//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//	http://www.gnu.org/copyleft/gpl.html

//	Copyright (C) 2007-2008:
//  monday2000  monday2000@yandex.ru

// DjVu SepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DjVu Sep.h"
#include "DjVu SepDlg.h"

#include "NewFolder.h"

#include "DialogOptions.h"

#include "DialogProgress.h"

// The first chunk of 4:
#define BASE_CHUNK "70" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString StCreateNewFolder; //"Create new folder"
CString StOpenFolder; /*"Open Folder"*/
CString StChooseInputFolder;
CString StChooseOutputFolder;

CString StCancel;
CString StNewFolder;
CString StNewFolderName;

CString ErrorOnFile;
CString FitBitmaps;
CString Only_24_8Bitmaps;

CString StartFolder;

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepDlg dialog

CDjVuSepDlg::CDjVuSepDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDjVuSepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDjVuSepDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDjVuSepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDjVuSepDlg)
	DDX_Control(pDX, IDC_CHECK_BG, m_CheckBg);
	DDX_Control(pDX, IDC_STATIC_BG, m_StaticBg);
	DDX_Control(pDX, IDC_SLIDER_BG, m_SliderBg);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_StaticSize);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_ButtonReset);
	DDX_Control(pDX, IDC_BUTTON_INPUT_DJVU, m_ButtonInputDjvu);
	DDX_Control(pDX, IDC_EDIT_INPUT_DJVU, m_EditInputDjvu);
	DDX_Control(pDX, IDC_STATIC_PARAMETERS, m_StaticParameters);
	DDX_Control(pDX, IDC_STATIC_APPLY_TO, m_StaticApplyTo);
	DDX_Control(pDX, IDC_RADIO_CURRENT, m_RadioCurrent);
	DDX_Control(pDX, IDC_RADIO_ALL, m_RadioAll);
	DDX_Control(pDX, IDC_LIST_FILES, m_ListFiles);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_DJVU, m_EditOutputDjvu);
	DDX_Control(pDX, IDC_COMBO_BSF, m_ComboBsf);
	DDX_Control(pDX, IDC_CHECK_BSF, m_CheckBsf);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_ButtonView);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_ButtonRemove);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT_DJVU, m_ButtonOutputDjvu);
	DDX_Control(pDX, IDC_BUTTON_INSERT_DJVU, m_ButtonInsertDjvu);
	DDX_Control(pDX, IDC_BUTTON_CURRENT, m_ButtonCurrent);
	DDX_Control(pDX, IDC_BUTTON_CONVERT, m_ButtonConvert);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_ButtonAdd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDjVuSepDlg, CDialog)
//{{AFX_MSG_MAP(CDjVuSepDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnButtonConvert)
ON_BN_CLICKED(IDC_BUTTON_CURRENT, OnButtonCurrent)
ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
ON_BN_CLICKED(IDC_BUTTON_INSERT_DJVU, OnButtonInsertDjvu)
ON_BN_CLICKED(IDC_BUTTON_OPEN_FILES, OnButtonOpenFiles)
ON_BN_CLICKED(IDC_BUTTON_OPEN_FOLDER, OnButtonOpenFolder)
ON_BN_CLICKED(IDC_BUTTON_OPTIONS, OnButtonOptions)
ON_BN_CLICKED(IDC_BUTTON_OUTPUT_DJVU, OnButtonOutputDjvu)
ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
ON_BN_CLICKED(IDC_CHECK_BSF, OnCheckBsf)
ON_BN_CLICKED(IDC_RADIO_CURRENT, OnRadioCurrent)
ON_LBN_SELCHANGE(IDC_LIST_FILES, OnSelchangeListFiles)
ON_CBN_SELENDOK(IDC_COMBO_BSF, OnSelendokComboBsf)
ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
ON_BN_CLICKED(IDC_BUTTON_INPUT_DJVU, OnButtonInputDjvu)
ON_WM_DRAWITEM()
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_CHECK_BG, OnCheckBg)
//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepDlg message handlers


/////////////////////////////////////////////////////////////////////////////

BOOL CDjVuSepDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here	

	char lp[1000]; // the path to the folder where the program's exe file is located

	::GetModuleFileName(NULL, lp, 1000);

	CString Path = lp;

	Path = GetPath(Path);

	if (Path[Path.GetLength()-1] != '\\') Path += '\\';
	
	m_MainPath = Path;	

	
	if( chdir(m_MainPath + "help"))
	{
		AfxMessageBox("The \"help\" folder is not found.");
		exit(0);
	}
	
	m_HelpPath = m_MainPath + "help\\";	
	
	
	m_c44_path = m_MainPath + "fi_c44.exe";
	
	if(access(m_c44_path,0) == -1)
    {
		AfxMessageBox("File \"fi_c44.exe\" not found.");
		exit(0);
	}
	
	m_c44_path = "\"";
	
	m_c44_path += m_MainPath;
	
	m_c44_path = m_MainPath + "fi_c44.exe";
	
	m_c44_path += "\"";
	
	m_djvm_path = m_MainPath + "djvm.exe";
	
	if(access(m_djvm_path,0) == -1)
    {
		AfxMessageBox("File \"djvm.exe\" not found.");
		exit(0);
	}

	m_LangPath = m_MainPath + "languages\\";
	
	if( chdir(m_LangPath))
	{
		AfxMessageBox("Folder \"languages\" is not found.");
		exit(0);
	}
	
	m_IniPath = m_MainPath + "settings.ini";
	
	if(access(m_IniPath,0) == -1)
    {	// create ini-file
		WritePrivateProfileSection("General","",m_IniPath);

		write_param("General","Language", "english", m_IniPath);

		CString LPath = m_LangPath;
		
		char path2[MAX_PATH];
		
		if (LPath[LPath.GetLength()-1] != '\\') LPath += "\\";
		
		strcpy(path2,LPath.GetBuffer(LPath.GetLength()));
		
		strcat(path2,"*.*");
		
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile (path2, &fd);				
		
		CString LFilePath;
		
		if (hFind != INVALID_HANDLE_VALUE)
		{		
			do {
				if (
					!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
					&& strncmp(fd.cFileName,".",1)
					)
				{				
					LFilePath = LPath + fd.cFileName;
					
					if (Get_Ext(LFilePath) == "ini")
					{
						CString LangID;

						LangID = read_param("General", "Language_Identifier_MSDN", LFilePath);

						char *stopstring = {0};

						int lang_id = strtol(LPCSTR(LangID),&stopstring,0);					
	
						if (::GetUserDefaultLangID() == lang_id)
						// http://msdn.microsoft.com/en-us/library/dd318134(VS.85).aspx	
						// http://msdn.microsoft.com/en-us/library/dd318693(v=VS.85).aspx
						{
							write_param("General","Language", Get_Filename_no_ext(fd.cFileName), m_IniPath);

							break;
						}
					}				
				}
			}
			while (::FindNextFile (hFind, &fd));
			
			::FindClose (hFind);
		}
	}
	
	m_EditOutputDjvu.SetWindowText(read_param("General", "DjVuTargetOutputFile", m_IniPath));
	
	m_EditInputDjvu.SetWindowText(read_param("General", "DjVuTargetInputFile", m_IniPath));
	
	
	if (read_param_int("General","CheckBsf", m_IniPath))
		
		m_CheckBsf.SetCheck(TRUE);
	
	
	m_RadioAll.SetCheck(TRUE);
	
	
	m_ButtonRemove.EnableWindow(FALSE);
	
	m_StaticParameters.EnableWindow(FALSE);
	
	m_CheckBsf.EnableWindow(FALSE);
	
	m_ComboBsf.EnableWindow(FALSE);
	
	m_CheckBg.EnableWindow(FALSE);
	
	m_SliderBg.EnableWindow(FALSE);
	
	m_StaticBg.EnableWindow(FALSE);
	
	
	m_ButtonConvert.EnableWindow(FALSE);
	
	m_ButtonView.EnableWindow(FALSE);
	
	m_ButtonCurrent.EnableWindow(FALSE);
	
	m_ButtonInsertDjvu.EnableWindow(FALSE);
	
	m_EditInputDjvu.EnableWindow(FALSE);
	
	m_ButtonInputDjvu.EnableWindow(FALSE);
	
	m_EditOutputDjvu.EnableWindow(FALSE);
	
	m_ButtonOutputDjvu.EnableWindow(FALSE);
	
	m_ButtonReset.EnableWindow(FALSE);
	
	m_StaticApplyTo.EnableWindow(FALSE);
	
	m_RadioAll.EnableWindow(FALSE);
	
	m_RadioCurrent.EnableWindow(FALSE);
	
	m_ComboBsf.ResetContent();
	
	//m_ComboBsf.AddString("1");
	m_ComboBsf.AddString("2");
	m_ComboBsf.AddString("3");
	m_ComboBsf.AddString("4");
	m_ComboBsf.AddString("5");
	m_ComboBsf.AddString("6");
	m_ComboBsf.AddString("7");
	m_ComboBsf.AddString("8");
	m_ComboBsf.AddString("9");
	m_ComboBsf.AddString("10");
	m_ComboBsf.AddString("11");
	m_ComboBsf.AddString("12");
	
	m_ComboBsf.SetCurSel(0);
	
	if (read_param_int("General","CheckBg", m_IniPath))
		
		m_CheckBg.SetCheck(TRUE);
	
	
	// removing section
	WritePrivateProfileSection("Current", NULL, m_IniPath);
	
	m_ComboBsf.SetCurSel(read_param_int("General","ComboBsf", m_IniPath));
	
	if (read_param("General","CheckOpenConverted", m_IniPath) == "") // does not exist
		
		write_param("General","CheckOpenConverted", 1, m_IniPath);
	
	if (read_param("General","EditOutSuffix", m_IniPath) == "") // does not exist
		
		write_param("General","EditOutSuffix", "out", m_IniPath);
	
	if (read_param("General","CheckOpenInserted", m_IniPath) == "") // does not exist
		
		write_param("General","CheckOpenInserted", 1, m_IniPath);
	
	if (read_param("General","CheckOutputDesktop", m_IniPath) == "") // does not exist
		
		write_param("General","CheckOutputDesktop", 1, m_IniPath);	
	
	if (read_param("General","m_addon2", m_IniPath) == "") // does not exist
		
		write_param("General","m_addon2", 3, m_IniPath);	
	
	if (read_param("General","EditFileAddon2", m_IniPath) == "") // does not exist
		
		write_param("General","EditFileAddon2", "b", m_IniPath);
	
	if (read_param("General","Language", m_IniPath) == "") // does not exist
		
		write_param("General","Language", "english", m_IniPath);
	
	if (read_param("General","SliderBg", m_IniPath) == "") // does not exist
		
		write_param("General","SliderBg", 0, m_IniPath);

	if (read_param("General","CheckRecognizeSTFilenames", m_IniPath) == "") // does not exist
		
		write_param("General","CheckRecognizeSTFilenames", 1, m_IniPath);	
	
	m_StaticSize.EnableWindow(FALSE);			
	
	m_fore_flag = 0;
	
	
	m_SliderBg.SetRange(0,50);
	
	CString Buf = read_param("General","SliderBg", m_IniPath);
	
	m_StaticBg.SetWindowText(Buf);
	
	m_SliderBg.SetPos(atoi(Buf));
	

	CString Lang = read_param("General","Language", m_IniPath);			
	
	if (Lang == "english")
	{
		SetEnglish();
		
		m_LangFilePath.Empty();
	}
	
	else
	{
		CString Path = m_LangPath;
		
		char path2[MAX_PATH];
		
		if (Path[Path.GetLength()-1] != '\\') Path += "\\";
		
		strcpy(path2,Path.GetBuffer(Path.GetLength()));
		
		strcat(path2,"*.*");
		
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile (path2, &fd);				
		
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
					
					if (Get_Ext(FilePath) == "ini" && Get_Filename_no_ext(FilePath) == Lang)							
					{
						m_LangFilePath = FilePath;
						
						break;
					}				
				}
			}
			while (::FindNextFile (hFind, &fd));
			
			::FindClose (hFind);
		}
		
		SetLanguage(m_LangFilePath);
	}
	
	ShowFileSize();

	m_ListFiles.m_IniPath = m_IniPath;	

	CRect rt;

	m_ListFiles.GetClientRect(rt);

	int width_col_1 = (rt.Width()/5)*4;

	int width_col_2 = (rt.Width()/5)+2;

	m_ListFiles.InsertColumn(0,_T(m_ListColumn0),LVCFMT_LEFT,width_col_1);
	
	m_ListFiles.InsertColumn(1,_T(m_ListColumn1),LVCFMT_LEFT,width_col_2);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

////////////////////////////////////////////////////////////////////////////////////////////////

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDjVuSepDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDjVuSepDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonOpenFiles() 
{
	// TODO: Add your control notification handler code here
	
	DWORD open_flags;
	
	CString Filter;
	
	int flag_cust_ff = read_param_int("General","CheckCustomFilename", m_IniPath);
	
	CString LayerFiles;
	
	CString sep;
	
	int choice_back = read_param_int("General","m_addon2", m_IniPath);
	
	
	if (!flag_cust_ff)
	{
		CString SKBackgroundFiles = GetUniLangString(STRING_SK_BACKGROUND_FILES);
		
		//strcpy(szFilter, "Растровые файлы (*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.*.pbm)|*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.pgm|BMP файлы (*.bmp)|*.bmp|JPEG файлы (*.jpeg;*.jpg)|*.jpeg;*.jpeg|GIF файлы (*.gif)|*.gif|TIF файлы (*.tiff;*.tif)|*.tiff;*.tif|PNM файлы (*.pnm;*.ppm;*.pgm;*.pbm)|*.pnm;*.ppm;*.pgm;*.pbm|");
		
		Filter = SKBackgroundFiles + " (*.sep.*(graph))|*.sep.bmp;*.sep.jpeg;*.sep.jpg;*.sep.gif;*.sep.tiff;*.sep.tif;*.sep.pnm;*.sep.ppm;*.sep.pgm;*.sep.pbm;*.sep.png|";
	}
	
	else
	{
		sep = read_param("General","EditFileAddon2", m_IniPath);		
		
		//LayerFiles = "background files";
		
		LayerFiles = GetUniLangString(STRING_BACK_FILES);
		
		if (choice_back == 1)
			
			Filter = GetPrefixedFilter(LayerFiles, sep);		
		
		else if (choice_back == 2)
			
			Filter = GetSuffixedFilter(LayerFiles, sep);

		else if (choice_back == 3)	
		{
			//strcpy(szFilter, "Растровые файлы (*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.*.pbm)|*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.pgm|BMP файлы (*.bmp)|*.bmp|JPEG файлы (*.jpeg;*.jpg)|*.jpeg;*.jpeg|GIF файлы (*.gif)|*.gif|TIF файлы (*.tiff;*.tif)|*.tiff;*.tif|PNM файлы (*.pnm;*.ppm;*.pgm;*.pbm)|*.pnm;*.ppm;*.pgm;*.pbm|");
			Filter = m_StRasterFiles + " (*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.*.pbm)|*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.pgm|BMP ";
			Filter += m_StFiles;
			Filter += " (*.bmp)|*.bmp|JPEG ";
			Filter += m_StFiles;
			Filter += " (*.jpeg;*.jpg)|*.jpeg;*.jpeg|GIF ";
			Filter += m_StFiles;
			Filter += " (*.gif)|*.gif|TIF ";
			Filter += m_StFiles;
			Filter += " (*.tiff;*.tif)|*.tiff;*.tif|PNM ";
			Filter += m_StFiles;
			Filter += " (*.pnm;*.ppm;*.pgm;*.pbm)|*.pnm;*.ppm;*.pgm;*.pbm|";		
		}
	}	
	
	open_flags = OFN_ALLOWMULTISELECT | OFN_NOVALIDATE | OFN_FILEMUSTEXIST;
	
	
	CFileDialog fileDialog(TRUE,NULL,NULL,open_flags,Filter.GetBuffer(Filter.GetLength())); // выбор нескольких файлов
	
	
	int iBufferSize = 100000;
	
	fileDialog.m_ofn.nMaxFile = iBufferSize; // set the buffer size
	
	char* cNewBuffer = new char[iBufferSize]; // allocating memory
	
	fileDialog.m_ofn.lpstrFile = cNewBuffer; // assigning the pointer to the new place
	
	fileDialog.m_ofn.lpstrFile[0] = NULL; // nothing is available	
	
	CString Path = read_param("General","GraphFilesOpenDir", m_IniPath);
	
	fileDialog.m_ofn.lpstrInitialDir = Path.GetBuffer(Path.GetLength());
	
	int result = fileDialog.DoModal();
	
	
	if (result==IDOK)		
	{		
		POSITION ps=fileDialog.GetStartPosition();		
		
		// Get start position
		
		m_ListFiles.ResetContent();
		
		m_DFileList.RemoveAll();
		
		CString FilePath;		
		
		
		write_param("General","GraphFilesOpenDir", GetPath(fileDialog.GetPathName()), m_IniPath);
		
		while (ps)	 
		{
			FilePath = fileDialog.GetNextPathName(ps);
			
			if (IsBaseLayerFilename(FilePath, m_IniPath))
			{
				CDString elem;			
				
				elem.name = Get_FilenameExt(FilePath);
				
				elem.path = FilePath;
				
				m_DFileList.Add(elem);
			}
		}		
		// пока есть выбранные файлы		
	}
	
	delete []cNewBuffer;
	
	if (result==IDOK) CheckOpenedFiles();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::CheckOpenedFiles(void)
{
	m_DFileList.Sort();
	
	
	for (int pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
	{
		CString name(st(pos+1)); // extra
		
		name += ".  ";
		
		name += m_DFileList[pos].name;
		
		m_ListFiles.AddString(name);
	}
	
	
	m_ButtonRemove.EnableWindow(TRUE);
	
	m_StaticParameters.EnableWindow(TRUE);
	
	m_CheckBsf.EnableWindow(TRUE);
	
	m_CheckBg.EnableWindow(TRUE);
	
	m_ButtonConvert.EnableWindow(TRUE);
	
	//m_ButtonView.EnableWindow(TRUE);
	
	m_ButtonCurrent.EnableWindow(TRUE);
	
	m_ButtonInsertDjvu.EnableWindow(TRUE);
	
	m_ButtonInputDjvu.EnableWindow(TRUE);
	
	m_EditInputDjvu.EnableWindow(TRUE);	
	
	
	CString InputDjvuPath;
	
	m_EditInputDjvu.GetWindowText(InputDjvuPath);	
	
	if (IsDjvuFile(InputDjvuPath))
	{
		m_EditOutputDjvu.EnableWindow(TRUE);
		
		m_ButtonOutputDjvu.EnableWindow(TRUE);
	}
	
	
	m_ButtonReset.EnableWindow(TRUE);
	
	m_StaticApplyTo.EnableWindow(TRUE);
	
	m_RadioAll.EnableWindow(TRUE);
	
	m_RadioCurrent.EnableWindow(TRUE);
	
	
	if (read_param_int("General","CheckBsf", m_IniPath))
		
		m_ComboBsf.EnableWindow(TRUE);
	
	m_CheckBg.EnableWindow(TRUE);
	
	if (read_param_int("General","CheckBg", m_IniPath))
	{		
		m_SliderBg.EnableWindow(TRUE);
		
		m_StaticBg.EnableWindow(TRUE);
	}
	
	
	m_ListFiles.SetCurSel(0);
	
	// removing section
	WritePrivateProfileSection("Current", NULL, m_IniPath);
	
	CheckDjvuPicsReady();
	
	m_StaticSize.EnableWindow(TRUE);	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonOpenFolder() 
{
	// TODO: Add your control notification handler code here
	
	char path[MAX_PATH];
	
	BOOL res;	
	
	//NFolder.m_StNewFolderName = GetUniLangString(IDC_STATIC_NEW_FOLDER_NAME2);

	StartFolder = read_param("General", "StartFolderInput", m_IniPath);
	
	try
	{
		res = FGetDirectory(path,StChooseInputFolder.GetBuffer(StChooseInputFolder.GetLength()));
	}
	
	catch (CFileException* e) {
		//    e->ReportError ();
		e->Delete ();
	}	
	
	if (!res) return;
	
	CString Path = path;

	write_param("General", "StartFolderInput", CString(path), m_IniPath);	
	
	OpenFolder(Path);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OpenFolder(CString Path)
{ 	
	int flag_cust_ff = read_param_int("General","CheckCustomFilename", m_IniPath);
	
	CString LayerFiles;
	
	CString sep;
	
	int choice_back = read_param_int("General","m_addon2", m_IniPath);
	
	char path[MAX_PATH];
	
	if (Path[Path.GetLength()-1] != '\\') Path += "\\";
	
	strcpy(path,Path.GetBuffer(Path.GetLength()));
	
	strcat(path,"*.*");
	
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile (path, &fd);
	
	CString FilePath;
	
	
	if (hFind != INVALID_HANDLE_VALUE)
	{
		m_ListFiles.ResetContent();
		
		m_DFileList.RemoveAll();
		
		do {
			if (
				!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				&& strncmp(fd.cFileName,".",1)
				)
			{				
				FilePath = Path + fd.cFileName;				
				
				if (IsBaseLayerFilename(FilePath, m_IniPath))
				{
					CDString elem;			
					
					elem.name = Get_FilenameExt(FilePath);
					
					elem.path = FilePath;
					
					m_DFileList.Add(elem);
				}
			}
		}
		while (::FindNextFile (hFind, &fd));
		
		::FindClose (hFind);
	}
	
	int pos;
	
	if (m_DFileList.GetSize())
	{
		m_DFileList.Sort();
		
		for (pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
		{
			CString name(st(pos+1)); // extra
			
			name += ".  ";
			
			name += m_DFileList[pos].name;
			
			m_ListFiles.AddString(name);
		}
		
		m_ButtonRemove.EnableWindow(TRUE);
		
		m_StaticParameters.EnableWindow(TRUE);
		
		m_CheckBsf.EnableWindow(TRUE);
		
		m_CheckBg.EnableWindow(TRUE);		
		
		m_ButtonConvert.EnableWindow(TRUE);
		
		m_ButtonView.EnableWindow(TRUE);
		
		m_ButtonCurrent.EnableWindow(TRUE);
		
		m_ButtonInsertDjvu.EnableWindow(TRUE);
		
		
		CString InputDjvuPath;
		
		m_EditInputDjvu.GetWindowText(InputDjvuPath);	
		
		if (IsDjvuFile(InputDjvuPath))
		{
			m_EditOutputDjvu.EnableWindow(TRUE);
			
			m_ButtonOutputDjvu.EnableWindow(TRUE);
		}
		
		
		m_ButtonInputDjvu.EnableWindow(TRUE);
		
		m_EditInputDjvu.EnableWindow(TRUE);		
		
		m_ButtonReset.EnableWindow(TRUE);
		
		m_StaticApplyTo.EnableWindow(TRUE);
		
		m_RadioAll.EnableWindow(TRUE);
		
		m_RadioCurrent.EnableWindow(TRUE);
		
		if (read_param_int("General","CheckBsf", m_IniPath))
			
			m_ComboBsf.EnableWindow(TRUE);
		
		//m_CheckBg.EnableWindow(FALSE);
		
		if (read_param_int("General","CheckBg", m_IniPath))
		{			
			m_SliderBg.EnableWindow(TRUE);
			
			m_StaticBg.EnableWindow(TRUE);
		}
		
		
		m_ListFiles.SetCurSel(0);
		
		// removing section
		WritePrivateProfileSection("Current", NULL, m_IniPath);
		
		CheckDjvuPicsReady();
		
		m_StaticSize.EnableWindow(TRUE);
	}
	
}

//****************************************************************

// http://www.sources.ru/cpp/faqs/39.htm

BOOL WINAPI BrowseProc (HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam); // Описали ф-цию

#define BUTTONID 1000 // идентификатор первой кнопки

WNDPROC MainBrowseProc; // дефолтовский процесс

HINSTANCE hInst; // HINSTANCE нашей проги

CString CurrentFolder;

char sz[MAX_PATH]={0};

// **********************
TCHAR szDir[MAX_PATH]; // Имя папка будет храниться здесь

int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMessage, LPARAM wParam, LPARAM lParam)
{
	strcpy(sz, StartFolder);

    switch(uMessage)
    {
    case BFFM_INITIALIZED:
		{
			
			if(!strlen(sz))
			{
				
				LPITEMIDLIST pidl2;
				LPMALLOC lpMalloc;
				
				if (0 != SHGetSpecialFolderLocation(HWND_DESKTOP,
					//CSIDL_DESKTOP 
					CSIDL_DRIVES, &pidl2))
					return FALSE;
				if (NULL == pidl2) return FALSE;
				
				// add your initialization code here
				
				ASSERT( hWnd != NULL );
				
				SendMessage(hWnd,BFFM_SETSELECTION,(WPARAM)FALSE,(LPARAM)pidl2);
				
				if (!SHGetMalloc(&lpMalloc) && (NULL != lpMalloc))
				{
					if (NULL != pidl2)
					{
						lpMalloc->Free(pidl2);
					}
					lpMalloc->Release();
				}
				
			}
			else
			{
				SendMessage(hWnd, BFFM_SETSELECTION, 1, (LPARAM)sz);
				
				memset(sz,0,MAX_PATH);

				StartFolder.Empty();
			}
			
			// *********************************************************** 
			
			RECT R;
			GetClientRect(hWnd, &R);
			
			HWND h = CreateWindow("Button",                        
				StCreateNewFolder, //"Create new folder"
				WS_CHILDWINDOW | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_TEXT | BS_PUSHBUTTON,
				R.left + 10, R.bottom - 36, 110, 23,
				hWnd,
				(HMENU)BUTTONID,
				hInst,
				NULL); // Создали батон по созданию новой папки
			
			
			HANDLE hFont = (HANDLE)SendMessage(FindWindowEx(hWnd, NULL, "Button", NULL), 
				WM_GETFONT,
				(WPARAM)NULL,
				(LPARAM)NULL);
			
			SendMessage(h, WM_SETFONT, (LPARAM)hFont, true); // поставили ему шрифт
			
			long l = GetWindowLong(hWnd, GWL_EXSTYLE);
			l -= WS_EX_CONTEXTHELP;
			SetWindowLong(hWnd, GWL_EXSTYLE, (LONG)l); // убрали кнопку ХЕЛПа в заголовке окна
			
			MainBrowseProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)BrowseProc); // Хммм... Это тоже надо :)
			// Будем обрабатывать нажатие батонов именно там
			
			SetWindowText(hWnd, StOpenFolder /*"Open Folder"*/); // Меняем название окна
			
			break;
		}
		
    case BFFM_SELCHANGED:
		{        
			if (SHGetPathFromIDList((LPITEMIDLIST)wParam ,szDir))
			{
				CurrentFolder = szDir;
			}
			break;
		}
		
    default:
        break;
    }
    return 0;
}

/************************/


BOOL WINAPI BrowseProc (HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) // Обрабатываем здесь нажатия батонов
{
    switch (uMessage)
    {
    case WM_RBUTTONDOWN:
        return 0;
		
    case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				
			case IDOK: // Нажата кнопка ОК
				break;
			case BUTTONID: // или кнопка "создать директорию"
				
				NewFolder NFolder;
				
				NFolder.m_StNewFolderName = StNewFolderName;
				
				NFolder.m_StNewFolder = StNewFolder;
				
				NFolder.m_StCancel = StCancel;
				
				
				CString strName;
				
				if (NFolder.DoModal () == IDOK)
				{
					CString newf;
					
					NFolder.m_EditNewFolder.GetWindowText(newf);
					
					strName = CurrentFolder + "\\" + newf;
				}
				else
				{ 
					SetFocus(hWnd);
					break;
				}
				
				strcpy(sz, LPCTSTR(strName));
				
				DWORD d = CreateDirectory(sz, (LPSECURITY_ATTRIBUTES)NULL); // создаем на всякий случай директорию (вдруг юзверь отредактировал путь)
				
				if (!d) // Проверка на ошибки
				{
					d = GetLastError();
					if (d != 183)
					{
						AfxMessageBox(CString("Can't create folder ") + sz);
					}
					else if (LOWORD(wParam) == BUTTONID) // Если папка уже существует и нажата кнопка "создания папки"
					{
						AfxMessageBox(CString("The folder is existent: ") + sz);
						
						SetFocus(hWnd);
						d = SendMessage(hWnd, BFFM_SETSELECTION, 1, (LPARAM)sz);
						return 0;
					}
				}
				
				SendMessage(hWnd, WM_CLOSE,0,0);
				
				return 0;
			}
			break;
		}
		
    default:
        break;
    }
	
    return CallWindowProc(MainBrowseProc, hWnd, uMessage, wParam, lParam);
	
}

//*******************************************************************
BOOL FGetDirectory(LPTSTR szDir, LPTSTR szPrompt)
{ 
	BOOL fRet;
	TCHAR szPath[MAX_PATH];
	LPITEMIDLIST pidl;
	LPITEMIDLIST pidlRoot;
	LPMALLOC lpMalloc;
	
	BROWSEINFO bi =
	{
		NULL,	
			NULL,
			szPath,
			szPrompt,       
			BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_BROWSEFORCOMPUTER,  // Стили                        
			BrowseCallbackProc,//NULL,
			0L,
			0
	};
	
	if (0 != SHGetSpecialFolderLocation(HWND_DESKTOP,
		CSIDL_DESKTOP /*CSIDL_DRIVES*/, &pidlRoot))
		return FALSE;
	if (NULL == pidlRoot)
		return FALSE;
	bi.pidlRoot = pidlRoot;
	pidl = SHBrowseForFolder(&bi);
	if (NULL != pidl)
		fRet = SHGetPathFromIDList(pidl, szDir);
	else
		fRet = FALSE; // Get the shell's allocator to free PIDLs
	if (!SHGetMalloc(&lpMalloc) && (NULL != lpMalloc))
	{
		if (NULL != pidlRoot)
		{
			lpMalloc->Free(pidlRoot);
		}
		if (NULL != pidl)
		{
			lpMalloc->Free(pidl);
		}
		lpMalloc->Release();
	}
	return fRet;
}

////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI ThreadProgressDlg(LPVOID lpParameter)   // thread data
{
	DWORD exit=0;  //код завершения процесса
	
	CDjVuSepDlg* pDlg = (CDjVuSepDlg*)lpParameter;
	
	pDlg->ConvertToDjvu();
	
	return exit;
}

////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonConvert() 
{
	// TODO: Add your control notification handler code here
	
	if(chdir(m_MainPath + "tmp"))
		
		mkdir(m_MainPath + "tmp");
	
	CString tpath(m_MainPath + "tmp\\images");
	
	DeleteFolder(tpath);	
	
	if(mkdir(tpath))
	{
		//AfxMessageBox("The \"tmp\" folder was not created.");
		//exit(0);
	}
	
	
	m_TmpImagesPath = m_MainPath + "tmp\\images\\";
	
	m_sep_path = m_TmpImagesPath;
	
	//****************************************************************
	
	//m_DlgProg.m_TextProcessed = "Processed pages:  ";
	
	m_DlgProg.m_TextProcessed = GetUniLangString(STRING_PROCESSED_PAGES);
	
	//m_DlgProg.m_TextFrom = "  of  ";
	
	m_DlgProg.m_TextOf = GetUniLangString(STRING_OF);
	
	m_DlgProg.m_TextStatus = GetUniLangString(STRING_PROGRESS_STATUS);
	
	m_DlgProg.m_ProgressName = GetUniLangString(STRING_PROGRESS);
	
	m_DlgProg.m_Cancel = GetUniLangString(STRING_CANCEL);
	
	
	
	DWORD ThreadId;
	
	HANDLE hThrd = ::CreateThread(NULL,0,ThreadProgressDlg,this,0,&ThreadId);
	
	m_DlgProg.m_hSepDlg = this->m_hWnd;
	
	m_DlgProg.m_range = m_DFileList.GetSize();
	
	m_DlgProg.DoModal();
	
	//****************************************************************	
}

////////////////////////////////////////////////////////////////////////////////

BOOL CDjVuSepDlg::ConvertToDjvu(void) 
{	
	int res = 0;
	
	CString bsf;
	
	for (int pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
	{		
		if (::WaitForSingleObject(m_DlgProg.m_hStopEvent,1) == WAIT_OBJECT_0)
		{ // if the encoding is interrupted
			res = -2;

			break;	
		}		
		
		CString pic_path = m_DFileList[pos].path;
		
		CString djvu_path = Noned(pic_path, 0, m_IniPath);
		
		djvu_path = m_TmpImagesPath + Get_Filename_no_ext(djvu_path) + ".djvu";
		
		CString st_prof = read_param("Current",m_DFileList[pos].name, m_IniPath);
		
		
		if (GetRadioAll(st_prof)) // RadioAll
		{			
			if(read_param_int("General","CheckBg", m_IniPath))
			{				
				m_st_chunks = BASE_CHUNK;
				m_st_chunks += FormChunksString(read_param_int("General","SliderBg", m_IniPath));
			}			
			
			if (read_param_int("General","CheckBsf", m_IniPath))
				
				m_ComboBsf.GetLBText(read_param_int("General","ComboBsf", m_IniPath),bsf);
		}
		
		else // RadioCurrent
		{			
			int p1,p2,p3,p4;
			
			GetCurrent(st_prof, p1,p2,p3,p4);	
			
			if (p1)
				
				m_ComboBsf.GetLBText(p3,bsf);	
			
			if(p2)
			{
				m_st_chunks = BASE_CHUNK;
				m_st_chunks += FormChunksString(p4);
			}
		}		
		
		
		CString st_dpi;
		
		if (read_param_int("General","CheckDpi", m_IniPath))
			
			st_dpi = read_param("General","Dpi", m_IniPath);		
		
		
		res = MakeSep(m_c44_path, m_sep_path, pic_path, bsf, m_st_chunks, st_dpi, djvu_path);
		
		//if (res == -1)  m_DlgProg.m_pos = m_DlgProg.m_range;
		
		bsf.Empty();
		
		m_st_chunks.Empty();
		
		m_DlgProg.DisplayProgress();
		
		if (res == -1) break;		
	}
	
	remove(m_sep_path);
	
	
	if (::WaitForSingleObject(m_DlgProg.m_hStopEvent,1) == WAIT_OBJECT_0)
		// if the encoding is interrupted
		res = -2;

	m_DlgProg.m_ButtonCancel.SetWindowText("OK");
	
	if (res == -1)
			
		//m_DlgProg.m_StaticStatus.SetWindowText("Conversion failed.");
		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_CONVERSION_FAILED));	
	
	else if (res == -2)
		
		//m_DlgProg.m_StaticStatus.SetWindowText("Conversion stopped.");
		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_CONVERSION_STOPPED));		
		
	else
	{
		//m_DlgProg.m_StaticStatus.SetWindowText("Conversion successfully ended.");
		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_CONVERSION_SUCCESS));		
		
		CheckDjvuPicsReady();
		
		m_StaticSize.EnableWindow(TRUE);
		
		ShowFileSize();
		
		if (read_param_int("General","CheckOpenConverted", m_IniPath))
		{
			//Wait until the "OK" button is pressed
			
			::WaitForSingleObject(m_DlgProg.m_hEvent, INFINITE);			
			
			OnButtonView();		
		}
	}

	m_DlgProg.m_pos = -1;
		
	m_DlgProg.m_doubleclick = 0;
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error 
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
	CString mes("\n*** ");
	mes += FreeImage_GetFormatFromFIF(fif);
	mes += (" Format\n");
	mes += message;
	mes += (" ***\n");
	AfxMessageBox(mes);	
}

// ----------------------------------------------------------

/** Generic image loader

  @param lpszPathName Pointer to the full file name
  @param flag Optional load flag constant
  @return Returns the loaded dib if successful, returns NULL otherwise
*/

FIBITMAP* GenericLoader(const char* lpszPathName, int flag)
{	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	
	FIBITMAP* dib;
	
	fif = FreeImage_GetFileType(lpszPathName, 0);
	
	if(fif == FIF_UNKNOWN)
	{
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		// ok, let's load the file
		dib = FreeImage_Load(fif, lpszPathName, flag);
		
		// unless a bad file format, we are done !
		if (!dib)
		{
			CString mes("File \"");
			mes += lpszPathName;
			mes += ("\" not found.");
			AfxMessageBox(mes);
			
			return NULL;
		}
	}
	
	return dib;
}

////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------

/** Generic image loader

  @param lpszPathName Pointer to the full file name
  @param flag Optional load flag constant
  @return Returns the loaded dib if successful, returns NULL otherwise
*/

int GenericLoader2(const char* lpszPathName1, const char* lpszPathName2, int flag, FIBITMAP* *pdib1, FIBITMAP* *pdib2)
{	
	FREE_IMAGE_FORMAT fif1 = FIF_UNKNOWN;
	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	
	fif1 = FreeImage_GetFileType(lpszPathName1, 0);
	
	if(fif1 == FIF_UNKNOWN)
	{
		// no signature ?
		// try to guess the file format from the file extension
		fif1 = FreeImage_GetFIFFromFilename(lpszPathName1);
	}
	
	// check that the plugin has reading capabilities ...
	if((fif1 != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif1))
	{
		// ok, let's load the file
		*pdib1 = FreeImage_Load(fif1, lpszPathName1, flag);
		
		// unless a bad file format, we are done !
		if (!*pdib1)
		{
			CString mes("File \"");
			mes += lpszPathName1;
			mes += ("\" not found.");
			AfxMessageBox(mes);
			
			return NULL;
		}
	}
	
	// 2nd file
	FREE_IMAGE_FORMAT fif2 = FIF_UNKNOWN;
	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	
	fif2 = FreeImage_GetFileType(lpszPathName2, 0);
	
	if(fif2 == FIF_UNKNOWN)
	{
		// no signature ?
		// try to guess the file format from the file extension
		fif2 = FreeImage_GetFIFFromFilename(lpszPathName2);
	}
	
	// check that the plugin has reading capabilities ...
	if((fif2 != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif2))
	{
		// ok, let's load the file
		*pdib2 = FreeImage_Load(fif2, lpszPathName2, flag);
		
		// unless a bad file format, we are done !
		if (!*pdib2)
		{
			CString mes("File \"");
			mes += lpszPathName2;
			mes += ("\" not found.");
			AfxMessageBox(mes);
			
			return NULL;
		}
		
		return 1;
	}
	
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////

int MakeSep(CString csepdjvu_path, CString sep_path, CString dib_path, CString bsf,
			CString st_chunks, CString st_dpi, CString djvu_path)
{	
	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif // FREEIMAGE_LIB	
	
	// initialize your own FreeImage error handler	
	
	FreeImage_SetOutputMessage(FreeImageErrorHandler);
	
	
	CString CommandLine;
	
	CommandLine += "\"";
	
	CommandLine += csepdjvu_path;
	
	CommandLine += " ";
	
	if (!st_chunks.IsEmpty())
	{		
		CommandLine += "-slice ";
		
		CommandLine += st_chunks;
		
		CommandLine += " ";		
	}
	
	if (!st_dpi.IsEmpty())
	{
		CommandLine += "-dpi ";			
		
		CommandLine += st_dpi;
		
		CommandLine += " ";		
	}
	
	int bsfn = atoi(bsf);
	
	if (bsfn>1)
	{
		CommandLine += "-bsf ";			
		
		CommandLine += bsf;
		
		CommandLine += " ";		
	}	
	
	CommandLine += "\"";
	
	CommandLine += dib_path;
	
	CommandLine += "\" ";
	
	
	CommandLine += "\"";
	
	CommandLine += djvu_path;
	
	CommandLine += "\"";
	
	PROCESS_INFORMATION pi;
	
	STARTUPINFO si;				// структура
	ZeroMemory(&si,sizeof(STARTUPINFO));	// обнулить
	si.cb=sizeof(STARTUPINFO);			// указать размер
	::GetStartupInfo(&si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW; // http://cbuilder.ru/index.php?topic=4224.0
	
	//AfxMessageBox(CommandLine);
	
	if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
	{
		AfxMessageBox("Console utility is not launched.");
		return -1;
	}

	
	//Wait for the application to finish
	::WaitForSingleObject(pi.hProcess, INFINITE);
	
	DWORD exit=0;  //код завершения процесса
	
	::GetExitCodeProcess(pi.hProcess,&exit);
	
	int exit_code = (int)exit;
	
	if(exit_code != 0)
	{
		
		CString mes;
		
		//mes = "Error on the file \"";
		mes = ErrorOnFile;	 
		
		mes += " \"";
		
		mes += dib_path;
		
		mes += "\": \n\n";
		
		if(exit_code == -2)
			
			//mes += "Only FIT_BITMAP images are supported.";
			mes += FitBitmaps;
		
		else if (exit_code == -3)
			
			//mes += "Only 24-bit color or 8-bit greyscale images are supported.";
			mes += Only_24_8Bitmaps;
		
		AfxMessageBox(mes);
		
		return -1;
		
	}
	
	//Close the handle to the process
	::CloseHandle(pi.hProcess);
	
	//Close the handle to the thread created
	::CloseHandle(pi.hThread);
	
	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif // FREEIMAGE_LIB
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonOptions() 
{
	// TODO: Add your control notification handler code here
	
	DialogOptions DlgOpt;	
	
	DlgOpt.m_pDlgMain = this;	
	
	DlgOpt.DoModal();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::ChCurr(int num, int val)
{
	int p1,p2,p3,p4;
	
	GetCurrent(read_param("Current",m_DFileList[m_ListFiles.GetCurSel()].name, m_IniPath), p1,p2,p3,p4);
	
	if (num == 1)
		
		p1 = m_CheckBsf.GetCheck();
	
	else if (num == 2)
		
		p2 = m_CheckBg.GetCheck();
	
	else if (num == 3)
		
		p3 = m_ComboBsf.GetCurSel();
	
	else if (num == 4)
		
		p4 = m_SliderBg.GetPos();
	
	
	CString st_prof = SetCurrent(p1, p2, p3, p4);
	
	st_prof = SetRadioAll(st_prof, 0);
	
	CString st_prof_name = m_DFileList[m_ListFiles.GetCurSel()].name;
	
	write_param("Current",st_prof_name, st_prof, m_IniPath);
}
////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnCheckBsf() 
{
	// TODO: Add your control notification handler code here
	
	int state = m_CheckBsf.GetCheck();
	
	if(state)
		
		m_ComboBsf.EnableWindow();	
	
	else
		m_ComboBsf.EnableWindow(FALSE);
	
	if (!m_RadioCurrent.GetCheck())
		
		write_param("General","CheckBsf", state, m_IniPath);
	
	else
		
		ChCurr(1, state);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnSelendokComboBsf() 
{
	// TODO: Add your control notification handler code here
	
	if (!m_RadioCurrent.GetCheck())
		
		write_param("General","ComboBsf", m_ComboBsf.GetCurSel(), m_IniPath);
	
	else
		
		ChCurr(3, m_ComboBsf.GetCurSel());
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CString Buf = st(m_SliderBg.GetPos());
	
	m_StaticBg.SetWindowText(Buf);
	
	
	if (!m_RadioCurrent.GetCheck())
		
		write_param("General","SliderBg", Buf, m_IniPath);
	
	else
		
		ChCurr(4, m_SliderBg.GetPos());
	
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnCheckBg() 
{
	// TODO: Add your control notification handler code here
	int state = m_CheckBg.GetCheck();
	
	if(state)
	{
		m_SliderBg.EnableWindow();
		
		m_StaticBg.EnableWindow();
	}
	
	else
	{		
		m_SliderBg.EnableWindow(0);
		
		m_StaticBg.EnableWindow(0);
	}
	
	
	if (!m_RadioCurrent.GetCheck())
		
		write_param("General","CheckBg", state, m_IniPath);
	
	else
		
		ChCurr(2, state);	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	
	DWORD open_flags;
	
	CString Filter;
	
	int flag_cust_ff = read_param_int("General","CheckCustomFilename", m_IniPath);
	
	CString LayerFiles;
	
	CString sep;
	
	int choice_back = read_param_int("General","m_addon2", m_IniPath);
	
	
	if (!flag_cust_ff)
	{
		CString SKBackgroundFiles = GetUniLangString(STRING_SK_BACKGROUND_FILES);
		
		//strcpy(szFilter, "Растровые файлы (*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.*.pbm)|*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.pgm|BMP файлы (*.bmp)|*.bmp|JPEG файлы (*.jpeg;*.jpg)|*.jpeg;*.jpeg|GIF файлы (*.gif)|*.gif|TIF файлы (*.tiff;*.tif)|*.tiff;*.tif|PNM файлы (*.pnm;*.ppm;*.pgm;*.pbm)|*.pnm;*.ppm;*.pgm;*.pbm|");
		
		Filter = SKBackgroundFiles + " (*.sep.*(graph))|*.sep.bmp;*.sep.jpeg;*.sep.jpg;*.sep.gif;*.sep.tiff;*.sep.tif;*.sep.pnm;*.sep.ppm;*.sep.pgm;*.sep.pbm;*.sep.png|";
	}
	
	else
	{		
		sep = read_param("General","EditFileAddon2", m_IniPath);		
		
		//LayerFiles = "background files";
		LayerFiles = GetUniLangString(STRING_BACK_FILES);
		
		if (choice_back == 1)
			
			Filter = GetPrefixedFilter(LayerFiles, sep);		

		else if (choice_back == 2)
			
			Filter = GetSuffixedFilter(LayerFiles, sep);

		else if (choice_back == 3)	
		{
			//strcpy(szFilter, "Растровые файлы (*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.*.pbm)|*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.pgm|BMP файлы (*.bmp)|*.bmp|JPEG файлы (*.jpeg;*.jpg)|*.jpeg;*.jpeg|GIF файлы (*.gif)|*.gif|TIF файлы (*.tiff;*.tif)|*.tiff;*.tif|PNM файлы (*.pnm;*.ppm;*.pgm;*.pbm)|*.pnm;*.ppm;*.pgm;*.pbm|");
			Filter = m_StRasterFiles + " (*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.*.pbm)|*.bmp;*.jpeg;*.jpg;*.gif;*.tiff;*.tif;*.pnm;*.ppm;*.pgm;.pgm|BMP ";
			Filter += m_StFiles;
			Filter += " (*.bmp)|*.bmp|JPEG ";
			Filter += m_StFiles;
			Filter += " (*.jpeg;*.jpg)|*.jpeg;*.jpeg|GIF ";
			Filter += m_StFiles;
			Filter += " (*.gif)|*.gif|TIF ";
			Filter += m_StFiles;
			Filter += " (*.tiff;*.tif)|*.tiff;*.tif|PNM ";
			Filter += m_StFiles;
			Filter += " (*.pnm;*.ppm;*.pgm;*.pbm)|*.pnm;*.ppm;*.pgm;*.pbm|";		
		}		
	}
	
	open_flags = OFN_ALLOWMULTISELECT | OFN_NOVALIDATE | OFN_FILEMUSTEXIST;
	
	
	CFileDialog fileDialog(TRUE,NULL,NULL,open_flags,Filter.GetBuffer(Filter.GetLength())); // выбор нескольких файлов
	
	
	int iBufferSize = 100000;
	
	fileDialog.m_ofn.nMaxFile = iBufferSize; // set the buffer size
	
	char* cNewBuffer = new char[iBufferSize]; // allocating memory
	
	fileDialog.m_ofn.lpstrFile = cNewBuffer; // assigning the pointer to the new place
	
	fileDialog.m_ofn.lpstrFile[0] = NULL; // nothing is available
	
	CString Path = read_param("General","AddGraphFilesOpenDir", m_IniPath);
	
	fileDialog.m_ofn.lpstrInitialDir = Path.GetBuffer(Path.GetLength());
	
	int result = fileDialog.DoModal();
	
	int added = 0;
	
	CDString elem;
	
	CDSortArray temp_array;
	
	if (result==IDOK)		
	{
		POSITION ps=fileDialog.GetStartPosition();
		
		// Get start position
		
		CString FilePath;		
		
		
		Path = GetPath(fileDialog.GetPathName());
		
		CString Filename;
		
		write_param("General","AddGraphFilesOpenDir", Path, m_IniPath);
		
		CString mes;
		
		while (ps)	 
		{
			FilePath = fileDialog.GetNextPathName(ps);
			
			if (IsBaseLayerFilename(FilePath, m_IniPath))
			{				
				elem.name = Get_FilenameExt(FilePath);
				
				elem.path = FilePath;	
				
				
				m_DFileList.Add(elem);
				
				temp_array.Add(elem);
				
				added = 1;				
			}			
		}		
		// пока есть выбранные файлы
	}	
	delete []cNewBuffer;
	
	
	if (added)
	{
		m_ListFiles.ResetContent();
		
		m_DFileList.Sort();
		
		temp_array.Sort();	
		
		int p = 0;
		
		for (int pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
		{
			CString name(st(pos+1)); // extra
			
			name += ".  ";
			
			name += m_DFileList[pos].name;
			
			m_ListFiles.AddString(name);
			
			if (!m_DFileList[pos].name.CompareNoCase(temp_array[temp_array.GetUpperBound()].name))
				
				p = pos;
		}
		
		m_ListFiles.SetCurSel(p);
		
		m_ButtonRemove.EnableWindow();
		
		m_StaticParameters.EnableWindow();
		
		m_CheckBsf.EnableWindow();
		
		m_CheckBg.EnableWindow();
		
		m_ButtonConvert.EnableWindow();
		
		m_ButtonView.EnableWindow();
		
		m_ButtonCurrent.EnableWindow();
		
		m_ButtonInsertDjvu.EnableWindow();
		
		
		CString InputDjvuPath;
		
		m_EditInputDjvu.GetWindowText(InputDjvuPath);	
		
		if (IsDjvuFile(InputDjvuPath))
		{
			m_EditOutputDjvu.EnableWindow(TRUE);
			
			m_ButtonOutputDjvu.EnableWindow(TRUE);
		}
		
		
		m_ButtonInputDjvu.EnableWindow(TRUE);
		
		m_EditInputDjvu.EnableWindow(TRUE);		
		
		m_ButtonReset.EnableWindow(TRUE);
		
		m_StaticApplyTo.EnableWindow();
		
		m_RadioAll.EnableWindow();
		
		m_RadioCurrent.EnableWindow();	
		
		
		CheckDjvuPicsReady();	
		
		
		m_RadioAll.SetCheck(1);
		
		m_RadioCurrent.SetCheck(0);
		
		OnRadioAll();
		
		m_StaticSize.EnableWindow(TRUE);
	}		
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	
	if (m_ListFiles.GetCount() == 0)
	{
		AfxMessageBox("The filelist is empty.");
		
		return;	
	}
	
	int index = m_ListFiles.GetCurSel();
	
	CString mes;
	
	//mes = "Remove the file \"";
	mes = GetUniLangString(STRING_REMOVE_FILE);
	
	mes += " \"";
	
	mes += m_DFileList[index].name;
	
	mes += "\" ";
	
	//mes += "\" from list?";
	mes += GetUniLangString(STRING_FROM_LIST);
	
	int pos = 0;
	
	if (AfxMessageBox(mes, MB_OKCANCEL) == IDOK)
	{
		
		m_DFileList.RemoveAt(index);
		
		m_ListFiles.ResetContent();	
		
		
		for (pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
		{
			CString name(st(pos+1)); // extra
			
			name += ".  ";
			
			name += m_DFileList[pos].name;
			
			m_ListFiles.AddString(name);
		}
		
		if (index <= m_DFileList.GetUpperBound())
			
			m_ListFiles.SetCurSel(index);
		
		else
			
			m_ListFiles.SetCurSel(m_DFileList.GetUpperBound());
		
		
		if (m_ListFiles.GetCount() == 0)
		{
			
			m_ButtonRemove.EnableWindow(FALSE);
			
			m_StaticParameters.EnableWindow(FALSE);
			
			m_CheckBsf.EnableWindow(FALSE);
			
			m_CheckBg.EnableWindow(FALSE);
			
			m_ComboBsf.EnableWindow(FALSE);
			
			m_SliderBg.EnableWindow(FALSE);
			
			m_StaticBg.EnableWindow(FALSE);
			
			m_ButtonConvert.EnableWindow(FALSE);
			
			m_ButtonView.EnableWindow(FALSE);
			
			m_ButtonCurrent.EnableWindow(FALSE);
			
			m_ButtonInsertDjvu.EnableWindow(FALSE);
			
			m_EditOutputDjvu.EnableWindow(FALSE);
			
			m_ButtonOutputDjvu.EnableWindow(FALSE);
			
			m_ButtonInputDjvu.EnableWindow(FALSE);
			
			m_EditInputDjvu.EnableWindow(FALSE);
			
			m_ButtonReset.EnableWindow(FALSE);
			
			m_StaticApplyTo.EnableWindow(FALSE);
			
			m_RadioAll.EnableWindow(FALSE);
			
			m_RadioCurrent.EnableWindow(FALSE);
			
			
			m_RadioAll.SetCheck(1);		
			
			m_RadioCurrent.SetCheck(0);		
			
			m_CheckBsf.SetCheck(read_param_int("General", "CheckBsf", m_IniPath));
			
			m_CheckBg.SetCheck(read_param_int("General", "CheckBg", m_IniPath));
			
			int pos = read_param_int("General","ComboBsf", m_IniPath);	
			
			m_ComboBsf.SetCurSel(pos);
			
			pos = read_param_int("General","SliderBg", m_IniPath);	
			
			m_SliderBg.SetPos(pos);
			
			m_StaticBg.SetWindowText(st(pos));
			
			m_StaticSize.EnableWindow(FALSE);
		}
		
		else
			
			OnSelchangeListFiles();
		
		CheckDjvuPicsReady();		
	}
}

////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI ThreadProgressDlg2(LPVOID lpParameter)   // thread data
{
	DWORD exit=0;  //код завершения процесса
	
	CDjVuSepDlg* pDlg = (CDjVuSepDlg*)lpParameter;
	
	pDlg->InsertDjvu();
	
	return exit;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonInsertDjvu() 
{
	// TODO: Add your control notification handler code here

	//InsertDjvu();

	DWORD ThreadId;
	
	HANDLE hThrd = ::CreateThread(NULL,0,ThreadProgressDlg2,this,0,&ThreadId);
	
	m_DlgProg.m_hSepDlg = this->m_hWnd;
	
	m_DlgProg.m_range = m_DFileList.GetSize();


	//****************************************************************
	
	//m_DlgProg.m_TextProcessed = "Processed pages:  ";
	
	m_DlgProg.m_TextProcessed = GetUniLangString(STRING_PROCESSED_PAGES);
	
	//m_DlgProg.m_TextFrom = "  of  ";
	
	m_DlgProg.m_TextOf = GetUniLangString(STRING_OF);
	
	m_DlgProg.m_TextStatus = GetUniLangString(STRING_INSERTION_STATUS);
	
	m_DlgProg.m_ProgressName = GetUniLangString(STRING_INSERTION);
	
	m_DlgProg.m_Cancel = GetUniLangString(STRING_CANCEL);


	
	m_DlgProg.DoModal();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::InsertDjvu(void) 
{	
	CString InputDjvuPath;
	
	m_EditInputDjvu.GetWindowText(InputDjvuPath);
	
	if (!IsDjvuFile(InputDjvuPath))
	{
		//AfxMessageBox("No DjVu file is chosen or wrong DjVu filename.");
		
		AfxMessageBox(GetUniLangString(STRING_WRONG_DJVU));		
		
		return;	
	}
	
	if(access(InputDjvuPath,0) == -1)
    {
		CString mes("The file \"");
		
		mes += InputDjvuPath;
		
		mes += "\" is not found.";
		
		AfxMessageBox(mes);
		
		return;
	}
	
	CString OutputDjvuPath;
	
	m_EditOutputDjvu.GetWindowText(OutputDjvuPath);
	
	if (!IsDjvuFile(OutputDjvuPath))
	{
		//AfxMessageBox("Output DjVu file has wrong filename.");
		AfxMessageBox(GetUniLangString(STRING_WRONG_OUTDJVU));
		
		return;	
	}
	
	
	if( chdir(m_MainPath + "tmp\\images\\"))
	{	
		//AfxMessageBox("The \"tmp\" folder was not found.");
		//AfxMessageBox("No DjVu pics are yet created.");
		AfxMessageBox(GetUniLangString(STRING_WRONG_DJVUPICS));
		return;	
	}
	
	m_TmpImagesPath = m_MainPath + "tmp\\images\\";
	
	m_TmpFilesPath = m_MainPath + "tmp\\file\\";
	
	
	if(!chdir(m_TmpFilesPath)) DeleteFolder(m_TmpFilesPath);
	
	mkdir(m_TmpFilesPath);
	
	int res = 0;
	
	CString NewPath = OutputDjvuPath;
	
	NewPath.MakeLower();	// Active Directory is case-sensitive :-(
	
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	
	PROCESS_INFORMATION pi;
	
	STARTUPINFO si;				// структура
	ZeroMemory(&si,sizeof(STARTUPINFO));	// обнулить
	si.cb=sizeof(STARTUPINFO);			// указать размер
	::GetStartupInfo(&si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW; // http://cbuilder.ru/index.php?topic=4224.0
	
	///////////////////////////////////////////////////////////
	//
	// dumping the multi-page DjVu document structure:
	
	CString dumpfile_path = m_TmpFilesPath + "dump.txt";
	
	CString djvudump_path = m_MainPath + "djvudumpm.exe";
	
	CString CommandLine;
	
	CString CommandLine2;
	
	CommandLine += "\"";
	
	CommandLine += djvudump_path;
	
	CommandLine += "\" \"";
	
	CommandLine += InputDjvuPath;
	
	CommandLine += "\" \"";	
	
	CommandLine += dumpfile_path;
	
	CommandLine += "\"";
	
	//AfxMessageBox(CommandLine);
	
	
	if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
	{
		AfxMessageBox("Console utility is not launched.");
		
		res = -1;
		
		return;
	}
	
	//Wait for the application to finish
	::WaitForSingleObject(pi.hProcess, INFINITE);
	
	//Close the handle to the process
	::CloseHandle(pi.hProcess);
	
	//Close the handle to the thread created
	::CloseHandle(pi.hThread);	
	
	///////////////////////////////////////////////////////////
	//
	//  converting multi-page DjVu document to the indirect multi-page format:
	
	CString djvmcvt_path = "\"" + m_MainPath + "djvmcvt.exe\" -i ";
	
	CommandLine.Empty();	
	
	CommandLine += djvmcvt_path;
	
	CommandLine += " \"";
	
	CommandLine += InputDjvuPath;
	
	CommandLine += "\" \"";
	
	CommandLine += m_MainPath + "tmp\\file";
	
	CommandLine += "\" \"";
	
	CommandLine += m_TmpFilesPath;
	
	CommandLine += "index.djvu\"";
	
	//AfxMessageBox(CommandLine);
	
	
	if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
	{
		AfxMessageBox("Console utility is not launched.");
		
		res = -1;
		
		return;
	}
	
	//Wait for the application to finish
	::WaitForSingleObject(pi.hProcess, INFINITE);
	
	//Close the handle to the process
	::CloseHandle(pi.hProcess);
	
	//Close the handle to the thread created
	::CloseHandle(pi.hThread);	
	
	
	///////////////////////////////////////////////////////////
	
	CString djvumake_path = "\"" + m_MainPath + "djvumake.exe\"";
	
	///////////////////////////////////////////////////////////
	//
	// main work here:
	
	CString djvuextract_path = "\"" + m_MainPath + "djvuextract.exe\" ";
	
	CString DjvuImage_FilenameExt;	
	
	CString DjvuDumpName;
	
	CString extract_djvu;
	
	CString DjvuFile_FilenameExt;
	
	int pos;
	
	BOOL is_fg44 = 0;
	
	BOOL is_fgbz = 0;
	
	BOOL is_sjbz = 0;
	
	CString INCL_Chunk;

	int djvu_num = GetDjvuPagesNum(InputDjvuPath);	

	///////////////////////////////////////////////////////////	
	
	for (int list_pos = 0;list_pos < m_DFileList.GetUpperBound()+1;list_pos++)
	{
		if (::WaitForSingleObject(m_DlgProg.m_hStopEvent,1) == WAIT_OBJECT_0)
		{ // if the encoding is interrupted
			res = -2;
			
			break;	
		}

		CString djvu_image_path = Noned(m_DFileList[list_pos].path, 0, m_IniPath);	
		
		djvu_image_path = m_TmpImagesPath + Get_Filename_no_ext(djvu_image_path) + ".djvu";
		
		if(access(djvu_image_path,0) == -1)
		{
			CString mes("The file \"");
			
			mes += djvu_image_path;
			
			mes += "\" is not found.";
			
			AfxMessageBox(mes);

			res = -2;
			
			break;			
			//return;
		}

		//mb("Inserting page.");
		
		is_fg44 = 0;
		
		is_sjbz = 0;

		is_fgbz = 0;

		int djvu_image_width=0;

		int djvu_image_height=0;

		GetDjvuSizes(djvu_image_path, djvu_image_width, djvu_image_height);
		
		////////////////////////////
		//
		// Extract BG44 chunk from the current DjVu-pic:
		
		CommandLine.Empty();	
		
		CommandLine += djvuextract_path;
		
		CommandLine += " \"";
		
		CommandLine += djvu_image_path;
		
		CommandLine += "\" \"BG44=";
		
		CommandLine += m_TmpFilesPath;
		
		CommandLine += "BG44.cnk\"";
		
		
		//AfxMessageBox(CommandLine);
		
		if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
		{
			AfxMessageBox("Console utility is not launched.");
			
			res = -1;
			
			return;
		}
		
		//Wait for the application to finish
		::WaitForSingleObject(pi.hProcess, INFINITE);
		
		//Close the handle to the process
		::CloseHandle(pi.hProcess);
		
		//Close the handle to the thread created
		::CloseHandle(pi.hThread);
		
		if (res == -1) break;
		
		////////////////////////////
		
		DjvuImage_FilenameExt = Get_FilenameExt(djvu_image_path);
		
		DjvuImage_FilenameExt.MakeLower();
		
		int  count = 0;   
		
		int total = 0;
		
		char buf[260] = {0};
		
		char buf2[260] = {0};

		int page_node_num = 0;
		
		CString Buf;
		
		FILE *stream;
		
		if((stream = fopen(dumpfile_path, "r")) == NULL)
			return;	
		
		BOOL is_break = 0;
		
		BOOL single_page = 0;
		
		CString stDPI;

		CString stWidth;

		CString stHeight;
		
		while(!feof(stream))
		{
			memset(buf,0,260);
			
			fgets(buf, 260, stream);
			
			Buf = buf;
			
			if(ferror(stream))
			{
				mb("Read error");
				break;
			}
			// Total up actual bytes read
			total += count;			
			
			////////////////////////////
			// parsing buffer:
			
			if (Buf.Find("FORM:DJVU") != -1)
			{
				page_node_num++;

				int p1 = Buf.Find("{");
				
				int p2 = Buf.Find("}");
				
				memset(buf2,0,260);				
				
				if ((p1 == -1) && (p2 == -1)) single_page = 1; // it is a single-page DjVu-file				
				
				if (single_page)
					
					DjvuFile_FilenameExt = Get_FilenameExt(InputDjvuPath);
				
				else
				{				
					strncpy(buf2,buf+p1+1,p2-p1-1);
					
					DjvuDumpName = buf2;
					
					DjvuDumpName.MakeLower();
					
					//ConvertCP(CP_OEMCP, CP_ACP, DjvuDumpName.GetBuffer(DjvuDumpName.GetLength()));

					//int file_num = GetNumOutFilename(m_DFileList[list_pos].path, m_IniPath);					
					int file_num = atoi(m_ListFiles.GetItemText(list_pos,1));

					if (page_node_num == file_num)
											
						DjvuFile_FilenameExt = DjvuDumpName;					
					
					else continue; // the needed page is not found yet
				}
				
				// the given dump "node" is found:
				
				CommandLine.Empty();
				
				CommandLine += djvuextract_path;
				
				CommandLine += " \"";
				
				extract_djvu = m_TmpFilesPath + DjvuFile_FilenameExt;
				
				CommandLine += extract_djvu;
				
				CommandLine += "\" ";
				
				CommandLine2.Empty();
				
				while(!feof(stream))
				{
					memset(buf,0,260);
					
					fgets(buf, 260, stream);
					
					Buf = buf;
					
					if ((Buf.Find("FORM:DJVU") != -1) || (Buf.Find("FORM:DJVI") != -1))
					{// the end of the current node is found
						is_break = 1;
						
						break;
					}
					
					if (Buf.IsEmpty()) continue; // bug fixed						
					
					///////////////////////////////////
					//
					// handling chunks:				
					
					size_t i=0;
					
					for (i=0;i<strlen(buf); i++) // eating leading whitespaces
						
						if (buf[i] == ' ') continue; else break;					  
						
						
						Buf = buf+i;
						
						pos = Buf.Find(" ");
						
						memset(buf2,0,260);
						
						strncpy(buf2,Buf.GetBuffer(Buf.GetLength()),pos);
						
						Buf = buf2;							
						

						if (!Buf.CompareNoCase("INFO"))							
						{
							stDPI = buf+i;

							p1 = stDPI.Find("DjVu");
		
							stWidth = stDPI.Mid(p1+5);

							p1 = stWidth.Find(",");

							stWidth = stWidth.Left(p1);

							p1 = stWidth.Find("x");

							stHeight = stWidth.Mid(p1+1);

							stWidth = stWidth.Left(p1);

							p1 = stDPI.Find(",",stDPI.Find(",")+1);
							
							stDPI = stDPI.Right(stDPI.GetLength() - p1 - 2);
							
							p1 = stDPI.Find("d");
							
							stDPI = stDPI.Left(p1-1);
							
							continue;
						}						
						
						if (!Buf.CompareNoCase("BG44")) continue;

						if (!Buf.CompareNoCase("FG44")) is_fg44 = 1;
						
						if (!Buf.CompareNoCase("FGbz")) is_fgbz = 1;
						
						if (Buf.CompareNoCase("INCL"))
						{								
							///////////////////////////////////
							// djvuextract [-page=pagenum] djvufile [chkid=filename]... 
							
							if (!Buf.CompareNoCase("Sjbz")) is_sjbz = 1;
							
							CommandLine2 += "\"";
							
							CommandLine2 += Buf;
							
							CommandLine2 += "=";
							
							CommandLine2 += m_TmpFilesPath;
							
							CommandLine2 += Buf;
							
							CommandLine2 += ".cnk\" ";								
						}
						else
						{
							///////////////////////////////////
							// INCL processing								
							
							Buf = buf+i;							
							
							p1 = Buf.Find("{");
							
							p2 = Buf.Find("}");
							
							memset(buf2,0,260);
							
							strncpy(buf2,buf+p1+i+1,p2-p1-1);
							
							INCL_Chunk = buf2;
							
							///////////////////////////////////
						}
				}					
			}
			
			////////////////////////////
			// just for some case...				
			if (single_page) is_break = 1;						
	
			if (is_break) break;
		}
		
		fclose(stream);
		
		////////////////////////////
		//
		// djvuextact main file page:		
		
		CommandLine += CommandLine2;
		
		//AfxMessageBox(CommandLine);
		
		if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
		{
			//AfxMessageBox("Console utility is not launched.");
			
			res = -1;
			
			return;
		}
		
		//Wait for the application to finish
		::WaitForSingleObject(pi.hProcess, INFINITE);
		
		//Close the handle to the process
		::CloseHandle(pi.hProcess);
		
		//Close the handle to the thread created
		::CloseHandle(pi.hThread);
		
		if (res == -1) break;		
		
		////////////////////////////
		//
		// djvumake it back					
		
		CommandLine = djvumake_path;
		
		CommandLine += " \"";
		
		CommandLine += extract_djvu;


		if (is_sjbz == 0) // the illustration is pasted on an empty page
		{
			ChangeDPI(djvu_image_path, stDPI); // fixing the bug with different DPI's
			
			//CopyFile(djvu_image_path, extract_djvu);
			
			////////////////////////////
			//
			// Extract BG44 chunk from the current DjVu-pic:
			
			CString CommandLineBG44;
			
			CommandLineBG44 += djvuextract_path;
			
			CommandLineBG44 += " \"";
			
			CommandLineBG44 += djvu_image_path;
			
			CommandLineBG44 += "\" \"BG44=";
			
			CommandLineBG44 += m_TmpFilesPath;
			
			CommandLineBG44 += "BG44.cnk\"";
			
			
			//AfxMessageBox(CommandLineBG44);
			
			if (!::CreateProcess(NULL, CommandLineBG44.GetBuffer(CommandLineBG44.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
			{
				AfxMessageBox("Console utility is not launched.");
				
				res = -1;
				
				return;
			}
			
			//Wait for the application to finish
			::WaitForSingleObject(pi.hProcess, INFINITE);
			
			//Close the handle to the process
			::CloseHandle(pi.hProcess);
			
			//Close the handle to the thread created
			::CloseHandle(pi.hThread);
			
			if (res == -1) break;
			
			////////////////////////////

			CommandLine += "\" \"INFO=" + stWidth + "," + stHeight + ",";			
		}		
		
		else
			
			CommandLine += "\" \"INFO=,,";
		
		CommandLine += stDPI;
		
		CommandLine += "\"";
		
		CommandLine += " ";

		if (!INCL_Chunk.IsEmpty())
		{
			CommandLine += "\"INCL=";
			
			CommandLine += m_TmpFilesPath;
			
			CommandLine += INCL_Chunk;
			
			CommandLine += "\" ";
		}

		if(is_fg44) 
		{
/* bug fixed
			CommandLine += "\"FG44=";
			
			CommandLine += m_TmpFilesPath;
			
			CommandLine += "FG44.cnk\" ";
*/
		}
		
		else if (!is_fgbz && is_sjbz)
		{

			CommandLine += "\"FGbz=#";			
			
			CString color = "black";
			
			//res++;
			//if(res==1) color = "red";
			//if(res==2) color = "green";
			//if(res==3) color = "blue";
			
			CommandLine += color;
			
			CommandLine += "\" ";

		}

		CommandLine += CommandLine2;

		CommandLine += "\"BG44=";
		
		CommandLine += m_TmpFilesPath;
		
		//CommandLine += "BG44.cnk:99\"";  // djvumake bug - normally "99" is optional - not obligatory		
		CommandLine += "BG44.cnk\"";

		int file_num = atoi(m_ListFiles.GetItemText(list_pos,1));		

		if (file_num > djvu_num)
		{
			//AfxMessageBox("The number of the illustration \"" + m_DFileList[list_pos].name +

				//"\" is \"" + st(file_num) + "\" which is not within the pages range (1-"
				
				//+ st(djvu_num) + ")\n" +
				
				//"of the processed pasted-on DjVu-file." +
			
				//"\n\n" + "Processing aborted.");

			AfxMessageBox(m_StNotNumber1 + " \"" + m_DFileList[list_pos].name +

				"\" " + m_StNotNumber4 + " \"" + st(file_num) + "\" " + m_StNotNumber2
				
				+ st(djvu_num) + ")\n" + m_StNotNumber3 +
			
				"\n\n" + m_StNotEqual4);

			res = -2;
			
			break;
		}

		if (!CheckSubSizes(atoi(stWidth), atoi(stHeight), djvu_image_width, djvu_image_height))
		{
			//AfxMessageBox("The sizes of the illustration #" + st(list_pos+1) + 

				//" do not correlate with the sizes " + "\n" + 
				
				//"of the corresponding pasted-on DjVu-page." + 
				
				//"\n\n" + "Processing aborted.");			

			AfxMessageBox(m_StNotEqual1 + " \"" + m_DFileList[list_pos].name + "\" " + 

				m_StNotEqual2 + "\n" + m_StNotEqual3 +
				
				"\n\n" + m_StNotEqual4);

			res = -2;
			
			break;
		}
		
		//AfxMessageBox(CommandLine);
			
		if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
		{
			AfxMessageBox("Console utility is not launched.");
			
			res = -1;
				
			return;
		}
			
		//Wait for the application to finish
		::WaitForSingleObject(pi.hProcess, INFINITE);
			
		//Close the handle to the process
		::CloseHandle(pi.hProcess);
			
		//Close the handle to the thread created
		::CloseHandle(pi.hThread);

		m_DlgProg.DisplayProgress();
			
		if (res == -1) break;		
	}	
	
	///////////////////////////////////////////////////////////
	//
	//  converting multi-page DjVu document to the bundled multi-page format:
	
	djvmcvt_path = "\"" + m_MainPath + "djvmcvt.exe\" -b ";
	
	CommandLine.Empty();	
	
	CommandLine += djvmcvt_path;
	
	CommandLine += " \"";
	
	CommandLine += m_TmpFilesPath;
	
	CommandLine += "index.djvu\" \"";
	
	//CommandLine += NewPath;
	
	CommandLine += OutputDjvuPath;
	
	CommandLine += " \"";
	
	//AfxMessageBox(CommandLine);
	
	
	if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
	{
		AfxMessageBox("Console utility is not launched.");
		
		res = -1;
		
		return;
	}
	
	//Wait for the application to finish
	::WaitForSingleObject(pi.hProcess, INFINITE);
	
	//Close the handle to the process
	::CloseHandle(pi.hProcess);
	
	//Close the handle to the thread created
	::CloseHandle(pi.hThread);
	
	
	///////////////////////////////////////////////////////////

	if (::WaitForSingleObject(m_DlgProg.m_hStopEvent,1) == WAIT_OBJECT_0)
		// if the encoding is interrupted
		res = -2;

	
	if (res == -1)
	{
		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_WRONG_INSERT));
		
		m_DlgProg.m_ButtonCancel.SetWindowText("OK");
	}	
	else if (res == -2)
	{
		m_DlgProg.m_Progress.SetPos(m_DlgProg.m_range);

		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_INSERTION_STOPPED));
		
		m_DlgProg.m_ButtonCancel.SetWindowText("OK");
	}
	else 
	{
		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_GOOD_INSERT));
		
		m_DlgProg.m_ButtonCancel.SetWindowText("OK");

		if(read_param_int("General","CheckOpenInserted", m_IniPath))
		{
			//Wait until the "OK" button is pressed

			::WaitForSingleObject(m_DlgProg.m_hEvent, INFINITE);

			if ((long)ShellExecute(m_hWnd, "open", OutputDjvuPath, NULL, NULL, SW_MAXIMIZE) < 32)	
				
				//AfxMessageBox("Processing successfully finished.\nBut no DjVu viewer is installed on your computer.");
				AfxMessageBox(GetUniLangString(STRING_NO_VIEWER));
		}
	}
	
	m_DlgProg.m_pos = -1;
		
	m_DlgProg.m_doubleclick = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonView() 
{
	// TODO: Add your control notification handler code here
	
	if( chdir(m_MainPath + "tmp"))
	{	
		//AfxMessageBox("The \"tmp\" folder was not found.");
		AfxMessageBox("No DjVu pics are yet created.");
		return;	
	}
	
	m_TmpImagesPath = m_MainPath + "tmp\\images\\";
	
	
	CString DjvuList;
	
	for (int pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
	{
		CString pic_path = m_DFileList[pos].path;
		
		CString djvu_path = Noned(pic_path, 0, m_IniPath);
		
		djvu_path = m_TmpImagesPath + Get_Filename_no_ext(djvu_path) + ".djvu";
		
		
		if(access(djvu_path,0) == -1)
		{
			CString mes("The file \"");
			
			mes += djvu_path;
			
			mes += "\" is not found.";
			
			AfxMessageBox(mes);
			
			return;
		}
		
		DjvuList += "\"";
		
		DjvuList += djvu_path;
		
		DjvuList += "\"";
		
		DjvuList += " ";
		
		//if (res == -1) break;
	}
	
	if (!DjvuList.IsEmpty())
	{
		PROCESS_INFORMATION pi;
		
		STARTUPINFO si;				// структура
		ZeroMemory(&si,sizeof(STARTUPINFO));	// обнулить
		si.cb=sizeof(STARTUPINFO);			// указать размер
		::GetStartupInfo(&si);
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW; // http://cbuilder.ru/index.php?topic=4224.0
		
		CString DjvuBundled("\"");
		
		DjvuBundled += m_TmpImagesPath;
		
		DjvuBundled += "view.djvu\"";
		
		remove(DjvuBundled);
		
		CString CommandLine(m_djvm_path);
		
		CommandLine += " -c ";
		
		CommandLine += DjvuBundled;	
		
		CommandLine += " ";
		
		CommandLine += DjvuList;
		
		//AfxMessageBox(CommandLine);
		
		int res = 0;
		
		if (!::CreateProcess(NULL, CommandLine.GetBuffer(CommandLine.GetLength()), NULL, NULL, true, 0, NULL, NULL, &si, &pi))		
		{
			AfxMessageBox("Console utility is not launched.");
			
			res = -1;
			
			return;
		}
		
		//Wait for the application to finish
		::WaitForSingleObject(pi.hProcess, INFINITE);
		
		//Close the handle to the process
		::CloseHandle(pi.hProcess);
		
		//Close the handle to the thread created
		::CloseHandle(pi.hThread);
		
		if (res == -1)
			
			//AfxMessageBox("Processing failed.");		
			AfxMessageBox(GetUniLangString(STRING_PROCESSING_FAILED));
		
		else
		{
			if ((long)ShellExecute(m_hWnd, "open", DjvuBundled, NULL, NULL, SW_MAXIMIZE) < 32)	
				
				//AfxMessageBox("Processing successfully finished.\nBut no DjVu viewer is installed on your computer.");				
				AfxMessageBox(GetUniLangString(STRING_NO_VIEWER));
		}
		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonCurrent() 
{
	// TODO: Add your control notification handler code here
	
	int pos = m_ListFiles.GetCurSel();
	
	CString bsf;
	
	CString st_chunks;
	/*		
	CString st_prof = read_param("Current",m_DFileList[pos].name, m_IniPath);
	
	  if (GetRadioAll(st_prof)) // RadioAll
	  {			
	  if(read_param_int("General","CheckBg", m_IniPath))
	  {			
	  st_chunks = BASE_CHUNK;
	  st_chunks += "+" + st(m_SliderBg.GetPos());			
	  }			
	  
		if (read_param_int("General","CheckBsf", m_IniPath))
		
		  m_ComboBsf.GetLBText(read_param_int("General","ComboBsf", m_IniPath),bsf);			
		  }
		  
			else // RadioCurrent
			{
			
			  int p1,p2,p3,p4;
			  
				GetCurrent(st_prof, p1,p2,p3,p4);	
				
				  if (p1)
				  
					m_ComboBsf.GetLBText(p3,bsf);	
					
					  if(p2)
					  {
					  st_chunks = BASE_CHUNK;
					  st_chunks += "+" + st(p4);
					  }
					  }
	*/	
	if(m_CheckBg.GetCheck())
	{			
		st_chunks = BASE_CHUNK;
		st_chunks += FormChunksString(m_SliderBg.GetPos());			
	}			
	
	if (m_CheckBsf.GetCheck())
		
		m_ComboBsf.GetLBText(read_param_int("General","ComboBsf", m_IniPath),bsf);			
	
	
	if(chdir(m_MainPath + "tmp"))
		
		mkdir(m_MainPath + "tmp");
	
	CString tpath(m_MainPath + "tmp\\images");
	
	//DeleteFolder(tpath);	
	
	if(chdir(tpath)) mkdir(tpath);
	
	
	m_TmpImagesPath = m_MainPath + "tmp\\images\\";
	
	m_sep_path = m_TmpImagesPath;	
	
	int res = 0;	
	
	CString pic_path = m_DFileList[pos].path;
	
	CString djvu_path = Noned(pic_path, 0, m_IniPath);
	
	djvu_path = m_TmpImagesPath + Get_Filename_no_ext(djvu_path) + ".djvu";
	
	
	CString st_dpi;
	
	if (read_param_int("General","CheckDpi", m_IniPath))
		
		st_dpi = read_param("General","Dpi", m_IniPath);
	
	
	res = MakeSep(m_c44_path, m_sep_path, pic_path, bsf, st_chunks, st_dpi, djvu_path);
	
	remove(m_sep_path);
	
	
	if (res == -1)
		
		//AfxMessageBox("Conversion failed.");	
		m_DlgProg.m_StaticStatus.SetWindowText(GetUniLangString(STRING_CONVERSION_FAILED));
	
	else
	{		
		m_StaticSize.EnableWindow(TRUE);
		
		ShowFileSize();
		
		if ((long)ShellExecute(m_hWnd, "open", djvu_path, NULL, NULL, SW_MAXIMIZE) < 32)
			
			//AfxMessageBox("Processing successfully finished\n. But no DjVu viewer is installed on your computer.");	
			AfxMessageBox(GetUniLangString(STRING_NO_VIEWER));
	}
	
	CheckDjvuPicsReady();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonHelp() 
{
	// TODO: Add your control notification handler code here
	
	CString HelpPath = m_HelpPath + GetUniLangString(HELP_PATHNAME);
	
	if ((long)ShellExecute(this->m_hWnd, "open", "iexplore.exe", HelpPath, NULL, SW_MAXIMIZE) < 32)
		
		AfxMessageBox("Help file not found!");
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnRadioCurrent() 
{
	// TODO: Add your control notification handler code here
	
	CString st_prof_name = m_DFileList[m_ListFiles.GetCurSel()].name;
	
	CString st_prof = read_param("Current",st_prof_name, m_IniPath);
	
	
	if (!st_prof.IsEmpty())
	{		
		int p1,p2,p3,p4;
		
		GetCurrent(st_prof, p1,p2,p3,p4);
		
		m_CheckBsf.SetCheck(p1);
		
		m_CheckBg.SetCheck(p2);
		
		m_ComboBsf.SetCurSel(p3);
		
		if (p1) m_ComboBsf.EnableWindow(TRUE);
		
		else m_ComboBsf.EnableWindow(FALSE);
		
		
		
		m_SliderBg.SetPos(p4);
		
		CString Buf = st(p4);
		
		m_StaticBg.SetWindowText(Buf);
		
		
		if (p2)
		{
			m_SliderBg.EnableWindow(TRUE);
			
			m_StaticBg.EnableWindow(TRUE);
		}			
		
		else
		{
			m_SliderBg.EnableWindow(FALSE);
			
			m_StaticBg.EnableWindow(FALSE);
		}
		
	}	
	
	st_prof = SetCurrent(m_CheckBsf.GetCheck(), m_CheckBg.GetCheck(), m_ComboBsf.GetCurSel(), m_SliderBg.GetPos());
	
	st_prof = SetRadioAll(st_prof, 0);	
	
	write_param("Current",st_prof_name, st_prof, m_IniPath);
	
	CRect rt;
	
	m_ListFiles.GetItemRect(m_ListFiles.GetCurSel(), &rt);
	
	m_ListFiles.InvalidateRect(&rt, FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnRadioAll() 
{
	// TODO: Add your control notification handler code here
	
	m_CheckBsf.SetCheck(read_param_int("General", "CheckBsf", m_IniPath));
	
	m_CheckBg.SetCheck(read_param_int("General", "CheckBg", m_IniPath));
	
	int pos = read_param_int("General","SliderBg", m_IniPath);
	
	m_SliderBg.SetPos(pos);
	
	CString Buf = st(pos);
	
	m_StaticBg.SetWindowText(Buf);
	
	
	
	pos = read_param_int("General","ComboBsf", m_IniPath);	
	
	m_ComboBsf.SetCurSel(pos);
	
	
	OnCheckBsf();
	
	OnCheckBg();
	
	OnSelendokComboBsf();	
	
	
	CString st_prof_name = m_DFileList[m_ListFiles.GetCurSel()].name;
	
	CString st_prof = read_param("Current",st_prof_name, m_IniPath);
	
	
	if (!st_prof.IsEmpty())
	{
		st_prof = SetRadioAll(st_prof, 1);
		
		write_param("Current",st_prof_name, st_prof, m_IniPath);
	}
	
	CRect rt;
	
	m_ListFiles.GetItemRect(m_ListFiles.GetCurSel(),&rt);
	
	m_ListFiles.InvalidateRect(&rt, FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnSelchangeListFiles() 
{
	// TODO: Add your control notification handler code here
	
	CString st_prof = read_param("Current",m_DFileList[m_ListFiles.GetCurSel()].name, m_IniPath);
	
	if (GetRadioAll(st_prof)) // RadioAll
	{
		m_RadioAll.SetCheck(1);
		
		m_RadioCurrent.SetCheck(0);
		
		OnRadioAll();
	}
	else // RadioCurrent
	{	
		m_RadioAll.SetCheck(0);
		
		m_RadioCurrent.SetCheck(1);
		
		OnRadioCurrent();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::CheckDjvuPicsReady()
{
	int flag = 1;
	
	CString OutputDjvuPath;
	
	m_EditOutputDjvu.GetWindowText(OutputDjvuPath);
	
	if (!m_DFileList.GetSize())
		
		flag = 0;	
	
	for (int pos = 0;pos < m_DFileList.GetUpperBound()+1;pos++)
	{
		CString djvu_path = Noned(m_DFileList[pos].path, 0, m_IniPath);
		
		djvu_path = m_MainPath + "tmp\\images\\" + Get_Filename_no_ext(djvu_path) + ".djvu";
		
		//mb(djvu_path);
		
		if(access(djvu_path,0) == -1)
		{					
			flag = 0;
			
			break;
		}
	}
	
	
	if (flag == 1)
	{
		m_ButtonView.EnableWindow(TRUE);
		
		if (!OutputDjvuPath.IsEmpty() && IsDjvuFile(OutputDjvuPath))
			
			m_ButtonInsertDjvu.EnableWindow(TRUE);
		
		else
			
			m_ButtonInsertDjvu.EnableWindow(FALSE);
	}
	
	else
	{
		m_ButtonView.EnableWindow(FALSE);
		
		m_ButtonInsertDjvu.EnableWindow(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	
	// removing section
	WritePrivateProfileSection("Current", NULL, m_IniPath);	
	
	CRect rt;
	
	m_ListFiles.Invalidate(FALSE);
	
	m_RadioCurrent.SetCheck(FALSE);
	
	m_RadioAll.SetCheck(TRUE);
	
	OnRadioAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonInputDjvu() 
{
	// TODO: Add your control notification handler code here
	
	DWORD open_flags;
	
	CString Filter;
	
	Filter = "DjVu files (*.djvu;*.djv)|*.djvu;*.djv;|";
	
	open_flags = OFN_NOVALIDATE | OFN_FILEMUSTEXIST;
	
	
	CFileDialog fileDialog(TRUE,NULL,NULL,open_flags,Filter.GetBuffer(Filter.GetLength())); // выбор нескольких файлов
	
	CString Path = GetPath(read_param("General","DjVuTargetInputFile", m_IniPath));
	
	fileDialog.m_ofn.lpstrInitialDir = Path.GetBuffer(Path.GetLength());
	
	
	int result = fileDialog.DoModal();
	
	if (result==IDOK)
	{		
		Path = fileDialog.GetPathName();
		
		m_EditInputDjvu.SetWindowText(Path);
		
		write_param("General","DjVuTargetInputFile", Path, m_IniPath);
		
		
		m_EditOutputDjvu.EnableWindow(TRUE);
		
		m_ButtonOutputDjvu.EnableWindow(TRUE);
		
		
		CString Filename = Get_FilenameExt(Path);
		
		if (read_param_int("General","CheckOutputDesktop", m_IniPath))		
			
			Path = GetDesktopPath();	
		
		
		Path = GetPath(Path);
		
		Filename = Filename.Left(Filename.ReverseFind('.')+1);
		
		Filename += read_param("General","EditOutSuffix", m_IniPath);	
		
		Filename += ".djvu";
		
		Path += Filename;
		
		m_EditOutputDjvu.SetWindowText(Path);
		
		write_param("General","DjVuTargetOutputFile", Path, m_IniPath);
		
		CheckDjvuPicsReady();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnButtonOutputDjvu() 
{
	// TODO: Add your control notification handler code here
	
	CString Buf;
	
	m_EditInputDjvu.GetWindowText(Buf);
	
	if(access(Buf,0) == -1)
    {
		//AfxMessageBox("Input target DjVu file is not found.");
		AfxMessageBox(GetUniLangString(STRING_NO_INPUTDJVU));
		return;
	}
	
	char path[MAX_PATH];
	
	BOOL res;
	
	char ext[10]={0};

	StartFolder = read_param("General", "StartFolderOutput", m_IniPath);
	
	try
	{
		res = FGetDirectory(path,StChooseOutputFolder.GetBuffer(StChooseOutputFolder.GetLength()));
	}
	
	catch (CFileException* e) {
		//    e->ReportError ();
		e->Delete ();
	}
	
	if (!res) return;
	
	if (path[strlen(path)-1] != '\\') strcat(path,"\\");
	
	CString Path = path;
	
	Path = GetPath(Path);	
	
	CString Filename = Get_Filename_no_ext(Buf);
	
	Filename += ".";
	
	Filename += read_param("General","EditOutSuffix", m_IniPath);
	
	Filename += ".djvu";
	
	Path += Filename;
	
	m_EditOutputDjvu.SetWindowText(Path);
	
	write_param("General","DjVuTargetOutputFile", Path, m_IniPath);

	write_param("General", "StartFolderOutput", CString(path), m_IniPath);
	
	CheckDjvuPicsReady();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::ShowFileSize(void)
{
	CString TempDjvuPath = m_MainPath + "tmp\\images\\";
	
	CString FileSize;
	
	CString Filename;
	
	CString Filename2;
	
	long size = 0;
	
	char path[MAX_PATH];
	
	strcpy(path, TempDjvuPath + "*.*");		
	
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile (path, &fd);
	
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (
				(
				!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				&& strncmp(fd.cFileName,".",1)
				) 
				
				&& IsDjvuFile(fd.cFileName)
				)
			{
				Filename2 = fd.cFileName;
				
				Filename = TempDjvuPath + Filename2;
				
				if (Filename2.CompareNoCase("view.djvu"))
					
					size += GetFileSize(Filename);				
			}
		}
		while (::FindNextFile (hFind, &fd));
		
		::FindClose (hFind);
	}
	
	if (size)
	{		
		int decimal, sign;
		int precision = 2;
		double source = (double)size/1024;
		
		char* buffer;
		
		buffer = _fcvt( source, 2, &decimal, &sign );
		
		FileSize = buffer;
		
		FileSize.Insert(FileSize.GetLength() - 2,",");
		
		FileSize += " ";		
		
		CString KB = GetUniLangString(IDS_STRING_KB);
		
		FileSize += KB;
	}
	
	else
		
		FileSize.Empty();
	
	m_StaticSize.SetWindowText(FileSize);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDCtl == IDC_LIST_FILES)
	{
		
		int pos = lpDrawItemStruct->itemID;
		
		if (pos != -1)
		{
			
			CString name(st(pos+1)); // extra
			
			name += ".  ";
			
			name += m_DFileList[pos].name;
			
			CString st_prof = read_param("Current",m_DFileList[pos].name, m_IniPath);	
			
			if (GetRadioAll(st_prof)) // RadioAll
				
				name += "0";
			
			else
				
				name += "1";
			
			
			char buf[255] = {0};
			
			strcpy(buf,name.GetBuffer(name.GetLength()));
			
			lpDrawItemStruct->itemData = (unsigned long)((LPCTSTR)buf);			
		}
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CDjVuSepDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//модиф. код с http://forum.vingrad.ru/index.php?showtopic=48301

	if(pMsg->hwnd == m_ListFiles.GetSafeHwnd())
	{
		if(pMsg->message == WM_DROPFILES)
		{
			HDROP hDropInfo = (HDROP)pMsg->wParam;
			
			int nCount = ::DragQueryFile (hDropInfo, (UINT) -1, NULL, 0);
			// Find out how many files were dropped.
			
			int ResultQueryFile = 0;
			char szBuf[MAX_PATH];  //  вот такой буфер был нужен
			
			
			::DragQueryFile(hDropInfo, 0, szBuf, sizeof(szBuf));
			// получаем имя первого брошенного файла
			
			if (!IsGraphicalFile(szBuf) && (!chdir(szBuf))) // если это папка
			{
				OpenFolder(szBuf);
				
				return TRUE;
			}			
			
			int flag_cust_ff;
			
			int choice_back;
			
			CString FilePath;			
			
			BOOL flag = FALSE;
			
			
			for (int i=0; i<nCount; i++)
			{				
				memset(szBuf,strlen(szBuf),0);
				
				ResultQueryFile = ::DragQueryFile(hDropInfo, i, szBuf, sizeof(szBuf));
				
				if((ResultQueryFile > 0) && IsBaseLayerFilename(szBuf, m_IniPath))
				{	
					flag = TRUE;
					
					break;
				}
			}
			
			
			if (flag)
			{				
				///////////////////////////////////////////////////
				
				flag_cust_ff = read_param_int("General","CheckCustomFilename", m_IniPath);
				
				choice_back = read_param_int("General","m_addon2", m_IniPath);
				
				// Get start position
				
				m_ListFiles.ResetContent();
				
				m_DFileList.RemoveAll();
			}
			
			else return TRUE;
			
			
			for (i=0; i<nCount; i++)
			{				
				memset(szBuf,strlen(szBuf),0);
				
				ResultQueryFile = ::DragQueryFile(hDropInfo, i, szBuf, sizeof(szBuf));
				
				if((ResultQueryFile > 0) && IsGraphicalFile(szBuf))
				{	
					
					FilePath = szBuf;
					
					if (IsBaseLayerFilename(FilePath, m_IniPath))
					{
						CDString elem;			
						
						elem.name = Get_FilenameExt(FilePath);
						
						elem.path = FilePath;
						
						m_DFileList.Add(elem);
					}						
				}
			}
			
			CheckOpenedFiles();			 
			
			///////////////////////////////////////////////////
			
			return TRUE;
		}	
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::SetLangItem2(int id1, char* st2)
{
	CString Buf;
	
	CString Buf2(st2);
	
	Buf = read_param("DialogMain", Buf2, m_LangFilePath);
	
	GetDlgItem(id1)->SetWindowText(Buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::SetEngItem2(int id1, int id2)
{
	char buf[MAX_PATH] = {0};
	
	m_hInstance=GetModuleHandle(NULL);
	
	LoadString(m_hInstance, id2, buf, sizeof(buf));
	GetDlgItem(id1)->SetWindowText(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString CDjVuSepDlg::LangItem(CString st)
{
	return read_param("DialogMain", st, m_LangFilePath);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString CDjVuSepDlg::EngItem(int id)
{
	char buf[MAX_PATH] = {0};
	
	m_hInstance=GetModuleHandle(NULL);
	
	LoadString(m_hInstance, id, buf, sizeof(buf));
	
	return CString(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////


void CDjVuSepDlg::SetLanguage(CString LangPath)
{	
	m_LangFilePath = LangPath;
	//////////////////	
	
	SetLangItem(IDC_STATIC_INPUT);
	SetLangItem(IDC_BUTTON_OPEN_FILES);
	SetLangItem(IDC_BUTTON_OPEN_FOLDER);
	SetLangItem(IDC_STATIC_PARAMETERS);
	SetLangItem(IDC_CHECK_BG);
	SetLangItem(IDC_CHECK_BSF);
	SetLangItem(IDC_STATIC_APPLY_TO);
	SetLangItem(IDC_RADIO_ALL);
	SetLangItem(IDC_RADIO_CURRENT);
	SetLangItem(IDC_BUTTON_CONVERT);
	SetLangItem(IDC_BUTTON_VIEW);
	SetLangItem(IDC_BUTTON_CURRENT);
	SetLangItem(IDC_BUTTON_INSERT_DJVU);
	SetLangItem(IDC_BUTTON_OPTIONS);
	SetLangItem(IDC_BUTTON_HELP);
	SetLangItem(IDC_BUTTON_INPUT_DJVU);
	SetLangItem(IDC_BUTTON_OUTPUT_DJVU);
	
	StCreateNewFolder = GetUniLangString(STRING_CREATE_NEW_FOLDER); //"Create new folder"
	StOpenFolder = GetUniLangString(STRING_OPEN_FOLDER); //"Open Folder"
	StChooseInputFolder=GetUniLangString(STRING_CHOOSE_INPUT_FILES);
	StChooseOutputFolder=GetUniLangString(STRING_CHOOSE_OUTPUT_FOLDER);
	
	// "New Folder" window
	StCancel = GetUniLangString(STRING_CANCEL);
	StNewFolder = GetUniLangString(STRING_NEW_FOLDER);
	StNewFolderName = GetUniLangString(STRING_NEW_FOLDER_NAME);
	
	ErrorOnFile = GetUniLangString(STRING_ERROR_ON_FILE);
	FitBitmaps = GetUniLangString(STRING_FIT_BITMAPS);
	Only_24_8Bitmaps = GetUniLangString(STRING_ONLY_24_8_BITMAPS);

	m_StRasterFiles = GetUniLangString(STRING_RASTER_FILES);
	m_StFiles = GetUniLangString(STRING_FILES);
	m_ListColumn0 = GetUniLangString(STRING_LIST_COLUMN0);
	m_ListColumn1 = GetUniLangString(STRING_LIST_COLUMN1);

	m_ListFiles.m_StNonNumeric = GetUniLangString(STRING_NON_NUMERIC);

	char buf[MAX_PATH]={0};
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = buf;
	lvc.cchTextMax = MAX_PATH;
	
	m_ListFiles.GetColumn(0,&lvc);
	strcpy(buf,LPCSTR(m_ListColumn0));
	m_ListFiles.SetColumn(0,&lvc);	

	m_ListFiles.GetColumn(1,&lvc);
	strcpy(buf,LPCSTR(m_ListColumn1));
	m_ListFiles.SetColumn(1,&lvc);	

	m_StNotEqual1 = GetUniLangString(STRING_NOT_EQUAL1);
	m_StNotEqual2 = GetUniLangString(STRING_NOT_EQUAL2);
	m_StNotEqual3 = GetUniLangString(STRING_NOT_EQUAL3);
	m_StNotEqual4 = GetUniLangString(STRING_NOT_EQUAL4);

	m_StNotNumber1 = GetUniLangString(STRING_NOT_NUMBER1);
	m_StNotNumber2 = GetUniLangString(STRING_NOT_NUMBER2);
	m_StNotNumber3 = GetUniLangString(STRING_NOT_NUMBER3);
	m_StNotNumber4 = GetUniLangString(STRING_NOT_NUMBER4);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CDjVuSepDlg::SetEnglish()
{
	SetEngItem(IDC_STATIC_INPUT);
	SetEngItem(IDC_BUTTON_OPEN_FILES);
	SetEngItem(IDC_BUTTON_OPEN_FOLDER);
	SetEngItem(IDC_STATIC_PARAMETERS);
	SetEngItem(IDC_CHECK_BG);
	SetEngItem(IDC_CHECK_BSF);
	SetEngItem(IDC_STATIC_APPLY_TO);
	SetEngItem(IDC_RADIO_ALL);
	SetEngItem(IDC_RADIO_CURRENT);
	SetEngItem(IDC_BUTTON_CONVERT);
	SetEngItem(IDC_BUTTON_VIEW);
	SetEngItem(IDC_BUTTON_CURRENT);
	SetEngItem(IDC_BUTTON_INSERT_DJVU);
	SetEngItem(IDC_BUTTON_OPTIONS);
	SetEngItem(IDC_BUTTON_HELP);
	SetEngItem(IDC_BUTTON_INPUT_DJVU);
	SetEngItem(IDC_BUTTON_OUTPUT_DJVU);
	
	StCreateNewFolder = GetUniLangString(STRING_CREATE_NEW_FOLDER); //"Create new folder"
	StOpenFolder = GetUniLangString(STRING_OPEN_FOLDER); //"Open Folder"
	StChooseInputFolder=GetUniLangString(STRING_CHOOSE_INPUT_FILES);
	StChooseOutputFolder=GetUniLangString(STRING_CHOOSE_OUTPUT_FOLDER);
	
	// "New Folder" window
	StCancel = GetUniLangString(STRING_CANCEL);
	StNewFolder = GetUniLangString(STRING_NEW_FOLDER);
	StNewFolderName = GetUniLangString(STRING_NEW_FOLDER_NAME);
	
	ErrorOnFile = GetUniLangString(STRING_ERROR_ON_FILE);
	FitBitmaps = GetUniLangString(STRING_FIT_BITMAPS);
	Only_24_8Bitmaps = GetUniLangString(STRING_ONLY_24_8_BITMAPS);

	m_StRasterFiles = GetUniLangString(STRING_RASTER_FILES);
	m_StFiles = GetUniLangString(STRING_FILES);
	m_ListColumn0 = GetUniLangString(STRING_LIST_COLUMN0);
	m_ListColumn1 = GetUniLangString(STRING_LIST_COLUMN1);

	m_ListFiles.m_StNonNumeric = GetUniLangString(STRING_NON_NUMERIC);

	char buf[MAX_PATH]={0};
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = buf;
	lvc.cchTextMax = MAX_PATH;
	
	m_ListFiles.GetColumn(0,&lvc);
	strcpy(buf,LPCSTR(m_ListColumn0));
	m_ListFiles.SetColumn(0,&lvc);	

	m_ListFiles.GetColumn(1,&lvc);
	strcpy(buf,LPCSTR(m_ListColumn1));
	m_ListFiles.SetColumn(1,&lvc);		

	m_StNotEqual1 = GetUniLangString(STRING_NOT_EQUAL1);
	m_StNotEqual2 = GetUniLangString(STRING_NOT_EQUAL2);
	m_StNotEqual3 = GetUniLangString(STRING_NOT_EQUAL3);
	m_StNotEqual4 = GetUniLangString(STRING_NOT_EQUAL4);

	m_StNotNumber1 = GetUniLangString(STRING_NOT_NUMBER1);
	m_StNotNumber2 = GetUniLangString(STRING_NOT_NUMBER2);
	m_StNotNumber3 = GetUniLangString(STRING_NOT_NUMBER3);
	m_StNotNumber4 = GetUniLangString(STRING_NOT_NUMBER4);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString CDjVuSepDlg::GetUniLangString2(int id, char* st)
{
	CString Res;
	
	if (m_LangFilePath.IsEmpty())  // English
		
		Res = EngItem(id);
	
	else
		
		Res = LangItem(st);
	
	return Res;
}

////////////////////////////////////////////////////////////////////////////////////////////////
