// GotoSlideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "GotoSlideDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGotoSlideDlg dialog


CGotoSlideDlg::CGotoSlideDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGotoSlideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoSlideDlg)
	m_SlideNo = _T("");
	//}}AFX_DATA_INIT
}


void CGotoSlideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoSlideDlg)
	DDX_Control(pDX, IDC_SPIN1, m_SpinBox);
	DDX_Text(pDX, IDC_NUMBER_EDIT, m_SlideNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoSlideDlg, CDialog)
	//{{AFX_MSG_MAP(CGotoSlideDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGotoSlideDlg message handlers

BOOL CGotoSlideDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SpinBox.SetBuddy( GetDescendantWindow(IDC_NUMBER_EDIT) );	
	m_SpinBox.SetRange( 1, m_MaxSlideNo );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
