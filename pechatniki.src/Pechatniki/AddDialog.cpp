// AddDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Concordance.h"
#include "AddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddDialog

IMPLEMENT_DYNAMIC(CAddDialog, CFileDialog)

CAddDialog::CAddDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags | OFN_NOVALIDATE | OFN_EXPLORER | OFN_ENABLETEMPLATE, lpszFilter, pParentWnd)
{
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_ADD_FILES);
	m_bRecursive = FALSE;
	m_NamesBuffer[0] = NULL;
	m_ofn.lpstrFile = m_NamesBuffer;
	m_ofn.nMaxFile = MAX_FILES_COUNT*MAX_PATH;
}


BEGIN_MESSAGE_MAP(CAddDialog, CFileDialog)
	//{{AFX_MSG_MAP(CAddDialog)
	ON_BN_CLICKED(IDC_CHECK_RECURSIVE, OnCheckRecursive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetColorDialog)
	DDX_Check(pDX, IDC_CHECK_RECURSIVE, m_bRecursive);
	//}}AFX_DATA_MAP
}

BOOL CAddDialog::OnInitDialog()
{
	BOOL bResult = CFileDialog::OnInitDialog();
	return (bResult);
}

bool	CAddDialog::ParseFilePathNames(const CString& src, _CStringVector_t& dst)
{
	int posPath = src.ReverseFind('\\') + 1;
	CString path = src.Left(posPath);
	CString names = src.Right(src.GetLength() - posPath);
	CString filename;
	_CStringVector_t::iterator	loc_name;
	for (int i = 0; i < names.GetLength(); i++)
	{
		if (	(names[i] == ',')
			||	(names[i] == ' ')
			||	(names[i] == ';'))
		{
			filename = names.Left(i);
			names = names.Right(names.GetLength() - i - 1);
			i = -1;
			if (!filename.IsEmpty())
			{
				filename = path + filename;
				loc_name = lower_bound(dst.begin(), dst.end(), filename);
				if (	(loc_name == dst.end())
					||	(*loc_name != filename))
					dst.insert(loc_name, filename);
			}
		}
	}
	if (!names.IsEmpty())
	{
		filename = path + names;
		loc_name = lower_bound(dst.begin(), dst.end(), filename);
		if (	(loc_name == dst.end())
			||	(*loc_name != filename))
			dst.insert(loc_name, filename);
	}
	return (true);
}

bool	CAddDialog::FindFilesWithPath(const CString& filename, const bool recursive, _CStringVector_t& results)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = INVALID_HANDLE_VALUE;
	CString	pathname;
	int posPath = filename.ReverseFind('\\');
	CString path = filename.Left(posPath + 1);
	if (recursive)
	{
// Extracting path
		CString name = filename.Right(filename.GetLength() - posPath);
//
		WIN32_FIND_DATA FindFileData;
		hFindFile = ::FindFirstFile(path + "*.*", &FindFileData);
		if (hFindFile == INVALID_HANDLE_VALUE)
			return (false);
		do 
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				pathname = FindFileData.cFileName; 
				if (	(pathname != ".")
					&&	(pathname != ".."))
				{
					pathname += name;
					pathname = path + pathname;
					FindFilesWithPath(pathname, recursive, results);
				}
			}
		}	while (::FindNextFile(hFindFile, &FindFileData));
		::FindClose(hFindFile);
	}
	_CStringVector_t::iterator	loc_name;
	hFindFile = ::FindFirstFile(filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return (false);
	do 
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			pathname = path + CString(FindFileData.cFileName);
			loc_name = lower_bound(results.begin(), results.end(), pathname);
			if (	(loc_name == results.end())
				||	(*loc_name != pathname))
				results.insert(loc_name, pathname);
		}
	}	while (::FindNextFile(hFindFile, &FindFileData));
	::FindClose(hFindFile);
	return (true);
}


BOOL CAddDialog::OnFileNameOK( )
{
	POSITION pos = GetStartPosition();
	CString	filename;
	_CStringVector_t names;
	while (pos != NULL)
	{
		filename = GetNextPathName(pos);
		ParseFilePathNames(filename, names);
	}

	m_Results.clear();
	for (int i = 0; i < names.size(); i++)
		FindFilesWithPath(names[i], m_bRecursive == TRUE, m_Results);
	return (m_Results.empty());
}


void CAddDialog::OnCheckRecursive() 
{
	m_bRecursive = !m_bRecursive;		
}
