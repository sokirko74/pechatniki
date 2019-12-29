#if !defined(AFX_ADDDIALOG_H__E85061A0_1E4A_11D3_9CD1_00105A68BAF2__INCLUDED_)
#define AFX_ADDDIALOG_H__E85061A0_1E4A_11D3_9CD1_00105A68BAF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddDialog.h : header file
//

#define MAX_FILES_COUNT 100

/////////////////////////////////////////////////////////////////////////////
// CAddDialog dialog

class CAddDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CAddDialog)
protected:
	bool	ParseFilePathNames(const CString& src, _CStringVector_t& dst);
	bool	FindFilesWithPath(const CString& filename, const bool recursive, _CStringVector_t& results);


public:
	_CStringVector_t	m_Results;
	CAddDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
//{{AFX_DATA(CSetColorDialog)
	BOOL m_bRecursive;
//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColorDialog)
	virtual BOOL OnInitDialog( );
	virtual BOOL OnFileNameOK( );
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	char m_NamesBuffer[MAX_FILES_COUNT*MAX_PATH];
	//{{AFX_MSG(CAddDialog)
	afx_msg void OnCheckRecursive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDDIALOG_H__E85061A0_1E4A_11D3_9CD1_00105A68BAF2__INCLUDED_)
