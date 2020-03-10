// DjVu Sep.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DjVu Sep.h"
#include "DjVu SepDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepApp

BEGIN_MESSAGE_MAP(CDjVuSepApp, CWinApp)
	//{{AFX_MSG_MAP(CDjVuSepApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepApp construction

CDjVuSepApp::CDjVuSepApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDjVuSepApp object

CDjVuSepApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDjVuSepApp initialization

BOOL CDjVuSepApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CDjVuSepDlg dlg;
	m_pMainWnd = &dlg;

	//if (::GetUserDefaultLangID() == 0x0419) // Russian
	// http://msdn.microsoft.com/en-us/library/dd318134(VS.85).aspx
	// http://msdn.microsoft.com/en-us/library/dd318693(v=VS.85).aspx

	//mb("Russian");

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//***********************************************************************************************

// http://netcode.ru/cpp/?lang=&katID=6&skatID=44&artID=566

// Класс для "массив строк с пузырьковой сортировкой".
// Применяется для сортировки имён файлов, полученных функциями
// FindFirstFile, FindNextFile

void CDSortArray::Sort()
{
   BOOL bNotDone = TRUE;

   while (bNotDone)
   {
      bNotDone = FALSE;
      for(int pos = 0;pos < GetUpperBound();pos++)
         bNotDone |= CompareAndSwap(pos);
   }
}


BOOL CDSortArray::CompareAndSwap(int pos)
{
   CDString temp;
   int posFirst = pos;
   int posNext = pos + 1;

   if (GetAt(posFirst).name.CompareNoCase(GetAt(posNext).name) > 0)
   {
      temp = GetAt(posFirst);
      SetAt(posFirst, GetAt(posNext));
      SetAt(posNext, temp);
      return TRUE;

   }
   return FALSE;
}


