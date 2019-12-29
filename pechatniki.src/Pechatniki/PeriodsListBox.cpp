// PeriodsListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "PeriodsListBox.h"
#include "SlideShow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeriodsListBox

CPeriodsListBox::CPeriodsListBox()
{
}

CPeriodsListBox::~CPeriodsListBox()
{
}


BEGIN_MESSAGE_MAP(CPeriodsListBox, CListCtrl)
	//{{AFX_MSG_MAP(CPeriodsListBox)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeriodsListBox message handlers



void CPeriodsListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F2)
	{	
		CSlideShow* P = ((CPeriodsDlg*)GetParent())->m_pSlideShow;
		P->OnShowPeriods();
		return;
	};	

	
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
