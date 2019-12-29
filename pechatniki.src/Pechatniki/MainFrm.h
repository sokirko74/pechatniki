// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B8DCAC16_2C97_47B2_B0D4_835FFB32973A__INCLUDED_)
#define AFX_MAINFRM_H__B8DCAC16_2C97_47B2_B0D4_835FFB32973A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	
	

// Attributes
public:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndPlayToolBar; // control bar embedded members
	CToolBar    m_wndFileToolBar; // control bar embedded members

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:  
	

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B8DCAC16_2C97_47B2_B0D4_835FFB32973A__INCLUDED_)
