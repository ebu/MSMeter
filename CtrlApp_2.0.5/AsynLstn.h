#if !defined(AFX_ASYNLSTN_H__3FA2EEC5_8B7F_11D1_A121_0000F875C45F__INCLUDED_)
#define AFX_ASYNLSTN_H__3FA2EEC5_8B7F_11D1_A121_0000F875C45F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Asynlstn.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CAsynlstn command target

class CAsynlstn : public CAsyncSocket
{
// Attributes
public:
	CDialog*	m_pDlg; //backpointers
// Operations
public:
	CAsynlstn(CDialog* pDlg);
	virtual ~CAsynlstn();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsynlstn)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CAsynlstn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNLSTN_H__3FA2EEC5_8B7F_11D1_A121_0000F875C45F__INCLUDED_)
