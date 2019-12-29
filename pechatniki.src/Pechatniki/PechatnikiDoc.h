// PechatnikiDoc.h : interface of the CPechatnikiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PECHATNIKIDOC_H__05239327_C5B1_48E2_8A76_D899ABA33469__INCLUDED_)
#define AFX_PECHATNIKIDOC_H__05239327_C5B1_48E2_8A76_D899ABA33469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CSlide
{
	CString m_FullPath;
	CString m_FileNameInSmil;
	int		m_SecondsFromTheStart;
};


class CPechatnikiDoc : public CDocument
{
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CPechatnikiDoc)

// Attributes
public:
	CPechatnikiDoc();
	CString			m_FilmName;
	CString			m_Copyright;
	CString			m_Author;
	vector<CSlide>	m_Slides;
	int				m_SlideNo;

	CString			m_Folder;
	CString			m_AudioPath;

	IPicture*		m_pPicture;
	CString			m_LastLoadedFileName;

	IPicture*		m_pNextPicture;
	CString			m_LastLoadedNextFileName;
	bool			m_bCheckExistanceOfFilesWhileLoading;
	
	bool			ReadSmilAsString(CArchive& ar, CString& Smil);
	bool			ReadSmilFile(CArchive& ar);
	bool			SaveSmilFile(CArchive& ar);
	bool			CheckExistance();
	void			LoadSlide();
	void			LoadNextSlide();
	bool			CanUseNextSlide() const
	{
		if (m_SlideNo == -1) return false;
		if (m_Slides.empty()) return false;
		return m_LastLoadedNextFileName == m_Slides[m_SlideNo].m_FullPath;
	};
	int GetDuration(int SlideNo) const
	{
		int Res = m_Slides[SlideNo].m_SecondsFromTheStart;

		if (SlideNo > 0)
			Res -= m_Slides[SlideNo-1].m_SecondsFromTheStart;

		return Res;
	};


	void			UseNextSlide();
    void            SetTitle(LPCTSTR lpszTitle); 


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPechatnikiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPechatnikiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPechatnikiDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PECHATNIKIDOC_H__05239327_C5B1_48E2_8A76_D899ABA33469__INCLUDED_)
