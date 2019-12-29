// FilmList.cpp : implementation file
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "FilmList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilmList dialog


CFilmList::CFilmList(CWnd* pParent /*=NULL*/)
	: CDialog(CFilmList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilmList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFilmList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilmList)
	DDX_Control(pDX, IDC_FILE_LISt_CTRL, m_FilmListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilmList, CDialog)
	//{{AFX_MSG_MAP(CFilmList)
	ON_BN_CLICKED(IDSHOW, OnShow)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LISt_CTRL, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilmList message handlers



void CFilmList::AddFilmToControl(int FilmNo) 
{
	m_FilmListCtrl.InsertItem( FilmNo, m_Films[FilmNo].m_FilmName);
	m_FilmListCtrl.SetItem (FilmNo, 1, LVIF_TEXT , m_Films[FilmNo].m_Duration, 0 , 0 , 0 ,0);

	CString S;
	S.Format("%i", m_Films[FilmNo].m_YearOfCreation);
	m_FilmListCtrl.SetItem (FilmNo, 2, LVIF_TEXT , S, 0 , 0 , 0 ,0);

	
	S.Format("%i", m_Films[FilmNo].m_SlidesCount);
	m_FilmListCtrl.SetItem (FilmNo, 3, LVIF_TEXT , S, 0 , 0 , 0 ,0);
};

BOOL CFilmList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_FilmListCtrl.InsertColumn(1,"Film name",LVCFMT_LEFT, 90);
	m_FilmListCtrl.InsertColumn(2,"Duration",LVCFMT_LEFT, 60);
	m_FilmListCtrl.InsertColumn(3,"Year",LVCFMT_LEFT, 40);
	m_FilmListCtrl.InsertColumn(4,"Slides",LVCFMT_LEFT, 40);

	for (int i = 0; i < m_Films.size(); i++)
	{
		AddFilmToControl(i);
	};

	
	m_FilmListCtrl.SetExtendedStyle (LVS_EX_FULLROWSELECT);	
	m_FilmListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED  , LVIS_SELECTED|LVIS_FOCUSED  );
	
	return TRUE; 
}

void CFilmList::OnShow() 
{
	int Item = GetSel();
	if (Item == -1) return;
	EndDialog(1);
	AfxGetApp()->OpenDocumentFile(m_Films[Item].m_FileName);

}

void CFilmList::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnShow();
	*pResult = 0;
}

