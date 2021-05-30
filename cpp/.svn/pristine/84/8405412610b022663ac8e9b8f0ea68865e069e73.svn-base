// SlideShow.cpp : implementation file
//

#include "stdafx.h"
#include "algorithm"
#include "Pechatniki.h"
#include "PechatnikiDoc.h"
#include "SlideShow.h"
#include "MainFrm.h"
#include "vfw.h"
#include "assert.h"
#include <afxctl.h>
#include "GotoSlideDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlideShow

IMPLEMENT_DYNCREATE(CSlideShow, CFormView)

CSlideShow::CSlideShow()
	: CFormView(CSlideShow::IDD)
{
	//{{AFX_DATA_INIT(CSlideShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bPlay = false;
	m_bPaused = false;
	m_AudioWnd = NULL;
	m_bFullScreen = false;
	m_Menu.LoadMenu(IDR_SMILFILENAME);
	m_pPeriodsDlg = 0;	
	
}	

CSlideShow::~CSlideShow()
{
	if(m_AudioWnd !=NULL)
	{
		MCIWndDestroy(m_AudioWnd);
	}

}

void CSlideShow::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlideShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSlideShow, CFormView)
	//{{AFX_MSG_MAP(CSlideShow)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_PLAY, OnFilePlay)
	ON_COMMAND(ID_FULL_SCREEN, OnFullScreen)
	ON_COMMAND(ID_NORMAL_SCREEN, OnNormalScreen)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_GOTO_SLIDE, OnGotoSlide)
	ON_COMMAND(ID_NEXT_SLIDE, OnNextSlide)
	ON_COMMAND(ID_PREVIOUS_SLIDE, OnPreviousSlide)
	ON_COMMAND(ID_SHOW_PERIODS, OnShowPeriods)
	ON_COMMAND(ID_HOME, OnHome)
	ON_COMMAND(ID_END, OnEnd)
	ON_COMMAND(ID_SHOW_FILM_LIST, OnShowFilmList)
	ON_COMMAND(ID_CREATE_SLIM_FILE, OnCreateSlimFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlideShow diagnostics

#ifdef _DEBUG
void CSlideShow::AssertValid() const
{
	CFormView::AssertValid();
}

void CSlideShow::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
CPechatnikiDoc* CSlideShow::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPechatnikiDoc)));
	return (CPechatnikiDoc*)m_pDocument;
}
#endif //_DEBUG






void CSlideShow::OnInitialUpdate() 
{
	CMainFrame* Parent = (CMainFrame*)GetParent();

	CFormView::OnInitialUpdate();

	m_Menu.DestroyMenu();
	m_Menu.LoadMenu(IDR_SMILFILENAME);


	if (m_bFullScreen)
		OnNormalScreen();
	else
		Parent->SetMenu(&m_Menu);	

	if(m_AudioWnd !=NULL)
	{
		MCIWndDestroy(m_AudioWnd);
		m_AudioWnd = 0;
	}
	

	SetTimer(2, 100, 0);
	
	m_bPlay = false;
	m_bPaused = false;


	CToolBar& T = Parent->m_wndPlayToolBar;
	
	if (GetDocument()->m_Slides.size() == 0)
	{
		Parent->ShowControlBar( &Parent->m_wndPlayToolBar,FALSE, FALSE );
	}
	else
	{
		Parent->ShowControlBar( &Parent->m_wndPlayToolBar,TRUE, FALSE );
		//T.SetWindowPos(this, 0, 0, 200, 49, SWP_NOZORDER  | SWP_SHOWWINDOW  | SWP_NOMOVE );
	};

	if (GetDocument()->m_Slides.size() > 0)
		if(m_AudioWnd == NULL)
		{
			m_AudioWnd = MCIWndCreate(this->GetSafeHwnd(),
			  AfxGetInstanceHandle(),
			  WS_CHILD | MCIWNDF_NOMENU,GetDocument()->m_AudioPath);

			if (!m_AudioWnd)
				throw CExpc ("Cannot load audio file ");
		
			char TimeFormat[200];
			long iTimeFormat = MCIWndGetTimeFormat(m_AudioWnd, TimeFormat, 200);
			if (MCI_FORMAT_MILLISECONDS != iTimeFormat)
				throw CExpc ("Cannot support this time format (only milliseconds  are supported)");
			
			m_FilmSpeed = MCIWndGetSpeed(m_AudioWnd);
			m_FilmSeconds = MCIWndGetLength(m_AudioWnd)/m_FilmSpeed;
			
		}


	//GetDocument()->SetModifiedFlag();

}


