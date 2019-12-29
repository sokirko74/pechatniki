// PeriodsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "PeriodsDlg.h"
#include "SlideShow.h"
#include "PeriodEditDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeriodsDlg dialog


CPeriodsDlg::CPeriodsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPeriodsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeriodsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIgnoreChangeSlectTrigger = false;
}


void CPeriodsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeriodsDlg)
	DDX_Control(pDX, IDC_EARLIER, m_EarlierBtn);
	DDX_Control(pDX, IDC_LATER, m_LaterBtn);
	DDX_Control(pDX, IDC_PERIODS_GROUP, m_PeriodsGroup);
	DDX_Control(pDX, IDC_AUDIO_GROUP, m_AudioGroup);
	DDX_Control(pDX, IDC_PAUSE_AUDIO, m_PauseBtn);
	DDX_Control(pDX, IDC_PLAY_AUDIO, m_PlayBtn);
	DDX_Control(pDX, IDC_SHOW_NEXT_SLIDE, m_ShowNextSlideBtn);
	DDX_Control(pDX, IDC_INSERT_SLIDE, m_InsertBtn);
	DDX_Control(pDX, IDC_DELETE_SLIDE, m_DeleteBtn);
	DDX_Control(pDX, IDC_ADD_SLIDE, m_AddBtn);
	DDX_Control(pDX, IDC_EDIT_SLIDE_PERIOD, m_EditPeriodBtn);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeriodsDlg, CDialog)
	//{{AFX_MSG_MAP(CPeriodsDlg)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EDIT_SLIDE_PERIOD, OnEditSlidePeriod)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_ADD_SLIDE, OnAddSlide)
	ON_BN_CLICKED(IDC_SHOW_NEXT_SLIDE, OnShowNextSlide)
	ON_BN_CLICKED(IDC_DELETE_SLIDE, OnDeleteSlide)
	ON_BN_CLICKED(IDC_INSERT_SLIDE, OnInsertSlide)
	ON_BN_CLICKED(IDC_PLAY_AUDIO, OnPlayAudio)
	ON_BN_CLICKED(IDC_PAUSE_AUDIO, OnPauseAudio)
	ON_BN_CLICKED(IDC_LATER, OnLater)
	ON_BN_CLICKED(IDC_EARLIER, OnEarlier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeriodsDlg message handlers


BOOL CPeriodsDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CPeriodsDlg::OnClose() 
{
	m_pSlideShow->OnShowPeriods();
}

void CPeriodsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	const int Border = 10;
	const int ButtonSize = 70;
	const int ButtonHeight = 20;
	m_ListBox.MoveWindow (Border, Border, cx - 4*Border - ButtonSize, cy - 2*Border);  

	m_PeriodsGroup.SetWindowPos(0,cx - ButtonSize - 2*Border,  ButtonHeight*0+1*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_EditPeriodBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*1+1*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_DeleteBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*2+2*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_AddBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*3+3*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_InsertBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*4+4*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_LaterBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*5+5*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_EarlierBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*6+6*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);


	m_AudioGroup.SetWindowPos(0,cx - ButtonSize - 2*Border,  ButtonHeight*7+8*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_ShowNextSlideBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*8+8*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_PlayBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*9+9*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	m_PauseBtn.SetWindowPos(0,cx - ButtonSize - Border,  ButtonHeight*10+10*Border,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	
	Invalidate();

	
}

void CPeriodsDlg::CopySlideToBox(int SlideNo) 
{
	const vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;

	if (SlideNo == m_ListBox.GetItemCount())
		m_ListBox.InsertItem( SlideNo, Slides[SlideNo].m_FileNameInSmil );
	else
		m_ListBox.SetItem (SlideNo, 0, LVIF_TEXT , Slides[SlideNo].m_FileNameInSmil, 0 , 0 , 0 ,0);

	CString S;
	S.Format("%02i:%02i", Slides[SlideNo].m_SecondsFromTheStart/60, Slides[SlideNo].m_SecondsFromTheStart%60);
	m_ListBox.SetItem (SlideNo, 1, LVIF_TEXT , S, 0 , 0 , 0 ,0);

	int Duration = Slides[SlideNo].m_SecondsFromTheStart;
	if (SlideNo > 0)
		Duration -= Slides[SlideNo-1].m_SecondsFromTheStart;
	S.Format("%i", Duration);
	m_ListBox.SetItem (SlideNo, 2, LVIF_TEXT , S, 0 , 0 , 0 ,0);
};

BOOL CPeriodsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_ListBox.InsertColumn(1,"File Name",LVCFMT_LEFT, 100);
	m_ListBox.InsertColumn(2,"End time",LVCFMT_LEFT, 70);
	m_ListBox.InsertColumn(2,"Duration",LVCFMT_LEFT, 70);
	
	const vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;
    int i = 0;
	for (; i < Slides.size(); i++)
	{
		CopySlideToBox(i);
	};

	i = m_ListBox.GetItemCount();

	int SelectedItemNo = m_pSlideShow->GetDocument()->m_SlideNo;
	m_ListBox.SetItem (SelectedItemNo, 0, LVIF_STATE , 0, 0 , LVIS_SELECTED, LVIS_SELECTED  ,0);

	m_ListBox.SetExtendedStyle (LVS_EX_FULLROWSELECT);
	
	return TRUE;  
}

bool CPeriodsDlg::EditOneSlide(int SlideNo) 
{
	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;
	CPeriodEditDlg D;
	D.m_FileName = Slides[SlideNo].m_FileNameInSmil; 
	D.m_Seconds.Format("%i",  Slides[SlideNo].m_SecondsFromTheStart);
	if (D.DoModal() != IDOK) return false;
	int Sec = atoi(D.m_Seconds);
	if (Sec == 0)
	{
		AfxMessageBox ("Cannot get value from " + D.m_Seconds);
		return false;
	};

	Slides[SlideNo].m_FileNameInSmil =  D.m_FileName;
	Slides[SlideNo].m_SecondsFromTheStart = Sec;
	
	
	if (!m_pSlideShow->GetDocument()->CheckExistance())
		return false;

	
	m_pSlideShow->GetDocument()->SetModifiedFlag();
	return true;
};

void CPeriodsDlg::OnEditSlidePeriod() 
{
	int Item = GetSel();
	if (Item == -1) return;

	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;

	const vector<CSlide> SaveSlides = Slides;
	if (!EditOneSlide(Item))
	{
		Slides = SaveSlides;
		return;
	};

	CopySlideToBox(Item);

}

void CPeriodsDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditSlidePeriod();	
	*pResult = 0;
}

void CPeriodsDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (!m_bIgnoreChangeSlectTrigger)
		m_pSlideShow->SetPosition(pNMListView->iItem, false);
	
	
	*pResult = 0;
}



void CPeriodsDlg::OnAddSlide() 
{
	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;

	const vector<CSlide> SaveSlides = Slides;
	CSlide T;
	T.m_SecondsFromTheStart = 1;
	if ( !Slides.empty() )
		T.m_SecondsFromTheStart += Slides.back().m_SecondsFromTheStart;
	Slides.push_back(T);
	int Item = Slides.size() - 1;
	if (!EditOneSlide(Item))
	{
		Slides = SaveSlides;
		return;
	};
	
	CopySlideToBox(Item);

	m_pSlideShow->GetDocument()->m_SlideNo = Item;

	m_ListBox.SetItemState( m_pSlideShow->GetDocument()->m_SlideNo, LVIS_SELECTED , LVIS_SELECTED );

}




void CPeriodsDlg::OnDeleteSlide() 
{
	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;
	int Item = GetSel();
	if (Item == -1) return;
	Slides.erase(Slides.begin() + Item),
	m_ListBox.DeleteItem(Item);	

	if (m_pSlideShow->GetDocument()->m_SlideNo == Slides.size())
			m_pSlideShow->GetDocument()->m_SlideNo--;

	int SlideNo  = m_pSlideShow->GetDocument()->m_SlideNo;

	if (SlideNo < Slides.size())
	{
		CopySlideToBox ( SlideNo );
		m_ListBox.SetItemState( SlideNo, LVIS_SELECTED , LVIS_SELECTED );
	};
	
}

void CPeriodsDlg::OnInsertSlide() 
{
	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;
	int Item = GetSel();
	if (Item == -1) return;

	const vector<CSlide> SaveSlides = Slides;
	CSlide T;
	T.m_SecondsFromTheStart = 1;
	if ( Item > 0 )
		T.m_SecondsFromTheStart += Slides[Item-1].m_SecondsFromTheStart;

    Slides.insert(Slides.begin() + Item, T);
	if (!EditOneSlide(Item))
	{
		Slides = SaveSlides;
		return;
	};

	m_ListBox.InsertItem( Item, "empty" );
	
	CopySlideToBox(Item);

	m_ListBox.SetItemState( Item, LVIS_SELECTED , LVIS_SELECTED );
	
	if (Item > 0)
		CopySlideToBox(Item-1);
	
};



void CPeriodsDlg::OnShowNextSlide() 
{
	m_pSlideShow->ShowNextSlideAndRememberPeriod();
	
}


void CPeriodsDlg::OnPlayAudio() 
{
	m_pSlideShow->Play();
		
}

void CPeriodsDlg::OnPauseAudio() 
{
	m_pSlideShow->OnStop();
	
}

void CPeriodsDlg::OnLater() 
{
	int Item = GetSel();
	if (Item == -1) return;
	if (Item == 0) return;
	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;	
	Slides[Item-1].m_SecondsFromTheStart++;
	CopySlideToBox(Item-1);
	CopySlideToBox(Item);
	m_pSlideShow->GetDocument()->SetModifiedFlag();	

}

void CPeriodsDlg::OnEarlier() 
{
	int Item = GetSel();
	if (Item == -1) return;
	if (Item == 0) return;
	vector<CSlide>&	Slides = m_pSlideShow->GetDocument()->m_Slides;	

	Slides[Item-1].m_SecondsFromTheStart--;
	CopySlideToBox(Item-1);
	CopySlideToBox(Item);
	m_pSlideShow->GetDocument()->SetModifiedFlag();	
}




void CPeriodsDlg::OnCancel() 
{
	m_pSlideShow->OnShowPeriods();
	
	//CDialog::OnCancel();
}
