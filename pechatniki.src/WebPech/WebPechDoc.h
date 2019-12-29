// This is part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// WebPechDoc.h : interface of the CWebPechDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WebPechDOC_H__47FF4083_CE1B_11D0_BEB6_00C04FC99F83__INCLUDED_)
#define AFX_WebPechDOC_H__47FF4083_CE1B_11D0_BEB6_00C04FC99F83__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CWebPechDoc : public CDocument
{
protected: // create from serialization only
	CWebPechDoc();
	DECLARE_DYNCREATE(CWebPechDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebPechDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWebPechDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWebPechDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WebPechDOC_H__47FF4083_CE1B_11D0_BEB6_00C04FC99F83__INCLUDED_)