int SavePeriod = -1;


void CSlideShow::OnTimer(UINT nIDEvent) 
{
	if (GetDocument()->m_SlideNo != -1)
	  if (m_bPlay)
		if (!m_bPaused) 
		{
			UINT Period = MCIWndGetPosition(m_AudioWnd)/m_FilmSpeed;
					

			if (Period >= GetDocument()->m_Slides[GetDocument()->m_SlideNo].m_SecondsFromTheStart)
			{
				if (GetDocument()->m_SlideNo + 1  < GetDocument()->m_Slides.size())
				{
					GetDocument()->m_SlideNo++;
					UpdatePeriodsDlg();
					Invalidate();
				}
				
			}
			// "больше" происходит во время редактирования, когда  последние слайды
			// показываются после конца фильма (ошибочно)
			// потом эти слайды будут норально выставлены
			if (Period >= m_FilmSeconds)
			{
				m_bPlay = false;
			};

			/*if (Period != SavePeriod)
			{
				CString LogDebug;
				LogDebug.Format("Slide %i/%i;  Elapsed Time %i:%i", GetDocument()->m_SlideNo+1, GetDocument()->m_Slides.size(), Period/60,Period%60 );
				Log(LogDebug);
			};*/

			
		};

	CFormView::OnTimer(nIDEvent);
}



void PaintSlideInRect(CPaintDC& dc, IPicture* pPicture, const CRect& Rect, bool bDrawRectangle) 
{

	long hmWidth;
	long hmHeight;
	pPicture->get_Width(&hmWidth);
	pPicture->get_Height(&hmHeight);
    float pic_ratio = (float) hmWidth / hmHeight;

    int cx = Rect.right - Rect.left;
    int cy = Rect.bottom - Rect.top;

	float screen_ratio = (float) cx / cy;
	int x = Rect.left;
    int y = Rect.top;


    
    if ( pic_ratio > screen_ratio )
    {
        cy = screen_ratio * cy / pic_ratio;
        y += (Rect.bottom - Rect.top - cy) / 2;
    } else {
        cx = pic_ratio * cx / screen_ratio;
        x += (Rect.right - Rect.top - cx ) / 2;
    }

   if (	bDrawRectangle )
   {
	   const int Offset = 5;
	   CRect r;
	   r.left =  x - Offset;
	   r.top = y - Offset;
	   r.right = x + cx + Offset;
	   r.bottom = y + cy + Offset;
	   dc.Rectangle(&r);
   };

    pPicture->Render( dc, 
                        x, y,                  // destination x,y
                        cx, cy,                // destination cx, cy
                        0, hmHeight,           // xSrc, ySrc
                        hmWidth, -hmHeight, 
                        NULL );


};

void CSlideShow::PaintSlide(CPaintDC& dc) 
{
	CRect R;
	if (m_bFullScreen)
	{
		R.left = 0;
		R.top = 0;
		R.right = GetSystemMetrics( SM_CXSCREEN );
		R.bottom = GetSystemMetrics( SM_CYSCREEN );
	}
	else
	{
		GetClientRect(  &R );
	};

	dc.FillSolidRect(  &R, 0 );

	PaintSlideInRect(dc, GetDocument()->m_pPicture,  R, false);

	if (ShowNextSlide())
	{
		int width = R.right - R.left;
		int height = R.bottom - R.top;
		int y_to_center = 20;
		int x_to_center = 80;
		R.top += height  * 2 / 3 - y_to_center;
		R.left +=  width * 2 / 3 - x_to_center;
		R.bottom -=  y_to_center;
		R.right -=  x_to_center;
		if (GetDocument()->m_pNextPicture != 0)
			PaintSlideInRect(dc, GetDocument()->m_pNextPicture,  R, true);
	};

	

};


void CSlideShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
   	// showing slide
	if (m_pDocument != 0)
	if (GetDocument()->m_SlideNo  != -1)
	{
		bool	bNextSlideWasNotLoaded;

		// когда мы показываем  картинку без следующего кадра
		// и следующий кадр уже загружен, тогда  нужно его просто скопировать,
		// а не читать с диска
		if (	 !ShowNextSlide()
			 &&   GetDocument()->CanUseNextSlide() 
			)
		{
			bNextSlideWasNotLoaded = true;
			GetDocument()->UseNextSlide();
		}
		else
		{
			bNextSlideWasNotLoaded = false;
			GetDocument()->LoadSlide ( );
			GetDocument()->LoadNextSlide ( );
		};

		PaintSlide(dc);

		//после отрисовки  текщего слайда можно  загрузить следующий кадр
		if (bNextSlideWasNotLoaded)
			GetDocument()->LoadNextSlide ( );

		

		if (m_bPaused)
		{
			RECT r;
			GetClientRect(  &r );
			CSize S = dc.GetTextExtent("ПАУЗА");
			CRect r_bar;
			r_bar.left =   (r.right)/2 - (S.cx/2+10);
			r_bar.top =   (r.bottom)/2;
			
			r_bar.right = r_bar.left + S.cx+20;
			r_bar.bottom = r_bar.top + S.cy+20;
			dc.FillSolidRect(  &r_bar, 0x00ffff00 );
			dc.TextOut(r_bar.left + 10, r_bar.top + 10, "ПАУЗА");
		};

	};
	
}

void CSlideShow::OnPause() 
{
	if (!m_bPlay) return;
	CMainFrame* Parent = (CMainFrame*)GetParent();
	if(m_bPaused)
	{
		CMenu* M = GetMenu();
		m_Menu.CheckMenuItem (ID_PAUSE,	MF_BYCOMMAND  | MF_UNCHECKED );

		CToolBarCtrl& C = Parent->m_wndPlayToolBar.GetToolBarCtrl();
		C.PressButton(ID_PAUSE, FALSE);

		MCIWndPlay(m_AudioWnd);
		m_bPaused = FALSE;
		Invalidate();
	}
	else
	{
		MCIWndPause(m_AudioWnd);
		if (Parent->GetMenu() != 0)
			Parent->GetMenu()->CheckMenuItem (ID_PAUSE,	MF_BYCOMMAND  | MF_CHECKED );
		CToolBarCtrl& C = Parent->m_wndPlayToolBar.GetToolBarCtrl();
		C.PressButton(ID_PAUSE);
		m_bPaused = TRUE;
		Invalidate();
		
	}
	

}



void CSlideShow::OnStop() 
{
	if (m_bPaused)
		OnPause();

	MCIWndStop(m_AudioWnd);
	m_bPlay = false;
	m_Menu.EnableMenuItem (ID_PLAY,	MF_BYCOMMAND  | MF_ENABLED );

}


void CSlideShow::SetAudioPosition() 
{
	if (GetDocument()->m_SlideNo == 0)
	{
		MCIWndHome(m_AudioWnd);
	}
	else
	{
		int ElapsedTime = GetDocument()->m_Slides[GetDocument()->m_SlideNo -1].m_SecondsFromTheStart;
		MCIWndSeek(m_AudioWnd, ElapsedTime*m_FilmSpeed);
	};

};

void CSlideShow::Play() 
{
	if (GetDocument()->m_Slides.size() == 0)
	{
		AfxMessageBox ("There is no slides");
		return;

	};
	assert (m_AudioWnd != 0);

	SetAudioPosition();

	MCIWndPlay(m_AudioWnd);
	m_bPaused = false;
	Invalidate();

};
void CSlideShow::OnFilePlay() 
{
	m_Menu.EnableMenuItem (ID_PLAY,	MF_BYCOMMAND  | MF_GRAYED );
	m_bPlay = true;
	Play();
}

void CSlideShow::SetNormalWindow() 
{
	CMainFrame* Parent = (CMainFrame*)GetParent();
	HWND hwnd = Parent->m_hWnd;
	
	m_SavedWindowStyle |= WS_CAPTION;
	m_SavedWindowStyle |= WS_BORDER;
	m_SavedWindowStyle |= WS_SYSMENU;
	m_SavedWindowStyle |= WS_THICKFRAME | WS_MINIMIZEBOX   | WS_MAXIMIZE;
	::SetWindowLong( hwnd, GWL_STYLE, m_SavedWindowStyle );          
	::ShowWindow(hwnd, SW_RESTORE );
	Parent->MoveWindow( &m_SaveNormalRect); 

	Parent->SetMenu(&m_Menu);
	
	Parent->ShowControlBar( &Parent->m_wndPlayToolBar,TRUE, FALSE );
	Parent->ShowControlBar( &Parent->m_wndFileToolBar,TRUE, FALSE );
	Parent->ShowControlBar( &Parent->m_wndStatusBar,TRUE, FALSE );
};	

