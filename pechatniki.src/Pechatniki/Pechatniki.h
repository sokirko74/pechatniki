// Pechatniki.h : main header file for the PECHATNIKI application
//

#if !defined(AFX_PECHATNIKI_H__465FB2BF_2AA5_424A_BEF6_1A96460B3595__INCLUDED_)
#define AFX_PECHATNIKI_H__465FB2BF_2AA5_424A_BEF6_1A96460B3595__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiApp:
// See Pechatniki.cpp for the implementation of this class
//

class CPechatnikiApp : public CWinApp
{
public:
	CPechatnikiApp();
	void ShowFilmLists();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPechatnikiApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile( LPCTSTR lpszFileName );
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPechatnikiApp)
	afx_msg void OnAppAbout();
	afx_msg void OnShowFilmList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PECHATNIKI_H__465FB2BF_2AA5_424A_BEF6_1A96460B3595__INCLUDED_)
