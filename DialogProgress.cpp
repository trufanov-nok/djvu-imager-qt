// DialogProgress.cpp : implementation file
//

#include "stdafx.h"
#include "DjVu Sep.h"
#include "DialogProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DialogProgress dialog


DialogProgress::DialogProgress(CWnd* pParent /*=NULL*/)
	: CDialog(DialogProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(DialogProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DialogProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogProgress)
	DDX_Control(pDX, IDC_STATIC_ENC_PAGES, m_ProcessedPages);
	DDX_Control(pDX, IDC_BUTTON_ENC_CANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_STATIC_ENC_STATUS, m_StaticStatus);
	DDX_Control(pDX, IDC_PROGRESS_ENCODING, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogProgress, CDialog)
	//{{AFX_MSG_MAP(DialogProgress)
	ON_BN_CLICKED(IDC_BUTTON_ENC_CANCEL, OnButtonEncCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogProgress message handlers

BOOL DialogProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_Progress.SetRange(0,m_range);

	m_pos = 0;

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_doubleclick = 0;

	m_num_pages = 0;

	// disable the "X" closing button
	//
	// http://www.agpi.itech.ru/forum/showthread.php?t=373
	//
	HMENU MenuHandle = ::GetSystemMenu( this->m_hWnd, FALSE );
	if ( MenuHandle )
	DeleteMenu ( MenuHandle, SC_CLOSE, MF_BYCOMMAND );	

	m_ProcessedPages.SetWindowText(m_TextProcessed);

	m_StaticStatus.SetWindowText(m_TextStatus);

	this->SetWindowText(m_ProgressName);

	m_ButtonCancel.SetWindowText(m_Cancel);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////

void DialogProgress::OnButtonEncCancel() 
{
	// TODO: Add your control notification handler code here

	if (m_doubleclick) return; // protect against occasional doubleclicks
	// causing SetWindowText on this dialog when it is already destroyed
	m_doubleclick++;

	if (m_pos == m_range)
	{
		SetEvent(m_hEvent);  // allow viewing in main window

		::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
	}

	else if (m_pos == -1)

		::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);

	else
	{
		m_pos = m_range;

		SetEvent(m_hStopEvent);
	}
}

/////////////////////////////////////////////////////////////////////////////

void DialogProgress::DisplayProgress(void)
{
	m_Progress.SetPos(++m_pos);	

	//CString Text("Processed pages:  ");

	CString Text = m_TextProcessed;

	Text += "  ";

	Text += st(++m_num_pages);

	//Text += "  of  ";

	Text += "  ";

	Text += m_TextOf;

	Text += "  ";

	Text += st(m_range);

	m_ProcessedPages.SetWindowText(Text);
}

/////////////////////////////////////////////////////////////////////////////
