#if !defined(AFX_PERIODSDLG_H__CEFEB9B0_1714_4B4B_9D72_4D83D49EC55F__INCLUDED_)
#define AFX_PERIODSDLG_H__CEFEB9B0_1714_4B4B_9D72_4D83D49EC55F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PeriodsDlg.h : header file
//

#include "PeriodsListBox.h"
#include "PeriodsBtn.h"
/////////////////////////////////////////////////////////////////////////////
// CPeriodsDlg dialog
class CSlideShow;

class CPeriodsDlg : public CDialog
{
// Construction
public:
	CPeriodsDlg(CWnd* pParent = NULL);   // standard constructor
	CSlideShow*		m_pSlideShow;
	bool			m_bIgnoreChangeSlectTrigger;

	void			CopySlideToBox(int SlideNo);
	bool			EditOneSlide(int SlideNo);
	int				GetSel() const
	{
		POSITION pos = m_ListBox.GetFirstSelectedItemPosition();
		if (pos == 0) return -1;
		return m_ListBox.GetNextSelectedItem(pos);
	};

// Dialog Data
	//{{AFX_DATA(CPeriodsDlg)
	enum { IDD = IDD_PERIODS_DLG };
	CPeriodsBtn	m_EarlierBtn;
	CPeriodsBtn	m_LaterBtn;
	CButton	m_PeriodsGroup;
	CButton	m_AudioGroup;
	CPeriodsBtn m_PauseBtn;
	CPeriodsBtn m_PlayBtn;
	CPeriodsBtn	m_ShowNextSlideBtn;
	CPeriodsBtn	m_InsertBtn;
	CPeriodsBtn	m_DeleteBtn;
	CPeriodsBtn	m_AddBtn;
	CPeriodsBtn	m_EditPeriodBtn;
	CPeriodsListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeriodsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPeriodsDlg)
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnEditSlidePeriod();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddSlide();
	afx_msg void OnShowNextSlide();
	afx_msg void OnDeleteSlide();
	afx_msg void OnInsertSlide();
	afx_msg void OnPlayAudio();
	afx_msg void OnPauseAudio();
	afx_msg void OnLater();
	afx_msg void OnEarlier();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERIODSDLG_H__CEFEB9B0_1714_4B4B_9D72_4D83D49EC55F__INCLUDED_)
