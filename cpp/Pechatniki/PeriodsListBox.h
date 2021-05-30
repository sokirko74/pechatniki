#if !defined(AFX_PERIODSLISTBOX_H__4849E28D_6F1A_4F74_90E2_56FA83122C20__INCLUDED_)
#define AFX_PERIODSLISTBOX_H__4849E28D_6F1A_4F74_90E2_56FA83122C20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PeriodsListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPeriodsListBox window

class CPeriodsListBox : public CListCtrl
{
// Construction
public:
	CPeriodsListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeriodsListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPeriodsListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPeriodsListBox)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERIODSLISTBOX_H__4849E28D_6F1A_4F74_90E2_56FA83122C20__INCLUDED_)
