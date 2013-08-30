// TestClient.cpp : implementation file
//

#include "stdafx.h"
#include "CtrlApp.h"
//#include "ctrlapp.h"
#include "TestClient.h"
#include "CtrlAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestClient

CTestClient::CTestClient(CDialog* pDlg, int index)
{
	m_pTDlg = pDlg;

	if ((m_pSrvrSock = new CAsynSvSk(pDlg, index)) == NULL)
	{
		AfxMessageBox("Failed to allocate server socket");
		return;
	}

	m_index = index;
	first_time = 1;
	m_bRunning = FALSE;
	m_header.changed = TRUE;		// Set this up first time to make sure initial values are sent
	m_bCPflag = FALSE;

	//CString str;
	//str.Format("outfile%d", m_index);
	//outfile.Open(str, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyWrite);
	//AfxMessageBox((LPCTSTR) str);

	//m_index = ((CControlDlg*)pDlg)->m_nClients++;
	//((CControlDlg*)pDlg)->m_nClients++;
}

CTestClient::~CTestClient()
{
	// AfxMessageBox("Test Client destructor");
	if (m_pSrvrSock) {
		m_pSrvrSock->Close();
		delete m_pSrvrSock;
		m_pSrvrSock = NULL;
	}

	//if(outfile){
	//close output file
	//outfile.Close();
	//}
}


BEGIN_MESSAGE_MAP(CTestClient, CWnd)
	//{{AFX_MSG_MAP(CTestClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTestClient message handlers


int CTestClient::Send(CString str)
{
	if (m_pSrvrSock) {
		m_pSrvrSock->m_sendBuffer = str;
		m_pSrvrSock->m_nBytesSent = 0;
		m_pSrvrSock->m_nBytesBufferSize = str.GetLength() + 1;
		m_pSrvrSock->DoAsyncSendBuff();
		return 1;
	}
	else return 0;

}

BOOL CTestClient::GetName()
{
	unsigned int port;

	if(!(m_pSrvrSock->GetPeerName(m_strSockName, port))){
		CString str;
		str.Format("Failed to get name\nError number = %d", m_pSrvrSock->GetLastError());
		AfxMessageBox((LPCTSTR)str);
		return 0;
	}
	return 1;
}

// Create header and send to client as -p ... command
BOOL CTestClient::SendHeader(BOOL flag, DWORD size)
{
	if (m_header.changed){
		CString str, str2, str3, str4, str5, str6;

		if(flag) {
			//AfxMessageBox("Create flag set");
			if (m_header.testfilepath.GetLength() > 0) {
			str = "-p ";

			str += m_header.testfilepath;
			}

			str2.Format(" -n %d", m_header.maxnum);
			str += str2;

			str2.Format(" -c %d", size);
			str += str2;
		}
		else {

			if (m_header.testfilepath.GetLength() > 0) {
				str = "-p ";
				str += m_header.testfilepath;
			}

			str2.Format(" -n %d", m_header.maxnum);
			str += str2;

			str2.Format(" -l %d", m_header.maxlength);
			str += str2;

			str2.Format(" -d %d", m_header.maxdelay);
			str += str2;

			str2.Format(" -w %d", m_header.pcwrite);
			str += str2;

			str2.Format(" -r %d", m_header.pcrand);
			str += str2;

			str += " -v";

			if (!m_header.verbose) {
				str += "-";
			}
			str += " -f";

			if (!m_header.sharing) {
				str += "-";
			}

			// Set directio flag
			if (!m_header.directio) {
				str += " -dio 0";
			}
			else str += " -dio 1";

			// Set number of block sizes and types
			int typecount = 0;
			str3.Format("-bs");
			for(int i = 1 ; i < MAXBLOCKTYPE ; i++) {
				if (m_header.lstblocksize[i]) {
					typecount++;
					str4.Format(" %d", i);
					str3 += str4;
				}
			}
			// Append 0 for remaing types
			for (int i = 1 ; i < (MAXBLOCKTYPE - typecount) ; i++) {
				str4.Format(" 0");
				str3 += str4;
			}
		
			str2.Format(" -bn %d %s", typecount, str3);
			str += str2;


			//MSMeter 3 additions

			// set ntp flag if needed
			if (m_header.ntp_stamps){
				str5.Format(" -ntp %s", m_header.ntp_address);
				str += str5;
			}

			//set per frame flags if needed
			if (m_header.per_frame){
				str6.Format(" -pf %d", m_header.frame_size);
				str += str6;
			}
		}

		//AfxMessageBox((LPCTSTR)str);
		Send(str);

		m_header.changed = FALSE;
		return TRUE;
	}
	return FALSE;
}

// Create version of header to be output to CEdit window
CString CTestClient::CreateHeaderStr(BOOL flag, DWORD size)
{
	CString str, str2, str3, str4, str5, str6;

	if(flag) {
		//AfxMessageBox("Create flag set");
		if (m_header.testfilepath.GetLength() > 0) {
		str = "-p ";
		str += m_header.testfilepath;
		}

		str2.Format(" -n %d", m_header.maxnum);
		str += str2;

		str2.Format(" -c %d", size);
		str += str2;
	}
	else {

		if (m_header.testfilepath.GetLength() > 0) {
			str = "-p ";
			str += m_header.testfilepath;
			}

		str2.Format(" -n %d", m_header.maxnum);
		str += str2;

		str2.Format(" -l %d", m_header.maxlength);
		str += str2;

		str2.Format(" -d %d", m_header.maxdelay);
		str += str2;

		str2.Format(" -w %d", m_header.pcwrite);
		str += str2;

		str2.Format(" -r %d", m_header.pcrand);
		str += str2;

		str += " -v";

		if (!m_header.verbose) {
			str += "-";
		}
		str += " -f";

		if (!m_header.sharing) {
			str += "-";
		}

		// Set directio flag
		if (!m_header.directio) {
			str += " -dio 0";
		}
		else str += " -dio 1";

		// Set number of block sizes and types
		int typecount = 0;
		str3.Format("-bs");
		for(int i = 1 ; i < MAXBLOCKTYPE ; i++) {
			if (m_header.lstblocksize[i]) {
				typecount++;
				str4.Format(" %d", i);
				str3 += str4;
			}
		}
		// Append 0 for remaing types
		for (int i = 1 ; i < (MAXBLOCKTYPE - typecount) ; i++) {
			str4.Format(" 0");
			str3 += str4;
		}
		
		str2.Format(" -bn %d %s", typecount, str3);
		str += str2;

		//MSMeter 3 additions

		// set ntp flag if needed
		if (m_header.ntp_stamps){
			str5.Format(" -ntp %s", m_header.ntp_address);
			str += str5;
		}

		//set per frame flags if needed
		if (m_header.per_frame){
			str6.Format(" -pf %d", m_header.frame_size);
			str += str6;
		}

	}
	return str;
}


