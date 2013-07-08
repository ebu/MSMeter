// AsynSvSk.cpp : implementation file
// Asynchronous client control
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
// CAsynSvSk

CAsynSvSk::CAsynSvSk(CDialog* pDlg, int index)
{
	m_pDlg = pDlg;
	m_sendBuffer = "";   //for async send
	m_nBytesSent = 0;
	m_nBytesBufferSize = 0;
	m_strReturn = new TCHAR[1024];
	m_nClient = index;
	m_nline = 0;
}

CAsynSvSk::~CAsynSvSk()
{
	//if (m_strReturn) 
	delete m_strReturn;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CAsynSvSk, CAsyncSocket)
	//{{AFX_MSG_MAP(CAsynSvSk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CAsynSvSk member functions

void CAsynSvSk::OnAccept(int nErrorCode) 
{
	CAsyncSocket::OnAccept(nErrorCode);
}

void CAsynSvSk::OnClose(int nErrorCode) 
{
	((CControlDlg*)m_pDlg)->m_pTestClient[m_nClient]->m_pSrvrSock = NULL;
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}

void CAsynSvSk::OnConnect(int nErrorCode) 
{
	CAsyncSocket::OnConnect(nErrorCode);
}

void CAsynSvSk::OnOutOfBandData(int nErrorCode) 
{
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CAsynSvSk::OnReceive(int nErrorCode) 
{
	//static int i = 0;
	int j, nRead;
	TCHAR buff[256], t;

	nRead = Receive(buff, 256); 

	CAsyncSocket::OnReceive(nErrorCode);
	
	switch (nRead)
	{
	case 0:
		Close();
		break;

	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) 
		{
			AfxMessageBox ("Error occurred");
			Close();
		}
		break;

	default:
		// In here I need to re-format the string so that it only returns with whole lines.
		//buff[nRead] = 0; //terminate the string - actually not necessary because it is that way anyway.
		for (j = 0 ; j < nRead ; ) {		
			// The string returned in buff has a '0' at the end. 
			// This has to be ignored for part strings.
			if ((t = buff[j++]) == 0) continue;		// Ignore null characters

			m_strReturn[m_nline++] = t;					// Copy other characters
			if (t == '\n') {						// Deal with end of lines properly
				m_strReturn[m_nline - 1] = 0;
				//((CControlDlg*)m_pDlg)->m_strRecv = m_strReturn;
				((CControlDlg*)m_pDlg)->ProcessPendingRead(m_nClient);		// Send whole lines to the processing function
				//m_strReturn[m_nline - 1] = '\n';
				//m_strReturn[m_nline] = 0;
				//((CControlDlg*)m_pDlg)->m_pTestClient[m_nClient]->outfile.Write(m_strReturn, strlen(m_strReturn));
				m_nline = 0;
			}
		}
	}
}

void CAsynSvSk::OnSend(int nErrorCode) 
{
	DoAsyncSendBuff();	
	CAsyncSocket::OnSend(nErrorCode);
}

int CAsynSvSk::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CAsynSvSk::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}

void CAsynSvSk::DoAsyncSendBuff()
{
	while (m_nBytesSent < m_nBytesBufferSize)  
	{
		int dwBytes;

		if ((dwBytes = Send((LPCTSTR)m_sendBuffer + m_nBytesSent, m_nBytesBufferSize - m_nBytesSent))
			== SOCKET_ERROR)
		{
			if (GetLastError() == WSAEWOULDBLOCK) break;
			else
			{
				TCHAR szError[256];
				wsprintf(szError, "Server Socket failed to send: %d", GetLastError());
				Close();
				((CControlDlg*)m_pDlg)->m_pTestClient[m_nClient]->m_pSrvrSock = NULL;
				AfxMessageBox (szError);
			}

		}
		else
		{
			m_nBytesSent += dwBytes;
		}
	}

	if (m_nBytesSent == m_nBytesBufferSize)
	{
		m_nBytesSent = m_nBytesBufferSize = 0;
		m_sendBuffer = "";
	}
}
