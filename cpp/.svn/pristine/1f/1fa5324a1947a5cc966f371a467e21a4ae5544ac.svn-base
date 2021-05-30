// PeriodEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "PeriodEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeriodEditDlg dialog


CPeriodEditDlg::CPeriodEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPeriodEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeriodEditDlg)
	m_FileName = _T("");
	m_Seconds = _T("");
	//}}AFX_DATA_INIT
}


void CPeriodEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeriodEditDlg)
	DDX_Text(pDX, IDC_FILENAME_EDT, m_FileName);
	DDX_Text(pDX, IDC_SECONDS_EDT, m_Seconds);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeriodEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPeriodEditDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeriodEditDlg message handlers
