// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0C1D92E8_4716_49F9_B8F6_B72F8840B313__INCLUDED_)
#define AFX_STDAFX_H__0C1D92E8_4716_49F9_B8F6_B72F8840B313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


class CExpc
{
 public:
	int  m_nCode;
	char m_strCause[200];

	CExpc(const char* strCause, int code=0) { 
        m_nCode=code; 
        strncpy (m_strCause, strCause, 199);
		size_t l = strlen (strCause);
		if (l > 199) 
			m_strCause[199] = 0;
        else
		   m_strCause[l] = 0;
		   


     };
	CExpc(const CExpc& from)
	{
		*this = from;
	}
	CExpc& operator= (const CExpc& from)
	{
		m_nCode =		from.m_nCode;
		strcpy (m_strCause, from.m_strCause);
		return *this;
	}

};



inline void	Log(CString S) 
{
	CFrameWnd* Wnd = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	if (Wnd)
		Wnd->SetMessageText(S);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0C1D92E8_4716_49F9_B8F6_B72F8840B313__INCLUDED_)
