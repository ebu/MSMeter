#if !defined(AFX_PSHEET_H__1B817C23_CF93_40DA_8446_0C08E8C12CD3__INCLUDED_)
#define AFX_PSHEET_H__1B817C23_CF93_40DA_8446_0C08E8C12CD3__INCLUDED_

#include "Control.h"	// Added by ClassView
#include "Files.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPSheet: Control Panel window class

class CPSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPSheet)

// Construction
public:
	CPSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	CFiles m_filespage;
	CControl m_controlpage;
	virtual ~CPSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPSheet)
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// SPSheet: Script Control Panel Class

class SPSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(SPSheet)

// Construction
public:
	SPSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	SPSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	CFiles m_filespage;
	//Point to script control page
	SControl m_controlpage;
	virtual ~SPSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPSheet)
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSHEET_H__1B817C23_CF93_40DA_8446_0C08E8C12CD3__INCLUDED_)
