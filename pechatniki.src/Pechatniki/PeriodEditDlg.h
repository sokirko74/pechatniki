#if !defined(AFX_PERIODEDITDLG_H__E7409247_7B1F_4AF8_8BB2_98A63185C75C__INCLUDED_)
#define AFX_PERIODEDITDLG_H__E7409247_7B1F_4AF8_8BB2_98A63185C75C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PeriodEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPeriodEditDlg dialog

class CPeriodEditDlg : public CDialog
{
// Construction
public:
	CPeriodEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPeriodEditDlg)
	enum { IDD = IDD_EDIT_PERIOD };
	CString	m_FileName;
	CString	m_Seconds;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeriodEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPeriodEditDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERIODEDITDLG_H__E7409247_7B1F_4AF8_8BB2_98A63185C75C__INCLUDED_)