void CSlideShow::OnFullScreen() 
{
	m_bFullScreen = !m_bFullScreen;
	if (m_bFullScreen)
	{
		CMainFrame* Parent = (CMainFrame*)GetParent();
		HWND hwnd = Parent->m_hWnd;
		Parent->GetWindowRect(  &m_SaveNormalRect );
		
	    const int width = GetSystemMetrics( SM_CXSCREEN );
	    const int height = GetSystemMetrics( SM_CYSCREEN );

		
		RECT work_area;
        ::SystemParametersInfo( SPI_GETWORKAREA, 0, &work_area, 0 );
		
        WORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);        
		m_SavedWindowStyle = dwStyle;
        dwStyle &= ~( WS_CAPTION );
        ::SetWindowLong( hwnd, GWL_STYLE, dwStyle );          
        ::SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, width, height,  SWP_NOZORDER /*| SWP_DRAWFRAME*/ ); 
		::ShowWindow(hwnd, SW_SHOW);

		
		Parent->SetMenu(0);

		Parent->ShowControlBar( &Parent->m_wndPlayToolBar,FALSE, FALSE );
		Parent->ShowControlBar( &Parent->m_wndFileToolBar,FALSE, FALSE );
		Parent->ShowControlBar( &Parent->m_wndStatusBar,FALSE, FALSE );

		
	}
	else
		SetNormalWindow();
	Invalidate();
	
}

void CSlideShow::OnNormalScreen() 
{
	if (m_bFullScreen) 
	{
		SetNormalWindow();

		m_bFullScreen = false;

		Invalidate();
	};

	
	
}


BOOL CSlideShow::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	RECT r;
	GetClientRect(  &r );
	pDC->FillSolidRect(  &r, 0 );
	
	return TRUE;
}

void CSlideShow::UpdatePeriodsDlg() 
{
	if (m_pPeriodsDlg)
	{
		m_pPeriodsDlg->m_bIgnoreChangeSlectTrigger = true;
		m_pPeriodsDlg->m_ListBox.SetItemState( GetDocument()->m_SlideNo, LVIS_SELECTED , LVIS_SELECTED );
		m_pPeriodsDlg->m_ListBox.EnsureVisible( GetDocument()->m_SlideNo, TRUE );
		m_pPeriodsDlg->m_bIgnoreChangeSlectTrigger = false;
		m_pPeriodsDlg->Invalidate();
	};

};

void CSlideShow::SetPosition(int SlideNo, bool bUpdatePeriodsDlg) 
{
	if	(			(GetDocument()->m_SlideNo < 0)
			||		(GetDocument()->m_SlideNo > GetDocument()->m_Slides.size())
		)
	{
		AfxMessageBox ("Cannot Set position");
		return;
	};
	GetDocument()->m_SlideNo = SlideNo;

	if (m_bPlay)
	{
		if (!m_bPaused)
			OnFilePlay();
		else
			SetAudioPosition();
	};

	if (bUpdatePeriodsDlg)
		UpdatePeriodsDlg();
	Invalidate();
};

void CSlideShow::OnGotoSlide() 
{
	CGotoSlideDlg D;
	D.m_MaxSlideNo = GetDocument()->m_Slides.size();
	D.m_SlideNo.Format("%i",GetDocument()->m_SlideNo+1);
	if (D.DoModal() != IDOK) return;
	int SlideNo = atoi(D.m_SlideNo);
	SetPosition(SlideNo - 1, true);
	
	
}

void CSlideShow::OnNextSlide() 
{
	if (GetDocument()->m_Slides.size() == 0) return;
	if (GetDocument()->m_SlideNo + 1 < GetDocument()->m_Slides.size())
		SetPosition(GetDocument()->m_SlideNo + 1, true);

	
}

void CSlideShow::OnPreviousSlide() 
{
	if (GetDocument()->m_Slides.size() == 0) return;
	if (GetDocument()->m_SlideNo > 0)
		SetPosition(GetDocument()->m_SlideNo - 1, true);
	
}

