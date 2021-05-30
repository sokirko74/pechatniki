// PeriodsBtn.cpp : implementation file
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "PeriodsBtn.h"
#include "PeriodsDlg.h"
#include "SlideShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeriodsBtn

CPeriodsBtn::CPeriodsBtn()
{
}

CPeriodsBtn::~CPeriodsBtn()
{
}


BEGIN_MESSAGE_MAP(CPeriodsBtn, CButton)
	//{{AFX_MSG_MAP(CPeriodsBtn)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeriodsBtn message handlers

void CPeriodsBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F2)
	{	
		CSlideShow* P = ((CPeriodsDlg*)GetParent())->m_pSlideShow;
		P->OnShowPeriods();
		return;
	};	
	
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
