#if !defined(AFX_SLIDESHOW_H__D5E9B7A2_89CE_4D98_995C_B1F464066AEE__INCLUDED_)
#define AFX_SLIDESHOW_H__D5E9B7A2_89CE_4D98_995C_B1F464066AEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlideShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSlideShow form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "PeriodsDlg.h"
#include "PechatnikiDoc.h"
class CSlideShow : public CFormView
{
protected:
	CSlideShow();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSlideShow)

// Form Data
public:
	CPechatnikiDoc* GetDocument();
	bool			m_bPlay;
	HWND			m_AudioWnd;
	bool			m_bPaused;
	bool			m_bFullScreen;
	CRect			m_SaveNormalRect;

	UINT			m_SavedWindowStyle;
	CMenu			m_Menu;

	UINT			m_FilmSeconds;
	UINT			m_FilmSpeed;

	CPeriodsDlg*	m_pPeriodsDlg;

	void			SetNormalWindow();
	void			SetAudioPosition();
	void			SetPosition(int SlideNo, bool bUpdatePeriodsDlg );
	void			PaintSlide(CPaintDC& dc);
	bool			ShowNextSlide() const 
	{
		return m_pPeriodsDlg != 0;
	};
	void			ShowNextSlideAndRememberPeriod();
	void			UpdatePeriodsDlg();
	void			Play();
	
	//{{AFX_DATA(CSlideShow)
	enum { IDD = IDD_SLIDESHOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlideShow)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSlideShow();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSlideShow)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnFilePlay();
	afx_msg void OnFullScreen();
	afx_msg void OnNormalScreen();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGotoSlide();
	afx_msg void OnNextSlide();
	afx_msg void OnPreviousSlide();
	afx_msg void OnShowPeriods();
	afx_msg void OnHome();
	afx_msg void OnEnd();
	afx_msg void OnShowFilmList();
	afx_msg void OnCreateSlimFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PechatnikiView.cpp
inline CPechatnikiDoc* CSlideShow::GetDocument()
   { return (CPechatnikiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDESHOW_H__D5E9B7A2_89CE_4D98_995C_B1F464066AEE__INCLUDED_)