int CDSortArray::FindString(CString fst)
{
	for(int pos = 0;pos < GetUpperBound()+1;pos++)
		if (GetAt(pos).name.CompareNoCase(fst) == 0)		

			return pos;
   
   return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////

char* get_ext(char* filename, char* ext)
{
	for (size_t i=strlen(filename)-1; i>0; i--)
		
		if(filename[i]=='.') 
		{
			strcpy(ext,filename+i+1);
			
			break;			
		}
		return strlwr(ext); // приводим строку с расширением к нижнему регистру
}

////////////////////////////////////////////////////////////////////////////////////////////////

int IsGraphicalFile(char* filename)
{

char ext[10]={0};

if (
	!strcmp(get_ext(filename,ext),"bmp")||
	!strcmp(get_ext(filename,ext),"jpeg")||
	!strcmp(get_ext(filename,ext),"jpg")||
	!strcmp(get_ext(filename,ext),"gif")||
	!strcmp(get_ext(filename,ext),"tiff")||
	!strcmp(get_ext(filename,ext),"tif")||
	!strcmp(get_ext(filename,ext),"pnm")||
	!strcmp(get_ext(filename,ext),"ppm")||
	!strcmp(get_ext(filename,ext),"pgm")||
	!strcmp(get_ext(filename,ext),"pbm")
   )

    return 1;

	else return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int IsGraphicalFile(CString filename)
{
return IsGraphicalFile(filename.GetBuffer(filename.GetLength()));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString st(long n)
{
char buf[255] = {0};

itoa(n,buf,10);

CString res(buf);

return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void mb(char* mes)
{
AfxMessageBox(mes);
}

void mb(CString mes)
{
AfxMessageBox(mes);
}

void mb(long mes)
{
AfxMessageBox(st(mes));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString Get_FilenameExt(CString Filename)
{
return Filename.Right(Filename.GetLength()-Filename.ReverseFind('\\')-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString Get_Ext(CString Filename)
{
return Filename.Right(Filename.GetLength()-Filename.ReverseFind('.')-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString Cut_Ext(CString Filename)
{
return Filename.Left(Filename.ReverseFind('.'));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString Get_Filename_no_ext(CString Filename)
{
	int pos = Filename.ReverseFind('\\');

	if (pos != -1)

		Filename = Filename.Mid(pos+1);

	pos = Filename.ReverseFind('.');

	if (pos != -1)

		Filename = Filename.Left(pos);

	return Filename;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString GetPath(CString Filename)
{
if (Filename.ReverseFind('.') > Filename.ReverseFind('\\'))

return Filename.Left(Filename.ReverseFind('\\')+1);

else
{

if (Filename.GetAt(Filename.GetLength()-1) != '\\') Filename += "\\";

return Filename;
}
}

////////////////////////////////////////////////////////////////////////////////////////////////

int Is_SK_BackgroundFile(char* filename)
{

CString St(filename);

St.MakeLower();

if (St.Find(".sep.") != -1)

return 1;

else return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int Is_SK_BackgroundFile(CString filename)
{

filename.MakeLower();

if (filename.Find(".sep.") != -1)

return 1;

else return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////

CString Prefixed(CString FilePath, BOOL fore_basis, CString IniPath)
{
	CString Path = GetPath(FilePath);

	CString Ext = Get_Ext(FilePath);

	CString Filename_no_ext = Get_Filename_no_ext(FilePath);

	CString FileAddon;

	CString FileAddonFore = read_param("General","EditFileAddon", IniPath);

	CString FileAddonBack = read_param("General","EditFileAddon2", IniPath);
	

	if (!fore_basis)
	{
		Filename_no_ext.Replace(FileAddonBack,"");

		Filename_no_ext.Replace(".","");

		FileAddon = FileAddonFore;
	}

	else
	{
		Filename_no_ext.Replace(FileAddonFore,"");

		Filename_no_ext.Replace(".","");

		FileAddon = FileAddonBack;
	}

	return Path + FileAddon + "." + Filename_no_ext + "." + Ext;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString Suffixed(CString FilePath, BOOL fore_basis, CString IniPath)
{
	CString Path = GetPath(FilePath);

	CString Ext = Get_Ext(FilePath);

	CString Filename_no_ext = Get_Filename_no_ext(FilePath);

	CString FileAddon;

	CString FileAddonFore = read_param("General","EditFileAddon", IniPath);

	CString FileAddonBack = read_param("General","EditFileAddon2", IniPath);
	

	if (!fore_basis)
	{
		Filename_no_ext.Replace(FileAddonBack,"");

		Filename_no_ext.Replace(".","");

		FileAddon = FileAddonFore;
	}

	else
	{
		Filename_no_ext.Replace(FileAddonFore,"");

		Filename_no_ext.Replace(".","");

		FileAddon = FileAddonBack;
	}	

	return Path + Filename_no_ext + "." + FileAddon + "." + Ext;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString Noned(CString FilePath, BOOL fore_basis, CString IniPath)
{
	CString Path = GetPath(FilePath);

	CString Ext = Get_Ext(FilePath);	

	CString Filename_no_ext = Get_Filename_no_ext(FilePath);

	CString FileAddonFore = read_param("General","EditFileAddon", IniPath);
	
	CString FileAddonBack = read_param("General","EditFileAddon2", IniPath);	

	
	int flag_cust_ff = read_param_int("General","CheckCustomFilename", IniPath);		
	
	if (!flag_cust_ff) // SK 5.91 notation

		FileAddonBack = "sep";

	int choice = read_param_int("General","m_addon2", IniPath);

	if (choice != 3)
	{
		if (!fore_basis)
		{
			Filename_no_ext.Replace(FileAddonBack,"");
			
			Filename_no_ext.Replace(".","");			
		}
		
		else
		{
			Filename_no_ext.Replace(FileAddonFore,"");
			
			Filename_no_ext.Replace(".","");	
		}
	}

	return Path + Filename_no_ext + "." + Ext;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CString GetLayerFilename(CString FilePath, int choice, int layer, CString IniPath)
{
	CString Res;
	
	if (choice == 1) // Prefix
		
		Res = Prefixed(FilePath, layer, IniPath);
	
	else if (choice == 2) // Suffix
		
		Res = Suffixed(FilePath,layer, IniPath);
	
	else if (choice == 3) // Noned	
		
		Res = Noned(FilePath, layer, IniPath);

	return Res;	
}

////////////////////////////////////////////////////////////////////////////////////////////////

BOOL IsBaseLayerFilename(CString FilePath, CString IniPath)
{
	if (!IsGraphicalFile(FilePath)) return FALSE;	

	int flag_cust_ff = read_param_int("General","CheckCustomFilename", IniPath);	
	
	if (!flag_cust_ff) // SK 5.91 notation
	
		return Is_SK_BackgroundFile(FilePath);

		
	CString Filename_no_ext = Get_Filename_no_ext(FilePath);


	//if (Filename_no_ext.Find(".") == -1)  return FALSE; // no suffix, no prefix


	CString FileAddon;	

	CString FoundAddon;


	int choice = read_param_int("General","m_addon2", IniPath);

		FileAddon = read_param("General","EditFileAddon2", IniPath);


	if (choice == 1) // Prefix	
	
		FoundAddon = Filename_no_ext.Left(Filename_no_ext.Find("."));
	
	else if (choice == 2) // Suffix
	
		FoundAddon = Filename_no_ext.Right(Filename_no_ext.GetLength() - Filename_no_ext.Find(".")-1);

	else if (choice == 3) // None

		return TRUE;


	if (!FileAddon.CompareNoCase(FoundAddon)) return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString GetDjvuNum(CString Res)
{

Res.MakeLower();

int pos = Res.ReverseFind('.');

Res = Res.Left(pos);

int num = atoi(Res.GetBuffer(Res.GetLength()));

char buf[20] = {0};

itoa(num, buf, 10);

Res = buf;

return Res;

}

////////////////////////////////////////////////////////////////////////////////////////////////

CString GetForegroundPath(CString filename)
{
return GetForegroundPath(filename.GetBuffer(filename.GetLength()));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString read_param(CString section, CString param, CString ini_path)
{
	char buf[255] = {0};

	GetPrivateProfileString(section, param, NULL,buf,254,ini_path);

	CString Res(buf);

	return Res;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int read_param_int(CString section, CString param, CString ini_path)
{
	char buf[255] = {0};

	GetPrivateProfileString(section, param, NULL,buf,254,ini_path);	

	return atoi(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void write_param(CString section, CString param, CString value, CString ini_path)
{

WritePrivateProfileString(section, param, value, ini_path);

}

////////////////////////////////////////////////////////////////////////////////////////////////

void write_param(CString section, CString param, int value, CString ini_path)
{
	char buf[255] = {0};

	itoa(value,buf,10);

WritePrivateProfileString(section, param, buf, ini_path);

}

////////////////////////////////////////////////////////////////////////////////////////////////

int IsDjvuFile(char* filename)
{

char ext[10]={0};

if (
	!strcmp(get_ext(filename,ext),"djvu")||
	!strcmp(get_ext(filename,ext),"djv")
   )

    return 1;

	else return 0;
}

int IsDjvuFile(CString filename)
{
	if (filename.GetLength() == 0) return 0;

return IsDjvuFile(filename.GetBuffer(filename.GetLength()));
}

////////////////////////////////////////////////////////////////////////////////////////////////

int DeleteFolder(CString Path)
{
	char path[MAX_PATH] = {0};	

	if (Path[Path.GetLength()-1] != '\\') Path += "\\";

	strcpy(path, LPCSTR(Path));
	
	strcat(path,"*.*");
	
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile (path, &fd);
	
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (				
				!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				&& strncmp(fd.cFileName,".",1)				 				
				)

				if(remove(Path + fd.cFileName) == -1) return 0;
		}
		while (::FindNextFile (hFind, &fd));
		
		::FindClose (hFind);
	}

	if (rmdir(LPCSTR(Path)))
	
		return 0;

	else

		return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int ParseProfile(CString StProfile, int& p1, int& p2, int& p3, int& p4)
{
	int p = 0;
	
	p = StProfile.Find('+');
	
	p1 = atoi(StProfile.Left(p));
	
	StProfile = StProfile.Right(StProfile.GetLength()-p-1);
	
	p = StProfile.Find('+');
	
	p2= atoi(StProfile.Left(p));
	
	StProfile = StProfile.Right(StProfile.GetLength()-p-1);
	
	p = StProfile.Find('+');
	
	p3= atoi(StProfile.Left(p));
	
	StProfile = StProfile.Right(StProfile.GetLength()-p-1);
	
	p4= atoi(StProfile);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString SetCurrent(int p1, int p2, int p3, int p4)
{
	CString st_prof = st(p1);

	st_prof += ":";

	st_prof += st(p2);

	st_prof += ":";

	st_prof += st(p3);

	st_prof += ":";

	st_prof += st(p4);

	return st_prof;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void GetCurrent(CString St, int& p1, int& p2, int& p3, int& p4)
{
	//0001.tif=1:1:0:0

	p1 = atoi(St.Left(1));

	p2 = atoi(St.Mid(2,1));

	p3 = atoi(St.Mid(4,1));

	p4 = atoi(St.Mid(6,St.GetLength()-6));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString SetRadioAll(CString St, int p)
{
	if (St.GetLength() > 8)

		St = St.Left(St.GetLength()-1);

	St += ":";

	St += st(p);

	return St;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int GetRadioAll(CString St)
{
	if (!St.IsEmpty())
		
		return atoi(St.Right(1));

	else

		return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int CopyFile(CString Path, CString NewPath)
{
	FILE* infile;
	
	FILE* outfile;

   int  count;

   char buffer[2];
   
   if((infile = fopen(Path, "rb" )) == NULL )
   {
	   mb(11);
	   exit( 1 );
   }

   if((outfile = fopen(NewPath, "wb" )) == NULL )
   {
	   mb(22);
	   exit( 1 );
   }

   // Cycle until end of file reached:
   while(!feof(infile))
   {
      // Attempt to read in 1 byte:
      count = fread( buffer, sizeof( char ), 1, infile);
      if(ferror(infile))
	  {
         AfxMessageBox("Read error");
         break;
      }
      // Total up actual bytes read
	  count = fwrite(buffer, sizeof(char), 1, outfile);
   }
   
   fclose(infile);
   fclose(outfile);

   return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString GetDesktopPath(void)
{
// http://neomok.narod.ru/inf/17_05_01/17_05_01/ip/

// ---------- Описание переменных
  HKEY       hKey;
  DWORD   dwType, dwCount;
  CString   strValue;
//----------- Определить имя OC

 RegOpenKeyEx(HKEY_CURRENT_USER,
    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
    0,KEY_READ, &hKey);
 RegQueryValueEx (hKey,"Desktop", NULL,
    &dwType, NULL, &dwCount);
 RegQueryValueEx (hKey,"Desktop", NULL, 
    &dwType, (LPBYTE)strValue.GetBuffer(dwCount/
    sizeof(TCHAR)), &dwCount);
 strValue.ReleaseBuffer();
 RegCloseKey(hKey);

 return strValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////

long GetFileSize(CString Path)
{
int fh;

long size = 0;

if((fh = open(Path, _O_RDONLY )) != -1)
{
	 size = filelength(fh);

     close(fh);
}

return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString GetPrefixedFilter(CString LayerFiles, CString sep)
{			
	return LayerFiles + " (" + sep + ".*.*(graph))|" + sep + ".*.bmp;" + sep + 
		
		".*.jpeg;" + sep + ".*.jpg;" + sep + ".*.gif;" + sep + ".*.tiff;" + sep + 
		
		".*.tif;" + sep + ".*.pnm;" + sep + ".*.ppm;" + sep + ".*.pgm;" + sep + 
		
		".*.pbm;" + sep + ".*.png|";		
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString GetSuffixedFilter(CString LayerFiles, CString sep)
{
	return LayerFiles + " (*." + sep + ".*(graph))|*." + sep + ".bmp;*." + 
		
		sep + ".jpeg;*." + sep + ".jpg;*." + sep + ".gif;*." + sep + 
		
		".tiff;*." + sep + ".tif;*." + sep + ".pnm;*." + sep + ".ppm;*." +
		
		sep + ".pgm;*." + sep + ".pbm;*." + sep + ".png|";
}

////////////////////////////////////////////////////////////////////////////////////////////////

CString FormChunksString(int val)
{
/* This function forms the chunk string

  The total val is simply distributed evenly
  among 3 chunks (the first chunk is always a constant).
*/
	CString st_chunks;	

	int num = val / 16;	

	int last = val % 16; // the remainder from int-division

	int i;


	if (val > 48) // special case
	{
		st_chunks += "+" + st(val-32);

		for (i=1; i<num; i++)
			
			st_chunks += "+16";
	}

	else

	{	
	if (num)	

	for (int i=0; i<num; i++)
			
			st_chunks += "+16";	
	

	if (last) st_chunks += "+" +st(last);	
	}
		
	return st_chunks;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// http://www.sources.ru/cpp/faqs/98.htm

// Transforming the encoding "right here"
BOOL ConvertCP(UINT uiIN, UINT uiOUT, LPSTR lpString)
{
  if(!lpString) return FALSE; // Got NULL - bad, a mistake
  if(!lpString[0]) return TRUE; // empty string - OK, the transform is not needed

  // First get the necessary intermediary buffer size
  int nRet = MultiByteToWideChar(uiIN, MB_PRECOMPOSED, lpString, -1, NULL, 0);
  if(nRet == 0) return FALSE;

  // Create it
  WCHAR* pwcBuffer = new WCHAR[nRet+2];

  // Transforming uiIN -> Unicode
  nRet = MultiByteToWideChar(uiIN, MB_PRECOMPOSED, lpString, -1, pwcBuffer, nRet+2);
  if(nRet == 0)
  {
    delete[] pwcBuffer;
    return FALSE;
  }
  
  // Transforming Unicode -> uiOUT.
  // Change the unknown symbols to "#". Otherwise you can play with the WideCharToMultiByte() parameters
  nRet = WideCharToMultiByte(uiOUT, 0, pwcBuffer, -1, lpString, strlen(lpString)+1, "#", NULL);
  // Free the memory
  delete[] pwcBuffer;

  if(nRet == 0) return FALSE;

  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int GetNumOutFilename(CString FileName, CString IniPath)
{
	int rst = read_param_int("General","CheckRecognizeSTFilenames", IniPath);

	int num = 0; 

	int num_found = 0; 

	CString Ext = Get_Ext(FileName);	

	FileName = Get_Filename_no_ext(FileName);

	if(rst) // auto-recognize Scan Tailor filenames mode
	{
		if (FileName.GetLength()>8)
		{		
		char s1 = FileName[4];
		//char s2 = FileName[FileName.GetLength()-5];

		//if (Ext == "tiff" && s1 == '_' && s2 == '_') // this is ST filename (most likely)
		if (Ext == "tiff" && s1 == '_') // this is ST filename (most likely)

			FileName = FileName.Mid(5);
		}
	}

	int i = 0;

	for(i=0; i<FileName.GetLength(); i++)
	{
		if (!isdigit(FileName[i])) FileName.SetAt(i,' ');

		else num_found = 1;
	}	

	num = atoi(FileName.GetBuffer(FileName.GetLength()));

	if (num_found) 
		
		return num;
	
	else
		
		return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CheckSubSizes(int w, int h, int sw, int sh)
{
// GP<GPixmap> read_background(BufferByteStream &bs, int w, int h, int &bgred)
// from csepdjvu.cpp
int bgred;

for (bgred=1; bgred<=12; bgred++)
{
int subw = (w + bgred - 1) / bgred;
int subh = (h + bgred - 1) / bgred;
if (subh == sh && subw == sw)
       // Found reduction factor
       //return pix;
	   return 1;
    }
 // Failure
 //G_THROW("Background pixmap size does not match foreground");
return 0;
}


