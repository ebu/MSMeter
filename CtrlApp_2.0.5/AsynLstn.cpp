// Asynlstn.cpp : implementation file
// Asynchronous listen for clients

#include "stdafx.h"
#include "ctrlapp.h"
#include "CtrlAppDlg.h"

#include "AsynLstn.h"
#include "AsynSvSk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAsynlstn

CAsynlstn::CAsynlstn(CDialog* pDlg)
{
	m_pDlg = pDlg;
}

CAsynlstn::~CAsynlstn()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CAsynlstn, CAsyncSocket)
	//{{AFX_MSG_MAP(CAsynlstn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CAsynlstn member functions

void CAsynlstn::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int t;
	SOCKADDR_IN address;
	int s;
	s = sizeof(SOCKADDR_IN);

	if ((t = ((CControlDlg*)m_pDlg)->CreateClient()) >= 0) {
			Accept(*((CControlDlg*)m_pDlg)->m_pTestClient[t]->m_pSrvrSock, (SOCKADDR *)(&address), &s);

			//CString str;
			//str.Format("Family = %d\nPort = %d\nAddress = %d.%d.%d.%d", address.sin_family, address.sin_port, address.sin_addr.S_un.S_un_w.s_w1 & 0xff, (address.sin_addr.S_un.S_un_w.s_w1 & 0xff00) >> 8, address.sin_addr.S_un.S_un_w.s_w2 & 0xff, (address.sin_addr.S_un.S_un_w.s_w2 & 0xff00) >> 8);
			//AfxMessageBox(str);
	}
	else {
		CAsynSvSk tempSvSk(NULL, 0);
		Accept(tempSvSk);
		tempSvSk.Close();
		return;
	}

	//CString str;
	//str.Format("Client size = %d bytes",sizeof(CTestClient));
	//AfxMessageBox((LPCTSTR)str);

	CAsyncSocket::OnAccept(nErrorCode);
}

void CAsynlstn::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CControlDlg*)m_pDlg)->m_pLstnSock = NULL;		
	CAsyncSocket::OnClose(nErrorCode);
}

void CAsynlstn::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsyncSocket::OnConnect(nErrorCode);	

}

void CAsynlstn::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CAsynlstn::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsyncSocket::OnReceive(nErrorCode);
}

void CAsynlstn::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsyncSocket::OnSend(nErrorCode);
}

int CAsynlstn::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CAsynlstn::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}
