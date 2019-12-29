// Pechatniki.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Pechatniki.h"

#include "MainFrm.h"
#include "PechatnikiDoc.h"
#include "SlideShow.h"
#include "io.h"
#include "FilmList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiApp

BEGIN_MESSAGE_MAP(CPechatnikiApp, CWinApp)
	//{{AFX_MSG_MAP(CPechatnikiApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SHOW_FILM_LIST, OnShowFilmList)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiApp construction

CPechatnikiApp::CPechatnikiApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPechatnikiApp object

CPechatnikiApp theApp;

void CPechatnikiApp::ShowFilmLists()
{
	CString Path = AfxGetApp()->m_pszHelpFilePath;
	int LenToDelete = strlen("Pechatniki.hlp");
	if (Path.GetLength() < LenToDelete) return;
	Path.Delete(Path.GetLength()-LenToDelete, LenToDelete);

	CString LstFileName = Path + "Pechatniki.lst";
	if ( access(LstFileName, 04) ) return;
	FILE * fp = fopen (LstFileName, "r");
	if (!fp) return;

	CFilmList FilmListDlg;
	char buffer[1024];	
	int CountOfFilms = 0;
	while (fgets(buffer, 1024, fp))
	{
		CString R = buffer;
		R.TrimLeft();
		R.TrimRight();
		CString FileName = Path + R;
		if (access(FileName, 04))
		{
			AfxMessageBox ("Cannot access file from the film list" + FileName);
			break;
		};
		CString Ext = FileName.Right(4);
		Ext.MakeLower();
		if (Ext != "smil")
		{
			AfxMessageBox ("Only smil-files  can be in the film list ("+FileName + ")");
			break;
		};
		CPechatnikiDoc D;
		D.m_bCheckExistanceOfFilesWhileLoading = false;
		CFile F(FileName,CFile::modeRead);
		CArchive ar(&F, CArchive::load);
		D.Serialize(ar);
		if (!D.m_Slides.empty())
		{
			int Duration = D.m_Slides.back().m_SecondsFromTheStart/60;
			if (Duration == 0) continue;
			CString DurationStr;
			DurationStr.Format("%i:%02i", Duration/60, Duration%60);

			int YearOfCreation = 0;
			if (		(D.m_Copyright.Left(3) == "(c)")
					||	(D.m_Copyright.Left(3) == "(C)")
				)

				YearOfCreation	= atoi(D.m_Copyright.Right(D.m_Copyright.GetLength() - 3));
			
			CFilm Film(FileName, D.m_FilmName, D.m_Slides.size(), DurationStr, YearOfCreation);
			FilmListDlg.m_Films.push_back(Film);
			CountOfFilms++;
		};
		
	};
	fclose(fp);

	if (CountOfFilms > 0)
		FilmListDlg.DoModal();
};

class CPCommandLineInfo : public CCommandLineInfo {
public:
    BOOL m_bPlayImmediate;
	// Sets default values
    CPCommandLineInfo():CCommandLineInfo() {
        m_bPlayImmediate = FALSE;
    };

	// plain char* version on UNICODE for source-code backwards compatibility
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast) {
        CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
        if (bFlag && CString(pszParam) == "immed") {
            m_bPlayImmediate = TRUE;
        }
    }

};

BOOL CPechatnikiApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_SMILFILENAME,
		RUNTIME_CLASS(CPechatnikiDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSlideShow));
	AddDocTemplate(pDocTemplate);

 	CPCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	m_pMainWnd = 0;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
			return FALSE;

    if (cmdInfo.m_bPlayImmediate) {
        POSITION pos = GetFirstDocTemplatePosition();;
        while (pos)
        {
            CDocTemplate* pTemplate = (CDocTemplate*)GetNextDocTemplate(pos);
            POSITION pos2 = pTemplate->GetFirstDocPosition();
            while (pos2)
            {
             CDocument * pDocument;
             if ((pDocument=pTemplate->GetNextDoc(pos2)) != NULL){
                 if ( pDocument->IsKindOf(RUNTIME_CLASS(CPechatnikiDoc))) {
                      POSITION pos3 = pDocument->GetFirstViewPosition();
                      while (pos3 != NULL)
                      {
                            CSlideShow* pView = (CSlideShow*)pDocument->GetNextView(pos3);
                            pView->OnFilePlay();
                      }
                 }
             }
            }
        }
    }

	ShowFilmLists();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPechatnikiApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiApp message handlers

CDocument* CPechatnikiApp::OpenDocumentFile( LPCTSTR lpszFileName )
{
	try 
	{
		return CWinApp::OpenDocumentFile(lpszFileName);
	}
	catch (CExpc _E)
	{
		AfxMessageBox (_E.m_strCause);
	}
	catch (...)
	{

	};
	return NULL;
	
}



void CPechatnikiApp::OnShowFilmList() 
{
	ShowFilmLists();	
	
}
