#if !defined(AFX_FILMLIST_H__8070DCAF_CCDB_4162_A0D6_35D9651B78B0__INCLUDED_)
#define AFX_FILMLIST_H__8070DCAF_CCDB_4162_A0D6_35D9651B78B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilmList.h : header file
//

struct CFilm
{
	CString			m_FileName;
	CString			m_FilmName;
	int 			m_SlidesCount;
	CString			m_Duration;
	int				m_YearOfCreation;
	CFilm(CString  FileName, CString FilmName, int SlidesCount, CString Duration, int YearOfCreation) 
	{
		m_FileName =  FileName;
		m_FilmName = FilmName;
		m_SlidesCount = SlidesCount;
		m_Duration = Duration;
		m_YearOfCreation = YearOfCreation;
	};
};
class CFilmList : public CDialog
{
// Construction
public:
	CFilmList(CWnd* pParent = NULL);   // standard constructor
	vector<CFilm>	m_Films;
	void AddFilmToControl(int FilmNo);
	int				GetSel() const
	{
		POSITION pos = m_FilmListCtrl.GetFirstSelectedItemPosition();
		if (pos == 0) return -1;
		return m_FilmListCtrl.GetNextSelectedItem(pos);
	};
// Dialog Data
	//{{AFX_DATA(CFilmList)
	enum { IDD = IDD_FILM_LIST_DLG };
	CListCtrl	m_FilmListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilmList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilmList)
	virtual BOOL OnInitDialog();
	afx_msg void OnShow();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILMLIST_H__8070DCAF_CCDB_4162_A0D6_35D9651B78B0__INCLUDED_)
