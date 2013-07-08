#if !defined(AFX_TESTCLIENT_H__0D517305_5DEC_4991_AF6A_A69DB0036723__INCLUDED_)
#define AFX_TESTCLIENT_H__0D517305_5DEC_4991_AF6A_A69DB0036723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestClient.h : header file
//

#include "AsynSvSk.h"
#include "Header.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CTestClient window

class CTestClient : public CWnd
{
// Construction
public:
	CTestClient(CDialog* pDlg, int index);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestClient)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bResponding;						//Clinet respond and control flags
	BOOL m_bCPflag;
	BOOL SendHeader(BOOL flag, DWORD size);
	Header m_header;
	BOOL m_bRunning;
	//CFile outfile;
	int first_time;
	int m_index;
	BOOL GetName();
	CString m_strSockName;
	int Send(CString str);
	CAsynSvSk * m_pSrvrSock;
	CDialog * m_pTDlg;
	//Added for MS Meter 2
	CString CreateHeaderStr(BOOL flag, DWORD size); //create header string for worker config

	virtual ~CTestClient();



	// Generated message map functions
protected:
	//{{AFX_MSG(CTestClient)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCLIENT_H__0D517305_5DEC_4991_AF6A_A69DB0036723__INCLUDED_)
