#if !defined(AFX_ASYNSVSK_H__53E2F283_8BA2_11D1_9507_440100C10000__INCLUDED_)
#define AFX_ASYNSVSK_H__53E2F283_8BA2_11D1_9507_440100C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AsynSvSk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAsynSvSk command target

class CAsynSvSk : public CAsyncSocket
{
// Attributes
public:
	CDialog* m_pDlg; //backpointers
// Operations
public:
	CAsynSvSk(CDialog* pDlg, int index);
	virtual ~CAsynSvSk();

	CString		m_sendBuffer;   //for async send
	int			m_nBytesSent;
	int			m_nBytesBufferSize;
	
	void		DoAsyncSendBuff();

// Overrides
public:
	int m_nline;
	int m_nClient;
	TCHAR * m_strReturn;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsynSvSk)
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
	//{{AFX_MSG(CAsynSvSk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNSVSK_H__53E2F283_8BA2_11D1_9507_440100C10000__INCLUDED_)