void CSlideShow::OnShowPeriods() 
{
	
	if (m_Menu.GetMenuState(ID_SHOW_PERIODS, MF_BYCOMMAND) & MF_CHECKED)
	{
		m_Menu.CheckMenuItem (ID_SHOW_PERIODS, MF_BYCOMMAND | MF_UNCHECKED);
		assert (m_pPeriodsDlg != 0);
		m_pPeriodsDlg->ShowWindow(SW_HIDE);
		m_pPeriodsDlg->EndDialog(1);
		delete m_pPeriodsDlg;
		m_pPeriodsDlg = 0;
	}
	else
	{
		m_Menu.CheckMenuItem (ID_SHOW_PERIODS, MF_BYCOMMAND | MF_CHECKED);
		assert (m_pPeriodsDlg == 0);
		m_pPeriodsDlg = new CPeriodsDlg ();
		m_pPeriodsDlg->m_pSlideShow = this;
		m_pPeriodsDlg->Create(IDD_PERIODS_DLG, this);
		m_pPeriodsDlg->ShowWindow(SW_SHOW);
		
		
	};
	Invalidate();
	
}


void CSlideShow::ShowNextSlideAndRememberPeriod() 
{
	vector<CSlide>&	Slides = GetDocument()->m_Slides;
	UINT Period = MCIWndGetPosition(m_AudioWnd)/m_FilmSpeed;
	int& SlideNo = GetDocument()->m_SlideNo;
	{
		CSlide SaveSlide =  Slides[SlideNo];
		Slides[SlideNo].m_SecondsFromTheStart = Period;
	};

	m_pPeriodsDlg->m_bIgnoreChangeSlectTrigger = true;
	m_pPeriodsDlg->CopySlideToBox(SlideNo);
	m_pPeriodsDlg->m_bIgnoreChangeSlectTrigger = false;

	if (SlideNo + 1 < Slides.size())
		SlideNo++;

	Invalidate();
	
	UpdatePeriodsDlg();
	GetDocument()->SetModifiedFlag();	
}


void CSlideShow::OnHome() 
{
	if (GetDocument()->m_Slides.size() == 0) return;
	SetPosition(0, true);
		
}

void CSlideShow::OnEnd() 
{
	if (GetDocument()->m_Slides.size() == 0) return;
	SetPosition(GetDocument()->m_Slides.size() - 1 , true);
	
}

void CSlideShow::OnShowFilmList() 
{
	((CPechatnikiApp*)AfxGetApp())->ShowFilmLists();
}


bool	FindFilesWithPath(const CString& filename, vector<CString>& results)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = INVALID_HANDLE_VALUE;
	CString	pathname;
	int posPath = filename.ReverseFind('\\');
	CString path = filename.Left(posPath + 1);
	vector<CString>::iterator	loc_name;
	hFindFile = ::FindFirstFile(path+"*.jpg", &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return (false);
	do 
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			pathname = CString(FindFileData.cFileName);
			loc_name = lower_bound(results.begin(), results.end(), pathname);
			if (	(loc_name == results.end())
				||	(*loc_name != pathname))
				results.insert(loc_name, pathname);
		}
	}	while (::FindNextFile(hFindFile, &FindFileData));
	::FindClose(hFindFile);
	return (true);
}



void CSlideShow::OnCreateSlimFile() 
{
	CFileDialog cDlg(TRUE,NULL, "audio.mp3",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		"MP3(*.mp3)|*.mp3|All files(*.*)|*.*||", AfxGetMainWnd());
	cDlg.m_ofn.lpstrTitle = " Input path to audio file:";

	if( cDlg.DoModal() != IDOK )
		return;

	CPechatnikiDoc D;
	D.m_AudioPath = cDlg.GetPathName();
	
	D.m_FilmName = "unk";
	D.m_Copyright = "unk";
	D.m_Author = "unk";
	vector<CString> Files;
	FindFilesWithPath (D.m_AudioPath, Files);
	for (int i =0; i<Files.size(); i++)
	{
		CSlide C;
		C.m_FileNameInSmil = Files[i];
		C.m_SecondsFromTheStart = (i+1)*10;
		D.m_Slides.push_back(C);
	}
	D.m_bCheckExistanceOfFilesWhileLoading = false;
	int posPath = D.m_AudioPath.ReverseFind('\\');
	CString path = D.m_AudioPath.Left(posPath + 1);

	CFile F(path+"smil.smil",CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&F, CArchive::store);
	D.Serialize(ar);

}
