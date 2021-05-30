// PechatnikiDoc.cpp : implementation of the CPechatnikiDoc class
//

#include "stdafx.h"
#include "assert.h"
#include "Pechatniki.h"

#include "PechatnikiDoc.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiDoc

IMPLEMENT_DYNCREATE(CPechatnikiDoc, CDocument)

BEGIN_MESSAGE_MAP(CPechatnikiDoc, CDocument)
	//{{AFX_MSG_MAP(CPechatnikiDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiDoc construction/destruction

CPechatnikiDoc::CPechatnikiDoc()
{
	m_SlideNo = -1;
	m_LastLoadedFileName = "";
	m_LastLoadedNextFileName = "",
	m_pPicture = 0;
	m_pNextPicture = 0;
	m_bCheckExistanceOfFilesWhileLoading = true;
}

CPechatnikiDoc::~CPechatnikiDoc()
{
   if ( m_pPicture )
    {
        m_pPicture->Release();
        m_pPicture = 0;
    }
  if ( m_pNextPicture )
    {
        m_pNextPicture->Release();
        m_pNextPicture = 0;
    }
}

BOOL CPechatnikiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPechatnikiDoc serialization

void CPechatnikiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		SaveSmilFile(  ar );

	}
	else
	{
		if (!ReadSmilFile(  ar ) )
		{
			m_Slides.clear();
			return;
		};
      		
		if (m_Slides.size () > 0)
			m_SlideNo = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPechatnikiDoc diagnostics

#ifdef _DEBUG
void CPechatnikiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPechatnikiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

CString UTF8toUSC2(CString s)
{
    wchar_t MBout[512];
    MultiByteToWideChar(CP_UTF8,0,(LPCSTR)s,-1, MBout, 512);
    return CString(MBout);
}

void CPechatnikiDoc::SetTitle(LPCTSTR lpszTitle) 
{
    
    CDocument::SetTitle(UTF8toUSC2 (m_FilmName) );
}

inline unsigned long FileSize (FILE *fp) 
 {
   long  pos,size;
   assert (fp != NULL);
	pos = ftell (fp);
   size = lseek (fileno(fp),0,SEEK_END);
   fseek(fp,pos,SEEK_SET);
   return (size);
 }


void LoadPicture( const string &filename, IPicture*& pPicture ) 
{
	if ( pPicture != NULL)
		pPicture->Release();

    pPicture = NULL;
    //ifstream file( filename.c_str(), ios_base::binary | ios_base::in );
	FILE *file = fopen(filename.c_str(), "rb");
    if ( file )
    {
        //file.seekg( 0, ios_base::end );
        //long length = file.tellg();
        //file.seekg( 0, ios_base::beg );
		long length = lseek (fileno(file),0,SEEK_END);
		fseek(file, 0, SEEK_SET);

        HGLOBAL hGlobal = GlobalAlloc( GMEM_MOVEABLE, length );
	    void* p = GlobalLock(hGlobal);
        //file.read( (char *) p, length );
		fread((char *) p, 1, length, file);
		fclose (file);
	    GlobalUnlock(hGlobal);
	    IStream* pStream = NULL;
	    if ( CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
        {
		    HRESULT hr = ::OleLoadPicture( pStream, 
                                           length, 
                                           FALSE, 
                                           IID_IPicture, 
                                           (LPVOID *) &pPicture );
		    pStream->Release();
        }


    }
};


string globalFilename;
UINT LoadPictureInThread    ( LPVOID pParam )
{
	IPicture** pNextPicture = (IPicture**)pParam;
	LoadPicture(globalFilename, *pNextPicture);
	return 0;
};






void CPechatnikiDoc::LoadNextSlide(  )
{
	if (m_Slides.size() == 0) return;

	// loading next slide
	if (m_SlideNo+1 < m_Slides.size())
	{
		const string &filename = (const char*)m_Slides[m_SlideNo+1].m_FullPath;
		if (m_LastLoadedNextFileName != filename.c_str() ) 
		{
			m_LastLoadedNextFileName = filename.c_str();

			//globalFilename = filename;

			//AfxBeginThread(LoadPictureInThread, &m_pNextPicture, THREAD_PRIORITY_LOWEST);
			
			LoadPicture(filename, m_pNextPicture);
		};
	}
	else
	{
		m_pNextPicture = 0;
		m_LastLoadedNextFileName = "";
	};

};


void CPechatnikiDoc::LoadSlide(  )
{
	if (m_Slides.size() == 0) return;

	const string &filename = (const char*)m_Slides[m_SlideNo].m_FullPath;
	if (m_LastLoadedFileName != filename.c_str() ) 
	{
		m_LastLoadedFileName = filename.c_str();
		LoadPicture(filename, m_pPicture);
	};



}

void  CPechatnikiDoc::UseNextSlide()
{
	if (m_Slides.size() == 0) return;
	m_LastLoadedFileName = m_LastLoadedNextFileName;
	if ( m_pPicture != NULL)
		m_pPicture->Release();

    m_pPicture = NULL;

	if (m_pNextPicture == 0)  return;

	m_pPicture = m_pNextPicture;
	
	m_pPicture->AddRef();


};



struct CParameter
{
	CString m_Name;
	CString	m_Value;
};

struct CTag {
	CString m_Source;
	CString m_Name;
	vector <CParameter> m_Parameters;
	void	WriteToFile (FILE* fp) const 
	{
		fprintf (fp, "<%s ", m_Name);
		for (int i =0; i<m_Parameters.size(); i++)
		{
			fprintf (fp, " %s = \"%s\" ", m_Parameters[i].m_Name, m_Parameters[i].m_Value);
			
		};


		fprintf (fp, ">\n");
	};
};


inline string GetPathByFile (string FileName)
{ 
  int i = FileName.rfind ("\\");	  
  if (i ==-1)
	  i = FileName.rfind ("/");	  
  if (i == -1) return "";
  return FileName.substr(0, i+1);
};


bool GetTag (const CString& Buffer, int& Index, CTag& Tag)
{
	int LastCharNo;
    int i = Index;
	for (; i < Buffer.GetLength(); i++)
		if (Buffer[i] == '<')
		{
            int j = i;
			for (; j < Buffer.GetLength(); j++)
				if (Buffer[j] == '>')
					break;
			Tag.m_Source = Buffer.Mid(i, j - i + 1);
			
			LastCharNo = j;
			break;
		};
	if (i ==  Buffer.GetLength())
	{
		Index = i;
		return false;
	};

	// getting the name of tag
	CString S = Tag.m_Source;
	S = Tag.m_Source.Mid(1, Tag.m_Source.GetLength() - 2 );

	S.TrimLeft();
	Tag.m_Name = S.SpanExcluding(" \r\n\t");
	if (Tag.m_Name == "")
		return false;
	S.Delete(0, Tag.m_Name.GetLength());
	Tag.m_Name.MakeUpper();

	Tag.m_Parameters.clear();
	while (S.GetLength() > 0)
	{
		S.TrimLeft();
		if (S == "" || S == "/") break;
		CParameter P;
		P.m_Name = S.SpanExcluding("=");
		S.Delete(0,  P.m_Name.GetLength()+1);
		P.m_Name.TrimRight();
		P.m_Name.MakeUpper();

		// after equal char can be spaces
		int end = S.SpanIncluding(" \r\n\t").GetLength();
		if (end == -1) break;

		// all values shoud be in quotation marks 
		if (S[end] != '\"')
			return false;
		// delete spaces after equal char and the open quotation mark
		S.Delete(0, end+1);

		// find the close quotation mark and getting Value
		P.m_Value = S.SpanExcluding("\"");
		P.m_Value.TrimLeft();
		P.m_Value.TrimRight();
		
		end = P.m_Value.GetLength();
		// the close quotation mark should really exist
		if (S[end] != '\"')
			return false;
		
		// delete Value with quotation marks
		S.Delete(0, end+1);

		// adding a new parameter
		Tag.m_Parameters.push_back(P);
		
	};
	Index = LastCharNo;
	return true;
};

bool CheckFile(CString SmilLine, CString& FileName, CString Folder) 
{
	SmilLine.TrimLeft();
	if (SmilLine.IsEmpty()) 
	{
		AfxMessageBox ("Cannot read a empty file name ");
		return false;
	};
	FileName = Folder + SmilLine;
	if (access (FileName, 04) != 0)
	{
		AfxMessageBox ("Cannot read a file " +FileName);
		return false;
	};

	return true;

};

bool  CPechatnikiDoc::ReadSmilAsString(CArchive& ar, CString& Smil) 
{
	Smil = "";
	char buffer [10000];
	while  (ar.ReadString(buffer, 10000))
	{
			Smil += buffer;
	};
	return true;

};

bool CPechatnikiDoc::ReadSmilFile(CArchive& ar) 
{
	CString SmilFileName = ar.GetFile()->GetFilePath();

	m_Folder = GetPathByFile((const char*)SmilFileName).c_str();

	m_Slides.clear();

	CString Smil;

	if (!ReadSmilAsString (ar, Smil)) return false;

	CTag Tag;
	int Index = 0;
	while (GetTag (Smil, Index, Tag))
	{
        CString TagNameUpper = Tag.m_Name;
        TagNameUpper.MakeUpper();
		if (TagNameUpper == "META" )
		{
			CString NameValue;
			for (int i = 0; i< Tag.m_Parameters.size(); i++)
			{
		        CString ParamNameUpper = Tag.m_Parameters[i].m_Name;
                ParamNameUpper.MakeUpper();    
        		if	(ParamNameUpper == "NAME")
				{
					NameValue = Tag.m_Parameters[i].m_Value;
					NameValue.MakeUpper();
				};
				if	(ParamNameUpper == "CONTENT")
				{
					if (NameValue == "TITLE")
						m_FilmName =  Tag.m_Parameters[i].m_Value;

					if (NameValue == "AUTHOR")
						m_Author =  Tag.m_Parameters[i].m_Value;

					if (NameValue == "COPYRIGHT")
						m_Copyright =  Tag.m_Parameters[i].m_Value;
				};

			};
		};

		if (TagNameUpper == "IMG" )
		{
			CSlide Slide;
			Slide.m_FullPath = "";
			Slide.m_SecondsFromTheStart = 0;
			if ( ! m_Slides.empty() )
				Slide.m_SecondsFromTheStart = m_Slides.back().m_SecondsFromTheStart + 1;

			for (int i = 0; i< Tag.m_Parameters.size(); i++)
			{
		        CString ParamNameUpper = Tag.m_Parameters[i].m_Name;
                ParamNameUpper.MakeUpper();    
				if (ParamNameUpper == "SRC")
				{
					Slide.m_FileNameInSmil = Tag.m_Parameters[i].m_Value;

					if (m_bCheckExistanceOfFilesWhileLoading)
						if (!CheckFile (Tag.m_Parameters[i].m_Value, Slide.m_FullPath, m_Folder))
							return false;
				};

				if (ParamNameUpper == "DUR")
				{
					CString  S =  Tag.m_Parameters[i].m_Value;
					if (S.GetLength() == 0) return false;
					char LastChar  = S[S.GetLength() - 1];
					if (	(LastChar != 'S') &&  (LastChar != 's'))
						return false;
					S.Delete(S.GetLength() - 1);
					int SecondsFromThePrevious = atoi(S);
					if (SecondsFromThePrevious == 0)
					{
						AfxMessageBox ("Cannot read duration from " + S);
						return false;
					};
					if ( ! m_Slides.empty() )
						Slide.m_SecondsFromTheStart = m_Slides.back().m_SecondsFromTheStart + SecondsFromThePrevious;
					else
						Slide.m_SecondsFromTheStart = SecondsFromThePrevious;

				};
			};
			if (m_bCheckExistanceOfFilesWhileLoading)
				if (Slide.m_FullPath == "")
				{
					AfxMessageBox ("Cannot find source filename in " + Tag.m_Source);
					return false;
				};
			if (Slide.m_SecondsFromTheStart == 0)
			{
				AfxMessageBox ("Cannot find duration value in  " + Tag.m_Source);
				return false;
			};
			m_Slides.push_back(Slide);
		};

		if (TagNameUpper == "AUDIO" )
		{
			for (int i = 0; i< Tag.m_Parameters.size(); i++)
			{
	            CString ParamNameUpper = Tag.m_Parameters[i].m_Name;
                ParamNameUpper.MakeUpper();    
				if (ParamNameUpper == "SRC")
					if (m_bCheckExistanceOfFilesWhileLoading)
						if (!CheckFile (Tag.m_Parameters[i].m_Value, m_AudioPath, m_Folder))
							return false;
			};
			
		};

	};
	if (Index != Smil.GetLength()) 
	{
		AfxMessageBox ("Cannot read smil file from  " + Smil.Mid(Index, 50));
		return false;
	};
	CString LogDebug;
	LogDebug.Format("%i slides read", m_Slides.size());
	Log(LogDebug);
	m_LastLoadedFileName = "";
	m_LastLoadedNextFileName = "";
	m_SlideNo = -1;
	if (m_bCheckExistanceOfFilesWhileLoading)
		if (!CheckExistance()) 
			return false;


	return true;
};



bool CPechatnikiDoc::CheckExistance()  
{
	for (int i = 0;  i < m_Slides.size(); i++)
	{
			if (!CheckFile(m_Slides[i].m_FileNameInSmil, m_Slides[i].m_FullPath, m_Folder))
				return false;
	};

	return true;
};

const  char SmilHeader[] = 
"<smil xmlns=\"http://www.w3.org/2001/SMIL20/Language\">\r\n"\
"	<head>\r\n"\
"		<meta name=\"title\" content=\"%s\"/>\r\n"\
"		<meta name=\"author\" content=\"%s\"/>\r\n "\
"		<meta name=\"copyright\" content=\"%s\"/>\r\n"\
"		<layout>\r\n"\
"		<root-layout width=\"640\" height=\"480\" backgroundColor=\"white\"/>\r\n"\
"		<region id=\"images\" width=\"640\" height=\"480\" backgroundColor=\"black\" z-index=\"1\" fit=\"meet\"/>\r\n"\
"		</layout>\r\n"\
"	</head>\r\n"\
"	<body>\r\n"\
" <par syncTolerance=\"1s\">\r\n"\
"<seq syncBehavior=\"locked\">\r\n";

bool CPechatnikiDoc::SaveSmilFile(CArchive& ar) 
{
	CString S;
	S.Format( SmilHeader, m_FilmName, m_Author, m_Copyright);
	ar.WriteString(S);
	for (int i = 0; i < m_Slides.size(); i++)
	{
		S.Format("<img src=\"%s\" region=\"images\" regPoint=\"center\" regAlign=\"center\" dur=\"%is\"/>\r\n", m_Slides[i].m_FileNameInSmil, GetDuration(i), "s");
		ar.WriteString(S);
	};
	S.Format("</seq>\r\n");
	ar.WriteString(S);
	
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

	_splitpath( m_AudioPath, drive, dir, fname, ext );

	S.Format("<audio src=\"%s%s\" syncBehavior=\"locked\"/>\r\n",  fname, ext);
	ar.WriteString(S);
	S.Format("</par>\r\n</body>\r\n</smil>\r\n");
	ar.WriteString(S);

	return true;
};
