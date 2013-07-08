// PSheet.cpp : implementation file
//

#include "stdafx.h"
#include "CtrlApp.h"
#include "PSheet.h"
#include "Header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPSheet: Control Panel window class

IMPLEMENT_DYNAMIC(CPSheet, CPropertySheet)

CPSheet::CPSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	
	m_controlpage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	m_filespage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	AddPage(&m_controlpage);
	AddPage(&m_filespage);
	
}

CPSheet::CPSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_controlpage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	m_filespage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	AddPage(&m_controlpage);
	AddPage(&m_filespage);

}

CPSheet::~CPSheet()
{
}


BEGIN_MESSAGE_MAP(CPSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPSheet)
	ON_BN_CLICKED(ID_APPLY_NOW, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSheet message handlers

void CPSheet::OnApply() 
{
	// TODO: Add your message handler code here and/or call default
	GetActivePage()->UpdateData(TRUE);
	//AfxMessageBox(m_controlpage.m_sTestFilePath);

	Header h;					// Set up Header object to return data
	// and fill in data values
	h.testfilepath = m_controlpage.m_sTestFilePath;
	h.maxnum = m_controlpage.m_nMaxnum;
	h.maxlength = m_controlpage.m_dMaxlength;
	h.maxdelay = m_controlpage.m_nMaxdelay;
	h.pcwrite = m_controlpage.m_nPCwrite;
	h.pcrand = m_controlpage.m_nPCrandom;
	h.verbose = m_controlpage.m_bVerbose;
	h.changed = TRUE;

	//Send message back to main dialog window with pointer to return data
	GetParent()->SendMessage(WM_USER_APPLY, 0, (LPARAM) &h);
	//Restore non-modified status
	m_controlpage.SetModified(FALSE);
	m_filespage.SetModified(FALSE);
}





/////////////////////////////////////////////////////////////////////////////
// SPSheet: Script Control Panel Class

IMPLEMENT_DYNAMIC(SPSheet, CPropertySheet)

SPSheet::SPSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &= ~(PSH_HASHELP); //Disable help button

	m_controlpage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	//m_filespage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	AddPage(&m_controlpage);
	//AddPage(&m_filespage);
}

SPSheet::SPSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_controlpage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	//m_filespage.m_psp.dwFlags &= ~(PSP_HASHELP); //Disable help button
	AddPage(&m_controlpage);
	//AddPage(&m_filespage);
}

SPSheet::~SPSheet()
{
}


BEGIN_MESSAGE_MAP(SPSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPSheet)
	ON_BN_CLICKED(ID_APPLY_NOW, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SPSheet message handlers

void SPSheet::OnApply() 
{
	// TODO: Add your message handler code here and/or call default
	GetActivePage()->UpdateData(TRUE);
	//AfxMessageBox(m_controlpage.m_sTestFilePath);

	Header h;					// Set up Header object to return data
	// and fill in data values
	h.testfilepath = m_controlpage.m_ScriptFilePath;
	h.changed = TRUE;

	//Send message back to main dialog window with pointer to return data
	GetParent()->SendMessage(WM_USER_APPLY, 0, (LPARAM) &h);
	//Restore non-modified status
	m_controlpage.SetModified(FALSE);
	m_filespage.SetModified(FALSE);
}
