// CtrlApp.h : main header file for the CTRLAPP application
//

#if !defined(AFX_PROPERTY2_H__14096242_BE33_427F_8B63_7A039284964A__INCLUDED_)
#define AFX_PROPERTY2_H__14096242_BE33_427F_8B63_7A039284964A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//#include "PSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CControlApp:
// See CtrlApp.cpp for the implementation of this class
//

class CControlApp : public CWinApp
{
public:
	CControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CControlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
//	CPSheet * m_ps;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTY2_H__14096242_BE33_427F_8B63_7A039284964A__INCLUDED_)
