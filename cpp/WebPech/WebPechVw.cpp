// This is part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// WebPechVw.cpp : implementation of the CWebPechView class
//

#include "stdafx.h"
#include "WebPech.h"
#include "MainFrm.h"
#include <vector>
#include "WebPechDoc.h"
#include "WebPechVw.h"
#include <stdlib.h>
#include <direct.h>
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebPechView

IMPLEMENT_DYNCREATE(CWebPechView, CHtmlView)

BEGIN_MESSAGE_MAP(CWebPechView, CHtmlView)
	//{{AFX_MSG_MAP(CWebPechView)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_HELP_WEB_TUTORIAL, OnHelpWebTutorial)
	ON_COMMAND(ID_HELP_ONLINE_SUPPORT, OnHelpOnlineSupport)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREE_STUFF, OnHelpMicrosoftOnTheWebFreeStuff)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREQUENTLY_ASKED_QUESTIONS, OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_GET_FASTER_INTERNET_ACCESS, OnHelpMicrosoftOnTheWebGetFasterInternetAccess)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_MICROSOFT_HOME_PAGE, OnHelpMicrosoftOnTheWebMicrosoftHomePage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEARCH_THE_WEB, OnHelpMicrosoftOnTheWebSearchTheWeb)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEND_FEEDBACK, OnHelpMicrosoftOnTheWebSendFeedback)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_INTERNET_START_PAGE, OnHelpMicrosoftOnTheWebInternetStartPage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_BEST_OF_THE_WEB, OnHelpMicrosoftOnTheWebBestOfTheWeb)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebPechView construction/destruction

CWebPechView::CWebPechView()
{
}

CWebPechView::~CWebPechView()
{
}

BOOL CWebPechView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxRegisterWndClass(0);
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWebPechView drawing

void CWebPechView::OnDraw(CDC* /*pDC*/)
{
	CWebPechDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CWebPechView diagnostics

#ifdef _DEBUG
void CWebPechView::AssertValid() const
{
	CView::AssertValid();
}

void CWebPechView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWebPechDoc* CWebPechView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWebPechDoc)));
	return (CWebPechDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebPechView message handlers

void CWebPechView::OnTitleChange(LPCTSTR lpszText)
{
	// this will change the main frame's title bar
	if (m_pDocument != NULL)
		m_pDocument->SetTitle(lpszText);
}

void CWebPechView::OnDocumentComplete(LPCTSTR lpszUrl)
{
	// make sure the main frame has the new URL.  This call also stops the animation
	((CMainFrame*)GetParentFrame())->SetAddress(lpszUrl);
}

void CWebPechView::OnInitialUpdate()
{
	// Go to the home page initially
	CHtmlView::OnInitialUpdate();
	CString strCmdLine(AfxGetApp()->m_lpCmdLine);
	if(strCmdLine == _T(""))
	{
		strCmdLine = "index.html";
	}
	{
        if ( strCmdLine.Find("/") == -1 && strCmdLine.Find("\\")) {
            CString dir = _getcwd( NULL, 0 );
            if (dir.Right(1) != "\\") {
                dir += CString("\\");
            }
            strCmdLine = dir + strCmdLine;
        }
     	//AfxMessageBox (strCmdLine);
        Navigate2(strCmdLine, 0, NULL);
		//Navigate(strCmdLine);
	}
}

void RunPechatniki (CString toto)
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION  pi;
    toto =  "Pechatniki.exe /immed " + toto;
    std::vector<TCHAR> V( toto.GetLength() + 1);
    for (int i=0; i< (int)toto.GetLength();i++)
        V[i] = toto[i];
   CreateProcess(NULL, &V[0],0, 0, FALSE, 0, 0, 0, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void CWebPechView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
	LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
	LPCTSTR lpszHeaders, BOOL* pbCancel)
{
    AfxMessageBox(lpszURL);
    if (CString (lpszURL).Right(5) == ".smil") {
//       AfxMessageBox("Smil detected");
       *pbCancel = TRUE;     
       RunPechatniki(lpszURL);
    }
    else {
	    // start the animation so that is plays while the new page is being loaded
	    ((CMainFrame*)GetParentFrame())->StartAnimation();
    }
}

// these are all simple one-liners to do simple controlling of the browser
void CWebPechView::OnGoBack()
{
	GoBack();
}

void CWebPechView::OnGoForward()
{
	GoForward();
}

void CWebPechView::OnGoSearchTheWeb()
{
	GoSearch();
}

void CWebPechView::OnGoStartPage()
{
	GoHome();
}

void CWebPechView::OnViewStop()
{
	Stop();
}

void CWebPechView::OnViewRefresh()
{
	Refresh();
}

// these all go to specific web pages, just like Internet Explorer's help menu
void CWebPechView::OnHelpWebTutorial()
{
	CString str;

	str.LoadString(IDS_WEB_TUTORIAL);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpOnlineSupport()
{
	CString str;

	str.LoadString(IDS_ONLINE_SUPPORT);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebBestOfTheWeb()
{
	CString str;

	str.LoadString(IDS_BEST_OF_THE_WEB);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebFreeStuff()
{
	CString str;

	str.LoadString(IDS_FREE_STUFF);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions()
{
	CString str;

	str.LoadString(IDS_FREQUENTLY_ASKED_QUESTIONS);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebGetFasterInternetAccess()
{
	CString str;

	str.LoadString(IDS_GET_FASTER_INTERNET_ACCESS);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebMicrosoftHomePage()
{
	CString str;

	str.LoadString(IDS_MICROSOFT_HOME_PAGE);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebSearchTheWeb()
{
	CString str;

	str.LoadString(IDS_SEARCH_THE_WEB);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebSendFeedback()
{
	CString str;

	str.LoadString(IDS_SEND_FEEDBACK);
	Navigate2(str, 0, NULL);
}

void CWebPechView::OnHelpMicrosoftOnTheWebInternetStartPage()
{
	CString str;

	str.LoadString(IDS_INTERNET_START_PAGE);
	Navigate2(str, 0, NULL);
}

// these functions control the font size.  There is no explicit command in the
// CHtmlView class to do this, but we can do it by using the ExecWB() function.
void CWebPechView::OnViewFontsLargest()
{
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebPechView::OnViewFontsLarge()
{
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebPechView::OnViewFontsMedium()
{
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebPechView::OnViewFontsSmall()
{
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebPechView::OnViewFontsSmallest()
{
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

// This demonstrates how we can use the Navigate2() function to load local files
// including local HTML pages, GIFs, AIFF files, etc.
void CWebPechView::OnFileOpen()
{
	CString str;

	str.LoadString(IDS_FILETYPES);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);

	if(fileDlg.DoModal() == IDOK)
		Navigate2(fileDlg.GetPathName(), 0, NULL);
}
