// This is part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// WebPechDoc.cpp : implementation of the CWebPechDoc class
//

#include "stdafx.h"
#include "WebPech.h"

#include "WebPechDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebPechDoc

IMPLEMENT_DYNCREATE(CWebPechDoc, CDocument)

BEGIN_MESSAGE_MAP(CWebPechDoc, CDocument)
	//{{AFX_MSG_MAP(CWebPechDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebPechDoc construction/destruction

CWebPechDoc::CWebPechDoc()
{
}

CWebPechDoc::~CWebPechDoc()
{
}

BOOL CWebPechDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWebPechDoc serialization

void CWebPechDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWebPechDoc diagnostics

#ifdef _DEBUG
void CWebPechDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWebPechDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebPechDoc commands
