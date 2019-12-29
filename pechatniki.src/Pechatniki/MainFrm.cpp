// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Pechatniki.h"
#include "PechatnikiDoc.h"
#include "SlideShow.h"


#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndPlayToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP 
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndPlayToolBar.LoadToolBar(IDR_PLAY_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	};

	if (!m_wndFileToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP 
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndFileToolBar.LoadToolBar(IDR_FILE_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	

	//CRect R;
	//m_wndFileToolBar.GetWindowRect(R);
	//int cx = R.right;
	//int cy = R.bottom; 
	//m_wndPlayToolBar.SetWindowPos(0,100,  0,  0, 0, SWP_NOSIZE | 		SWP_SHOWWINDOW);
	//m_wndPlayToolBar.MoveWindow(100,  0,  100, 30, TRUE);
	


	

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndPlayToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	
	EnableDocking(CBRS_ALIGN_ANY);

	

	CRect R;
	GetWindowRect(R);
	DockControlBar(&m_wndFileToolBar, CBRS_ALIGN_ANY, &R);

	R.left += 50;
	DockControlBar(&m_wndPlayToolBar, CBRS_ALIGN_ANY, &R);

	

	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}


