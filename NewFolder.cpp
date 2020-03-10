// NewFolder.cpp : implementation file
//

#include "stdafx.h"
#include "DjVu Sep.h"
#include "NewFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NewFolder dialog


NewFolder::NewFolder(CWnd* pParent /*=NULL*/)
	: CDialog(NewFolder::IDD, pParent)
{
	//{{AFX_DATA_INIT(NewFolder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void NewFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewFolder)
	DDX_Control(pDX, IDC_EDIT_NEW_FOLDER, m_EditNewFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewFolder, CDialog)
	//{{AFX_MSG_MAP(NewFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewFolder message handlers

BOOL NewFolder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	

	this->SetWindowText(m_StNewFolder);

	GetDlgItem(IDC_STATIC_NEW_FOLDER_NAME)->SetWindowText(m_StNewFolderName);	

	GetDlgItem(IDCANCEL)->SetWindowText(m_StCancel);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
